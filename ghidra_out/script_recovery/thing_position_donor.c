//=== ?GetPos@CScriptThing@@UBEABVC3DVector@@XZ @ 018c5650 (seed 0x018c5650) ===

/* [ported from ego_r via strfp] */

undefined * _GetPos_CScriptThing__UBEABVC3DVector__XZ(void)

{
  code *pcVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  char cVar5;
  BOOL BVar6;
  undefined *puVar7;
  int *piVar8;
  char local_38;
  undefined1 local_2c [8];
  undefined1 local_24 [8];
  undefined1 local_1c [11];
  char local_11;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03c0d9db;
  local_10 = ExceptionList;
  bVar4 = false;
  bVar3 = false;
  bVar2 = false;
  ExceptionList = &local_10;
  cVar5 = _IsNull___CCountedPointer_VCScriptThing____QBE_NXZ();
  if (cVar5 == '\0') {
    piVar8 = (int *)__C__CCountedPointer_VCScriptThing____QBEPAVCScriptThing__XZ();
    puVar7 = (undefined *)(**(code **)(*piVar8 + 0x18))();
    ExceptionList = local_10;
    return puVar7;
  }
  if (DAT_04bac270 != 0) {
    __0CWideString__QAE_PB_W_Z
              (
              L"e:\\fabletlc\\fable tlc build repository\\bbbprojects\\fable1_5mainpc\\fablescripting\\script_thing.hpp"
              );
    local_8 = 0;
    __0CCharString__QAE_PBDJ_Z("This script thing is not valid!",0xffffffff);
    local_8 = 1;
    __0CCharString__QAE_PBDJ_Z(&DAT_0401a250,0xffffffff);
    local_8 = 2;
    bVar4 = true;
    bVar3 = true;
    bVar2 = true;
    cVar5 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (local_2c,local_24,local_1c,0x34,1);
    if (cVar5 != '\x01') {
      local_38 = '\0';
      goto LAB_018c5726;
    }
  }
  local_38 = '\x01';
LAB_018c5726:
  local_11 = local_38;
  local_8 = 1;
  if (bVar3) {
    __1CCharString__QAE_XZ();
  }
  local_8 = 0;
  if (bVar4) {
    __1CCharString__QAE_XZ();
  }
  local_8 = 0xffffffff;
  if (bVar2) {
    __1CWideString__QAE_XZ();
  }
  if ((local_11 != '\0') && (BVar6 = IsDebuggerPresent(), BVar6 != 0)) {
    pcVar1 = (code *)swi(3);
    puVar7 = (undefined *)(*pcVar1)();
    return puVar7;
  }
  ExceptionList = local_10;
  return &DAT_04bc16c0;
}


// CALLEES of ?GetPos@CScriptThing@@UBEABVC3DVector@@XZ:
//   01822233  ??1CCharString@@QAE@XZ
//   017ea333  ??1CWideString@@QAE@XZ
//   0183552c  ?DoErrorMessage@CDebugManager@@QAE_NABVCCharString@@0ABVCWideString@@KW4EErrorType@NDebugManager@@@Z
//   017de12d  ?IsNull@?$CCountedPointer@VCScriptThing@@@@QBE_NXZ
//   EXTERNAL:00000128  IsDebuggerPresent
//   017f059e  ??0CWideString@@QAE@PB_W@Z
//   0181c126  ??0CCharString@@QAE@PBDJ@Z
//   017b7f50  ??C?$CCountedPointer@VCScriptThing@@@@QBEPAVCScriptThing@@XZ

//=== ?GetPos@CGameScriptThing@@UBEABVC3DVector@@XZ @ 02b001d0 (seed 0x02b001d0) ===

/* [ported from ego_r via strfp] */

int _GetPos_CGameScriptThing__UBEABVC3DVector__XZ(void)

{
  int in_ECX;
  
  return in_ECX + 0x30;
}


// CALLEES of ?GetPos@CGameScriptThing@@UBEABVC3DVector@@XZ:

