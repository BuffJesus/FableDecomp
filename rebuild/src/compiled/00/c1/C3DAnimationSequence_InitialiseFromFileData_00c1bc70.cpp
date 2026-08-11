// C3DAnimationSequence::InitialiseFromFileData  @ 0x00c1bc70
// Retail shape:
//   fld  dword ptr [esp+4]        ; float param
//   push esi
//   fmul dword ptr [0x12b322c]    ; * float literal in rdata
//   mov  esi, ecx                 ; this
//   call 0xbfea70                 ; _ftol-style: consume st0 -> eax
//   mov  word ptr [esi+0xe], ax   ; store low 16 bits
//   ret 4
//
// i.e. this->m_field_e = (short)(long)(param * SCALE);
// SCALE is a float literal (rdata memory operand of fmul), NOT an extern
// global -- an extern global makes VC7.1 fld the global first (wrong order).

struct C3DAnimationSequence
{
    char  pad0[0xe];
    short m_field_e;   // offset 0xe

    void InitialiseFromFileData(float t);
};

void C3DAnimationSequence::InitialiseFromFileData(float t)
{
    // 0x12b322c holds the retail float literal; value is reloc-masked for parity.
    m_field_e = (short)(long)(t * 1234.5f);
}