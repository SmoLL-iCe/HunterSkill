
#include "header.h"
#include "interface/d3d11_impl.h"
#include "interface/d3d12_impl.h"
#include "interface/win32_impl.h"
#include "game/shared_config.h"
#include "game/game_manager.h"
#include "utils/mem.h"
#include "game/hunter_skin.h"

#include "thirdparty/imgui/imgui.h"
#include "game/hooks.h"
#include "game/Monster_names.hpp"
#include "thirdparty/minhook/include/MinHook.h"
#include "interface/ImDraw.h"
#include <iomanip>
#include <time.h> // for clock



void drawn_bar( float hp, float max_hp, int size_, ImVec2 Pos )
{
    // BASE DRAWN
    ImGui::GetOverlayDrawList( )->AddTriangleFilled( Pos, { Pos.x - 15, Pos.y + 7.5f }, { Pos.x, Pos.y + 10 }, ImGui::GetColorU32( { 0.219f, 0.223f, 0.219f, 1.f } ) );

    ImGui::GetOverlayDrawList( )->AddTriangleFilled( { Pos.x + size_, Pos.y }, { Pos.x + size_, Pos.y + 15 }, { Pos.x + size_ + 10, Pos.y + 7.5f }, ImGui::GetColorU32( { 0.219f, 0.223f, 0.219f, 1.f } ) );

    ImGui::GetOverlayDrawList( )->AddRectFilled( { Pos.x, Pos.y - 1 }, { Pos.x + size_, Pos.y + 16 }, ImGui::GetColorU32( { 0.219f, 0.223f, 0.219f, 1.f } ) );


    if ( hp <= 0 )
        return;

    auto percent = ( hp / max_hp ) * 100.f;

    if ( hp <= 0.f )
        return;

    ImVec4 Color_ = { 0.203f, 0.415f, 0.223f, 1.0f };
    if ( percent >= 51 )
        Color_ = { 0.203f, 0.415f, 0.223f, 1.0f };
    else if ( percent >= 21 )
        Color_ = { 0.75f, 0.75f, 0.078f, 1.0f };
    else if ( percent <= 20 )
        Color_ = { 1.f, 0.015f, 0.053f, 1.0f };

    //1, 0.019, 0.050

    //ANIMATION BAR
    if ( percent )
    {
        auto x = size_ * ( percent / 100.f );
        //printf("%f\n", x);
        ImGui::GetOverlayDrawList( )->AddTriangleFilled( Pos, { Pos.x - 15, Pos.y + 7.5f }, { Pos.x, Pos.y + 15 }, ImGui::GetColorU32( Color_ ) );
        ImGui::GetOverlayDrawList( )->AddTriangleFilled( { Pos.x + x, Pos.y }, { Pos.x + x, Pos.y + 15 }, { Pos.x + x + 15, Pos.y + 7.5f }, ImGui::GetColorU32( Color_ ) );
        ImGui::GetOverlayDrawList( )->AddRectFilled( { Pos.x, Pos.y - 1 }, { Pos.x + x, Pos.y + 16 }, ImGui::GetColorU32( Color_ ) );
    }
}

void drawn_background( ImVec2 Pos, float around, ImVec2 Size = { 250, 130 } )
{
    ImGui::GetOverlayDrawList( )->AddRectFilled( Pos, { Pos.x + Size.x, Pos.y + Size.y }, ImGui::GetColorU32( { 0, 0, 0, 0.5 } ), around );
}

std::ostringstream elapsed_time( clock_t t1 )
{
    std::ostringstream ss;
    int dhh = 0;
    int dmm = 0;
    int dss = 0;

    auto t2 = clock( ); // we get the time now

    float difference = ( ( (float)t2 ) - ( (float)t1 ) ); // gives the time elapsed since t1 in milliseconds

   // now get the time elapsed in seconds

    float seconds = difference / 1000; // float value of seconds
    if ( seconds < ( 60 * 60 * 24 ) ) // a day is not over
    {
        dss = fmod( seconds, 60 ); // the remainder is seconds to be displayed
        float minutes = seconds / 60;  // the total minutes in float
        dmm = fmod( minutes, 60 );  // the remainder are minutes to be displayed
        float hours = minutes / 60; // the total hours in float
        dhh = hours;  // the hours to be displayed
    }

    ss << std::setfill( '0' ) << std::setw( 2 ) << dhh << ":" << std::setfill( '0' ) << std::setw( 2 ) << dmm << ":" << std::setfill( '0' ) << std::setw( 2 ) << dss;

    return ss;
}

