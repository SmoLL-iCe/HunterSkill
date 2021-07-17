#include "../header.h"
#include "hooks.h"
#include "shared_config.h"
#include "../utils/mem.h"
#include "../thirdparty/minhook/include/MinHook.h"
#include "../utils/utils.h"

void __fastcall ReportProblem( DWORD code, const char* str )
{
	if ( !code || code == 0x887A0001 )
		return;

	printf( "report problem, code: 0x%X\n", code );

	return reinterpret_cast<decltype( ReportProblem )*>( options::reversed::i( )->ptr.func_crash )( code, str );
}

long __stdcall internal_handler( EXCEPTION_POINTERS* p_exception_info )
{
	auto* const p_context		= p_exception_info->ContextRecord;

	auto* const p_exception_rec = p_exception_info->ExceptionRecord;

	u::display_context(p_context, p_exception_rec);

	return EXCEPTION_CONTINUE_SEARCH;
}

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

bool hooks::init( )
{
	while ( options::reversed::i( )->ptr.func_crash[ 0 ] != 0x85 )
		Sleep( 1 );

	Sleep( 500 );

	//auto p_handle = AddVectoredExceptionHandler( 1, internal_handler );

	auto is_ok = hook( ReportProblem, options::reversed::i( )->ptr.func_crash );
	
	return is_ok;
}

