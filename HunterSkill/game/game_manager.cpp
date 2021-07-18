#include "../header.h"
#include "shared_config.h"
#include <map>
#include "game_manager.h"
#include "../utils/mem.h"
#include "../interface/d3d11_impl.h"
#include <vector>

//bool called_callback = false;
//bool update_list = false;

//std::vector<game::c_entity*> entities{ };

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
	if ( updated_list )
	{
		entities.clear( );
		entities = new_entities;
		updated_list = false;
	}
	for ( auto &entity : entities )
		func( entity );
}

void game::manager::update_entities( )
{
	if ( updated_list )
		return;

	if ( !m_instance_mgr ) return;

	auto total_entity	= *r_cast<uint32_t*>( m_instance_mgr + options::reversed::i( )->offset.entity_max );

	if ( !total_entity )
		return;

	auto entities_entry = *r_cast<uintptr_t**>( m_instance_mgr + options::reversed::i( )->offset.entities_entry );

	if ( !entities_entry )
		return;

	new_entities.clear( );
	for ( auto i = 0UL; i < total_entity; ++i )
	{
		auto ptr = entities_entry[ i ];

		if ( !ptr ) 
			continue;

		if ( !mem::is_valid_read( ptr ) || !mem::is_valid_read( ptr + 0x7670 ) )
			continue;

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

		if ( sub == 0 || !mem::is_valid_read( sub ) )
			continue;

		auto entity = r_cast<c_entity*>( ptr );

		new_entities.push_back( {

				entity->is_boss( ),

				*r_cast<float*>( sub + 0x64 ),

				*r_cast<float*>( sub + 0x60 ),

				entity->get_pos( ),

				entity
			} );
	}

	updated_list = true;
	//while ( called_callback ) //race condition
	//	Sleep( 0 );
	//update_list = true;
	//entities.clear();
	//entities = new_entities;
	//update_list = false;

}