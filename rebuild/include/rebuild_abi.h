#pragma once

// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __thiscall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

// Per-unit inline policy. Some retail objects expand small inline helpers on
// paths where VC7.1 keeps them out of line (conditional calls under /O2, and
// most inline calls under /Os): the map<CCharString, long> insert unit
// (insert_unique 0x00431606, _M_insert 0x004316CF, _M_create_node 0x00431773)
// and vector<CWideString>::operator= 0x00433DF0. Only __forceinline
// reproduces that, so those landings define FABLE_UNIT_INLINE as
// __forceinline before their first include; everywhere else it is inline.
#ifndef FABLE_UNIT_INLINE
#  define FABLE_UNIT_INLINE inline
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_static_assert_, __LINE__)[(expression) ? 1 : -1]
