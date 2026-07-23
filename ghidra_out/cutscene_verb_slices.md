# RunCutsceneMacro_Func @ 00cbfb7d — verb handler slices

## UseCamera (xref 00cc9f39, line 8884, dist 5)

```c
      8872 00cc9089 | (**(code **)(*DAT_0143e8f8 + 0x1c))();
      8873          | }
      8874 00cc9097 | PlayCutsceneSound((map<int,CCharString,std::less<int>,std::allocator<std::pair<int_const_,CCharString>_>_>
      8875 00cc9094 | *)local_78,DAT_013b83c8);
      8876 00cc90a3 | ppVar22 = extraout_EDX_x00270;
      8877 00cc909c | if (DAT_013b83c8 != -1) {
      8878 00cc90a9 | DAT_013b83c8 = DAT_013b83c8 + 1;
      8879          | }
      8880          | }
      8881          | }
      8882 00cd17fd | goto LAB_00cd17fd;
      8883          | }
>>>   8884 00cc9f3e | CCharString::CCharString(local_1f0,"UseCamera",-1);
      8885 00cc9f49 | sVar13 = CCharString::GetLength(local_1f0);
      8886 00cc9f55 | pcVar9 = CCharString::operator_char_const*(local_1f0);
      8887 00cc9f61 | pcVar12 = CCharString::operator_char_const*(local_c);
      8888 00cc9f6a | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      8889          | std::
      8890 00cc9f80 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8891          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8892 00cc9f78 | local_1f0,extraout_EDX_x00328,ppVar48);
      8893 00cc9f74 | if (iVar33 == 0) {
      8894 00cc9f8d | lVar6 = CCharString::GetLength((CCharString *)&local_44);
      8895 00cc9f97 | ppVar22 = extraout_EDX_x00329;
      8896 00cc9f95 | if ((lVar6 == 0) ||
      8897 00cc9f9d | (bVar3 = IsFalse((CCharString *)&local_44), ppVar22 = extraout_EDX_x00330,
      8898 00cc9fa7 | bVar3)) goto LAB_00cd17fd;
      8899 00cc9fb3 | local_118 = -1.0;
      8900 00cc9fad | CCarriedReadableDef::CCarriedReadableDef(local_85c);
      8901 00cc9fc2 | local_6c = (C3DMeshInfo *)0x0;
      8902 00cc9fc6 | lVar6 = CCharString::GetLength((CCharString *)&local_40);
      8903 00cc9fd1 | if (lVar6 != 0) {
      8904 00cc9fd5 | pcVar9 = CCharString::operator_char_const*((CCharString *)&local_40);
      8905 00cc9fdf | local_118 = GFStringToFloat(pcVar9);
      8906          | }
      8907 00cc9fea | lVar6 = CCharString::GetLength((CCharString *)&local_3c);
      8908 00cc9ff2 | if ((lVar6 != 0) && (bVar3 = IsFalse((CCharString *)&local_3c), !bVar3)) {
      8909 00cca00d | CRegion::SetMiniMapRegionExitTextOffsetX(local_48,local_1188,(float)&local_3c)
      8910          | ;
      8911 00cca01d | iVar33 = *extraout_EAX_13;
      8912 00cca022 | param_4 = *(pair<EHeroMorphType,CParticleMorphs::CEntry> **)local_48;
      8913 00cca02e | local_50 = (double)CONCAT44(iVar33,(EWeaponClass)local_50);
      8914 00cca031 | bVar3 = std::
      8915          | _Tree_const_iterator<std::_Tree_val<std::_Tmap_traits<CCharString,long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,long>_>,0>_>_>
      8916 00cca031 | ::operator!=((_Tree_const_iterator<std::_Tree_val<std::_Tmap_traits<CCharString,long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,long>_>,0>_>_>
      8917 00cca02b | *)((int)&local_50 + 4),
      8918          | (_Tree_const_iterator<class_std::_Tree_val<class_std::_Tmap_traits<class_CCharString,long,struct_std::less<class_CCharString>,class_std::allocator<struct_std::pair<class_CCharString_const_,long>_>,0>_>_>
      8919 00cca027 | *)&param_4);
      8920 00cca038 | if (bVar3) {
      8921 00cca03a | pCVar31 = (C3DMeshInfo *)(iVar33 + 0x14);
      8922 00cca03f | bVar3 = C3DMeshInfo::HasPhysicsMesh(pCVar31);
      8923 00cca046 | if (((bVar3) && (pCVar31 != (C3DMeshInfo *)0x0)) &&
      8924 00cca04e | (bVar3 = C3DMeshInfo::HasPhysicsMesh(pCVar31), bVar3)) {
      8925 00cca057 | pCVar15 = (CScriptThing *)(**(code **)(*(int *)pCVar31 + 0x30))();
      8926 00cca066 | CScriptThing::operator=((CScriptThing *)local_85c,pCVar15);
      8927 00cca071 | C3DClothPrimitive::~C3DClothPrimitive(local_12c0);
      8928 00cca109 | goto LAB_00cca109;
      8929          | }
      8930          | }
      8931 00cca084 | puVar14 = (undefined4 *)CCharString::CCharString(local_b4c,"HERO",-1);
      8932 00cca08f | pCVar1 = (CTCParticleAttacher *)*puVar14;
      8933 00cca091 | if (pCVar1 == local_3c) {
      8934 00cca0b0 | LAB_00cca0b0:
      8935 00cca0b6 | pCVar15 = (CScriptThing *)(**(code **)(*DAT_0143e8f8 + 0x118))();
      8936          | }
      8937          | else {
      8938 00cca096 | if ((pCVar1 == (CTCParticleAttacher *)0x0) ||
      8939 00cca09a | (local_3c == (CTCParticleAttacher *)0x0)) {
      8940 00cca0aa | cVar4 = '\0';
      8941          | }
      8942          | else {
      8943 00cca0a3 | CTCParticleAttacher::OnInitialActivate(pCVar1);
      8944 00cca0a8 | cVar4 = extraout_AL_36;
      8945          | }
      8946 00cca0ac | if (cVar4 != '\0') goto LAB_00cca0b0;
      8947 00cca0c8 | local_14 = local_14 | 0x200;
      8948 00cca0c6 | pCVar15 = (CScriptThing *)(**(code **)(*DAT_0143e8f8 + 0x120))();
      8949          | }
      8950 00cca0de | CScriptThing::operator=((CScriptThing *)local_85c,pCVar15);
      8951 00cca0ed | ppVar22 = extraout_EDX_x00331;
      8952 00cca0e9 | if ((local_14 & 0x200) != 0) {
      8953 00cca0ef | local_14 = local_14 & 0xfffffdff;
      8954 00cca0f3 | C3DClothPrimitive::~C3DClothPrimitive(local_12d8);
      8955 00cca0f9 | ppVar22 = extraout_EDX_x00332;
      8956          | }
      8957          | std::
      8958 00cca104 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8959          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8960 00cca0fe | local_b4c,ppVar22,ppVar48);
      8961          | }
      8962 00cca109 | LAB_00cca109:
      8963 00cca109 | lVar6 = CCharString::GetLength((CCharString *)&local_38);
      8964 00cca111 | if (lVar6 != 0) {
      8965 00cca115 | pcVar9 = CCharString::operator_char_const*((CCharString *)&local_38);
      8966 00cca11f | GFStringToInt(pcVar9);
      8967          | }
      8968 00cca12b | lVar6 = CCharString::GetLength((CCharString *)&local_34);
      8969 00cca133 | if (lVar6 != 0) {
      8970 00cca137 | pcVar9 = CCharString::operator_char_const*((CCharString *)&local_34);
      8971 00cca141 | GFStringToInt(pcVar9);
      8972          | }
      8973 00cca148 | bVar3 = CScriptThing::?IsNull@CScriptThing@@UBE_NXZ((CScriptThing *)local_85c);
      8974 00cca155 | if ((bVar3) ||
```

Helpers called in window:
- `?IsNull@CScriptThing@@UBE_NXZ` (callsite 00cca14e)
- `CCarriedReadableDef` (callsite 00cc9fbd)
- `CCharString` (callsite 00cca08a)
- `GFStringToFloat` (callsite 00cc9fdf)
- `GFStringToInt` (callsite 00cca141)
- `GetLength` (callsite 00cca12e)
- `HasPhysicsMesh` (callsite 00cca04e)
- `IsFalse` (callsite 00cc9ffd)
- `OnInitialActivate` (callsite 00cca0a3)
- `PlayCutsceneSound` (callsite 00cc9097)
- `SetMiniMapRegionExitTextOffsetX` (callsite 00cca018)
- `_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>` (callsite 00cca104)
- `operator!=` (callsite 00cca031)
- `operator=` (callsite 00cca0e4)
- `operator_char_const*` (callsite 00cca13a)
- `strncmp` (callsite 00cc9f6a)
- `~C3DClothPrimitive` (callsite 00cca0f9)

## NoLoadUseCamera (xref 00cc9e69, line 8844, dist 5)

