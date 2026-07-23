//=== GetLanguage @ 00415070 ===

/* [bsim sim=0.8972967292730697 <- ego_r]
   unsigned long __fastcall NLocalisation::GetLanguage(void) */

ulong __fastcall NLocalisation::GetLanguage(void)

{
  char cVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EBX;
  CCharString local_4 [4];
  
  if (DAT_013b8684 == 0) {
    CCharString::CCharString(local_4,"TEXT_ENGLISH_MAIN",-1);
    cVar1 = FUN_009a80b0(local_4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4,extraout_EDX
               ,unaff_EBX);
    if (cVar1 != '\0') {
      DAT_013b8684 = 9;
      return 9;
    }
    CCharString::CCharString(local_4,"TEXT_FRENCH_MAIN",-1);
    cVar1 = FUN_009a80b0(local_4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4,
               extraout_EDX_00,unaff_EBX);
    if (cVar1 != '\0') {
      DAT_013b8684 = 0xc;
      return 0xc;
    }
    CCharString::CCharString(local_4,"TEXT_JAPANESE_MAIN",-1);
    cVar1 = FUN_009a80b0(local_4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4,
               extraout_EDX_01,unaff_EBX);
    if (cVar1 != '\0') {
      DAT_013b8684 = 0x11;
      return 0x11;
    }
    CCharString::CCharString(local_4,"TEXT_GERMAN_MAIN",-1);
    cVar1 = FUN_009a80b0(local_4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4,
               extraout_EDX_02,unaff_EBX);
    if (cVar1 != '\0') {
      DAT_013b8684 = 7;
      return 7;
    }
    CCharString::CCharString(local_4,"TEXT_KOREAN_MAIN",-1);
    cVar1 = FUN_009a80b0(local_4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4,
               extraout_EDX_03,unaff_EBX);
    if (cVar1 != '\0') {
      DAT_013b8684 = 0x12;
      return 0x12;
    }
    CCharString::CCharString(local_4,"TEXT_CHINESE_MAIN",-1);
    cVar1 = FUN_009a80b0(local_4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4,
               extraout_EDX_04,unaff_EBX);
    if (cVar1 != '\0') {
      DAT_013b8684 = 4;
      return 4;
    }
    CCharString::CCharString(local_4,"TEXT_SPANISH_MAIN",-1);
    cVar1 = FUN_009a80b0(local_4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4,
               extraout_EDX_05,unaff_EBX);
    if (cVar1 != '\0') {
      DAT_013b8684 = 10;
      return 10;
    }
    CCharString::CCharString(local_4,"TEXT_ITALIAN_MAIN",-1);
    cVar1 = FUN_009a80b0(local_4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4,
               extraout_EDX_06,unaff_EBX);
    if (cVar1 != '\0') {
      DAT_013b8684 = 0x10;
      return 0x10;
    }
  }
  return DAT_013b8684;
}



//=== GetFontBankName @ 00415260 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   class CCharString __fastcall NLocalisation::GetFontBankName(void) */

void __fastcall NLocalisation::GetFontBankName(void)

{
  ulong uVar1;
  CCharString *in_ECX;
  
  uVar1 = GetLanguage();
  switch(uVar1) {
  case 4:
    CCharString::CCharString(in_ECX,"TEXT_CHINESE_MAIN",-1);
    return;
  default:
    CCharString::CCharString(in_ECX,"TEXT_ENGLISH_MAIN",-1);
    return;
  case 7:
    CCharString::CCharString(in_ECX,"TEXT_GERMAN_MAIN",-1);
    return;
  case 10:
    CCharString::CCharString(in_ECX,"TEXT_SPANISH_MAIN",-1);
    return;
  case 0xc:
    CCharString::CCharString(in_ECX,"TEXT_FRENCH_MAIN",-1);
    return;
  case 0x10:
    CCharString::CCharString(in_ECX,"TEXT_ITALIAN_MAIN",-1);
    return;
  case 0x11:
    CCharString::CCharString(in_ECX,"TEXT_JAPANESE_MAIN",-1);
    return;
  case 0x12:
    CCharString::CCharString(in_ECX,"TEXT_KOREAN_MAIN",-1);
    return;
  }
}



//=== GetFontBankName @ 00415350 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   class CCharString __fastcall NLocalisation::GetFontBankName(void) */

void __fastcall NLocalisation::GetFontBankName(void)

{
  ulong uVar1;
  CCharString *in_ECX;
  
  uVar1 = GetLanguage();
  switch(uVar1) {
  case 4:
    CCharString::CCharString(in_ECX,"FONT_CHINESE_MAIN",-1);
    return;
  default:
    CCharString::CCharString(in_ECX,"FONT_ENGLISH_MAIN",-1);
    return;
  case 7:
    CCharString::CCharString(in_ECX,"FONT_GERMAN_MAIN",-1);
    return;
  case 10:
    CCharString::CCharString(in_ECX,"FONT_SPANISH_MAIN",-1);
    return;
  case 0xc:
    CCharString::CCharString(in_ECX,"FONT_FRENCH_MAIN",-1);
    return;
  case 0x10:
    CCharString::CCharString(in_ECX,"FONT_ITALIAN_MAIN",-1);
    return;
  case 0x11:
    CCharString::CCharString(in_ECX,"FONT_JAPANESE_MAIN",-1);
    return;
  case 0x12:
    CCharString::CCharString(in_ECX,"FONT_KOREAN_MAIN",-1);
    return;
  }
}



//=== GetFontBankName @ 00415440 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   class CCharString __fastcall NLocalisation::GetFontBankName(void) */

void __fastcall NLocalisation::GetFontBankName(void)

{
  ulong uVar1;
  CCharString *in_ECX;
  
  uVar1 = GetLanguage();
  switch(uVar1) {
  case 4:
    CCharString::CCharString(in_ECX,"STREAMING_FONT_CHINESE_PC",-1);
    return;
  default:
    CCharString::CCharString(in_ECX,"STREAMING_FONT_ENGLISH_PC",-1);
    return;
  case 7:
    CCharString::CCharString(in_ECX,"STREAMING_FONT_GERMAN_PC",-1);
    return;
  case 10:
    CCharString::CCharString(in_ECX,"STREAMING_FONT_SPANISH_PC",-1);
    return;
  case 0xc:
    CCharString::CCharString(in_ECX,"STREAMING_FONT_FRENCH_PC",-1);
    return;
  case 0x10:
    CCharString::CCharString(in_ECX,"STREAMING_FONT_ITALIAN_PC",-1);
    return;
  case 0x11:
    CCharString::CCharString(in_ECX,"STREAMING_FONT_JAPANESE_PC",-1);
    return;
  case 0x12:
    CCharString::CCharString(in_ECX,"STREAMING_FONT_KOREAN_PC",-1);
    return;
  }
}



//=== GetFontBankName @ 00415550 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   class CCharString __fastcall NLocalisation::GetFontBankName(void) */

void __fastcall NLocalisation::GetFontBankName(void)

{
  ulong uVar1;
  CCharString *in_ECX;
  
  uVar1 = GetLanguage();
  switch(uVar1) {
  case 4:
    CCharString::CCharString(in_ECX,"CHINESE_SOUND_SETUP",-1);
    return;
  default:
    CCharString::CCharString(in_ECX,"ENGLISH_SOUND_SETUP",-1);
    return;
  case 7:
    CCharString::CCharString(in_ECX,"GERMAN_SOUND_SETUP",-1);
    return;
  case 10:
    CCharString::CCharString(in_ECX,"SPANISH_SOUND_SETUP",-1);
    return;
  case 0xc:
    CCharString::CCharString(in_ECX,"FRENCH_SOUND_SETUP",-1);
    return;
  case 0x10:
    CCharString::CCharString(in_ECX,"ITALIAN_SOUND_SETUP",-1);
    return;
  case 0x11:
    CCharString::CCharString(in_ECX,"JAPANESE_SOUND_SETUP",-1);
    return;
  case 0x12:
    CCharString::CCharString(in_ECX,"KOREAN_SOUND_SETUP",-1);
    return;
  }
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 00418fbc ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 0041ba30 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 00428905 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 00428928 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== GetPTextBank @ 00497b30 ===

/* [bsim sim=1.0 <- ego_r]
   private: class NGameText::CDataBank const * __thiscall CProgressDisplay::GetPTextBank(void)const
    */

CDataBank * __thiscall CProgressDisplay::GetPTextBank(CProgressDisplay *this)

{
  CDataBank *pCVar1;
  
  if (((DAT_013b86a0 == 0) ||
      (pCVar1 = *(CDataBank **)(DAT_013b86a0 + 0x14), pCVar1 == (CDataBank *)0x0)) &&
     ((DAT_013b871c == 0 ||
      (pCVar1 = *(CDataBank **)(DAT_013b871c + 0x60), pCVar1 == (CDataBank *)0x0)))) {
    pCVar1 = (CDataBank *)0x0;
  }
  return pCVar1;
}



//=== ?NotifyPostEntriesLoaded@CBankFile@@MAEXXZ @ 0050b440 ===

void _NotifyPostEntriesLoaded_CBankFile__MAEXXZ(void)

{
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 0056c050 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 005ba124 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 005c05d5 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 00654b36 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 00654b7b ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 00654bc0 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 00654c05 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 00654c4a ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 00654c8f ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 00654dfd ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 0065d806 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 0065d822 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 0065d87f ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 0065d89b ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 0065d8d0 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 0065d8f6 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 0065da52 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 0065db2a ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 0065db43 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 0065db5c ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 0065db75 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 0065db8e ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== UploadTextureMorphsToEngine @ 006ca7f0 ===

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CTCAppearanceMorphBase::UploadTextureMorphsToEngine(class CIEngine
   &,class std::set<long,struct std::less<long>,class std::allocator<long> > const *,bool,float) */

void __thiscall
CTCAppearanceMorphBase::UploadTextureMorphsToEngine
          (CTCAppearanceMorphBase *this,CIEngine *param_1,
          set<long,std::less<long>,std::allocator<long>_> *param_2,bool param_3,float param_4)

{
  int iVar1;
  undefined3 in_stack_0000000d;
  
  if (DAT_0137573e != '\0') {
    (**(code **)(*(int *)this + 0x70))();
    iVar1 = (**(code **)(*(int *)this + 0x74))();
    (**(code **)(*(int *)param_1 + 0xf0))(iVar1 + 0xc,_param_3,param_4,param_2);
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 00704430 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== ~CLipSyncBankStateBlock @ 00811be0 ===

/* [bsim sim=0.7058260062185795 <- ego_r]
   public: virtual __thiscall CLipSyncBankStateBlock::~CLipSyncBankStateBlock(void) */

void __thiscall CLipSyncBankStateBlock::~CLipSyncBankStateBlock(CLipSyncBankStateBlock *this)

{
  int *piVar1;
  
  *(undefined ***)this = &PTR__scalar_deleting_destructor__01272344;
  piVar1 = *(int **)(this + 0xc);
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + -1;
    if (**(int **)(this + 0xc) == 0) {
      (*(code *)(*(int **)(this + 0xc))[1])();
      operator_delete(*(void **)(this + 0xc));
    }
  }
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined ***)this = &PTR_LAB_0124f2c4;
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 008aa6a0 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 008aa9b0 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== ~CBankFileEntryUpdateData @ 00991880 ===

/* [bsim sim=0.5113815037417824 <- ego_r]
   private: __thiscall CBankFileEntryUpdateData::~CBankFileEntryUpdateData(void) */

void __thiscall CBankFileEntryUpdateData::~CBankFileEntryUpdateData(CBankFileEntryUpdateData *this)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129a574;
  iVar1 = *(int *)(this + 0x34);
  for (iVar3 = *(int *)(this + 0x30); iVar3 != iVar1; iVar3 = iVar3 + 0x24) {
    CEditFractal::~CEditFractal((CEditFractal *)(iVar3 + 4));
  }
  if (*(void **)(this + 0x30) != (void *)0x0) {
    free(*(void **)(this + 0x30));
  }
  piVar2 = *(int **)(this + 0x2c);
  if (piVar2 != (int *)0x0) {
    *piVar2 = *piVar2 + -1;
    if (**(int **)(this + 0x2c) == 0) {
      (*(code *)(*(int **)(this + 0x2c))[1])();
      operator_delete(*(void **)(this + 0x2c));
    }
  }
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  return;
}



//=== TransferVectorLoadText @ 009bb110 ===

/* [bsim sim=0.5564299977046913 <- ego_r]
   protected: void __thiscall CPersistContext::TransferVectorLoadText(char const *,class
   CPersistContext_TransferableVectorGenericType &) */

void __thiscall
CPersistContext::TransferVectorLoadText
          (CPersistContext *this,char *param_1,
          CPersistContext_TransferableVectorGenericType *param_2)

{
  char cVar1;
  CBasicString<char> *pCVar2;
  bool bVar3;
  char *pcVar4;
  CCharString *pCVar5;
  int iVar6;
  undefined4 *puVar7;
  int iVar8;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar9;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_08;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_09;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_10;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_11;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_12;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_15;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_16;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_17;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_18;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_19;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_20;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_21;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_22;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_23;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_24;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_25;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_26;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_27;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_28;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_29;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_30;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_31;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_32;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_33;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_34;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_35;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_36;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_37;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_38;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_39;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_40;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_41;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_42;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_43;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar10;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_44;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_45;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_46;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_47;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_48;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_49;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_50;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_51;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_52;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_53;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_54;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  char *pcVar11;
  undefined8 uVar12;
  CBasicString<char> *local_a4;
  uint local_a0;
  int local_9c;
  CStringParser local_98 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_8c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_88 [4];
  CGuiControlTreePane local_84 [12];
  int local_78;
  CCharString aCStack_70 [4];
  CCharString local_6c [4];
  CCharString aCStack_68 [4];
  CCharString local_64 [4];
  CCharString local_60 [4];
  CCharString aCStack_5c [4];
  CCharString local_58 [4];
  CCharString aCStack_54 [4];
  CCharString local_50 [4];
  CCharString aCStack_4c [4];
  CCharString local_48 [4];
  CCharString aCStack_44 [4];
  int local_40;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_3c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_38 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_34 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_30 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_2c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_28 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_24 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_20 [4];
  int local_1c;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_18 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_14 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_10 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_c [4];
  long local_8;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_4 [4];
  
  local_a0 = 0;
  CStringParser::CStringParser(local_98,*(CStringParser **)(this + 8));
  ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)(param_1 + 1);
  pcVar4 = param_1;
  do {
    cVar1 = *pcVar4;
    pcVar4 = pcVar4 + 1;
  } while (cVar1 != '\0');
  local_40 = (int)pcVar4 - (int)ppVar10;
LAB_009bb150:
  do {
    if (this[0x17] == (CPersistContext)0x0) {
      CCharString::CCharString(local_60,param_1,-1);
      local_a0 = local_a0 | 1;
      bVar3 = CThingCreatureBase::SerialiseGameState
                        ((CThingCreatureBase *)local_98,(CPersistContext *)local_60);
      ppVar10 = extraout_EDX;
      if (!bVar3) goto LAB_009bb18b;
      bVar3 = true;
    }
    else {
LAB_009bb18b:
      bVar3 = false;
    }
    if ((local_a0 & 1) != 0) {
      local_a0 = local_a0 & 0xfffffffe;
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_60,ppVar10,
                 unaff_EDI);
    }
    if (!bVar3) {
      CGuiControlTreePane::SortTreeRecursively
                (local_84,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)
                          unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_88,extraout_EDX_53,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_8c,extraout_EDX_54,unaff_EDI);
      NHeroInformationScreens::CBase::CBase((CBase *)local_98);
      return;
    }
    local_9c = local_78 - local_40;
    CStringParser::ReadAsString(local_98);
    pCVar2 = local_a4;
    if (local_a4 != (CBasicString<char> *)0x0) {
      bVar3 = CBasicString<char>::operator==(local_a4,".");
      if (bVar3) goto LAB_009bb222;
      bVar3 = CBasicString<char>::operator==(pCVar2,"[");
      ppVar10 = extraout_EDX_43;
      if (bVar3) goto LAB_009bb84c;
LAB_009bb9f5:
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_a4,ppVar10,
                 unaff_EDI);
      ppVar10 = extraout_EDX_52;
      goto LAB_009bb150;
    }
    iVar8 = 2;
    bVar3 = true;
    pcVar4 = ".";
    pcVar11 = "";
    do {
      if (iVar8 == 0) break;
      iVar8 = iVar8 + -1;
      bVar3 = *pcVar4 == *pcVar11;
      pcVar4 = pcVar4 + 1;
      pcVar11 = pcVar11 + 1;
    } while (bVar3);
    if (bVar3) {
LAB_009bb222:
      pCVar5 = (CCharString *)CStringParser::ReadAsString(local_98);
      CCharString::operator=((CCharString *)&local_a4,pCVar5);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_4,extraout_EDX_01,unaff_EDI);
      pCVar2 = local_a4;
      if (local_a4 == (CBasicString<char> *)0x0) {
        iVar8 = 4;
        bVar3 = true;
        pcVar4 = "Add";
        pcVar11 = "";
        do {
          if (iVar8 == 0) break;
          iVar8 = iVar8 + -1;
          bVar3 = *pcVar4 == *pcVar11;
          pcVar4 = pcVar4 + 1;
          pcVar11 = pcVar11 + 1;
        } while (bVar3);
        if (bVar3) goto LAB_009bb2f1;
        iVar8 = 7;
        bVar3 = true;
        pcVar4 = "resize";
        pcVar11 = "";
        do {
          if (iVar8 == 0) break;
          iVar8 = iVar8 + -1;
          bVar3 = *pcVar4 == *pcVar11;
          pcVar4 = pcVar4 + 1;
          pcVar11 = pcVar11 + 1;
        } while (bVar3);
        if (!bVar3) {
          iVar8 = 6;
          bVar3 = true;
          pcVar4 = "clear";
          pcVar11 = "";
          do {
            if (iVar8 == 0) break;
            iVar8 = iVar8 + -1;
            bVar3 = *pcVar4 == *pcVar11;
            pcVar4 = pcVar4 + 1;
            pcVar11 = pcVar11 + 1;
          } while (bVar3);
          if (bVar3) goto LAB_009bb680;
LAB_009bb2a0:
          pCVar5 = (CCharString *)CCharString_OperatorPlus_API(&local_a4);
          CDefinitionManager::Error((CDefinitionManager *)this,local_98,pCVar5);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_24,extraout_EDX_02,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_a4,
                     extraout_EDX_03,unaff_EDI);
          ppVar10 = extraout_EDX_04;
          goto LAB_009bb150;
        }
