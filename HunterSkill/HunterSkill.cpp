
#include "header.h"
#include "interface/d3d11_impl.h"
#include "interface/d3d12_impl.h"
#include "interface/win32_impl.h"
#include "game/shared_config.h"
#include "game/game_manager.h"
#include "utils/mem.h"

#include "thirdparty/imgui/imgui.h"
#include "game/hooks.h"
#include "Monster_names.hpp"
#include "thirdparty/minhook/include/MinHook.h"
#include "interface/ImDraw.h"


void drawn_bar(float hp, float max_hp, int size_, ImVec2 Pos)
{
    // BASE DRAWN
    ImGui::GetOverlayDrawList()->AddTriangleFilled(Pos, { Pos.x - 15, Pos.y + 7.5f }, { Pos.x, Pos.y + 10 }, ImGui::GetColorU32({ 0.219f, 0.223f, 0.219f,1.f }));

    ImGui::GetOverlayDrawList()->AddTriangleFilled({ Pos.x + size_, Pos.y }, { Pos.x + size_, Pos.y + 15 }, { Pos.x + size_ + 10, Pos.y + 7.5f }, ImGui::GetColorU32({ 0.219f, 0.223f, 0.219f,1.f }));

    ImGui::GetOverlayDrawList()->AddRectFilled({ Pos.x, Pos.y - 1 }, { Pos.x + size_, Pos.y + 16 }, ImGui::GetColorU32({ 0.219f, 0.223f, 0.219f,1.f }));


    if (hp <= 0)
        return;

    auto percent = (hp / max_hp) * 100.f;

    if (hp <= 0.f)
        return;

    ImVec4 Color_ = { 0.203f, 0.415f, 0.223f,1.0f };
    if(percent >= 51 )
        Color_ = { 0.203f, 0.415f, 0.223f,1.0f };
    else if(percent >= 21)
        Color_ = { 0.75f, 0.75f, 0.078f,1.0f };
    else if (percent <= 20)
        Color_ = { 1.f, 0.015f, 0.053f,1.0f };

    //1, 0.019, 0.050

    //ANIMATION BAR
    if (percent)
    {
        auto x = size_ * (percent / 100.f);
        //printf("%f\n", x);
        ImGui::GetOverlayDrawList()->AddTriangleFilled(Pos, { Pos.x - 15, Pos.y + 7.5f }, { Pos.x, Pos.y + 15 }, ImGui::GetColorU32(Color_));
        ImGui::GetOverlayDrawList()->AddTriangleFilled({ Pos.x + x, Pos.y }, { Pos.x + x, Pos.y + 15 }, { Pos.x + x + 15, Pos.y + 7.5f }, ImGui::GetColorU32(Color_));
        ImGui::GetOverlayDrawList()->AddRectFilled({ Pos.x, Pos.y - 1 }, { Pos.x + x, Pos.y + 16 }, ImGui::GetColorU32(Color_));
    }
}

std::string getMonsterName( std::string file_monster )
{

    //std::string file_monster = entity.file;
    size_t erease = file_monster.find( "mod" );

    if ( erease != std::string::npos ) file_monster = file_monster.substr( erease + 4 );

    for ( const auto mh : monster_name )
    {
        size_t find = mh.find( file_monster );
        if ( find != std::string::npos )
        {
            file_monster = mh.substr( file_monster.length( )+1 );
            break;
        }
    }

    if (!file_monster.compare("em001_00 Rathian"))
        file_monster.clear();

    return file_monster;
}

void drawn_background(ImVec2 Pos, float around, ImVec2 Size = { 250,130 })
{
    ImGui::GetOverlayDrawList()->AddRectFilled(Pos, { Pos.x + Size.x, Pos.y + Size.y }, ImGui::GetColorU32({0,0,0,0.5}), around);
}


// Global vars to edit later
bool show_estatics = true, winconfig = true;
bool less, av, best, icon, saved;

//filter to menu (TEMP)
bool show_player = true, show_entry = false, show_hp_num = true, show_distance = false;
int distance_max = 10000;

