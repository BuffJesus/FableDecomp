#include <stdio.h>

struct Entry8 { int a; int b; };
struct SpeakerRec { int index; int pad[3]; };

struct CScriptConversation {
    char pad0[8];
    Entry8*    entries;
    char pad2[8];
    SpeakerRec* speakers;
    char pad3[8];
    int        current;
};

// model of the tail callee: returns a distinguishable pointer derived from the entry
Entry8* g_lastArg = 0;
void* ResolveSpeaker(Entry8* e) { g_lastArg = e; return (void*)e; }

void* GetCurrentSpeaker(CScriptConversation* self)
{
    int idx = self->current;
    if (idx <= -1) return 0;
    return ResolveSpeaker(&self->entries[ self->speakers[idx].index ]);
}

int main()
{
    Entry8 entries[8];
    SpeakerRec speakers[4];
    CScriptConversation c;
    c.entries = entries;
    c.speakers = speakers;

    // case 1: current = -1 -> null
    c.current = -1;
    if (GetCurrentSpeaker(&c) != 0) { printf("FAIL neg\n"); return 1; }

    // case 2: current = 2, speakers[2].index = 5 -> &entries[5]
    c.current = 2;
    speakers[2].index = 5;
    void* r = GetCurrentSpeaker(&c);
    if (r != (void*)&entries[5]) { printf("FAIL idx\n"); return 1; }

    // case 3: current = 0, speakers[0].index = 0 -> &entries[0]
    c.current = 0;
    speakers[0].index = 0;
    if (GetCurrentSpeaker(&c) != (void*)&entries[0]) { printf("FAIL zero\n"); return 1; }

    printf("SPEAKER_OK\n");
    return 0;
}