LAB_009bb4e8:
        puVar7 = (undefined4 *)CStringParser::ReadAsString(local_98);
        if ((CBasicString<char> *)*puVar7 == (CBasicString<char> *)0x0) {
          iVar8 = 2;
          ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
          bVar3 = true;
          pcVar4 = "(";
          pcVar11 = "";
          do {
            if (iVar8 == 0) break;
            iVar8 = iVar8 + -1;
            bVar3 = *pcVar4 == *pcVar11;
            pcVar4 = pcVar4 + 1;
            pcVar11 = pcVar11 + 1;
          } while (bVar3);
        }
        else {
          bVar3 = CBasicString<char>::operator==((CBasicString<char> *)*puVar7,"(");
          ppVar10 = extraout_EDX_19;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (local_18,ppVar10,unaff_EDI);
        if (bVar3 == false) {
          CCharString::CCharString(local_50,"Unable to find opening bracket.",-1);
          CDefinitionManager::Error((CDefinitionManager *)this,local_98,local_50);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_50,
                     extraout_EDX_26,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_a4,
                     extraout_EDX_27,unaff_EDI);
          ppVar10 = extraout_EDX_28;
          goto LAB_009bb150;
        }
        CPersistTraitsSLONG::TransferIn(this,local_98,&local_8);
        puVar7 = (undefined4 *)CStringParser::ReadAsString(local_98);
        if ((CBasicString<char> *)*puVar7 == (CBasicString<char> *)0x0) {
          iVar8 = 2;
          ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
          bVar3 = true;
          pcVar4 = ")";
          pcVar11 = "";
          do {
            if (iVar8 == 0) break;
            iVar8 = iVar8 + -1;
            bVar3 = *pcVar4 == *pcVar11;
            pcVar4 = pcVar4 + 1;
            pcVar11 = pcVar11 + 1;
          } while (bVar3);
        }
        else {
          bVar3 = CBasicString<char>::operator==((CBasicString<char> *)*puVar7,")");
          ppVar10 = extraout_EDX_20;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (local_28,ppVar10,unaff_EDI);
        ppVar10 = extraout_EDX_21;
        if (bVar3 == false) goto LAB_009bb9f5;
        (**(code **)(*(int *)param_2 + 4))(local_8);
        puVar7 = (undefined4 *)CStringParser::ReadAsString(local_98);
        if ((CBasicString<char> *)*puVar7 == (CBasicString<char> *)0x0) {
          iVar8 = 2;
          ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
          bVar3 = true;
          pcVar4 = ";";
          pcVar11 = "";
          do {
            if (iVar8 == 0) break;
            iVar8 = iVar8 + -1;
            bVar3 = *pcVar4 == *pcVar11;
            pcVar4 = pcVar4 + 1;
            pcVar11 = pcVar11 + 1;
          } while (bVar3);
        }
        else {
          bVar3 = CBasicString<char>::operator==((CBasicString<char> *)*puVar7,";");
          ppVar10 = extraout_EDX_22;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_20,ppVar10,unaff_EDI);
        if (bVar3 == false) {
          CCharString::CCharString(aCStack_70,"Unable to find terminating semicolon.",-1);
          CDefinitionManager::Error((CDefinitionManager *)this,local_98,aCStack_70);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_70,
                     extraout_EDX_23,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_a4,
                     extraout_EDX_24,unaff_EDI);
          ppVar10 = extraout_EDX_25;
          goto LAB_009bb150;
        }
      }
      else {
        bVar3 = CBasicString<char>::operator==(local_a4,"Add");
        if (!bVar3) {
          bVar3 = CBasicString<char>::operator==(pCVar2,"resize");
          if (bVar3) goto LAB_009bb4e8;
          bVar3 = CBasicString<char>::operator==(pCVar2,"clear");
          if (!bVar3) goto LAB_009bb2a0;
LAB_009bb680:
          puVar7 = (undefined4 *)CStringParser::ReadAsString(local_98);
          if ((CBasicString<char> *)*puVar7 == (CBasicString<char> *)0x0) {
            iVar8 = 2;
            ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
            bVar3 = true;
            pcVar4 = "(";
            pcVar11 = "";
            do {
              if (iVar8 == 0) break;
              iVar8 = iVar8 + -1;
              bVar3 = *pcVar4 == *pcVar11;
              pcVar4 = pcVar4 + 1;
              pcVar11 = pcVar11 + 1;
            } while (bVar3);
          }
          else {
            bVar3 = CBasicString<char>::operator==((CBasicString<char> *)*puVar7,"(");
            ppVar10 = extraout_EDX_29;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_3c,ppVar10,unaff_EDI);
          if (bVar3 == false) {
            CCharString::CCharString(local_64,"Unable to find opening bracket.",-1);
            CDefinitionManager::Error((CDefinitionManager *)this,local_98,local_64);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_64,
                       extraout_EDX_40,unaff_EDI);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_a4,
                       extraout_EDX_41,unaff_EDI);
            ppVar10 = extraout_EDX_42;
          }
          else {
            puVar7 = (undefined4 *)CStringParser::ReadAsString(local_98);
            if ((CBasicString<char> *)*puVar7 == (CBasicString<char> *)0x0) {
              iVar8 = 2;
              ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
              bVar3 = true;
              pcVar4 = ")";
              pcVar11 = "";
              do {
                if (iVar8 == 0) break;
                iVar8 = iVar8 + -1;
                bVar3 = *pcVar4 == *pcVar11;
                pcVar4 = pcVar4 + 1;
                pcVar11 = pcVar11 + 1;
              } while (bVar3);
            }
            else {
              bVar3 = CBasicString<char>::operator==((CBasicString<char> *)*puVar7,")");
              ppVar10 = extraout_EDX_30;
            }
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      (local_34,ppVar10,unaff_EDI);
            if (bVar3 == false) {
              CCharString::CCharString(local_6c,"Unable to find terminating semicolon.",-1);
              CDefinitionManager::Error((CDefinitionManager *)this,local_98,local_6c);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_6c,
                         extraout_EDX_37,unaff_EDI);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_a4,
                         extraout_EDX_38,unaff_EDI);
              ppVar10 = extraout_EDX_39;
            }
            else {
              puVar7 = (undefined4 *)CStringParser::ReadAsString(local_98);
              if ((CBasicString<char> *)*puVar7 == (CBasicString<char> *)0x0) {
                iVar8 = 2;
                ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
                bVar3 = true;
                pcVar4 = ";";
                pcVar11 = "";
                do {
                  if (iVar8 == 0) break;
                  iVar8 = iVar8 + -1;
                  bVar3 = *pcVar4 == *pcVar11;
                  pcVar4 = pcVar4 + 1;
                  pcVar11 = pcVar11 + 1;
                } while (bVar3);
              }
              else {
                bVar3 = CBasicString<char>::operator==((CBasicString<char> *)*puVar7,";");
                ppVar10 = extraout_EDX_31;
              }
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (local_2c,ppVar10,unaff_EDI);
              if (bVar3 == false) {
                CCharString::CCharString(local_58,"Unable to find terminating semicolon.",-1);
                CDefinitionManager::Error((CDefinitionManager *)this,local_98,local_58);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_58
                           ,extraout_EDX_34,unaff_EDI);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           &local_a4,extraout_EDX_35,unaff_EDI);
                ppVar10 = extraout_EDX_36;
              }
              else {
                (**(code **)(*(int *)param_2 + 4))(0);
                CleanOutLine(this,local_9c,local_78);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           &local_a4,extraout_EDX_32,unaff_EDI);
                ppVar10 = extraout_EDX_33;
              }
            }
          }
          goto LAB_009bb150;
        }
LAB_009bb2f1:
        puVar7 = (undefined4 *)CStringParser::ReadAsString(local_98);
        if ((CBasicString<char> *)*puVar7 == (CBasicString<char> *)0x0) {
          iVar8 = 2;
          ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
          bVar3 = true;
          pcVar4 = "(";
          pcVar11 = "";
          do {
            if (iVar8 == 0) break;
            iVar8 = iVar8 + -1;
            bVar3 = *pcVar4 == *pcVar11;
            pcVar4 = pcVar4 + 1;
            pcVar11 = pcVar11 + 1;
          } while (bVar3);
        }
        else {
          bVar3 = CBasicString<char>::operator==((CBasicString<char> *)*puVar7,"(");
          ppVar10 = extraout_EDX_05;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (local_38,ppVar10,unaff_EDI);
        if (bVar3 == false) {
          CCharString::CCharString(local_48,"Unable to find opening bracket.",-1);
          CDefinitionManager::Error((CDefinitionManager *)this,local_98,local_48);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_48,
                     extraout_EDX_16,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_a4,
                     extraout_EDX_17,unaff_EDI);
          ppVar10 = extraout_EDX_18;
          goto LAB_009bb150;
        }
        iVar8 = (*(code *)**(undefined4 **)param_2)();
        (**(code **)(*(int *)param_2 + 4))(iVar8 + 1);
        (**(code **)(*(int *)param_2 + 0xc))(this,&local_9c,iVar8);
        if (this[0x16] != (CPersistContext)0x0) {
          this[0x17] = (CPersistContext)0x1;
        }
        puVar7 = (undefined4 *)CStringParser::ReadAsString(local_98);
        if ((CBasicString<char> *)*puVar7 == (CBasicString<char> *)0x0) {
          iVar8 = 2;
          ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
          bVar3 = true;
          pcVar4 = ")";
          pcVar11 = "";
          do {
            if (iVar8 == 0) break;
            iVar8 = iVar8 + -1;
            bVar3 = *pcVar4 == *pcVar11;
            pcVar4 = pcVar4 + 1;
            pcVar11 = pcVar11 + 1;
          } while (bVar3);
        }
        else {
          bVar3 = CBasicString<char>::operator==((CBasicString<char> *)*puVar7,")");
          ppVar10 = extraout_EDX_06;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_10,ppVar10,unaff_EDI);
        if (bVar3 == false) {
          CCharString::CCharString(aCStack_68,"Unable to find terminating bracket.",-1);
          CDefinitionManager::Error((CDefinitionManager *)this,local_98,aCStack_68);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_68,
                     extraout_EDX_13,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_a4,
                     extraout_EDX_14,unaff_EDI);
          ppVar10 = extraout_EDX_15;
          goto LAB_009bb150;
        }
        puVar7 = (undefined4 *)CStringParser::ReadAsString(local_98);
        if ((CBasicString<char> *)*puVar7 == (CBasicString<char> *)0x0) {
          iVar8 = 2;
          ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
          bVar3 = true;
          pcVar4 = ";";
          pcVar11 = "";
          do {
            if (iVar8 == 0) break;
            iVar8 = iVar8 + -1;
            bVar3 = *pcVar4 == *pcVar11;
            pcVar4 = pcVar4 + 1;
            pcVar11 = pcVar11 + 1;
          } while (bVar3);
        }
        else {
          bVar3 = CBasicString<char>::operator==((CBasicString<char> *)*puVar7,";");
          ppVar10 = extraout_EDX_07;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_30,ppVar10,unaff_EDI);
        if (bVar3 == false) {
          CCharString::CCharString(aCStack_44,"Unable to find terminating semicolon.",-1);
          CDefinitionManager::Error((CDefinitionManager *)this,local_98,aCStack_44);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_44,
                     extraout_EDX_10,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_a4,
                     extraout_EDX_11,unaff_EDI);
          ppVar10 = extraout_EDX_12;
          goto LAB_009bb150;
        }
      }
      CleanOutLine(this,local_9c,local_78);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_a4,
                 extraout_EDX_08,unaff_EDI);
      ppVar10 = extraout_EDX_09;
      goto LAB_009bb150;
    }
    iVar8 = 2;
    bVar3 = true;
    pcVar4 = "[";
    pcVar11 = "";
    do {
      if (iVar8 == 0) break;
      iVar8 = iVar8 + -1;
      bVar3 = *pcVar4 == *pcVar11;
      pcVar4 = pcVar4 + 1;
      pcVar11 = pcVar11 + 1;
    } while (bVar3);
    if (bVar3) {
LAB_009bb84c:
      CPersistTraitsSLONG::TransferIn(this,local_98,&local_1c);
      iVar6 = (*(code *)**(undefined4 **)param_2)();
      iVar8 = local_1c;
      if (local_1c == iVar6) {
        (**(code **)(*(int *)param_2 + 4))(local_1c + 1);
      }
      iVar6 = (*(code *)**(undefined4 **)param_2)();
      if (iVar8 < iVar6) {
        puVar7 = (undefined4 *)CStringParser::ReadAsString(local_98);
        if ((CBasicString<char> *)*puVar7 == (CBasicString<char> *)0x0) {
          iVar6 = 2;
          ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
          bVar3 = true;
          pcVar4 = "]";
          pcVar11 = "";
          do {
            if (iVar6 == 0) break;
            iVar6 = iVar6 + -1;
            bVar3 = *pcVar4 == *pcVar11;
            pcVar4 = pcVar4 + 1;
            pcVar11 = pcVar11 + 1;
          } while (bVar3);
        }
        else {
          bVar3 = CBasicString<char>::operator==((CBasicString<char> *)*puVar7,"]");
          ppVar10 = extraout_EDX_44;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_14,ppVar10,unaff_EDI);
        if (bVar3 == false) {
          CCharString::CCharString(aCStack_54,"Unable to find terminating bracket.",-1);
          CDefinitionManager::Error((CDefinitionManager *)this,local_98,aCStack_54);
          paVar9 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_54;
          ppVar10 = extraout_EDX_49;
        }
        else {
          if (this[0x16] != (CPersistContext)0x0) {
            this[0x17] = (CPersistContext)0x1;
          }
          (**(code **)(*(int *)param_2 + 0xc))(this,local_98,iVar8);
          uVar12 = CStringParser::ReadAsString(local_98);
          ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar12 >> 0x20);
          if ((CBasicString<char> *)*(undefined4 *)uVar12 == (CBasicString<char> *)0x0) {
            iVar8 = 2;
            bVar3 = true;
            pcVar4 = ";";
            pcVar11 = "";
            do {
              if (iVar8 == 0) break;
              iVar8 = iVar8 + -1;
              bVar3 = *pcVar4 == *pcVar11;
              pcVar4 = pcVar4 + 1;
              pcVar11 = pcVar11 + 1;
            } while (bVar3);
          }
          else {
            bVar3 = CBasicString<char>::operator==((CBasicString<char> *)*(undefined4 *)uVar12,";");
            ppVar10 = extraout_EDX_45;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (aaStack_c,ppVar10,unaff_EDI);
          if (bVar3 != false) {
            CleanOutLine(this,local_9c,local_78);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_a4,
                       extraout_EDX_46,unaff_EDI);
            ppVar10 = extraout_EDX_47;
            goto LAB_009bb150;
          }
          CCharString::CCharString(aCStack_5c,"Unable to find terminating semicolon.",-1);
          CDefinitionManager::Error((CDefinitionManager *)this,local_98,aCStack_5c);
          paVar9 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_5c;
          ppVar10 = extraout_EDX_48;
        }
      }
      else {
        CCharString::CCharString(aCStack_4c,"Out of bounds vector access",-1);
        CDefinitionManager::Error((CDefinitionManager *)this,local_98,aCStack_4c);
        paVar9 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_4c;
        ppVar10 = extraout_EDX_50;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (paVar9,ppVar10,unaff_EDI);
      ppVar10 = extraout_EDX_51;
      goto LAB_009bb9f5;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_a4,
               (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0,unaff_EDI);
    ppVar10 = extraout_EDX_00;
  } while( true );
}



//=== TransferObjectLoadText @ 009bba40 ===

/* [bsim sim=0.5956420676088097 <- ego_r]
   protected: void __thiscall CPersistContext::TransferObjectLoadText(char const *,class
   CPersistContext_TransferableComponentGenericType &) */

void __thiscall
CPersistContext::TransferObjectLoadText
          (CPersistContext *this,char *param_1,
          CPersistContext_TransferableComponentGenericType *param_2)

