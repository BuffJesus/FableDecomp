// CCombatSequence_TrollRockPursue::GetName - returns CCharString by value.
// Retail: builds a CCharString from a string literal (len=-1) into the hidden
// return buffer (esi) and returns that buffer.

class CCharString {
public:
    CCharString(const char* s, int len);
    // Construct-in-place without a null check (matches retail's unconditional ctor call).
    void Init(const char* s, int len);
    void* m_data;
};

// String literal lives at 0x1278aec in retail.
static const char* const s_name = "CombatSequence_TrollRockPursue";

// __fastcall member of CCombatSequence_TrollRockPursue.
struct CCombatSequence_TrollRockPursue {
    void* vtbl;
    CCharString* GetName_CCombatSequence_TrollRockPursue(CCharString* __ret) const;
};

CCharString* CCombatSequence_TrollRockPursue::GetName_CCombatSequence_TrollRockPursue(CCharString* __ret) const {
    __ret->Init(s_name, -1);
    return __ret;
}