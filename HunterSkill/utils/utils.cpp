#include "utils.h"
#include "ntos.h"

auto v_seed = 1ul;
int64_t u::random( const int min, const int max ) 
{
	if ( v_seed == 1ul )
		RtlRandomEx( &v_seed );
	return static_cast<int64_t>( min + static_cast<int64_t>( RtlRandomEx( &v_seed ) % ( ( max + 1 ) - min ) ) );
}

wchar_t bin_dir[ MAX_PATH ] = { 0 };
wchar_t* u::get_dir_file( const wchar_t* name )
{
	RtlGetFullPathName_U( name, MAX_PATH, bin_dir, nullptr );
	return bin_dir;
}

void u::open_console( const char* title )
{
	FILE* stream = nullptr;
	AllocConsole( );
	freopen_s( &stream, "CONIN$", "r", stdin );
	freopen_s( &stream, "CONOUT$", "w", stdout );
	freopen_s( &stream, "CONOUT$", "w", stderr );
	SetConsoleTitleA( "" );
}

void u::close_console( )
{
	FreeConsole( );
}

void u::set_console_color( const WORD color )
{
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), color );
}
