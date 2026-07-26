#include <cstdio>

struct CUserProfileManager {
  char pad0[0xC];
  char m_flagC;
  char pad0d[0x14-0x0D];
  char m_state14;
  char pad15[0xD4-0x15];
  char m_stateD4;
};

static int g_sub1 = 0;
static int g_sub2 = 0;

void __fastcall CUserProfileManager_Sub1(CUserProfileManager* self) { (void)self; g_sub1++; }
void __fastcall CUserProfileManager_Sub2(CUserProfileManager* self) { (void)self; g_sub2++; }

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, int unused, char a, char b)
{
  (void)unused;
  if (self->m_flagC)
    self->m_stateD4 = a;
  self->m_state14 = a;
  CUserProfileManager_Sub1(self);
  if (b)
    CUserProfileManager_Sub2(self);
}

int main() {
  CUserProfileManager o;
  o.m_flagC = 1; o.m_state14 = 0; o.m_stateD4 = 0;
  g_sub1 = g_sub2 = 0;
  CUserProfileManager_SetCameraResetting(&o, 0, 7, 1);
  if (o.m_stateD4 != 7) { std::printf("FAIL d4\n"); return 1; }
  if (o.m_state14 != 7) { std::printf("FAIL 14\n"); return 1; }
  if (g_sub1 != 1) { std::printf("FAIL sub1\n"); return 1; }
  if (g_sub2 != 1) { std::printf("FAIL sub2\n"); return 1; }

  o.m_flagC = 0; o.m_state14 = 0; o.m_stateD4 = 99;
  g_sub1 = g_sub2 = 0;
  CUserProfileManager_SetCameraResetting(&o, 0, 3, 0);
  if (o.m_stateD4 != 99) { std::printf("FAIL d4-skip\n"); return 1; }
  if (o.m_state14 != 3) { std::printf("FAIL 14-2\n"); return 1; }
  if (g_sub1 != 1) { std::printf("FAIL sub1-2\n"); return 1; }
  if (g_sub2 != 0) { std::printf("FAIL sub2-skip\n"); return 1; }

  std::printf("CUserProfileManager_0040b150_TEST PASS\n");
  return 0;
}