```c
      8832 00cc9e18 | pcVar12 = CCharString::operator_char_const*(local_c);
      8833 00cc9e21 | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      8834          | std::
      8835 00cc9e37 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8836          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8837 00cc9e2f | local_204,extraout_EDX_x00323,ppVar48);
      8838 00cc9e2b | if (iVar33 == 0) {
      8839 00cc9e48 | (**(code **)(*DAT_0143e8f8 + 0x684))();
      8840 00cc9e5a | (**(code **)(*DAT_0143e8f8 + 0x680))();
      8841 00cc9e62 | ppVar22 = extraout_EDX_x00324;
      8842 00cd17fd | goto LAB_00cd17fd;
      8843          | }
>>>   8844 00cc9e6e | CCharString::CCharString(local_20c,"NoLoadUseCamera",-1);
      8845 00cc9e79 | sVar13 = CCharString::GetLength(local_20c);
      8846 00cc9e85 | pcVar9 = CCharString::operator_char_const*(local_20c);
      8847 00cc9e91 | pcVar12 = CCharString::operator_char_const*(local_c);
      8848 00cc9e9a | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      8849          | std::
      8850 00cc9eb0 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8851          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8852 00cc9ea8 | local_20c,extraout_EDX_x00325,ppVar48);
      8853 00cc9ea4 | if (iVar33 == 0) {
      8854 00cc9eb9 | lVar6 = CCharString::GetLength((CCharString *)&local_44);
      8855 00cc9ec3 | ppVar22 = extraout_EDX_x00326;
      8856 00cc9ec1 | if (lVar6 != 0) {
      8857 00cc9ec9 | lVar6 = CCharString::GetLength((CCharString *)&local_40);
      8858 00cc9ed6 | if (lVar6 != 0) {
      8859 00cc9eda | pcVar9 = CCharString::operator_char_const*((CCharString *)&local_40);
      8860 00cc9ee4 | GFStringToInt(pcVar9);
      8861          | }
      8862 00cc9eeb | lVar6 = CCharString::GetLength((CCharString *)&local_3c);
      8863 00cc9ef3 | if (lVar6 != 0) {
      8864 00cc9ef7 | pcVar9 = CCharString::operator_char_const*((CCharString *)&local_3c);
      8865 00cc9f01 | GFStringToInt(pcVar9);
      8866          | }
      8867 00cc9f14 | (**(code **)(*DAT_0143e8f8 + 0x670))(&local_44);
      8868 00cc9f2c | ppVar22 = extraout_EDX_x00327;
      8869 00cc9f28 | if (local_91 != '\0') {
      8870 00cc907d | LAB_00cc907d:
      8871 00cc907d | if (uStack_8._3_1_ != '\0') {
      8872 00cc9089 | (**(code **)(*DAT_0143e8f8 + 0x1c))();
      8873          | }
      8874 00cc9097 | PlayCutsceneSound((map<int,CCharString,std::less<int>,std::allocator<std::pair<int_const_,CCharString>_>_>
      8875 00cc9094 | *)local_78,DAT_013b83c8);
      8876 00cc90a3 | ppVar22 = extraout_EDX_x00270;
      8877 00cc909c | if (DAT_013b83c8 != -1) {
      8878 00cc90a9 | DAT_013b83c8 = DAT_013b83c8 + 1;
      8879          | }
      8880          | }
      8881          | }
      8882 00cd17fd | goto LAB_00cd17fd;
      8883          | }
      8884 00cc9f3e | CCharString::CCharString(local_1f0,"UseCamera",-1);
      8885 00cc9f49 | sVar13 = CCharString::GetLength(local_1f0);
      8886 00cc9f55 | pcVar9 = CCharString::operator_char_const*(local_1f0);
      8887 00cc9f61 | pcVar12 = CCharString::operator_char_const*(local_c);
      8888 00cc9f6a | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      8889          | std::
      8890 00cc9f80 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8891          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8892 00cc9f78 | local_1f0,extraout_EDX_x00328,ppVar48);
      8893 00cc9f74 | if (iVar33 == 0) {
      8894 00cc9f8d | lVar6 = CCharString::GetLength((CCharString *)&local_44);
      8895 00cc9f97 | ppVar22 = extraout_EDX_x00329;
      8896 00cc9f95 | if ((lVar6 == 0) ||
      8897 00cc9f9d | (bVar3 = IsFalse((CCharString *)&local_44), ppVar22 = extraout_EDX_x00330,
      8898 00cc9fa7 | bVar3)) goto LAB_00cd17fd;
      8899 00cc9fb3 | local_118 = -1.0;
      8900 00cc9fad | CCarriedReadableDef::CCarriedReadableDef(local_85c);
      8901 00cc9fc2 | local_6c = (C3DMeshInfo *)0x0;
      8902 00cc9fc6 | lVar6 = CCharString::GetLength((CCharString *)&local_40);
      8903 00cc9fd1 | if (lVar6 != 0) {
      8904 00cc9fd5 | pcVar9 = CCharString::operator_char_const*((CCharString *)&local_40);
      8905 00cc9fdf | local_118 = GFStringToFloat(pcVar9);
      8906          | }
      8907 00cc9fea | lVar6 = CCharString::GetLength((CCharString *)&local_3c);
      8908 00cc9ff2 | if ((lVar6 != 0) && (bVar3 = IsFalse((CCharString *)&local_3c), !bVar3)) {
      8909 00cca00d | CRegion::SetMiniMapRegionExitTextOffsetX(local_48,local_1188,(float)&local_3c)
      8910          | ;
      8911 00cca01d | iVar33 = *extraout_EAX_13;
      8912 00cca022 | param_4 = *(pair<EHeroMorphType,CParticleMorphs::CEntry> **)local_48;
      8913 00cca02e | local_50 = (double)CONCAT44(iVar33,(EWeaponClass)local_50);
      8914 00cca031 | bVar3 = std::
      8915          | _Tree_const_iterator<std::_Tree_val<std::_Tmap_traits<CCharString,long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,long>_>,0>_>_>
      8916 00cca031 | ::operator!=((_Tree_const_iterator<std::_Tree_val<std::_Tmap_traits<CCharString,long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,long>_>,0>_>_>
      8917 00cca02b | *)((int)&local_50 + 4),
      8918          | (_Tree_const_iterator<class_std::_Tree_val<class_std::_Tmap_traits<class_CCharString,long,struct_std::less<class_CCharString>,class_std::allocator<struct_std::pair<class_CCharString_const_,long>_>,0>_>_>
      8919 00cca027 | *)&param_4);
      8920 00cca038 | if (bVar3) {
      8921 00cca03a | pCVar31 = (C3DMeshInfo *)(iVar33 + 0x14);
      8922 00cca03f | bVar3 = C3DMeshInfo::HasPhysicsMesh(pCVar31);
      8923 00cca046 | if (((bVar3) && (pCVar31 != (C3DMeshInfo *)0x0)) &&
      8924 00cca04e | (bVar3 = C3DMeshInfo::HasPhysicsMesh(pCVar31), bVar3)) {
      8925 00cca057 | pCVar15 = (CScriptThing *)(**(code **)(*(int *)pCVar31 + 0x30))();
      8926 00cca066 | CScriptThing::operator=((CScriptThing *)local_85c,pCVar15);
      8927 00cca071 | C3DClothPrimitive::~C3DClothPrimitive(local_12c0);
      8928 00cca109 | goto LAB_00cca109;
      8929          | }
      8930          | }
      8931 00cca084 | puVar14 = (undefined4 *)CCharString::CCharString(local_b4c,"HERO",-1);
      8932 00cca08f | pCVar1 = (CTCParticleAttacher *)*puVar14;
      8933 00cca091 | if (pCVar1 == local_3c) {
      8934 00cca0b0 | LAB_00cca0b0:
```

Helpers called in window:
- `CCarriedReadableDef` (callsite 00cc9fbd)
- `CCharString` (callsite 00cca08a)
- `GFStringToFloat` (callsite 00cc9fdf)
- `GFStringToInt` (callsite 00cc9f01)
- `GetLength` (callsite 00cc9fed)
- `HasPhysicsMesh` (callsite 00cca04e)
- `IsFalse` (callsite 00cc9ffd)
- `PlayCutsceneSound` (callsite 00cc9097)
- `SetMiniMapRegionExitTextOffsetX` (callsite 00cca018)
- `_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>` (callsite 00cc9f80)
- `operator!=` (callsite 00cca031)
- `operator=` (callsite 00cca06c)
- `operator_char_const*` (callsite 00cc9fd8)
- `strncmp` (callsite 00cc9f6a)
- `~C3DClothPrimitive` (callsite 00cca077)

## DoCameraPreloading (xref 00cc86d0, line 7970, dist 5)

```c
      7958 00cc869e | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      7959 00cc8696 | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_438,
      7960 00cc869e | extraout_EDX_x00224,ppVar48);
      7961 00cc8692 | if (iVar33 == 0) {
      7962 00cc86a7 | lVar6 = CCharString::GetLength((CCharString *)&local_44);
      7963 00cc86b1 | ppVar22 = extraout_EDX_x00225;
      7964 00cc86af | if (lVar6 != 0) {
      7965 00cc86bd | (**(code **)(*DAT_0143e8f8 + 0x870))();
      7966 00cc86c9 | ppVar22 = extraout_EDX_x00226;
      7967          | }
      7968          | }
      7969          | else {
>>>   7970 00cc86d5 | CCharString::CCharString(local_348,"DoCameraPreloading",-1);
      7971 00cc86e0 | sVar13 = CCharString::GetLength(local_348);
      7972 00cc86ec | pcVar9 = CCharString::operator_char_const*(local_348);
      7973 00cc86f8 | pcVar12 = CCharString::operator_char_const*(local_c);
      7974 00cc8701 | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      7975          | std::
      7976 00cc8717 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      7977 00cc870f | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_348,
      7978 00cc8717 | extraout_EDX_x00227,ppVar48);
      7979 00cc870b | if (iVar33 == 0) {
      7980 00cc8726 | (**(code **)(*DAT_0143e8f8 + 0x61c))();
      7981 00cc872e | bVar3 = IsFalse((CCharString *)&local_44);
      7982 00cc8738 | if (bVar3) {
      7983 00cc873d | local_770 = 2.0;
      7984 00cc873a | lVar6 = CCharString::GetLength((CCharString *)&local_40);
      7985 00cc874c | if (lVar6 != 0) {
      7986 00cc8753 | fVar39 = (float10)FUN_0099e690();
      7987 00cc8758 | local_770 = (float)fVar39;
      7988          | }
      7989 00cc876a | (**(code **)(*DAT_0143e8f8 + 0x618))();
      7990          | }
      7991          | else {
      7992 00cc8782 | CutsceneCameraPreloader
      7993 00cc877f | ((vector<CCharString,std::allocator<CCharString>_> *)&local_a4,false,
      7994 00cc8782 | &DAT_013b83c8);
      7995          | }
      7996 00cc878d | (**(code **)(*DAT_0143e8f8 + 0x620))();
      7997 00cc8795 | ppVar22 = extraout_EDX_x00228;
      7998          | }
      7999          | else {
      8000 00cc87a1 | CCharString::CCharString(local_4bc,"StartProgressSpinner",-1);
      8001 00cc87ac | sVar13 = CCharString::GetLength(local_4bc);
      8002 00cc87b8 | pcVar9 = CCharString::operator_char_const*(local_4bc);
      8003 00cc87c4 | pcVar12 = CCharString::operator_char_const*(local_c);
      8004 00cc87cd | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      8005          | std::
      8006 00cc87e3 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8007 00cc87db | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4bc,
      8008 00cc87e3 | extraout_EDX_x00229,ppVar48);
      8009 00cc87ea | ppVar22 = extraout_EDX_x00230;
      8010 00cc87d7 | if (iVar33 != 0) {
      8011 00cc87f7 | CCharString::CCharString(local_350,"StopProgressSpinner",-1);
      8012 00cc8802 | sVar13 = CCharString::GetLength(local_350);
      8013 00cc880e | pcVar9 = CCharString::operator_char_const*(local_350);
      8014 00cc881a | pcVar12 = CCharString::operator_char_const*(local_c);
      8015 00cc8823 | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      8016          | std::
      8017 00cc8839 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8018          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8019 00cc8831 | local_350,extraout_EDX_x00231,ppVar48);
      8020 00cc8840 | ppVar22 = extraout_EDX_x00232;
      8021 00cc882d | if (iVar33 == 0) goto LAB_00cd17fd;
      8022 00cc884d | CCharString::CCharString(local_440,"TakeObjectFromHero",-1);
      8023 00cc8858 | sVar13 = CCharString::GetLength(local_440);
      8024 00cc8864 | pcVar9 = CCharString::operator_char_const*(local_440);
      8025 00cc8870 | pcVar12 = CCharString::operator_char_const*(local_c);
      8026 00cc8879 | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      8027          | std::
      8028 00cc888f | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8029          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8030 00cc8887 | local_440,extraout_EDX_x00233,ppVar48);
      8031 00cc8883 | if (iVar33 == 0) {
      8032 00cc8898 | lVar6 = CCharString::GetLength((CCharString *)&local_44);
      8033 00cc88a2 | ppVar22 = extraout_EDX_x00234;
      8034 00cc88a0 | if ((lVar6 != 0) &&
      8035 00cc88a8 | (lVar6 = CCharString::GetLength((CCharString *)&local_44),
      8036 00cc88b0 | ppVar22 = extraout_EDX_x00235, lVar6 != 0)) {
      8037 00cc88be | (**(code **)(*DAT_0143e8f8 + 500))();
      8038 00cc88ca | ppVar22 = extraout_EDX_x00236;
      8039          | }
      8040 00cd17fd | goto LAB_00cd17fd;
      8041          | }
      8042 00cc88d6 | CCharString::CCharString(local_358,"GamePause",-1);
      8043 00cc88e1 | sVar13 = CCharString::GetLength(local_358);
      8044 00cc88ed | pcVar9 = CCharString::operator_char_const*(local_358);
      8045 00cc88f9 | pcVar12 = CCharString::operator_char_const*(local_c);
      8046 00cc8902 | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      8047          | std::
      8048 00cc8918 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8049          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8050 00cc8910 | local_358,extraout_EDX_x00237,ppVar48);
      8051 00cc890c | if (iVar33 == 0) {
      8052 00cc8925 | lVar6 = CCharString::GetLength((CCharString *)&local_44);
      8053 00cc892f | ppVar22 = extraout_EDX_x00238;
      8054 00cc892d | if (lVar6 != 0) {
      8055 00cc8938 | fVar39 = (float10)FUN_0099e690();
      8056 00cc893d | param_4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)(float)fVar39;
      8057 00cc8949 | CCharString::CCharString(local_6a0,"clock",-1);
      8058 00cc8954 | pcVar9 = CCharString::operator_char_const*(local_6a0);
      8059 00cc8960 | pcVar12 = CCharString::operator_char_const*((CCharString *)&local_40);
      8060 00cc8969 | iVar33 = _stricmp(pcVar12,pcVar9);
```

