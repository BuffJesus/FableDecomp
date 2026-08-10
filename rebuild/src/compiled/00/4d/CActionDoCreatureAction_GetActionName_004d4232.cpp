struct CCharString {
  void* p;
  void Ctor(const char* s, int n);
};
CCharString* __fastcall GetActionName(CCharString* ret) {
  ret->Ctor("DoCreatureAction", -1);
  return ret;
}