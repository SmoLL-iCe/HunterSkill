﻿#include "../thirdparty/kiero/kiero.h"
#include "d3d12_impl.h"
#include <dxgi.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#pragma comment(lib, "d3d12.lib")
#include <assert.h>
#include "win32_impl.h"
#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui/imgui_impl_win32.h"
#include "../thirdparty/imgui/imgui_impl_dx12.h"
#include "../utils/mem.h"
#include "steam.h"
#include "../utils/ntos.h"

extern t_render_overlay p_overlay;

void impl::d3d12::set_overlay( t_render_overlay p )
{
	p_overlay = p;
}

using PresentD3D12 =  long( __fastcall* ) ( IDXGISwapChain*, UINT, UINT );
PresentD3D12 oPresentD3D12 = nullptr;

void *oExecuteCommandLists = nullptr;
HRESULT( *oSignalD3D12 )( ID3D12CommandQueue*, ID3D12Fence*, UINT64 );


namespace DirectX12Interface 
{
	ID3D12Device*					Device						= nullptr;
	ID3D12DescriptorHeap*			DescriptorHeapBackBuffers	= nullptr;
	ID3D12DescriptorHeap*			DescriptorHeapImGuiRender	= nullptr;
	ID3D12GraphicsCommandList*		CommandList					= nullptr;
	ID3D12CommandQueue*				CommandQueue				= nullptr;

	struct _FrameContext 
	{
		ID3D12CommandAllocator*		CommandAllocator			= nullptr;
		ID3D12Resource*				Resource					= nullptr;
		D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle			= { };
	};

	uintptr_t BuffersCounts = -1;
	_FrameContext* FrameContext;
}


