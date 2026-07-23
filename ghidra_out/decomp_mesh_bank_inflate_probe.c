INFO  DecompFuncs.java> ==================== FUN_00996610 @ 00996610 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
void __thiscall FUN_00996610(int *param_1,undefined1 *param_2,undefined1 *param_3)

{
  undefined4 ***pppuVar1;
  int iVar2;
  undefined4 ***pppuVar3;
  undefined1 *unaff_EBX;
  void *unaff_ESI;
  undefined4 ***unaff_retaddr;
  undefined1 *puVar4;
  undefined1 *puVar5;
  void *_Memory;
  undefined1 *local_1c;
  undefined4 **local_18;
  undefined4 **local_14;
  undefined4 **local_c;
  undefined4 **local_8;
  undefined4 **local_4;
  
  puVar5 = param_3;
  if (0 < (int)param_3) {
    if ((int)param_3 < 4) {
      param_3 = (undefined1 *)0x0;
      (**(code **)(*param_1 + 0x10))(&param_3,2);
      (**(code **)(*param_1 + 0x10))(local_4,puVar5);
    }
    else {
      param_3 = (undefined1 *)
                (((int)((int)param_3 * 0x41 + ((int)param_3 * 0x41 >> 0x1f & 0x3fU)) >> 6) + 0x80);
      FUN_00514e20(param_3);
      local_c = (undefined4 **)0x0;
      local_8 = (undefined4 **)0x0;
      local_4 = (undefined4 **)0x0;
      local_c = malloc(0x70000);
      puVar4 = param_2;
      local_4 = local_c + 0x1c000;
      pppuVar3 = (undefined4 ***)local_c;
      for (iVar2 = 0x1c000; iVar2 != 0; iVar2 = iVar2 + -1) {
        *pppuVar3 = (undefined4 **)0x0;
        pppuVar3 = pppuVar3 + 1;
      }
      local_8 = local_c + 0x1c000;
      pppuVar3 = (undefined4 ***)local_c;
      if (local_8 == local_c) {
        pppuVar3 = &local_c;
      }
      pppuVar1 = (undefined4 ***)local_18;
      if (local_14 == local_18) {
        pppuVar1 = &local_18;
      }
      _lzo1x_999_compress(param_2,puVar5 + -3,pppuVar1,&param_3,pppuVar3);
      local_1c = puVar5;
      if (local_14 == local_18) {
        local_18 = &local_18;
      }
      _lzo1x_decompress(local_18,param_3,puVar4,&local_1c,0);
      _Memory = (void *)0x2;
      if (param_3 < puVar5 + -3) {
        if (param_3 < (undefined1 *)0x8000) {
          param_2 = param_3;
          (**(code **)(*param_1 + 0x10))(&param_2);
        }
        else {
          param_2 = (undefined1 *)0xffffffff;
          (**(code **)(*param_1 + 0x10))(&param_2);
          local_4 = unaff_retaddr;
          (**(code **)(*param_1 + 0x10))(&local_4,4);
        }
        if (local_1c == unaff_EBX) {
          unaff_EBX = &stack0xffffffe0;
        }
        (**(code **)(*param_1 + 0x10))(unaff_EBX,unaff_retaddr);
        puVar4 = puVar5 + -3 + (int)puVar4;
        puVar5 = (undefined1 *)0x3;
      }
      else {
        param_2 = (undefined1 *)0x0;
        (**(code **)(*param_1 + 0x10))(&param_2);
      }
      (**(code **)(*param_1 + 0x10))(puVar4,puVar5);
      if (unaff_ESI != (void *)0x0) {
        free(unaff_ESI);
      }
      if (_Memory != (void *)0x0) {
        free(_Memory);
        return;
      }
    }
  }
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== FUN_00996610 @ 00996610 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
void __thiscall FUN_00996610(int *param_1,undefined1 *param_2,undefined1 *param_3)

{
  undefined4 ***pppuVar1;
  int iVar2;
  undefined4 ***pppuVar3;
  undefined1 *unaff_EBX;
  void *unaff_ESI;
  undefined4 ***unaff_retaddr;
  undefined1 *puVar4;
  undefined1 *puVar5;
  void *_Memory;
  undefined1 *local_1c;
  undefined4 **local_18;
  undefined4 **local_14;
  undefined4 **local_c;
  undefined4 **local_8;
  undefined4 **local_4;
  
  puVar5 = param_3;
  if (0 < (int)param_3) {
    if ((int)param_3 < 4) {
      param_3 = (undefined1 *)0x0;
      (**(code **)(*param_1 + 0x10))(&param_3,2);
      (**(code **)(*param_1 + 0x10))(local_4,puVar5);
    }
    else {
      param_3 = (undefined1 *)
                (((int)((int)param_3 * 0x41 + ((int)param_3 * 0x41 >> 0x1f & 0x3fU)) >> 6) + 0x80);
      FUN_00514e20(param_3);
      local_c = (undefined4 **)0x0;
      local_8 = (undefined4 **)0x0;
      local_4 = (undefined4 **)0x0;
      local_c = malloc(0x70000);
      puVar4 = param_2;
      local_4 = local_c + 0x1c000;
      pppuVar3 = (undefined4 ***)local_c;
      for (iVar2 = 0x1c000; iVar2 != 0; iVar2 = iVar2 + -1) {
        *pppuVar3 = (undefined4 **)0x0;
        pppuVar3 = pppuVar3 + 1;
      }
      local_8 = local_c + 0x1c000;
      pppuVar3 = (undefined4 ***)local_c;
      if (local_8 == local_c) {
        pppuVar3 = &local_c;
      }
      pppuVar1 = (undefined4 ***)local_18;
      if (local_14 == local_18) {
        pppuVar1 = &local_18;
      }
      _lzo1x_999_compress(param_2,puVar5 + -3,pppuVar1,&param_3,pppuVar3);
      local_1c = puVar5;
      if (local_14 == local_18) {
        local_18 = &local_18;
      }
      _lzo1x_decompress(local_18,param_3,puVar4,&local_1c,0);
      _Memory = (void *)0x2;
      if (param_3 < puVar5 + -3) {
        if (param_3 < (undefined1 *)0x8000) {
          param_2 = param_3;
          (**(code **)(*param_1 + 0x10))(&param_2);
        }
        else {
          param_2 = (undefined1 *)0xffffffff;
          (**(code **)(*param_1 + 0x10))(&param_2);
          local_4 = unaff_retaddr;
          (**(code **)(*param_1 + 0x10))(&local_4,4);
        }
        if (local_1c == unaff_EBX) {
          unaff_EBX = &stack0xffffffe0;
        }
        (**(code **)(*param_1 + 0x10))(unaff_EBX,unaff_retaddr);
        puVar4 = puVar5 + -3 + (int)puVar4;
        puVar5 = (undefined1 *)0x3;
      }
      else {
        param_2 = (undefined1 *)0x0;
        (**(code **)(*param_1 + 0x10))(&param_2);
      }
      (**(code **)(*param_1 + 0x10))(puVar4,puVar5);
      if (unaff_ESI != (void *)0x0) {
        free(unaff_ESI);
      }
      if (_Memory != (void *)0x0) {
        free(_Memory);
        return;
      }
    }
  }
  return;
}

 (GhidraScript)  
INFO  REPORT: Save succeeded for processed file: /Fable.exe (HeadlessAnalyzer)  
