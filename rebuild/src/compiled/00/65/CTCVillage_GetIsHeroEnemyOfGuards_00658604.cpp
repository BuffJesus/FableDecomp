#pragma optimize("s",on)
// CTCVillage::GetIsHeroEnemyOfGuards @ 0x00658604 (retail, 28 bytes)
// VC7.1 / MSVC 2003, compile: cl /c /O2 /Oy /W3
//
// Retail disasm:
//   push  esi
//   mov   esi, ecx            ; esi = this
//   call  <FirstFunc>         ; result discarded
//   call  <SecondFunc>        ; eax = result
//   xor   ecx, ecx
//   cmp   eax, [esi+0xd4]     ; compare SecondFunc() with this->m_guardEnemyThreshold
//   pop   esi
//   setl  cl                 ; cl = (SecondFunc() < this->m_field) ? 1 : 0  (signed)
//   mov   al, cl
//   ret
//
// A this-only member accessor. Modeled as __fastcall so the object pointer
// arrives in ECX exactly like __fastcall. Two engine callees; the first's
// return value is discarded, the second's is the compared value.

// --- Engine callees (relocation-masked; need not resolve for parity) ---
extern int __cdecl CTCVillage_GetIsHeroEnemyOfGuards_Prep(void); // call A (discarded)
extern int __cdecl CTCVillage_GetIsHeroEnemyOfGuards_Query(void); // call B (compared)

// --- Inlined object layout with the load-bearing field at +0xd4 ---
struct CTCVillage
{
    char _pad0[0xd4];
    int  m_guardEnemyThreshold; // +0xd4  (signed compare via setl)
};

// __fastcall models __fastcall: 'this' arrives in ECX.
bool __fastcall CTCVillage_GetIsHeroEnemyOfGuards(CTCVillage* thisPtr)
{
    CTCVillage_GetIsHeroEnemyOfGuards_Prep();          // call A, result discarded
    return CTCVillage_GetIsHeroEnemyOfGuards_Query()   // call B, eax
           < thisPtr->m_guardEnemyThreshold;           // cmp eax,[esi+0xd4] ; setl
}