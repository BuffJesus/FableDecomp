//=== WriteZlibCompressed @ 009967e0 ===

/* WARNING: Removing unreachable block (ram,0x00996917) */
/* WARNING: Removing unreachable block (ram,0x0099695f) */
/* [bsim sim=0.6738983538747179 <- ego_r]
   public: void __thiscall CDataOutputStream::WriteZlibCompressed(void const *,long,long) */

void __thiscall
CDataOutputStream::WriteZlibCompressed
          (CDataOutputStream *this,void *param_1,long param_2,long param_3)

{
  bool bVar1;
  undefined1 *puVar2;
  CEngineInternalPrimitiveBase *this_00;
  undefined1 *unaff_EBP;
  CEnginePrimitive *pCVar3;
  ulong uVar4;
  undefined1 *apuStack_68 [2];
  undefined **appuStack_60 [23];
  void *pvStack_4;
  
  if (0 < param_2) {
    (**(code **)(*(int *)this + 8))();
    uVar4 = 4;
    pCVar3 = (CEnginePrimitive *)apuStack_68;
    apuStack_68[0] = (undefined1 *)0x0;
    (**(code **)(*(int *)this + 0x10))();
    if ((int)param_1 < 0x400) {
      param_1 = (void *)0x400;
    }
    else if ((int)(param_2 + 0x400U) < (int)param_1) {
      param_1 = (void *)(param_2 + 0x400U);
    }
    FUN_00514e20(param_1);
    CEngineInternalPrimitiveBase::AddChildPrimitive(this_00,pCVar3,uVar4);
    CPreallocTexturePool::CPreallocTexturePool((CPreallocTexturePool *)appuStack_60);
    appuStack_60[0] = &PTR__vector_deleting_destructor__01245974;
    NScript::CV_OracleScript::CVO_OracleMarker::Init((CVO_OracleMarker *)appuStack_60);
    CCompressorZlib::SetStreamInput((CCompressorZlib *)appuStack_60,pvStack_4,param_2);
    (**(code **)(*(int *)this + 8))();
    bVar1 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)appuStack_60);
    if (bVar1) {
      do {
        puVar2 = unaff_EBP;
        if (apuStack_68[0] == unaff_EBP) {
          puVar2 = &stack0xffffff94;
        }
        uVar4 = CCompressorZlib::CompressStream
                          ((CCompressorZlib *)appuStack_60,puVar2,(ulong)param_1);
        puVar2 = unaff_EBP;
        if (apuStack_68[0] == unaff_EBP) {
          puVar2 = &stack0xffffff94;
        }
        (**(code **)(*(int *)this + 0x10))(puVar2,uVar4);
        bVar1 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)appuStack_60);
      } while (bVar1);
    }
    do {
      puVar2 = unaff_EBP;
      if (apuStack_68[0] == unaff_EBP) {
        puVar2 = &stack0xffffff94;
      }
      uVar4 = CCompressorZlib::FinishCompressStream
                        ((CCompressorZlib *)appuStack_60,puVar2,(ulong)param_1,
                         (bool *)&stack0x00000000);
      puVar2 = unaff_EBP;
      if (apuStack_68[0] == unaff_EBP) {
        puVar2 = &stack0xffffff94;
      }
      (**(code **)(*(int *)this + 0x10))(puVar2,uVar4);
    } while( true );
  }
  return;
}



