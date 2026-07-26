#pragma optimize("ty",off)
struct CShadowSink { void Push(void* x); };
struct CLocalScopeVec { void Do(void* obj24, void* param); };
struct CEngineLightingManager {
  char pad0[0x18];
  int mode;              /* +0x18 */
  char pad1c[0x24-0x1c];
  CShadowSink* obj24;    /* +0x24 */
  CShadowSink* obj28;    /* +0x28 */
  void Log(const char* s);
  void UpdateShadowScene(void* param);
};
void CEngineLightingManager::UpdateShadowScene(void* param)
{
  int local;
  this->Log((const char*)0x122d70e);
  local = 0;
  switch (this->mode) {
  case 2:
    ((CLocalScopeVec*)&local)->Do(this->obj24, param);
    break;
  case 3:
    this->obj28->Push(*(void**)param);
    break;
  }
}