#include <stddef.h>

// Minimal CWideString stand-in: a single-pointer opaque handle. The
// candidate only ever passes pointers to it around and never reads its
// fields, so the exact retail layout does not matter here -- only the
// by-value-return / by-reference-argument calling convention that drives
// the surrounding codegen.
class CWideString
{
public:
    // Default constructor -- declared-only (never defined in this TU) so
    // the compiler emits a genuine out-of-line call for it, matching the
    // retail call site which constructs its scratch working buffer via a
    // real call rather than an inlined no-op.
    CWideString();
    ~CWideString();

private:
    wchar_t* m_data;
};

namespace
{
// __fastcall CWideString ExpandPath(CWideString* in, CWideString* scratch)
// Hidden return-value pointer occupies ecx (reused directly as the
// caller's own return slot via the tail-forward below), edx = source path
// pointer, one stack-pushed argument = a scratch CWideString buffer
// (materialised in place as ExpandPath's own temporary argument) used as
// working storage during expansion.
CWideString __fastcall ExpandPath(CWideString* in, CWideString* scratch);
}

// static CWideString __fastcall CAFile::ConvertPathToFullPath(const CWideString& path)
// Hidden return-value pointer occupies ecx (the static function has no
// 'this', so the by-value CWideString result slot takes ecx's place under
// __fastcall); the sole real parameter (a const CWideString&) occupies
// edx. The body is a thin forward: default-construct a scratch working
// buffer as ExpandPath's own temporary argument, then tail-return
// ExpandPath's by-value result directly into the caller's return slot (no
// separate named local, so no extra address recompute/copy/dtor).
class CAFile
{
public:
    static CWideString __fastcall ConvertPathToFullPath(const CWideString& path);
};

CWideString __fastcall CAFile::ConvertPathToFullPath(const CWideString& path)
{
    return ExpandPath(const_cast<CWideString*>(&path), &CWideString());
}