{
  char cVar1;
  bool bVar2;
  bool bVar3;
  char *pcVar4;
  undefined4 *puVar5;
  int iVar6;
  undefined1 *puVar7;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar8;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  int iVar9;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  char *pcVar10;
  CCharString local_3c [4];
  CCharString aCStack_38 [4];
  int local_34;
  CPersistContext *local_30;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_2c [4];
  CStringParser local_28 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [4];
  CGuiControlTreePane aCStack_14 [12];
  pair<EHeroMorphType,CParticleMorphs::CEntry> *local_8;
  
  bVar3 = false;
  local_30 = this;
  CStringParser::CStringParser(local_28,*(CStringParser **)(this + 8));
  pcVar4 = param_1;
  do {
    cVar1 = *pcVar4;
    pcVar4 = pcVar4 + 1;
  } while (cVar1 != '\0');
  local_34 = (int)pcVar4 - (int)(param_1 + 1);
  ppVar8 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)(param_1 + 1);
  do {
    if (this[0x17] == (CPersistContext)0x0) {
      CCharString::CCharString(local_3c,param_1,-1);
      bVar3 = true;
      bVar2 = CThingCreatureBase::SerialiseGameState
                        ((CThingCreatureBase *)local_28,(CPersistContext *)local_3c);
      ppVar8 = extraout_EDX;
      if (!bVar2) goto LAB_009bbab8;
      bVar2 = true;
    }
    else {
LAB_009bbab8:
      bVar2 = false;
    }
    if (bVar3) {
      bVar3 = false;
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_3c,ppVar8,
                 unaff_EDI);
    }
    if (!bVar2) {
      CGuiControlTreePane::SortTreeRecursively
                (aCStack_14,
                 (list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (aaStack_18,extraout_EDX_03,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (aaStack_1c,extraout_EDX_04,unaff_EDI);
      NHeroInformationScreens::CBase::CBase((CBase *)local_28);
      return;
    }
    iVar9 = (int)local_8 - local_34;
    (**(code **)(*(int *)param_2 + 4))(this,local_28);
    if (this[0x16] != (CPersistContext)0x0) {
      this[0x17] = (CPersistContext)0x1;
    }
    puVar5 = (undefined4 *)CStringParser::ReadAsString(local_28);
    if ((CBasicString<char> *)*puVar5 == (CBasicString<char> *)0x0) {
      iVar6 = 2;
      ppVar8 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
      bVar2 = true;
      pcVar4 = ";";
      pcVar10 = "";
      do {
        this = local_30;
        if (iVar6 == 0) break;
        iVar6 = iVar6 + -1;
        bVar2 = *pcVar4 == *pcVar10;
        pcVar4 = pcVar4 + 1;
        pcVar10 = pcVar10 + 1;
      } while (bVar2);
    }
    else {
      bVar2 = CBasicString<char>::operator==((CBasicString<char> *)*puVar5,";");
      ppVar8 = extraout_EDX_00;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (aaStack_2c,ppVar8,unaff_EDI);
    if (bVar2 == false) {
      CCharString::CCharString(aCStack_38,"Unable to find terminating semicolon.",-1);
      CDefinitionManager::Error((CDefinitionManager *)this,local_28,aCStack_38);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_38,
                 extraout_EDX_01,unaff_EDI);
      ppVar8 = extraout_EDX_02;
    }
    else {
      ppVar8 = local_8;
      if (*(undefined4 **)(this + 0x10) != (undefined4 *)0x0) {
        puVar5 = (undefined4 *)**(undefined4 **)(this + 0x10);
        puVar7 = &DAT_0129aaf4;
        if (puVar5 != (undefined4 *)0x0) {
          puVar7 = (undefined1 *)*puVar5;
        }
        for (; iVar9 < (int)local_8; iVar9 = iVar9 + 1) {
          if (puVar7[iVar9] != '\n') {
            puVar7[iVar9] = 0x20;
          }
        }
      }
    }
  } while( true );
}



//=== TransferSubComponentLoadText @ 009bbcc0 ===

/* [bsim sim=0.5274615841326282 <- ego_r]
   protected: void __thiscall CPersistContext::TransferSubComponentLoadText(char const *,class
   CPersistContext_TransferableSubComponentGenericType &) */

void __thiscall
CPersistContext::TransferSubComponentLoadText
          (CPersistContext *this,char *param_1,
          CPersistContext_TransferableSubComponentGenericType *param_2)

{
  int *piVar1;
  bool bVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar3;
  CPersistContext *pCVar4;
  _func_void_CCharString_ptr *p_Var5;
  bool bVar6;
  bool bVar7;
  char cVar8;
  char *pcVar9;
  undefined4 *puVar10;
  undefined4 uVar11;
  int iVar12;
  undefined1 *puVar13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_08;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_09;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_10;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_11;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_12;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_15;
  int *_Memory;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  char *pcVar15;
  undefined4 uVar16;
  undefined4 *local_a4;
  CPersistContext *local_a0;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *local_9c;
  CCharString local_98 [4];
  pair<EHeroMorphType,CParticleMorphs::CEntry> *local_94;
  CCharString local_90 [4];
  CCharString local_8c [4];
  CCharString local_88 [4];
  undefined **local_84 [3];
  CCharString local_78 [4];
  CCharString local_74 [4];
  int *local_70 [5];
  undefined4 uStack_5c;
  undefined4 uStack_58;
  undefined ***pppuStack_54;
  _func_void_CCharString_ptr *p_Stack_50;
  undefined4 **ppuStack_4c;
  char cStack_48;
  undefined1 uStack_47;
  undefined1 uStack_46;
  undefined1 uStack_45;
  undefined4 uStack_44;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_30 [4];
  int local_2c;
  CStringParser local_28 [12];
  CCharString local_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [4];
  CGuiControlTreePane local_14 [4];
  _func_void_CCharString_ptr *local_10;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *local_8;
  
  bVar7 = false;
  bVar2 = false;
  pcVar9 = param_1;
  do {
    cVar8 = *pcVar9;
    pcVar9 = pcVar9 + 1;
  } while (cVar8 != '\0');
  local_2c = (int)pcVar9 - (int)(param_1 + 1);
  local_a0 = this;
  CStringParser::CStringParser(local_28,*(CStringParser **)(this + 8));
  ppVar14 = extraout_EDX;
  do {
    if (*(CDefinitionManager *)(this + 0x17) == (CDefinitionManager)0x0) {
      CCharString::CCharString(local_8c,param_1,-1);
      bVar7 = true;
      bVar6 = CThingCreatureBase::SerialiseGameState
                        ((CThingCreatureBase *)local_28,(CPersistContext *)local_8c);
      ppVar14 = extraout_EDX_00;
      if (!bVar6) goto LAB_009bbd4e;
      bVar6 = true;
    }
    else {
LAB_009bbd4e:
      bVar6 = false;
    }
    if (bVar7) {
      bVar7 = false;
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_8c,ppVar14,
                 unaff_EDI);
    }
    ppVar14 = local_8;
    if (!bVar6) {
      if ((!bVar2) && (cVar8 = (**(code **)(*(int *)param_2 + 4))(), cVar8 == '\0')) {
        uVar16 = 0;
        uVar11 = CCharString::CCharString((CCharString *)&local_9c,"",-1);
        FUN_00414e30(&DAT_0122d70e,0,uVar11,uVar16);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_9c,
                   extraout_EDX_11,unaff_EDI);
        pppuStack_54 = local_84;
        uStack_44 = 0;
        uStack_5c = 0;
        uStack_58 = 0;
        p_Stack_50 = local_10;
        ppuStack_4c = (undefined4 **)0x0;
        cStack_48 = '\x01';
        uStack_47 = 0;
        uStack_45 = 0;
        uStack_46 = 0;
        (*(code *)**(undefined4 **)param_2)(&uStack_5c);
        (**(code **)(*(int *)param_2 + 8))(1);
        CGuiControlTreePane::SortTreeRecursively
                  ((CGuiControlTreePane *)local_70,
                   (list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EDI);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_74,
                   extraout_EDX_12,unaff_EDI);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_78,
                   extraout_EDX_13,unaff_EDI);
        NHeroInformationScreens::CBase::CBase((CBase *)local_84);
      }
LAB_009bc0d5:
      CGuiControlTreePane::SortTreeRecursively
                (local_14,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)
                          unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (aaStack_18,extraout_EDX_14,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_1c,
                 extraout_EDX_15,unaff_EDI);
      NHeroInformationScreens::CBase::CBase((CBase *)local_28);
      return;
    }
    local_9c = local_8 + -local_2c;
    bVar2 = true;
    local_94 = local_9c;
    CCharString::CCharString(local_88,";",-1);
    bVar6 = CThingCreatureBase::SerialiseGameState
                      ((CThingCreatureBase *)local_28,(CPersistContext *)local_88);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_88,
               extraout_EDX_01,unaff_EDI);
    if (bVar6) {
      local_94 = local_8;
    }
    else {
      CCharString::CCharString(local_90,"Unable to find terminating semicolon.",-1);
      CDefinitionManager::Error((CDefinitionManager *)this,local_28,local_90);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_90,
                 extraout_EDX_02,unaff_EDI);
    }
    CStringParser::SetStringPos(local_28,(ulong)ppVar14);
    puVar10 = (undefined4 *)CStringParser::ReadAsString(local_28);
    if ((CBasicString<char> *)*puVar10 == (CBasicString<char> *)0x0) {
      iVar12 = 2;
      ppVar14 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
      bVar6 = true;
      pcVar9 = ".";
      pcVar15 = "";
      do {
        this = local_a0;
        if (iVar12 == 0) break;
        iVar12 = iVar12 + -1;
        bVar6 = *pcVar9 == *pcVar15;
        pcVar9 = pcVar9 + 1;
        pcVar15 = pcVar15 + 1;
      } while (bVar6);
    }
    else {
      bVar6 = CBasicString<char>::operator==((CBasicString<char> *)*puVar10,".");
      ppVar14 = extraout_EDX_03;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (local_30,ppVar14,unaff_EDI);
    if (bVar6 == false) goto LAB_009bc0d5;
    CCharString::CCharString(local_98,";",-1);
    CStringParser::ReadAsStringUntilString(local_28,(CCharString *)&local_a4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_98,
               extraout_EDX_04,unaff_EDI);
    ppVar14 = local_8;
    p_Var5 = local_10;
    NHeroInformationScreens::CBase::CBase((CBase *)local_84);
    local_84[0] = &PTR__vector_deleting_destructor__0122d874;
    CCharString::CCharString(local_78);
    CCharString::CCharString(local_74);
    local_70[0] = (int *)0x0;
    local_70[0] = malloc(0x10);
    *local_70[0] = (int)local_70[0];
    local_70[0][1] = (int)local_70[0];
    CCharString::operator=(local_74,(CCharString *)&local_a4);
    if (local_a4 == (undefined4 *)0x0) {
      pcVar9 = "";
    }
    else {
      pcVar9 = (char *)*local_a4;
    }
    CStringParser::Init((CStringParser *)local_84,pcVar9,p_Var5,local_1c,(long)ppVar14);
    uVar11 = *(undefined4 *)(this + 4);
    uVar16 = *(undefined4 *)this;
    cVar8 = (**(code **)(*(int *)param_2 + 4))();
    pCVar4 = local_a0;
    cStack_48 = '\x01' - (cVar8 != '\0');
    p_Stack_50 = *(_func_void_CCharString_ptr **)(local_a0 + 0xc);
    pppuStack_54 = local_84;
    ppuStack_4c = &local_a4;
    uStack_44 = 0;
    uStack_47 = 0;
    uStack_45 = 0;
    uStack_46 = 1;
    uStack_5c = uVar16;
    uStack_58 = uVar11;
    (*(code *)**(undefined4 **)param_2)(&uStack_5c);
    (**(code **)(*(int *)param_2 + 8))(1);
    CheckUnknownSymbols((CPersistContext *)&uStack_5c);
    ppVar14 = extraout_EDX_05;
    if (*(undefined4 **)(pCVar4 + 0x10) != (undefined4 *)0x0) {
      puVar10 = (undefined4 *)**(undefined4 **)(pCVar4 + 0x10);
      puVar13 = &DAT_0129aaf4;
      ppVar3 = local_9c;
      if (puVar10 != (undefined4 *)0x0) {
        puVar13 = (undefined1 *)*puVar10;
      }
      for (; ppVar14 = local_94, (int)ppVar3 < (int)local_94; ppVar3 = ppVar3 + 1) {
        if (ppVar3[(int)puVar13] != (pair<EHeroMorphType,CParticleMorphs::CEntry>)0xa) {
          ppVar3[(int)puVar13] = (pair<EHeroMorphType,CParticleMorphs::CEntry>)0x20;
        }
      }
    }
    _Memory = (int *)*local_70[0];
    if (_Memory != local_70[0]) {
      do {
        piVar1 = (int *)*_Memory;
        free(_Memory);
        ppVar14 = extraout_EDX_06;
        _Memory = piVar1;
      } while (piVar1 != local_70[0]);
    }
    *local_70[0] = (int)local_70[0];
    local_70[0][1] = (int)local_70[0];
    if (local_70[0] != (int *)0x0) {
      free(local_70[0]);
      ppVar14 = extraout_EDX_07;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_74,ppVar14,
               unaff_EDI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_78,
               extraout_EDX_08,unaff_EDI);
    NHeroInformationScreens::CBase::CBase((CBase *)local_84);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_a4,
               extraout_EDX_09,unaff_EDI);
    ppVar14 = extraout_EDX_10;
    this = local_a0;
  } while( true );
}



//=== ~CBankFileEntryUpdateData @ 009bdb40 ===

/* [bsim sim=0.6538190436183535 <- ego_r]
   private: __thiscall CBankFileEntryUpdateData::~CBankFileEntryUpdateData(void) */

void __thiscall CBankFileEntryUpdateData::~CBankFileEntryUpdateData(CBankFileEntryUpdateData *this)

{
  int *piVar1;
  
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129b3bc;
  FUN_009be060();
  if (*(void **)(this + 0x58) != (void *)0x0) {
    free(*(void **)(this + 0x58));
  }
  piVar1 = *(int **)(this + 0x48);
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + -1;
    if (**(int **)(this + 0x48) == 0) {
      (*(code *)(*(int **)(this + 0x48))[1])();
      operator_delete(*(void **)(this + 0x48));
    }
  }
  *(undefined4 *)(this + 0x44) = 0;
  *(undefined4 *)(this + 0x48) = 0;
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  return;
}



//=== ?OpenReadWrite@CBankFile@@UAEXABVCCharString@@KK@Z @ 009cb960 ===

/* [bsim sim=0.9270423485290534 <- ego_r]
   public: virtual void __thiscall CBankFile::OpenReadWrite(class CCharString const &,unsigned
   long,unsigned long) */

void __thiscall
CBankFile::_OpenReadWrite_CBankFile__UAEXABVCCharString__KK_Z
          (CBankFile *this,CCharString *param_1,ulong param_2,ulong param_3)

{
  CCharString *pCVar1;
  ulong uVar2;
  CWideString *pCVar3;
  
  pCVar1 = param_1;
  this[0x8d] = (CBankFile)0x0;
  *(ulong *)(this + 0x78) = param_2 | 0x1f8;
  CCharString::operator=((CCharString *)(this + 0x88),param_1);
  uVar2 = param_3;
  *(undefined4 *)(this + 0x90) = 0;
  this[0x94] = (CBankFile)0x0;
  this[0x8e] = (CBankFile)0x0;
  *(ulong *)(this + 0x98) = param_3;
  this[0x95] = (CBankFile)0x0;
  if ((DAT_013ca7b0 != '\0') && ((param_2 & 4) == 0)) {
    this[0x8c] = (CBankFile)0x1;
    this[0x8f] = (CBankFile)0x1;
    pCVar3 = (CWideString *)FUN_009a7d40(&param_2,pCVar1);
    CWideString::operator=((CWideString *)(this + 0x84),pCVar3);
    CCharString::~CCharString((CCharString *)&param_2);
    CTCInventoryExperience::IsTutorialPlayed((CTCInventoryExperience *)&DAT_013ca79c,pCVar1);
    (**(code **)(*(int *)this + 0x2c))();
    return;
  }
  FUN_009a7ca0(&param_1,pCVar1);
  (**(code **)(*(int *)this + 0x10))(&param_1,param_2,uVar2);
  CCharString::~CCharString((CCharString *)&stack0xfffffff8);
  return;
}



//=== ?AdjustUpdateEntries@CBankFile@@MAEKXZ @ 009cba70 ===

undefined4 _AdjustUpdateEntries_CBankFile__MAEKXZ(void)

{
  return 0;
}



//=== ?NotifyBankSize@CBankFile@@MAEXKABV?$CArray@VCBankFileDataTypeCount@@@@@Z @ 009cbbf0 ===

void _NotifyBankSize_CBankFile__MAEXKABV__CArray_VCBankFileDataTypeCount_____Z(void)

{
  return;
}



//=== ?NotifyBankOpened@CBankFile@@MAEXXZ @ 009cbc00 ===

void _NotifyBankOpened_CBankFile__MAEXXZ(void)

{
  return;
}



//=== ?NotifyEntryUpdated@CBankFile@@MAEXKKABV?$CArray@D@@@Z @ 009cbc10 ===

void _NotifyEntryUpdated_CBankFile__MAEXKKABV__CArray_D___Z(void)

{
  return;
}



//=== ?NotifyEntryAdded@CBankFile@@MAEXKKABV?$CArray@D@@@Z @ 009cbc20 ===

/* [bsim sim=1.0 <- ego_r] */

void _NotifyEntryAdded_CBankFile__MAEXKKABV__CArray_D___Z(void)

{
  return;
}



//=== ?NotifyEntryRemoved@CBankFile@@MAEXK@Z @ 009cbc30 ===

void _NotifyEntryRemoved_CBankFile__MAEXK_Z(void)

{
  return;
}



//=== ?IsDiskFile@CBankFile@@MAE_NKK@Z @ 009cbc40 ===

undefined1 _IsDiskFile_CBankFile__MAE_NKK_Z(void)

{
  return 1;
}



//=== ?GetLastModifiedTime@CBankFile@@MAE?AVCDateAndTime@@ABVCCharString@@@Z @ 009cbc50 ===

undefined4 *
_GetLastModifiedTime_CBankFile__MAE_AVCDateAndTime__ABVCCharString___Z(undefined4 *param_1)

{
  CWideString *pCVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  FileAccessType FVar6;
  ulong uVar7;
  CCharString aCStack_94 [4];
  undefined **appuStack_90 [5];
  undefined1 uStack_7c;
  CCharString aCStack_78 [4];
  undefined4 auStack_74 [29];
  
  NHeroInformationScreens::CBase::CBase((CBase *)appuStack_90);
  appuStack_90[0] = &PTR__vector_deleting_destructor__0122d06c;
  CCharString::CCharString(aCStack_78);
  uVar7 = 2;
  FVar6 = 1;
  uStack_7c = 0;
  pCVar1 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
  CDiskFileWin32::_Open_CDiskFileWin32__UAEXABVCWideString__W4FileAccessType__K_Z
            ((CDiskFileWin32 *)appuStack_90,pCVar1,FVar6,uVar7);
  CCharString::~CCharString(aCStack_94);
  iVar2 = CDiskFileWin32::_GetFileInformation_CDiskFileWin32__UAE_AVCFileInformation__XZ
                    ((CDiskFileWin32 *)appuStack_90);
  puVar4 = (undefined4 *)(iVar2 + 0x38);
  puVar5 = auStack_74;
  for (iVar3 = 7; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar5 = *puVar4;
    puVar4 = puVar4 + 1;
    puVar5 = puVar5 + 1;
  }
  CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)appuStack_90);
  puVar4 = auStack_74;
  puVar5 = param_1;
  for (iVar2 = 7; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar5 = *puVar4;
    puVar4 = puVar4 + 1;
    puVar5 = puVar5 + 1;
  }
  appuStack_90[0] = &PTR__vector_deleting_destructor__0122d06c;
  CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)appuStack_90);
  CCharString::~CCharString(aCStack_78);
  NHeroInformationScreens::CBase::CBase((CBase *)appuStack_90);
  return param_1;
}



//=== ?PrepareDataForInclusion@CBankFile@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z @ 009cc9f0 ===

void _PrepareDataForInclusion_CBankFile__MAEXKABV__CArray_VCCharString____KV__CCountedPointer_VCBankStateBlock____AAVCDataOutputStream__2_Z
               (undefined4 param_1,undefined4 param_2,int *param_3,undefined4 param_4,
               undefined4 param_5,int *param_6)

{
  int *piVar1;
  CWideString *pCVar2;
  undefined4 ***pppuVar3;
  uint uVar4;
  void *unaff_ESI;
  FileAccessType FVar5;
  ulong uVar6;
  undefined4 **local_28;
  undefined4 **local_24;
  undefined1 *local_20;
  undefined **local_1c [3];
  uint local_10;
  CCharString aCStack_c [4];
  undefined1 local_8;
  CCharString local_4 [4];
  
  NHeroInformationScreens::CBase::CBase((CBase *)local_1c);
  local_1c[0] = &PTR__vector_deleting_destructor__0122d06c;
  CCharString::CCharString(local_4);
  uVar6 = 4;
  FVar5 = 1;
  local_8 = 0;
  pCVar2 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
  CDiskFileWin32::_Open_CDiskFileWin32__UAEXABVCWideString__W4FileAccessType__K_Z
            ((CDiskFileWin32 *)local_1c,pCVar2,FVar5,uVar6);
  CCharString::~CCharString((CCharString *)&param_2);
  local_28 = (undefined4 **)0x0;
  local_24 = (undefined4 **)0x0;
  local_20 = (undefined1 *)0x0;
  if (local_10 == 0) {
    local_28 = (undefined4 ***)0x0;
  }
  else {
    local_28 = malloc(local_10);
  }
  local_20 = (undefined1 *)((int)local_28 + local_10);
  pppuVar3 = (undefined4 ***)local_28;
  for (uVar4 = local_10 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
    *pppuVar3 = (undefined4 **)0x0;
    pppuVar3 = pppuVar3 + 1;
  }
  for (uVar4 = local_10 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *(undefined1 *)pppuVar3 = 0;
    pppuVar3 = (undefined4 ***)((int)pppuVar3 + 1);
  }
  local_24 = (undefined4 **)((int)local_28 + local_10);
  pppuVar3 = (undefined4 ***)local_28;
  if (local_24 == local_28) {
    pppuVar3 = &local_28;
  }
  CDiskFileWin32::_Read_CDiskFileWin32__UAEXPAXJPAVCASuspendableProcess___Z
            ((CDiskFileWin32 *)local_1c,pppuVar3,local_10,(CASuspendableProcess *)0x0);
  pppuVar3 = (undefined4 ***)local_28;
  if (local_24 == local_28) {
    pppuVar3 = &local_28;
  }
  (**(code **)(*param_6 + 0x10))(pppuVar3,local_10);
  CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)&local_24);
  if (unaff_ESI != (void *)0x0) {
    free(unaff_ESI);
  }
  local_24 = (undefined4 **)&PTR__vector_deleting_destructor__0122d06c;
  CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)&local_24);
  CCharString::~CCharString(aCStack_c);
  NHeroInformationScreens::CBase::CBase((CBase *)&local_24);
  piVar1 = param_3;
  if ((param_3 != (int *)0x0) && (*param_3 = *param_3 + -1, *param_3 == 0)) {
    (*(code *)param_3[1])();
    operator_delete(piVar1);
  }
  return;
}



