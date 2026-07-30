#include <stdio.h>

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

int main()
{
    CMovie m;
    unsigned char r;

    // case: playing = 1
    m.m_bIsPlaying = 1;
    {
        void *fn = (void*)&IsPlaying;
        void *pthis = &m;
        unsigned char out;
        __asm {
            mov ecx, pthis
            call fn
            mov out, al
        }
        r = out;
    }
    if (r != 1) { printf("FAIL playing=1 got %d\n", r); return 1; }

    // case: playing = 0
    m.m_bIsPlaying = 0;
    {
        void *fn = (void*)&IsPlaying;
        void *pthis = &m;
        unsigned char out;
        __asm {
            mov ecx, pthis
            call fn
            mov out, al
        }
        r = out;
    }
    if (r != 0) { printf("FAIL playing=0 got %d\n", r); return 1; }

    // case: arbitrary value passes through
    m.m_bIsPlaying = 0x2A;
    {
        void *fn = (void*)&IsPlaying;
        void *pthis = &m;
        unsigned char out;
        __asm {
            mov ecx, pthis
            call fn
            mov out, al
        }
        r = out;
    }
    if (r != 0x2A) { printf("FAIL playing=0x2A got %d\n", r); return 1; }

    printf("OK_0x00548520\n");
    return 0;
}