#pragma optimize("s",on)
// std::_Fill<pair<unsigned long, CSoundPair>*, pair<...> > @ 0x00578c0f
// Fills [_First, _Last) with _Val. _Ty is a 12-byte POD.

struct CSoundPair {
    unsigned long a;
    unsigned long b;
};

struct SoundEntry {
    unsigned long first;
    CSoundPair second;
};

// MSVC STL _Fill: for (; _First != _Last; ++_First) *_First = _Val;
// ecx = _First, edx = _Last, stack = &_Val (const ref).
void __fastcall _Fill_SoundEntry(SoundEntry* _First, SoundEntry* _Last, const SoundEntry& _Val)
{
    for (; _First != _Last; ++_First)
        *_First = _Val;
}