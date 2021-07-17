#include "../thirdparty/kiero/kiero.h"
#include "d3d11_impl.h"
#include <d3d11.h>
#include <assert.h>
#include "win32_impl.h"
#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui/imgui_impl_win32.h"
#include "../thirdparty/imgui/imgui_impl_dx11.h"
#include "../utils/mem.h"
#include "steam.h"

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

		impl::screen( )[ 0 ] = viewport.Width;
		impl::screen( )[ 1 ] = viewport.Height;

		init = true;
	}

	ImGui_ImplDX11_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	impl::show_menu( );

	ImGui::EndFrame( );


	if ( p_overlay )
		p_overlay( );

	ImGui::Render( );
	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

	return oPresent( pSwapChain, SyncInterval, Flags );
}

void impl::d3d11::init( )
{

	auto ptr = steam_overlay::get_ptr( );

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
