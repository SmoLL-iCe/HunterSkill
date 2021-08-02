#include "menu.h"
#include <d3d12.h>

struct s_loaded_images
{
	uint8_t* buff = nullptr;
	int				size = 0;
	SIZE_T			ptr_handle_cpu_pos = 0;
	ID3D12Resource* texture = nullptr;
	int				width = 0;
	int				height = 0;
};

namespace impl
{
	namespace d3d12
	{
		s_loaded_images* load_imgs();
		
		s_loaded_images* load_bkg();

		void init();
		float* screen( );
		void set_menu(t_render_overlay p);
		void set_overlay( t_render_overlay p );
		bool LoadTextureFromFile( const char* filename, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height );
		bool LoadTextureFromMemory( const uint8_t* image, size_t size, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height );
	}
}
