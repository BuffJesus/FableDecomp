// CGameScriptInterface::SetDeathRecoveryMarkerName
struct DeathRecoveryMarkerNameNode
{
    int key;
    void* value;
};

struct DeathRecoveryMarkerNameMap
{
    DeathRecoveryMarkerNameNode* begin;
    DeathRecoveryMarkerNameNode* end;

    DeathRecoveryMarkerNameNode* lower_bound(const int* key);
};

struct DeathRecoveryMarkerNameSource
{
    void* GetTarget();
};

struct CCharString
{
    CCharString& operator=(const CCharString& other);
};

struct DeathRecoveryMarkerNameInterface
{
    char pad00[0x170];
    CCharString markerName;
};

struct CGameScriptInterface_00896f60
{
    char pad00[0x14];
    DeathRecoveryMarkerNameSource* source;
};

extern "C" void* __fastcall ResolveDeathRecoveryMarkerTarget(void* source);

void __fastcall CGameScriptInterface_SetDeathRecoveryMarkerName(
    CGameScriptInterface_00896f60* self,
    unsigned long,
    const CCharString& name)
{
    void* source = self->source->GetTarget();
    unsigned char* target =
        (unsigned char*)ResolveDeathRecoveryMarkerTarget(source);

    if (target == 0)
        return;
    if (*(unsigned char*)(target + 0x91) & 0x01)
        return;
    if (!(*(unsigned char*)(target + 0x20) & 0x10))
        return;

    int key = 4;
    DeathRecoveryMarkerNameMap* map =
        (DeathRecoveryMarkerNameMap*)(target + 0x44);
    DeathRecoveryMarkerNameNode* found = map->lower_bound(&key);
    DeathRecoveryMarkerNameNode* end = map->end;
    if (found == end || found->key > 4)
        found = end;

    int markerInterface = ((volatile int*)found)[1];
    ((CCharString*)(markerInterface + 0x170))->operator=(name);
}
