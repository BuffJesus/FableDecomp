extern int g_subtypeCounterA;
extern int g_subtypeCounterB;
extern int __fastcall GetSubTypeAnalogDirection(void* record);
int __stdcall GetSubTypeForAction(int* record){
    switch (*record) {
    case 0x37: return ++g_subtypeCounterA;
    case 0x38: return ++g_subtypeCounterB;
    case 0x3c:
        switch (GetSubTypeAnalogDirection(record)) {
        case 1: return 0xa; case 2: return 0xb; case 3: return 0xc; case 4: return 0xd; }
    }
    return 0;
}