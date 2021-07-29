#include "../header.h"
#include "shared_config.h"
#include <map>
#include "game_manager.h"
#include "../utils/mem.h"
#include "../interface/d3d11_impl.h"
#include <vector>
#include <sstream>

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
		m_entities.clear( );
		m_entities = m_new_entities;
		m_updated_list = false;
	}
	for ( auto &entity : m_entities )
		func( entity );
}


game::c_entity* game::manager::get_self_player( )
{
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
		m_bosses.clear( );
	}
}

void game::manager::update_entities( )
{

	auto bs = *reinterpret_cast<uintptr_t*>( options::reversed::i( )->ptr.to_lp );

	if ( !bs ) return;

	m_localplayer = reinterpret_cast<uintptr_t( __fastcall* )( uintptr_t )>( options::reversed::i( )->ptr.get_lp )( bs );



	if ( m_updated_list )
		return;

	if ( !m_instance_mgr ) return;

	auto total_entity	= *r_cast<uint32_t*>( m_instance_mgr + options::reversed::i( )->offset.entity_max );

	if ( !total_entity )
		return;

	auto entities_entry = *r_cast<uintptr_t**>( m_instance_mgr + options::reversed::i( )->offset.entities_entry );

	if ( !entities_entry )
		return;


	auto player_vtable = *r_cast<uintptr_t*>( m_localplayer );

	m_new_entities.clear( );
	for ( auto i = 0UL; i < total_entity; ++i )
	{
		s_entity ent;

		auto ptr	= entities_entry[ i ];

		if ( m_localplayer == ptr ) continue;

		ent.ptr		= r_cast<c_entity*>( ptr );

		if ( !ptr ) 
			continue;

		if ( !mem::is_valid_read( ptr ) || !mem::is_valid_read( ptr + 0x7670 ) )
			continue;

		ent.is_player =  *r_cast<uintptr_t*>( ptr ) == player_vtable;

		ent.pos	= ent.ptr->get_pos( );

		ent.is_boss = ent.ptr->is_boss( );

		if (!(	*r_cast< uint32_t*>( ptr + 0xC ) == 8 &&

				*r_cast<  uint8_t*>( ptr + 0x14 ) & 1 &&

			!( ~*r_cast<uintptr_t*>( m_instance_mgr + options::reversed::i( )->offset.mgr_flags1 ) & 

				*r_cast<uintptr_t*>( ptr + 0x40 ) ) ))

			continue;

		if ( *r_cast<uint8_t*>( ptr + 0x561 ) )
			continue;

		if ( *r_cast<uintptr_t*>( ptr + 0x950 ) == 0ui64 )
			continue;

		auto sub = *r_cast<uintptr_t*>( ptr + 0x7670 );

		if ( sub != 0 && mem::is_valid_read( sub + 0x60 ) )
		{
			ent.health		= *r_cast<float*>( sub + 0x64 );

			ent.max_health	= *r_cast<float*>( sub + 0x60 );		
		}

		if ( mem::is_valid_read( ptr + 0x5f28 ) )
		{
			auto ptr_str = *r_cast<uintptr_t*>( ptr + 0x5f28 );

			if ( ptr_str > ptr && ptr_str < (ptr + 0x8f28) )
			{
				strcpy_s( ent.file, r_cast<char*>( ptr_str ) );
			}
		}

		if (ent.is_player)
		{
			ent.type_ = *r_cast<uint32_t*>(ptr + 0xECC4);
		}


		if ( ent.is_boss )
		{
			auto exist = false;
			for (const auto boss : m_bosses )
			{
				if ( boss.ptr == ent.ptr )
				{
					exist = true;
					break;
				}
			}

			if ( !exist )
				m_bosses.push_back( ent );
		}


		m_new_entities.push_back( ent );
	}

	m_updated_list = true;
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



	//std::ostringstream ss;
	//ss << "Attack from" << ( ( isSelfPlayerDamage ) ? " SelfPlayer " : " " ) << "0x" << std::hex << std::uppercase << who_caused_damage << ", to target" <<
	//	( ( isSelfPlayerTarget ) ? " SelfPlayer " : " " ) << "0x" << target << " Damage " << std::dec << (int)damage;

	//std::cout << ss.str( ) << std::endl;;
}