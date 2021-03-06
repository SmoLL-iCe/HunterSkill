#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include "shared_config.h"
#include "../utils/mem.h"

struct vec3 final
{
	float x = 0, y = 0, z = 0;
	vec3( ) {};
	vec3( const float x, const float y, const float z ) : x( x ), y( y ), z( z ) {}
	vec3 operator  + ( const vec3& rhs ) const { return vec3( x + rhs.x, y + rhs.y, z + rhs.z ); }
	vec3 operator  - ( const vec3& rhs ) const { return vec3( x - rhs.x, y - rhs.y, z - rhs.z ); }
	vec3 operator  * ( const float& rhs ) const { return vec3( x * rhs, y * rhs, z * rhs ); }
	vec3 operator  / ( const float& rhs ) const { return vec3( x / rhs, y / rhs, z / rhs ); }
	bool operator == ( const vec3& rhs ) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	vec3& operator += ( const vec3& rhs ) { return *this = *this + rhs; }
	vec3& operator -= ( const vec3& rhs ) { return *this = *this - rhs; }
	vec3& operator *= ( const float& rhs ) { return *this = *this * rhs; }
	vec3& operator /= ( const float& rhs ) { return *this = *this / rhs; }
	float length( ) const { return sqrtf( x * x + y * y + z * z ); }
	vec3 normalize( ) const { return *this * ( 1 / length( ) ); }
	float distance( const vec3* rhs ) const
	{
		if ( !rhs ) return 0;
		return ( *this - *rhs ).length( );
	}
	float dis_x( const vec3* rhs ) const
	{
		if ( !rhs ) return 0;
		const auto sub_x = ( this->x - rhs->x );
		return sqrtf( sub_x * sub_x );
	}
	float dis_y( const vec3* rhs ) const
	{
		if ( !rhs ) return 0;
		const auto sub_y = ( this->y - rhs->y );
		return sqrtf( sub_y * sub_y );
	}
	float dis_z( const vec3* rhs ) const
	{
		if ( !rhs ) return 0;
		const auto sub_z = ( this->z - rhs->z );
		return sqrtf( sub_z * sub_z );
	}
	float distance_horizontal( const vec3* rhs ) const
	{
		if ( !rhs ) return 0;
		const auto sub_x = ( this->x - rhs->x );
		const auto sub_z = ( this->z - rhs->z );
		return sqrtf( sub_x * sub_x + sub_z * sub_z );
	}
	float distance_vertical( const vec3* rhs ) const
	{
		if ( !rhs ) return 0;
		const auto sub_y = ( this->y - rhs->y );
		return sqrtf( sub_y * sub_y );
	}
	void Invert( ) { *this *= -1; }
	bool empty( ) const
	{
		return ( x == 0.f && y == 0.f && z == 0.f );
	}
};

namespace game 
{

	class c_entity
	{
	public:
		bool is_boss( )
		{
			return ( *reinterpret_cast<uintptr_t*>( uintptr_t( this ) + options::reversed::i( )->offset.is_boss ) == 0x1 );
		}

		bool is_valid( )
		{
			if ( !mem::is_valid_read( uintptr_t( this ) ) || !mem::is_valid_read( uintptr_t( this ) + options::reversed::i( )->offset.sub_to_health ) )
				return false;
			return *r_cast<uint32_t*>( uintptr_t( this ) + 0xC ) == 8 && *r_cast<uint8_t*>( uintptr_t( this ) + 0x14 ) & 1;
		}

		vec3 get_pos( )
		{
			return *reinterpret_cast<vec3*>( uintptr_t( this ) + options::reversed::i( )->offset.pos );
		}

		float get_health( )
		{
			auto sub = *r_cast<uintptr_t*>( uintptr_t( this ) + options::reversed::i( )->offset.sub_to_health );

			if ( !sub )
				return 0.f;

			return *r_cast<float*>( sub + 0x64 );
		}

		float get_max_health( )
		{
			auto sub = *r_cast<uintptr_t*>( uintptr_t( this ) + options::reversed::i( )->offset.sub_to_health );

			if ( !sub )
				return 0.f;

			return *r_cast<float*>( sub + 0x60 );
		}

	};

	struct s_boss_entity
	{
		c_entity* ptr		= nullptr;
		float health		= 0.f;
		float max_health	= 0.f;
		vec3 pos{ };
		char file[ 100 ]{ };
	};
	struct s_player
	{
		int type_ = -1;
		bool valid = false;
		vec3 pos{ };
		char name[ 100 ]{ };
		c_entity* ptr = nullptr;

	};
	struct s_caused_damage
	{
		uintptr_t entity = 0;
		float best_hit = 0;
		float last_hit = 0;
		float low_hit = 0;
		int hit_count = 0;
		float total_damage = 0;
		int type = 0;
	};
	struct s_monster_damage
	{
		uintptr_t target_ptr = 0;
		char name[ 100 ]{ };
		std::vector<s_caused_damage> who_caused_damage{ };
		float hp_max = 0;
		float hp = 0.f;
	};
#pragma pack(push, 1)
	struct s_body
	{
		bool head = false;
		bool chest = false;
		bool arms = false;
		bool waist = false;
		bool foot = false;
		bool fail = true;
	};
	struct s_body_ids
	{
		uint32_t head = 0xFFFFFFFF;
		uint32_t chest = 0xFFFFFFFF;;
		uint32_t arms = 0xFFFFFFFF;
		uint32_t waist = 0xFFFFFFFF;
		uint32_t foot = 0xFFFFFFFF;
		uint32_t fail = 0xFFFFFFFF;
	};
#pragma pack(pop)
	struct s_skin
	{
		uint32_t id = 0;
		game::s_body b;

	};

}