Helpers called in window:
- `CCharString` (callsite 00cc894f)
- `CutsceneCameraPreloader` (callsite 00cc8782)
- `FUN_0099e690` (callsite 00cc8938)
- `GetLength` (callsite 00cc8928)
- `IsFalse` (callsite 00cc8731)
- `_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>` (callsite 00cc8918)
- `_stricmp` (callsite 00cc8969)
- `operator_char_const*` (callsite 00cc8963)
- `strncmp` (callsite 00cc8902)

## GamePause (xref 00cc88d1, line 8042, dist 5)

```c
      8030 00cc8887 | local_440,extraout_EDX_x00233,ppVar48);
      8031 00cc8883 | if (iVar33 == 0) {
      8032 00cc8898 | lVar6 = CCharString::GetLength((CCharString *)&local_44);
      8033 00cc88a2 | ppVar22 = extraout_EDX_x00234;
      8034 00cc88a0 | if ((lVar6 != 0) &&
      8035 00cc88a8 | (lVar6 = CCharString::GetLength((CCharString *)&local_44),
      8036 00cc88b0 | ppVar22 = extraout_EDX_x00235, lVar6 != 0)) {
      8037 00cc88be | (**(code **)(*DAT_0143e8f8 + 500))();
      8038 00cc88ca | ppVar22 = extraout_EDX_x00236;
      8039          | }
      8040 00cd17fd | goto LAB_00cd17fd;
      8041          | }
>>>   8042 00cc88d6 | CCharString::CCharString(local_358,"GamePause",-1);
      8043 00cc88e1 | sVar13 = CCharString::GetLength(local_358);
      8044 00cc88ed | pcVar9 = CCharString::operator_char_const*(local_358);
      8045 00cc88f9 | pcVar12 = CCharString::operator_char_const*(local_c);
      8046 00cc8902 | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      8047          | std::
      8048 00cc8918 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8049          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8050 00cc8910 | local_358,extraout_EDX_x00237,ppVar48);
      8051 00cc890c | if (iVar33 == 0) {
      8052 00cc8925 | lVar6 = CCharString::GetLength((CCharString *)&local_44);
      8053 00cc892f | ppVar22 = extraout_EDX_x00238;
      8054 00cc892d | if (lVar6 != 0) {
      8055 00cc8938 | fVar39 = (float10)FUN_0099e690();
      8056 00cc893d | param_4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)(float)fVar39;
      8057 00cc8949 | CCharString::CCharString(local_6a0,"clock",-1);
      8058 00cc8954 | pcVar9 = CCharString::operator_char_const*(local_6a0);
      8059 00cc8960 | pcVar12 = CCharString::operator_char_const*((CCharString *)&local_40);
      8060 00cc8969 | iVar33 = _stricmp(pcVar12,pcVar9);
      8061          | std::
      8062 00cc897e | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8063          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8064 00cc8976 | local_6a0,extraout_EDX_x00239,ppVar48);
      8065 00cc8971 | if (iVar33 == 0) {
      8066 00cc8987 | local_50 = GFGetTime();
      8067 00cc898c | local_50 = local_50 + (double)(float)param_4;
      8068          | do {
      8069 00cc89e0 | dVar41 = GFGetTime();
      8070 00cc89ed | ppVar22 = extraout_EDX_x00242;
      8071 00cc899b | if ((local_50 <= dVar41) ||
      8072 00cc8994 | (bVar3 = IsCutsceneSkipped(), ppVar22 = extraout_EDX_x00240, bVar3))
      8073          | break;
      8074 00cc89a1 | if (uStack_8._3_1_ != '\0') {
      8075 00cc89ac | (**(code **)(*DAT_0143e8f8 + 0x1c))();
      8076          | }
      8077 00cc89ba | PlayCutsceneSound((map<int,CCharString,std::less<int>,std::allocator<std::pair<int_const_,CCharString>_>_>
      8078 00cc89b7 | *)local_78,DAT_013b83c8);
      8079 00cc89bf | if (DAT_013b83c8 != -1) {
      8080 00cc89c7 | DAT_013b83c8 = DAT_013b83c8 + 1;
      8081          | }
      8082 00cc89d2 | } while ((DAT_013d2838 == 0) ||
      8083 00cc89d6 | (ppVar22 = extraout_EDX_x00241, *(char *)(DAT_013d2838 + 5) == '\0'));
      8084          | }
      8085          | else {
      8086 00cc89f4 | if (uStack_8._3_1_ != '\0') {
      8087 00cc8a00 | (**(code **)(*DAT_0143e8f8 + 0x1c))();
      8088          | }
      8089 00cc8a0e | PlayCutsceneSound((map<int,CCharString,std::less<int>,std::allocator<std::pair<int_const_,CCharString>_>_>
      8090 00cc8a0b | *)local_78,DAT_013b83c8);
      8091 00cc8a13 | if (DAT_013b83c8 != -1) {
      8092 00cc8a1b | DAT_013b83c8 = DAT_013b83c8 + 1;
      8093          | }
      8094 00cc8a24 | local_60 = (float)param_4 * 15.0;
      8095 00cc8a2f | param_4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
      8096 00cc8a34 | ppVar22 = extraout_EDX_x00243;
      8097 00cc8a3d | while (((float)param_4 < local_60 &&
      8098 00cc8a36 | (bVar3 = IsCutsceneSkipped(), ppVar22 = extraout_EDX_x00244, !bVar3)))
      8099          | {
      8100 00cc8a43 | if (uStack_8._3_1_ != '\0') {
      8101 00cc8a4e | (**(code **)(*DAT_0143e8f8 + 0x1c))();
      8102          | }
      8103 00cc8a5c | PlayCutsceneSound((map<int,CCharString,std::less<int>,std::allocator<std::pair<int_const_,CCharString>_>_>
      8104 00cc8a59 | *)local_78,DAT_013b83c8);
      8105 00cc8a61 | if (DAT_013b83c8 != -1) {
      8106 00cc8a69 | DAT_013b83c8 = DAT_013b83c8 + 1;
      8107          | }
      8108 00cc8a72 | param_4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
      8109 00cc8a72 | ((float)param_4 + 1.0);
      8110 00cc8a78 | ppVar22 = extraout_EDX_x00245;
      8111          | }
      8112          | }
      8113          | }
      8114 00cd17fd | goto LAB_00cd17fd;
      8115          | }
      8116 00cc8a91 | CCharString::CCharString(local_4f4,"SetDoorOpen",-1);
      8117 00cc8a9c | sVar13 = CCharString::GetLength(local_4f4);
      8118 00cc8aa8 | pcVar9 = CCharString::operator_char_const*(local_4f4);
      8119 00cc8ab4 | pcVar12 = CCharString::operator_char_const*(local_c);
      8120 00cc8abd | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      8121          | std::
      8122 00cc8ad3 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8123          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8124 00cc8acb | local_4f4,extraout_EDX_x00246,ppVar48);
      8125 00cc8ac7 | if (iVar33 == 0) {
      8126 00cc8ae0 | lVar6 = CCharString::GetLength((CCharString *)&local_44);
      8127 00cc8aea | ppVar22 = extraout_EDX_x00247;
      8128 00cc8ae8 | if (lVar6 == 0) goto LAB_00cd17fd;
      8129 00cc8af0 | bVar3 = IsFalse((CCharString *)&local_40);
      8130 00cc8af8 | if (!bVar3) {
      8131 00cc8b8a | puVar14 = (undefined4 *)CCharString::CCharString(local_9d8,"HERO",-1);
      8132 00cc8b95 | pCVar1 = (CTCParticleAttacher *)*puVar14;
```

Helpers called in window:
- `CCharString` (callsite 00cc8b90)
- `FUN_0099e690` (callsite 00cc8938)
- `GFGetTime` (callsite 00cc89e0)
- `GetLength` (callsite 00cc8ae3)
- `IsCutsceneSkipped` (callsite 00cc8a36)
- `IsFalse` (callsite 00cc8af3)
- `PlayCutsceneSound` (callsite 00cc8a5c)
- `_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>` (callsite 00cc8ad3)
- `_stricmp` (callsite 00cc8969)
- `operator_char_const*` (callsite 00cc8ab7)
- `strncmp` (callsite 00cc8abd)

## DoScriptFrame (xref 00cc7085, line 2485, dist 4)

