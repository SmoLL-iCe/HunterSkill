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
		bool in_hunting( );
		void clear_boss( );
		std::vector<game::s_monster_damage>& hunting_damage( );
		void set_damage( uintptr_t who_caused_damage, uintptr_t target, float damage );
		s_body set_skin_full( uintptr_t entity, uint32_t skin_id );
		s_body set_self_skin_full( uint32_t skin_id );
		static bool is_valid_skin( uint32_t index, uint32_t skin_id );
		bool set_skin_parts( uint32_t skin_id, game::s_body b );
		bool m_updated_list = false;
		static byte get_map();
		game::s_body_ids current_skin{ };
	private:
		uintptr_t m_base = 0;
		uintptr_t m_instance_mgr = 0;
		uintptr_t m_localplayer = 0;
		std::vector<game::s_entity> m_entities{ };
		std::vector<game::s_entity> m_new_entities{ };
		std::vector<game::s_entity> m_bosses{ };
		std::vector<game::s_monster_damage> m_monster_damage{ };
	};
}


