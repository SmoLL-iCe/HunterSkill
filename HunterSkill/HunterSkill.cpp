
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


void drawn_bar(float hp, float max_hp, int size_, ImVec2 Pos)
{
    // BASE DRAWN
    ImGui::GetOverlayDrawList()->AddTriangleFilled(Pos, { Pos.x - 10, Pos.y + 5 }, { Pos.x, Pos.y + 10 }, ImGui::GetColorU32({ 0.219f, 0.223f, 0.219f,1.f }));
    ImGui::GetOverlayDrawList()->AddTriangleFilled({ Pos.x + size_, Pos.y }, { Pos.x + size_, Pos.y + 10 }, { Pos.x + size_ + 10, Pos.y + 5 }, ImGui::GetColorU32({ 0.219f, 0.223f, 0.219f,1.f }));
    ImGui::GetOverlayDrawList()->AddRectFilled({ Pos.x, Pos.y - 1 }, { Pos.x + size_, Pos.y + 11 }, ImGui::GetColorU32({ 0.219f, 0.223f, 0.219f,1.f }));

    if (hp <= 0)
        return;

    auto percent = (hp / max_hp) * 100.f;

    if (hp <= 0.f)
        return;
    //ANIMATION BAR
    if (percent)
    {
        auto x = size_ * (percent / 100.f);
        //printf("%f\n", x);
        ImGui::GetOverlayDrawList()->AddTriangleFilled(Pos, { Pos.x - 10, Pos.y + 5 }, { Pos.x, Pos.y + 10 }, ImGui::GetColorU32({ 0.203f, 0.415f, 0.223f,1.0f }));
        ImGui::GetOverlayDrawList()->AddTriangleFilled({ Pos.x + x, Pos.y }, { Pos.x + x, Pos.y + 10 }, { Pos.x + x + 10, Pos.y + 5 }, ImGui::GetColorU32({ 0.203f, 0.415f, 0.223f,1.0f }));
        ImGui::GetOverlayDrawList()->AddRectFilled({ Pos.x, Pos.y - 1 }, { Pos.x + x, Pos.y + 11 }, ImGui::GetColorU32({ 0.203f, 0.415f, 0.223f,1.0f }));
    }
}


void drawn_background(ImVec2 Pos, float around, ImVec2 Size = { 250,130 })
{
    ImGui::GetOverlayDrawList()->AddRectFilled(Pos, { Pos.x + Size.x, Pos.y + Size.y }, ImGui::GetColorU32({0,0,0,0.5}), around);
}


void __stdcall overgay( )
{
    if ( *reinterpret_cast<uint32_t*>( 0x145073B3C ) != 0xFFFFFFFF )
        return;
    game::manager::i( )->entity_callback(  
    	[ ] ( game::s_entity entity ) -> void
    	{
    		
            auto cor = ImVec4( 1.f, 0.f, 0.f, 1.f );
            if (! entity.is_boss )
            {
                cor.x = 0.f;
                cor.y = 1.f;
                return;
            }

            if ( entity.health <= 0 || entity.max_health <= 0 )
                return;

    		vec3 out;

    		if ( !game::manager::i( )->w2s( entity.pos, out ) )
    			return;

            std::string hp_hud = " HP: " +  std::to_string((int)entity.health) + "/" + std::to_string( (int)entity.max_health);

            auto distance = game::manager::i( )->get_self_player( )->get_pos( ).distance( &entity.pos );
 
    		std::ostringstream ss;
    		ss  <<"ENTITY 0x" << std::hex << std::uppercase << entity.ptr << "\nDistance [ " << std::dec << distance << " ]\nFile : Main0001_01 ";

            drawn_background({ out.x, out.y }, 15 , {210, 90});

            drawn_bar(entity.health, entity.max_health, 180, { out.x + 15, out.y + 15 });

            ImGui::GetOverlayDrawList()->AddText(ImVec2(out.x + 25, out.y + 13),
                ImGui::GetColorU32({ 1,1,1,1 }), hp_hud.c_str());


            ImGui::GetOverlayDrawList( )->AddText( ImVec2( out.x + 10, out.y+30 ),
                ImGui::GetColorU32({1,1,1,1}), ss.str().c_str());
         

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

bool suporte_to_d3d12 = true;

int check_support()
{
    char name[255];
    GetModuleFileNameA(NULL, name, 255);
    std::string local = name;
    size_t pz_str = local.find("MonsterHunterWorld.exe");
    if (pz_str == std::string::npos)
        return 0x58; // Error module

      
    local.replace(local.begin() + pz_str, local.end(), "graphics_option.ini");

    std::fstream gcm(local.c_str());

    if (!gcm.is_open())
        return 0x57; // Error Read file config


    while (!gcm.eof())
    {
        std::string line;
        std::getline(gcm, line);
        pz_str = line.find("DirectX12Enable=");

        if (pz_str != std::string::npos)
        {
            std::cout << line << std::endl;
            return line.compare("DirectX12Enable=On");
        }
    }
    return -1; // dont find any suport
}



int main( )
{
    
    open_console( );

    Sleep( 1000 );

    options::config( );

    int err = check_support();
    std::cout << err << std::endl;
    suporte_to_d3d12 = !err;




    hooks::init( );

    //auto sound = r_cast<bool*>( 0x1450A4998 );
    //while ( !*sound )
    //{
    //    Sleep( 100 );
    //}

    if (suporte_to_d3d12)
    {
        std::cout << "Have compatibily with Dx12\n";
        impl::d3d12::init();
        impl::d3d12::set_overlay(overgay);
    }
    else
    {
        std::cout << "Dont find compatibily with Dx12\n";
        impl::d3d11::init();
        impl::d3d11::set_overlay(overgay);
    }
        


    while ( true )
    {
        Sleep( 100 );
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

