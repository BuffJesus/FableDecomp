// CIEngine::SetSeaBodyToRender(CCharString) -- retail 0x00b23610
//
// Retail bytes: 8d 4c 24 04 | e8 c7 b4 e7 ff | c2 04 00
//   lea ecx,[esp+4]      -- address of the by-value CCharString parameter
//   call CCharString::~CCharString   (rel32 masked; target retail 0x0099eae0)
//   ret 4                -- thiscall stack cleanup of the one by-value arg
//
// This is the standard MSVC epilogue shape for a thiscall member function whose
// only by-value parameter needs cleanup and whose body is otherwise empty (the
// function does nothing with the string other than accept-and-discard it, or
// the meaningful work was fully optimized away, leaving just the temporary's
// destructor call before the ordinary ret). 'this' (ecx on entry) is unused,
// consistent with the retail bytes never touching the incoming ecx register
// after the lea overwrites it.
//
// CCharString: a 4-byte-handle type whose destructor is DECLARED but not
// DEFINED in this TU, so the compiler must emit a genuine out-of-line
// (relocation-masked) call rather than inlining a body -- exactly matching
// the retail `call` instruction. This mirrors the landed sibling pattern in
// rebuild/src/compiled/00/47/vectorstdpairCCharString_Destroy_0047baf0.cpp.
class CCharString {
public:
    ~CCharString();   // out-of-line -> masked call (retail target 0x0099eae0)
private:
    void* _storage;   // 4-byte handle
};

struct CIEngine {
    void SetSeaBodyToRender(CCharString bodyName);
};

void CIEngine::SetSeaBodyToRender(CCharString /*bodyName*/)
{
    // Body intentionally empty: the by-value parameter's implicit destructor
    // call at scope exit is the entire retail function.
}