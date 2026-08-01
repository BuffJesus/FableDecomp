// _Destroy_range<std::allocator<CSpeechFrame> >  @ retail 0x0047bca0
//
// This is the MSVC7.1 std::_Destroy_range helper specialised for a 12-byte
// element type (CSpeechFrame) whose only non-trivial sub-object is a CCharString
// at offset +4.  It walks [_First,_Last) and destroys each element in place.
//
// The retail 61-byte oracle slice OVER-CAPTURES: the real function is only the
// leading 34 bytes (0x00-0x21, ending at the `ret` at 0x21).  After it come 14
// int3 alignment fillers (0x22-0x2f) and then a completely separate adjacent
// helper (0x30-0x3c).  Those are NOT part of this symbol, so real_len = 34 and
// only routine 1 is reconstructed here.
//
// Retail routine 1 disasm:
//   push esi; push edi            ; _First/_Last live across the dtor call -> saved
//   mov  edi,edx                  ; _Last  (edx)
//   mov  esi,ecx                  ; _First (ecx)
//   cmp  esi,edi; je done         ; empty range -> nothing to destroy
//   lea  ebx,[ebx+0x0]            ; 6-byte /O2 loop-head alignment NOP (head @ 0x10)
// loop:
//   lea  ecx,[esi+4]             ; &_First->text  (member sub-object at +4)
//   call CCharString::~CCharString (masked, __fastcall, ecx=this)
//   add  esi,0xc                 ; ++_First  (sizeof(CSpeechFrame) == 12)
//   cmp  esi,edi; jne loop
// done:
//   pop edi; pop esi; ret         ; ret 0 (only the two register args are used)
//
// Genuine-C++ recipe (mirrors landed _Fill_n 0x0047b630 / _Copy_backward
// 0x00485372):
//   * The `_First != _Last` pointer compare is what makes cl emit the `je`
//     entry guard (byte 0x8 = 74) and the `jne` back-edge (0x1d = 75), and it
//     enregisters _First/_Last into the callee-saved esi/edi (hence push
//     esi/push edi) because they must survive the out-of-line dtor call.
//   * CCharString::~CCharString is declared but NOT defined here, so cl lowers
//     the member destruction to a single relocation-masked `call` with the
//     sub-object address (this+4) in ecx -> exactly `lea ecx,[esi+4]; call`.
//   * ++_First on the 12-byte element type lowers to `add esi,0xc`.

struct CCharString {
    char* _ptr;   // +0x00 within CCharString
    // Out-of-line destructor -> forces a relocation-masked __fastcall call.
    ~CCharString();
};

struct CSpeechFrame {
    long         _id;    // +0x00
    CCharString  text;   // +0x04  (the only sub-object with a destructor)
    long         _data;  // +0x08   -> sizeof(CSpeechFrame) == 0x0c
};

// __fastcall: _First in ecx, _Last in edx.  Only the two iterators are touched,
// so the epilogue is a plain `ret` (no stack cleanup).
void __fastcall _Destroy_range_CSpeechFrame(CSpeechFrame* _First, CSpeechFrame* _Last)
{
    // Pointer-inequality loop: destroy each element's CCharString sub-object,
    // then advance to the next 12-byte element.
    for (; _First != _Last; ++_First)
        _First->text.~CCharString();
}