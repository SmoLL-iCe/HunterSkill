#pragma once

#include <iostream>


using t_render_overlay = void( __stdcall* )( );
namespace impl
{
	void show_menu( );
	float* screen( );
}