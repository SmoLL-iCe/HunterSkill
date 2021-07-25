#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include "shared_config.h"

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
		vec3 get_pos( )
		{
			return *reinterpret_cast<vec3*>( uintptr_t( this ) + options::reversed::i( )->offset.pos );
		}
	};

	struct s_entity
	{
		bool is_boss = false;
		bool is_player = false;
		float health = 0.f;
		float max_health = 0.f;
		vec3 pos;
		c_entity* ptr = nullptr;
		char file[ 100 ]{ };
	};
	struct s_caused_damage
	{
		uintptr_t entity = 0;
		bool is_player = false;
		std::vector<float> damage{ };
		float total_damage = 0;
	};
	struct s_monster_damage
	{
		uintptr_t target_ptr = 0;
		char name[ 100 ]{ };
		std::vector<s_caused_damage> who_caused_damage{ };
	};

}

