
#include "header.h"
#include "interface/d3d11_impl.h"
#include "interface/d3d12_impl.h"
#include "interface/win32_impl.h"

#include "game/shared_config.h"
#include "game/game_manager.h"
#include "utils/mem.h"
#include <sstream>
#include "thirdparty/imgui/imgui.h"
#include "game/hooks.h"

void __stdcall overgay( )
{
    if ( *reinterpret_cast<uint32_t*>( 0x145073B3C ) != 0xFFFFFFFF )
        return;
    game::manager::i( )->entity_callback(  
    	[ ] ( game::c_entity* entity ) -> void
    	{
    		
    		if ( !entity->is_boss() )
    			return;
    
    		auto pos = entity->get_pos( );

    		vec3 out;

    		if ( !game::manager::i( )->w2s( pos, out ) )
    			return;

    		std::ostringstream ss;
    		ss << "BOSS 0x" << std::hex << std::uppercase << entity << ", POS[ X: " << std::dec << pos.x << ", Y: " << pos.y << ", Z: " << pos.z << " ]";

    		//printf( "%s\n", ss.str().c_str( ) );
    		ImGui::GetOverlayDrawList( )->AddText( ImVec2( out.x, out.y ),
    			ImGui::GetColorU32( ImVec4( 1.f, 0.f, 0.f, 1.f ) ), ss.str( ).c_str( ) );

    	} );

}

void open_console( )
{
    FILE* stream = nullptr;
    AllocConsole( );
    freopen_s( &stream, "CONIN$", "r", stdin );
    freopen_s( &stream, "CONOUT$", "w", stdout );
    freopen_s( &stream, "CONOUT$", "w", stderr );
    SetConsoleTitleA( "" );
}


int main( )
{
    
    open_console( );

    options::config( );

    //impl::d3d11::init( );

    hooks::init( );

    impl::d3d12::init( );

    impl::d3d12::set_overlay( overgay );

    while ( true )
    {
        Sleep( 500 );
        if ( *reinterpret_cast<uint32_t*>( 0x145073B3C ) != 0xFFFFFFFF )
            continue;
        game::manager::i( )->update_entities( );
   
    }

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread( nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>( main ), nullptr, 0, nullptr );

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

