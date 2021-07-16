
#include "shared.h"
#include <stdio.h>
#include "../thirdparty/imgui/imgui.h"

void impl::showExampleWindow(const char* comment)
{

	ImGui::Begin( "teste" ) ;

	ImGui::Text("Hello");
	ImGui::Button("World!");

	ImGui::End();
}