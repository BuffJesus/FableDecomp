// CActionDoCreatureAction::GetActionName - retail 0x00692210
// Returns a fixed CCharString by value (NRV hidden return pointer).

class CCharString {
public:
    CCharString(const char* s, int n);   // extern ctor -> real call (matches call 0x99ebf0)
    char* m_data;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
    int m_dummy;
};

CCharString CActionDoCreatureAction::GetActionName() {
    return CCharString("DO_CREATURE_ACTION", -1);
}