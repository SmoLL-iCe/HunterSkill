#include "../thirdparty/kiero/kiero.h"
#include "win32_impl.h"
#include <Windows.h>

#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui/imgui_impl_win32.h"

static WNDPROC oWndProc = NULL;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK hkWindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam) > 0)
		return 1L;	
	return oWndProc( hwnd, uMsg, wParam, lParam);
}


bool	m_keys[ 256 ] = { };
LRESULT __stdcall wnd_proc( HWND h_wnd, UINT const msg, WPARAM const w_param, LPARAM const l_param )
{
	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		m_keys[ VK_LBUTTON ] = true;
		break;
	case WM_LBUTTONUP:
		m_keys[ VK_LBUTTON ] = false;
		break;
	case WM_RBUTTONDOWN:
		m_keys[ VK_RBUTTON ] = true;
		break;
	case WM_RBUTTONUP:
		m_keys[ VK_RBUTTON ] = false;
		break;
	case WM_KEYDOWN:
		m_keys[ w_param ] = true;
		break;
	case WM_KEYUP:
		m_keys[ w_param ] = false;
		break;
	default: break;
	}
	static auto is_down = false;
	static auto is_clicked = false;
	if ( m_keys[ VK_HOME ] )
	{
		is_clicked = false;
		is_down = true;
	}
	else if ( !m_keys[ VK_HOME ] && is_down )
	{
		is_clicked = true;
		is_down = false;
	}
	else
	{
		is_clicked = false;
		is_down = false;
	}
	//if ( is_clicked )
	//	instance( )->visible( ) = !instance( )->visible( );

	ImGui_ImplWin32_WndProcHandler( h_wnd, msg, w_param, l_param );
	//if ( instance( )->visible( ) && ImGui::IsAnyWindowHovered( ) )
	//	return 1L;

	//ImGui::GetIO( ).MouseDrawCursor = instance( )->visible( );
	return oWndProc( h_wnd, msg, w_param, l_param );
	//return CallWindowProc( wnd_proc_old, h_wnd, msg, w_param, l_param );
}


void impl::win32::init(void* hwnd)
{
	oWndProc = reinterpret_cast<WNDPROC>( SetWindowLongPtr( (HWND)hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( wnd_proc ) ) );
}

