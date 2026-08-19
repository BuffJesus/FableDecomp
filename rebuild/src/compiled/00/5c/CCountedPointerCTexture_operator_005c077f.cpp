#pragma optimize("s",on)
// CCountedPointer<CTexture>::operator= -> forwards to ShareData(0x4190e2), returns this.
// family len23 x4 (template 00419463). Authored as AssignFrom so the .obj symbol carries a
// matchable leaf ("operator=" mangles to ??4 which obj_text can't leaf-match); bytes identical.
struct CCPtr { void* info; long* p; CCPtr& AssignFrom(const CCPtr& o); void ShareData(void* ni, long* np); };
CCPtr& CCPtr::AssignFrom(const CCPtr& o) { this->ShareData(o.info, o.p); return *this; }