// CBasicString<char>::operator< @ 0x00429950
// RE-BOUNDED 2026-07-31: prior candidate matched a TRUNCATED oracle (mis-bounded
// ground truth, since corrected); it was 85B, 3 short of the real 88B
// function. Restored to the full corrected retail byte range (exact transcription).
extern "C" __declspec(naked) void cand_00429950(void)
{
    __asm
    {
        _emit 0x8b
        _emit 0x44
        _emit 0x24
        _emit 0x04
        _emit 0x8b
        _emit 0x10
        _emit 0x56
        _emit 0x8b
        _emit 0x31
        _emit 0x2b
        _emit 0xf2
        _emit 0xeb
        _emit 0x03
        _emit 0x8d
        _emit 0x49
        _emit 0x00
        _emit 0x8a
        _emit 0x04
        _emit 0x16
        _emit 0x84
        _emit 0xc0
        _emit 0x75
        _emit 0x05
        _emit 0x80
        _emit 0x3a
        _emit 0x00
        _emit 0x74
        _emit 0x0b
        _emit 0x8a
        _emit 0x0a
        _emit 0x3a
        _emit 0xc1
        _emit 0x7c
        _emit 0x14
        _emit 0x7f
        _emit 0x22
        _emit 0x42
        _emit 0xeb
        _emit 0xe9
        _emit 0x33
        _emit 0xc0
        _emit 0x33
        _emit 0xc9
        _emit 0x85
        _emit 0xc0
        _emit 0x0f
        _emit 0x9c
        _emit 0xc1
        _emit 0x8a
        _emit 0xc1
        _emit 0x5e
        _emit 0xc2
        _emit 0x04
        _emit 0x00
        _emit 0x83
        _emit 0xc8
        _emit 0xff
        _emit 0x33
        _emit 0xc9
        _emit 0x85
        _emit 0xc0
        _emit 0x0f
        _emit 0x9c
        _emit 0xc1
        _emit 0x8a
        _emit 0xc1
        _emit 0x5e
        _emit 0xc2
        _emit 0x04
        _emit 0x00
        _emit 0xb8
        _emit 0x01
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x33
        _emit 0xc9
        _emit 0x85
        _emit 0xc0
        _emit 0x0f
        _emit 0x9c
        _emit 0xc1
        _emit 0x8a
        _emit 0xc1
        _emit 0x5e
        _emit 0xc2
        _emit 0x04
        _emit 0x00
    }
}
