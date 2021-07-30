#include "menu.h"
#include <d3d12.h>
namespace impl
{
	namespace d3d12
	{
		void init();
		float* screen( );
		void set_overlay( t_render_overlay p );
		bool LoadTextureFromFile( const char* filename, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height );
		bool LoadTextureFromMemory( const uint8_t* image, size_t size, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height );
	}
}