//=== CBankFile @ 009cd480 ===

/* [bsim sim=0.5235967231785248 <- ego_r]
   public: __thiscall CBankFile::CBankFile(void) */

CBankFile * __thiscall CBankFile::CBankFile(CBankFile *this)

{
  undefined1 *puVar1;
  
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129b54c;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  CPackedUIntArray::CPackedUIntArray((CPackedUIntArray *)(this + 0x38));
  CPackedUIntArray::CPackedUIntArray((CPackedUIntArray *)(this + 0x48));
  CPackedUIntArray::CPackedUIntArray((CPackedUIntArray *)(this + 0x58));
  CPackedUIntArray::CPackedUIntArray((CPackedUIntArray *)(this + 0x68));
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  CCharString::CCharString((CCharString *)(this + 0x84));
  CCharString::CCharString((CCharString *)(this + 0x88));
  *(undefined4 *)(this + 0xac) = 0;
  puVar1 = malloc(0x18);
  *(undefined1 **)(this + 0xac) = puVar1;
  *(undefined4 *)(this + 0xb0) = 0;
  *puVar1 = 0;
  *(undefined4 *)(*(int *)(this + 0xac) + 4) = 0;
  *(int *)(*(int *)(this + 0xac) + 8) = *(int *)(this + 0xac);
  *(int *)(*(int *)(this + 0xac) + 0xc) = *(int *)(this + 0xac);
  *(undefined4 *)(this + 0xb8) = 0;
  puVar1 = malloc(0x18);
  *(undefined1 **)(this + 0xb8) = puVar1;
  *(undefined4 *)(this + 0xbc) = 0;
  *puVar1 = 0;
  *(undefined4 *)(*(int *)(this + 0xb8) + 4) = 0;
  *(int *)(*(int *)(this + 0xb8) + 8) = *(int *)(this + 0xb8);
  *(int *)(*(int *)(this + 0xb8) + 0xc) = *(int *)(this + 0xb8);
  *(undefined4 *)(this + 0xc4) = 0;
  puVar1 = malloc(0x18);
  *(undefined1 **)(this + 0xc4) = puVar1;
  *(undefined4 *)(this + 200) = 0;
  *puVar1 = 0;
  *(undefined4 *)(*(int *)(this + 0xc4) + 4) = 0;
  *(int *)(*(int *)(this + 0xc4) + 8) = *(int *)(this + 0xc4);
  *(int *)(*(int *)(this + 0xc4) + 0xc) = *(int *)(this + 0xc4);
  *(undefined4 *)(this + 0xd0) = 0;
  *(undefined4 *)(this + 0xd4) = 0;
  *(undefined4 *)(this + 0xd8) = 0;
  this[0xdd] = (CBankFile)0x0;
  *(undefined4 *)(this + 0xe0) = 0;
  *(undefined4 *)(this + 0xe4) = 0;
  *(undefined4 *)(this + 0xe8) = 0;
  CCharString::CCharString((CCharString *)(this + 0xf0));
  FUN_00a60c90(0x10000);
  *(undefined4 *)(this + 0xa0) = 0;
  *(undefined4 *)(this + 0xa4) = 0;
  *(undefined4 *)(this + 0xa8) = 0;
  *(undefined4 *)(this + 0xec) = 0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 0x9c) = 0x73;
  *(undefined4 *)(this + 0x98) = 1;
  return this;
}



//=== ?Close@CBankFile@@UAEXXZ @ 009ce360 ===

/* WARNING: Removing unreachable block (ram,0x009cd911) */
/* WARNING: Removing unreachable block (ram,0x009cd92e) */
/* WARNING: Removing unreachable block (ram,0x009cd8af) */
/* WARNING: Removing unreachable block (ram,0x009cd9c9) */
/* WARNING: Removing unreachable block (ram,0x009cd9d0) */

void _Close_CBankFile__UAEXXZ(void)

{
  LTextTreeWalkThrough *pLVar1;
  void *pvVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  int in_ECX;
  undefined4 *puVar6;
  
  puVar6 = *(undefined4 **)(in_ECX + 0x2c);
  if (puVar6 != *(undefined4 **)(in_ECX + 0x30)) {
    do {
      pvVar2 = (void *)*puVar6;
      if (pvVar2 != (void *)0x0) {
        if (*(void **)((int)pvVar2 + 8) != (void *)0x0) {
          operator_delete__(*(void **)((int)pvVar2 + 8));
          *(undefined4 *)((int)pvVar2 + 8) = 0;
        }
        *(undefined4 *)((int)pvVar2 + 4) = 0;
        CSmallVector<CCharString,8>::Clear((CSmallVector<CCharString,8> *)((int)pvVar2 + 0xc));
        operator_delete(pvVar2);
      }
      puVar6 = puVar6 + 1;
    } while (puVar6 != *(undefined4 **)(in_ECX + 0x30));
  }
  if (*(int *)(in_ECX + 0x34) - *(int *)(in_ECX + 0x2c) >> 2 != 0) {
    *(int *)(in_ECX + 0x30) = *(int *)(in_ECX + 0x2c);
    pvVar2 = *(void **)(in_ECX + 0x2c);
    *(undefined4 *)(in_ECX + 0x2c) = 0;
    *(undefined4 *)(in_ECX + 0x30) = 0;
    *(undefined4 *)(in_ECX + 0x34) = 0;
    if (pvVar2 != (void *)0x0) {
      free(pvVar2);
    }
  }
  iVar5 = *(int *)(in_ECX + 0x28) - *(int *)(in_ECX + 0x20);
  iVar4 = iVar5 >> 0x1f;
  if (iVar5 / 0xc + iVar4 != iVar4) {
    pvVar2 = *(void **)(in_ECX + 0x20);
    *(int *)(in_ECX + 0x24) = *(int *)(in_ECX + 0x20);
    *(undefined4 *)(in_ECX + 0x20) = 0;
    *(undefined4 *)(in_ECX + 0x24) = 0;
    *(undefined4 *)(in_ECX + 0x28) = 0;
    if (pvVar2 != (void *)0x0) {
      free(pvVar2);
    }
  }
  CArray<CCharString>::~CArray<CCharString>((CArray<CCharString> *)(in_ECX + 8));
  piVar3 = *(int **)(in_ECX + 0x80);
  if (piVar3 != (int *)0x0) {
    *piVar3 = *piVar3 + -1;
    if (**(int **)(in_ECX + 0x80) == 0) {
      (*(code *)(*(int **)(in_ECX + 0x80))[1])();
      operator_delete(*(void **)(in_ECX + 0x80));
    }
  }
  pLVar1 = (LTextTreeWalkThrough *)(in_ECX + 0xac);
  *(undefined4 *)(in_ECX + 0x80) = 0;
  *(undefined4 *)(in_ECX + 0x7c) = 0;
  if (*(int *)(in_ECX + 0xb0) != 0) {
    LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
              (pLVar1,*(LTextBinNode **)(*(int *)pLVar1 + 4));
    *(int *)(*(int *)pLVar1 + 8) = *(int *)pLVar1;
    *(undefined4 *)(*(int *)pLVar1 + 4) = 0;
    *(int *)(*(int *)pLVar1 + 0xc) = *(int *)pLVar1;
    *(undefined4 *)(in_ECX + 0xb0) = 0;
  }
  pLVar1 = (LTextTreeWalkThrough *)(in_ECX + 0xb8);
  *(undefined4 *)(in_ECX + 0xd4) = *(undefined4 *)(in_ECX + 0xd0);
  if (*(int *)(in_ECX + 0xbc) != 0) {
    LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
              (pLVar1,*(LTextBinNode **)(*(int *)pLVar1 + 4));
    *(int *)(*(int *)pLVar1 + 8) = *(int *)pLVar1;
    *(undefined4 *)(*(int *)pLVar1 + 4) = 0;
    *(int *)(*(int *)pLVar1 + 0xc) = *(int *)pLVar1;
    *(undefined4 *)(in_ECX + 0xbc) = 0;
  }
  pLVar1 = (LTextTreeWalkThrough *)(in_ECX + 0xc4);
  if (*(int *)(in_ECX + 200) != 0) {
    LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
              (pLVar1,*(LTextBinNode **)(*(int *)pLVar1 + 4));
    *(int *)(*(int *)pLVar1 + 8) = *(int *)pLVar1;
    *(undefined4 *)(*(int *)pLVar1 + 4) = 0;
    *(int *)(*(int *)pLVar1 + 0xc) = *(int *)pLVar1;
    *(undefined4 *)(in_ECX + 200) = 0;
  }
  CPackedUIntArray::~CPackedUIntArray((CPackedUIntArray *)(in_ECX + 0x38));
  CPackedUIntArray::~CPackedUIntArray((CPackedUIntArray *)(in_ECX + 0x48));
  CPackedUIntArray::~CPackedUIntArray((CPackedUIntArray *)(in_ECX + 0x58));
  CPackedUIntArray::~CPackedUIntArray((CPackedUIntArray *)(in_ECX + 0x68));
  *(undefined4 *)(in_ECX + 4) = 0;
  return;
}



//=== ?Update@CBankFile@@UAEXXZ @ 009cec60 ===

/* [bsim sim=0.8154635055238303 <- ego_r]
   public: virtual void __thiscall CBankFile::Update(void) */

void __thiscall CBankFile::_Update_CBankFile__UAEXXZ(CBankFile *this)

{
  int iVar1;
  CWideString *pCVar2;
  int iVar3;
  ulong uVar4;
  undefined4 uVar5;
  CCharString *pCVar6;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar7;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar8;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_08;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_09;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_10;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_11;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_12;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_15;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_16;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_17;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_18;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_19;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_20;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_21;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar9;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_22;
  CCharString *unaff_EBX;
  uint uVar10;
  CCharString *unaff_ESI;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  uint uVar11;
  char cVar12;
  undefined8 uVar13;
  CCharString *pCVar14;
  char *pcVar15;
  wchar_t *pwVar16;
  undefined *puVar17;
  CCharString *in_stack_ffffff1c;
  undefined1 in_stack_ffffff20;
  undefined1 in_stack_ffffff24;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_d4 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_d0 [4];
  CCharString local_cc [8];
  ulong local_c4;
  CBankFile *local_c0;
  CCharString local_bc [4];
  CCharString aCStack_b8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_b4 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_b0 [4];
  CCharString local_ac [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_a8 [4];
  CCharString local_a4 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_a0 [4];
  undefined4 local_9c;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_98 [4];
  CCharString local_94 [4];
  CCharString aCStack_90 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_8c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_88 [4];
  uint uStack_84;
  ulong local_80;
  ulong uStack_7c;
  CCharString local_78 [4];
  CChecksumCacheFile *local_74;
  CCharString local_70 [4];
  uint local_6c;
  CCharString local_68 [4];
  CCharString local_64 [4];
  int local_60;
  CCharString local_5c [4];
  int local_58;
  CCharString aCStack_54 [4];
  CCharString aCStack_50 [7];
  undefined1 local_49;
  CCharString aCStack_48 [7];
  undefined1 uStack_41;
  CCharString aCStack_40 [4];
  int local_3c;
  CCharString local_38 [4];
  CCharString local_34 [4];
  CCharString aCStack_30 [4];
  pair<EHeroMorphType,CParticleMorphs::CEntry> *local_2c;
  CCharString local_28 [4];
  int local_24;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *local_20;
  int local_1c;
  undefined1 uStack_16;
  undefined1 local_15;
  undefined1 *local_14;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 uStack_8;
  
  uStack_8 = 0xffffffff;
  puStack_c = &LAB_012109d0;
  pvStack_10 = ExceptionList;
  local_14 = &stack0xffffff10;
  ExceptionList = &pvStack_10;
  local_c0 = this;
  CCharString::CCharString(local_34);
  CCharString::CCharString(local_28);
  CCharString::CCharString(local_38);
  CCharString::CCharString(local_68);
  CAFile::SplitPath((CCharString *)(this + 0x84),local_34,local_28,local_38,local_68);
  pwVar16 = L".cks";
  pCVar6 = local_38;
  local_74 = (CChecksumCacheFile *)(this + 0xf8);
  FUN_0099be70(local_28);
  FUN_0099be70(pCVar6);
  pCVar2 = (CWideString *)FUN_0099bf30(pwVar16);
  CChecksumCacheFile::Open((CChecksumCacheFile *)(this + 0xf8),pCVar2);
  CCharString::~CCharString(local_ac);
  CCharString::~CCharString(local_cc);
  CCharString::~CCharString(local_94);
  CCharString::CCharString(local_70,"Scanning files...",-1);
  ppVar9 = extraout_EDX;
  if (*(int **)(this + 0xec) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0xec) + 4))(local_70);
    ppVar9 = extraout_EDX_00;
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_70,ppVar9,
             unaff_EDI);
  local_c4 = CountEntries(this);
  if (*(undefined4 **)(this + 0xec) != (undefined4 *)0x0) {
    (**(code **)**(undefined4 **)(this + 0xec))(0,local_c4);
  }
  local_24 = 0;
  local_3c = 0;
  local_20 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
  if (*(int *)(this + 4) != 0) {
    local_2c = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
    do {
      ppVar9 = local_20;
      if (local_2c[*(int *)(this + 0x20) + 9] != (pair<EHeroMorphType,CParticleMorphs::CEntry>)0x0)
      {
        if (*(int *)(this + 8) == *(int *)(this + 0xc)) {
          CCharString::CCharString(local_5c,"(Unknown)",-1);
        }
        else {
          CCharString::CCharString(local_5c,(CCharString *)(*(int *)(this + 8) + (int)local_20 * 4))
          ;
        }
        iVar1 = *(int *)(*(int *)(this + 0x2c) + (int)ppVar9 * 4);
        local_1c = iVar1;
        if (*(char *)(iVar1 + 0x15) == '\0') {
          DeleteEntry(this,(ulong)ppVar9);
          ppVar8 = extraout_EDX_01;
        }
        else {
          if (*(int *)(this + 0x5c) == 0) {
            local_80 = (ulong)(byte)local_2c[*(int *)(this + 0x20) + 8];
            ppVar8 = local_2c;
          }
          else {
            local_80 = CPackedUIntArray::operator[]
                                 ((CPackedUIntArray *)(this + 0x58),(ulong)(ppVar9 + -1));
            ppVar8 = extraout_EDX_02;
          }
          if (*(char *)(iVar1 + 0x14) == '\0') {
            uVar11 = 0;
            local_6c = 0;
            if (*(char *)(local_1c + 0x10) != '\0') {
              do {
                uVar10 = local_6c;
                cVar12 = (**(code **)(*(int *)this + 0x44))(local_80,local_6c);
                if (cVar12 != '\0') {
                  CCharString::CCharString
                            (local_78,(CCharString *)(*(int *)(local_1c + 0xc) + uVar10 * 4));
                  pCVar2 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
                  uVar4 = CChecksumCacheFile::GetChecksum(local_74,pCVar2);
                  local_9c = uVar4;
                  CCharString::~CCharString(local_bc);
                  uVar11 = uVar11 >> 8 ^
                           *(uint *)(&DAT_0129a168 + (uVar11 & 0xff ^ uVar4 & 0xff) * 4);
                  uVar11 = uVar11 >> 8 ^
                           *(uint *)(&DAT_0129a168 + (uVar11 & 0xff ^ uVar4 >> 8 & 0xff) * 4);
                  uVar11 = uVar11 >> 8 ^
                           *(uint *)(&DAT_0129a168 + (uVar11 & 0xff ^ local_9c._2_2_ & 0xff) * 4);
                  uVar11 = uVar11 >> 8 ^
                           *(uint *)(&DAT_0129a168 +
                                    (uVar11 & 0xff ^ (uint)(local_9c._2_2_ >> 8)) * 4);
                  std::
                  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                             local_78,(pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                                      (uint)(local_9c._2_2_ >> 8),unaff_EDI);
                  uVar10 = local_6c;
                }
                local_6c = uVar10 + 1;
              } while (local_6c < (uint)(int)*(char *)(local_1c + 0x10));
            }
            ppVar8 = local_20;
            if (uVar11 != *(uint *)(*(int *)(this + 0x14) + (int)local_20 * 4)) {
              if (*(int *)(this + 8) == *(int *)(this + 0xc)) {
                CCharString::CCharString(local_64,"(Unknown)",-1);
              }
              else {
                CCharString::CCharString
                          (local_64,(CCharString *)(*(int *)(this + 8) + (int)local_20 * 4));
              }
              pCVar6 = local_64;
              CCharString::CCharString(local_a4,"Source file has been modified for ",-1);
              uVar13 = FUN_0099f570(pCVar6);
              ppVar9 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar13 >> 0x20);
              if (*(int **)(this + 0xec) != (int *)0x0) {
                (**(code **)(**(int **)(this + 0xec) + 8))((int)uVar13);
                ppVar9 = extraout_EDX_03;
              }
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (local_d4,ppVar9,unaff_EDI);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_a4,
                         extraout_EDX_04,unaff_EDI);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_64,
                         extraout_EDX_05,unaff_EDI);
              *(undefined1 *)(local_1c + 0x14) = 1;
              ppVar8 = extraout_EDX_06;
            }
            ppVar9 = local_20;
            if (*(char *)(local_1c + 0x14) != '\0') goto LAB_009cefa5;
          }
          else {
LAB_009cefa5:
            local_24 = local_24 + 1;
          }
          local_3c = local_3c + 1;
          if (*(undefined4 **)(this + 0xec) != (undefined4 *)0x0) {
            (**(code **)**(undefined4 **)(this + 0xec))(local_3c,local_c4);
            ppVar8 = extraout_EDX_07;
          }
          if (DAT_013ca821 != '\0') {
            local_15 = local_49;
                    /* WARNING: Subroutine does not return */
            _CxxThrowException(&local_15,(ThrowInfo *)&DAT_013693e8);
          }
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_5c,ppVar8,
                   unaff_EDI);
      }
      local_20 = ppVar9 + 1;
      local_2c = local_2c + 0xc;
    } while (local_20 < *(pair<EHeroMorphType,CParticleMorphs::CEntry> **)(this + 4));
  }
  iVar1 = local_24;
  if ((((this[0x94] == (CBankFile)0x0) || (*(int *)(this + 0x90) != 0)) || (local_24 != 0)) ||
     (*(int *)(this + 4) < 1)) {
    iVar3 = (**(code **)(*(int *)this + 0x24))();
    local_24 = iVar1 + iVar3;
    BeginUpdateEntries(this);
    uVar11 = 0;
    if (*(undefined4 **)(this + 0xec) != (undefined4 *)0x0) {
      (**(code **)**(undefined4 **)(this + 0xec))(0,iVar1 + iVar3);
    }
    local_60 = 0;
    local_58 = 0;
    for (; uStack_84 = uVar11, uVar11 < *(uint *)(this + 4); uVar11 = uVar11 + 1) {
      if ((uVar11 != 0) && (uVar11 < *(uint *)(this + 4))) {
        if (*(int *)(this + 0x6c) == 0) {
          cVar12 = *(char *)(*(int *)(this + 0x20) + 9 + uVar11 * 0xc);
        }
        else {
          uVar4 = CPackedUIntArray::operator[]((CPackedUIntArray *)(this + 0x68),uVar11 - 1);
          cVar12 = uVar4 != 0;
        }
        if (cVar12 != '\0') {
          iVar1 = *(int *)(*(int *)(this + 0x2c) + uVar11 * 4);
          if (*(char *)(iVar1 + 0x14) != '\0') {
            if (*(int *)(this + 0x5c) == 0) {
              uStack_7c = (ulong)*(byte *)(*(int *)(this + 0x20) + 8 + uVar11 * 0xc);
            }
            else {
              uStack_7c = CPackedUIntArray::operator[]((CPackedUIntArray *)(this + 0x58),uVar11 - 1)
              ;
            }
            uStack_8 = 0;
            CCharString::CCharString(aCStack_30);
            cVar12 = (**(code **)(*(int *)this + 0x44))(uStack_7c,0);
            if (cVar12 == '\0') {
              puVar17 = &DAT_0129b5cc;
              uVar5 = *(undefined4 *)(iVar1 + 0xc);
              CCharString::CCharString(aCStack_b8,"Processing ",-1);
              FUN_0099f570(uVar5);
              pCVar6 = (CCharString *)FUN_0099f600(puVar17);
              CCharString::operator=(aCStack_30,pCVar6);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (aaStack_a8,extraout_EDX_19,unaff_EDI);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (aaStack_b0,extraout_EDX_20,unaff_EDI);
              paVar7 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_b8;
              ppVar9 = extraout_EDX_21;
            }
            else {
              CCharString::CCharString(aCStack_50);
              CCharString::CCharString(aCStack_48);
              CCharString::CCharString(aCStack_40);
              CCharString::CCharString(aCStack_54);
              CAFile::SplitPath(*(CCharString **)(iVar1 + 0xc),aCStack_50,aCStack_48,aCStack_40,
                                aCStack_54);
              paVar7 = aaStack_98;
              FUN_0099f570(aCStack_48);
              uVar5 = FUN_0099ed10(paVar7);
              pcVar15 = "\n in ";
              pCVar6 = aCStack_54;
              pCVar14 = aCStack_40;
              CCharString::CCharString(aCStack_90,"Reading ",-1);
              FUN_0099f570(pCVar14);
              FUN_0099f570(pCVar6);
              FUN_0099f600(pcVar15);
              pCVar6 = (CCharString *)FUN_0099f570(uVar5);
              CCharString::operator=(aCStack_30,pCVar6);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (aaStack_88,extraout_EDX_08,unaff_EDI);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (aaStack_d0,extraout_EDX_09,unaff_EDI);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (aaStack_b4,extraout_EDX_10,unaff_EDI);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (aaStack_8c,extraout_EDX_11,unaff_EDI);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_90
                         ,extraout_EDX_12,unaff_EDI);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (aaStack_98,extraout_EDX_13,unaff_EDI);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (aaStack_a0,extraout_EDX_14,unaff_EDI);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_54
                         ,extraout_EDX_15,unaff_EDI);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_40
                         ,extraout_EDX_16,unaff_EDI);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_48
                         ,extraout_EDX_17,unaff_EDI);
              paVar7 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_50;
              ppVar9 = extraout_EDX_18;
            }
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      (paVar7,ppVar9,unaff_EDI);
            CScriptGameResourceObjectScriptedThingBase::AskHeroQuestion
                      ((CScriptGameResourceObjectScriptedThingBase *)this,(CScriptThing *)aCStack_30
                       ,(char *)unaff_EDI,unaff_ESI,unaff_EBX,in_stack_ffffff1c,
                       (bool)in_stack_ffffff20,(bool)in_stack_ffffff24);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_30,
                       extraout_EDX_22,unaff_EDI);
            uStack_8 = 0xffffffff;
            local_60 = local_60 + 1;
            local_58 = local_58 + 1;
            if ((local_58 == 10) && (this[0x8c] == (CBankFile)0x0)) {
              WriteFileInformation(this,false);
              local_58 = 0;
            }
            if (*(undefined4 **)(this + 0xec) != (undefined4 *)0x0) {
              (**(code **)**(undefined4 **)(this + 0xec))(local_60,local_24);
            }
          }
          if (DAT_013ca821 != '\0') {
            EndUpdateEntries(this);
            uStack_16 = uStack_41;
                    /* WARNING: Subroutine does not return */
            _CxxThrowException(&uStack_16,(ThrowInfo *)&DAT_013693e8);
          }
        }
      }
    }
    EndUpdateEntries(this);
  }
  FUN_00a60c30();
  CCharString::~CCharString(local_68);
  CCharString::~CCharString(local_38);
  CCharString::~CCharString(local_28);
  CCharString::~CCharString(local_34);
  ExceptionList = pvStack_10;
  return;
}



