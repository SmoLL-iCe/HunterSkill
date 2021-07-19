
#include "menu.h"
#include <stdio.h>
#include <string>
#include "../thirdparty/imgui/imgui.h"

float f_screen[ 2 ]{ };

float* impl::screen( )
{
	return f_screen;
}

uint32_t inc_val = 0;

void impl::show_menu( )
{
	ImGui::Begin( "teste" ) ;


	ImGui::Text(std::to_string(inc_val).c_str());
	ImGui::Text("Hello");

	ImGui::Button("World!");

	ImGui::End();
}