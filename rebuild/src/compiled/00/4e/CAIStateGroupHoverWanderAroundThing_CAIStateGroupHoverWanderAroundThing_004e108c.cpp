// Retail 0x004e108c : CAIStateGroup_HoverWanderAroundThing::~CAIStateGroup_HoverWanderAroundThing
//
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x174]; call 0x4df9cc     ; CActiveFile::OnReadFinished on member @ +0x174
//   mov [esi], 0x1230ba0                    ; store handler/def pointer into field0
//   mov ecx,esi; pop esi; jmp 0x9fc550      ; tail-call CMemoryBlock::~CMemoryBlock (base)
//
// The vtable-range constant 0x1230ba0 is written as an ORDINARY data-member
// assignment (field0), AFTER the member call — not a compiler vptr reset (which
// MSVC emits at entry). So this class is destroyed with a non-virtual dtor whose
// body is { m_file.OnReadFinished(); field0 = (T*)0x1230ba0; } and a base
// CMemoryBlock destroyed by tail-call.

// Member subobject at +0x174. Its OnReadFinished is the external routine 0x4df9cc.
struct CActiveFile {
    unsigned char body[4];
    CActiveFile();
    void OnReadFinished();
};

// Base class at offset 0; first field is a pointer written in the dtor body.
// Sized so the derived member lands exactly at +0x174. Non-virtual dtor (0x9fc550).
struct CMemoryBlock {
    unsigned int* field0;               // +0x00
    unsigned char pad[0x174 - 4];       // fill to 0x174
    CMemoryBlock();
    ~CMemoryBlock();
};

struct CAIStateGroup_HoverWanderAroundThing : public CMemoryBlock {
    CActiveFile m_file;                 // +0x174
    CAIStateGroup_HoverWanderAroundThing();
    ~CAIStateGroup_HoverWanderAroundThing();
};

CAIStateGroup_HoverWanderAroundThing::~CAIStateGroup_HoverWanderAroundThing()
{
    m_file.OnReadFinished();
    field0 = (unsigned int*)0x1230ba0;
}