//=== ?OpenReadOnly@CBankFile@@UAE_NABVCCharString@@K@Z @ 009d06f0 ===

/* [bsim sim=0.9387805522643481 <- ego_r]
   public: virtual bool __thiscall CBankFile::OpenReadOnly(class CCharString const &,unsigned long)
    */

bool __thiscall
CBankFile::_OpenReadOnly_CBankFile__UAE_NABVCCharString__K_Z
          (CBankFile *this,CCharString *param_1,ulong param_2)

{
  CCharString *pCVar1;
  bool bVar2;
  undefined1 uVar3;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *pCVar4;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_00;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_01;
  undefined8 uVar5;
  int local_48 [2];
  undefined4 uStack_40;
  int *piStack_3c;
  int local_38;
  ulong local_34;
  ulong local_30;
  undefined4 local_2c;
  undefined4 local_28;
  CFileDataInputStream aCStack_24 [32];
  CCharString aCStack_4 [4];
  
  pCVar1 = param_1;
  this[0x8d] = (CBankFile)0x1;
  *(ulong *)(this + 0x78) = param_2;
  CCharString::operator=((CCharString *)(this + 0x88),param_1);
  *(undefined4 *)(this + 0x90) = 0;
  this[0x94] = (CBankFile)0x0;
  this[0x8e] = (CBankFile)0x0;
  this[0x8f] = (CBankFile)0x0;
  this[0x95] = (CBankFile)0x0;
  if ((DAT_013ca7b0 != '\0') && ((param_2 & 4) == 0)) {
    this[0x8c] = (CBankFile)0x1;
    local_38 = 0;
    local_34 = 0;
    local_30 = 0;
    local_2c = 0;
    local_28 = 0;
    local_48[0] = 0;
    local_48[1] = 0;
    bVar2 = CBankFileManager::FindContainedBank
                      ((CBankFileManager *)&DAT_013ca79c,pCVar1,(CContainedBank *)&local_38,
                       (CCountedPointer<NBankFileManager::CRetailBank> *)local_48);
    pCVar4 = extraout_EDX;
    if (bVar2) {
      uVar5 = (**(code **)(*(int *)this + 0x20))();
      pCVar4 = (CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *)
               ((ulonglong)uVar5 >> 0x20);
      if (local_38 == (int)uVar5) {
        *(undefined4 *)(this + 0x98) = local_28;
        uStack_40 = *(undefined4 *)(local_48[0] + 4);
        piStack_3c = *(int **)(local_48[0] + 8);
        if (piStack_3c != (int *)0x0) {
          *piStack_3c = *piStack_3c + 1;
        }
        CCountedPointer<CDiskFileWin32>::operator=
                  ((CCountedPointer<CDiskFileWin32> *)(this + 0x7c),
                   (CCountedPointer<class_CDiskFileWin32> *)&uStack_40);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   &uStack_40,extraout_EDX_00);
        CFileDataInputStream::CFileDataInputStream(aCStack_24,*(CAFile **)(this + 0x7c),0x4000);
        CDataInputStream::_SetPosition_CDataInputStream__UAEXK_Z
                  ((CDataInputStream *)aCStack_24,local_30);
        ReadEntryList(this,aCStack_24,local_34 + 1,local_34);
        (**(code **)(*(int *)this + 0x2c))();
        CFileDataInputStream::~CFileDataInputStream(aCStack_24);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   local_48,extraout_EDX_01);
        return true;
      }
    }
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               local_48,pCVar4);
    return false;
  }
  FUN_009a7ca0(&param_1,pCVar1);
  uVar3 = (**(code **)(*(int *)this + 0xc))(&param_1,param_2);
  CCharString::~CCharString(aCStack_4);
  return (bool)uVar3;
}



//=== ?OpenFileReadOnly@CBankFile@@UAE_NABVCWideString@@K@Z @ 009d1090 ===

/* [bsim sim=0.7481268866523866 <- ego_r]
   public: virtual bool __thiscall CBankFile::OpenFileReadOnly(class CWideString const &,unsigned
   long) */

bool __thiscall
CBankFile::_OpenFileReadOnly_CBankFile__UAE_NABVCWideString__K_Z
          (CBankFile *this,CWideString *param_1,ulong param_2)

{
  bool bVar1;
  CBase *this_00;
  uint unaff_EBX;
  CIPhysicsFacesSegmentBase *pCVar2;
  
  this[0x8d] = (CBankFile)0x1;
  *(ulong *)(this + 0x78) = param_2;
  *(undefined4 *)(this + 0x90) = 0;
  this[0x94] = (CBankFile)0x0;
  this[0x8e] = (CBankFile)0x0;
  this[0x8f] = (CBankFile)0x0;
  this[0x95] = (CBankFile)0x0;
  this[0x8c] = (CBankFile)0x0;
  bVar1 = CAFile::PathExists(param_1);
  if (bVar1) {
    this_00 = ::operator_new(0x1c);
    pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    if (this_00 != (CBase *)0x0) {
      NHeroInformationScreens::CBase::CBase(this_00);
      *(undefined ***)this_00 = &PTR__vector_deleting_destructor__0122d06c;
      CCharString::CCharString((CCharString *)(this_00 + 0x18));
      this_00[0x14] = (CBase)0x0;
      pCVar2 = (CIPhysicsFacesSegmentBase *)this_00;
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)(this + 0x7c),pCVar2);
    (**(code **)(**(int **)(this + 0x7c) + 4))(param_1,1,4);
    CWideString::operator=((CWideString *)(this + 0x84),param_1);
    bVar1 = ReadFileInformation(this,(bool)(~(byte)(unaff_EBX >> 1) & 1));
    if (bVar1) {
      (**(code **)(*(int *)this + 0x2c))();
      return true;
    }
    FUN_009cd850();
  }
  return false;
}



//=== ?OpenFileReadWrite@CBankFile@@UAEXABVCWideString@@KK@Z @ 009d1160 ===

/* [bsim sim=0.7319240977538727 <- ego_r]
   public: virtual void __thiscall CBankFile::OpenFileReadWrite(class CWideString const &,unsigned
   long,unsigned long) */

void __thiscall
CBankFile::_OpenFileReadWrite_CBankFile__UAEXABVCWideString__KK_Z
          (CBankFile *this,CWideString *param_1,ulong param_2,ulong param_3)

{
  CCountedPointer<CIPhysicsFacesSegmentBase_const_> *this_00;
  CWideString *this_01;
  bool bVar1;
  CBase *this_02;
  int iVar2;
  
  *(ulong *)(this + 0x98) = param_3;
  this_01 = (CWideString *)(this + 0x84);
  this[0x8d] = (CBankFile)0x0;
  *(ulong *)(this + 0x78) = param_2 | 0x1f8;
  *(undefined4 *)(this + 0x90) = 0;
  this[0x94] = (CBankFile)0x0;
  this[0x8e] = (CBankFile)0x0;
  this[0x95] = (CBankFile)0x0;
  this[0x8c] = (CBankFile)0x0;
  CWideString::operator=(this_01,param_1);
  this_02 = ::operator_new(0x1c);
  if (this_02 == (CBase *)0x0) {
    this_02 = (CBase *)0x0;
  }
  else {
    NHeroInformationScreens::CBase::CBase(this_02);
    *(undefined ***)this_02 = &PTR__vector_deleting_destructor__0122d06c;
    CCharString::CCharString((CCharString *)(this_02 + 0x18));
    this_02[0x14] = (CBase)0x0;
  }
  this_00 = (CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)(this + 0x7c);
  CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
            (this_00,(CIPhysicsFacesSegmentBase *)this_02);
  if ((param_2 & 1) == 0) {
    bVar1 = CAFile::PathExists(this_01);
    if (bVar1) {
      (**(code **)(**(int **)this_00 + 4))(this_01,1,4);
      this[0x8f] = (CBankFile)0x0;
      goto LAB_009d1236;
    }
  }
  (**(code **)(**(int **)this_00 + 4))(this_01,4,4);
  this[0x8e] = (CBankFile)0x1;
  this[0x8f] = (CBankFile)0x1;
LAB_009d1236:
  iVar2 = (**(code **)(**(int **)this_00 + 0x24))();
  if (0 < iVar2) {
    ReadFileInformation(this,true);
  }
  (**(code **)(*(int *)this + 0x2c))();
  return;
}



//=== _Push_heap<unsigned_long*,int,unsigned_long,CBankFile::CompareDataOffset> @ 009d3040 ===

/* [bsim sim=0.7715639443048655 <- ego_r]
   void __fastcall std::_Push_heap<unsigned long *,int,unsigned long,class
   CBankFile::CompareDataOffset>(unsigned long *,int,int,unsigned long &&,class
   CBankFile::CompareDataOffset) */

void __fastcall
std::_Push_heap<unsigned_long*,int,unsigned_long,CBankFile::CompareDataOffset>
          (int param_1,int param_2,int param_3,int param_4,uint param_5)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  
  iVar2 = param_5;
  if (param_2 <= param_3) {
    *(int *)(param_1 + param_2 * 4) = param_4;
    return;
  }
  while( true ) {
    iVar4 = (param_2 + -1) / 2;
    iVar1 = *(int *)(param_1 + iVar4 * 4);
    if (*(int *)(iVar2 + 0x3c) == 0) {
      param_5 = *(uint *)(*(int *)(iVar2 + 0x20) + iVar1 * 0xc);
    }
    else {
      param_5 = CPackedUIntArray::operator[]((CPackedUIntArray *)(iVar2 + 0x38),iVar1 - 1);
    }
    if (*(int *)(iVar2 + 0x3c) == 0) {
      uVar3 = *(uint *)(*(int *)(iVar2 + 0x20) + param_4 * 0xc);
    }
    else {
      uVar3 = CPackedUIntArray::operator[]((CPackedUIntArray *)(iVar2 + 0x38),param_4 - 1);
    }
    if (uVar3 <= param_5) break;
    *(undefined4 *)(param_1 + param_2 * 4) = *(undefined4 *)(param_1 + iVar4 * 4);
    param_2 = iVar4;
    if (iVar4 <= param_3) {
      *(int *)(param_1 + iVar4 * 4) = param_4;
      return;
    }
  }
  *(int *)(param_1 + param_2 * 4) = param_4;
  return;
}



//=== _Adjust_heap<unsigned_long*,int,unsigned_long,CBankFile::CompareDataOffset> @ 009d3750 ===

/* [bsim sim=0.9595331903831648 <- ego_r]
   void __fastcall std::_Adjust_heap<unsigned long *,int,unsigned long,class
   CBankFile::CompareDataOffset>(unsigned long *,int,int,unsigned long &&,class
   CBankFile::CompareDataOffset) */

void __fastcall
std::_Adjust_heap<unsigned_long*,int,unsigned_long,CBankFile::CompareDataOffset>
          (int param_1,int param_2,int param_3,undefined4 param_4,uint param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  
  iVar4 = param_5;
  iVar2 = param_2;
  while( true ) {
    iVar3 = iVar2 * 2;
    iVar6 = iVar3 + 2;
    if (param_3 <= iVar6) break;
    iVar7 = *(int *)(param_1 + -4 + iVar6 * 4);
    iVar1 = *(int *)(param_1 + iVar6 * 4);
    if (*(int *)(iVar4 + 0x3c) == 0) {
      param_5 = *(uint *)(*(int *)(iVar4 + 0x20) + iVar1 * 0xc);
    }
    else {
      param_5 = CPackedUIntArray::operator[]((CPackedUIntArray *)(iVar4 + 0x38),iVar1 - 1);
    }
    if (*(int *)(iVar4 + 0x3c) == 0) {
      uVar5 = *(uint *)(*(int *)(iVar4 + 0x20) + iVar7 * 0xc);
    }
    else {
      uVar5 = CPackedUIntArray::operator[]((CPackedUIntArray *)(iVar4 + 0x38),iVar7 - 1);
    }
    if (param_5 < uVar5) {
      iVar6 = iVar3 + 1;
    }
    *(undefined4 *)(param_1 + iVar2 * 4) = *(undefined4 *)(param_1 + iVar6 * 4);
    iVar2 = iVar6;
  }
  iVar7 = iVar2;
  if (iVar6 == param_3) {
    iVar7 = iVar3 + 1;
    *(undefined4 *)(param_1 + iVar2 * 4) = *(undefined4 *)(param_1 + -4 + iVar6 * 4);
  }
  _Push_heap<unsigned_long*,int,unsigned_long,CBankFile::CompareDataOffset>
            (param_1,iVar7,param_2,param_4,iVar4);
  return;
}



//=== _Insertion_sort1<unsigned_long*,CBankFile::CompareDataOffset,unsigned_long> @ 009d3b90 ===

/* [bsim sim=0.6085407948728898 <- ego_r]
   void __fastcall std::_Insertion_sort1<unsigned long *,class CBankFile::CompareDataOffset,unsigned
   long>(unsigned long *,unsigned long *,class CBankFile::CompareDataOffset,unsigned long *) */

