// Lift target module #2: CMouseDX accessor surface (retail Fable.exe).
// A whole *class* lift (not a single function) — proves the harness scales from
// one function to a module. All 10 methods are pure `this`-offset field accessors
// with no external calls, so each body should compile byte-identical under VC7.1.
//
// The retail methods are virtual (UBE/UAE in the mangled names), but a method's
// BODY codegen is independent of its vtable slot, so we model them as ordinary
// non-virtual members: that keeps `this` at offset 0 (no vptr shift), so the raw
// member offsets below (0x3414, 0x343d.., 0x4848..) equal the retail ecx+disp.
//
// Compile 32-bit with VC7.1 (cl 13.10) and diff each function's .text against the
// retail bytes recorded per method. Build+diff: lift/scripts/build_module_diff.sh
//
// Retail decomp + bytes (from DumpFuncBytesAndC @ the addresses shown):
//   PeekPos        @00ab5f00 (7)  : return (C2DVector&)(this+0x3414)
//        8d 81 14 34 00 00 c3
//   SetInputOn     @00ab5f10 (13) : *(bool*)(this+0x343d) = arg
//        8a 44 24 04 88 81 3d 34 00 00 c2 04 00
//   SetRecentringOn@00ab5f20 (13) : *(bool*)(this+0x343e) = arg
//        8a 44 24 04 88 81 3e 34 00 00 c2 04 00
//   IsRecentringOn @00ab5f30 (7)  : return *(bool*)(this+0x343e)
//        8a 81 3e 34 00 00 c3
//   SetDrawCursor  @00ab5f40 (13) : *(bool*)(this+0x343f) = arg
//        8a 44 24 04 88 81 3f 34 00 00 c2 04 00
//   IsDrawingCursor@00ab5f50 (7)  : return *(bool*)(this+0x343f)
//        8a 81 3f 34 00 00 c3
//   IsLMBDown      @00ab5f70 (7)  : return *(bool*)(this+0x4848)
//        8a 81 48 48 00 00 c3
//   IsMMBDown      @00ab5f80 (7)  : return *(bool*)(this+0x4849)
//        8a 81 49 48 00 00 c3
//   IsRMBDown      @00ab5f90 (7)  : return *(bool*)(this+0x484a)
//        8a 81 4a 48 00 00 c3
//   GetPos         @00ab46e0 (24) : out[0]=(this+0x3414); out[1]=(this+0x3418)
//        8b 91 14 34 00 00 8b 44 24 04 89 10 8b 89 18 34 00 00 89 48 04 c2 04 00

struct C2DVector { float x; float y; };

class CMouseDX {
    unsigned char m_[0x4850]; // opaque state; only the offsets below are touched
public:
    const C2DVector& PeekPos() const;
    void GetPos(C2DVector* out) const;
    void SetInputOn(bool on);
    void SetRecentringOn(bool on);
    bool IsRecentringOn() const;
    void SetDrawCursor(bool on);
    bool IsDrawingCursor() const;
    bool IsLMBDown() const;
    bool IsMMBDown() const;
    bool IsRMBDown() const;
};

const C2DVector& CMouseDX::PeekPos() const {
    return *reinterpret_cast<const C2DVector*>(m_ + 0x3414);
}
void CMouseDX::GetPos(C2DVector* out) const {
    *out = *reinterpret_cast<const C2DVector*>(m_ + 0x3414);
}
void CMouseDX::SetInputOn(bool on)      { *reinterpret_cast<bool*>(m_ + 0x343d) = on; }
void CMouseDX::SetRecentringOn(bool on) { *reinterpret_cast<bool*>(m_ + 0x343e) = on; }
bool CMouseDX::IsRecentringOn() const   { return *reinterpret_cast<const bool*>(m_ + 0x343e); }
void CMouseDX::SetDrawCursor(bool on)   { *reinterpret_cast<bool*>(m_ + 0x343f) = on; }
bool CMouseDX::IsDrawingCursor() const  { return *reinterpret_cast<const bool*>(m_ + 0x343f); }
bool CMouseDX::IsLMBDown() const        { return *reinterpret_cast<const bool*>(m_ + 0x4848); }
bool CMouseDX::IsMMBDown() const        { return *reinterpret_cast<const bool*>(m_ + 0x4849); }
bool CMouseDX::IsRMBDown() const        { return *reinterpret_cast<const bool*>(m_ + 0x484a); }
