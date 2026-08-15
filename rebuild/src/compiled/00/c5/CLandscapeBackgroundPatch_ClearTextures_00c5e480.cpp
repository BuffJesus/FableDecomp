struct CLandscapeBackgroundPatch {
  void* vtable;   // +0
  void* tex;      // +4
};

// global function pointer at fixed address; callee cleans its own stack arg
extern void (__stdcall *g_ReleaseTex)(void* t);

void __fastcall CLandscapeBackgroundPatch_ClearTextures(CLandscapeBackgroundPatch* self)
{
  void* tex = self->tex;
  self->vtable = (void*)0x12b90ec;
  if (tex) {
    g_ReleaseTex(tex);
    self->tex = 0;
  }
}