```c
      2473 00cc082c | cVar4 = (**(code **)(*(int *)this_02 + 0x68))();
      2474 00cc07e0 | if ((cVar4 == '\0') || (bVar3 = IsCutsceneSkipped(), bVar3)) break;
      2475 00cc07ed | if (uStack_8._3_1_ != '\0') {
      2476 00cc07f8 | (**(code **)(*DAT_0143e8f8 + 0x1c))();
      2477          | }
      2478 00cc0806 | PlayCutsceneSound((map<int,CCharString,std::less<int>,std::allocator<std::pair<int_const_,CCharString>_>_>
      2479 00cc0803 | *)local_78,DAT_013b83c8);
      2480 00cc080b | if (DAT_013b83c8 != -1) {
      2481 00cc0813 | DAT_013b83c8 = DAT_013b83c8 + 1;
      2482          | }
      2483 00cc081e | } while ((DAT_013d2838 == 0) || (*(char *)(DAT_013d2838 + 5) == '\0'));
      2484          | }
>>>   2485 00cc7081 | goto LAB_00cc7081;
      2486          | }
      2487 00cc0842 | CCharString::CCharString(local_4b0,".WalkTo",-1);
      2488 00cc084d | sVar13 = CCharString::GetLength(local_4b0);
      2489 00cc0859 | pcVar9 = CCharString::operator_char_const*(local_4b0);
      2490 00cc0865 | pcVar12 = CCharString::operator_char_const*(local_c);
      2491 00cc086e | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      2492 00cc0882 | param_4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
      2493 00cc0876 | CONCAT13('\x01' - (iVar33 != 0),param_4._0_3_);
      2494          | std::
      2495 00cc0885 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      2496 00cc087c | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4b0,
      2497 00cc0885 | extraout_EDX_19,ppVar48);
      2498 00cc088a | if (param_4._3_1_ == 0) {
      2499 00cc0a7e | CCharString::CCharString(local_4b8,".RunTo",-1);
      2500 00cc0a89 | sVar13 = CCharString::GetLength(local_4b8);
      2501 00cc0a95 | pcVar9 = CCharString::operator_char_const*(local_4b8);
      2502 00cc0aa1 | pcVar12 = CCharString::operator_char_const*(local_c);
      2503 00cc0aaa | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      2504 00cc0abe | param_4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
      2505 00cc0ab2 | CONCAT13('\x01' - (iVar33 != 0),param_4._0_3_);
      2506          | std::
      2507 00cc0ac1 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      2508 00cc0ab8 | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4b8,
      2509 00cc0ac1 | extraout_EDX_22,ppVar48);
      2510 00cc0ac6 | if (param_4._3_1_ == 0) {
      2511 00cc0cba | CCharString::CCharString(local_4c0,".SneakTo",-1);
      2512 00cc0cc5 | sVar13 = CCharString::GetLength(local_4c0);
      2513 00cc0cd1 | pcVar9 = CCharString::operator_char_const*(local_4c0);
      2514 00cc0cdd | pcVar12 = CCharString::operator_char_const*(local_c);
      2515 00cc0ce6 | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      2516 00cc0cfa | param_4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
      2517 00cc0cee | CONCAT13('\x01' - (iVar33 != 0),param_4._0_3_);
      2518          | std::
      2519 00cc0cfd | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      2520 00cc0cf4 | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4c0,
      2521 00cc0cfd | extraout_EDX_25,ppVar48);
      2522 00cc0d02 | if (param_4._3_1_ != 0) {
      2523 00cc0d0c | if ((this_02 == (CMemoryDataOutputStream *)0x0) ||
      2524 00cc0d14 | (lVar6 = CCharString::GetLength((CCharString *)&local_44), lVar6 == 0))
      2525 00cc7081 | goto LAB_00cc7081;
      2526 00cc0d27 | local_6ec = 0.3;
      2527 00cc0d31 | param_4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((uint)param_4 & 0xffffff);
      2528 00cc0d35 | local_6e4 = CONCAT31(local_6e4._1_3_,1);
      2529 00cc0d3c | local_6dc = local_6dc & 0xffffff00;
      2530 00cc0d43 | local_6d4 = CONCAT31(local_6d4._1_3_,1);
      2531 00cc0d24 | lVar6 = CCharString::GetLength((CCharString *)&local_40);
      2532 00cc0d4f | if (lVar6 != 0) {
      2533 00cc0d56 | fVar39 = (float10)FUN_0099e690();
      2534 00cc0d5b | local_6ec = (float)fVar39;
      2535          | }
      2536 00cc0d61 | bVar3 = IsFalse((CCharString *)&local_38);
      2537 00cc0d6b | if (bVar3) {
      2538 00cc0d6d | param_4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)CONCAT13(1,param_4._0_3_);
      2539          | }
      2540 00cc0d71 | bVar3 = IsFalse((CCharString *)&local_34);
      2541 00cc0d7b | if (bVar3) {
      2542 00cc0d7d | local_6e4 = local_6e4 & 0xffffff00;
      2543          | }
      2544 00cc0d84 | bVar3 = IsFalse((CCharString *)&local_30);
      2545 00cc0d8e | if (bVar3) {
      2546 00cc0d90 | local_6dc = CONCAT31(local_6dc._1_3_,1);
      2547          | }
      2548 00cc0d97 | bVar3 = IsFalse(local_2c);
      2549 00cc0da1 | if (bVar3) {
      2550 00cc0da3 | local_6d4 = local_6d4 & 0xffffff00;
      2551          | }
      2552 00cc0daf | iVar33 = *DAT_0143e8f8;
      2553 00cc0db1 | (**(code **)(*(int *)this_02 + 0x30))();
      2554 00cc0dc8 | (**(code **)(iVar33 + 0x800))();
      2555 00cc0dce | C3DClothPrimitive::~C3DClothPrimitive(local_17d0);
      2556 00cc0ddc | puVar14 = (undefined4 *)CCharString::CCharString(local_ac8,"HERO",-1);
      2557 00cc0de7 | pCVar1 = (CTCParticleAttacher *)*puVar14;
      2558 00cc0de9 | if (pCVar1 == local_44) {
      2559 00cc0e08 | LAB_00cc0e08:
      2560 00cc0e0e | uVar42 = (**(code **)(*DAT_0143e8f8 + 0x118))();
      2561          | }
      2562          | else {
      2563 00cc0dee | if ((pCVar1 == (CTCParticleAttacher *)0x0) ||
      2564 00cc0df2 | (local_44 == (CTCParticleAttacher *)0x0)) {
      2565 00cc0e02 | cVar4 = '\0';
      2566          | }
      2567          | else {
      2568 00cc0dfb | CTCParticleAttacher::OnInitialActivate(pCVar1);
      2569 00cc0e00 | cVar4 = extraout_AL_02;
      2570          | }
      2571 00cc0e04 | if (cVar4 != '\0') goto LAB_00cc0e08;
      2572 00cc0e20 | local_10 = local_10 | 0x10;
      2573 00cc0e1e | uVar42 = (**(code **)(*DAT_0143e8f8 + 0x120))();
      2574          | }
      2575 00cc0e41 | (**(code **)(*(int *)this_02 + 0x14))(uVar42,local_6ec,2,0,local_6e4);
```

Helpers called in window:
- `CCharString` (callsite 00cc0de2)
- `FUN_0099e690` (callsite 00cc0d56)
- `GetLength` (callsite 00cc0d4a)
- `IsCutsceneSkipped` (callsite 00cc07e0)
- `IsFalse` (callsite 00cc0d9a)
- `OnInitialActivate` (callsite 00cc0dfb)
- `PlayCutsceneSound` (callsite 00cc0806)
- `_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>` (callsite 00cc0cfd)
- `operator_char_const*` (callsite 00cc0ce0)
- `strncmp` (callsite 00cc0ce6)
- `~C3DClothPrimitive` (callsite 00cc0dd4)

## Create (xref 00ccc246, line 10324, dist 5)

```c
     10312 00ccc201 | param_4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)(float)fVar39;
     10313 00ccc204 | bVar3 = CScriptThing::?IsAlive@CScriptThing@@UBE_NXZ(local_ea4);
     10314 00ccc211 | if (bVar3) {
     10315 00ccc21b | iVar33 = *DAT_0143e8f8;
     10316 00ccc21e | CScriptThing::?GetPos@CScriptThing@@UBEABVC3DVector@@XZ(local_ea4);
     10317 00ccc233 | (**(code **)(iVar33 + 0xbcc))();
     10318          | }
     10319 00ccc239 | pCVar26 = (C3DClothPrimitive *)local_ea4;
     10320 00cc864b | goto LAB_00cc864b;
     10321          | }
     10322 00cd17fd | goto LAB_00cd17fd;
     10323          | }
>>>  10324 00ccc24b | CCharString::CCharString(local_2b4,"Create",-1);
     10325 00ccc256 | sVar13 = CCharString::GetLength(local_2b4);
     10326 00ccc262 | pcVar9 = CCharString::operator_char_const*(local_2b4);
     10327 00ccc26e | pcVar12 = CCharString::operator_char_const*(local_c);
     10328 00ccc277 | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
     10329          | std::
     10330 00ccc28d | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     10331          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
     10332 00ccc285 | local_2b4,extraout_EDX_x00443,ppVar48);
     10333 00ccc281 | if (iVar33 != 0) {
     10334 00ccc500 | CCharString::CCharString(local_2c4,"ObjectCreate",-1);
     10335 00ccc50b | sVar13 = CCharString::GetLength(local_2c4);
     10336 00ccc517 | pcVar9 = CCharString::operator_char_const*(local_2c4);
     10337 00ccc523 | pcVar12 = CCharString::operator_char_const*(local_c);
     10338 00ccc52c | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
     10339          | std::
     10340 00ccc542 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     10341          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
     10342 00ccc53a | local_2c4,extraout_EDX_x00451,ppVar48);
     10343 00ccc536 | if (iVar33 == 0) {
     10344 00ccc54f | lVar6 = CCharString::GetLength((CCharString *)&local_44);
     10345 00ccc559 | ppVar22 = extraout_EDX_x00452;
     10346 00ccc557 | if (((lVar6 == 0) ||
     10347 00ccc55f | (lVar6 = CCharString::GetLength((CCharString *)&local_40),
     10348 00ccc567 | ppVar22 = extraout_EDX_x00453, lVar6 == 0)) ||
     10349 00ccc56f | (lVar6 = CCharString::GetLength((CCharString *)&local_3c),
     10350 00ccc577 | ppVar22 = extraout_EDX_x00454, lVar6 == 0)) goto LAB_00cd17fd;
     10351 00ccc582 | puVar14 = (undefined4 *)CCharString::CCharString(local_954,"HERO",-1);
     10352 00ccc58d | pCVar1 = (CTCParticleAttacher *)*puVar14;
     10353 00ccc58f | if (pCVar1 == local_40) {
     10354 00ccc5ad | LAB_00ccc5ad:
     10355 00ccc5b3 | pCVar15 = (CScriptThing *)(**(code **)(*DAT_0143e8f8 + 0x118))();
     10356          | }
     10357          | else {
     10358 00ccc594 | if ((pCVar1 == (CTCParticleAttacher *)0x0) ||
     10359 00ccc598 | (local_40 == (CTCParticleAttacher *)0x0)) {
     10360 00ccc5a7 | cVar4 = '\0';
     10361          | }
     10362          | else {
     10363 00ccc5a0 | CTCParticleAttacher::OnInitialActivate(pCVar1);
     10364 00ccc5a5 | cVar4 = extraout_AL_53;
     10365          | }
     10366 00ccc5a9 | if (cVar4 != '\0') goto LAB_00ccc5ad;
     10367 00ccc5c5 | local_14 = local_14 | 0x4000000;
     10368 00ccc5c3 | pCVar15 = (CScriptThing *)(**(code **)(*DAT_0143e8f8 + 0x120))();
     10369          | }
     10370 00ccc5db | CScriptThing::CScriptThing(local_115c,pCVar15);
     10371 00ccc5ea | ppVar22 = extraout_EDX_x00455;
     10372 00ccc5e6 | if ((local_14 & 0x4000000) != 0) {
     10373 00ccc5ec | local_14 = local_14 & 0xfbffffff;
     10374 00ccc5f0 | C3DClothPrimitive::~C3DClothPrimitive(local_14d0);
     10375 00ccc5f6 | ppVar22 = extraout_EDX_x00456;
     10376          | }
     10377          | std::
     10378 00ccc601 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     10379          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
     10380 00ccc5fb | local_954,ppVar22,ppVar48);
     10381 00ccc60b | iVar33 = *DAT_0143e8f8;
     10382 00ccc611 | CScriptThing::?GetPos@CScriptThing@@UBEABVC3DVector@@XZ(local_115c);
     10383 00ccc62e | (**(code **)(iVar33 + 0x188))();
     10384 00ccc634 | C3DClothPrimitive::~C3DClothPrimitive(local_14e8);
     10385 00ccc63f | pCVar26 = (C3DClothPrimitive *)local_115c;
     10386 00cc864b | goto LAB_00cc864b;
     10387          | }
     10388 00ccc651 | CCharString::CCharString(local_2cc,"CrowdCreateMixed",-1);
     10389 00ccc65c | sVar13 = CCharString::GetLength(local_2cc);
     10390 00ccc668 | pcVar9 = CCharString::operator_char_const*(local_2cc);
     10391 00ccc674 | pcVar12 = CCharString::operator_char_const*(local_c);
     10392 00ccc67d | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
     10393          | std::
     10394 00ccc693 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     10395          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
     10396 00ccc68b | local_2cc,extraout_EDX_x00457,ppVar48);
     10397 00ccc687 | if (iVar33 == 0) {
     10398 00ccc6a0 | lVar6 = CCharString::GetLength((CCharString *)&local_44);
     10399 00ccc6aa | ppVar22 = extraout_EDX_x00458;
     10400 00ccc6a8 | if ((((lVar6 != 0) &&
     10401 00ccc6b0 | (lVar6 = CCharString::GetLength((CCharString *)&local_40),
     10402 00ccc6b8 | ppVar22 = extraout_EDX_x00459, lVar6 != 0)) &&
     10403 00ccc6c0 | (lVar6 = CCharString::GetLength((CCharString *)&local_3c),
     10404 00ccc6c8 | ppVar22 = extraout_EDX_x00460, lVar6 != 0)) &&
     10405 00ccc6d0 | (lVar6 = CCharString::GetLength((CCharString *)&local_38),
     10406 00ccc6d8 | ppVar22 = extraout_EDX_x00461, lVar6 != 0)) {
     10407 00ccc6f0 | local_584 = 0;
     10408 00ccc6f6 | local_580 = 0;
     10409 00ccc6fc | local_57c = 0;
     10410 00ccc702 | (**(code **)(*DAT_0143e8f8 + 300))();
     10411 00ccc71b | ppVar22 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
     10412 00ccc711 | ((local_580 - local_584) / 0xc);
     10413 00ccc723 | local_8c8 = 0;
     10414 00ccc729 | local_8c4 = 0;
```