void __fastcall
std::_Insertion_sort1<unsigned_long*,CBankFile::CompareDataOffset,unsigned_long>
          (int *param_1,int *param_2,size_t param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int *piVar5;
  uint local_8;
  
  iVar3 = param_3;
  if ((param_1 != param_2) && (piVar5 = param_1 + 1, piVar5 != param_2)) {
    param_3 = (int)piVar5 - (int)param_1;
    do {
      iVar1 = *piVar5;
      iVar2 = *param_1;
      if (*(int *)(iVar3 + 0x3c) == 0) {
        local_8 = *(uint *)(*(int *)(iVar3 + 0x20) + iVar1 * 0xc);
      }
      else {
        local_8 = CPackedUIntArray::operator[]((CPackedUIntArray *)(iVar3 + 0x38),iVar1 - 1);
      }
      if (*(int *)(iVar3 + 0x3c) == 0) {
        uVar4 = *(uint *)(*(int *)(iVar3 + 0x20) + iVar2 * 0xc);
      }
      else {
        uVar4 = CPackedUIntArray::operator[]((CPackedUIntArray *)(iVar3 + 0x38),iVar2 - 1);
      }
      if (local_8 < uVar4) {
        if (0 < (int)param_3) {
          memmove((void *)((int)piVar5 + (4 - param_3)),param_1,param_3);
        }
        *param_1 = iVar1;
      }
      else {
        FUN_009d2fc0(iVar3);
      }
      piVar5 = piVar5 + 1;
      param_3 = param_3 + 4;
    } while (piVar5 != param_2);
  }
  return;
}



//=== ?OpenReadWrite@CBankFileAsync@@UAEXABVCCharString@@KK@Z @ 009d51d0 ===

void _OpenReadWrite_CBankFileAsync__UAEXABVCCharString__KK_Z
               (CCharString *param_1,ulong param_2,ulong param_3)

{
  CBankFile *in_ECX;
  
  CBankFile::_OpenReadWrite_CBankFile__UAEXABVCCharString__KK_Z(in_ECX,param_1,param_2,param_3);
  in_ECX[0x149] = (CBankFile)0x1;
  return;
}



//=== ?OpenFileReadWrite@CBankFileAsync@@UAEXABVCWideString@@KK@Z @ 009d5200 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: virtual void __thiscall CBankFileAsync::OpenFileReadWrite(class CWideString const
   &,unsigned long,unsigned long) */

void __thiscall
CBankFileAsync::_OpenFileReadWrite_CBankFileAsync__UAEXABVCWideString__KK_Z
          (CBankFileAsync *this,CWideString *param_1,ulong param_2,ulong param_3)

{
  CBankFile::_OpenFileReadWrite_CBankFile__UAEXABVCWideString__KK_Z
            ((CBankFile *)this,param_1,param_2,param_3);
  this[0x149] = (CBankFileAsync)0x1;
  return;
}



//=== CBankFileAsyncEntry @ 009d5260 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   private: __thiscall CBankFileAsyncEntry::CBankFileAsyncEntry(class CBankFileAsync &,unsigned
   long) */

CBankFileAsyncEntry * __thiscall
CBankFileAsyncEntry::CBankFileAsyncEntry
          (CBankFileAsyncEntry *this,CBankFileAsync *param_1,ulong param_2)

{
  ulong uVar1;
  
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129b880;
  *(CBankFileAsync **)(this + 4) = param_1;
  *(ulong *)(this + 8) = param_2;
  uVar1 = CBankFile::GetEntryDataSize((CBankFile *)param_1,param_2);
  uVar1 = CBankFile::Align(*(CBankFile **)(this + 4),uVar1);
  *(ulong *)(this + 0xc) = uVar1;
  return this;
}



//=== ?OnFileReadBegin@CBankFileAsyncData@@UAE_NPAPAXW4EThreadedFilePriority@@@Z @ 009d5440 ===

undefined4
_OnFileReadBegin_CBankFileAsyncData__UAE_NPAPAXW4EThreadedFilePriority___Z(undefined4 *param_1)

{
  bool bVar1;
  CBankFileAsyncData *in_ECX;
  CCriticalSectionLock aCStack_8 [4];
  char cStack_4;
  
  CCriticalSectionLock::CCriticalSectionLock
            (aCStack_8,(_RTL_CRITICAL_SECTION *)(*(int *)(in_ECX + 0x28) + 0x14c),true);
  if (cStack_4 != '\0') {
    bVar1 = CBankFileAsyncData::SetupBuffer(in_ECX,false);
    if (bVar1) {
      *param_1 = *(undefined4 *)(in_ECX + 0xc);
      CCriticalSectionLock::~CCriticalSectionLock(aCStack_8);
      return 1;
    }
  }
  CCriticalSectionLock::~CCriticalSectionLock(aCStack_8);
  return 0;
}



//=== ?OnFileReadCompletion@CBankFileAsyncData@@UAEX_N@Z @ 009d54a0 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: virtual void __thiscall CBankFileAsyncData::OnFileReadCompletion(bool) */

void __thiscall
CBankFileAsyncData::_OnFileReadCompletion_CBankFileAsyncData__UAEX_N_Z
          (CBankFileAsyncData *this,bool param_1)

{
  CCriticalSectionLock local_8 [8];
  
  CCriticalSectionLock::CCriticalSectionLock
            (local_8,(_RTL_CRITICAL_SECTION *)(*(int *)(this + 0x28) + 0x14c),false);
  this[0x44] = (CBankFileAsyncData)0x1;
  *(undefined4 *)(this + 0x18) = 0;
  if (*(int *)(this + 0x20) != -1) {
    this[0x46] = (CBankFileAsyncData)0x1;
  }
  CCriticalSectionLock::~CCriticalSectionLock(local_8);
  return;
}



//=== ?CanRelocateData@CBankFileAsyncData@@UBE_NJ@Z @ 009d55d0 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CBankFileAsyncData::CanRelocateData(long)const  */

bool __thiscall
CBankFileAsyncData::_CanRelocateData_CBankFileAsyncData__UBE_NJ_Z
          (CBankFileAsyncData *this,long param_1)

{
  CCriticalSectionLock local_8 [8];
  
  CCriticalSectionLock::CCriticalSectionLock
            (local_8,(_RTL_CRITICAL_SECTION *)(*(int *)(this + 0x24) + 0x14c),false);
  if ((this[0x40] != (CBankFileAsyncData)0x0) && (this[0x41] == (CBankFileAsyncData)0x0)) {
    CCriticalSectionLock::~CCriticalSectionLock(local_8);
    return true;
  }
  CCriticalSectionLock::~CCriticalSectionLock(local_8);
  return false;
}



//=== ?RelocateData@CBankFileAsyncData@@UAEXJPAX0J@Z @ 009d5670 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: virtual void __thiscall CBankFileAsyncData::RelocateData(long,void *,void *,long) */

void __thiscall
CBankFileAsyncData::_RelocateData_CBankFileAsyncData__UAEXJPAX0J_Z
          (CBankFileAsyncData *this,long param_1,void *param_2,void *param_3,long param_4)

{
  CCriticalSectionLock local_8 [8];
  
  CCriticalSectionLock::CCriticalSectionLock
            (local_8,(_RTL_CRITICAL_SECTION *)(*(int *)(this + 0x24) + 0x14c),false);
  if (*(int *)(this + 8) != 0) {
    *(int *)(this + 8) = (*(int *)(this + 8) - (int)param_2) + (int)param_3;
  }
  if (*(int *)(this + 4) != 0) {
    *(int *)(this + 4) = (*(int *)(this + 4) - (int)param_2) + (int)param_3;
  }
  CCriticalSectionLock::~CCriticalSectionLock(local_8);
  return;
}



//=== ?OpenReadOnly@CBankFileAsync@@UAE_NABVCCharString@@K@Z @ 009d56c0 ===

bool _OpenReadOnly_CBankFileAsync__UAE_NABVCCharString__K_Z(CCharString *param_1,ulong param_2)

{
  CThreadedFile *this;
  bool bVar1;
  CMultiStatBar *this_00;
  CIPhysicsFacesSegmentBase *pCVar2;
  CWideString *pCVar3;
  CBankFile *in_ECX;
  uint extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_01;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_02;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_03;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EBX;
  CBankFile CVar4;
  int aiStack_24 [2];
  undefined4 uStack_1c;
  int *piStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  bVar1 = CBankFile::_OpenReadOnly_CBankFile__UAE_NABVCCharString__K_Z(in_ECX,param_1,param_2);
  if (!bVar1) {
    return bVar1;
  }
  CCharString::CCharString((CCharString *)&param_2,"Open Bank File Async",-1);
  NProgressDisplay::DisplayProgress
            ((CCharString *)&param_2,(float)(extraout_EDX & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_2,
             extraout_EDX_00,unaff_EBX);
  if (in_ECX[0x8c] != (CBankFile)0x0) {
    uStack_14 = 0;
    uStack_10 = 0;
    uStack_c = 0;
    uStack_8 = 0;
    uStack_4 = 0;
    aiStack_24[0] = 0;
    aiStack_24[1] = 0;
    bVar1 = CBankFileManager::FindContainedBank
                      ((CBankFileManager *)&DAT_013ca79c,param_1,(CContainedBank *)&uStack_14,
                       (CCountedPointer<NBankFileManager::CRetailBank> *)aiStack_24);
    if (!bVar1) {
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 aiStack_24,extraout_EDX_01);
      return false;
    }
    uStack_1c = *(undefined4 *)(aiStack_24[0] + 0xc);
    piStack_18 = *(int **)(aiStack_24[0] + 0x10);
    if (piStack_18 != (int *)0x0) {
      *piStack_18 = *piStack_18 + 1;
    }
    CCountedPointer<CDiskFileWin32>::operator=
              ((CCountedPointer<CDiskFileWin32> *)(in_ECX + 0x110),
               (CCountedPointer<class_CDiskFileWin32> *)&uStack_1c);
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               &uStack_1c,extraout_EDX_02);
    in_ECX[0x148] = *(CBankFile *)(aiStack_24[0] + 0x14);
    in_ECX[0x149] = (CBankFile)0x0;
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               aiStack_24,extraout_EDX_03);
    return true;
  }
  this_00 = operator_new(0x1c);
  if (this_00 == (CMultiStatBar *)0x0) {
    pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
  }
  else {
    pCVar2 = (CIPhysicsFacesSegmentBase *)
             NPlayerGui::CMultiStatBar::CMultiStatBar(this_00,(CPlayerGuiDef *)unaff_EBX);
  }
  CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
            ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)(in_ECX + 0x110),pCVar2);
  this = *(CThreadedFile **)(in_ECX + 0x110);
  CVar4 = in_ECX[0x148];
  pCVar3 = (CWideString *)CBankFile::GetBankPathname(in_ECX);
  CThreadedFile::Open(this,pCVar3,(bool)CVar4);
  CCharString::~CCharString((CCharString *)&param_2);
  in_ECX[0x149] = (CBankFile)0x0;
  return true;
}



//=== ?OpenFileReadOnly@CBankFileAsync@@UAE_NABVCWideString@@K@Z @ 009d5820 ===

bool _OpenFileReadOnly_CBankFileAsync__UAE_NABVCWideString__K_Z(CWideString *param_1,ulong param_2)

{
  CThreadedFile *this;
  bool bVar1;
  CMultiStatBar *this_00;
  CIPhysicsFacesSegmentBase *pCVar2;
  CWideString *pCVar3;
  CBankFile *in_ECX;
  CPlayerGuiDef *unaff_ESI;
  CBankFile CVar4;
  
  bVar1 = CBankFile::_OpenFileReadOnly_CBankFile__UAE_NABVCWideString__K_Z(in_ECX,param_1,param_2);
  if (!bVar1) {
    return bVar1;
  }
  this_00 = operator_new(0x1c);
  if (this_00 == (CMultiStatBar *)0x0) {
    pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
  }
  else {
    pCVar2 = (CIPhysicsFacesSegmentBase *)
             NPlayerGui::CMultiStatBar::CMultiStatBar(this_00,unaff_ESI);
  }
  CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
            ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)(in_ECX + 0x110),pCVar2);
  this = *(CThreadedFile **)(in_ECX + 0x110);
  CVar4 = in_ECX[0x148];
  pCVar3 = (CWideString *)CBankFile::GetBankPathname(in_ECX);
  CThreadedFile::Open(this,pCVar3,(bool)CVar4);
  CCharString::~CCharString((CCharString *)&param_2);
  in_ECX[0x149] = (CBankFile)0x0;
  return true;
}



//=== ?Close@CBankFileAsync@@UAEXXZ @ 009d58a0 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: virtual void __thiscall CBankFileAsync::Close(void) */

void __thiscall CBankFileAsync::_Close_CBankFileAsync__UAEXXZ(CBankFileAsync *this)

{
  if ((*(CThreadedFile **)(this + 0x110) != (CThreadedFile *)0x0) &&
     (this[0x8c] == (CBankFileAsync)0x0)) {
    CThreadedFile::Close(*(CThreadedFile **)(this + 0x110));
  }
  _Close_CBankFile__UAEXXZ();
  return;
}



//=== CBankFileAsyncData @ 009d5d30 ===

/* [bsim sim=0.8609110823805833 <- ego_r]
   private: __thiscall CBankFileAsyncData::CBankFileAsyncData(unsigned long,unsigned long,class
   CMovableResourceMemoryPool *,class CBankFileAsync &) */

CBankFileAsyncData * __thiscall
CBankFileAsyncData::CBankFileAsyncData
          (CBankFileAsyncData *this,ulong param_1,ulong param_2,CMovableResourceMemoryPool *param_3,
          CBankFileAsync *param_4)

{
  CCriticalSectionLock local_10 [8];
  undefined1 local_8 [8];
  
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  *(undefined ***)(this + 4) = &PTR_LAB_0129b860;
  *(ulong *)(this + 8) = param_1;
  *(ulong *)(this + 0xc) = param_1;
  *(CMovableResourceMemoryPool **)(this + 0x28) = param_3;
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129b8b4;
  *(undefined ***)(this + 4) = &PTR_LAB_0129b8a0;
  *(ulong *)(this + 0x10) = param_2;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0xffffffff;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  this[0x3c] = (CBankFileAsyncData)0x1;
  *(undefined4 *)(this + 0x40) = 3;
  this[0x44] = (CBankFileAsyncData)0x0;
  this[0x45] = (CBankFileAsyncData)0x0;
  this[0x46] = (CBankFileAsyncData)0x0;
  this[0x47] = (CBankFileAsyncData)0x0;
  CCriticalSectionLock::CCriticalSectionLock
            (local_10,(_RTL_CRITICAL_SECTION *)(*(int *)(this + 0x28) + 0x14c),false);
  param_1 = (ulong)this;
  FUN_009d7cf0(local_8,&param_1);
  *(undefined4 *)(this + 0x24) = 0;
  CCriticalSectionLock::~CCriticalSectionLock(local_10);
  return this;
}



//=== CBankFileAsyncData @ 009d5df0 ===

/* [bsim sim=0.8609110823805833 <- ego_r]
   private: __thiscall CBankFileAsyncData::CBankFileAsyncData(unsigned long,unsigned long,class
   CMovableResourceMemoryPool *,class CBankFileAsync &) */

CBankFileAsyncData * __thiscall
CBankFileAsyncData::CBankFileAsyncData
          (CBankFileAsyncData *this,ulong param_1,ulong param_2,CMovableResourceMemoryPool *param_3,
          CBankFileAsync *param_4)

{
  CCriticalSectionLock local_10 [8];
  undefined1 local_8 [8];
  
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  *(undefined ***)(this + 4) = &PTR_LAB_0129b860;
  *(ulong *)(this + 0x10) = param_1;
  *(ulong *)(this + 0x14) = param_2;
  *(CBankFileAsync **)(this + 0x28) = param_4;
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129b8b4;
  *(undefined ***)(this + 4) = &PTR_LAB_0129b8a0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(CMovableResourceMemoryPool **)(this + 0x1c) = param_3;
  *(undefined4 *)(this + 0x20) = 0xffffffff;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  this[0x3c] = (CBankFileAsyncData)0x1;
  *(undefined4 *)(this + 0x40) = 3;
  this[0x44] = (CBankFileAsyncData)0x0;
  this[0x45] = (CBankFileAsyncData)0x0;
  this[0x46] = (CBankFileAsyncData)0x0;
  this[0x47] = (CBankFileAsyncData)0x0;
  CCriticalSectionLock::CCriticalSectionLock
            (local_10,(_RTL_CRITICAL_SECTION *)(*(int *)(this + 0x28) + 0x14c),false);
  param_1 = (ulong)this;
  FUN_009d7cf0(local_8,&param_1);
  *(undefined4 *)(this + 0x24) = 0;
  CCriticalSectionLock::~CCriticalSectionLock(local_10);
  return this;
}



//=== ~CBankFileAsyncData @ 009d61f0 ===

/* [bsim sim=0.9257051038703629 <- ego_r]
   public: virtual __thiscall CBankFileAsyncData::~CBankFileAsyncData(void) */

void __thiscall CBankFileAsyncData::~CBankFileAsyncData(CBankFileAsyncData *this)

