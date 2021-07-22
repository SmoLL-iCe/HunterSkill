#include "menu.h"
#include <Windows.h>
namespace impl
{
	namespace win32
	{
		void init(void* hwnd);
		HWND find_my_window(uint32_t pid);
	}
}