Helpers called in window:
- `?GetPos@CScriptThing@@UBEABVC3DVector@@XZ` (callsite 00ccc617)
- `?IsAlive@CScriptThing@@UBE_NXZ` (callsite 00ccc20a)
- `CCharString` (callsite 00ccc657)
- `CScriptThing` (callsite 00ccc5e1)
- `GetLength` (callsite 00ccc6d3)
- `OnInitialActivate` (callsite 00ccc5a0)
- `_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>` (callsite 00ccc693)
- `operator_char_const*` (callsite 00ccc677)
- `strncmp` (callsite 00ccc67d)
- `~C3DClothPrimitive` (callsite 00ccc63a)

## CreateEffect (xref 00ccbb9a, line 10088, dist 5)

```c
     10076 00ccbb31 | local_149 = 0xff;
     10077 00ccbb3b | iVar33 = *DAT_0143e8f8;
     10078 00ccbb48 | EVar19 = CScriptThing::?GetPos@CScriptThing@@UBEABVC3DVector@@XZ(local_112c);
     10079 00ccbb5a | (**(code **)(iVar33 + 0x198))(local_1114,EVar19);
     10080 00ccbb67 | bVar3 = IsFalse(local_24);
     10081 00ccbb71 | if (bVar3) {
     10082 00ccbb7d | FUN_008adf90();
     10083          | }
     10084 00ccbb82 | C3DClothPrimitive::~C3DClothPrimitive(local_1114);
     10085 00ccbb8d | pCVar26 = (C3DClothPrimitive *)local_112c;
     10086 00cc864b | goto LAB_00cc864b;
     10087          | }
>>>  10088 00ccbb9f | CCharString::CCharString(local_28c,"CreateEffect",-1);
     10089 00ccbbaa | sVar13 = CCharString::GetLength(local_28c);
     10090 00ccbbb6 | pcVar9 = CCharString::operator_char_const*(local_28c);
     10091 00ccbbc2 | pcVar12 = CCharString::operator_char_const*(local_c);
     10092 00ccbbcb | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
     10093          | std::
     10094 00ccbbe1 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     10095          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
     10096 00ccbbd9 | local_28c,extraout_EDX_x00422,ppVar48);
     10097 00ccbbd5 | if (iVar33 == 0) {
     10098 00ccbbee | lVar6 = CCharString::GetLength((CCharString *)&local_44);
     10099 00ccbbf8 | ppVar22 = extraout_EDX_x00423;
     10100 00ccbbf6 | if ((lVar6 == 0) ||
     10101 00ccbbfe | (lVar6 = CCharString::GetLength((CCharString *)&local_40),
     10102 00ccbc06 | ppVar22 = extraout_EDX_x00424, lVar6 == 0)) goto LAB_00cd17fd;
     10103 00ccbc1a | FUN_00cbf9de();
     10104 00ccbc1f | bVar3 = CScriptThing::?IsAlive@CScriptThing@@UBE_NXZ(local_b94);
     10105 00ccbc2c | if (bVar3) {
     10106 00ccbc3b | param_4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
     10107 00ccbc3e | CCharString::CCharString(local_154,"",-1);
     10108 00ccbc49 | lVar6 = CCharString::GetLength((CCharString *)&local_3c);
     10109 00ccbc51 | if (lVar6 != 0) {
     10110 00ccbc55 | CCharString::operator=(local_154,(CCharString *)&local_3c);
     10111          | }
     10112 00ccbc64 | lVar6 = CCharString::GetLength((CCharString *)&local_38);
     10113 00ccbc6c | if (lVar6 != 0) {
     10114 00ccbc73 | fVar39 = (float10)FUN_0099e690();
     10115 00ccbc78 | param_4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)(float)fVar39;
     10116          | }
     10117 00ccbc81 | puVar14 = (undefined4 *)
     10118 00ccbc7b | CScriptThing::?GetPos@CScriptThing@@UBEABVC3DVector@@XZ(local_b94);
     10119 00ccbc89 | local_e3c = (float)param_4 + (float)puVar14[2];
     10120 00ccbc8d | local_e40 = puVar14[1];
     10121 00ccbc90 | local_e44 = *puVar14;
     10122 00ccbca9 | iVar33 = *DAT_0143e8f8;
     10123 00ccbcac | CScriptThing::?GetAngleXY@CScriptThing@@UBEMXZ(local_b94);
     10124 00ccbccf | (**(code **)(iVar33 + 400))(local_a80,&local_44);
     10125 00ccbce0 | bVar3 = CScriptThing::?IsAlive@CScriptThing@@UBE_NXZ(local_a80);
     10126 00ccbced | if (bVar3) {
     10127 00ccbcf5 | (**(code **)(*DAT_0143e8f8 + 0x800))();
     10128 00ccbd06 | bVar3 = IsFalse((CCharString *)&local_34);
     10129 00ccbd10 | if ((bVar3) ||
     10130 00ccbd12 | (lVar6 = CCharString::GetLength((CCharString *)&local_34), lVar6 == 0)) {
     10131          | CScriptGameResourceObjectScriptedThingBase::
     10132 00ccbd2b | ?SetDataString@CScriptThing@@UAEXABVCCharString@@@Z
     10133 00ccbd25 | ((CScriptGameResourceObjectScriptedThingBase *)local_a80,
     10134 00ccbd1e | (CScriptThing *)local_154,(char *)ppVar48,pCVar5,pCVar49,
     10135 00ccbd2b | (CCharString *)ppVar50,SUB41(pCVar51,0),SUB41(uVar52,0));
     10136 00ccbd3a | FUN_008adf90();
     10137          | }
     10138          | }
     10139 00ccbd3f | C3DClothPrimitive::~C3DClothPrimitive((C3DClothPrimitive *)local_a80);
     10140          | std::
     10141 00ccbd50 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     10142          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
     10143 00ccbd4a | local_154,extraout_EDX_x00425,ppVar48);
     10144          | }
     10145 00ccbd55 | pCVar26 = (C3DClothPrimitive *)local_b94;
     10146 00cc864b | goto LAB_00cc864b;
     10147          | }
     10148 00ccbd67 | CCharString::CCharString(local_294,"DummyEffect",-1);
     10149 00ccbd72 | sVar13 = CCharString::GetLength(local_294);
     10150 00ccbd7e | pcVar9 = CCharString::operator_char_const*(local_294);
     10151 00ccbd8a | pcVar12 = CCharString::operator_char_const*(local_c);
     10152 00ccbd93 | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
     10153          | std::
     10154 00ccbda9 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     10155          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
     10156 00ccbda1 | local_294,extraout_EDX_x00426,ppVar48);
     10157 00ccbd9d | if (iVar33 == 0) {
     10158 00ccbdb6 | lVar6 = CCharString::GetLength((CCharString *)&local_44);
     10159 00ccbdc0 | ppVar22 = extraout_EDX_x00427;
     10160 00ccbdbe | if (((lVar6 != 0) &&
     10161 00ccbdc6 | (lVar6 = CCharString::GetLength((CCharString *)&local_40),
     10162 00ccbdce | ppVar22 = extraout_EDX_x00428, lVar6 != 0)) &&
     10163 00ccbdd6 | (lVar6 = CCharString::GetLength((CCharString *)&local_3c),
     10164 00ccbdde | ppVar22 = extraout_EDX_x00429, lVar6 != 0)) {
     10165 00ccbdf2 | FUN_00cbf9de();
     10166 00ccbdf7 | bVar3 = CScriptThing::?IsAlive@CScriptThing@@UBE_NXZ(local_e5c);
     10167 00ccbe04 | if (bVar3) {
     10168 00ccbe11 | CCharString::CCharString(local_15c,"",-1);
     10169 00ccbe1c | lVar6 = CCharString::GetLength((CCharString *)&local_38);
     10170 00ccbe24 | if (lVar6 != 0) {
     10171 00ccbe28 | CCharString::operator=(local_15c,(CCharString *)&local_38);
     10172          | }
     10173 00ccbe3d | (**(code **)(*DAT_0143e8f8 + 0x194))(local_a90,&local_44);
     10174 00ccbe65 | bVar3 = CScriptThing::?IsAlive@CScriptThing@@UBE_NXZ(local_a90);
     10175 00ccbe72 | if (bVar3) {
     10176 00ccbe7a | (**(code **)(*DAT_0143e8f8 + 0x800))();
     10177 00ccbe8b | bVar3 = IsFalse((CCharString *)&local_34);
     10178 00ccbe95 | if ((bVar3) ||
```

