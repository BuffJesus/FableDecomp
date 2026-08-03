// 0x004457f0 CXMVPlaybackInfo
// mov byte ptr [ecx+0x894], 0 ; ret  -> __fastcall writing a byte field
struct CXMVPlaybackInfoObj {
    char pad[0x894];
    unsigned char flag;
};

void __fastcall CXMVPlaybackInfo(CXMVPlaybackInfoObj *self)
{
    self->flag = 0;
}