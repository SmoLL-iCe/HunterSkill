#pragma once
#include "../header.h"
#include <vector>

#if _WIN32 || _WIN64
#if _WIN64
#define ENV64BIT
#else
#define ENV32BIT
#endif
#endif

namespace i_mem
{
#ifdef ENV32BIT
	void hook_inst32( const uint8_t* p_detour, uint8_t*& p_address_original, const int size_hk );
#endif
	uint8_t* search_pattern( const uint8_t* p_pattern, size_t size, uint8_t wildcard, bool use_wild_card, uint8_t* scan_start, size_t scan_size );
	bool wpm_ptr( void* p_address, const std::vector<uintptr_t>& offsets, void* p_val, size_t size );
	bool rpm_ptr( void* p_address, const std::vector<uintptr_t>& offsets, void* p_val, size_t size );
	bool virtual_protect( void* p_address, size_t number_of_bytes, uint32_t new_protect, uint32_t* old_protect );
	void* virtual_alloc( void* lp_address, size_t dw_size, uint32_t fl_allocation_type, uint32_t fl_protect );
	size_t virtual_query( void* p_address, void* p_buffer, size_t dw_length );
	bool virtual_free( void* lp_address, size_t dw_size, uint32_t dw_free_type );
	bool patch_mem( void* lp_address, const void* t_bytes, size_t size );
	bool set_mem( void* p_address, uint8_t t_byte, size_t size );
	void* get_module_handle( const wchar_t* w_module_name );
	bool wpm( void* p_address, void* p_val, size_t size );
	bool rpm( void* p_address, void* p_val, size_t size );
	size_t get_module_size( uint8_t* mod_base );
	bool is_valid_write_obsolete( void* p );
	bool is_valid_read_obsolete( void* p );
	bool is_valid_write_safe( void* p, size_t size = 8 );
	bool is_valid_read_safe( void* p, size_t size = 8 );

	inline bool is_valid_write( void* p, size_t size = 8 )
	{
#ifdef USENOSAFE
		return is_valid_write_obsolete( p );
#else
		return is_valid_write_safe( p, size );
#endif
	}
	inline bool is_valid_read( void* p, size_t size = 8 )
	{
#ifdef USENOSAFE
		return is_valid_read_obsolete( p );
#else
		return is_valid_read_safe( p, size );
#endif
	}

}
namespace mem
{
	template <typename A = uint8_t*, typename B = uint8_t*, typename C = uint8_t*>
	C find_pattern( A pattern, const size_t size, const uint8_t wildcard, const bool use_wild_card, B scan_start, const size_t scan_size )
	{
		return static_cast<C>( i_mem::search_pattern( reinterpret_cast<uint8_t*>( pattern ), size, wildcard, use_wild_card,
			reinterpret_cast<uint8_t*>( scan_start ), scan_size ) );
	}

	template <typename A = uint8_t*, typename B = uint8_t*, typename C = uint8_t*>
	C find_pattern( A pattern, const size_t size, const uint8_t wildcard, const bool use_wild_card, B scan_start )
	{
		auto const p_start = reinterpret_cast<uint8_t*>( scan_start );
		auto const s_size = i_mem::get_module_size( p_start );
		return static_cast<C>( i_mem::search_pattern( reinterpret_cast<uint8_t*>( pattern ), size, wildcard, use_wild_card, p_start,
			s_size ) );
	}

	template <typename A = uint8_t*, typename C = uint8_t*>
	C find_pattern( A pattern, const size_t size, const uint8_t wildcard, const bool use_wild_card = true )
	{
		auto const p_start = GetModuleHandle( nullptr );
		auto const s_size = i_mem::get_module_size( p_start );
		return static_cast<C>( i_mem::search_pattern( reinterpret_cast<uint8_t*>( pattern ), size, wildcard, use_wild_card, p_start,
			s_size ) );
	}

	template <typename A = uint8_t*, typename B = uint8_t*, typename C = uint8_t*>
	C find_pattern( A pattern, const size_t size, B scan_start )
	{
		auto const p_start = reinterpret_cast<uint8_t*>( scan_start );
		auto const s_size = i_mem::get_module_size( p_start );
		return static_cast<C>( i_mem::search_pattern( reinterpret_cast<uint8_t*>( pattern ), size, 0xCC, true, p_start, s_size ) );
	}

