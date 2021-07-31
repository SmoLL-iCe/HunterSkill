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

std::ostringstream u::gen_exception_report( PCONTEXT ContextRecord, PEXCEPTION_RECORD p_exception_record )
{
	std::ostringstream show;

	auto m_ExceptionCode = p_exception_record->ExceptionRecord->ExceptionCode;
	int m_exceptionInfo_0 = p_exception_record->ExceptionRecord->ExceptionInformation[ 0 ];
	int m_exceptionInfo_1 = p_exception_record->ExceptionRecord->ExceptionInformation[ 1 ];
	int m_exceptionInfo_2 = p_exception_record->ExceptionRecord->ExceptionInformation[ 2 ];
	show << "===> ExceptionAddress 0x" << std::hex << std::uppercase << p_exception_record->ExceptionAddress << std::endl;

	switch ( m_ExceptionCode )
	{
	case EXCEPTION_ACCESS_VIOLATION:
		show << "Cause: EXCEPTION_ACCESS_VIOLATION, ";
		if ( m_exceptionInfo_0 == 0 )
			// bad read
			show << "Attempted to read from: 0x" << std::hex << std::uppercase << m_exceptionInfo_1 << std::endl;
		
		else if ( m_exceptionInfo_0 == 1 )
			// bad write
			show << "Attempted to write to: 0x" << std::hex << std::uppercase << m_exceptionInfo_1 << std::endl;
		
		else if ( m_exceptionInfo_0 == 8 )
			// user-mode data execution prevention (DEP)
			show << "Data Execution Prevention (DEP) at: 0x" << std::hex << std::uppercase << m_exceptionInfo_1 << std::endl;
		
		else
			// unknown, shouldn't happen
			show << "Unknown access violation at: 0x" << std::hex << std::uppercase << m_exceptionInfo_1 << std::endl;
		break;

	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		show << "Cause: EXCEPTION_ARRAY_BOUNDS_EXCEEDED" << std::endl;
		break;

	case EXCEPTION_BREAKPOINT:
		show << "Cause: EXCEPTION_BREAKPOINT" << std::endl;
		break;

	case EXCEPTION_DATATYPE_MISALIGNMENT:
		show << "Cause: EXCEPTION_DATATYPE_MISALIGNMENT" << std::endl;
		break;

	case EXCEPTION_FLT_DENORMAL_OPERAND:
		show << "Cause: EXCEPTION_FLT_DENORMAL_OPERAND" << std::endl;
		break;

	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		show << "Cause: EXCEPTION_FLT_DIVIDE_BY_ZERO" << std::endl;
		break;

	case EXCEPTION_FLT_INEXACT_RESULT:
		show << "Cause: EXCEPTION_FLT_INEXACT_RESULT" << std::endl;
		break;

	case EXCEPTION_FLT_INVALID_OPERATION:
		show << "Cause: EXCEPTION_FLT_INVALID_OPERATION" << std::endl;
		break;

	case EXCEPTION_FLT_OVERFLOW:
		show << "Cause: EXCEPTION_FLT_OVERFLOW" << std::endl;
		break;

	case EXCEPTION_FLT_STACK_CHECK:
		show << "Cause: EXCEPTION_FLT_STACK_CHECK" << std::endl;
		break;

	case EXCEPTION_FLT_UNDERFLOW:
		show << "Cause: EXCEPTION_FLT_UNDERFLOW" << std::endl;
		break;

	case EXCEPTION_ILLEGAL_INSTRUCTION:
		show << "Cause: EXCEPTION_ILLEGAL_INSTRUCTION" << std::endl;
		break;

	case EXCEPTION_IN_PAGE_ERROR:
		show << "Cause: EXCEPTION_IN_PAGE_ERROR, ";
		if ( m_exceptionInfo_0 == 0 )
			// bad read
			show << "Attempted to read from: 0x" << std::hex << std::uppercase << m_exceptionInfo_1 << std::endl;

		else if ( m_exceptionInfo_0 == 1 )
			// bad write
			show << "Attempted to write to: 0x" << std::hex << std::uppercase << m_exceptionInfo_1 << std::endl;

		else if ( m_exceptionInfo_0 == 8 )
			// user-mode data execution prevention (DEP)
			show << "Data Execution Prevention (DEP) at: 0x" << std::hex << std::uppercase << m_exceptionInfo_1 << std::endl;

		else
			// unknown, shouldn't happen
			show << "Unknown access violation at: 0x" << std::hex << std::uppercase << m_exceptionInfo_1 << std::endl;
		break;

		show << "NTSTATUS: 0x" << std::hex << std::uppercase << m_exceptionInfo_2 << std::endl;
		break;

	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		show << "Cause: EXCEPTION_INT_DIVIDE_BY_ZERO" << std::endl;
		break;

	case EXCEPTION_INT_OVERFLOW:
		show << "Cause: EXCEPTION_INT_OVERFLOW" << std::endl;
		break;

	case EXCEPTION_INVALID_DISPOSITION:
		show << "Cause: EXCEPTION_INVALID_DISPOSITION" << std::endl;
		break;

	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		show << "Cause: EXCEPTION_NONCONTINUABLE_EXCEPTION" << std::endl;
		break;

	case EXCEPTION_PRIV_INSTRUCTION:
		show << "Cause: EXCEPTION_PRIV_INSTRUCTION" << std::endl;
		break;

	case EXCEPTION_SINGLE_STEP:
		show << "Cause: EXCEPTION_SINGLE_STEP" << std::endl;
		break;

	case EXCEPTION_STACK_OVERFLOW:
		show << "Cause: EXCEPTION_STACK_OVERFLOW" << std::endl;
		break;

	case DBG_CONTROL_C:
		show << "Cause: DBG_CONTROL_C" << std::endl;
		break;

	default:
		;
	}


	show << "===> ExceptionAddress 0x" << std::hex << std::uppercase << p_exception_record->ExceptionAddress << std::endl;

#ifdef _M_X64

	show << "r8[ " << std::hex << std::uppercase << ContextRecord->R8 << " ]\n";
	show << "r9[ " << std::hex << std::uppercase << ContextRecord->R9 << " ]\n";
	show << "r10[ " << std::hex << std::uppercase << ContextRecord->R10 << " ]\n";
	show << "r11[ " << std::hex << std::uppercase << ContextRecord->R11 << " ]\n";
	show << "r12[ " << std::hex << std::uppercase << ContextRecord->R12 << " ]\n";
	show << "r13[ " << std::hex << std::uppercase << ContextRecord->R13 << " ]\n";
	show << "r14[ " << std::hex << std::uppercase << ContextRecord->R14 << " ]\n";
	show << "r15[ " << std::hex << std::uppercase << ContextRecord->R15 << " ]\n";
	show << "rax[ " << std::hex << std::uppercase << ContextRecord->Rax << " ]\n";
	show << "rbx[ " << std::hex << std::uppercase << ContextRecord->Rbx << " ]\n";
	show << "rcx[ " << std::hex << std::uppercase << ContextRecord->Rcx << " ]\n";
	show << "rdx[ " << std::hex << std::uppercase << ContextRecord->Rdx << " ]\n";
	show << "rbp[ " << std::hex << std::uppercase << ContextRecord->Rbp << " ]\n";
	show << "rsi[ " << std::hex << std::uppercase << ContextRecord->Rsi << " ]\n";
	show << "rsp[ " << std::hex << std::uppercase << ContextRecord->Rsp << " ]\n";
	show << "rdi[ " << std::hex << std::uppercase << ContextRecord->Rdi << " ]\n";

#else
	show << "edi[ " << std::hex << std::uppercase << ContextRecord->Edi << " ]\n";
	show << "esi[ " << std::hex << std::uppercase << ContextRecord->Esi << " ]\n";
	show << "ebx[ " << std::hex << std::uppercase << ContextRecord->Ebx << " ]\n";
	show << "edx[ " << std::hex << std::uppercase << ContextRecord->Edx << " ]\n";
	show << "ecx[ " << std::hex << std::uppercase << ContextRecord->Ecx << " ]\n";
	show << "eax[ " << std::hex << std::uppercase << ContextRecord->Eax << " ]\n";
	show << "ebp[ " << std::hex << std::uppercase << ContextRecord->Ebp << " ]\n";
	show << "eip[ " << std::hex << std::uppercase << ContextRecord->Eip << " ]\n";
	show << "esp[ " << std::hex << std::uppercase << ContextRecord->Esp << " ]\n";
#endif
	show << "dr0[ " << std::hex << std::uppercase << ContextRecord->Dr0 << " ]\n";
	show << "dr1[ " << std::hex << std::uppercase << ContextRecord->Dr1 << " ]\n";
	show << "dr2[ " << std::hex << std::uppercase << ContextRecord->Dr2 << " ]\n";
	show << "dr3[ " << std::hex << std::uppercase << ContextRecord->Dr3 << " ]\n";
	show << "dr6[ " << std::hex << std::uppercase << ContextRecord->Dr6 << " ]\n";
	show << "dr7[ " << std::hex << std::uppercase << ContextRecord->Dr7 << " ]\n";
#ifdef _M_X64
	show << "DebugControl[ "			<< std::hex << std::uppercase << ContextRecord->DebugControl			<< " ]\n";
	show << "LastBranchToRip[ "			<< std::hex << std::uppercase << ContextRecord->LastBranchToRip			<< " ]\n";
	show << "LastBranchFromRip[ "		<< std::hex << std::uppercase << ContextRecord->LastBranchFromRip		<< " ]\n";
	show << "LastExceptionToRip[ "		<< std::hex << std::uppercase << ContextRecord->LastExceptionToRip		<< " ]\n";
	show << "LastExceptionFromRip[ "	<< std::hex << std::uppercase << ContextRecord->LastExceptionFromRip	<< " ]\n";
#endif
	for ( uint32_t i = 0; i < p_exception_record->NumberParameters; ++i )
		show << "ExceptionInformation[ " << std::dec << i << " ] == [ " << std::hex << std::uppercase << p_exception_record->ExceptionInformation[ i ] << " ]\n";


	show << "SegCs[ " << std::hex << std::uppercase << ContextRecord->SegCs << " ]\n";
	show << "SegDs[ " << std::hex << std::uppercase << ContextRecord->SegDs << " ]\n";
	show << "SegEs[ " << std::hex << std::uppercase << ContextRecord->SegEs << " ]\n";
	show << "SegFs[ " << std::hex << std::uppercase << ContextRecord->SegFs << " ]\n";
	show << "SegGs[ " << std::hex << std::uppercase << ContextRecord->SegGs << " ]\n";
	show << "SegSs[ " << std::hex << std::uppercase << ContextRecord->SegSs << " ]\n";
	show << "EFlags[ " << std::hex << std::uppercase << ContextRecord->EFlags << " ]\n";
	show << "ContextFlags[ " << std::hex << std::uppercase << ContextRecord->ContextFlags << " ]\n";

	show <<
		"==============================================================================================================\n";
	return show;
}