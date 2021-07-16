#include "mem.h"
#include "ntos.h"
/*
 * using NT for better performance.
 */

void* i_mem::get_module_handle(const wchar_t* w_module_name)
{
	uint32_t dw_offset = 0;
	PLIST_ENTRY p_user_mod_head = nullptr, p_user_mod = p_user_mod_head;
	PUNICODE_STRING p_unicode_mod;
#ifndef _WIN64
	uint32_t dw_peb = 0;
	_asm
	{
		pushad
		mov eax, fs: [48]
		mov dw_peb, eax
		popad
	}
	auto p_ldr_data = reinterpret_cast<PPEB_LDR_DATA>(  *reinterpret_cast<uintptr_t*>(dw_peb + 12)  );
#else
	auto* u_teb			= reinterpret_cast<uint8_t*>(__readgsqword(0x30));
	auto* p_ldr_data	= reinterpret_cast<PPEB_LDR_DATA>( *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(u_teb + 0x60)+ 0x18)  );
#endif  
	if (!p_ldr_data)
		return nullptr;
	for (auto i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			p_user_mod_head = p_user_mod = reinterpret_cast<PLIST_ENTRY>(&p_ldr_data->InLoadOrderModuleList);
			dw_offset = 0;
			break;

		case 1:
			p_user_mod_head = p_user_mod = reinterpret_cast<PLIST_ENTRY>(&p_ldr_data->InMemoryOrderModuleList);
#ifndef _WIN64
			dw_offset = 8;
#else
			dw_offset = 16;
#endif
			break;
		case 2:
			p_user_mod_head = p_user_mod = reinterpret_cast<PLIST_ENTRY>(&p_ldr_data->InInitializationOrderModuleList);
#ifndef _WIN64
			dw_offset = 16;
#else
			dw_offset = 32;
#endif
			break;
		default: ;
		}
		if (!p_user_mod)
			return nullptr;
		if ( !w_module_name )
		{
			p_user_mod = p_user_mod->Flink;
			const auto* p_ldr_mod = reinterpret_cast<PLDR_MODULE>( reinterpret_cast<uintptr_t>(p_user_mod) - dw_offset );
			return p_ldr_mod->pBaseAddress;
		}

		while (p_user_mod->Flink != p_user_mod_head)
		{
			p_user_mod = p_user_mod->Flink;
#ifndef _WIN64
			p_unicode_mod = reinterpret_cast<PUNICODE_STRING>(uintptr_t(p_user_mod) + (36 - dw_offset));
#else
			p_unicode_mod = reinterpret_cast<PUNICODE_STRING>(reinterpret_cast<uintptr_t>(p_user_mod) + (72 - dw_offset));
#endif
			//printf( "mod %ls\n", p_unicode_mod->Buffer );
			if ( wcswcs( p_unicode_mod->Buffer, w_module_name ) )
			{
				const auto* p_ldr_mod = reinterpret_cast<PLDR_MODULE>( reinterpret_cast<uintptr_t>(p_user_mod) - dw_offset);
				return p_ldr_mod->pBaseAddress;
			}
		}
	}
	return nullptr;
}


size_t i_mem::virtual_query( void* p_address, void* p_buffer, const size_t dw_length )
{
	size_t  return_len = 0;
	NtQueryVirtualMemory( NtCurrentProcess( ), p_address, MemoryBasicInformation, p_buffer, dw_length, &return_len );
	return return_len;
}

bool i_mem::virtual_protect( void* p_address, size_t number_of_bytes, uint32_t new_protect, uint32_t* old_protect )
{
	const auto status = NtProtectVirtualMemory( NtCurrentProcess( ), &p_address, &number_of_bytes, new_protect, reinterpret_cast<PULONG>(old_protect) );
	return ( status == 0 );
}

void* __stdcall i_mem::virtual_alloc( void* lp_address, const size_t dw_size, const uint32_t fl_allocation_type, const uint32_t fl_protect )
{
	auto* address		= lp_address;
	auto size			= dw_size;
	const auto status	= NtAllocateVirtualMemory( NtCurrentProcess( ), &address, 0, &size, fl_allocation_type, fl_protect );
	return  ( status == 0 ) ? address : nullptr;
}

bool __stdcall i_mem::virtual_free( void* lp_address, const size_t dw_size, const uint32_t dw_free_type )
{
	auto* address		= lp_address;
	auto size			= dw_size;
	const auto status	= NtFreeVirtualMemory( NtCurrentProcess( ), &address, &size, dw_free_type );
	return  ( status == 0 );
}

