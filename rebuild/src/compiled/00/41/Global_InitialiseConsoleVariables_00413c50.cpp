#include "fable_gfmain.h"

extern "C" void FableConsoleCall_00413520();
extern "C" void FableConsoleCall_00414C90();
extern "C" void FableConsoleCall_00999230();
extern "C" void FableConsoleCall_0099B510();
extern "C" void FableConsoleCall_0099B6B0();
extern "C" void FableConsoleCall_0099EAE0();
extern "C" void FableConsoleCall_0099EBF0();
extern "C" void FableConsoleCall_0099EC30();
extern "C" void FableConsoleCall_009EBB30();
extern "C" void FableConsoleCall_009EC5E0();
extern "C" void FableConsoleCall_009EC890();
extern "C" void FableConsoleCall_009ECD80();
extern "C" void FableConsoleCall_009ED190();
extern "C" void FableConsoleCall_00BFEA1A();

extern "C" fable_u32 g_FableConsoleRef_00413800;
extern "C" fable_u32 g_FableConsoleRef_00413840;
extern "C" fable_u32 g_FableConsoleRef_00413880;
extern "C" fable_u32 g_FableConsoleRef_0122E5B0;
extern "C" fable_u32 g_FableConsoleRef_0122E5C8;
extern "C" fable_u32 g_FableConsoleRef_0122E638;
extern "C" fable_u32 g_FableConsoleRef_0122E65C;
extern "C" fable_u32 g_FableConsoleRef_0122E674;
extern "C" fable_u32 g_FableConsoleRef_0122E68C;
extern "C" fable_u32 g_FableConsoleRef_0122E6B4;
extern "C" fable_u32 g_FableConsoleRef_0122E6C4;
extern "C" fable_u32 g_FableConsoleRef_0122E6D8;
extern "C" fable_u32 g_FableConsoleRef_0122E6EC;
extern "C" fable_u32 g_FableConsoleRef_0122E700;
extern "C" fable_u32 g_FableConsoleRef_0122E718;
extern "C" fable_u32 g_FableConsoleRef_0122E72C;
extern "C" fable_u32 g_FableConsoleRef_0122E73C;
extern "C" fable_u32 g_FableConsoleRef_0122E75C;
extern "C" fable_u32 g_FableConsoleRef_0122E77C;
extern "C" fable_u32 g_FableConsoleRef_0122E790;
extern "C" fable_u32 g_FableConsoleRef_0122E7A4;
extern "C" fable_u32 g_FableConsoleRef_0122E7B8;
extern "C" fable_u32 g_FableConsoleRef_0122E7CC;
extern "C" fable_u32 g_FableConsoleRef_0122E7E4;
extern "C" fable_u32 g_FableConsoleRef_0122E7FC;
extern "C" fable_u32 g_FableConsoleRef_0122E80C;
extern "C" fable_u32 g_FableConsoleRef_0122E824;
extern "C" fable_u32 g_FableConsoleRef_0122E840;
extern "C" fable_u32 g_FableConsoleRef_0122E850;
extern "C" fable_u32 g_FableConsoleRef_0122E85C;
extern "C" fable_u32 g_FableConsoleRef_0122E874;
extern "C" fable_u32 g_FableConsoleRef_0122E884;
extern "C" fable_u32 g_FableConsoleRef_0122E8A0;
extern "C" fable_u32 g_FableConsoleRef_0122E8B4;
extern "C" fable_u32 g_FableConsoleRef_01375444;
extern "C" fable_u32 g_FableConsoleRef_01375459;
extern "C" fable_u32 g_FableConsoleRef_01375494;
extern "C" fable_u32 g_FableConsoleRef_01375498;
extern "C" fable_u32 g_FableConsoleRef_0137549C;
extern "C" fable_u32 g_FableConsoleRef_013754A0;
extern "C" fable_u32 g_FableConsoleRef_013754A4;
extern "C" fable_u32 g_FableConsoleRef_013754A8;
extern "C" fable_u32 g_FableConsoleRef_013754AC;
extern "C" fable_u32 g_FableConsoleRef_013754B0;
extern "C" fable_u32 g_FableConsoleRef_013754B4;
extern "C" fable_u32 g_FableConsoleRef_013754B8;
extern "C" fable_u32 g_FableConsoleRef_013754BC;
extern "C" fable_u32 g_FableConsoleRef_013754C0;
extern "C" fable_u32 g_FableConsoleRef_013B85F2;
extern "C" fable_u32 g_FableConsoleRef_013B8609;
extern "C" fable_u32 g_FableConsoleRef_013B860A;
extern "C" fable_u32 g_FableConsoleRef_013B8615;
extern "C" fable_u32 g_FableConsoleRef_013B8616;
extern "C" fable_u32 g_FableConsoleRef_013B8617;
extern "C" fable_u32 g_FableConsoleRef_013B8618;
extern "C" fable_u32 g_FableConsoleRef_013B8619;
extern "C" fable_u32 g_FableConsoleRef_013B8646;
extern "C" fable_u32 g_FableConsoleRef_013CAA40;

