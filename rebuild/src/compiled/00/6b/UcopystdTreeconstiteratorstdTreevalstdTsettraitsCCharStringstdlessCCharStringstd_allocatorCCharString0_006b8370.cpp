// _Ucopy forwarder. __fastcall, this=vector* (ecx). 3 stack params (ret 0xc).
// Forwards to a helper subobject at this+0x90, reordering the args.
struct CCharString;

struct TreeIter { void* _Ptr; };

struct CopyHelper {
    // callee at this+0x90; __fastcall(self, first, dest, last) -> CCharString*
    CCharString* Do(TreeIter first, CCharString* dest, TreeIter last);
};

struct VecObj {
    char _pad[0x90];
    CopyHelper helper;   // at offset 0x90
    CCharString* _Ucopy(TreeIter first, TreeIter last, CCharString* dest);
};

CCharString* VecObj::_Ucopy(TreeIter first, TreeIter last, CCharString* dest)
{
    return helper.Do(first, dest, last);
}