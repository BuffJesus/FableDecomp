extern const float g_defaultHealth;

struct HeroObj {
    unsigned char _pad_0x6c[0x6c];
    unsigned char flag6c; // +0x6c
    unsigned char _pad_0x6d[0x24]; // to 0x91
    unsigned char flag91; // +0x91
    unsigned char _pad_0x92[0x22]; // to 0xb4
    float health; // +0xb4
};

struct GSI;
typedef HeroObj* (__fastcall *GetObjFn)(GSI*);
struct GSIVT {
    void* slots0[11];
    GetObjFn getObj; // slot at +0x2c
};

struct GSI {
    GSIVT* vt;
};

float __stdcall CGameScriptInterface_GetHealth(GSI* self)
{
    HeroObj* o = self->vt->getObj(self);
    if (o != 0 &&
        (o->flag91 & 0x1) == 0 &&
        (o->flag6c & 0x2) != 0) {
        return o->health;
    }
    return g_defaultHealth;
}