Helpers called in window:
- `?GetAngleXY@CScriptThing@@UBEMXZ` (callsite 00ccbcb2)
- `?GetPos@CScriptThing@@UBEABVC3DVector@@XZ` (callsite 00ccbc81)
- `?IsAlive@CScriptThing@@UBE_NXZ` (callsite 00ccbe6b)
- `?SetDataString@CScriptThing@@UAEXABVCCharString@@@Z` (callsite 00ccbd2b)
- `CCharString` (callsite 00ccbe17)
- `FUN_008adf90` (callsite 00ccbd3a)
- `FUN_0099e690` (callsite 00ccbc73)
- `FUN_00cbf9de` (callsite 00ccbdf2)
- `GetLength` (callsite 00ccbe1f)
- `IsFalse` (callsite 00ccbe8e)
- `_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>` (callsite 00ccbda9)
- `operator=` (callsite 00ccbe32)
- `operator_char_const*` (callsite 00ccbd8d)
- `strncmp` (callsite 00ccbd93)
- `~C3DClothPrimitive` (callsite 00ccbd45)

## FadeOut (xref 00cd098b, line 13261, dist 5)

```c
     13249          | ;
     13250 00cd0955 | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
     13251          | std::
     13252 00cd096b | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     13253          | ((
     13254          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
     13255 00cd0963 | *)local_414,extraout_EDX_x00695,ppVar48);
     13256 00cd095f | if (iVar33 == 0) {
     13257 00cd097a | (**(code **)(*DAT_0143e8f8 + 0x5d8))();
     13258 00cd0982 | ppVar22 = extraout_EDX_x00696;
     13259          | }
     13260          | else {
>>>  13261 00cd0990 | CCharString::CCharString(local_41c,"FadeOut",-1);
     13262 00cd099b | sVar13 = CCharString::GetLength(local_41c);
     13263 00cd09ad | pcVar9 = CCharString::operator_char_const*
     13264 00cd09a7 | (local_41c);
     13265 00cd09b6 | pcVar12 = CCharString::operator_char_const*
     13266 00cd09b3 | (local_c);
     13267 00cd09bc | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
     13268          | std::
     13269 00cd09d2 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     13270          | ((
     13271          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
     13272 00cd09ca | *)local_41c,extraout_EDX_x00697,ppVar48);
     13273 00cd09c6 | if (iVar33 == 0) {
     13274 00cd09e5 | cVar4 = (**(code **)(*DAT_0143e8f8 + 0x5dc))();
     13275 00cd09ef | ppVar22 = extraout_EDX_x00698;
     13276 00cd09ed | if (cVar4 == '\0') {
     13277 00cd0a2a | LAB_00cd0a2a:
     13278 00cd0a2a | bVar3 = true;
     13279          | }
     13280          | else {
     13281 00cd09fd | CCharString::CCharString
     13282 00cd09f7 | (local_664,"override",-1);
     13283 00cd0a02 | local_68 = local_68 | 1;
     13284 00cd0a0c | pcVar9 = CCharString::operator_char_const*
     13285 00cd0a06 | (local_664);
     13286 00cd0a15 | pcVar12 = CCharString::operator_char_const*
     13287 00cd0a12 | ((CCharString *)&local_38)
     13288          | ;
     13289 00cd0a1b | iVar33 = _stricmp(pcVar12,pcVar9);
     13290 00cd0a24 | ppVar22 = extraout_EDX_x00699;
     13291 00cd0a20 | if (iVar33 == 0) goto LAB_00cd0a2a;
     13292 00cd0a26 | bVar3 = false;
     13293          | }
     13294 00cd0a2c | if ((local_68 & 1) != 0) {
     13295 00cd0a32 | local_68 = local_68 & 0xfffffffe;
     13296          | std::
     13297 00cd0a3c | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     13298          | ((
     13299          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
     13300 00cd0a36 | *)local_664,ppVar22,ppVar48);
     13301 00cd0a3c | ppVar22 = extraout_EDX_x00700;
     13302          | }
     13303 00cd0a43 | if (bVar3) {
     13304 00cd0a52 | local_60 = 0.5;
     13305 00cd0a59 | param_4 = (
     13306          | pair<EHeroMorphType,CParticleMorphs::CEntry> *)
     13307 00cd0a59 | &DAT_3f000000;
     13308 00cd0a64 | local_b0 = 0xff000000;
     13309 00cd0a68 | lVar6 = CCharString::GetLength
     13310 00cd0a4f | ((CCharString *)&local_44);
     13311 00cd0a6d | if (lVar6 != 0) {
     13312 00cd0a74 | fVar39 = (float10)FUN_0099e690();
     13313 00cd0a79 | local_60 = (float)fVar39;
     13314          | }
     13315 00cd0a7f | lVar6 = CCharString::GetLength
     13316 00cd0a7c | ((CCharString *)&local_40);
     13317 00cd0a84 | if (lVar6 != 0) {
     13318 00cd0a8b | fVar39 = (float10)FUN_0099e690();
     13319 00cd0a90 | param_4 = (
     13320          | pair<EHeroMorphType,CParticleMorphs::CEntry> *)
     13321 00cd0a90 | (float)fVar39;
     13322          | }
     13323 00cd0a96 | lVar6 = CCharString::GetLength
     13324 00cd0a93 | ((CCharString *)&local_3c);
     13325 00cd0a9d | ppVar22 = extraout_EDX_x00701;
     13326 00cd0a9b | if (lVar6 == 0) {
     13327 00cd0ad8 | LAB_00cd0ad8:
     13328 00cd0ad8 | bVar3 = true;
     13329          | }
     13330          | else {
     13331 00cd0aa5 | CCharString::CCharString(local_66c,"black",-1);
     13332 00cd0ab0 | local_68 = local_68 | 2;
     13333 00cd0aba | pcVar9 = CCharString::operator_char_const*
     13334 00cd0ab4 | (local_66c);
     13335 00cd0ac3 | pcVar12 = CCharString::operator_char_const*
     13336 00cd0ac0 | ((CCharString *)&local_3c);
     13337 00cd0ac9 | iVar33 = _stricmp(pcVar12,pcVar9);
     13338 00cd0ad2 | ppVar22 = extraout_EDX_x00702;
     13339 00cd0ace | if (iVar33 == 0) goto LAB_00cd0ad8;
     13340 00cd0ad4 | bVar3 = false;
     13341          | }
     13342 00cd0ada | if ((local_68 & 2) != 0) {
     13343 00cd0ae0 | local_68 = local_68 & 0xfffffffd;
     13344          | std::
     13345 00cd0aea | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     13346          | ((
     13347          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
     13348 00cd0ae4 | *)local_66c,ppVar22,ppVar48);
     13349          | }
     13350 00cd0af1 | if (bVar3) {
     13351 00cd0afc | (**(code **)(*DAT_0143e8f8 + 0x5d0))();
```

Helpers called in window:
- `CCharString` (callsite 00cd0aab)
- `FUN_0099e690` (callsite 00cd0a8b)
- `GetLength` (callsite 00cd0a96)
- `_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>` (callsite 00cd0aea)
- `_stricmp` (callsite 00cd0ac9)
- `operator_char_const*` (callsite 00cd0ac3)
- `strncmp` (callsite 00cd09bc)

## FadeIn (xref 00cd0924, line 13244, dist 5)

```c
     13232 00cd08f1 | pcVar12 = CCharString::operator_char_const*(local_c);
     13233 00cd08fa | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
     13234          | std::
     13235 00cd0910 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     13236          | ((
     13237          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
     13238 00cd0908 | *)local_40c,extraout_EDX_x00693,ppVar48);
     13239 00cd0904 | if (iVar33 == 0) {
     13240 00cd0919 | local_a8 = '\0';
     13241 00cd091d | ppVar22 = extraout_EDX_x00694;
     13242          | }
     13243          | else {
>>>  13244 00cd0929 | CCharString::CCharString(local_414,"FadeIn",-1);
     13245 00cd0934 | sVar13 = CCharString::GetLength(local_414);
     13246 00cd0946 | pcVar9 = CCharString::operator_char_const*
     13247 00cd0940 | (local_414);
     13248 00cd094c | pcVar12 = CCharString::operator_char_const*(local_c)
     13249          | ;
     13250 00cd0955 | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
     13251          | std::
     13252 00cd096b | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     13253          | ((
     13254          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
     13255 00cd0963 | *)local_414,extraout_EDX_x00695,ppVar48);
     13256 00cd095f | if (iVar33 == 0) {
     13257 00cd097a | (**(code **)(*DAT_0143e8f8 + 0x5d8))();
     13258 00cd0982 | ppVar22 = extraout_EDX_x00696;
     13259          | }
     13260          | else {
     13261 00cd0990 | CCharString::CCharString(local_41c,"FadeOut",-1);
     13262 00cd099b | sVar13 = CCharString::GetLength(local_41c);
     13263 00cd09ad | pcVar9 = CCharString::operator_char_const*
     13264 00cd09a7 | (local_41c);
     13265 00cd09b6 | pcVar12 = CCharString::operator_char_const*
     13266 00cd09b3 | (local_c);
     13267 00cd09bc | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
     13268          | std::
     13269 00cd09d2 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     13270          | ((
     13271          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
     13272 00cd09ca | *)local_41c,extraout_EDX_x00697,ppVar48);
     13273 00cd09c6 | if (iVar33 == 0) {
     13274 00cd09e5 | cVar4 = (**(code **)(*DAT_0143e8f8 + 0x5dc))();
     13275 00cd09ef | ppVar22 = extraout_EDX_x00698;
     13276 00cd09ed | if (cVar4 == '\0') {
     13277 00cd0a2a | LAB_00cd0a2a:
     13278 00cd0a2a | bVar3 = true;
     13279          | }
     13280          | else {
     13281 00cd09fd | CCharString::CCharString
     13282 00cd09f7 | (local_664,"override",-1);
     13283 00cd0a02 | local_68 = local_68 | 1;
     13284 00cd0a0c | pcVar9 = CCharString::operator_char_const*
     13285 00cd0a06 | (local_664);
     13286 00cd0a15 | pcVar12 = CCharString::operator_char_const*
     13287 00cd0a12 | ((CCharString *)&local_38)
     13288          | ;
     13289 00cd0a1b | iVar33 = _stricmp(pcVar12,pcVar9);
     13290 00cd0a24 | ppVar22 = extraout_EDX_x00699;
     13291 00cd0a20 | if (iVar33 == 0) goto LAB_00cd0a2a;
     13292 00cd0a26 | bVar3 = false;
     13293          | }
     13294 00cd0a2c | if ((local_68 & 1) != 0) {
     13295 00cd0a32 | local_68 = local_68 & 0xfffffffe;
     13296          | std::
     13297 00cd0a3c | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
     13298          | ((
     13299          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
     13300 00cd0a36 | *)local_664,ppVar22,ppVar48);
     13301 00cd0a3c | ppVar22 = extraout_EDX_x00700;
     13302          | }
     13303 00cd0a43 | if (bVar3) {
     13304 00cd0a52 | local_60 = 0.5;
     13305 00cd0a59 | param_4 = (
     13306          | pair<EHeroMorphType,CParticleMorphs::CEntry> *)
     13307 00cd0a59 | &DAT_3f000000;
     13308 00cd0a64 | local_b0 = 0xff000000;
     13309 00cd0a68 | lVar6 = CCharString::GetLength
     13310 00cd0a4f | ((CCharString *)&local_44);
     13311 00cd0a6d | if (lVar6 != 0) {
     13312 00cd0a74 | fVar39 = (float10)FUN_0099e690();
     13313 00cd0a79 | local_60 = (float)fVar39;
     13314          | }
     13315 00cd0a7f | lVar6 = CCharString::GetLength
     13316 00cd0a7c | ((CCharString *)&local_40);
     13317 00cd0a84 | if (lVar6 != 0) {
     13318 00cd0a8b | fVar39 = (float10)FUN_0099e690();
     13319 00cd0a90 | param_4 = (
     13320          | pair<EHeroMorphType,CParticleMorphs::CEntry> *)
     13321 00cd0a90 | (float)fVar39;
     13322          | }
     13323 00cd0a96 | lVar6 = CCharString::GetLength
     13324 00cd0a93 | ((CCharString *)&local_3c);
     13325 00cd0a9d | ppVar22 = extraout_EDX_x00701;
     13326 00cd0a9b | if (lVar6 == 0) {
     13327 00cd0ad8 | LAB_00cd0ad8:
     13328 00cd0ad8 | bVar3 = true;
     13329          | }
     13330          | else {
     13331 00cd0aa5 | CCharString::CCharString(local_66c,"black",-1);
     13332 00cd0ab0 | local_68 = local_68 | 2;
     13333 00cd0aba | pcVar9 = CCharString::operator_char_const*
     13334 00cd0ab4 | (local_66c);
```

