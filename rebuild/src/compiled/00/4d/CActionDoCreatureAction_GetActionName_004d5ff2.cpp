struct CCharString {
  char* p;
  CCharString* ctorB(const char* s, int n);
  CCharString* GetActionName_8();
};
CCharString* CCharString::GetActionName_8() {
  ctorB((const char*)0x123dd2c, -1);
  return this;
}