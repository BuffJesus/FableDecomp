// CActionDoCreatureAction::GetActionName
// Retail 0x00b17910 - __fastcall, returns CCharString by value from a string literal.
// 'this' is unused; body constructs CCharString(literal, -1) into the hidden return buffer.

class CCharString {
public:
    // (const char*, int length = -1) constructor; out-of-line (matches call 0x99ebf0)
    CCharString(const char* s, int len);
    CCharString(const CCharString& o);
    ~CCharString();
private:
    char* m_data;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName() const;
};

CCharString CActionDoCreatureAction::GetActionName() const {
    return CCharString("DoCreatureAction", -1);
}