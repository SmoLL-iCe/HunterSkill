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
	if ( !code || code == 0x887A0001 )
		return;

	printf( "report problem, code: 0x%X\n", code );

	return reinterpret_cast<decltype( ReportProblem )*>( options::reversed::i( )->ptr.func_crash )( code, str );
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

//void* ochange_skin_internal = (void*)0x1419B3B20;
//char* __fastcall change_skin_internal( char* a1, char feminine, unsigned int body_index, unsigned int skin_id ) 
//{
//	//skin_id = 0x1A4;
//	auto ret = reinterpret_cast<decltype( change_skin_internal )*>( ochange_skin_internal )( a1, feminine, body_index, skin_id );
//
//
//	auto p = game::manager::i( )->get_self_player( );
//
//	printf( " %d, %p\n",feminine, p );
//	return ret;
//
//}
//
//
//void* oget_current_skin_id = (void*)0x01419B3000;
//__int64 __fastcall get_current_skin_id( __int64 ptr, unsigned int index )
//{
//	auto p = game::manager::i( )->get_self_player( );
//	if ( __int64( p + 0x13690 ) == ptr )
//		return 0x1A4;
//
//	auto skin_id = reinterpret_cast<decltype( get_current_skin_id )*>( oget_current_skin_id )( ptr, index );
//
//	return skin_id;
//}

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
		auto val2 = *r_cast<uintptr_t*>( p + 0x13690 + 0xA0 ) == *r_cast<uintptr_t*>( ptr2 + 0xA0 );
		if ( options::config::i( )->fix_skin &&  val2 )
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

	return reinterpret_cast<decltype( modified_skin )*>( omodified_skin )( ptr1, ptr2 );
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

	//return true;
	//hook( change_skin_internal, ochange_skin_internal );
	//hook( get_current_skin_id, oget_current_skin_id );
	hook( modified_skin, omodified_skin );
	hook( show_damage_4, options::reversed::i( )->ptr.damage_meter_func );
	auto is_ok = hook( ReportProblem, options::reversed::i( )->ptr.func_crash );
	Sleep( 5000 );
	//auto p_handle = AddVectoredExceptionHandler( 1, internal_handler );
	return is_ok;
}