uint8_t* i_mem::search_pattern( const uint8_t* p_pattern, size_t size, const uint8_t wildcard, const bool use_wild_card, uint8_t* scan_start, const size_t scan_size )
{
	//DarthTon
	for (auto* ptr = scan_start; ptr < scan_start + scan_size - size; ++ptr )
	{
		for ( size_t i = 0; i < size; ++i )
			if ( use_wild_card )
			{
				if ( ptr[ i ] != p_pattern[ i ] && p_pattern[ i ] != wildcard )
					goto end;
			}
			else
			{
				if ( ptr[ i ] != p_pattern[ i ] )
					goto end;
			}
		return ptr;
	end:;
	}
	return nullptr;
}

bool i_mem::patch_mem( void* lp_address, const void* t_bytes, const size_t size )
{
	uint32_t old_p = 0;
	if ( virtual_protect( lp_address, size, PAGE_EXECUTE_READWRITE, &old_p ) )
	{
		memcpy( lp_address, t_bytes, size );
		virtual_protect( lp_address, size, old_p, &old_p );
		return true;
	}
	return false;
}

bool i_mem::set_mem( void* p_address, const uint8_t t_byte, const size_t size )
{
	uint32_t old_p = 0;
	if ( virtual_protect( p_address, size, PAGE_EXECUTE_READWRITE, &old_p ) )
	{
		memset( p_address, t_byte, size );
		virtual_protect( p_address, size, old_p, &old_p );
		return true;
	}
	return false;
}

DWORD readable = ( PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY | PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY );
DWORD writable = ( PAGE_EXECUTE_READWRITE | PAGE_READWRITE );
DWORD forbidden = ( PAGE_GUARD | PAGE_NOCACHE | PAGE_NOACCESS );


/*
 * // IsBadReadPtr or IsBadWritePtr
 * It is an obsolete function and should not be used.
 * Despite its name, it does not guarantee that the pointer is valid or that the memory pointed to is safe to use.
 */
bool i_mem::is_valid_write_obsolete( void* p )
{
	return ( !IsBadWritePtr( p, sizeof uintptr_t ) );
}

bool i_mem::is_valid_read_obsolete( void* p )
{
	return ( !IsBadReadPtr( p, sizeof uintptr_t ) );
}

bool i_mem::is_valid_write_safe( void* p, size_t size )
{
	MEMORY_BASIC_INFORMATION mbi = { };
	mbi.Protect = 0;

	auto ss = mbi.RegionSize - (reinterpret_cast<uintptr_t>( p ) - reinterpret_cast<uintptr_t>( mbi.BaseAddress ) );

	return ( virtual_query( p, &mbi, sizeof mbi ) && ( mbi.Protect & forbidden ) == 0 && ( mbi.Protect & writable ) != 0 && ss > size );
}

bool i_mem::is_valid_read_safe( void* p, size_t size  )
{
	MEMORY_BASIC_INFORMATION mbi = { };
	mbi.Protect = 0;
	//auto ss = mbi.RegionSize - ( reinterpret_cast<uintptr_t>( p ) - reinterpret_cast<uintptr_t>( mbi.BaseAddress ) );
	return ( virtual_query( p, &mbi, sizeof mbi ) && ( mbi.Protect & forbidden ) == 0 && ( mbi.Protect & readable ) != 0 /*&& ss > size*/ );
}

#ifdef ENV32BIT
void i_mem::hook_inst32( const uint8_t* p_detour, uint8_t*& p_address_original, const int size_hk )
{
	const auto p_address = reinterpret_cast<uint8_t*>( p_address_original );
	const uintptr_t dw_dis = ( ( p_detour - p_address ) - 0x5 );
	const auto to_jmp = reinterpret_cast<uint8_t*>( malloc( size_hk ) );
	if ( !to_jmp ) return;
	to_jmp[ 0 ] = 0xE9;
	*reinterpret_cast<uintptr_t*>( to_jmp + 1 ) = dw_dis;
	if ( size_hk > 5 )
		for ( auto i = 5; i < size_hk; i++ )
			to_jmp[ i ] = 0x90;
	uint32_t old_p = 0;
	if ( i_mem::virtual_protect( p_address, size_hk, PAGE_EXECUTE_READWRITE, &old_p ) )
	{
		p_address_original = reinterpret_cast<uint8_t*>( p_address + size_hk );
		memcpy( p_address, to_jmp, size_hk );
		i_mem::virtual_protect( p_address, size_hk, old_p, &old_p );
	}
	free( to_jmp );
}
#endif

bool i_mem::rpm_ptr( void* p_address, const std::vector<uintptr_t>& offsets, void* p_val, const size_t size )
{
	if ( !p_address || !is_valid_read( p_address ) ) return false;

	auto* tmp_address = static_cast<uint8_t*>( p_address );
	for (const auto& offset : offsets )
	{
		if ( tmp_address && is_valid_read( tmp_address ) )
			tmp_address = *reinterpret_cast<uint8_t**>( tmp_address ) + offset;
		else return false;
	}
	if ( !tmp_address || !is_valid_read( tmp_address ) ) return false;
	memcpy( p_val, tmp_address, size );
	return true;
}