bool closed = true;
bool finish_open = false;
float anim = 0;
bool hovered = false;
void __stdcall overgay( )
{
    s_loaded_images* imgs = impl::d3d12::load_imgs( );
    
    auto width = impl::screen( )[ 0 ];
    auto form_width = float( 400 );
    auto form_height = 130.f;
    float button_height = 18.f;
    float closed_height = button_height + 6.f;
    float inc_val = 3.f;


    if ( closed )
    {
        finish_open = false;
        if ( closed_height != anim )
        {
            if ( abs( closed_height - anim ) <= inc_val )
                anim = closed_height;

            if ( anim > closed_height )
                anim -= inc_val;
            else
                anim += inc_val;
        }

    }
    else
    {
        if ( form_height != anim )
        {
            if ( abs( form_height - anim ) <= inc_val )
            {
                anim = form_height;
                finish_open = true;
            }

            if ( anim > form_height )
                anim -= inc_val;
            else
                anim += inc_val;
        }
        else
            finish_open = true;

    }
    

    if ( show_estatics )
    {
        ImGui::PushStyleVar( ImGuiStyleVar_WindowMinSize, ImVec2( 15.f, 15.f ) );

        ImGui::SetNextWindowSize( { form_width, anim }, ImGuiCond_Always );

        ImGui::SetNextWindowPos( { float( width / 2.f ) - float( form_width / 2.f ), 0.f } );

        ImGui::Begin( "##ESTATICS", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse );                          // Create a window called "Hello, world!" and append into it.
      
        if ( finish_open )
        {
            ImGui::BeginChild( "#EST", { form_width - 16.f, ( form_height - 12.f ) - button_height } );
            if ( ImGui::TreeNode( "Options##MONSTER" ) )
            {
                //ImGui::Checkbox( "Auto Save", &saved ); // none now, myby button? for save one time?
                ImGui::Checkbox( "Show average damage", &av );
                ImGui::Checkbox( "Show less damage", &less );
                ImGui::TreePop( );
            }
            auto myplayer = uintptr_t( game::manager::i( )->get_self_player( ) );
            if ( myplayer )
            {
                for ( auto& monster : game::manager::i( )->hunting_damage( ) )
                {
                    if ( ImGui::TreeNode( getMonsterName( monster.name ).c_str( ) ) )
                    {

                        for ( auto& who_dam : monster.who_caused_damage )
                        {
                            int player_index = 0;
                            uintptr_t size_player = 0x13F40;
                            for ( ; player_index < 4; player_index++ )
                            {
                                if ( who_dam.entity == ( myplayer + ( size_player * player_index ) ) )
                                    break;

                            }
                            ++player_index;
                            float mid = 0;
                            ImGui::Image( r_cast<ImTextureID>( imgs[ who_dam.type ].ptr_handle_cpu_pos ), ImVec2( s_cast<float>( imgs[ who_dam.type ].width / 2 ), s_cast<float>( imgs[ who_dam.type ].height / 2 ) ) );

                            ImGui::SameLine( 55 );
                            

                            std::ostringstream show;

                            show << " Player" << player_index << "-Damage:" << (int)who_dam.total_damage;

                            show << ", Best:" << (int)who_dam.best_hit;

                            if ( av )
                            {
                                if ( who_dam.hit_count > 0 && who_dam.total_damage > 0 )
                                    mid = who_dam.total_damage / who_dam.hit_count;
                                show << ", AV:" << (int)mid;
                            }
                            if ( less )
                                show << ", LESS:" << (int)who_dam.low_hit;

                            ImGui::Text( show.str( ).c_str( ) );

                        }
                        ImGui::TreePop( );
                    }
                }
            }
            ImGui::EndChild( );
        
        }
        

        ImGui::SetCursorPos( { 0, ( anim - button_height ) - 3.f } );

        auto& style = ImGui::GetStyle( );
        auto backup1_y = style.ButtonTextAlign.y;
        auto backup2_y = style.FramePadding.y;
        style.FramePadding.y = -1.3f;
        style.ButtonTextAlign.y = 0.f;
        std::string dmg = "Damage Meter";
        if ( ImGui::Button( ((closed) ? dmg + " [SHOW]" : dmg + " [HIDE]" ).c_str( ), { float( form_width ), button_height } ) )
        {

            closed = !closed;
        }
        style.FramePadding.y = backup2_y;
        style.ButtonTextAlign.y = backup1_y;
        ImGui::End( );
        ImGui::PopStyleVar( );




    }

    ImGui::GetIO( ).MouseDrawCursor = ImGui::IsWindowHovered( ImGuiHoveredFlags_AnyWindow );

    if ( !game::manager::i( )->in_hunting( ) )
        return;

    game::manager::i( )->entity_callback(  
    	[ ] ( game::s_entity entity ) -> void
    	{
            vec3 out;
            auto cor = ImVec4( 1.f, 0.f, 0.f, 1.f );
            if (! entity.is_boss )
            {
                cor.x = 0.f; cor.y = 1.f;
                if ( entity.is_player && show_player)
                {
                  
                    if ( !game::manager::i( )->w2s( entity.pos, out ) )
                        return;

                    auto distance = game::manager::i( )->get_self_player( )->get_pos( ).distance( &entity.pos );

                    std::ostringstream ss;
                    ss << "PLAYER 0x" << std::hex << std::uppercase << entity.ptr << "\nDistance [ " << std::dec << distance << " ]\n";


                    ImGui::GetOverlayDrawList( )->AddText( ImVec2( out.x + 10, out.y + 30 ),
                        ImGui::GetColorU32( cor ), ss.str( ).c_str( ) );

                }
                return;
            }

            if ( entity.health <= 0 || entity.max_health <= 0 )
                return;

    		if ( !game::manager::i( )->w2s( entity.pos, out ) )
    			return;

            auto distance = game::manager::i( )->get_self_player( )->get_pos( ).distance( &entity.pos );
 
            auto file_monster = getMonsterName( entity.file );
   
            if (file_monster.empty())
                return;

            if (distance > distance_max)
                return;

            std::string hp_hud = " HP:" + std::to_string((int)entity.health) + "/" + std::to_string((int)entity.max_health);

            auto bg_height = 50.f + ( 20.f * ( int( show_distance ) + int( show_entry ) ) );

            drawn_background({ out.x, out.y }, 15, {210.f, bg_height });

            auto bar_size = 170;

            auto now_x = out.x + 20;
            auto now_y = out.y + 10;

            drawn_bar(entity.health, entity.max_health, bar_size, { now_x, now_y });

            now_y += 15;
            if ( show_hp_num )
            {
                draw::string( 18.f, ImVec2( now_x + ( bar_size / 2 ), now_y ), { 1, 1, 1, 1 }, true, true, hp_hud.c_str( ) );               
            } 
            

            now_y += 20;
            draw::string( 18.f, ImVec2( now_x + ( bar_size / 2 ), now_y ), { 1, 1, 1, 1 }, true, true, file_monster.c_str( ) );
            

            if ( show_distance )
            {
                std::ostringstream ss;
                ss << "Distance [ " << (int)distance << "]";
                now_y += 20;
                draw::string( 18.f, ImVec2( now_x + ( bar_size / 2 ), now_y ), { 1, 1, 1, 1 }, true, true, ss.str().c_str( ) );
            }

            if ( show_entry )
            {
                std::ostringstream ss;
                ss << "ENTITY 0x" << std::hex << std::uppercase << entity.ptr << std::endl;
                now_y += 20;
                draw::string( 18.f, ImVec2( now_x + ( bar_size / 2 ), now_y ), { 1, 1, 1, 1 }, true, true, ss.str( ).c_str( ) );
            }

    	} );
}

