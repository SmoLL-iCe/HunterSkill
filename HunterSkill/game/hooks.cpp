#include "../header.h"
#include "hooks.h"
#include "shared_config.h"
#include "../utils/mem.h"
#include "../thirdparty/minhook/include/MinHook.h"
#include "../utils/utils.h"
#include <intrin.h>
#include <istream>
#include "game_manager.h"

template <typename A1, typename A2>
inline bool hook( A1 detour, A2& r_original )
{
	static auto once = true;
	if ( once )
		once = !( MH_Initialize( ) == MH_OK );
	auto* p_original = reinterpret_cast<void*>( r_original );
	auto mh_status = MH_CreateHook( p_original, reinterpret_cast<void*>( detour ), reinterpret_cast<void**>( &r_original ) );
	if ( mh_status != MH_OK )
		return false;
	mh_status = MH_EnableHook( p_original );
	return ( mh_status == MH_OK );
}

template <typename A1>
inline bool rem_hook( A1 r_original )
{
	auto* p_original = reinterpret_cast<void*>( r_original );
	const auto mh_status = MH_DisableHook( p_original );
	return ( mh_status == MH_OK );
}

void __fastcall ReportProblem( DWORD code, const char* str )
{
	//if ( !code || code == 0x887A0001 )
	//	return;

	//printf( "report problem, code: 0x%X\n", code );

	// return reinterpret_cast<decltype( ReportProblem )*>( options::reversed::i( )->ptr.func_crash )( code, str );
}

long __stdcall internal_handler( EXCEPTION_POINTERS* p_exception_info )
{
	auto* const p_context = p_exception_info->ContextRecord;

	auto* const p_exception_rec = p_exception_info->ExceptionRecord;

	auto report = u::gen_exception_report( p_context, p_exception_rec );

	std::ofstream myfile( "hunterSkillCrashReport.log" );
	if ( myfile.is_open( ) )
	{
		myfile << report.str();
		myfile.close( );
	}

	system( "cls" );
	std::cout << report.str( );

	return EXCEPTION_CONTINUE_SEARCH;
}


//1452275A8
/*
MonsterHunterWorld.exe+23C6774 - 48 8B 8C CB 70140000  - mov rcx,[rbx+rcx*8+00001470]
MonsterHunterWorld.exe+23C677C - 48 85 C9              - test rcx,rcx
MonsterHunterWorld.exe+23C677F - 74 1F                 - je MonsterHunterWorld.exe+23C67A0
*/


char __fastcall show_damage_4( __int64 pre_entity_target, __int64 whoCausedDamage, __int64 damage_info )
{
	if ( whoCausedDamage && pre_entity_target &&
		mem::is_valid_read( pre_entity_target + 8 ) && mem::is_valid_read( whoCausedDamage ) && mem::is_valid_read( damage_info + 0xC ) )
	{
		auto target = *(uintptr_t*)( pre_entity_target + 8 );
		auto entity = *(uintptr_t*)( whoCausedDamage );

		if ( target && entity && target != entity )
		{
			auto last_damage = *(float*)( damage_info + 0xC ) + *(float*)( damage_info + 4 );
			if ( last_damage > 0.f )
			{
				game::manager::i( )->set_damage( entity, target, last_damage );
			}

		}
	}
	return reinterpret_cast<decltype( show_damage_4 )*>( options::reversed::i( )->ptr.damage_meter_func )( pre_entity_target, whoCausedDamage, damage_info );
}

