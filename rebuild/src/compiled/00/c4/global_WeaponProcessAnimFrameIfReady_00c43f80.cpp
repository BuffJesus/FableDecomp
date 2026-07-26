struct CObj;

struct CObj2 {
    char Check1(CObj* a);
    int  GetState();
};

struct CSub {
    char DoProcess(CObj* a, int b, int c);
};

struct CWeapon {
    char pad[0x54];
    char Check1(CObj* a);
};

struct CLogger {
    void LogErr(int code);
};

extern CLogger g_logger;

char __fastcall Weapon_ProcessAnimFrameIfReady(CWeapon* self, int edx, CObj* a, int b, int c)
{
    if (self->Check1(a))
    {
        int s = ((CObj2*)a)->GetState();
        if (s < 0)
        {
            g_logger.LogErr(s);
            return 0;
        }
        return ((CSub*)((char*)self + 0x54))->DoProcess(a, b, c);
    }
    return 0;
}