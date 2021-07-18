#pragma once
#include <Windows.h>
#include <iostream>
#include "shared_config.h"

struct vec3 final
{
	float x, y, z;
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
	inline void Clamp( )
	{
		if ( this->x < -89.0f )
			this->x = -89.0f;

		if ( this->x > 89.0f )
			this->x = 89.0f;

		while ( this->y < -180.0f )
			this->y += 360.0f;

		while ( this->y > 180.0f )
			this->y -= 360.0f;

		this->z = 0.0f;
	}
	inline void to_angle( )
	{
		//57.295776

		this->x *= 57.295776f;
		this->y *= 57.295776f;
		this->z = 0;
	}
	inline void to_radiant( )
	{
		//57.295776

		this->x /= 57.295776f;
		this->y /= 57.295776f;
		this->z = 0;
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
		float health;
		float max_health;
		vec3 pos;
		c_entity* ptr;
	};

}