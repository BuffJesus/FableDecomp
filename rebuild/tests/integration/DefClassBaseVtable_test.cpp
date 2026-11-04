// ABI probe, never linked: compiled /Od so the inline CDefClassBase
// constructor stays out of line and emits the base vtable for comparison
// with retail 0x01230BCC.
#include "fable_def_class_base.h"
struct AbiDef : public CDefClassBase
{
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass() const;
};
CDefClassBase* MakeAbiDef() { return new AbiDef; }
