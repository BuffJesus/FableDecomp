// STLport _Rb_tree<long, pair<const long, NProfileTimer::CProfilingInfo>, ...>::clear() @ 009d84a0 (51 bytes)
//
// The tree behind NProfileTimer's map<long, CProfilingInfo> (Ego_r: the
// NProfileTimer module, map<long,CProfilingInfo>::clear, sits just before
// MicroThread; retail keeps the same order: StartProfile 0x009D8240 /
// EndProfile 0x009D8250 compiled out, then this tree's ctor 0x009D8410,
// _M_erase 0x009D8460, clear 0x009D84A0, dtors 0x009D84E0/0x009D8570, then
// MicroThreadSleep 0x009D8650). Node is 0x28 bytes (0x10 base + 24-byte
// pair<const long, CProfilingInfo>, second at +8 for the double); _M_erase
// destroys only CurrentProfileName (CCharString at node+0x1C, ~CCharString
// 0x0099EAE0).
//
// No caller in retail (profiling compiled out); the instantiation survives.

#include "fable_stlport_tree.h"

namespace NProfileTimer
{
    // Ego_r PDB layout (16 bytes; the pair places it at +8 for the double).
    struct CProfilingInfo
    {
        bool Profiling;                  // +0x0
        CCharString CurrentProfileName;  // +0x4
        double ProfileStartTime;         // +0x8
    };
}

template void _STL::_Rb_tree<
    long,
    _STL::pair<const long, NProfileTimer::CProfilingInfo>,
    _STL::_Select1st<_STL::pair<const long, NProfileTimer::CProfilingInfo> >,
    _STL::less<long>,
    _STL::allocator<_STL::pair<const long, NProfileTimer::CProfilingInfo> > >::clear();
