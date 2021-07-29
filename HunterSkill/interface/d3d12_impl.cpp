#include "../thirdparty/kiero/kiero.h"
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
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

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
			tick_wait = tick_now + 1000;

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

			if ( DirectX12Interface::Device->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, nullptr, IID_PPV_ARGS( &DirectX12Interface::CommandList ) ) != S_OK ||
				DirectX12Interface::CommandList->Close( ) != S_OK )
				return oPresentD3D12( p_swap_chain, sync_interval, flags );

			D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers;
			DescriptorBackBuffers.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			DescriptorBackBuffers.NumDescriptors	= DirectX12Interface::BuffersCounts;
			DescriptorBackBuffers.Flags				= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DescriptorBackBuffers.NodeMask			= 1;

			if ( DirectX12Interface::Device->CreateDescriptorHeap( &DescriptorBackBuffers, IID_PPV_ARGS( &DirectX12Interface::DescriptorHeapBackBuffers ) ) != S_OK )
				return oPresentD3D12( p_swap_chain, sync_interval, flags );

			const auto RTVDescriptorSize			= DirectX12Interface::Device->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );
			auto RTVHandle							= DirectX12Interface::DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart( );

			for ( size_t i = 0; i < DirectX12Interface::BuffersCounts; i++ ) 
			{
				ID3D12Resource* pBackBuffer = nullptr;
				DirectX12Interface::FrameContext[ i ].DescriptorHandle = RTVHandle;
				p_swap_chain->GetBuffer( i, IID_PPV_ARGS( &pBackBuffer ) );
				DirectX12Interface::Device->CreateRenderTargetView( pBackBuffer, nullptr, RTVHandle );
				DirectX12Interface::FrameContext[ i ].Resource = pBackBuffer;
				RTVHandle.ptr += RTVDescriptorSize;
			}
			
			RECT rct{ };
			GetWindowRect( Desc.OutputWindow, &rct );
			impl::screen( )[ 0 ] = (float)( rct.right	- rct.left );
			impl::screen( )[ 01 ] = (float)( rct.bottom	- rct.top );

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


	if ( p_overlay )
		p_overlay( );

	ImGui::EndFrame( );

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

// Simple helper function to load an image into a DX12 texture with common settings
// Returns true on success, with the SRV CPU handle having an SRV for the newly-created texture placed in it (srv_cpu_handle must be a handle in a valid descriptor heap)
bool impl::d3d12::LoadTextureFromFile(const char* filename, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture resource
	D3D12_HEAP_PROPERTIES props;
	memset(&props, 0, sizeof(D3D12_HEAP_PROPERTIES));
	props.Type = D3D12_HEAP_TYPE_DEFAULT;
	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = image_width;
	desc.Height = image_height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* pTexture = NULL;
	d3d_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&pTexture));

	// Create a temporary upload resource to move the data in
	UINT uploadPitch = (image_width * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
	UINT uploadSize = image_height * uploadPitch;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = uploadSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	props.Type = D3D12_HEAP_TYPE_UPLOAD;
	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	ID3D12Resource* uploadBuffer = NULL;
	HRESULT hr = d3d_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&uploadBuffer));
	IM_ASSERT(SUCCEEDED(hr));

	// Write pixels into the upload resource
	void* mapped = NULL;
	D3D12_RANGE range = { 0, uploadSize };
	hr = uploadBuffer->Map(0, &range, &mapped);
	IM_ASSERT(SUCCEEDED(hr));
	for (int y = 0; y < image_height; y++)
		memcpy((void*)((uintptr_t)mapped + y * uploadPitch), image_data + y * image_width * 4, image_width * 4);
	uploadBuffer->Unmap(0, &range);

	// Copy the upload resource content into the real resource
	D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
	srcLocation.pResource = uploadBuffer;
	srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	srcLocation.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srcLocation.PlacedFootprint.Footprint.Width = image_width;
	srcLocation.PlacedFootprint.Footprint.Height = image_height;
	srcLocation.PlacedFootprint.Footprint.Depth = 1;
	srcLocation.PlacedFootprint.Footprint.RowPitch = uploadPitch;

	D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
	dstLocation.pResource = pTexture;
	dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dstLocation.SubresourceIndex = 0;

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = pTexture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	// Create a temporary command queue to do the copy with
	ID3D12Fence* fence = NULL;
	hr = d3d_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	IM_ASSERT(SUCCEEDED(hr));

	HANDLE event = CreateEvent(0, 0, 0, 0);
	IM_ASSERT(event != NULL);

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 1;

	ID3D12CommandQueue* cmdQueue = NULL;
	hr = d3d_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue));
	IM_ASSERT(SUCCEEDED(hr));

	ID3D12CommandAllocator* cmdAlloc = NULL;
	hr = d3d_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));
	IM_ASSERT(SUCCEEDED(hr));

	ID3D12GraphicsCommandList* cmdList = NULL;
	hr = d3d_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc, NULL, IID_PPV_ARGS(&cmdList));
	IM_ASSERT(SUCCEEDED(hr));

	cmdList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, NULL);
	cmdList->ResourceBarrier(1, &barrier);

	hr = cmdList->Close();
	IM_ASSERT(SUCCEEDED(hr));

	// Execute the copy
	cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&cmdList);
	hr = cmdQueue->Signal(fence, 1);
	IM_ASSERT(SUCCEEDED(hr));

	// Wait for everything to complete
	fence->SetEventOnCompletion(1, event);
	WaitForSingleObject(event, INFINITE);

	// Tear down our temporary command queue and release the upload resource
	cmdList->Release();
	cmdAlloc->Release();
	cmdQueue->Release();
	CloseHandle(event);
	fence->Release();
	uploadBuffer->Release();

	// Create a shader resource view for the texture
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	d3d_device->CreateShaderResourceView(pTexture, &srvDesc, srv_cpu_handle);

	// Return results
	*out_tex_resource = pTexture;
	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}