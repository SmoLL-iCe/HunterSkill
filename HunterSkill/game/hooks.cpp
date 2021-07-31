#include "../header.h"
#include "hooks.h"
#include "shared_config.h"
#include "../utils/mem.h"
#include "../thirdparty/minhook/include/MinHook.h"
#include "../utils/utils.h"
#include <intrin.h>
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
	hook( show_damage_4, options::reversed::i( )->ptr.damage_meter_func );
	auto is_ok = hook( ReportProblem, options::reversed::i( )->ptr.func_crash );
	Sleep( 5000 );
	auto p_handle = AddVectoredExceptionHandler( 1, internal_handler );
	return is_ok;
}