Helpers called in window:
- `CCharString` (callsite 00cd0aab)
- `FUN_0099e690` (callsite 00cd0a8b)
- `GetLength` (callsite 00cd0a96)
- `_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>` (callsite 00cd0a3c)
- `_stricmp` (callsite 00cd0a1b)
- `operator_char_const*` (callsite 00cd0aba)
- `strncmp` (callsite 00cd09bc)

## PutUpYourSwords (xref 00cc9302, line 8472, dist 5)

```c
      8460 00cc92b9 | pcVar9 = CCharString::operator_char_const*(local_458);
      8461 00cc92c5 | pcVar12 = CCharString::operator_char_const*(local_c);
      8462 00cc92ce | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      8463          | std::
      8464 00cc92e4 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8465          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8466 00cc92dc | local_458,extraout_EDX_x00283,ppVar48);
      8467 00cc92d8 | if (iVar33 == 0) {
      8468 00cc92f3 | (**(code **)(*DAT_0143e8f8 + 0x2f4))();
      8469 00cc92fb | ppVar22 = extraout_EDX_x00284;
      8470 00cd17fd | goto LAB_00cd17fd;
      8471          | }
>>>   8472 00cc9307 | CCharString::CCharString(local_388,"PutUpYourSwords",-1);
      8473 00cc9312 | sVar13 = CCharString::GetLength(local_388);
      8474 00cc931e | pcVar9 = CCharString::operator_char_const*(local_388);
      8475 00cc932a | pcVar12 = CCharString::operator_char_const*(local_c);
      8476 00cc9333 | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      8477          | std::
      8478 00cc9349 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8479          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8480 00cc9341 | local_388,extraout_EDX_x00285,ppVar48);
      8481 00cc933d | if (iVar33 == 0) {
      8482 00cc9356 | bVar3 = IsFalse((CCharString *)&local_44);
      8483 00cc9360 | if (bVar3) {
      8484 00cc9368 | pCVar15 = (CScriptThing *)(**(code **)(*DAT_0143e8f8 + 0x118))();
      8485 00cc9371 | CScriptThing::CScriptThing(local_e74,pCVar15);
      8486 00cc9382 | cVar4 = (**(code **)(*DAT_0143e8f8 + 0x314))();
      8487 00cc9391 | if (cVar4 == '\0') {
      8488 00cc93a2 | cVar4 = (**(code **)(*DAT_0143e8f8 + 0x318))();
      8489 00cc93b1 | if (cVar4 != '\0') {
      8490 00cc93b5 | pcVar9 = "RANGED";
      8491 00cc93ba | goto LAB_00cc93ba;
      8492          | }
      8493          | }
      8494          | else {
      8495 00cc9395 | pcVar9 = "MELEE";
      8496 00cc93ba | LAB_00cc93ba:
      8497 00cc93ba | CCharString::operator=((CCharString *)&local_b8,pcVar9);
      8498          | }
      8499 00cc93c2 | C3DClothPrimitive::~C3DClothPrimitive((C3DClothPrimitive *)local_e74);
      8500          | }
      8501 00cc93d3 | (**(code **)(*DAT_0143e8f8 + 0x208))();
      8502 00cc93db | ppVar22 = extraout_EDX_x00286;
      8503 00cd17fd | goto LAB_00cd17fd;
      8504          | }
      8505 00cc93e7 | CCharString::CCharString(local_460,"CameraRig",-1);
      8506 00cc93f2 | sVar13 = CCharString::GetLength(local_460);
      8507 00cc93fe | pcVar9 = CCharString::operator_char_const*(local_460);
      8508 00cc940a | pcVar12 = CCharString::operator_char_const*(local_c);
      8509 00cc9413 | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      8510          | std::
      8511 00cc9429 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8512          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8513 00cc9421 | local_460,extraout_EDX_x00287,ppVar48);
      8514 00cc941d | if (iVar33 != 0) {
      8515 00cc96c1 | CCharString::CCharString(local_390,"UseCameraFOVMarkerList",-1);
      8516 00cc96cc | sVar13 = CCharString::GetLength(local_390);
      8517 00cc96d8 | pcVar9 = CCharString::operator_char_const*(local_390);
      8518 00cc96e4 | pcVar12 = CCharString::operator_char_const*(local_c);
      8519 00cc96ed | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      8520          | std::
      8521 00cc9703 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      8522          | ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
      8523 00cc96fb | local_390,extraout_EDX_x00298,ppVar48);
      8524 00cc96f7 | if (iVar33 == 0) {
      8525 00cc9710 | lVar6 = CCharString::GetLength((CCharString *)&local_44);
      8526 00cc971a | ppVar22 = extraout_EDX_x00299;
      8527 00cc9718 | if ((((((lVar6 == 0) ||
      8528 00cc9720 | (lVar6 = CCharString::GetLength((CCharString *)&local_40),
      8529 00cc9728 | ppVar22 = extraout_EDX_x00300, lVar6 == 0)) ||
      8530 00cc9730 | (lVar6 = CCharString::GetLength((CCharString *)&local_3c),
      8531 00cc9738 | ppVar22 = extraout_EDX_x00301, lVar6 == 0)) ||
      8532 00cc9740 | ((lVar6 = CCharString::GetLength((CCharString *)&local_38),
      8533 00cc9748 | ppVar22 = extraout_EDX_x00302, lVar6 == 0 ||
      8534 00cc9750 | (lVar6 = CCharString::GetLength((CCharString *)&local_34),
      8535 00cc9758 | ppVar22 = extraout_EDX_x00303, lVar6 == 0)))) ||
      8536 00cc9760 | (lVar6 = CCharString::GetLength((CCharString *)&local_30),
      8537 00cc9768 | ppVar22 = extraout_EDX_x00304, lVar6 == 0)) ||
      8538 00cc9770 | (lVar6 = CCharString::GetLength(local_2c), ppVar22 = extraout_EDX_x00305,
      8539 00cc9778 | lVar6 == 0)) goto LAB_00cd17fd;
      8540 00cc978b | local_1f8 = -1.0;
      8541 00cc9795 | local_6a8 = CONCAT31(local_6a8._1_3_,1);
      8542 00cc9785 | puVar14 = (undefined4 *)CCharString::CCharString(local_a00,"HERO",-1);
      8543 00cc97a1 | pCVar1 = (CTCParticleAttacher *)*puVar14;
      8544 00cc97a3 | if (pCVar1 == local_44) {
      8545 00cc97c1 | LAB_00cc97c1:
      8546 00cc97c7 | pCVar15 = (CScriptThing *)(**(code **)(*DAT_0143e8f8 + 0x118))();
      8547          | }
      8548          | else {
      8549 00cc97a8 | if ((pCVar1 == (CTCParticleAttacher *)0x0) ||
      8550 00cc97ac | (local_44 == (CTCParticleAttacher *)0x0)) {
      8551 00cc97bb | cVar4 = '\0';
      8552          | }
      8553          | else {
      8554 00cc97b4 | CTCParticleAttacher::OnInitialActivate(pCVar1);
      8555 00cc97b9 | cVar4 = extraout_AL_29;
      8556          | }
      8557 00cc97bd | if (cVar4 != '\0') goto LAB_00cc97c1;
      8558 00cc97d9 | local_14 = local_14 | 4;
      8559 00cc97d7 | pCVar15 = (CScriptThing *)(**(code **)(*DAT_0143e8f8 + 0x120))();
      8560          | }
      8561 00cc97ef | CScriptThing::CScriptThing(local_d3c,pCVar15);
      8562 00cc97fe | ppVar22 = extraout_EDX_x00306;
```

Helpers called in window:
- `CCharString` (callsite 00cc979c)
- `CScriptThing` (callsite 00cc97f5)
- `GetLength` (callsite 00cc9773)
- `IsFalse` (callsite 00cc9359)
- `OnInitialActivate` (callsite 00cc97b4)
- `_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>` (callsite 00cc9703)
- `operator=` (callsite 00cc93bd)
- `operator_char_const*` (callsite 00cc96e7)
- `strncmp` (callsite 00cc96ed)
- `~C3DClothPrimitive` (callsite 00cc93c8)

## RemoveExtras (xref 00cc6acd, line 5705, dist 5)

