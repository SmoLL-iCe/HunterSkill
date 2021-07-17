#pragma once
#include "../header.h"
namespace u
{
	void open_console( const char* title );
	void close_console( );
	void set_console_color( WORD color );
	wchar_t* get_dir_file( const wchar_t* name );
	int64_t random( int min, int max );
	void display_context( PCONTEXT ContextRecord, PEXCEPTION_RECORD p_exception_record );
};