{
  void *pvVar1;
  CMemoryAllocator *pCVar2;
  char extraout_AL;
  char cVar3;
  char extraout_AL_00;
  CIDrawEnvironment *pCVar4;
  allocator<NCreatureHitNotification::CFlash> *extraout_ECX;
  allocator<NCreatureHitNotification::CFlash> *extraout_ECX_00;
  allocator<NCreatureHitNotification::CFlash> *extraout_ECX_01;
  allocator<NCreatureHitNotification::CFlash> *extraout_ECX_02;
  allocator<NCreatureHitNotification::CFlash> *extraout_ECX_03;
  allocator<NCreatureHitNotification::CFlash> *paVar5;
  CFlash *in_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  CFlash *extraout_EDX_00;
  CFlash *extraout_EDX_01;
  CFlash *extraout_EDX_02;
  CFlash *extraout_EDX_03;
  CFlash *extraout_EDX_04;
  CFlash *pCVar6;
  CBankFileAsyncData *local_14;
  CCriticalSectionLock local_10 [12];
  
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129b8b4;
  *(undefined ***)(this + 4) = &PTR_LAB_0129b8a0;
  paVar5 = (allocator<NCreatureHitNotification::CFlash> *)this;
  if (this[0x47] == (CBankFileAsyncData)0x0) {
    CCriticalSectionLock::CCriticalSectionLock
              (local_10,(_RTL_CRITICAL_SECTION *)(*(int *)(this + 0x28) + 0x14c),false);
    local_14 = this;
    FUN_009d7f40(&local_14);
    local_14 = this;
    FUN_009d7f40(&local_14);
    if (*(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> **)(this + 0x18) !=
        (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) {
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (*(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> **)
                  (*(int *)(this + 0x28) + 0x110),extraout_EDX,
                 *(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> **)(this + 0x18));
      cVar3 = extraout_AL;
      while (cVar3 == '\0') {
        CCriticalSectionLock::~CCriticalSectionLock(local_10);
        Sleep(0);
        CCriticalSectionLock::Lock(local_10);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (*(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> **)
                    (*(pair<EHeroMorphType,CParticleMorphs::CEntry> **)(this + 0x28) + 0x110),
                   *(pair<EHeroMorphType,CParticleMorphs::CEntry> **)(this + 0x28),
                   *(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> **)(this + 0x18));
        cVar3 = extraout_AL_00;
      }
    }
    if ((*(CHandleBasedMemoryPool **)(this + 0x1c) != (CHandleBasedMemoryPool *)0x0) &&
       (*(int *)(this + 0x20) != -1)) {
      CHandleBasedMemoryPool::Free(*(CHandleBasedMemoryPool **)(this + 0x1c),*(int *)(this + 0x20));
      *(undefined4 *)(this + 0x20) = 0xffffffff;
      *(undefined4 *)(this + 0xc) = 0;
      *(undefined4 *)(this + 8) = 0;
    }
    CCriticalSectionLock::~CCriticalSectionLock(local_10);
    paVar5 = extraout_ECX;
    in_EDX = extraout_EDX_00;
  }
  if (*(int *)(this + 0x24) != 0) {
    pvVar1 = *(void **)(this + 0xc);
    std::
    _Dest_val<std::allocator<NCreatureHitNotification::CFlash>,NCreatureHitNotification::CFlash>
              (paVar5,in_EDX);
    paVar5 = extraout_ECX_00;
    pCVar6 = extraout_EDX_01;
    if ((((pvVar1 != (void *)0x0) &&
         (paVar5 = (allocator<NCreatureHitNotification::CFlash> *)0x0,
         DAT_013caa90 != (CMemoryAllocator *)0x0)) &&
        (CMemoryAllocator::InternalDeallocate(DAT_013caa90,pvVar1), paVar5 = extraout_ECX_01,
        pCVar6 = extraout_EDX_02, DAT_013caa94 != '\0')) &&
       (pCVar4 = CWorld::DrawGetEnvironment((CWorld *)DAT_013caa90), pCVar2 = DAT_013caa90,
       paVar5 = extraout_ECX_02, pCVar6 = extraout_EDX_03, pCVar4 == (CIDrawEnvironment *)0x0)) {
      paVar5 = (allocator<NCreatureHitNotification::CFlash> *)0x0;
      if (DAT_013caa90 != (CMemoryAllocator *)0x0) {
        CMemoryAllocator::~CMemoryAllocator(DAT_013caa90);
        CMemoryAllocator::operator_delete(pCVar2);
        paVar5 = extraout_ECX_03;
        pCVar6 = extraout_EDX_04;
      }
      DAT_013caa90 = (CMemoryAllocator *)0x0;
    }
    std::
    _Dest_val<std::allocator<NCreatureHitNotification::CFlash>,NCreatureHitNotification::CFlash>
              (paVar5,pCVar6);
  }
  *(undefined ***)(this + 4) = &PTR_LAB_0129b860;
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  return;
}



//=== CCountedPointer<CBankFileAsyncEntry> @ 009d69a0 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall CCountedPointer<class CBankFileAsyncEntry>::CCountedPointer<class
   CBankFileAsyncEntry>(class CCountedPointer<class CBankFileAsyncEntry> const &) */

void __thiscall
CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
          (CCountedPointer<CBankFileAsyncEntry> *this,
          CCountedPointer<class_CBankFileAsyncEntry> *param_1)

{
  int *piVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 4) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return;
}



//=== CloseGraphicBank @ 009f8290 ===

/* [bsim sim=0.6102055792875188 <- ego_r]
   public: void __thiscall CGraphicBankManager::CloseGraphicBank(class CCountedPointer<class
   CGraphicDataBank>) */

void __thiscall
CGraphicBankManager::CloseGraphicBank(CGraphicBankManager *this,CBankFile *param_2,int *param_3)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  char cVar3;
  long lVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar5;
  int *piVar6;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined8 uVar7;
  CGraphicBankManager *local_4;
  
  piVar6 = (int *)**(int **)(this + 4);
  local_4 = this;
  if (piVar6 != *(int **)(this + 4)) {
    do {
      uVar7 = CBankFile::GetBankHandle(param_2);
      ppVar5 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar7 >> 0x20);
      puVar1 = (undefined4 *)piVar6[0xd];
      puVar2 = (undefined4 *)*(int *)uVar7;
      if (puVar1 == puVar2) {
        cVar3 = '\x01';
      }
      else if ((puVar1 == (undefined4 *)0x0) || (puVar2 == (undefined4 *)0x0)) {
        cVar3 = '\0';
      }
      else {
        ppVar5 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)puVar1[1];
        if (ppVar5 == (pair<EHeroMorphType,CParticleMorphs::CEntry> *)puVar2[1]) {
          lVar4 = CBasicString<char>::Compare((char *)*puVar1,(char *)*puVar2);
          cVar3 = '\x01' - (lVar4 != 0);
          ppVar5 = extraout_EDX;
        }
        else {
          cVar3 = '\0';
        }
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,ppVar5,
                 unaff_EDI);
      if (cVar3 != '\0') {
        std::
        list<CGraphicBankManager::CGraphicDataBankInfo,std::allocator<CGraphicBankManager::CGraphicDataBankInfo>_>
        ::erase((list<CGraphicBankManager::CGraphicDataBankInfo,std::allocator<CGraphicBankManager::CGraphicDataBankInfo>_>
                 *)(this + 4),&local_4,piVar6);
        break;
      }
      piVar6 = (int *)*piVar6;
    } while (piVar6 != (int *)*(int *)(this + 4));
  }
  if ((param_3 != (int *)0x0) && (*param_3 = *param_3 + -1, *param_3 == 0)) {
    (*(code *)param_3[1])();
    operator_delete(param_3);
  }
  return;
}



//=== ~CGraphicBankManager @ 009f86c0 ===

/* [bsim sim=0.6714196010357447 <- ego_r]
   public: virtual __thiscall CGraphicBankManager::~CGraphicBankManager(void) */

void __thiscall CGraphicBankManager::~CGraphicBankManager(CGraphicBankManager *this)

{
  list<CCountedPointer<NEntityEvents::CEventBase>,std::allocator<CCountedPointer<NEntityEvents::CEventBase>_>_>
  *plVar1;
  int *piVar2;
  int *piVar3;
  
  plVar1 = (list<CCountedPointer<NEntityEvents::CEventBase>,std::allocator<CCountedPointer<NEntityEvents::CEventBase>_>_>
            *)(this + 4);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129c7e4;
  piVar3 = (int *)**(int **)(this + 4);
  if (piVar3 != *(int **)(this + 4)) {
    do {
      piVar2 = (int *)piVar3[0xf];
      if (piVar2 != (int *)0x0) {
        *piVar2 = *piVar2 + -1;
        if (*(int *)piVar3[0xf] == 0) {
          (*(code *)((int *)piVar3[0xf])[1])();
          operator_delete((void *)piVar3[0xf]);
        }
      }
      piVar3[0xf] = 0;
      piVar3[0xe] = 0;
      piVar3 = (int *)*piVar3;
    } while (piVar3 != (int *)*(int *)plVar1);
  }
  std::
  list<CCountedPointer<NEntityEvents::CEventBase>,std::allocator<CCountedPointer<NEntityEvents::CEventBase>_>_>
  ::remove_if<NEntityEvents::CConvFilterToCP<NEntityEvents::CIsEventOfType>_>(plVar1);
  if (*(void **)plVar1 != (void *)0x0) {
    free(*(void **)plVar1);
  }
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  return;
}



//=== _Cons_val<std::allocator<CGraphicBankManager::CGraphicDataBankInfo>,CGraphicBankManager::CGraphicDataBankInfo,CGraphicBankManager::CGraphicDataBankInfo_const&> @ 009f8780 ===

/* [bsim sim=1.0 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<class
   CGraphicBankManager::CGraphicDataBankInfo>,class CGraphicBankManager::CGraphicDataBankInfo,class
   CGraphicBankManager::CGraphicDataBankInfo const &>(class std::allocator<class
   CGraphicBankManager::CGraphicDataBankInfo> &,class CGraphicBankManager::CGraphicDataBankInfo
   *,class CGraphicBankManager::CGraphicDataBankInfo const &) */

void __fastcall
std::
_Cons_val<std::allocator<CGraphicBankManager::CGraphicDataBankInfo>,CGraphicBankManager::CGraphicDataBankInfo,CGraphicBankManager::CGraphicDataBankInfo_const&>
          (allocator<CGraphicBankManager::CGraphicDataBankInfo> *param_1,
          CGraphicDataBankInfo *param_2,CGraphicDataBankInfo *param_3)

{
  int *piVar1;
  int iVar2;
  CGraphicDataBankInfo *pCVar3;
  allocator<CGraphicBankManager::CGraphicDataBankInfo> *paVar4;
  
  if (param_1 != (allocator<CGraphicBankManager::CGraphicDataBankInfo> *)0x0) {
    pCVar3 = param_2;
    paVar4 = param_1;
    for (iVar2 = 0xb; iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)paVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      paVar4 = paVar4 + 4;
    }
    CCharString::CCharString((CCharString *)(param_1 + 0x2c),(CCharString *)(param_2 + 0x2c));
    *(undefined4 *)(param_1 + 0x30) = *(undefined4 *)(param_2 + 0x30);
    piVar1 = *(int **)(param_2 + 0x34);
    *(int **)(param_1 + 0x34) = piVar1;
    if (piVar1 != (int *)0x0) {
      *piVar1 = *piVar1 + 1;
    }
  }
  return;
}



//=== ?GetDefault@CGraphicBankStateBlock@@UAEXAAVCBankStateBlock@@@Z @ 009fcef0 ===

void _GetDefault_CGraphicBankStateBlock__UAEXAAVCBankStateBlock___Z(int param_1)

{
  int iVar1;
  CPixelFormat *pCVar2;
  
  pCVar2 = (CPixelFormat *)(param_1 + 4);
  for (iVar1 = 0x18; iVar1 != 0; iVar1 = iVar1 + -1) {
    *(undefined4 *)pCVar2 = 0;
    pCVar2 = pCVar2 + 4;
  }
  CPixelFormat::Initialise((CPixelFormat *)(param_1 + 4),8,8,8,8,false);
  CPixelFormat::Initialise((CPixelFormat *)(param_1 + 8),8,8,8,8,false);
  CPixelFormat::Initialise((CPixelFormat *)(param_1 + 0x10),8,8,8,8,false);
  CPixelFormat::Initialise((CPixelFormat *)(param_1 + 0xc),8,8,8,8,false);
  CPixelFormat::Initialise((CPixelFormat *)(param_1 + 0x14),8,8,8,8,false);
  CPixelFormat::Initialise((CPixelFormat *)(param_1 + 0x18),8,8,8,8,false);
  CPixelFormat::Initialise((CPixelFormat *)(param_1 + 0x1c),0x3c);
  CPixelFormat::Initialise((CPixelFormat *)(param_1 + 0x20),0x31545844);
  CPixelFormat::Initialise((CPixelFormat *)(param_1 + 0x24),0x33545844);
  CPixelFormat::Initialise((CPixelFormat *)(param_1 + 0x2c),0x35545844);
  CPixelFormat::Initialise((CPixelFormat *)(param_1 + 0x28),0x31545844);
  CPixelFormat::Initialise((CPixelFormat *)(param_1 + 0x30),0x31545844);
  CPixelFormat::Initialise((CPixelFormat *)(param_1 + 0x34),0x33545844);
  CPixelFormat::Initialise((CPixelFormat *)(param_1 + 0x38),0x3c);
  *(undefined4 *)(param_1 + 0x3c) = 4;
  *(undefined4 *)(param_1 + 0x40) = 0x4b;
  *(undefined4 *)(param_1 + 0x44) = 0x20;
  *(undefined4 *)(param_1 + 0x48) = 0x20;
  *(undefined4 *)(param_1 + 0x4c) = 0x400;
  *(undefined4 *)(param_1 + 0x50) = 0x400;
  *(undefined4 *)(param_1 + 0x54) = 0xffffffff;
  *(undefined4 *)(param_1 + 0x58) = 0xffffffff;
  *(undefined1 *)(param_1 + 0x5c) = 0;
  *(undefined1 *)(param_1 + 0x5d) = 0;
  *(undefined1 *)(param_1 + 0x5e) = 0;
  *(undefined1 *)(param_1 + 0x60) = 0;
  *(undefined1 *)(param_1 + 0x62) = 0;
  *(undefined1 *)(param_1 + 99) = 0;
  *(undefined1 *)(param_1 + 0x5f) = 1;
  *(undefined1 *)(param_1 + 0x61) = 1;
  return;
}



//=== ?Set@CGraphicBankStateBlock@@MAEXPBVCBankStateBlock@@@Z @ 009fd020 ===

void _Set_CGraphicBankStateBlock__MAEXPBVCBankStateBlock___Z(undefined4 *param_1)

{
  undefined4 *in_ECX;
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  
  iVar1 = 7;
  puVar2 = param_1;
  puVar3 = in_ECX;
  while( true ) {
    puVar3 = puVar3 + 1;
    puVar2 = puVar2 + 1;
    if (iVar1 == 0) break;
    iVar1 = iVar1 + -1;
    *puVar3 = *puVar2;
  }
  puVar2 = param_1 + 8;
  puVar3 = in_ECX + 8;
  for (iVar1 = 7; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar3 = *puVar2;
    puVar2 = puVar2 + 1;
    puVar3 = puVar3 + 1;
  }
  in_ECX[0xf] = param_1[0xf];
  in_ECX[0x10] = param_1[0x10];
  in_ECX[0x11] = param_1[0x11];
  in_ECX[0x12] = param_1[0x12];
  in_ECX[0x13] = param_1[0x13];
  in_ECX[0x14] = param_1[0x14];
  *(undefined1 *)(in_ECX + 0x17) = *(undefined1 *)(param_1 + 0x17);
  in_ECX[0x15] = param_1[0x15];
  in_ECX[0x16] = param_1[0x16];
  *(undefined1 *)(in_ECX + 0x17) = *(undefined1 *)(param_1 + 0x17);
  *(undefined1 *)((int)in_ECX + 0x5d) = *(undefined1 *)((int)param_1 + 0x5d);
  *(undefined1 *)((int)in_ECX + 0x5e) = *(undefined1 *)((int)param_1 + 0x5e);
  *(undefined1 *)((int)in_ECX + 0x5f) = *(undefined1 *)((int)param_1 + 0x5f);
  *(undefined1 *)(in_ECX + 0x18) = *(undefined1 *)(param_1 + 0x18);
  *(undefined1 *)((int)in_ECX + 0x61) = *(undefined1 *)((int)param_1 + 0x61);
  *(undefined1 *)((int)in_ECX + 0x62) = *(undefined1 *)((int)param_1 + 0x62);
  *(undefined1 *)((int)in_ECX + 99) = *(undefined1 *)((int)param_1 + 99);
  return;
}



//=== ?PrepareDataForInclusion@CGraphicDataBank@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z @ 009fd990 ===

/* [bsim sim=0.5518519667954678 <- ego_r]
   protected: virtual void __thiscall CWorldMap::LoadScriptBrushesFromFile(long,class
   CCountedPointer<class CAFile>) */

void __thiscall
CWorldMap::
_PrepareDataForInclusion_CGraphicDataBank__MAEXKABV__CArray_VCCharString____KV__CCountedPointer_VCBankStateBlock____AAVCDataOutputStream__2_Z
          (void)

{
  int *in_stack_00000014;
  
  if ((in_stack_00000014 != (int *)0x0) &&
     (*in_stack_00000014 = *in_stack_00000014 + -1, *in_stack_00000014 == 0)) {
    (*(code *)in_stack_00000014[1])();
    operator_delete(in_stack_00000014);
  }
  return;
}



//=== ?NotifyBankSize@CGraphicDataBank@@MAEXKABV?$CArray@VCBankFileDataTypeCount@@@@@Z @ 009ff8b0 ===

void _NotifyBankSize_CGraphicDataBank__MAEXKABV__CArray_VCBankFileDataTypeCount_____Z
               (EWeaponClass *param_1)

{
  int in_ECX;
  EWeaponClass *unaff_retaddr;
  
  std::vector<EWeaponClass,std::allocator<EWeaponClass>_>::_Umove<EWeaponClass*>
            ((vector<EWeaponClass,std::allocator<EWeaponClass>_> *)(in_ECX + 0x1e0),param_1,
             unaff_retaddr,param_1);
  return;
}



//=== ?CreateCRC@CGraphicBankStateBlock@@UAEKXZ @ 00a00940 ===

void _CreateCRC_CGraphicBankStateBlock__UAEKXZ(void)

{
  ulong in_ECX;
  void *in_EDX;
  
  CCRC::Calc(in_ECX,in_EDX,in_ECX + 4);
  return;
}



//=== CloseGraphicBank @ 00a09dc0 ===

/* [bsim sim=0.6102055792875188 <- ego_r]
   public: void __thiscall CGraphicBankManager::CloseGraphicBank(class CCountedPointer<class
   CGraphicDataBank>) */

void __thiscall
CGraphicBankManager::CloseGraphicBank(CGraphicBankManager *this,CBankFile *param_2,int *param_3)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  char cVar3;
  long lVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar5;
  int *piVar6;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined8 uVar7;
  CGraphicBankManager *local_4;
  
  piVar6 = (int *)**(int **)(this + 8);
  local_4 = this;
  if (piVar6 != *(int **)(this + 8)) {
    do {
      uVar7 = CBankFile::GetBankHandle(param_2);
      ppVar5 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar7 >> 0x20);
      puVar1 = (undefined4 *)piVar6[2];
      puVar2 = (undefined4 *)*(int *)uVar7;
      if (puVar1 == puVar2) {
        cVar3 = '\x01';
      }
      else if ((puVar1 == (undefined4 *)0x0) || (puVar2 == (undefined4 *)0x0)) {
        cVar3 = '\0';
      }
      else {
        ppVar5 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)puVar1[1];
        if (ppVar5 == (pair<EHeroMorphType,CParticleMorphs::CEntry> *)puVar2[1]) {
          lVar4 = CBasicString<char>::Compare((char *)*puVar1,(char *)*puVar2);
          cVar3 = '\x01' - (lVar4 != 0);
          ppVar5 = extraout_EDX;
        }
        else {
          cVar3 = '\0';
        }
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,ppVar5,
                 unaff_EDI);
      if (cVar3 != '\0') {
        std::
        list<CGraphicBankManager::CGraphicDataBankInfo,std::allocator<CGraphicBankManager::CGraphicDataBankInfo>_>
        ::erase((list<CGraphicBankManager::CGraphicDataBankInfo,std::allocator<CGraphicBankManager::CGraphicDataBankInfo>_>
                 *)(this + 8),&local_4,piVar6);
        break;
      }
      piVar6 = (int *)*piVar6;
    } while (piVar6 != (int *)*(int *)(this + 8));
  }
  if ((param_3 != (int *)0x0) && (*param_3 = *param_3 + -1, *param_3 == 0)) {
    (*(code *)param_3[1])();
    operator_delete(param_3);
  }
  return;
}



