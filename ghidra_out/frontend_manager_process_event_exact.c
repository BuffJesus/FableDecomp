//=== NUISystem_CManager_ProcessEvent @ 0055cb10 (seed 0055cb10) ===

/* [pdb name; fn created by CreateMissingFunctions] */

void NUISystem_CManager_ProcessEvent(undefined4 param_1)

{
  char cVar1;
  int *in_ECX;
  int *piVar2;
  int *piStack_4;
  
  if ((int *)in_ECX[2] == (int *)0x0) {
    piStack_4 = in_ECX;
    if (*(int **)in_ECX[3] == (int *)in_ECX[3]) {
      LinkedList_Init_Copy(in_ECX + 1);
      for (piVar2 = (int *)*piStack_4; piVar2 != piStack_4; piVar2 = (int *)*piVar2) {
        cVar1 = (**(code **)(*(int *)piVar2[2] + 8))(param_1);
        if (cVar1 != '\0') {
          (**(code **)(*(int *)piVar2[2] + 4))(param_1);
        }
      }
    }
    else {
      LinkedList_Init_Copy(in_ECX + 3);
      piVar2 = (int *)*piStack_4;
      if (piVar2 != piStack_4) {
        do {
          cVar1 = (**(code **)(*(int *)piVar2[2] + 8))(param_1);
          if (cVar1 != '\0') {
            (**(code **)(*(int *)piVar2[2] + 4))(param_1);
          }
          piVar2 = (int *)*piVar2;
        } while (piVar2 != piStack_4);
        CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&piStack_4);
        return;
      }
    }
    CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&piStack_4);
  }
  else {
    cVar1 = (**(code **)(*(int *)in_ECX[2] + 8))(param_1);
    if (cVar1 != '\0') {
      (**(code **)(*(int *)in_ECX[2] + 4))(param_1);
      return;
    }
  }
  return;
}


// CALLEES of NUISystem_CManager_ProcessEvent:
//   0042ac25  OnReadFinished
//   0055cf50  LinkedList_Init_Copy

