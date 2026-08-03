//=== 0055cb10 : NO FUNCTION ===
//=== LinkedList_FilterByPredicate @ 0055cbe0 (seed 0055cbe0) ===

/* autoname-t5 [medium]: Thiscall iterates linked list via LinkedList_CopyElements, filters elements
   using vtable callback at offset 8, calls cleanup callbacks; linked list filtering/mapping
   operation. */

void LinkedList_FilterByPredicate(int *param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  char cVar4;
  undefined4 *_Memory;
  int in_ECX;
  
  _Memory = malloc(0xc);
  *_Memory = _Memory;
  _Memory[1] = _Memory;
  LinkedList_CopyElements
            (_Memory,**(undefined4 **)(in_ECX + 4),*(undefined4 **)(in_ECX + 4),&param_2);
  uVar3 = param_2;
  for (puVar1 = (undefined4 *)*_Memory; puVar1 != _Memory; puVar1 = (undefined4 *)*puVar1) {
    if (((int *)puVar1[2] == param_1) &&
       (cVar4 = (**(code **)(*(int *)puVar1[2] + 8))(uVar3), cVar4 != '\0')) {
      (**(code **)(*(int *)puVar1[2] + 4))(uVar3);
    }
  }
  puVar1 = (undefined4 *)*_Memory;
  while (puVar1 != _Memory) {
    puVar2 = (undefined4 *)*puVar1;
    free(puVar1);
    puVar1 = puVar2;
  }
  *_Memory = _Memory;
  _Memory[1] = _Memory;
  free(_Memory);
  return;
}


// CALLEES of LinkedList_FilterByPredicate:
//   00bfea0e  malloc
//   00bfea14  free
//   0055ce90  LinkedList_CopyElements

//=== 0055ca40 : NO FUNCTION ===
//=== 0055c930 : NO FUNCTION ===
//=== 0055c940 : NO FUNCTION ===
//=== 0055c9c0 : NO FUNCTION ===
//=== ProcessLoading @ 0055c950 (seed 0055c950) ===

/* [bsim sim=0.6530620316191298 <- ego_r]
   public: void __thiscall CGraphicBankManager::ProcessLoading(bool,double) */

void __thiscall
CGraphicBankManager::ProcessLoading(CGraphicBankManager *this,bool param_1,double param_2)

{
  int *piVar1;
  int *piVar2;
  
  piVar2 = (int *)**(int **)(this + 4);
  if (piVar2 != *(int **)(this + 4)) {
    do {
      piVar1 = (int *)*piVar2;
      free(piVar2);
      piVar2 = piVar1;
    } while (piVar1 != (int *)*(int *)(this + 4));
  }
  *(undefined4 *)*(undefined4 *)(this + 4) = *(undefined4 *)(this + 4);
  *(int *)(*(int *)(this + 4) + 4) = *(int *)(this + 4);
  *(undefined4 *)(this + 8) = 0;
  piVar2 = (int *)**(int **)(this + 0xc);
  if (piVar2 != *(int **)(this + 0xc)) {
    do {
      piVar1 = (int *)*piVar2;
      free(piVar2);
      piVar2 = piVar1;
    } while (piVar1 != (int *)*(int *)(this + 0xc));
  }
  *(undefined4 *)*(undefined4 *)(this + 0xc) = *(undefined4 *)(this + 0xc);
  *(int *)(*(int *)(this + 0xc) + 4) = *(int *)(this + 0xc);
  return;
}


// CALLEES of ProcessLoading:
//   00bfea14  free

//=== 0055cae0 : NO FUNCTION ===
//=== 0055ca90 : NO FUNCTION ===
//=== 0055ca00 : NO FUNCTION ===
