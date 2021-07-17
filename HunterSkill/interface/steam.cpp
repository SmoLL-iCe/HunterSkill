#include "../header.h"
#include "steam.h"
#include "../utils/mem.h"

/*
gameoverlayrenderer64.dll+88E7E - 48 FF 25 93571000     - jmp qword ptr [gameoverlayrenderer64.dll+18E618]
gameoverlayrenderer64.dll+88E85 - 48 89 5C 24 30        - mov [rsp+30],rbx
gameoverlayrenderer64.dll+88E8A - 48 8D 0D AF4F1000     - lea rcx,[gameoverlayrenderer64.dll+18DE40]
*/
uint8_t shell_code[ ] = {
	//  gameoverlayrenderer64.dll+88E7E =>	jmp qword ptr [gameoverlayrenderer64.dll+18E618]
	0x48, 0xFF, 0x25, 0xCC, 0xCC, 0xCC, 0xCC,
	//  gameoverlayrenderer64.dll+88E85 =>	mov [rsp+30],rbx
	0x48, 0x89, 0x5C, 0x24, 0x30,
	//  gameoverlayrenderer64.dll+88E8A =>	lea rcx,[gameoverlayrenderer64.dll+18DE40]
	0x48, 0x8D, 0x0D
};

uintptr_t* steam_overlay::get_ptr( )
{
	HMODULE gameoverlayrenderer64 = nullptr;
	while ( !gameoverlayrenderer64 )
	{
		gameoverlayrenderer64 = GetModuleHandle( L"gameoverlayrenderer64.dll" );
	}

	auto found = mem::find_pattern( shell_code, sizeof shell_code, gameoverlayrenderer64 );

	if ( !found ) return nullptr;

	auto ptr = reinterpret_cast<uintptr_t*>( found + *r_cast<uint32_t*>( &found[ 3 ] ) + 7 );

	while ( !*ptr )
	{
		Sleep( 100 );
	}

	return ptr;
}
