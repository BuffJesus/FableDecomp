#include <cstdio>

struct CUserProfileManager {
  char pad0[0x24];
  float f24;
  float f28;
  float f2c;
  unsigned char b30;
  unsigned char b31;
};

static int g_helper_called = 0;

void __fastcall CUserProfileManager_Helper(CUserProfileManager* self)
{
  g_helper_called = 1;
  (void)self;
}

void __fastcall CUserProfileManager_SetDefaultValuesGameplay(CUserProfileManager* self)
{
  self->f24 = 0.8f;
  self->f28 = 0.6f;
  self->f2c = 0.9f;
  self->b31 = 0;
  self->b30 = 0;
  CUserProfileManager_Helper(self);
}

int main()
{
  CUserProfileManager m;
  m.f24 = m.f28 = m.f2c = 1.0f;
  m.b30 = m.b31 = 0x55;
  CUserProfileManager_SetDefaultValuesGameplay(&m);
  if (m.f24 == 0.8f && m.f28 == 0.6f && m.f2c == 0.9f &&
      m.b30 == 0 && m.b31 == 0 && g_helper_called == 1) {
    std::printf("CUserProfileManager_004083b0_TEST PASS\n");
    return 0;
  }
  std::printf("CUserProfileManager_004083b0_TEST FAIL\n");
  return 1;
}