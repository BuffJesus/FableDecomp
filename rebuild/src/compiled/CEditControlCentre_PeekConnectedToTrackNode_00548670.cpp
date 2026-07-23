// CEditControlCentre::PeekConnectedToTrackNode @ 0x00548670
// Retail: 8a410c 84c0 7407 8b81f4000000 c3 8b4138 c3
//   mov  al, byte ptr [ecx+0x0c]
//   test al, al
//   je   .else
//   mov  eax, [ecx+0xf4]   ; connected node
//   ret
// .else:
//   mov  eax, [ecx+0x38]   ; default track node
//   ret
//
// Modeled as __fastcall so the object pointer arrives in ecx (byte-identical
// to __fastcall for a this-only accessor). VC7.1 / MSVC 2003, no C++11.

struct CThingTrackNode;

// Layout: a byte flag at +0x0c selects which pointer field to return.
struct CEditControlCentre {
    char _pad0[0x0c];               // 0x00 .. 0x0b
    unsigned char use_connected;    // 0x0c  selector flag
    char _pad1[0x38 - 0x0d];        // 0x0d .. 0x37
    CThingTrackNode* default_node;  // 0x38
    char _pad2[0xf4 - 0x3c];        // 0x3c .. 0xf3
    CThingTrackNode* connected_node;// 0xf4
};

CThingTrackNode* __fastcall CEditControlCentre_PeekConnectedToTrackNode(CEditControlCentre* self)
{
    if (self->use_connected)
        return self->connected_node;
    return self->default_node;
}