#include <stdio.h>

struct CCharString {
    void* p;
};

struct CDefString {
    void* p;
    bool operator==(const CCharString& rhs) const;
};

struct CThing {
    CDefString GetDefName() const;
};

struct CThingFilter_HasDefName {
    CCharString m_name;
    bool operator()(const CThing& thing) const;
};

// --- fakes to drive behavior ---
static void* g_defname_value = 0;
static const CCharString* g_seen_rhs = 0;
static CDefString* g_seen_this = 0;

CDefString CThing::GetDefName() const
{
    CDefString d;
    // encode "this" (the CThing) so we can verify it was used as the receiver
    d.p = (void*)this;
    g_defname_value = d.p;
    return d;
}

bool CDefString::operator==(const CCharString& rhs) const
{
    g_seen_this = (CDefString*)this;
    g_seen_rhs = &rhs;
    // true iff the def-string pointer matches the stored name pointer
    return this->p == rhs.p;
}

int main()
{
    CThing thing;
    CThingFilter_HasDefName filter;
    // Make m_name.p equal to &thing so equality holds
    filter.m_name.p = (void*)&thing;

    bool r = filter(thing);

    bool ok = r
        && g_defname_value == (void*)&thing
        && g_seen_rhs == &filter.m_name
        && g_seen_this != 0;

    // Also verify the negative case
    CThingFilter_HasDefName filter2;
    filter2.m_name.p = (void*)0xDEAD;
    bool r2 = filter2(thing);

    if (ok && !r2)
        printf("006a7be0_TEST PASS\n");
    else
        printf("FAIL r=%d r2=%d dv=%p rhs=%p\n", (int)r, (int)r2, g_defname_value, (void*)g_seen_rhs);
    return 0;
}