void* omodified_skin = (void*)0x141265770;
__int64 __fastcall modified_skin( __int64 ptr1, __int64 ptr2 )
{
	static int loaded = 0;
	auto result = r_cast<uint32_t*>( &game::manager::i( )->current_skin );
	auto p = game::manager::i( )->get_self_player( );
	if ( !p )
	{

		if ( !loaded )
		{
			++loaded;
			std::ifstream fin;
			std::string word;
			fin.open( "skin.ini", std::ifstream::in );
			if ( fin.good( ) )
			{
				++loaded;
				fin >> word >> options::config::i( )->fix_skin;

				for ( size_t i = 0; i < 5; i++ )
						fin >> word >> result[ i ];
				fin.close( );
				if ( options::config::i( )->fix_skin )
					++loaded;

			}



		}
		if ( loaded == 3  )
		{
			for ( size_t i = 0; i < 5; i++ )
			{
				if ( result[ i ] && result[ i ] != 0xFFFFFFFF )
				{
					*r_cast<uint32_t*>( ptr2 + 4 * i + 0xC4 ) = result[ i ];

				}
			}
		}

	
	}
	else
	{
		auto ptr = p + 0x13690;
		if ( mem::is_valid_read( ptr ) )
		{
			auto val2 = *r_cast<uintptr_t*>( ptr + 0xA0 ) == *r_cast<uintptr_t*>( ptr2 + 0xA0 );
			if ( options::config::i( )->fix_skin && val2 )
			{
				auto result = r_cast<uint32_t*>( &game::manager::i( )->current_skin );
				for ( size_t i = 0; i < 5; i++ )
				{
					if ( result[ i ] && result[ i ] != 0xFFFFFFFF )
					{
						*r_cast<uint32_t*>( ptr2 + 4 * i + 0xC4 ) = result[ i ];
						*r_cast<uint32_t*>( p + 4 * i + 0x13690 + 0xC4 ) = result[ i ];



					}
				}
				auto anotherptr = *r_cast<uintptr_t*>( p + 0x14F8 );
				if ( anotherptr )
				{
					*(BYTE*)( anotherptr + 0x4A45 ) = 1;

				}

			}
		}

	
	
	}

	return reinterpret_cast<decltype( modified_skin )*>( omodified_skin )( ptr1, ptr2 );
}

void* EntityNames = (void*)0x0141E533F0;
extern bool pronto;
char __fastcall NearPlayerName( __int64 a1 )
{
	auto ret = reinterpret_cast<decltype( NearPlayerName )*>( EntityNames )( a1 );

	auto p = game::manager::i( )->get_self_player( );

	if ( p )
	{
		auto aa1		= *reinterpret_cast<uintptr_t*>( a1 + 0x40 );
		auto a2			= *reinterpret_cast<char**>( a1 + 0x100 );
		auto entity		= *reinterpret_cast<uintptr_t*>( a1 + 0x18 );

		if ( aa1 && a2 )
		{
			for ( size_t i = 0; i < 4; i++ )
			{
				if ( uintptr_t( game::manager::i( )->get_players( i )->ptr ) == entity )
				{
					if ( *reinterpret_cast<uint32_t*>( game::manager::i( )->get_players( i )->name ) != *reinterpret_cast<uint32_t*>(a2) )
						strcpy( game::manager::i( )->get_players( i )->name, a2 );
		
					break;
				}
			}
		}
	
	}

	return ret;
}


bool hooks::init( )
{
	while ( options::reversed::i( )->ptr.func_crash[ 0 ] != 0x85 )
		Sleep( 1 );

	Sleep( 500 );

	uint8_t* ac[ ] =
	{

		u8ptr( 0x142781010 ),
		u8ptr( 0x142782790 ),
		u8ptr( 0x142758DD0 ),
		u8ptr( 0x14277BDD0 ),
		u8ptr( 0x142776B90 ),
		u8ptr( 0x14278CE20 ),

		//hook damage
		u8ptr( 0x14277C990 ),
		u8ptr( 0x14276C9D0 ),
		u8ptr( 0x14275DCD0 ),

		//pl
		u8ptr( 0x14277B210 ),

		//sub_142258160 hook
		//u8ptr( 0x142757A10 ),
		//u8ptr( 0x14277F890 ),

		//change_skin_internal
		u8ptr( 0x14277A650 ),
		u8ptr( 0x1427718D0 ),

		//tmp
		u8ptr( 0x1427879D0 ),
		u8ptr( 0x142757A10 ),
		u8ptr( 0x14277D550 ),
	};

	for ( auto* ptr : ac )
	{
		DWORD p;
		if ( VirtualProtect( ptr, 0x8, PAGE_EXECUTE_READWRITE, &p ) )
		{
			ptr[ 0 ] = 0xc3;
			VirtualProtect( ptr, 0x8, p, &p );
		}

	}


	hook( NearPlayerName, EntityNames );
	hook( modified_skin, omodified_skin );
	hook( show_damage_4, options::reversed::i( )->ptr.damage_meter_func );
	auto is_ok = hook( ReportProblem, options::reversed::i( )->ptr.func_crash );
	Sleep( 5000 );
	//auto p_handle = AddVectoredExceptionHandler( 1, internal_handler );
	return is_ok;
}