void __stdcall filter_menu() {
    /* GERADO COM IMGUI BUILDER :) TEM  8 Objetos & 1 forms */
    ImGui::Begin("WConfig", &winconfig, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize );
    ImGui::SetCursorPos({ 14.f,36.f });
    if(ImGui::BeginChild("child0", { 364.f,235.f }, true))
    {
        ImGui::SetCursorPos({ 14.f,15.f });
        ImGui::Text("Distance:");
        ImGui::SetCursorPos({ 14.f,40.f });
        ImGui::SliderInt("##distance", &distance_max, 500, 50000);
        ImGui::SetCursorPos({ 15.f,70.f });
        ImGui::Checkbox("Show Statics", &show_estatics);
        ImGui::SetCursorPos({ 190.f,70.f });
        ImGui::Checkbox("Show Player", &show_player);
        ImGui::SetCursorPos({ 15.f,100.f });
        ImGui::Checkbox("Show HP Numeric", &show_hp_num);
        ImGui::SetCursorPos({ 190.f,100.f });
        ImGui::Checkbox("Show Distance", &show_distance);
        ImGui::SetCursorPos({ 15.f,180 });
        ImGui::Checkbox("[DEV] ADDRESS ENT", &show_entry);
    } 
    ImGui::EndChild();
    ImGui::Text("Hello -> Press F2 to Show / Hide, statics! <- \\o.");
    ImGui::End();
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

    if (suporte_to_d3d12)
    {
        std::cout << "Have compatibily with Dx12\n";
        impl::d3d12::init();
        impl::d3d12::set_overlay(overgay);
        impl::d3d12::set_menu(filter_menu);
    }
    else
    {
        std::cout << "Dont find compatibily with Dx12\n";
        impl::d3d11::init();
        impl::d3d11::set_overlay(overgay);
        // set menu filter ? 
    }

    hooks::init( );

    bool init_hunting = false;

    while ( true )
    {
        Sleep( 100 );

        if (GetAsyncKeyState(VK_F2) & 0x8000)
            show_estatics = !show_estatics;

        if (GetAsyncKeyState(VK_F1)&0x8000)
            winconfig = !winconfig;
            // system("cls");
        

        game::manager::i( )->clear_boss( );

        if ( !game::manager::i( )->in_hunting( ) )
        {
            init_hunting = false;
            continue;
        }

        if ( !init_hunting )
        {
            init_hunting = true;
            std::cout << ( "START THE BATTLE, NOW CLEAR THE LIST!.\n" );
            for ( auto& monster : game::manager::i( )->hunting_damage( ) )
            {
                monster.who_caused_damage.clear( );
                monster.who_caused_damage.swap( monster.who_caused_damage );
            }
            game::manager::i( )->hunting_damage( ).clear( );
            game::manager::i( )->hunting_damage( ).swap( game::manager::i( )->hunting_damage( ) );
            std::cout << "CLEAR LIST\n";
            //printf( "lista limpa.\n" );
        }

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

