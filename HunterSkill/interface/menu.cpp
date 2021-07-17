
#include "menu.h"
#include <stdio.h>
#include "../thirdparty/imgui/imgui.h"

float f_screen[ 2 ]{ };

float* impl::screen( )
{
	return f_screen;
}

void impl::show_menu( )
{
	ImGui::Begin( "teste" ) ;

	ImGui::Text("Hello");

	ImGui::Button("World!");

	ImGui::End();
}