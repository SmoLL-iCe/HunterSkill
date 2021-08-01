#include "../header.h"
#include "ImDraw.h"

auto draw::string(const float font_size, const ImVec2& vec, const ImVec4 color, const bool b_center, const bool stroke, const char* p_text, ...) -> void
{
	va_list va_a_list;
	char buf[1024] = { 0 };
	va_start(va_a_list, p_text);
	_vsnprintf_s(buf, sizeof(buf), p_text, va_a_list);
	va_end(va_a_list);
	auto draw_pos = vec;
	auto* const text = buf;
	if ( b_center )
	{
		const auto text_size = ImGui::CalcTextSize( text );
		draw_pos.x = vec.x - ( text_size.x / 2 );
		draw_pos.y = vec.y - text_size.y;
	}
	if (stroke)
	{
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), font_size, ImVec2(draw_pos.x + 1, draw_pos.y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text);
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), font_size, ImVec2(draw_pos.x - 1, draw_pos.y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text);
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), font_size, ImVec2(draw_pos.x + 1, draw_pos.y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text);
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), font_size, ImVec2(draw_pos.x - 1, draw_pos.y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text);
	}
	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), font_size, ImVec2(draw_pos.x, draw_pos.y), ImGui::GetColorU32( color ), text);
}

void draw::line_box( ImVec2 pos, ImVec2 dim, ImColor color, int thickness )
{
	ImGui::GetOverlayDrawList( )->AddLine( pos, ImVec2( pos.x - dim.x, pos.y ), color, thickness );
	ImGui::GetOverlayDrawList( )->AddLine( ImVec2( pos.x - dim.x, pos.y ), ImVec2( pos.x - dim.x, pos.y + dim.y ), color, thickness );
	ImGui::GetOverlayDrawList( )->AddLine( ImVec2( pos.x - dim.x, pos.y + dim.y ), ImVec2( pos.x, pos.y + dim.y ), color, thickness );
	ImGui::GetOverlayDrawList( )->AddLine( ImVec2( pos.x, pos.y + dim.y ), ImVec2( pos.x, pos.y ), color, thickness );
}

//void Renderer::DrawLine( ImVec2 dst, ImVec2 src, ImColor col, int thickness ) 
//{
//	this->GetDrawList( )->AddLine( src, dst, col, thickness );
//}
//
//void Renderer::DrawText( ImVec2 pos, ImColor col, const char* text )
//{
//	this->GetDrawList( )->AddText( pos, col, text, 0 );
//}
//
//void Renderer::DrawHealthBar( ImVec2 pos, ImVec2 dim, ImColor col )
//{
//	this->GetDrawList( )->AddLine( pos, ImVec2( pos.x, pos.y - dim.y ), col, dim.x );
//}


auto draw::box(const ImVec2& vec_start, const ImVec2& vec_end, const ImVec4 color, const float thickness) -> void
{
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(vec_start.x, vec_start.y), ImVec2(vec_start.x + vec_end.x, vec_start.y + vec_end.y), ImGui::GetColorU32( color ), 0, 0, thickness);
}

auto draw::line(const ImVec2& vec_start, const ImVec2& vec_end, const ImVec4 color, const float thickness) -> void
{
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vec_start.x, vec_start.y), ImVec2(vec_end.x, vec_end.y), ImGui::GetColorU32( color ), thickness);
}

auto draw::circle(const ImVec2& vec_center, const float radius, const ImVec4 color, const int num_seg, const float thickness) -> void
{
	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(vec_center.x, vec_center.y), radius, ImGui::GetColorU32( color ), num_seg, thickness);
}

auto draw::rect(const ImVec2& from, const ImVec2& to, const ImVec4& color, const float rounding, const uint32_t rounding_corners_flags, const float thickness) -> void
{
	ImGui::GetOverlayDrawList( )->AddRect( from, to, ImGui::GetColorU32( color ), rounding, rounding_corners_flags, thickness );
}

auto draw::filled_box( const ImVec2& from, const ImVec2& to, const ImVec4& color ) -> void
{
	ImGui::GetOverlayDrawList( )->AddRectFilled( from, to, ImGui::GetColorU32( color ) );
}

void draw::box_ol( float x, float y, float w, float h, const ImVec4 color, bool outlined, float thickness )
{
	if ( outlined )
	{
		auto outline_color = ImGui::GetColorU32( { 1, 1, 1, 1 } );
		const auto b_x = x, b_y = y;
		x += 1;
		y += 1;
		ImGui::GetOverlayDrawList( )->AddRect( ImVec2( x, y ), ImVec2( x + w, y + h ), outline_color, 0.0f, 15, thickness );
		x -= 2;
		ImGui::GetOverlayDrawList()->AddRect( ImVec2( x, y ), ImVec2( x + w, y + h ), outline_color, 0.0f, 15, thickness );
		x += 2;
		y -= 2;
		ImGui::GetOverlayDrawList()->AddRect( ImVec2( x, y ), ImVec2( x + w, y + h ), outline_color, 0.0f, 15, thickness );
		x -= 2;
		ImGui::GetOverlayDrawList()->AddRect( ImVec2( x, y ), ImVec2( x + w, y + h ), outline_color, 0.0f, 15, thickness );
		x = b_x;
		y = b_y;
	}
	ImGui::GetOverlayDrawList()->AddRect( ImVec2( x, y ), ImVec2( x + w, y + h ), ImGui::GetColorU32( color ), 0.0f, 15, thickness );
}

void DrawLine( int x1, int y1, int x2, int y2, float* color, int thickness )
{
	ImGui::GetOverlayDrawList( )->AddLine( ImVec2( x1, y1 ), ImVec2( x2, y2 ), ImGui::ColorConvertFloat4ToU32( reinterpret_cast<ImVec4&>( color ) ), thickness );
}
std::string string_To_UTF8( const std::string& str )
{
	const auto nw_len = ::MultiByteToWideChar( CP_ACP, 0, str.c_str( ), -1, nullptr, 0 );

	auto* pw_buf = new wchar_t[ nw_len + 1 ];
	ZeroMemory( pw_buf, nw_len * 2 + 2 );

	::MultiByteToWideChar( CP_ACP, 0, str.c_str( ), str.length( ), pw_buf, nw_len );

	const auto n_len = ::WideCharToMultiByte( CP_UTF8, 0, pw_buf, -1, nullptr, 0, nullptr, nullptr );

	auto* p_buf = new char[ n_len + 1 ];
	ZeroMemory( p_buf, n_len + 1 );

	::WideCharToMultiByte( CP_UTF8, 0, pw_buf, nw_len, p_buf, n_len, nullptr, nullptr );

	std::string ret_str( p_buf );

	delete[ ]pw_buf;
	delete[ ]p_buf;

	pw_buf = nullptr;
	p_buf = nullptr;

	return ret_str;
}
void DrawNewText( int x, int y, float* color, const char* str )
{
	ImFont a;
	const auto utf_8_1 = std::string( str );
	const auto utf_8_2 = string_To_UTF8( utf_8_1 );
	ImGui::GetOverlayDrawList( )->AddText( ImVec2( x, y ), ImGui::ColorConvertFloat4ToU32( reinterpret_cast<ImVec4&>( color ) ), utf_8_2.c_str( ) );
}