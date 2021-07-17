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

void u::display_context( PCONTEXT ContextRecord, PEXCEPTION_RECORD p_exception_record )
{
	printf( "===> ExceptionAddress 0x%p\n", p_exception_record->ExceptionAddress );
#ifdef _M_X64
	printf( " r8[%I64X]\n r9[%I64X]\nr10[%I64X]\nr11[%I64X]\nr12[%I64X]\nr13[%I64X]\nr14[%I64X]\nr15[%I64X]\n"
		"rax[%I64X]\nrbx[%I64X]\nrcx[%I64X]\nrdx[%I64X]\nrbp[%I64X]\nrsi[%I64X]\nrsp[%I64X]\nrdi[%I64X]\n",
		ContextRecord->R8,
		ContextRecord->R9,
		ContextRecord->R10,
		ContextRecord->R11,
		ContextRecord->R12,
		ContextRecord->R13,
		ContextRecord->R14,
		ContextRecord->R15,
		ContextRecord->Rax,
		ContextRecord->Rbx,
		ContextRecord->Rcx,
		ContextRecord->Rdx,
		ContextRecord->Rbp,
		ContextRecord->Rsi,
		ContextRecord->Rsp,
		ContextRecord->Rdi
	);
	printf( "\nP1Home 0x%I64X\nP2Home 0x%I64X\nP3Home 0x%I64X\nP4Home 0x%I64X\nP5Home 0x%I64X\nP6Home 0x%I64X\n",
		ContextRecord->P1Home,
		ContextRecord->P2Home,
		ContextRecord->P3Home,
		ContextRecord->P4Home,
		ContextRecord->P5Home,
		ContextRecord->P6Home
	);
	printf( "\nDr0 0x%I64X\nDr1 0x%I64X\nDr2 0x%I64X\nDr3 0x%I64X\nDr6 0x%I64X\nDr7 0x%I64X\n",
		ContextRecord->Dr0,
		ContextRecord->Dr1,
		ContextRecord->Dr2,
		ContextRecord->Dr3,
		ContextRecord->Dr6,
		ContextRecord->Dr7
	);
	printf( "\nContextFlags 0x%X\nVectorControl 0x%I64X\n",
		ContextRecord->ContextFlags,
		ContextRecord->VectorControl
	);
	printf( "\nDebugControl 0x%I64X\nLastBranchToRip 0x%I64X\nLastBranchFromRip 0x%I64X\nLastExceptionToRip 0x%I64X\nLastExceptionFromRip 0x%I64X\n",
		ContextRecord->DebugControl,
		ContextRecord->LastBranchToRip,
		ContextRecord->LastBranchFromRip,
		ContextRecord->LastExceptionToRip,
		ContextRecord->LastExceptionFromRip
	);
	for ( uint32_t i = 0; i < p_exception_record->NumberParameters; ++i )
	{
		printf( "ExceptionInformation[%d] == %I64X\n", i, p_exception_record->ExceptionInformation[ i ] );
	}
#else
	printf( " Edi[%lX]\nEsi[%lX]\nEbx[%lX]\nEdx[%lX]\nEcx[%lX]"
		"\nEax[%lX]\nEbp[%lX]\nEip[%lX]\nEsp[%lX]\n",
		ContextRecord->Edi,
		ContextRecord->Esi,
		ContextRecord->Ebx,
		ContextRecord->Edx,
		ContextRecord->Ecx,
		ContextRecord->Eax,
		ContextRecord->Ebp,
		ContextRecord->Eip,
		ContextRecord->Esp
	);
	printf( "\nDr0 0x%lX\nDr1 0x%lX\nDr2 0x%lX\nDr3 0x%lX\nDr6 0x%lX\nDr7 0x%lX\n",
		ContextRecord->Dr0,
		ContextRecord->Dr1,
		ContextRecord->Dr2,
		ContextRecord->Dr3,
		ContextRecord->Dr6,
		ContextRecord->Dr7
	);
	for ( uint32_t i = 0; i < p_exception_record->NumberParameters; ++i )
	{
		printf( "ExceptionInformation[%d] == %luX\n", i, p_exception_record->ExceptionInformation[ i ] );
	}
#endif
	printf( "\nSegCs %lu \nSegDs %lu\nSegEs %lu\nSegFs %lu\nSegGs %lu\nSegSs %lu\nEFlags %lu\n",
		ContextRecord->SegCs,
		ContextRecord->SegDs,
		ContextRecord->SegEs,
		ContextRecord->SegFs,
		ContextRecord->SegGs,
		ContextRecord->SegSs,
		ContextRecord->EFlags
	);
	printf( "\nContextFlags 0x%lX\n",
		ContextRecord->ContextFlags
	);
	printf(
		"======================================================="
		"\n" );

}