bool i_mem::wpm_ptr( void* p_address, const std::vector<uintptr_t>& offsets, void* p_val, const size_t size )
{

	if ( !p_address || !p_val || !is_valid_read( p_address ) ) return false;
	auto* tmp_address = static_cast<uint8_t*>( p_address );
	for (const auto& offset : offsets )
	{
		if ( tmp_address && is_valid_read( tmp_address ) )
			tmp_address = *reinterpret_cast<uint8_t**>( tmp_address ) + offset;
		else return false;
	}
	return patch_mem( tmp_address, p_val, size );
}

bool i_mem::wpm( void* p_address, void* p_val, const size_t size )
{
	return patch_mem( p_address, p_val, size );
}

bool i_mem::rpm( void* p_address, void* p_val, const size_t size )
{
	if ( !p_address || !is_valid_read( p_address ) ) return false;
	memcpy( p_val, p_address, size );
	return true;
}

size_t i_mem::get_module_size( uint8_t* mod_base )
{
	if ( !mod_base ) return 0;
	auto* const img_dos_h	= reinterpret_cast<PIMAGE_DOS_HEADER>( mod_base );
	auto* const img_nt_h	= reinterpret_cast<PIMAGE_NT_HEADERS>( mod_base + img_dos_h->e_lfanew );
	return img_nt_h->OptionalHeader.SizeOfImage;
}

#ifdef ENV64BIT

template <typename T>
bool find_replace_val( const void** lp_local, uint8_t* to_find, T change_to, const size_t size_to_scan = 100 )
{
	const auto start_mem = reinterpret_cast<uint8_t*>( lp_local );
	uint32_t m_pro = 0;
	if ( !i_mem::virtual_protect( start_mem, size_to_scan, PAGE_EXECUTE_READWRITE, &m_pro ) )
		return false;
	T find_me = *reinterpret_cast<T*>( to_find );
	auto ret = false;
	for ( size_t i = 0; i < size_to_scan; i++ )
		if ( *reinterpret_cast<T*>( &start_mem[ i ] ) == find_me )
		{
			ret = true;
			*reinterpret_cast<T*>( &start_mem[ i ] ) = change_to;
			break;
		}
	i_mem::virtual_protect( start_mem, size_to_scan, m_pro, &m_pro );
	return ret;
}


bool masm_hook_s64( uint8_t* lp_original, const void** lp_detour, const size_t size_hk )
{
	///=================================================<Configure MASM>=================================================
	auto* u_detour = reinterpret_cast<uint8_t*>( lp_detour );
	uint32_t m_pro = 0;
	const auto trampoline = static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(lp_original) + size_hk);
	uint8_t find_me[ ] = { 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25 };
	//uint8_t findMe[] = { 0xB9, 0xFF, 0xFF, 0xFF, 0x1F, 0xB9, 0xFF, 0xFF, 0xFF, 0x1F, 0xB9, 0xFF, 0xFF, 0xFF, 0x1F };
	uint8_t jmp_abs[ ] = {
		// >   jmp CCCCCCCCCCCCCCCC
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x90 /*13*/
	};
	if ( !i_mem::virtual_protect( u_detour, 1000, PAGE_EXECUTE_READWRITE, &m_pro ) )
		return false;
	for ( size_t i = 0; i < 1000; i++ )
		if ( memcmp( reinterpret_cast<uint8_t*>( &u_detour[ i ] ), find_me, sizeof find_me ) == 0 )
		{
			*reinterpret_cast<uintptr_t*>( &u_detour[ i ] ) = trampoline;
			break;
		}
	i_mem::virtual_protect( u_detour, 1000, m_pro, &m_pro );
	///=================================================<Configure Hook>=================================================
	if ( !i_mem::virtual_protect( lp_original, size_hk, PAGE_EXECUTE_READWRITE, &m_pro ) )
		return false;
	auto* const alloc = static_cast<uint8_t*>( malloc( size_hk ) );
	if ( !alloc ) return false;
	if ( size_hk > 14 )
		for ( size_t i = 0; i < size_hk; i++ ) alloc[ i ] = 0x90;
	*reinterpret_cast<uintptr_t*>( &jmp_abs[ 6 ] ) = reinterpret_cast<uintptr_t>( u_detour );
	memcpy( alloc, jmp_abs, sizeof jmp_abs - 1 );
	memcpy( lp_original, alloc, size_hk );
	free( alloc );
	i_mem::virtual_protect( lp_original, size_hk, m_pro, &m_pro );
	return true;
}
#endif

