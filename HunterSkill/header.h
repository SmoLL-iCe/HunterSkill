#pragma once
#include <windows.h>
#include <iostream>
#define r_cast reinterpret_cast
#define s_cast static_cast
#define d_cast dynamic_cast
#define c_cast const_cast
#define u8ptr r_cast<uint8_t*>