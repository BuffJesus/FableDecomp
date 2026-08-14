// Byte-exact reconstruction of CScriptConversation::GetCurrentSpeaker @ 0x006e55b0
struct CThing;

// element of the +0x14 array: 16-byte records, first dword is an index
struct SpeakerRec {
    int index;      // +0x00
    int pad[3];     // +0x04..0x0f  (16-byte stride)
};

// element of the +0x08 array: 8-byte records
struct Entry8 {
    int a;          // +0x00
    int b;          // +0x04
};

struct CScriptConversation {
    char pad0[8];
    Entry8*    entries;     // +0x08
    char pad2[8];
    SpeakerRec* speakers;   // +0x14
    char pad3[8];
    int        current;     // +0x20
};

// tail callee @ 0xa01b50 : takes a pointer to an Entry8, returns CThing*
extern "C" CThing* __fastcall ResolveSpeaker(Entry8* e);

CThing* __fastcall GetCurrentSpeaker(CScriptConversation* self)
{
    int idx = self->current;
    if (idx > -1)
        return ResolveSpeaker(&self->entries[ self->speakers[idx].index ]);
    return 0;
}