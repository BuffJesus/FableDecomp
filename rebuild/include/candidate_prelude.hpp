#pragma once

// Common language/runtime surface for syntax-checking auto-RE candidates.
// Engine declarations intentionally do not live here: a candidate only clears
// the compile gate after it has a real module header with real declarations.
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <map>
#include <memory>
#include <new>
#include <utility>
#include <vector>

#if !defined(_MSC_VER)
#  define __cdecl
#  define __stdcall
#  define __thiscall
#  define __fastcall
#endif

typedef unsigned char uchar;

void operator_delete(void* pointer);
