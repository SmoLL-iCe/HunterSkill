#include "../header.h"
#include "shared_config.h"
#include <map>
#include "game_manager.h"
#include "../interface/d3d11_impl.h"
#include <vector>
#include <sstream>
#include "../thirdparty/minhook/include/MinHook.h"
#include "Monster_names.hpp"

game::manager* c_manager = nullptr;

game::manager::manager( )
{

	m_instance_mgr		= *r_cast<uintptr_t*>(	options::reversed::i( )->ptr.to_inst_mgr );

	m_base				= r_cast<uintptr_t>(	GetModuleHandle( nullptr ) );

	c_manager			= this;
}

game::manager* game::manager::i( )
{
	if ( !c_manager ) new manager();
	return c_manager;
}

game::manager::~manager( ) = default;

typedef struct _D3DMATRIX {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[ 4 ][ 4 ];
	};
} D3DMATRIX;


float D3DXVec3Dot( CONST vec3* pV1, CONST vec3* pV2 )
{
	return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}


bool game::manager::w2s( vec3 origin, vec3& out )
{

	auto mm					= *r_cast<uintptr_t*>( options::reversed::i( )->ptr.to_viewmatrix );

	if ( !mm ) return false;

	auto temp				= *r_cast<D3DMATRIX*>( mm + options::reversed::i( )->offset.viewmatrix );

	auto translation_vec	= vec3( temp._41, temp._42, temp._43 );
	auto up					= vec3( temp._21, temp._22, temp._23 );
	auto right				= vec3( temp._11, temp._12, temp._13 );

	auto w = D3DXVec3Dot( &translation_vec, &origin ) + temp._44;

	if ( w < 1.29f )
	{
		out.x = 0;
		out.y = 0;
		return false;
	}

	auto y = D3DXVec3Dot( &up,		&origin ) + temp._24;
	auto x = D3DXVec3Dot( &right,	&origin ) + temp._14;

	out.x = ( impl::screen( )[ 0 ] / 2 ) * ( 1 + ( x / w ) );
	out.y = ( impl::screen( )[ 1 ] / 2 ) * ( 1 - ( y / w ) );

	return true;
}

void game::manager::entity_callback( entity_cb func )
{
	if ( !func ) return;
	if ( m_updated_list )
	{
		//m_entities.clear( );
		memcpy( m_entities, m_new_entities, sizeof( game::s_boss_entity ) * 3 );
		m_updated_list = false;
	}
	for ( auto &entity : m_entities )
		func( entity );
}


game::c_entity* game::manager::get_self_player( )
{
	if ( !m_localplayer )
	{

		auto bs = *reinterpret_cast<uintptr_t*>( options::reversed::i( )->ptr.to_lp );

		if ( !bs ) return 0 ;

		m_localplayer = reinterpret_cast<uintptr_t( __fastcall* )( uintptr_t )>( options::reversed::i( )->ptr.get_lp )( bs );
	}
	return r_cast<c_entity*>( m_localplayer );
}

bool game::manager::in_hunting( )
{
	return ( 0 != *reinterpret_cast<uintptr_t*>( options::reversed::i( )->ptr.ptr_ingame ) );
}

std::vector<game::s_monster_damage>& game::manager::hunting_damage( )
{
	return m_monster_damage;
}

void game::manager::clear_boss( )
{
	if ( !game::manager::i( )->in_hunting( ) )
	{
		//m_bosses.clear( );
	}
}

__int64 __fastcall sub_140A197B0( __int64 a1 )
{
	__int64 result; // rax

	result = *(uintptr_t*)( a1 + 0x138 );
	if ( !result || !( *(BYTE*)( result + 12 ) & 0xE ) )
		result = 0i64;
	return result;
}

bool __fastcall sub_141BCA690( __int64 a1 )
{
	return *(DWORD*)( a1 + 0xA38 ) == 4;
}


