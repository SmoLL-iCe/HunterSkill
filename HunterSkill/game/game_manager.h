#pragma once
#include "classes.h"
#include <vector>

using entity_cb = void( __fastcall* )( game::s_entity );
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
		void update_entities( );
		c_entity* get_self_player( );
		bool updated_list = false;
	private:
		uintptr_t m_base = 0;
		uintptr_t m_instance_mgr = 0;
		uintptr_t m_localplayer = 0;
		std::vector<game::s_entity> entities{ };
		std::vector<game::s_entity> new_entities{ };
	};
}


