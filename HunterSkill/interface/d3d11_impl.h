#include "shared.h"
using t_render_overlay = void( __stdcall* )( );
namespace impl
{
	namespace d3d11
	{
		void init();
		float* screen( );
		void set_overlay( t_render_overlay p );
	}
}