void game::manager::update_entities( )
{

	auto bs = *reinterpret_cast<uintptr_t*>( options::reversed::i( )->ptr.to_lp );

	if ( !bs ) return;

	m_localplayer = reinterpret_cast<uintptr_t( __fastcall* )( uintptr_t )>( options::reversed::i( )->ptr.get_lp )( bs );

	players[ 0 ].ptr = (c_entity*)m_localplayer;


	if ( m_updated_list )
		return;

	auto player_vtable = *r_cast<uintptr_t*>( m_localplayer );

	//m_new_entities.clear( );


	for ( auto player_index = 0; player_index < 4; player_index++ )
	{
		auto ptr = m_localplayer + ( options::reversed::i( )->offset.entity_size * player_index );

		auto t_v = *r_cast<uintptr_t*>( ptr );

		players[ player_index ].valid = player_vtable == t_v;

		if ( players[ player_index ].valid )
		{
			auto entity						= r_cast<c_entity*>( ptr );

			players[ player_index ].type_	= *r_cast<uint32_t*>( ptr + options::reversed::i( )->offset.weapon_type );

			players[ player_index ].pos		= entity->get_pos( );

			players[ player_index ].ptr		= entity;
		}
	}

	auto inst = *reinterpret_cast<uintptr_t*>( 0x14506D410 );
	auto v28 = 0x80i64;
	auto v29 = inst + 0x38;
	auto v30 = 0i64;
	int x = 0;

	for ( size_t j = 0; j < v28; j++ )
	{
		auto v31 = *(uintptr_t*)v29;
		v29 += 8i64;

		if ( !v31 ) continue;


		auto ENTITY = sub_140A197B0( v31 );

		auto ptr = ENTITY;

		s_boss_entity ent;

		if ( m_localplayer == ptr ) continue;

		ent.ptr = r_cast<c_entity*>( ptr );

		if ( !ptr )
			continue;

		if ( !ent.ptr->is_valid() )
			continue;

		ent.pos = ent.ptr->get_pos( );

		if ( !ent.ptr->is_boss( ) )
			continue;

		if ( mem::is_valid_read( ptr + options::reversed::i( )->offset.string_file ) )
		{
			auto ptr_str = *r_cast<uintptr_t*>( ptr + options::reversed::i( )->offset.string_file );
			if ( ptr_str > ptr && ptr_str < ( ptr + options::reversed::i( )->offset.entity_size ) )
			{
				auto file = std::string( r_cast<char*>( ptr_str ) );

				auto monster_name = getMonsterName( file );

				if ( monster_name.empty( ) )
					monster_name = file;


				strcpy_s( ent.file, monster_name.data( ) );
			}
		}

		ent.health = ent.ptr->get_health( );

		ent.max_health = ent.ptr->get_max_health( );

		m_new_entities[ x++ ] = ent;
	}




	//auto inst = *(uintptr_t*)0x145073DB0;
	//auto total = *(DWORD*)( inst + 0x108 );
	//auto enty = *(uintptr_t**)( inst + 0x118 );


	//for ( size_t i = 0, x = 0; i < total; i++ )
	//{
	//	auto v23 = enty[ i ];
	//	auto ptr = *(uintptr_t*)( v23 + 8 );

	//	s_boss_entity ent;

	//	if ( m_localplayer == ptr ) continue;

	//	ent.ptr = r_cast<c_entity*>( ptr );

	//	if ( !ptr )
	//		continue;

	//	if ( !ent.ptr->is_valid() )
	//		continue;

	//	ent.pos = ent.ptr->get_pos( );

	//	if ( !ent.ptr->is_boss( ) )
	//		continue;

	//	if ( mem::is_valid_read( ptr + options::reversed::i( )->offset.string_file ) )
	//	{
	//		auto ptr_str = *r_cast<uintptr_t*>( ptr + options::reversed::i( )->offset.string_file );
	//		if ( ptr_str > ptr && ptr_str < ( ptr + options::reversed::i( )->offset.entity_size ) )
	//		{
	//			auto file = std::string( r_cast<char*>( ptr_str ) );

	//			auto monster_name = getMonsterName( file );

	//			if ( monster_name.empty( ) )
	//				monster_name = file;


	//			strcpy_s( ent.file, monster_name.data( ) );
	//		}
	//	}

	//	ent.health = ent.ptr->get_health( );

	//	ent.max_health = ent.ptr->get_max_health( );

	//	//if ( ent.is_boss )
	//	//{
	//	//	auto exist = false;
	//	//	for ( const auto& boss : m_bosses )
	//	//	{
	//	//		if ( boss.ptr == ent.ptr )
	//	//		{
	//	//			exist = true;
	//	//			break;
	//	//		}
	//	//	}

	//	//	if ( !exist )
	//	//		m_bosses.push_back( ent );
	//	//}

	//	m_new_entities[ x++ ] = ent;
	//}
	memcpy( m_bosses, m_new_entities, sizeof( game::s_boss_entity ) * 3 );

	m_updated_list = true;
}

game::s_player* game::manager::get_players(int index )
{
	return &players[index];
}

byte game::manager::get_map()
{
	if (mem::is_valid_read(options::reversed::i()->ptr.get_mapp))
		return	*r_cast<int*>(options::reversed::i()->ptr.get_mapp);

	return 0;
}

