#pragma once
#include "classes.h"

using entity_cb = void( __fastcall* )( game::c_entity* );
namespace game
{
	class manager
	{
	public:
		manager( );
		~manager( );
		static manager* i( );
		void entity_callback( entity_cb func );
		static bool w2s( vec3 origin, vec3& out );
	private:
		uintptr_t m_base = 0;
		uintptr_t m_instance_mgr = 0;
		void update_entities( );
	};
}