long __fastcall hkPresentDX12( IDXGISwapChain3* p_swap_chain, UINT sync_interval, UINT flags )
{
	//auto* wind = FindWindow( L"MT FRAMEWORK", nullptr );

	//if ( !wind )
	//	return oPresentD3D12( p_swap_chain, sync_interval, flags );

	static bool init = false;

	if ( !init )
	{
		static uint64_t tick_wait = 0;

		auto tick_now = GetTickCount64( );

		if ( !tick_wait )
			tick_wait = tick_now + 10000;

		if (  tick_wait >= tick_now )
			return oPresentD3D12( p_swap_chain, sync_interval, flags );

		if ( SUCCEEDED( p_swap_chain->GetDevice( __uuidof( ID3D12Device ), r_cast<void**>( &DirectX12Interface::Device ) ) ) )
		{
			ImGui::CreateContext( );

			ImGuiIO& io = ImGui::GetIO( ); (void)io;

			ImGui::GetIO( ).WantCaptureMouse || ImGui::GetIO( ).WantTextInput || ImGui::GetIO( ).WantCaptureKeyboard;

			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			DXGI_SWAP_CHAIN_DESC Desc;
			p_swap_chain->GetDesc( &Desc );
			Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			//Desc.OutputWindow = wind;
			//Desc.Windowed = ( ( GetWindowLongPtr( wind, GWL_STYLE ) & WS_POPUP ) != 0 ) ? false : true;

			DirectX12Interface::BuffersCounts		= Desc.BufferCount;
			DirectX12Interface::FrameContext		= new DirectX12Interface::_FrameContext[ DirectX12Interface::BuffersCounts ];

			D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender = { };
			DescriptorImGuiRender.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			DescriptorImGuiRender.NumDescriptors	= DirectX12Interface::BuffersCounts;
			DescriptorImGuiRender.Flags				= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

			if ( DirectX12Interface::Device->CreateDescriptorHeap( &DescriptorImGuiRender, IID_PPV_ARGS( &DirectX12Interface::DescriptorHeapImGuiRender ) ) != S_OK )
				return oPresentD3D12( p_swap_chain, sync_interval, flags );

			ID3D12CommandAllocator* Allocator;
			if ( DirectX12Interface::Device->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( &Allocator ) ) != S_OK )
				return oPresentD3D12( p_swap_chain, sync_interval, flags );

			for ( size_t i = 0; i < DirectX12Interface::BuffersCounts; i++ ) 
			{
				DirectX12Interface::FrameContext[ i ].CommandAllocator = Allocator;
			}

			if ( DirectX12Interface::Device->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS( &DirectX12Interface::CommandList ) ) != S_OK ||
				DirectX12Interface::CommandList->Close( ) != S_OK )
				return oPresentD3D12( p_swap_chain, sync_interval, flags );

			D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers;
			DescriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			DescriptorBackBuffers.NumDescriptors = DirectX12Interface::BuffersCounts;
			DescriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DescriptorBackBuffers.NodeMask = 1;

			//printf( "p4\n" );
			if ( DirectX12Interface::Device->CreateDescriptorHeap( &DescriptorBackBuffers, IID_PPV_ARGS( &DirectX12Interface::DescriptorHeapBackBuffers ) ) != S_OK )
				return oPresentD3D12( p_swap_chain, sync_interval, flags );

			const auto RTVDescriptorSize = DirectX12Interface::Device->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );
			D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = DirectX12Interface::DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart( );

			for ( size_t i = 0; i < DirectX12Interface::BuffersCounts; i++ ) 
			{
				ID3D12Resource* pBackBuffer = nullptr;
				DirectX12Interface::FrameContext[ i ].DescriptorHandle = RTVHandle;
				p_swap_chain->GetBuffer( i, IID_PPV_ARGS( &pBackBuffer ) );
				DirectX12Interface::Device->CreateRenderTargetView( pBackBuffer, nullptr, RTVHandle );
				DirectX12Interface::FrameContext[ i ].Resource = pBackBuffer;
				RTVHandle.ptr += RTVDescriptorSize;
			}
			
			

			ImGui_ImplWin32_Init( Desc.OutputWindow );
			ImGui_ImplDX12_Init( DirectX12Interface::Device, DirectX12Interface::BuffersCounts, DXGI_FORMAT_R8G8B8A8_UNORM, DirectX12Interface::DescriptorHeapImGuiRender, DirectX12Interface::DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart( ), DirectX12Interface::DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart( ) );
			ImGui_ImplDX12_CreateDeviceObjects( );

			impl::win32::init( Desc.OutputWindow );
			init = true;
		}
	}

	if ( DirectX12Interface::CommandQueue == nullptr )
		return oPresentD3D12( p_swap_chain, sync_interval, flags );

	ImGui_ImplDX12_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	impl::show_menu( );

	ImGui::EndFrame( );

	if ( p_overlay )
		p_overlay( );

	DirectX12Interface::_FrameContext& CurrentFrameContext = DirectX12Interface::FrameContext[ p_swap_chain->GetCurrentBackBufferIndex( ) ];
	CurrentFrameContext.CommandAllocator->Reset( );

	D3D12_RESOURCE_BARRIER Barrier;
	Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	Barrier.Transition.pResource = CurrentFrameContext.Resource;
	Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	DirectX12Interface::CommandList->Reset( CurrentFrameContext.CommandAllocator, nullptr );
	DirectX12Interface::CommandList->ResourceBarrier( 1, &Barrier );
	DirectX12Interface::CommandList->OMSetRenderTargets( 1, &CurrentFrameContext.DescriptorHandle, FALSE, nullptr );
	DirectX12Interface::CommandList->SetDescriptorHeaps( 1, &DirectX12Interface::DescriptorHeapImGuiRender );

	ImGui::Render( );
	ImGui_ImplDX12_RenderDrawData( ImGui::GetDrawData( ), DirectX12Interface::CommandList );
	Barrier.Transition.StateBefore	= D3D12_RESOURCE_STATE_RENDER_TARGET;
	Barrier.Transition.StateAfter	= D3D12_RESOURCE_STATE_PRESENT;
	DirectX12Interface::CommandList->ResourceBarrier( 1, &Barrier );
	DirectX12Interface::CommandList->Close( );
	DirectX12Interface::CommandQueue->ExecuteCommandLists( 1, reinterpret_cast<ID3D12CommandList* const*>( &DirectX12Interface::CommandList ) );

	return oPresentD3D12( p_swap_chain, sync_interval, flags );
}


void hkExecuteCommandLists( ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists )
{
	if ( !DirectX12Interface::CommandQueue )
		DirectX12Interface::CommandQueue = queue;

	r_cast<decltype( hkExecuteCommandLists )*>( oExecuteCommandLists )( queue, NumCommandLists, ppCommandLists );
}

void* oResizeBuffers = nullptr;

HRESULT hkResizeBuffers( IDXGISwapChain3* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags )
{
	impl::screen( )[ 0 ] = Width;
	impl::screen( )[ 1 ] = Height;

	return r_cast<decltype( hkResizeBuffers )*>( oResizeBuffers )( pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags );
}


void impl::d3d12::init( )
{
	auto ptr = steam_overlay::get_ptr( );

	oPresentD3D12 = reinterpret_cast<decltype( oPresentD3D12 )>( *ptr );

	auto status = kiero::init( kiero::RenderType::D3D12 );

	if ( status == kiero::Status::Success )
	{
		kiero::bind( 54, &oExecuteCommandLists, hkExecuteCommandLists );
		kiero::bind( 145, &oResizeBuffers, hkResizeBuffers );

		*ptr = r_cast<uintptr_t>( hkPresentDX12 );
	}

}
