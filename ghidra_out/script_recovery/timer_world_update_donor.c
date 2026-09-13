//=== ?WorldUpdate@CGameScriptInterface@@QAEX_N@Z @ 02466920 (seed 0x02466920) ===

/* WARNING: Removing unreachable block (ram,0x024669a0) */
/* [ported from ego_r via strfp] */

uint _WorldUpdate_CGameScriptInterface__QAEX_N_Z(char param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int *in_ECX;
  undefined1 local_2c [15];
  char local_1d;
  undefined1 local_1c [12];
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03d531b0;
  local_10 = ExceptionList;
  uVar1 = 0;
  if ((param_1 != '\0') &&
     (ExceptionList = &local_10, uVar1 = (**(code **)(*in_ECX + 0x62c))(), (uVar1 & 0xff) == 0)) {
    uVar1 = (**(code **)(*in_ECX + 0x608))();
    uVar1 = uVar1 & 0xff;
    if (uVar1 == 0) {
      iVar2 = _GetFrame_CWorld__SIJXZ();
      iVar3 = _GetConstantFPS_CMainGameComponent__SIJXZ();
      uVar1 = iVar2 / iVar3;
      if (iVar2 % iVar3 == 0) {
        _begin____Tree_V___Tmap_traits_JJU__less_J_std__V__allocator_U__pair___CBJJ_std___2__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits_JJU__less_J_std__V__allocator_U__pair___CBJJ_std___2__0A__std___std___2_XZ
                  (local_1c);
        local_8 = 0;
        while( true ) {
          uVar4 = _end____Tree_V___Tmap_traits_JJU__less_J_std__V__allocator_U__pair___CBJJ_std___2__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits_JJU__less_J_std__V__allocator_U__pair___CBJJ_std___2__0A__std___std___2_XZ
                            (local_2c);
          local_8._0_1_ = 1;
          local_1d = __9___Tree_const_iterator_V___Tree_val_V___Tmap_traits_JJU__less_J_std__V__allocator_U__pair___CBJJ_std___2__0A__std___std___std__QBE_NABV01__Z
                               (uVar4);
          local_8 = (uint)local_8._1_3_ << 8;
          __1___Tree_iterator_V___Tree_val_V___Tmap_traits_JJU__less_J_std__V__allocator_U__pair___CBJJ_std___2__0A__std___std___std__QAE_XZ
                    ();
          if (local_1d == '\0') break;
          iVar2 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits_JJU__less_J_std__V__allocator_U__pair___CBJJ_std___2__0A__std___std___std__QBEPAU__pair___CBJJ_1_XZ
                            ();
          if (0 < *(int *)(iVar2 + 4)) {
            iVar2 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits_JJU__less_J_std__V__allocator_U__pair___CBJJ_std___2__0A__std___std___std__QBEPAU__pair___CBJJ_1_XZ
                              ();
            *(int *)(iVar2 + 4) = *(int *)(iVar2 + 4) + -1;
          }
          __E___Tree_iterator_V___Tree_val_V___Tmap_traits_JJU__less_J_std__V__allocator_U__pair___CBJJ_std___2__0A__std___std___std__QAEAAV01_XZ
                    ();
        }
        local_8 = 0xffffffff;
        uVar1 = __1___Tree_iterator_V___Tree_val_V___Tmap_traits_JJU__less_J_std__V__allocator_U__pair___CBJJ_std___2__0A__std___std___std__QAE_XZ
                          ();
      }
    }
  }
  ExceptionList = local_10;
  return uVar1;
}


// CALLEES of ?WorldUpdate@CGameScriptInterface@@QAEX_N@Z:
//   0179f1a8  ??C?$_Tree_iterator@V?$_Tree_val@V?$_Tmap_traits@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@$0A@@std@@@std@@@std@@QBEPAU?$pair@$$CBJJ@1@XZ
//   017b0a7f  ??1?$_Tree_iterator@V?$_Tree_val@V?$_Tmap_traits@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@$0A@@std@@@std@@@std@@QAE@XZ
//   018275ad  ?begin@?$_Tree@V?$_Tmap_traits@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@$0A@@std@@@std@@QAE?AV?$_Tree_iterator@V?$_Tree_val@V?$_Tmap_traits@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@$0A@@std@@@std@@@2@XZ
//   017e6bb6  ??E?$_Tree_iterator@V?$_Tree_val@V?$_Tmap_traits@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@$0A@@std@@@std@@@std@@QAEAAV01@XZ
//   0179f40f  ?GetFrame@CWorld@@SIJXZ
//   017fc15f  ?end@?$_Tree@V?$_Tmap_traits@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@$0A@@std@@@std@@QAE?AV?$_Tree_iterator@V?$_Tree_val@V?$_Tmap_traits@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@$0A@@std@@@std@@@2@XZ
//   01805255  ?GetConstantFPS@CMainGameComponent@@SIJXZ
//   017a3258  ??9?$_Tree_const_iterator@V?$_Tree_val@V?$_Tmap_traits@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@$0A@@std@@@std@@@std@@QBE_NABV01@@Z