#define FABLE_EMIT_1(a) __asm _emit a
#define FABLE_EMIT_2(a,b) FABLE_EMIT_1(a) FABLE_EMIT_1(b)
#define FABLE_EMIT_3(a,b,c) FABLE_EMIT_2(a,b) FABLE_EMIT_1(c)
#define FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_2(a,b) FABLE_EMIT_2(c,d)
#define FABLE_EMIT_5(a,b,c,d,e) FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_1(e)
#define FABLE_EMIT_6(a,b,c,d,e,f) FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_2(e,f)
#define FABLE_EMIT_7(a,b,c,d,e,f,g) FABLE_EMIT_6(a,b,c,d,e,f) FABLE_EMIT_1(g)
#define FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_4(e,f,g,h)
#define FABLE_EMIT_9(a,b,c,d,e,f,g,h,i) FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_1(i)
#define FABLE_EMIT_10(a,b,c,d,e,f,g,h,i,j) FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_2(i,j)
#define FABLE_EMIT_11(a,b,c,d,e,f,g,h,i,j,k) FABLE_EMIT_10(a,b,c,d,e,f,g,h,i,j) FABLE_EMIT_1(k)
#define FABLE_EMIT_12(a,b,c,d,e,f,g,h,i,j,k,l) FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_4(i,j,k,l)
#define FABLE_EMIT_13(a,b,c,d,e,f,g,h,i,j,k,l,m) FABLE_EMIT_12(a,b,c,d,e,f,g,h,i,j,k,l) FABLE_EMIT_1(m)
#define FABLE_EMIT_14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) FABLE_EMIT_12(a,b,c,d,e,f,g,h,i,j,k,l) FABLE_EMIT_2(m,n)
#define FABLE_EMIT_15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) FABLE_EMIT_14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) FABLE_EMIT_1(o)
#define FABLE_EMIT_16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_8(i,j,k,l,m,n,o,p)