```c
      5693 00cc6a6c | (**(code **)(*DAT_0143e8f8 + 0x5d4))();
      5694 00cc6a8f | (**(code **)(*DAT_0143e8f8 + 0x5e0))();
      5695          | }
      5696 00cc6aa1 | (**(code **)(*DAT_0143e8f8 + 0x3bc))();
      5697 00cc6aa9 | bVar3 = IsFalse((CCharString *)&local_44);
      5698 00cc6ab3 | if (bVar3) {
      5699 00cc6abf | (**(code **)(*DAT_0143e8f8 + 0x5d8))();
      5700          | }
      5701          | }
      5702          | }
      5703          | else {
      5704 00cc6ad8 | CCharString::CCharString
>>>   5705 00cc6ad2 | (local_408,"RemoveExtras",-1);
      5706 00cc6add | sVar13 = CCharString::GetLength(local_408);
      5707 00cc6aef | pcVar9 = CCharString::operator_char_const*
      5708 00cc6ae9 | (local_408);
      5709 00cc6af8 | pcVar12 = CCharString::operator_char_const*
      5710 00cc6af5 | (local_c);
      5711 00cc6afe | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      5712          | std::
      5713 00cc6b14 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      5714          | ((
      5715          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
      5716 00cc6b0c | *)local_408,extraout_EDX_x00174,ppVar48);
      5717 00cc6b08 | if (iVar33 == 0) {
      5718 00cc6b27 | CCarriedReadableDef::CCarriedReadableDef
      5719 00cc6b21 | (local_7ac);
      5720 00cc6b39 | param_4 = (
      5721          | pair<EHeroMorphType,CParticleMorphs::CEntry> *)
      5722 00cc6b39 | ((uint)param_4 & 0xffffff);
      5723 00cc6b3d | local_59 = '\0';
      5724 00cc6b33 | CCharString::CCharString(local_670,"limbo",-1);
      5725 00cc6b4c | pcVar9 = CCharString::operator_char_const*
      5726 00cc6b46 | (local_670);
      5727 00cc6b55 | pcVar12 = CCharString::operator_char_const*
      5728 00cc6b52 | ((CCharString *)&local_40);
      5729 00cc6b5b | iVar33 = _stricmp(pcVar12,pcVar9);
      5730          | std::
      5731 00cc6b70 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      5732          | ((
      5733          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
      5734 00cc6b68 | *)local_670,extraout_EDX_x00175,ppVar48);
      5735 00cc6b63 | if (iVar33 == 0) {
      5736 00cc6b79 | param_4 = (
      5737          | pair<EHeroMorphType,CParticleMorphs::CEntry> *)
      5738 00cc6b79 | CONCAT13(1,param_4._0_3_);
      5739          | }
      5740          | else {
      5741 00cc6b86 | CCharString::CCharString(local_850,"return",-1);
      5742 00cc6b97 | pcVar9 = CCharString::operator_char_const*
      5743 00cc6b91 | (local_850);
      5744 00cc6ba0 | pcVar12 = CCharString::operator_char_const*
      5745 00cc6b9d | ((CCharString *)&local_40);
      5746 00cc6ba6 | iVar33 = _stricmp(pcVar12,pcVar9);
      5747          | std::
      5748 00cc6bbb | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      5749          | ((
      5750          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
      5751 00cc6bb3 | *)local_850,extraout_EDX_x00176,ppVar48);
      5752 00cc6bae | if (iVar33 == 0) {
      5753 00cc6bc4 | local_59 = '\x01';
      5754          | }
      5755          | }
      5756 00cc6bcb | lVar6 = CCharString::GetLength
      5757 00cc6bc8 | ((CCharString *)&local_40);
      5758 00cc6bd0 | if (((lVar6 != 0) &&
      5759 00cc6bd4 | (bVar3 = IsFalse((CCharString *)&local_40),
      5760 00cc6bde | !bVar3)) &&
      5761 00cc6be0 | ((local_59 == '\0' && (param_4._3_1_ == 0)))) {
      5762 00cc6bf6 | pCVar15 = (CScriptThing *)FUN_00cbf9de();
      5763 00cc6c02 | CScriptThing::operator=
      5764 00cc6bfc | ((CScriptThing *)local_7ac,pCVar15);
      5765 00cc6c0d | C3DClothPrimitive::~C3DClothPrimitive
      5766 00cc6c07 | (local_14ac);
      5767          | }
      5768 00cc6c12 | if (local_a5 == '\0') {
      5769 00cc6c1f | lVar6 = CCharString::GetLength
      5770 00cc6c1c | ((CCharString *)&local_3c);
      5771 00cc6c24 | if ((lVar6 == 0) ||
      5772 00cc6c2f | (lVar6 = CCharString::GetLength
      5773 00cc6c2c | ((CCharString *)&local_38)
      5774 00cc6c34 | , lVar6 == 0)) {
      5775 00cc6cb8 | (**(code **)(*DAT_0143e8f8 + 0x150))();
      5776          | }
      5777          | else {
      5778 00cc6c3b | fVar39 = (float10)FUN_0099e690();
      5779 00cc6c40 | local_50 = (double)CONCAT44((float)fVar39,
      5780          | (EWeaponClass)
      5781 00cc6c40 | local_50);
      5782 00cc6c4f | FUN_00cbf9de();
      5783 00cc6c5b | CCharString::CCharString(local_678,"",-1);
      5784 00cc6c6e | iVar33 = *DAT_0143e8f8;
      5785          | CScriptThing::
      5786 00cc6c81 | ?GetPos@CScriptThing@@UBEABVC3DVector@@XZ
      5787 00cc6c78 | (local_106c);
      5788 00cc6c94 | (**(code **)(iVar33 + 0x130))();
      5789          | std::
      5790 00cc6ca0 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      5791          | ((
      5792          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
      5793 00cc6c9a | *)local_678,extraout_EDX_x00177,ppVar48);
      5794 00cc6cab | C3DClothPrimitive::~C3DClothPrimitive
      5795 00cc6ca5 | ((C3DClothPrimitive *)local_106c);
```

Helpers called in window:
- `?GetPos@CScriptThing@@UBEABVC3DVector@@XZ` (callsite 00cc6c81)
- `CCarriedReadableDef` (callsite 00cc6b27)
- `CCharString` (callsite 00cc6c61)
- `FUN_0099e690` (callsite 00cc6c3b)
- `FUN_00cbf9de` (callsite 00cc6c4f)
- `GetLength` (callsite 00cc6c2f)
- `IsFalse` (callsite 00cc6bd7)
- `_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>` (callsite 00cc6ca0)
- `_stricmp` (callsite 00cc6ba6)
- `operator=` (callsite 00cc6c02)
- `operator_char_const*` (callsite 00cc6ba0)
- `strncmp` (callsite 00cc6afe)
- `~C3DClothPrimitive` (callsite 00cc6cab)

## WaitActiveDialog (xref 00cc656b, line 5476, dist 5)

```c
      5464 00cc655a | ppVar22 = extraout_EDX_x00164;
      5465          | }
      5466          | }
      5467          | }
      5468 00cc6562 | paVar28 = (
      5469          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
      5470 00cc6562 | *)local_bc;
      5471 00cc2c6b | goto LAB_00cc2c6b;
      5472          | }
      5473          | }
      5474          | else {
      5475 00cc6576 | CCharString::CCharString
>>>   5476 00cc6570 | (local_49c,"WaitActiveDialog",-1);
      5477 00cc657b | sVar13 = CCharString::GetLength(local_49c);
      5478 00cc658d | pcVar9 = CCharString::operator_char_const*
      5479 00cc6587 | (local_49c);
      5480 00cc6596 | pcVar12 = CCharString::operator_char_const*
      5481 00cc6593 | (local_c);
      5482 00cc659c | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      5483          | std::
      5484 00cc65b2 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      5485          | ((
      5486          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
      5487 00cc65aa | *)local_49c,extraout_EDX_x00166,ppVar48);
      5488 00cc65a6 | if (iVar33 == 0) {
      5489 00cc65bb | if (local_98 != -1) {
      5490          | do {
      5491 00cc661b | cVar4 = (**(code **)(*DAT_0143e8f8 + 0x5c0))
      5492          | ();
      5493 00cc65cd | if ((cVar4 == '\0') ||
      5494 00cc65c6 | (bVar3 = IsCutsceneSkipped(), bVar3))
      5495          | break;
      5496 00cc65d3 | if (uStack_8._3_1_ != '\0') {
      5497 00cc65de | (**(code **)(*DAT_0143e8f8 + 0x1c))();
      5498          | }
      5499 00cc65ec | PlayCutsceneSound((
      5500          | map<int,CCharString,std::less<int>,std::allocator<std::pair<int_const_,CCharString>_>_>
      5501 00cc65e9 | *)local_78,DAT_013b83c8);
      5502 00cc65f1 | if (DAT_013b83c8 != -1) {
      5503 00cc65f9 | DAT_013b83c8 = DAT_013b83c8 + 1;
      5504          | }
      5505 00cc6604 | } while ((DAT_013d2838 == 0) ||
      5506 00cc6608 | (*(char *)(DAT_013d2838 + 5) == '\0'));
      5507          | }
      5508          | }
      5509          | else {
      5510 00cc6638 | CCharString::CCharString
      5511 00cc6632 | (local_2d0,"RegisterActor",-1);
      5512 00cc663d | sVar13 = CCharString::GetLength(local_2d0);
      5513 00cc664f | pcVar9 = CCharString::operator_char_const*
      5514 00cc6649 | (local_2d0);
      5515 00cc6658 | pcVar12 = CCharString::operator_char_const*
      5516 00cc6655 | (local_c);
      5517 00cc665e | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      5518          | std::
      5519 00cc6674 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      5520          | ((
      5521          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
      5522 00cc666c | *)local_2d0,extraout_EDX_x00167,ppVar48);
      5523 00cc6668 | if (iVar33 == 0) {
      5524 00cc6680 | lVar6 = CCharString::GetLength
      5525 00cc667d | ((CCharString *)&local_44);
      5526 00cc6685 | if (lVar6 != 0) {
      5527          | std::
      5528          | vector<EWeaponClass,std::allocator<EWeaponClass>_>
      5529 00cc669b | ::_Umove<EWeaponClass*>
      5530          | ((
      5531          | vector<EWeaponClass,std::allocator<EWeaponClass>_>
      5532 00cc6691 | *)local_c24,(EWeaponClass *)&local_50,
      5533 00cc668d | (EWeaponClass *)&local_44,(EWeaponClass *)ppVar48)
      5534          | ;
      5535          | }
      5536          | }
      5537          | else {
      5538 00cc66b1 | CCharString::CCharString
      5539 00cc66ab | (local_400,"RemoveAllThings",-1);
      5540 00cc66b6 | sVar13 = CCharString::GetLength(local_400);
      5541 00cc66c8 | pcVar9 = CCharString::operator_char_const*
      5542 00cc66c2 | (local_400);
      5543 00cc66d1 | pcVar12 = CCharString::operator_char_const*
      5544 00cc66ce | (local_c);
      5545 00cc66d7 | iVar33 = strncmp(pcVar12,pcVar9,sVar13);
      5546          | std::
      5547 00cc66ed | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      5548          | ((
      5549          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
      5550 00cc66e5 | *)local_400,extraout_EDX_x00168,ppVar48);
      5551 00cc66e1 | if (iVar33 == 0) {
      5552 00cc66fd | lVar6 = CCharString::GetLength
      5553 00cc66fa | ((CCharString *)&local_44);
      5554 00cc6702 | if (lVar6 != 0) {
      5555 00cc670a | uVar30 = 0;
      5556 00cc6718 | local_5a8 = 0;
      5557 00cc671e | local_5a4 = 0;
      5558 00cc6724 | local_5a0 = 0;
      5559 00cc672a | CCharString::CCharString
      5560 00cc6712 | (local_668,"LadyGreyIntro",-1);
      5561 00cc6735 | (**(code **)(*DAT_0143e8f8 + 300))();
      5562          | std::
      5563 00cc6751 | _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
      5564          | ((
      5565          | allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
      5566 00cc674b | *)local_668,extraout_EDX_x00169,ppVar48);
```

Helpers called in window:
- `CCharString` (callsite 00cc672a)
- `GetLength` (callsite 00cc66fd)
- `IsCutsceneSkipped` (callsite 00cc65c6)
- `PlayCutsceneSound` (callsite 00cc65ec)
- `_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>` (callsite 00cc6751)
- `_Umove<EWeaponClass*>` (callsite 00cc669b)
- `operator_char_const*` (callsite 00cc66d1)
- `strncmp` (callsite 00cc66d7)

