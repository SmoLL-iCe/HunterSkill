#pragma once
#include "../header.h"

#define U8PTR( val ) reinterpret_cast<uint8_t*>( val )
namespace options
{
	struct config
	{
		explicit config( );
		static config* i( );
		void load_config( );
		void save_config( uint64_t time_now ) const;
		void* start_address		= nullptr;
		bool is_dev_mode		= false;
		bool use_logger			= false;
		bool console_enable		= true;

	};

	struct reversed
	{
		explicit reversed();
		static reversed* i();
		//=====================//=====================
		struct c_ptr
		{
			uint8_t* to_inst_mgr = U8PTR( 0x145183E00 );
			uint8_t* to_viewmatrix = U8PTR( 0x145236E10 );
			uint8_t* func_crash = U8PTR( 0x142570850 );
		}ptr;
		//=====================//=====================
		struct c_offset
		{
			uint32_t viewmatrix = 0x10A460 + 8;
			uint32_t entity_max = 0x3DDEB0;
			uint32_t entities_entry = 0x3DDE70;
			uint32_t is_boss = 0x1395;

			uint32_t entity_flags1 = 0xC;
			uint32_t entity_flags2 = 0x14;
			uint32_t entity_flags3 = 0x40;
			uint32_t mgr_flags1 = 0x4A48;


			uint32_t pos = 0x160;
		}offset;
		//=====================//=====================
		//struct c_exe
		//{
		//	uint8_t * base		= nullptr;
		//	uintptr_t size		= 0;
		//	bool section_region = false;
		//}exe;
		//=====================//=====================
	};

}


