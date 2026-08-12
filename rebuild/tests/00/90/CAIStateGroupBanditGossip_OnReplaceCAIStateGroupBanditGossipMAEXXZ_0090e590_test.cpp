#include <stdio.h>

static int g_base_called = 0;
static int g_notify_called = 0;
static void* g_notify_self = 0;

extern "C" void __fastcall BaseGroupOnReplace(void* self) { g_base_called++; }
extern "C" void __fastcall SubNotify(void* self) { g_notify_called++; g_notify_self = self; }

struct CAIStateGroup_BanditGossip {
    void*  vtbl;
    void** owner;
    char   pad08[0x24 - 0x08];
    char   flag;
    void OnReplace();
};

void CAIStateGroup_BanditGossip::OnReplace()
{
    BaseGroupOnReplace(this);
    if (this->flag) {
        void* target = this->owner[0x18 / sizeof(void*)];
        SubNotify(target);
    }
}

int main()
{
    // owner array must have index 0x18/4 = 6 valid
    void* ownerArr[8];
    for (int i = 0; i < 8; ++i) ownerArr[i] = 0;
    int markerVal = 0xABCD;
    void* mp = &markerVal ;
    ownerArr[6] = mp;

    CAIStateGroup_BanditGossip obj;
    obj.vtbl = 0;
    obj.owner = ownerArr;
    obj.flag = 0;

    // flag = 0 : base called, no notify
    g_base_called = g_notify_called = 0;
    obj.OnReplace();
    if (g_base_called != 1 || g_notify_called != 0) { printf("FAIL case0\n"); return 1; }

    // flag = 1 : base called, notify called with *(owner+0x18)
    obj.flag = 1;
    g_base_called = g_notify_called = 0;
    g_notify_self = 0;
    obj.OnReplace();
    if (g_base_called != 1 || g_notify_called != 1) { printf("FAIL case1\n"); return 1; }
    if (g_notify_self != mp) { printf("FAIL self\n"); return 1; }

    printf("BANDITGOSSIP_ONREPLACE_OK\n");
    return 0;
}