// Global vars to edit later
bool show_estatics = true, winconfig = true;
bool less = true, av = true, best = true, icon = true, saved;

//filter to menu (TEMP)
bool show_player = true, show_entry = false, show_hp_num = true, show_distance = false;
int distance_max = 10000;

bool closed = true;
bool finish_open = false;
float anim = 0;
bool hovered = false;
clock_t tm = clock( );
s_loaded_images* imgs = impl::d3d12::load_imgs( );
s_loaded_images* bkgs = impl::d3d12::load_bkg( );


int check_map( byte mp )
{
    if ( mp == 28 || mp == 27 || mp == 220 || mp == 17 /* || mp == 136*/ )//ERMO SELVAGEM
        return 0;
    if ( mp == 22 || mp == 31 || mp == 180 || mp == 22 || mp == 118 )//FLORESTA ANCESTRAL
        return 1;
    if ( mp == 44 || mp == 36 || mp == 136 || mp == 45 || mp == 9 )//CORALINOS
        return 2;
    if ( mp == 42 || mp == 29 || mp == 12 || mp == 25 || mp == 162 )//PUTEFRATO
        return 3;
    if ( mp == 31 || mp == 33 || mp == 160 || mp == 37 || mp == 236 )//FENDA DO ANCIÃO
        return 4;
    if ( mp == 43 || mp == 62 || mp == 8 || mp == 34 || mp == 21 )//GLACIAL
        return 5;
    if ( mp == 84 || mp == 20 || mp == 144 || mp == 68 || mp == 11 )//ARENA
        return 6;

    return -1;
}
#define checkBox( val )  \
    if ( b.val ) \
        ImGui::Checkbox( #val, &active.val ); 

#define sameLineChkItem( val, s )  \
    if ( b.val ) \
        ImGui::SameLine( s ); 

#define save( val )  f_out << #val << " " << game::manager::i( )->current_skin.val << std::endl;
static auto skin_id = 1;
static auto old_skin_id = 0;
//static bool fix_skin = true;
// static bool old_fix_skin = true;
static game::s_body b;
static game::s_body active;



void __stdcall overgay( )
{

    auto width = impl::screen( )[ 0 ];
    auto form_width = float( 380 );
    auto form_height = 210.f;
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

    bool overlay = GetAsyncKeyState( VK_TAB ) & 0x8000;

    if ( overlay )
    {

        ImGui::SetNextWindowPos( ImVec2( impl::screen( )[ 0 ] * 0.5f, impl::screen( )[ 1 ] * 0.5f ), ImGuiCond_Always, ImVec2( 0.5f, 0.5f ) );
        ImGui::SetNextWindowSize( { 900, 600 } );
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.f );
        ImGui::Begin( "ESTATICS", &overlay, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar );

        auto pos = ImGui::GetCursorScreenPos( );
        auto a = game::manager::get_map( );
        a = check_map( a );
        std::string map;
        if ( a != -1 )
        {
            //ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
            if ( a >= 0 && a < 7 )
            {
                ImGui::SetCursorPos( { -1, -1 } );
                ImGui::Image( r_cast<ImTextureID>( bkgs[ a ].ptr_handle_cpu_pos ), ImVec2( 899, 599 ) );
                ImGui::SetCursorPos( { 0, 0 } );
            }
            else
            {
                //ImDrawList::AddImageRounded(r_cast<ImTextureID>(bkgs[7].ptr_handle_cpu_pos), ImVec2(899, 599))
                ImGui::SetCursorPos( { -1, -1 } );
                ImGui::Image( r_cast<ImTextureID>( bkgs[ 7 ].ptr_handle_cpu_pos ), ImVec2( 899, 599 ) );
                ImGui::SetCursorPos( { 0, 0 } );
            }
            //ImGui::PopStyleVar();

            switch ( a )
            {
            case 0:
                map = "ERMO SELVAGEM";//\nO MONSTRO SAIU CORRENDO, O PAUNUNKU DE QUEM ESTA LENDO";
                break;
            case 1:
                map = "FLORESTA ANCESTRAL";//\nO MONSTRO SAIU CORRENDO, O PAUNUNKU DE QUEM ESTA LENDO";
                break;
            case 2:
                map = "PLANALTOS CORALINOS";// \nO MONSTRO SAIU CORRENDO, O PAUNUNKU DE QUEM ESTA LENDO";
                break;
            case 3:
                map = "VALE PUTEFRATO";// \nO MONSTRO SAIU CORRENDO, O PAUNUNKU DE QUEM ESTA LENDO";
                break;
            case 4:
                map = "FENDA DO ANCIAO";// \nO MONSTRO SAIU CORRENDO, O PAUNUNKU DE QUEM ESTA LENDO";
                break;
            case 5:
                map = "FLORESTA GLACIAL";// \nO MONSTRO SAIU CORRENDO, O PAUNUNKU DE QUEM ESTA LENDO";
                break;
            case 6:
                map = "ARENA\nO MONSTRO";// SAIU CORRENDO, O PAUNUNKU DE QUEM ESTA LENDO";
                break;
            default:
                map = "NONE NONE \n NANIIIIIIIII??????????";
                break;
            }

        }
        draw::string( 25, { pos.x + 4, pos.y }, { 1, 1, 1, 1 }, false, true, map.c_str( ) );
        draw::string( 30, ImVec2( pos.x + 800, pos.y + 5 ), { 1, 1, 1, 1 }, false, true, elapsed_time( tm ).str( ).c_str( ) );
        ImGui::NewLine( ); ImGui::NewLine( );  ImGui::NewLine( );
        pos = ImGui::GetCursorPos( );
        ImGui::SetCursorPos( { pos.x, pos.y - 11 } );
        ImGui::Separator( );
        pos = ImGui::GetCursorPos( );
        ImGui::SetCursorPos( { pos.x + 10, pos.y } );
        ImGui::BeginChild( "Childe_of_players", { 875, 500 } );
        auto myplayer = uintptr_t( game::manager::i( )->get_self_player( ) );
        if ( myplayer )
        {
            for ( auto& monster : game::manager::i( )->hunting_damage( ) )
            {
                ImGuiWindowFlags  CollapsingHeader_flag = ImGuiTreeNodeFlags_DefaultOpen;
                if ( ImGui::CollapsingHeader( getMonsterName( monster.name ).c_str( ), CollapsingHeader_flag ) )
                {
                    ImGui::Separator( );
                    if ( icon )
                    {
                        ImGui::Text( "ICON" );
                    }
                    ImGui::SameLine( 100 );
                    ImGui::Text( "Name" );
                    ImGui::SameLine( 300 );
                    ImGui::Text( "Damage" );
                    ImGui::SameLine( 450 );
                    ImGui::Text( "BEST" );
                    if ( av )
                    {
                        ImGui::SameLine( 600 );
                        ImGui::Text( "AV" );
                    }
                    if ( less )
                    {
                        ImGui::SameLine( 750 );
                        ImGui::Text( "less" );
                    }

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
                        ImGui::Separator( );
                        float mid = 0;
                        if ( icon )
                        {
                            ImGui::Image( r_cast<ImTextureID>( imgs[ who_dam.type ].ptr_handle_cpu_pos ), ImVec2( s_cast<float>( imgs[ who_dam.type ].width / 2 ), s_cast<float>( imgs[ who_dam.type ].height / 2 ) ) );

                        }
                        ImGui::SameLine( 100 );
                        ImGui::Text( "PLAYER%d", player_index );
                        ImGui::SameLine( 300 );
                        ImGui::TextColored( ( ImVec4( 0, 1, 0, 1 ) ), std::to_string( (int)who_dam.total_damage ).c_str( ) );
                        ImGui::SameLine( 450 );
                        ImGui::Text( std::to_string( (int)who_dam.best_hit ).c_str( ) );
                        if ( av )
                        {
                            if ( who_dam.hit_count > 0 && who_dam.total_damage > 0 )
                                mid = who_dam.total_damage / who_dam.hit_count;
                            ImGui::SameLine( 600 );
                            ImGui::TextColored( ( ImVec4( 1, 1, 0, 1 ) ), std::to_string( (int)mid ).c_str( ) );
                        }
                        if ( less )
                        {
                            ImGui::SameLine( 750 );
                            ImGui::TextColored( ( ImVec4( 1, 0, 0, 1 ) ), std::to_string( (int)who_dam.low_hit ).c_str( ) );
                        }

                    }

                }

            }
        }
        ImGui::EndChild( );
        ImGui::End( );
        ImGui::PopStyleVar( );
        ImGui::PopStyleVar( );
    }


    //if ( false )
    {
        ImGui::PushStyleVar( ImGuiStyleVar_WindowMinSize, ImVec2( 15.f, 15.f ) );

        ImGui::SetNextWindowSize( { form_width, anim }, ImGuiCond_Always );

        ImGui::SetNextWindowPos( { float( width / 2.f ) - float( form_width / 2.f ), 0.f } );

        ImGui::Begin( "##ESTATICS", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse );                          // Create a window called "Hello, world!" and append into it.

        if ( finish_open )
        {
            if ( ImGui::BeginChild( "child0", { 364.f, 180.f }, true ) )
            {
                if ( ImGui::BeginTabBar( "CTabs", ImGuiTabBarFlags_None ) )
                {


                    if ( ImGui::BeginTabItem( "Main" ) )
                    {
                        //ImGui::SetCursorPos({ 14.f,15.f });
                        ImGui::Text( "Distance:" );

                        //ImGui::SetCursorPos({ 14.f,40.f });
                        ImGui::SliderInt( "##distance", &distance_max, 500, 50000 );

                        //ImGui::SetCursorPos({ 15.f,70.f });
                        ImGui::Checkbox( "Show Statics", &show_estatics );
                        ImGui::SameLine( 150 );
                        //ImGui::SetCursorPos({ 190.f,70.f });
                        ImGui::Checkbox( "Show Player", &show_player );
                        //ImGui::SetCursorPos({ 15.f,100.f });
                        ImGui::Checkbox( "Show HP Numeric", &show_hp_num );
                        ImGui::SameLine( 150 );
                        //ImGui::SetCursorPos({ 190.f,100.f });
                        ImGui::Checkbox( "Show Distance", &show_distance );
                        ImGui::EndTabItem( );
                    }
                    if ( ImGui::BeginTabItem( "Statics" ) )
                    {
                        ImGui::Checkbox( "Show average damage", &av );
                        ImGui::Checkbox( "Show less damage", &less );

                        ImGui::Text( "Pos:" );
                        ImGui::RadioButton( "Top", true );
                        ImGui::SameLine( 100 );
                        ImGui::RadioButton( "Left", false );


                        ImGui::EndTabItem( );
                    }
                    if ( ImGui::BeginTabItem( "Skin" ) )
                    {
                        ImGui::Checkbox( "freeze_skin", &options::config::i( )->fix_skin );

                        if ( ImGui::Button( "<<", { 50, 25 } ) )
                        {
                            if ( skin_id > 1 )
                                --skin_id;

                        }
                        ImGui::SameLine( 70.f );
                        ImGui::PushItemWidth( 200.f );
                        ImGui::PushID( 165 );
                        ImGui::SliderInt( "", &skin_id, 1, 727 );
                        ImGui::PopID( );
                        ImGui::PopItemWidth( );
                        ImGui::SameLine( 280.f );
                        if ( ImGui::Button( ">>", { 50, 25 } ) )
                        {
                            if ( skin_id < 727 )
                                ++skin_id;

                        }

                        auto chance_opt = false;
                        if ( skin_id >= 1 && skin_id <= 727 )
                        {
                            if ( old_skin_id != skin_id )
                            {
                                chance_opt = true;
                                for ( auto& skin : skins )
                                {
                                    if ( skin.id == skin_id )
                                    {
                                        b = skin.b;
                                        break;
                                    }
                                }
                                //active = b;
                                old_skin_id = skin_id;
                            }
                        }


                        checkBox( head );
                        sameLineChkItem( chest, 100.f );
                        checkBox( chest );
                        sameLineChkItem( arms, 200.f );
                        checkBox( arms );
                        checkBox( waist );
                        sameLineChkItem( waist, 100.f );
                        checkBox( foot );

                        if ( chance_opt )
                        {
                            auto v = active;

                            for ( size_t i = 0; i < 5; i++ )
                            {
                                if ( reinterpret_cast<bool*>( &v )[ i ] && !reinterpret_cast<bool*>( &b )[ i ] )
                                    reinterpret_cast<bool*>( &v )[ i ] = false;
                            }

                            game::manager::i( )->set_skin_parts( skin_id, v );

                        }


                        if ( ImGui::Button( "save skin config", { 300, 25 } ) )
                        {
                            auto result = r_cast<uint32_t*>( &game::manager::i( )->current_skin );
                            for ( size_t i = 0; i < 5; i++ )
                            {
                                if ( !result[ i ] ) continue;
                                result[ i ] = skin_id;
                            }
                            std::ofstream f_out;
                            f_out.open( "skin.ini", std::ios::trunc );
                            if ( f_out.is_open( ) )
                            {
                                f_out << "fx_skin " << options::config::i( )->fix_skin << std::endl;
                                save( head );
                                save( chest );
                                save( arms );
                                save( waist );
                                save( foot );
                                f_out.close( );
                            }
                        }

                        ImGui::EndTabItem( );
                    }
                    if ( ImGui::BeginTabItem( "Dev" ) )
                    {
                        ImGui::Checkbox( "[DEV] ADDRESS ENT", &show_entry );
                        ImGui::EndTabItem( );
                    }

                }

                ImGui::EndTabBar( );
                //ImGui::Separator();

            }
            ImGui::EndChild( );

        }


        ImGui::SetCursorPos( { 0, ( anim - button_height ) - 3.f } );

        auto& style = ImGui::GetStyle( );
        auto backup1_y = style.ButtonTextAlign.y;
        auto backup2_y = style.FramePadding.y;
        style.FramePadding.y = -1.3f;
        style.ButtonTextAlign.y = 0.f;
        std::string dmg = "HunterSkill Config";
        if ( ImGui::Button( ( ( closed ) ? dmg + " [SHOW]" : dmg + " [HIDE]" ).c_str( ), { float( form_width ), button_height } ) )
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
    {
        tm = clock( );
        return;
    }


    game::manager::i( )->entity_callback(
        []( game::s_entity entity ) -> void
        {
            vec3 out;
            auto cor = ImVec4( 1.f, 0.f, 0.f, 1.f );
            if ( !entity.is_boss )
            {
                cor.x = 0.f; cor.y = 1.f;
                if ( entity.is_player && show_player )
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

            if ( file_monster.empty( ) )
                return;

            if ( distance > distance_max )
                return;

            /// HUD GREAT MONSTER
            {
                std::string hp_hud = " HP:" + std::to_string( (int)entity.health ) + "/" + std::to_string( (int)entity.max_health );

                auto bg_height = 50.f + ( 20.f * ( int( show_distance ) + int( show_entry ) ) );

                drawn_background( { out.x, out.y }, 15, { 210.f, bg_height } );

                auto bar_size = 170;

                auto now_x = out.x + 20;
                auto now_y = out.y + 10;

                drawn_bar( entity.health, entity.max_health, bar_size, { now_x, now_y } );

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
                    draw::string( 18.f, ImVec2( now_x + ( bar_size / 2 ), now_y ), { 1, 1, 1, 1 }, true, true, ss.str( ).c_str( ) );
                }

                if ( show_entry )
                {
                    std::ostringstream ss;
                    ss << "ENTITY 0x" << std::hex << std::uppercase << entity.ptr << std::endl;
                    now_y += 20;
                    draw::string( 18.f, ImVec2( now_x + ( bar_size / 2 ), now_y ), { 1, 1, 1, 1 }, true, true, ss.str( ).c_str( ) );
                }
            }
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
int check_support( )
{
    char name[ 255 ];
    GetModuleFileNameA( NULL, name, 255 );
    std::string local = name;
    size_t pz_str = local.find( "MonsterHunterWorld.exe" );
    if ( pz_str == std::string::npos )
        return 0x58; // Error module


    local.replace( local.begin( ) + pz_str, local.end( ), "graphics_option.ini" );

    std::fstream gcm( local.c_str( ) );

    if ( !gcm.is_open( ) )
        return 0x57; // Error Read file config


    while ( !gcm.eof( ) )
    {
        std::string line;
        std::getline( gcm, line );
        pz_str = line.find( "DirectX12Enable=" );

        if ( pz_str != std::string::npos )
        {
            std::cout << line << std::endl;
            return line.compare( "DirectX12Enable=On" );
        }
    }
    return -1; // dont find any suport
}






int main( )
{

    open_console( );
    Sleep( 1000 );

    int err = check_support( );
    std::cout << err << std::endl;
    suporte_to_d3d12 = !err;

    if ( suporte_to_d3d12 )
    {
        std::cout << "Have compatibily with Dx12\n";
        impl::d3d12::init( );
        impl::d3d12::set_overlay( overgay );
    }
    else
    {
        std::cout << "Dont find compatibily with Dx12\n";
        impl::d3d11::init( );
        impl::d3d11::set_overlay( overgay );
        // set menu filter ? 
    }

    hooks::init( );

    //FROZEN_THREADS threads;
    //Freeze( &threads, UINT_MAX, 1 );
    //system( "pause" );
    //Unfreeze( &threads );

    bool init_hunting = false;

    //DWORD p;
    //VirtualProtect( options::reversed::i( )->ptr.fix_skin, 1, PAGE_EXECUTE_READWRITE, &p );

    while ( true )
    {
        Sleep( 100 );

        //if ( GetAsyncKeyState( VK_F5 ) & 0x8000 )
        //{

        //}
        if ( GetAsyncKeyState( VK_F2 ) & 0x8000 )
            show_estatics = !show_estatics;

        if ( GetAsyncKeyState( VK_F1 ) & 0x8000 )
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
    switch ( ul_reason_for_call )
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

