
#include <cstdio>
#include <cstring>

class CCharString {
public:
    char* m_pData;
    CCharString() : m_pData(0) {}
    CCharString(const CCharString& other) {
        if (other.m_pData) {
            m_pData = new char[strlen(other.m_pData) + 1];
            strcpy(m_pData, other.m_pData);
        } else {
            m_pData = 0;
        }
    }
    ~CCharString() { if (m_pData) delete[] m_pData; }
};

static char g_categoryText[64];
static char g_rumourText[64];
static bool g_called = false;

void __stdcall CGameScriptInterface_AddRumourToCategoryEx(CCharString rumourCategory, CCharString rumourText)
{
    g_called = true;
    if (rumourCategory.m_pData) strcpy(g_categoryText, rumourCategory.m_pData);
    else g_categoryText[0] = 0;
    if (rumourText.m_pData) strcpy(g_rumourText, rumourText.m_pData);
    else g_rumourText[0] = 0;
}

class CGameScriptInterface {
public:
    void AddNewRumourToCategory(const CCharString& rumourCategory, const CCharString& rumourText) const;
};

void __fastcall CGameScriptInterface_AddNewRumourToCategory(CGameScriptInterface* self, void* edx, const CCharString& rumourCategory, const CCharString& rumourText)
{
    CGameScriptInterface_AddRumourToCategoryEx(rumourCategory, rumourText);
}

void CGameScriptInterface::AddNewRumourToCategory(const CCharString& rumourCategory, const CCharString& rumourText) const
{
    CGameScriptInterface_AddNewRumourToCategory((CGameScriptInterface*)this, 0, rumourCategory, rumourText);
}

int main()
{
    CGameScriptInterface iface;
    CCharString cat;
    cat.m_pData = new char[16];
    strcpy(cat.m_pData, "Weather");

    CCharString txt;
    txt.m_pData = new char[16];
    strcpy(txt.m_pData, "ItRains");

    iface.AddNewRumourToCategory(cat, txt);

    if (g_called && strcmp(g_categoryText, "Weather") == 0 && strcmp(g_rumourText, "ItRains") == 0) {
        printf("PASS_ADDRUMOUR_9DD0\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}