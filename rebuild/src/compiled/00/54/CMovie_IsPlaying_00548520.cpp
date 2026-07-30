// CMovie::IsPlaying @ 0x00548520
// mov al, [ecx+0x15c]; ret
struct CMovie {
    char pad[0x15c];
    unsigned char m_bIsPlaying;
};

unsigned char __fastcall IsPlaying(CMovie *this_)
{
    return this_->m_bIsPlaying;
}