	template <typename A = uint8_t*, typename B = void*>
	bool wpm_ptr( A address, const std::vector<uintptr_t>& offsets, B* val )
	{
		return i_mem::wpm_ptr( reinterpret_cast<void*>( address ), offsets, reinterpret_cast<void*>( val ), sizeof B );
	}

	template <typename A = uint32_t, typename B = uint8_t*>
	A rpm_ptr( B address, const std::vector<uintptr_t> offsets )
	{
		A t_val;
		i_mem::rpm_ptr( reinterpret_cast<void*>( address ), offsets, reinterpret_cast<void*>( &t_val ), sizeof B );
		return t_val;
	}

	template <typename A = uint8_t*>
	size_t get_module_size( A mod_base )
	{
		return i_mem::get_module_size( reinterpret_cast<uint8_t*>( mod_base ) );
	}

	template <typename A = uint8_t*, typename B = uintptr_t, typename C = uintptr_t>
	bool patch_mem( A p_address, B bytes, const size_t size )
	{
		return i_mem::patch_mem( reinterpret_cast<void*>( p_address ), reinterpret_cast<void*>( bytes ), size );
	}

	template <typename A = uint8_t*>
	bool set_mem( A p_address, const uint8_t t_byte, const size_t size )
	{
		return i_mem::set_mem( reinterpret_cast<void*>( p_address ), t_byte, size );
	}

	template <typename A = uint8_t*, typename B = uint8_t*>
	bool wpm( A p_address, B* p_val )
	{
		return i_mem::wpm( reinterpret_cast<void*>( p_address ), reinterpret_cast<void*>( p_val ), sizeof B );
	}

	template <typename A = uint32_t, typename B = uint8_t*>
	A rpm( B p_address )
	{
		A t_val;
		i_mem::rpm( reinterpret_cast<void*>( p_address ), reinterpret_cast<void*>( &t_val ), sizeof B );
		return t_val;
	}

	template <typename A = uint8_t*>
	bool is_valid_write( A p, size_t s = 0 )
	{
#ifdef USENOSAFE
		return i_mem::is_valid_write_obsolete( reinterpret_cast<void*>( p ) );
#else
		return i_mem::is_valid_write_safe( reinterpret_cast<void*>( p ), s );
#endif
	}

	template <typename A = uint8_t*>
	bool is_valid_read( A p, size_t s = 0 )
	{
#ifdef USENOSAFE
		return i_mem::is_valid_read_obsolete( reinterpret_cast<void*>( p ) );
#else
		return i_mem::is_valid_read_safe( reinterpret_cast<void*>( p ), s );
#endif
	}

#ifdef ENV32BIT
	template <typename A = void*, typename B = void*>
	void hook_inst32( A detour, B& address_original, const int size_hk )
	{
		const auto p_detour = reinterpret_cast<uint8_t*>( detour );
		i_mem::hook_inst32( p_detour, address_original, size_hk );
	}
#endif


	template<typename T>
	uint32_t get_offset( uint8_t* local_address, uint32_t const index )
	{
		uint32_t ret = 0;
		if ( local_address && i_mem::is_valid_read( local_address ) )
			ret = *reinterpret_cast<T*>( &local_address[ index ] );
		return ret;
	}

	template <typename T, typename B = uint8_t*>
	uint32_t scan_offset_index( uint8_t* a_pattern, const uint32_t size, const uint32_t index, uint8_t wild_card, uint8_t * base, size_t m_size  )
	{
		auto const p_ptr	= i_mem::search_pattern( a_pattern, size, wild_card, true, base, m_size );
		return get_offset<T>( p_ptr, index );
	}

	template <typename T>
	uint32_t scan_offset_index( uint8_t* a_pattern, const uint32_t size, const uint32_t index )
	{
		const auto h_main	= GetModuleHandle( nullptr );
		const auto h_size	= i_mem::get_module_size( h_main );
		auto const p_ptr	= i_mem::search_pattern( a_pattern, size, 0xCC, true, h_main, h_size );
		return get_offset<T>( p_ptr, index );
	}

	template<typename T>
	T relative_address( T instruction, uint32_t index_offset, uint32_t instruction_size )
	{
		const auto p_inst = reinterpret_cast<void*>( instruction );
		if ( nullptr == p_inst || !i_mem::is_valid_read( p_inst ) )
			return nullptr;
		auto er_ip_offset = *reinterpret_cast<uint32_t*>( instruction + index_offset );
		return reinterpret_cast<T>( instruction + instruction_size + er_ip_offset );
	}

}