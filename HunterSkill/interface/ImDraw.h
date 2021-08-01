#pragma once
#include "../thirdparty/imgui/imgui.h"
namespace draw
{
	void line_box( ImVec2 pos, ImVec2 dim, ImColor color, int thickness );
	void string( float font_size, const ImVec2& vec, ImVec4 color, bool b_center, bool stroke, const char* p_text, ... );
	void box( const ImVec2& vec_start, const ImVec2& vec_end, ImVec4 color, float thickness );
	void line( const ImVec2& vec_start, const ImVec2& vec_end, ImVec4 color, float thickness = 1.0f );
	void circle( const ImVec2& vec_center, float radius, ImVec4 color, int num_seg, float thickness );
	void rect( const ImVec2& from, const ImVec2& to, const ImVec4& color, float rounding, uint32_t rounding_corners_flags, float thickness );
	void filled_box( const ImVec2& from, const ImVec2& to, const ImVec4& color );
	void box_ol( float x, float y, float w, float h, const ImVec4 color, bool outlined = true, float thickness = 0.4f );
};