struct CHitLocationsDef {
    unsigned char _tail[0x34];
    void BaseCopy(const CHitLocationsDef* src);
};
struct CSub {
    void SubCopy(const CSub* src);
};

void __fastcall CHitLocationsDef_Copy(CHitLocationsDef* self, int /*edx*/, const CHitLocationsDef* src)
{
    self->BaseCopy(src);
    ((CSub*)((unsigned char*)self + 0x28))->SubCopy((const CSub*)((const unsigned char*)src + 0x28));
}