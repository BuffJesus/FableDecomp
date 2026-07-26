struct CNetworkClient {
    char pad0[8];
    char flag8;
};

extern bool __fastcall CNetworkClient_GetGameEventPackageSet_delegate(CNetworkClient* self, int arg);

bool __fastcall CNetworkClient_GetGameEventPackageSet(CNetworkClient* self, int arg)
{
    if (self->flag8)
        return CNetworkClient_GetGameEventPackageSet_delegate(self, arg);
    return true;
}