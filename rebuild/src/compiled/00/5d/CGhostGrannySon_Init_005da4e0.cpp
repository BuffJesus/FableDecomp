// CGhostGrannySon::Init  @ 0x005da4e0
// Sets [ecx+0x14] = -1 (int), [ecx+0x18] = 0 (bool), ret.  __fastcall (this=ecx), no stack args.
struct CGhostGrannySon {
    char pad[0x14];
    int  m_index;   // +0x14
    bool m_flag;    // +0x18
};

void __fastcall Init(CGhostGrannySon *this_) {
    this_->m_index = -1;
    this_->m_flag  = false;
}