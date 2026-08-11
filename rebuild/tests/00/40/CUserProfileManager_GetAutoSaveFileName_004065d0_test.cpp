#include <cstdio>
struct CWideString { int d; CWideString(const unsigned short* lit); };
static const unsigned short* g_seen = 0;
CWideString::CWideString(const unsigned short* lit) { g_seen = lit; d = 0x1234; }
struct CUserProfileManager { CWideString GetAutoSaveFileName(); };
int main() {
  CUserProfileManager m;
  CWideString r = m.GetAutoSaveFileName();
  if (g_seen == (const unsigned short*)0x122de80 && r.d == 0x1234) printf("004065d0_TEST PASS");
  else printf("FAIL");
  return 0;
}