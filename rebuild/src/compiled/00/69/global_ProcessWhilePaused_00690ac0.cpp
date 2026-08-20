// CInputProcessCutScene::ProcessWhilePaused — byte flag on a fixed global object.
#pragma pack(push,1)
struct G { char pad[0xf9]; unsigned char flag; };
#pragma pack(pop)
extern G* g_obj;
extern "C" unsigned char __fastcall ProcessWhilePaused(void* self) { return g_obj->flag; }