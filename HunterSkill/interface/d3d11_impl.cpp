#include "../thirdparty/kiero/kiero.h"
#include "d3d11_impl.h"
#include <d3d11.h>
#include <assert.h>
#include "win32_impl.h"
#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui/imgui_impl_win32.h"
#include "../thirdparty/imgui/imgui_impl_dx11.h"
#include "../utils/mem.h"

typedef long( __stdcall* Present )( IDXGISwapChain*, UINT, UINT );
static Present oPresent = NULL;

t_render_overlay p_overlay = nullptr;

void impl::d3d11::set_overlay( t_render_overlay p )
{
	p_overlay = p;
}

D3D11_VIEWPORT viewport = { };
long __stdcall hkPresent11( IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags )
{
	static bool init = false;

	if ( !init )
	{
		DXGI_SWAP_CHAIN_DESC desc;
		pSwapChain->GetDesc( &desc );

		ID3D11Device* device;
		pSwapChain->GetDevice( __uuidof( ID3D11Device ), (void**)&device );

		ID3D11DeviceContext* context;
		device->GetImmediateContext( &context );

		impl::win32::init( desc.OutputWindow );

		ImGui::CreateContext( );
		ImGui_ImplWin32_Init( desc.OutputWindow );
		ImGui_ImplDX11_Init( device, context );


		UINT vps = 1;
		context->RSGetViewports( &vps, &viewport );

		init = true;
	}

	ImGui_ImplDX11_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	impl::showExampleWindow( "D3D11" );

	ImGui::EndFrame( );


	if ( p_overlay )
		p_overlay( );

	ImGui::Render( );
	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

	return oPresent( pSwapChain, SyncInterval, Flags );
}

float* impl::d3d11::screen( )
{
	return reinterpret_cast<float*>( &viewport.Width );
}

/*
gameoverlayrenderer64.dll+88E7E - 48 FF 25 93571000     - jmp qword ptr [gameoverlayrenderer64.dll+18E618]
gameoverlayrenderer64.dll+88E85 - 48 89 5C 24 30        - mov [rsp+30],rbx
gameoverlayrenderer64.dll+88E8A - 48 8D 0D AF4F1000     - lea rcx,[gameoverlayrenderer64.dll+18DE40]
*/
uint8_t shell_code[ ] = {
	//  gameoverlayrenderer64.dll+88E7E =>	jmp qword ptr [gameoverlayrenderer64.dll+18E618]
	0x48, 0xFF, 0x25, 0xCC, 0xCC, 0xCC, 0xCC,
	//  gameoverlayrenderer64.dll+88E85 =>	mov [rsp+30],rbx
	0x48, 0x89, 0x5C, 0x24, 0x30,
	//  gameoverlayrenderer64.dll+88E8A =>	lea rcx,[gameoverlayrenderer64.dll+18DE40]
	0x48, 0x8D, 0x0D
};

void impl::d3d11::init( )
{

	HMODULE gameoverlayrenderer64 = nullptr;
	while ( !gameoverlayrenderer64 )
	{
		gameoverlayrenderer64 = GetModuleHandle( L"gameoverlayrenderer64.dll" );
	}

	auto found = mem::find_pattern( shell_code, sizeof shell_code, gameoverlayrenderer64 );
	
	if ( !found ) return;

	auto ptr = reinterpret_cast<uintptr_t*>( found + *r_cast<uint32_t*>( &found[ 3 ] ) + 7 );

	while ( !*ptr )
	{
		Sleep( 100 );
	}
	oPresent = reinterpret_cast<decltype( oPresent )>( *ptr );
	*ptr = uintptr_t( hkPresent11 );


	//auto status = kiero::init( kiero::RenderType::D3D11 );
	//printf( "status : %d\n", status );
	//if ( status == kiero::Status::Success )
	//{
	//	printf( "successs\n" );
	//	printf( "present %p\n", kiero::getMethodsTable( )[ 8 ] );

	//	assert( kiero::bind( 8, (void**)&oPresent, hkPresent11 ) == kiero::Status::Success );
	//}

}