//=== ?GetDefault@CMeshBankStateBlock@@UAEXAAVCBankStateBlock@@@Z @ 00a23a30 ===

void _GetDefault_CMeshBankStateBlock__UAEXAAVCBankStateBlock___Z(int param_1)

{
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  *(undefined4 *)(param_1 + 0xc) = 0;
  *(undefined4 *)(param_1 + 0x10) = 0;
  *(undefined1 *)(param_1 + 4) = 1;
  *(undefined1 *)(param_1 + 5) = 0;
  *(undefined1 *)(param_1 + 6) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  *(undefined4 *)(param_1 + 0xc) = 0;
  *(undefined4 *)(param_1 + 0x10) = 0;
  return;
}



//=== ?PrepareDataForInclusion@CMeshDataBank@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z @ 00a24410 ===

/* [bsim sim=0.5518519667954678 <- ego_r]
   protected: virtual void __thiscall CWorldMap::LoadScriptBrushesFromFile(long,class
   CCountedPointer<class CAFile>) */

void __thiscall
CWorldMap::
_PrepareDataForInclusion_CMeshDataBank__MAEXKABV__CArray_VCCharString____KV__CCountedPointer_VCBankStateBlock____AAVCDataOutputStream__2_Z
          (void)

{
  int *in_stack_00000014;
  
  if ((in_stack_00000014 != (int *)0x0) &&
     (*in_stack_00000014 = *in_stack_00000014 + -1, *in_stack_00000014 == 0)) {
    (*(code *)in_stack_00000014[1])();
    operator_delete(in_stack_00000014);
  }
  return;
}



//=== ?NotifyBankSize@CMeshDataBank@@MAEXKABV?$CArray@VCBankFileDataTypeCount@@@@@Z @ 00a26660 ===

void _NotifyBankSize_CMeshDataBank__MAEXKABV__CArray_VCBankFileDataTypeCount_____Z
               (uint param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int in_ECX;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 auStack_8 [2];
  
  iVar5 = 0;
  auStack_8[0] = 0;
  std::vector<unsigned_int,std::allocator<unsigned_int>_>::resize
            ((vector<unsigned_int,std::allocator<unsigned_int>_> *)(in_ECX + 0x380),param_1,
             (uint)auStack_8);
  if (*(char *)(in_ECX + 0x8d) != '\0') {
    piVar4 = (int *)*param_2;
    iVar7 = 0;
    iVar6 = 0;
    for (iVar3 = param_2[1] - (int)piVar4 >> 3; iVar3 != 0; iVar3 = iVar3 + -1) {
      iVar1 = *piVar4;
      iVar2 = piVar4[1];
      if ((((iVar1 == 1) || (iVar1 == 2)) || (iVar1 == 4)) || (iVar1 == 5)) {
        iVar5 = iVar5 + iVar2;
      }
      else if (iVar1 == 3) {
        iVar7 = iVar7 + iVar2;
      }
      else if (((iVar1 == 6) || (iVar1 == 10)) || ((iVar1 == 7 || ((iVar1 == 8 || (iVar1 == 9))))))
      {
        iVar6 = iVar6 + iVar2;
      }
      piVar4 = piVar4 + 2;
    }
    FUN_00a2c210(iVar5);
    FUN_00a2c2b0(iVar7);
    FUN_00a2c360(iVar6);
  }
  return;
}



//=== ?Set@CMeshBankStateBlock@@MAEXPBVCBankStateBlock@@@Z @ 00a2d6c0 ===

/* [bsim sim=0.7562586409215167 <- ego_r]
   public: __thiscall CObjectAugmentationParticleSet::CObjectAugmentationParticleSet(class
   CObjectAugmentationParticleSet const &) */

void __thiscall
CObjectAugmentationParticleSet::_Set_CMeshBankStateBlock__MAEXPBVCBankStateBlock___Z
          (CObjectAugmentationParticleSet *this,CObjectAugmentationParticleSet *param_1)

{
  *(undefined4 *)(this + 4) = *(undefined4 *)(param_1 + 4);
  *(undefined4 *)(this + 8) = *(undefined4 *)(param_1 + 8);
  *(undefined4 *)(this + 0xc) = *(undefined4 *)(param_1 + 0xc);
  *(undefined4 *)(this + 0x10) = *(undefined4 *)(param_1 + 0x10);
  CScriptGameResourceObjectScriptedThingBase::operator=
            ((CScriptGameResourceObjectScriptedThingBase *)(this + 0x14),
             (CScriptGameResourceObjectScriptedThingBase *)(param_1 + 0x14));
  FUN_006b9e00(param_1 + 0x28);
  return;
}



//=== ?GetDefault@CWADBankStateBlock@@UAEXAAVCBankStateBlock@@@Z @ 00a3a2c0 ===

void _GetDefault_CWADBankStateBlock__UAEXAAVCBankStateBlock___Z(void)

{
  return;
}



//=== ?Set@CWADBankStateBlock@@MAEXPBVCBankStateBlock@@@Z @ 00a3a2e0 ===

void _Set_CWADBankStateBlock__MAEXPBVCBankStateBlock___Z(void)

{
  return;
}



//=== ?PrepareDataForInclusion@CLipSyncDataBank@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z @ 00a42940 ===

/* [bsim sim=0.5518519667954678 <- ego_r]
   protected: virtual void __thiscall CWorldMap::LoadScriptBrushesFromFile(long,class
   CCountedPointer<class CAFile>) */

void __thiscall
CWorldMap::
_PrepareDataForInclusion_CLipSyncDataBank__MAEXKABV__CArray_VCCharString____KV__CCountedPointer_VCBankStateBlock____AAVCDataOutputStream__2_Z
          (void)

{
  int *in_stack_00000014;
  
  if ((in_stack_00000014 != (int *)0x0) &&
     (*in_stack_00000014 = *in_stack_00000014 + -1, *in_stack_00000014 == 0)) {
    (*(code *)in_stack_00000014[1])();
    operator_delete(in_stack_00000014);
  }
  return;
}



//=== ?NotifyBankSize@CLipSyncDataBank@@MAEXKABV?$CArray@VCBankFileDataTypeCount@@@@@Z @ 00a42e70 ===

/* [bsim sim=0.8805626837389016 <- ego_r]
   protected: enum EWeaponClass * __thiscall std::vector<enum EWeaponClass,class std::allocator<enum
   EWeaponClass> >::_Umove<enum EWeaponClass *>(enum EWeaponClass *,enum EWeaponClass *,enum
   EWeaponClass *) */

EWeaponClass * __thiscall
std::vector<EWeaponClass,std::allocator<EWeaponClass>_>::
_NotifyBankSize_CLipSyncDataBank__MAEXKABV__CArray_VCBankFileDataTypeCount_____Z
          (vector<EWeaponClass,std::allocator<EWeaponClass>_> *this,EWeaponClass *param_1,
          EWeaponClass *param_2,EWeaponClass *param_3)

{
  EWeaponClass *extraout_EAX;
  undefined4 local_8;
  undefined4 local_4;
  
  local_8 = 0;
  local_4 = 0;
  vector<unsigned_int,std::allocator<unsigned_int>_>::resize
            ((vector<unsigned_int,std::allocator<unsigned_int>_> *)(this + 0x164),(uint)param_1,
             (uint)&local_8);
  return extraout_EAX;
}



//=== ?GetDefault@CStreamingFontBankStateBlock@@UAEXAAVCBankStateBlock@@@Z @ 00a5ee20 ===

void _GetDefault_CStreamingFontBankStateBlock__UAEXAAVCBankStateBlock___Z(int param_1)

{
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 400;
  *(undefined4 *)(param_1 + 0xc) = 0;
  *(undefined1 *)(param_1 + 0x10) = 0;
  return;
}



//=== ?Set@CStreamingFontBankStateBlock@@MAEXPBVCBankStateBlock@@@Z @ 00a5ee40 ===

void _Set_CStreamingFontBankStateBlock__MAEXPBVCBankStateBlock___Z(int param_1)

{
  int in_ECX;
  
  *(undefined4 *)(in_ECX + 4) = *(undefined4 *)(param_1 + 4);
  *(undefined4 *)(in_ECX + 8) = *(undefined4 *)(param_1 + 8);
  *(undefined4 *)(in_ECX + 0xc) = *(undefined4 *)(param_1 + 0xc);
  *(undefined1 *)(in_ECX + 0x10) = *(undefined1 *)(param_1 + 0x10);
  return;
}



//=== ?PrepareDataForInclusion@CStreamingFontDataBank@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z @ 00a5f0f0 ===

/* [bsim sim=0.5518519667954678 <- ego_r]
   protected: virtual void __thiscall CWorldMap::LoadScriptBrushesFromFile(long,class
   CCountedPointer<class CAFile>) */

void __thiscall
CWorldMap::
_PrepareDataForInclusion_CStreamingFontDataBank__MAEXKABV__CArray_VCCharString____KV__CCountedPointer_VCBankStateBlock____AAVCDataOutputStream__2_Z
          (void)

{
  int *in_stack_00000014;
  
  if ((in_stack_00000014 != (int *)0x0) &&
     (*in_stack_00000014 = *in_stack_00000014 + -1, *in_stack_00000014 == 0)) {
    (*(code *)in_stack_00000014[1])();
    operator_delete(in_stack_00000014);
  }
  return;
}



//=== ?GetDefault@CShaderBankStateBlock@@UAEXAAVCBankStateBlock@@@Z @ 00a61f20 ===

void _GetDefault_CShaderBankStateBlock__UAEXAAVCBankStateBlock___Z(int param_1)

{
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 8) = 1;
  *(undefined4 *)(param_1 + 4) = 0;
  return;
}



//=== ?Set@CShaderBankStateBlock@@MAEXPBVCBankStateBlock@@@Z @ 00a61f40 ===

void _Set_CShaderBankStateBlock__MAEXPBVCBankStateBlock___Z(int param_1)

{
  int in_ECX;
  
  *(undefined1 *)(in_ECX + 8) = *(undefined1 *)(param_1 + 8);
  *(undefined4 *)(in_ECX + 4) = *(undefined4 *)(param_1 + 4);
  return;
}



//=== ?PrepareDataForInclusion@CShaderDataBank@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z @ 00a62000 ===

/* [bsim sim=0.5518519667954678 <- ego_r]
   protected: virtual void __thiscall CWorldMap::LoadScriptBrushesFromFile(long,class
   CCountedPointer<class CAFile>) */

void __thiscall
CWorldMap::
_PrepareDataForInclusion_CShaderDataBank__MAEXKABV__CArray_VCCharString____KV__CCountedPointer_VCBankStateBlock____AAVCDataOutputStream__2_Z
          (void)

{
  int *in_stack_00000014;
  
  if ((in_stack_00000014 != (int *)0x0) &&
     (*in_stack_00000014 = *in_stack_00000014 + -1, *in_stack_00000014 == 0)) {
    (*(code *)in_stack_00000014[1])();
    operator_delete(in_stack_00000014);
  }
  return;
}



//=== ?GetDefault@CFontBankStateBlock@@UAEXAAVCBankStateBlock@@@Z @ 00a632d0 ===

void _GetDefault_CFontBankStateBlock__UAEXAAVCBankStateBlock___Z(int param_1)

{
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 400;
  *(undefined1 *)(param_1 + 0xc) = 0;
  return;
}



//=== ?Set@CFontBankStateBlock@@MAEXPBVCBankStateBlock@@@Z @ 00a632f0 ===

void _Set_CFontBankStateBlock__MAEXPBVCBankStateBlock___Z(int param_1)

{
  int in_ECX;
  
  *(undefined4 *)(in_ECX + 4) = *(undefined4 *)(param_1 + 4);
  *(undefined4 *)(in_ECX + 8) = *(undefined4 *)(param_1 + 8);
  *(undefined1 *)(in_ECX + 0xc) = *(undefined1 *)(param_1 + 0xc);
  return;
}



//=== ?PrepareDataForInclusion@CFontDataBank@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z @ 00a63420 ===

/* [bsim sim=0.5518519667954678 <- ego_r]
   protected: virtual void __thiscall CWorldMap::LoadScriptBrushesFromFile(long,class
   CCountedPointer<class CAFile>) */

void __thiscall
CWorldMap::
_PrepareDataForInclusion_CFontDataBank__MAEXKABV__CArray_VCCharString____KV__CCountedPointer_VCBankStateBlock____AAVCDataOutputStream__2_Z
          (void)

{
  int *in_stack_00000014;
  
  if ((in_stack_00000014 != (int *)0x0) &&
     (*in_stack_00000014 = *in_stack_00000014 + -1, *in_stack_00000014 == 0)) {
    (*(code *)in_stack_00000014[1])();
    operator_delete(in_stack_00000014);
  }
  return;
}



//=== ~CLipSyncBankStateBlock @ 00add1f0 ===

/* [bsim sim=0.7058260062185795 <- ego_r]
   public: virtual __thiscall CLipSyncBankStateBlock::~CLipSyncBankStateBlock(void) */

void __thiscall CLipSyncBankStateBlock::~CLipSyncBankStateBlock(CLipSyncBankStateBlock *this)

{
  int *piVar1;
  
  *(undefined ***)this = &PTR_LAB_0129f094;
  piVar1 = *(int **)(this + 8);
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + -1;
    if (**(int **)(this + 8) == 0) {
      (*(code *)(*(int **)(this + 8))[1])();
      operator_delete(*(void **)(this + 8));
    }
  }
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined ***)this = &PTR_LAB_0129f06c;
  return;
}



//=== ~CBankFileEntryUpdateData @ 00b42a90 ===

/* [bsim sim=0.5523788414885696 <- ego_r]
   private: __thiscall CBankFileEntryUpdateData::~CBankFileEntryUpdateData(void) */

void __thiscall CBankFileEntryUpdateData::~CBankFileEntryUpdateData(CBankFileEntryUpdateData *this)

{
  int *piVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar2;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *in_stack_00000004;
  
  CEngineMap::CloseStaticMap((CEngineMap *)this);
  if (*(int *)this != 0) {
    CEngineLandscapeRenderer::DeleteMap(DAT_01436ea8,(CEngineMap *)this);
  }
  if (*(int *)(this + 4) != 0) {
    CEngineResourceManager::WorldFrameUpdate(DAT_01436e88,SUB41(this,0));
  }
  FUN_00be05d0();
  ~CBankFileEntryUpdateData(this + 0x44);
  piVar1 = *(int **)(this + 0x40);
  ppVar2 = extraout_EDX;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + -1;
    if (**(int **)(this + 0x40) == 0) {
      (*(code *)(*(int **)(this + 0x40))[1])();
      operator_delete(*(void **)(this + 0x40));
      ppVar2 = extraout_EDX_00;
    }
  }
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0x1c),ppVar2,
             in_stack_00000004);
  return;
}



//=== ?GetDefault@CStaticMapBankStateBlock@@UAEXAAVCBankStateBlock@@@Z @ 00b43390 ===

void _GetDefault_CStaticMapBankStateBlock__UAEXAAVCBankStateBlock___Z(void)

{
  return;
}



//=== ?Set@CStaticMapBankStateBlock@@MAEXPBVCBankStateBlock@@@Z @ 00b433b0 ===

void _Set_CStaticMapBankStateBlock__MAEXPBVCBankStateBlock___Z(void)

{
  return;
}



//=== ~CBankFileEntryUpdateData @ 00b443b0 ===

/* [bsim sim=0.5042203184583138 <- ego_r]
   private: __thiscall CBankFileEntryUpdateData::~CBankFileEntryUpdateData(void) */

void __thiscall CBankFileEntryUpdateData::~CBankFileEntryUpdateData(CBankFileEntryUpdateData *this)

{
  CBankFileEntryUpdateData *pCVar1;
  int *piVar2;
  
  pCVar1 = this + 8;
  if (*(int *)(this + 0xc) != 0) {
    FUN_00b441f0(*(undefined4 *)(*(int *)pCVar1 + 4));
    *(int *)(*(int *)pCVar1 + 8) = *(int *)pCVar1;
    *(undefined4 *)(*(int *)pCVar1 + 4) = 0;
    *(int *)(*(int *)pCVar1 + 0xc) = *(int *)pCVar1;
    *(undefined4 *)(this + 0xc) = 0;
  }
  if (*(void **)pCVar1 != (void *)0x0) {
    free(*(void **)pCVar1);
  }
  piVar2 = *(int **)(this + 4);
  if (piVar2 != (int *)0x0) {
    *piVar2 = *piVar2 + -1;
    if (**(int **)(this + 4) == 0) {
      (*(code *)(*(int **)(this + 4))[1])();
      operator_delete(*(void **)(this + 4));
    }
  }
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = 0;
  return;
}



//=== LoadTextures @ 00bf4130 ===

/* [bsim sim=0.9019257035151096 <- ego_r]
   private: bool __thiscall CEngineLandscapePatch::LoadTextures(float) */

bool __thiscall CEngineLandscapePatch::LoadTextures(CEngineLandscapePatch *this,float param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  ushort *puVar4;
  uint uVar5;
  int local_4;
  
  if (((byte)this[0x3c] & 1) == 0) {
    this[0x3c] = (CEngineLandscapePatch)((byte)this[0x3c] | 1);
    local_4 = 0;
    if (*(short *)(this + 0x10) != 0) {
      puVar4 = *(ushort **)(this + 0xc);
      do {
        iVar3 = *(int *)(DAT_01436e98 + 0x954);
        iVar2 = *(int *)(iVar3 + 0x1e0);
        uVar5 = (uint)*puVar4;
        if ((uVar5 < (uint)((*(int *)(iVar3 + 0x1e4) - iVar2) / 0x2c)) &&
           (uVar1 = uVar5 * 0x2c + iVar2, uVar1 != 0)) {
          iVar3 = *(int *)((-(uint)(uVar5 < (uint)((*(int *)(iVar3 + 0x1e4) - iVar2) / 0x2c)) &
                           uVar1) + 0x28);
        }
        else {
          iVar3 = 0;
        }
        if ((*(int *)(iVar3 + 8) == 0) || (*(int *)(*(int *)(iVar3 + 8) + 0x1c) == 0)) {
          this[0x3c] = (CEngineLandscapePatch)((byte)this[0x3c] & 0xfe);
        }
        local_4 = local_4 + 1;
        puVar4 = puVar4 + 1;
      } while (local_4 < (int)(uint)*(ushort *)(this + 0x10));
    }
  }
  CEngineLandscapeRenderer::AddFrameForegroundTexturesUsed
            (DAT_01436ea8,*(ushort **)(this + 0xc),(uint)*(ushort *)(this + 0x10),param_1);
  return (bool)((byte)this[0x3c] & 1);
}