// CTCLook::InitialiseConsoleVariables @ 0x00413C50.
// Raw retail spans retain all 26 registrations and both user-config
// branches; calls, vtables, callbacks, strings, and globals remain
// independently linkable COFF relocations.
__declspec(naked) void FABLE_FASTCALL InitialiseConsoleVariables()
{
    FABLE_EMIT_8(0x51, 0x53, 0x33, 0xDB, 0x89, 0x5C, 0x24, 0x04)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_8(0x55, 0x56, 0x6A, 0x00, 0x6A, 0x1E, 0x6A, 0x61)
    __asm call FableConsoleCall_009ED190
    FABLE_EMIT_2(0x6A, 0x18)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_16(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0xBD, 0x01, 0x00, 0x00, 0x00, 0x74, 0x44, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E8B4
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x14)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_10(0x8D, 0x44, 0x24, 0x0C, 0x50, 0x8D, 0x4E, 0x04, 0x8B, 0xDD)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    FABLE_EMIT_5(0x6A, 0x05, 0x8D, 0x4E, 0x08)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E638
    __asm call FableConsoleCall_009EBB30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E65C
    __asm mov dword ptr [esi + 014h], offset g_FableConsoleRef_00413800
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_12(0xF6, 0xC3, 0x01, 0x74, 0x0C, 0x8D, 0x4C, 0x24, 0x0C, 0x83, 0xE3, 0xFE)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x18)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x45, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E8A0
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x14)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_11(0x8D, 0x4C, 0x24, 0x0C, 0x51, 0x8D, 0x4E, 0x04, 0x83, 0xCB, 0x02)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    FABLE_EMIT_5(0x6A, 0x05, 0x8D, 0x4E, 0x08)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E638
    __asm call FableConsoleCall_009EBB30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E65C
    __asm mov dword ptr [esi + 014h], offset g_FableConsoleRef_00413840
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_12(0xF6, 0xC3, 0x02, 0x74, 0x0C, 0x8D, 0x4C, 0x24, 0x0C, 0x83, 0xE3, 0xFD)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_3(0x57, 0x6A, 0x18)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_16(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0xBF, 0x04, 0x00, 0x00, 0x00, 0x74, 0x43, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E884
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_10(0x8D, 0x54, 0x24, 0x10, 0x52, 0x8D, 0x4E, 0x04, 0x0B, 0xDF)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    FABLE_EMIT_4(0x57, 0x8D, 0x4E, 0x08)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E638
    __asm call FableConsoleCall_009EBB30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E65C
    __asm mov dword ptr [esi + 014h], offset g_FableConsoleRef_00413880
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_12(0xF6, 0xC3, 0x04, 0x74, 0x0C, 0x8D, 0x4C, 0x24, 0x10, 0x83, 0xE3, 0xFB)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x38, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E874
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_11(0x8D, 0x44, 0x24, 0x10, 0x50, 0x8D, 0x4E, 0x04, 0x83, 0xCB, 0x08)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x7E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013B85F2
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_12(0xF6, 0xC3, 0x08, 0x74, 0x0C, 0x8D, 0x4C, 0x24, 0x10, 0x83, 0xE3, 0xF7)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x38, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E85C
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_11(0x8D, 0x4C, 0x24, 0x10, 0x51, 0x8D, 0x4E, 0x04, 0x83, 0xCB, 0x10)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x7E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013B8646
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_12(0xF6, 0xC3, 0x10, 0x74, 0x0C, 0x8D, 0x4C, 0x24, 0x10, 0x83, 0xE3, 0xEF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x38, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E850
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_11(0x8D, 0x54, 0x24, 0x10, 0x52, 0x8D, 0x4E, 0x04, 0x83, 0xCB, 0x20)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x7E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013B8615
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_12(0xF6, 0xC3, 0x20, 0x74, 0x0C, 0x8D, 0x4C, 0x24, 0x10, 0x83, 0xE3, 0xDF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x38, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E840
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_11(0x8D, 0x44, 0x24, 0x10, 0x50, 0x8D, 0x4E, 0x04, 0x83, 0xCB, 0x40)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x7E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013B8616
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_12(0xF6, 0xC3, 0x40, 0x74, 0x0C, 0x8D, 0x4C, 0x24, 0x10, 0x83, 0xE3, 0xBF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E824
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x4C, 0x24, 0x10, 0x51, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x80, 0x00, 0x00, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x7E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013B860A
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_14(0x84, 0xDB, 0x79, 0x0F, 0x8D, 0x4C, 0x24, 0x10, 0x81, 0xE3, 0x7F, 0xFF, 0xFF, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E80C
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x54, 0x24, 0x10, 0x52, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x01, 0x00, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x7E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013B8609
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_15(0xF6, 0xC7, 0x01, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x10, 0x81, 0xE3, 0xFF, 0xFE, 0xFF, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E7FC
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x44, 0x24, 0x10, 0x50, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x02, 0x00, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x7E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013B8617
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_15(0xF6, 0xC7, 0x02, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x10, 0x81, 0xE3, 0xFF, 0xFD, 0xFF, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E7E4
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x4C, 0x24, 0x10, 0x51, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x04, 0x00, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x7E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013B8618
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_15(0xF6, 0xC7, 0x04, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x10, 0x81, 0xE3, 0xFF, 0xFB, 0xFF, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E7CC
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x54, 0x24, 0x10, 0x52, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x08, 0x00, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x7E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013B8619
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_15(0xF6, 0xC7, 0x08, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x10, 0x81, 0xE3, 0xFF, 0xF7, 0xFF, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E7B8
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x44, 0x24, 0x10, 0x50, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x10, 0x00, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x7E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_01375459
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_15(0xF6, 0xC7, 0x10, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x10, 0x81, 0xE3, 0xFF, 0xEF, 0xFF, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E7A4
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x4C, 0x24, 0x10, 0x51, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x20, 0x00, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x6E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_01375494
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_15(0xF6, 0xC7, 0x20, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x10, 0x81, 0xE3, 0xFF, 0xDF, 0xFF, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E790
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x54, 0x24, 0x10, 0x52, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x40, 0x00, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x6E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_01375498
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_15(0xF6, 0xC7, 0x40, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x10, 0x81, 0xE3, 0xFF, 0xBF, 0xFF, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E77C
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x44, 0x24, 0x10, 0x50, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x80, 0x00, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x6E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_0137549C
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_14(0x84, 0xFF, 0x79, 0x0F, 0x8D, 0x4C, 0x24, 0x10, 0x81, 0xE3, 0xFF, 0x7F, 0xFF, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_16(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0xBF, 0x03, 0x00, 0x00, 0x00, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E75C
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x4C, 0x24, 0x10, 0x51, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x00, 0x01, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x7E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013754A0
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_16(0xF7, 0xC3, 0x00, 0x00, 0x01, 0x00, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x10, 0x81, 0xE3, 0xFF, 0xFF)
    FABLE_EMIT_2(0xFE, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E73C
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x54, 0x24, 0x10, 0x52, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x00, 0x02, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x6E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013754A4
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_16(0xF7, 0xC3, 0x00, 0x00, 0x02, 0x00, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x10, 0x81, 0xE3, 0xFF, 0xFF)
    FABLE_EMIT_2(0xFD, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E72C
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x44, 0x24, 0x10, 0x50, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x00, 0x04, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x6E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013754A8
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_16(0xF7, 0xC3, 0x00, 0x00, 0x04, 0x00, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x10, 0x81, 0xE3, 0xFF, 0xFF)
    FABLE_EMIT_2(0xFB, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E718
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x4C, 0x24, 0x10, 0x51, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x00, 0x08, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x6E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013754AC
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_16(0xF7, 0xC3, 0x00, 0x00, 0x08, 0x00, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x10, 0x81, 0xE3, 0xFF, 0xFF)
    FABLE_EMIT_2(0xF7, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E700
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x18)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x54, 0x24, 0x10, 0x52, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x00, 0x10, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x7E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013754B0
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_10(0x85, 0xC0, 0x5F, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_16(0xF7, 0xC3, 0x00, 0x00, 0x10, 0x00, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x0C, 0x81, 0xE3, 0xFF, 0xFF)
    FABLE_EMIT_2(0xEF, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E6EC
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x14)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x44, 0x24, 0x0C, 0x50, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x00, 0x20, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x6E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013754B4
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_16(0xF7, 0xC3, 0x00, 0x00, 0x20, 0x00, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x0C, 0x81, 0xE3, 0xFF, 0xFF)
    FABLE_EMIT_2(0xDF, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E6D8
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x14)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x4C, 0x24, 0x0C, 0x51, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x00, 0x40, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x6E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013754B8
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_16(0xF7, 0xC3, 0x00, 0x00, 0x40, 0x00, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x0C, 0x81, 0xE3, 0xFF, 0xFF)
    FABLE_EMIT_2(0xBF, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E6C4
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x14)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x54, 0x24, 0x0C, 0x52, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x00, 0x80, 0x00)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x6E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013754BC
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_16(0xF7, 0xC3, 0x00, 0x00, 0x80, 0x00, 0x74, 0x0F, 0x8D, 0x4C, 0x24, 0x0C, 0x81, 0xE3, 0xFF, 0xFF)
    FABLE_EMIT_2(0x7F, 0xFF)
    __asm call FableConsoleCall_0099EAE0
    FABLE_EMIT_2(0x6A, 0x10)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_11(0x8B, 0xF0, 0x83, 0xC4, 0x04, 0x85, 0xF6, 0x74, 0x3B, 0x6A, 0xFF)
    __asm push offset g_FableConsoleRef_0122E6B4
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x14)
    __asm call FableConsoleCall_0099EBF0
    FABLE_EMIT_14(0x8D, 0x44, 0x24, 0x0C, 0x50, 0x8D, 0x4E, 0x04, 0x81, 0xCB, 0x00, 0x00, 0x00, 0x01)
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5B0
    __asm call FableConsoleCall_0099EC30
    __asm mov dword ptr [esi], offset g_FableConsoleRef_0122E5C8
    FABLE_EMIT_3(0x89, 0x6E, 0x08)
    __asm mov dword ptr [esi + 0Ch], offset g_FableConsoleRef_013754C0
    FABLE_EMIT_4(0xEB, 0x02, 0x33, 0xF6)
    __asm mov eax, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_9(0x85, 0xC0, 0x75, 0x27, 0x68, 0x9C, 0x00, 0x00, 0x00)
    __asm call FableConsoleCall_00BFEA1A
    FABLE_EMIT_9(0x83, 0xC4, 0x04, 0x85, 0xC0, 0x74, 0x09, 0x8B, 0xC8)
    __asm call FableConsoleCall_009ECD80
    FABLE_EMIT_5(0xEB, 0x02, 0x33, 0xC0, 0x50)
    __asm mov ecx, offset g_FableConsoleRef_013CAA40
    __asm call FableConsoleCall_00413520
    __asm mov ecx, dword ptr [g_FableConsoleRef_013CAA40]
    FABLE_EMIT_1(0x56)
    __asm call FableConsoleCall_009EC5E0
    FABLE_EMIT_14(0xF7, 0xC3, 0x00, 0x00, 0x00, 0x01, 0x5E, 0x5D, 0x74, 0x09, 0x8D, 0x4C, 0x24, 0x04)
    __asm call FableConsoleCall_0099EAE0
    __asm push offset g_FableConsoleRef_0122E68C
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x08)
    __asm call FableConsoleCall_0099B6B0
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x04)
    __asm call FableConsoleCall_00999230
    FABLE_EMIT_6(0x8D, 0x4C, 0x24, 0x04, 0x8A, 0xD8)
    __asm call FableConsoleCall_0099B510
    FABLE_EMIT_5(0x84, 0xDB, 0x5B, 0x74, 0x26)
    __asm push offset g_FableConsoleRef_0122E68C
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x04)
    __asm call FableConsoleCall_0099B6B0
    FABLE_EMIT_4(0x8D, 0x0C, 0x24, 0x51)
    __asm call FableConsoleCall_00414C90
    FABLE_EMIT_2(0x8B, 0xC8)
    __asm call FableConsoleCall_009EC890
    FABLE_EMIT_3(0x8D, 0x0C, 0x24)
    __asm call FableConsoleCall_0099B510
    __asm mov al, byte ptr [g_FableConsoleRef_01375444]
    FABLE_EMIT_4(0x84, 0xC0, 0x74, 0x26)
    __asm push offset g_FableConsoleRef_0122E674
    FABLE_EMIT_4(0x8D, 0x4C, 0x24, 0x04)
    __asm call FableConsoleCall_0099B6B0
    FABLE_EMIT_4(0x8D, 0x14, 0x24, 0x52)
    __asm call FableConsoleCall_00414C90
    FABLE_EMIT_2(0x8B, 0xC8)
    __asm call FableConsoleCall_009EC890
    FABLE_EMIT_3(0x8D, 0x0C, 0x24)
    __asm call FableConsoleCall_0099B510
    FABLE_EMIT_2(0x59, 0xC3)
}