void game::manager::set_damage( uintptr_t who_caused_damage, uintptr_t target, float damage )
{

	if ( !m_localplayer )
		return;

	auto isSelfPlayerDamage = m_localplayer == who_caused_damage;

	auto isSelfPlayerTarget = m_localplayer == target;

	int type = -1;


	if ( *r_cast<uintptr_t*>( who_caused_damage ) != *r_cast<uintptr_t*>( m_localplayer ) )
		return;

	auto exist = false;

	for ( auto& monster : game::manager::i( )->hunting_damage( ) )
	{
		if ( exist = ( monster.target_ptr == target ) )
		{

			auto exist_2 = false;
			for ( auto & who_dam : monster.who_caused_damage )
			{

				if (monster.hp <= 0)
					continue;

				if(mem::is_valid_read(who_dam.entity + 0xECC4))
					who_dam.type = *r_cast<uint32_t*>(who_dam.entity +0xECC4);
				
				if ( exist_2 = ( who_dam.entity == who_caused_damage ) )
				{
					if ( who_dam.best_hit < damage )
						who_dam.best_hit = damage;

					if ( who_dam.low_hit > damage )
						who_dam.low_hit = damage;

					who_dam.last_hit = damage;
					++who_dam.hit_count;

					who_dam.total_damage += damage;
					break;
				}
			}

			if ( !exist_2 )
			{
				monster.who_caused_damage
				.push_back( {
					who_caused_damage,
					damage,
					damage,
					damage,
					1,
					damage
				} );
			}
			break;
		}
	}
	if ( !exist )
	{
		auto target_entity  = r_cast<c_entity*>( target );

		if ( target_entity->is_boss( ) )
		{
			game::manager::i( )->hunting_damage( )
				.push_back( { 
					target
				} );

			for ( auto &boss : m_bosses )
			{
				if ( uintptr_t(boss.ptr) == target )
				{
					strcpy_s( 
						game::manager::i( )->hunting_damage( )
						.back( ).name, 
						boss.file );
					break;
				}

			}

			if ( mem::is_valid_read( target + 0x7670 ) )
			{
				auto sub = *r_cast<uintptr_t*>( target + 0x7670 );

				if ( sub != 0 && mem::is_valid_read( sub + 0x60 ) )
				{
					game::manager::i( )->hunting_damage( )
						.back( )
						.hp_max = *r_cast<float*>( sub + 0x60 );
				}
				if (sub != 0 && mem::is_valid_read(sub + 0x64))
				{
					game::manager::i()->hunting_damage()
						.back()
						.hp = *r_cast<float*>(sub + 0x64);
				}
			}

			game::manager::i( )->hunting_damage( )
				.back( )
				.who_caused_damage
				.push_back( {
					who_caused_damage,
					damage,
					damage,
					damage,
					1,
					damage
				} );

		}

	

	}

}


bool game::manager::is_valid_skin( uint32_t index, uint32_t skin_id  )
{
	return reinterpret_cast<uintptr_t( __fastcall* )( unsigned int, unsigned int )>( options::reversed::i( )->ptr.get_skin_ptr )( index, skin_id ) != 0;

}

bool game::manager::set_skin_parts( uint32_t skin_id, game::s_body b )
{
	auto entity = uintptr_t( get_self_player( ) );
	if ( !entity || !skin_id )
		return false;


	auto ret = true;

	auto result = r_cast<uint8_t*>( &b );

	auto sub_localPlayer = *r_cast<uintptr_t*>( entity + 0x126C8i64 );

	FROZEN_THREADS threads;
	Freeze( &threads, UINT_MAX, 1 );

	if ( sub_localPlayer )
		for ( size_t i = 0; i < 5; i++ )
		{
			if ( !result[ i ] ) continue;

			if ( !is_valid_skin( i, skin_id ) ) continue;

			auto old_skin = *r_cast<uint32_t*>( entity + 4i64 * i + 0x13754 );

			auto SubModel = *r_cast<uintptr_t*>( ( (unsigned __int64)i << 7 ) + sub_localPlayer + 0x70 );

			if ( !SubModel )
				continue;

			Sleep( 20 );
			reinterpret_cast<void( __fastcall* )( uintptr_t, unsigned int, unsigned int )>( 0x1412FFA80 )( sub_localPlayer, i, skin_id );


			auto apllyed_ptr1 = *r_cast<uintptr_t*>( SubModel + 0x4A8 );

			auto applied_ptr2 = *r_cast<uintptr_t*>( SubModel + 0x4C0 );

			if ( apllyed_ptr1 && applied_ptr2 )
			{
				*r_cast<uint32_t*>( entity + 4i64 * i + 0x13754 ) = skin_id;
			}
			else
			{
				ret = false;
				Sleep( 20 );
				reinterpret_cast<void( __fastcall* )( uintptr_t, unsigned int, unsigned int )>( 0x1412FFA80 )( sub_localPlayer, i, old_skin );
			}


			//*r_cast<uint32_t*>( entity + 4i64 * i + 0x13754 ) = skin_id;

		}
	Unfreeze( &threads );
	auto anotherptr = *r_cast<uintptr_t*>( entity + 0x14F8 );
	if ( anotherptr )
	{
		*(BYTE*)( anotherptr + 0x4A45 ) = 1;

	}
	return ret;
}

