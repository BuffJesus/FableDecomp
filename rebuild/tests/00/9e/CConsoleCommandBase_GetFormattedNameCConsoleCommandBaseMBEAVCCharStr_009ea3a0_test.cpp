#include "fable_console.h"
#include <stdio.h>
#include <string.h>

static const CCharString* g_left;
static const CCharString* g_right;
static int g_calls;

CCharString::CCharString() : storage_(0) {}
CCharString::CCharString(const CCharString&) : storage_(0) {}
CCharString::~CCharString() {}

extern const CCharString g_013caa48;
const CCharString g_013caa48;

CCharString FABLE_FASTCALL operator+(const CCharString& left,
                                    const CCharString& right)
{
    ++g_calls;
    g_left = &left;
    g_right = &right;
    return CCharString();
}

class FormatAccess : public CConsoleCommandBase
{
public:
    CCharString Call() const { return CConsoleCommandBase::GetFormattedName(); }
};

int main()
{
    unsigned char storage[sizeof(CConsoleCommandBase)];
    memset(storage, 0, sizeof(storage));
    FormatAccess* command = reinterpret_cast<FormatAccess*>(storage);
    CCharString result = command->Call();
    const CCharString* expectedName =
        reinterpret_cast<const CCharString*>(storage + 4);
    if (g_calls != 1 || g_left != expectedName ||
        g_right != &g_013caa48 || sizeof(CConsoleCommandBase) != 0x14)
        return 1;
    puts("GETFMTNAME_OK");
    return 0;
}
