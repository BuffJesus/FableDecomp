//=== CThingCreatureDef @ 01fb3ed0 ===

/* [ported from ego_r via strfp] */

void _Transfer_CThingCreatureDef__UAEXAAVCPersistContext___Z(undefined4 param_1)

{
  char cVar1;
  undefined1 auStack_dc [4];
  undefined4 uStack_d8;
  undefined1 *puStack_d4;
  undefined1 auStack_d0 [4];
  undefined4 uStack_cc;
  undefined1 *puStack_c8;
  char *pcStack_c4;
  byte *pbStack_c0;
  undefined4 *puStack_bc;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  int local_80;
  undefined1 local_7a;
  undefined1 local_79;
  undefined1 local_78;
  undefined1 local_77;
  undefined1 local_76;
  undefined1 local_75;
  undefined1 local_74;
  undefined1 local_73;
  undefined1 local_72;
  undefined1 local_71;
  undefined4 local_70;
  undefined1 local_6a;
  undefined1 local_69;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined1 local_5c [24];
  undefined1 *local_44;
  undefined1 *local_40;
  undefined1 *local_3c;
  undefined1 local_38 [12];
  undefined1 *local_2c;
  undefined1 *local_28;
  undefined1 *local_24;
  undefined4 local_20;
  byte local_1b;
  byte local_1a;
  byte local_19;
  byte local_18;
  byte local_17;
  byte local_16;
  byte local_15;
  byte local_14;
  byte local_13;
  byte local_12;
  byte local_11;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03cd2738;
  local_10 = ExceptionList;
  puStack_bc = (undefined4 *)param_1;
  pbStack_c0 = (byte *)0x1fb3efd;
  ExceptionList = &local_10;
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z();
  puStack_bc = (undefined4 *)(local_80 + 0x78);
  pbStack_c0 = (byte *)0x414fe24;
  pcStack_c4 = (char *)0x1fb3f11;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x7c);
  pbStack_c0 = (byte *)0x414fe14;
  pcStack_c4 = (char *)0x1fb3f25;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x80);
  pbStack_c0 = (byte *)0x414fdfc;
  pcStack_c4 = (char *)0x1fb3f3b;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x84);
  pbStack_c0 = (byte *)0x414fdec;
  pcStack_c4 = (char *)0x1fb3f52;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x88);
  pbStack_c0 = (byte *)0x414fddc;
  pcStack_c4 = (char *)0x1fb3f69;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x8c);
  pbStack_c0 = (byte *)0x414fdc0;
  pcStack_c4 = (char *)0x1fb3f7f;
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z();
  local_20 = 0x41f00000;
  puStack_bc = &local_20;
  pbStack_c0 = (byte *)(local_80 + 0x90);
  pcStack_c4 = "NumSecondsToPersistOnDeath";
  puStack_c8 = (undefined1 *)0x1fb3fa3;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x94);
  pbStack_c0 = (byte *)0x414fd88;
  pcStack_c4 = (char *)0x1fb3fb9;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x98);
  pbStack_c0 = (byte *)0x414fd74;
  pcStack_c4 = (char *)0x1fb3fd0;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x9c);
  pbStack_c0 = (byte *)0x414fd60;
  pcStack_c4 = (char *)0x1fb3fe7;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xa0);
  pbStack_c0 = (byte *)0x414fd4c;
  pcStack_c4 = (char *)0x1fb3ffd;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xa4);
  pbStack_c0 = (byte *)0x414fd38;
  pcStack_c4 = (char *)0x1fb4014;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xa8);
  pbStack_c0 = (byte *)0x414fd24;
  pcStack_c4 = (char *)0x1fb402b;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xac);
  pbStack_c0 = (byte *)0x414fd0c;
  pcStack_c4 = (char *)0x1fb4041;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xb0);
  pbStack_c0 = (byte *)0x414fcf4;
  pcStack_c4 = (char *)0x1fb4058;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xb4);
  pbStack_c0 = (byte *)0x414fcdc;
  pcStack_c4 = (char *)0x1fb406f;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xb8);
  pbStack_c0 = (byte *)0x414fcbc;
  pcStack_c4 = (char *)0x1fb4085;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xbc);
  pbStack_c0 = (byte *)0x414fca8;
  pcStack_c4 = (char *)0x1fb409c;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xc0);
  pbStack_c0 = (byte *)0x414fc98;
  pcStack_c4 = (char *)0x1fb40b3;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xc4);
  pbStack_c0 = &DAT_0403e2ec;
  pcStack_c4 = (char *)0x1fb40c9;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  puStack_bc = (undefined4 *)(local_80 + 200);
  pbStack_c0 = &DAT_0414fc94;
  pcStack_c4 = (char *)0x1fb40e0;
  ___Transfer_W4ESex___CPersistContext__QAEXPBDAAW4ESex___Z();
  puStack_bc = (undefined4 *)(local_80 + 0xcc);
  pbStack_c0 = (byte *)0x414fc84;
  pcStack_c4 = (char *)0x1fb40f7;
  ___Transfer_W4ECreatureType___CPersistContext__QAEXPBDAAW4ECreatureType___Z();
  puStack_bc = (undefined4 *)(local_80 + 0xd0);
  pbStack_c0 = (byte *)0x414fc70;
  pcStack_c4 = (char *)0x1fb410d;
  ___TransferVector_W4ENavigatorType___CPersistContext__QAEXPBDAAV__vector_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___Z
            ();
  puStack_bc = (undefined4 *)0x1fb4115;
  cVar1 = _IsBinary_CPersistContext__QBE_NXZ();
  if (cVar1 == '\0') {
    local_24 = (undefined1 *)&pcStack_c4;
    puStack_c8 = (undefined1 *)&pcStack_c4;
    uStack_cc = 0x1fb4137;
    local_88 = _end___vector_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std__QAE_AV___Vector_iterator_V___Vector_val_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___2_XZ
                         ();
    local_8 = 0;
    local_28 = auStack_d0;
    puStack_d4 = auStack_d0;
    uStack_d8 = 0x1fb4161;
    local_84 = local_88;
    local_8c = _begin___vector_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std__QAE_AV___Vector_iterator_V___Vector_val_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___2_XZ
                         ();
    local_8 = 0xffffffff;
    puStack_d4 = (undefined1 *)0x1fb4173;
    ___sort_V___Vector_iterator_V___Vector_val_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___std___std__YIXV___Vector_iterator_V___Vector_val_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___0_0_Z
              ();
    puStack_bc = (undefined4 *)local_38;
    pbStack_c0 = (byte *)0x1fb4185;
    puStack_c8 = (undefined1 *)
                 _end___vector_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std__QAE_AV___Vector_iterator_V___Vector_val_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___2_XZ
                           ();
    local_8 = 1;
    local_2c = (undefined1 *)&pcStack_c4;
    uStack_cc = 0x1fb41b2;
    local_94 = puStack_c8;
    local_90 = puStack_c8;
    local_9c = FID_conflict__String_const_iterator<std::_String_val<std::_Simple_types<unsigned_short>_>_>
                         ();
    local_8._0_1_ = 2;
    local_40 = auStack_d0;
    puStack_d4 = auStack_d0;
    uStack_d8 = 0x1fb41df;
    local_98 = local_9c;
    local_a4 = _end___vector_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std__QAE_AV___Vector_iterator_V___Vector_val_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___2_XZ
                         ();
    local_8._0_1_ = 3;
    local_44 = auStack_dc;
    local_a0 = local_a4;
    local_a8 = _begin___vector_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std__QAE_AV___Vector_iterator_V___Vector_val_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___2_XZ
                         (auStack_dc);
    local_8._0_1_ = 2;
    puStack_d4 = (undefined1 *)
                 ___unique_V___Vector_iterator_V___Vector_val_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___std___std__YI_AV___Vector_iterator_V___Vector_val_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___0_V10_0_Z
                           ();
    local_8._0_1_ = 4;
    local_3c = auStack_d0;
    uStack_d8 = 0x1fb4248;
    local_b0 = puStack_d4;
    local_ac = puStack_d4;
    local_b4 = FID_conflict__String_const_iterator<std::_String_val<std::_Simple_types<unsigned_short>_>_>
                         ();
    puStack_d4 = local_5c;
    local_8._0_1_ = 5;
    uStack_d8 = 0x1fb4264;
    local_b8 = _erase___vector_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std__QAE_AV___Vector_iterator_V___Vector_val_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___2_V___Vector_const_iterator_V___Vector_val_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___2_0_Z
                         ();
    puStack_bc = (undefined4 *)0x1fb4272;
    __1___Vector_iterator_V___Vector_val_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___std__QAE_XZ
              ();
    local_8 = CONCAT31(local_8._1_3_,1);
    puStack_bc = (undefined4 *)0x1fb427e;
    __1___Vector_iterator_V___Vector_val_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___std__QAE_XZ
              ();
    local_8 = 0xffffffff;
    puStack_bc = (undefined4 *)0x1fb428d;
    __1___Vector_iterator_V___Vector_val_W4ENavigatorType__V__allocator_W4ENavigatorType___std___std___std__QAE_XZ
              ();
  }
  local_60 = 0xffffffff;
  puStack_bc = &local_60;
  pbStack_c0 = (byte *)(local_80 + 0xe4);
  pcStack_c4 = "InitialWeaponDef";
  puStack_c8 = (undefined1 *)0x1fb42af;
  ___Transfer_J_CPersistContext__QAEXPBDAAJABJ_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xe8);
  pbStack_c0 = (byte *)0x414fc40;
  pcStack_c4 = (char *)0x1fb42c5;
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xec);
  pbStack_c0 = (byte *)0x414fc24;
  pcStack_c4 = (char *)0x1fb42dc;
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z();
  local_64 = 0xffffffff;
  puStack_bc = &local_64;
  pbStack_c0 = (byte *)(local_80 + 0xf0);
  pcStack_c4 = "RangedWeaponDef";
  puStack_c8 = (undefined1 *)0x1fb42fd;
  ___Transfer_J_CPersistContext__QAEXPBDAAJABJ_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xf4);
  pbStack_c0 = (byte *)0x414fbf4;
  pcStack_c4 = (char *)0x1fb4314;
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xf8);
  pbStack_c0 = (byte *)0x414fbec;
  pcStack_c4 = (char *)0x1fb432b;
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z();
  puStack_bc = (undefined4 *)(local_80 + 0xfc);
  pbStack_c0 = (byte *)0x414fbd0;
  pcStack_c4 = (char *)0x1fb4341;
  ___Transfer_W4ECreatureInteractionType___CPersistContext__QAEXPBDAAW4ECreatureInteractionType___Z
            ();
  puStack_bc = (undefined4 *)(local_80 + 0x100);
  pbStack_c0 = (byte *)0x40328e8;
  pcStack_c4 = (char *)0x1fb4358;
  ___Transfer_W4EThingCreatureProperty___CPersistContext__QAEXPBDAAW4EThingCreatureProperty___Z();
  local_68 = 0x3f800000;
  puStack_bc = &local_68;
  pbStack_c0 = (byte *)(local_80 + 0x104);
  pcStack_c4 = "Damage";
  puStack_c8 = (undefined1 *)0x1fb4377;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x108);
  pbStack_c0 = (byte *)0x414fbbc;
  pcStack_c4 = (char *)0x1fb438e;
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x10c);
  pbStack_c0 = (byte *)0x414fba8;
  pcStack_c4 = (char *)0x1fb43a5;
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x110);
  pbStack_c0 = (byte *)0x4032930;
  pcStack_c4 = (char *)0x1fb43bb;
  ___Transfer_VCEngineGraphic___CPersistContext__QAEXPBDAAVCEngineGraphic___Z();
  puStack_bc = (undefined4 *)(local_80 + 0x11e);
  pbStack_c0 = (byte *)0x414fb98;
  pcStack_c4 = (char *)0x1fb43d2;
  ___Transfer_VCEngineGraphic___CPersistContext__QAEXPBDAAVCEngineGraphic___Z();
  puStack_bc = (undefined4 *)(local_80 + 300);
  pbStack_c0 = (byte *)0x414fb88;
  pcStack_c4 = (char *)0x1fb43e9;
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x130);
  pbStack_c0 = (byte *)0x414fb6c;
  pcStack_c4 = (char *)0x1fb43ff;
  ___Transfer_W4EControlledMovementType___CPersistContext__QAEXPBDAAW4EControlledMovementType___Z();
  puStack_bc = (undefined4 *)(local_80 + 0x134);
  pbStack_c0 = (byte *)0x414fb5c;
  pcStack_c4 = (char *)0x1fb4416;
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z();
  local_69 = 0;
  puStack_bc = (undefined4 *)&local_69;
  pbStack_c0 = (byte *)(local_80 + 0x138);
  pcStack_c4 = "ResetCombatTypeOnUnsheatheDefaultWeapons";
  puStack_c8 = (undefined1 *)0x1fb4434;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x13c);
  pbStack_c0 = (byte *)0x414fb18;
  pcStack_c4 = (char *)0x1fb444b;
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x140);
  pbStack_c0 = (byte *)0x414fb04;
  pcStack_c4 = (char *)0x1fb4462;
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x144);
  pbStack_c0 = (byte *)0x414faf0;
  pcStack_c4 = (char *)0x1fb4478;
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z();
  local_11 = *(byte *)(local_80 + 0x148) & 1;
  local_6a = 1;
  puStack_bc = (undefined4 *)&local_6a;
  pbStack_c0 = &local_11;
  pcStack_c4 = "DeferHitsIfBehindHitter";
  puStack_c8 = (undefined1 *)0x1fb44a0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z();
  *(byte *)(local_80 + 0x148) = *(byte *)(local_80 + 0x148) & 0xfe | local_11 & 1;
  puStack_bc = (undefined4 *)(local_80 + 0x14c);
  pbStack_c0 = (byte *)0x414fac4;
  pcStack_c4 = (char *)0x1fb44d3;
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z();
  local_70 = 0x42960000;
  puStack_bc = &local_70;
  pbStack_c0 = (byte *)(local_80 + 0x150);
  pcStack_c4 = "RecoverEvadeHitsPercent";
  puStack_c8 = (undefined1 *)0x1fb44f7;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z();
  puStack_bc = (undefined4 *)(local_80 + 0x154);
  pbStack_c0 = (byte *)0x414fa8c;
  pcStack_c4 = (char *)0x1fb450d;
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z();
  local_12 = *(byte *)(local_80 + 0x158) & 1;
  local_71 = 0;
  puStack_bc = (undefined4 *)&local_71;
  pbStack_c0 = &local_12;
  pcStack_c4 = "LeaveDeadCreature";
  puStack_c8 = (undefined1 *)0x1fb4535;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z();
  *(byte *)(local_80 + 0x158) = *(byte *)(local_80 + 0x158) & 0xfe | local_12 & 1;
  local_13 = *(byte *)(local_80 + 0x158) >> 1 & 1;
  local_72 = 0;
  puStack_bc = (undefined4 *)&local_72;
  pbStack_c0 = &local_13;
  pcStack_c4 = "AbleToStrafe";
  puStack_c8 = (undefined1 *)0x1fb457c;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z();
  *(byte *)(local_80 + 0x158) = *(byte *)(local_80 + 0x158) & 0xfd | (local_13 & 1) << 1;
  local_14 = *(byte *)(local_80 + 0x158) >> 2 & 1;
  local_73 = 0;
  puStack_bc = (undefined4 *)&local_73;
  pbStack_c0 = &local_14;
  pcStack_c4 = "CreateDummyOnDeath";
  puStack_c8 = (undefined1 *)0x1fb45c4;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z();
  *(byte *)(local_80 + 0x158) = *(byte *)(local_80 + 0x158) & 0xfb | (local_14 & 1) << 2;
  local_15 = *(byte *)(local_80 + 0x158) >> 3 & 1;
  local_74 = 0;
  puStack_bc = (undefined4 *)&local_74;
  pbStack_c0 = &local_15;
  pcStack_c4 = "ImmuneToAttack";
  puStack_c8 = (undefined1 *)0x1fb460e;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z();
  *(byte *)(local_80 + 0x158) = *(byte *)(local_80 + 0x158) & 0xf7 | (local_15 & 1) << 3;
  local_16 = *(byte *)(local_80 + 0x158) >> 4 & 1;
  local_75 = 1;
  puStack_bc = (undefined4 *)&local_75;
  pbStack_c0 = &local_16;
  pcStack_c4 = "UseMeleeWeaponAsDefault";
  puStack_c8 = (undefined1 *)0x1fb4659;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z();
  *(byte *)(local_80 + 0x158) = *(byte *)(local_80 + 0x158) & 0xef | (local_16 & 1) << 4;
  local_17 = *(byte *)(local_80 + 0x158) >> 5 & 1;
  local_76 = 1;
  puStack_bc = (undefined4 *)&local_76;
  pbStack_c0 = &local_17;
  pcStack_c4 = "UnsheatheWeaponsAutomatically";
  puStack_c8 = (undefined1 *)0x1fb46a2;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z();
  *(byte *)(local_80 + 0x158) = *(byte *)(local_80 + 0x158) & 0xdf | (local_17 & 1) << 5;
  local_18 = *(byte *)(local_80 + 0x158) >> 6 & 1;
  local_77 = 0;
  puStack_bc = (undefined4 *)&local_77;
  pbStack_c0 = &local_18;
  pcStack_c4 = "IsWillable";
  puStack_c8 = (undefined1 *)0x1fb46ec;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z();
  *(byte *)(local_80 + 0x158) = *(byte *)(local_80 + 0x158) & 0xbf | (local_18 & 1) << 6;
  local_19 = *(byte *)(local_80 + 0x158) >> 7;
  local_78 = 0;
  puStack_bc = (undefined4 *)&local_78;
  pbStack_c0 = &local_19;
  pcStack_c4 = "IsIndestructible";
  puStack_c8 = (undefined1 *)0x1fb4737;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z();
  *(byte *)(local_80 + 0x158) = *(byte *)(local_80 + 0x158) & 0x7f | local_19 << 7;
  local_1a = *(byte *)(local_80 + 0x159) & 1;
  local_79 = 0;
  puStack_bc = (undefined4 *)&local_79;
  pbStack_c0 = &local_1a;
  pcStack_c4 = "IsInvulnerableDuringGenericResponse";
  puStack_c8 = (undefined1 *)0x1fb477d;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z();
  *(byte *)(local_80 + 0x159) = *(byte *)(local_80 + 0x159) & 0xfe | local_1a & 1;
  local_1b = *(byte *)(local_80 + 0x159) >> 1 & 1;
  local_7a = 1;
  puStack_bc = (undefined4 *)&local_7a;
  pbStack_c0 = &local_1b;
  pcStack_c4 = "IsPushedByExplosions";
  puStack_c8 = (undefined1 *)0x1fb47c3;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z();
  *(byte *)(local_80 + 0x159) = *(byte *)(local_80 + 0x159) & 0xfd | (local_1b & 1) << 1;
  ExceptionList = local_10;
  return;
}



//=== CThingBaseDef @ 01fd5340 ===

/* [ported from ego_r via strfp] */

void _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(void)

{
  code *pcVar1;
  char cVar2;
  BOOL BVar3;
  int *in_ECX;
  undefined1 *puVar4;
  undefined1 *puVar5;
  char *pcVar6;
  undefined1 *puVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined1 local_a0 [24];
  undefined1 local_88 [8];
  undefined1 local_80 [4];
  undefined1 local_7c [11];
  char local_71;
  undefined1 local_70 [8];
  undefined1 local_68 [8];
  undefined1 local_60 [11];
  char local_55;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined1 local_38;
  undefined1 local_37;
  undefined1 local_36;
  undefined1 local_35;
  undefined1 local_34;
  undefined1 local_33;
  undefined1 local_32;
  undefined1 local_31;
  undefined1 local_30;
  undefined1 local_2f;
  undefined1 local_2e;
  undefined1 local_2d;
  undefined1 local_2c;
  undefined1 local_2b;
  undefined1 local_2a;
  undefined1 local_29;
  undefined1 local_28;
  undefined1 local_27;
  undefined1 local_26;
  undefined1 local_25;
  byte local_24;
  byte local_23;
  byte local_22;
  byte local_21;
  byte local_20;
  byte local_1f;
  byte local_1e;
  byte local_1d;
  byte local_1c;
  byte local_1b;
  byte local_1a;
  byte local_19;
  byte local_18;
  byte local_17;
  byte local_16;
  byte local_15;
  byte local_14;
  byte local_13;
  byte local_12;
  byte local_11;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03cd6b3c;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  ___Transfer_VCThingComponentSet___CPersistContext__QAEXPBDAAVCThingComponentSet___Z
            ("Components",(int)in_ECX + 0x41);
  local_11 = *(byte *)((int)in_ECX + 0x49) & 1;
  local_25 = 1;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("AvailableInEditor",&local_11,&local_25);
  *(byte *)((int)in_ECX + 0x49) = *(byte *)((int)in_ECX + 0x49) & 0xfe | local_11 & 1;
  local_12 = *(byte *)((int)in_ECX + 0x4a) >> 1 & 1;
  local_26 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("GameStatePersisted",&local_12,&local_26);
  *(byte *)((int)in_ECX + 0x4a) = *(byte *)((int)in_ECX + 0x4a) & 0xfd | (local_12 & 1) << 1;
  local_13 = *(byte *)((int)in_ECX + 0x49) >> 3 & 1;
  local_27 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("LevelLess",&local_13,&local_27);
  *(byte *)((int)in_ECX + 0x49) = *(byte *)((int)in_ECX + 0x49) & 0xf7 | (local_13 & 1) << 3;
  local_14 = *(byte *)((int)in_ECX + 0x49) >> 4 & 1;
  local_28 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("IsDrawable",&local_14,&local_28);
  *(byte *)((int)in_ECX + 0x49) = *(byte *)((int)in_ECX + 0x49) & 0xef | (local_14 & 1) << 4;
  local_15 = *(byte *)((int)in_ECX + 0x49) >> 1 & 1;
  local_29 = 1;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("AlwaysFrameUpdate",&local_15,&local_29);
  *(byte *)((int)in_ECX + 0x49) = *(byte *)((int)in_ECX + 0x49) & 0xfd | (local_15 & 1) << 1;
  local_16 = *(byte *)((int)in_ECX + 0x49) >> 7;
  local_2a = 1;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("InMapSearches",&local_16,&local_2a);
  *(byte *)((int)in_ECX + 0x49) = *(byte *)((int)in_ECX + 0x49) & 0x7f | local_16 << 7;
  local_17 = *(byte *)((int)in_ECX + 0x49) >> 2 & 1;
  local_2b = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("StartKillLocked",&local_17,&local_2b);
  *(byte *)((int)in_ECX + 0x49) = *(byte *)((int)in_ECX + 0x49) & 0xfb | (local_17 & 1) << 2;
  local_18 = *(byte *)((int)in_ECX + 0x4a) & 1;
  local_2c = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("IsLoadableGlobal",&local_18,&local_2c);
  *(byte *)((int)in_ECX + 0x4a) = *(byte *)((int)in_ECX + 0x4a) & 0xfe | local_18 & 1;
  local_19 = *(byte *)((int)in_ECX + 0x4a) >> 2 & 1;
  local_2d = 1;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z
            ("CanComeBetweenCameraAndObservedThing",&local_19,&local_2d);
  *(byte *)((int)in_ECX + 0x4a) = *(byte *)((int)in_ECX + 0x4a) & 0xfb | (local_19 & 1) << 2;
  local_1a = *(byte *)((int)in_ECX + 0x4a) >> 5 & 1;
  local_2e = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("IsVulnerableToMelee",&local_1a,&local_2e);
  *(byte *)((int)in_ECX + 0x4a) = *(byte *)((int)in_ECX + 0x4a) & 0xdf | (local_1a & 1) << 5;
  local_1b = *(byte *)((int)in_ECX + 0x4a) >> 6 & 1;
  local_2f = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("Damageable",&local_1b,&local_2f);
  *(byte *)((int)in_ECX + 0x4a) = *(byte *)((int)in_ECX + 0x4a) & 0xbf | (local_1b & 1) << 6;
  local_1c = *(byte *)((int)in_ECX + 0x49) >> 6 & 1;
  local_30 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("ForceNoSerialise",&local_1c,&local_30);
  *(byte *)((int)in_ECX + 0x49) = *(byte *)((int)in_ECX + 0x49) & 0xbf | (local_1c & 1) << 6;
  local_1d = *(byte *)((int)in_ECX + 0x4a) >> 3 & 1;
  local_31 = 1;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("DrawWeaponTrails",&local_1d,&local_31);
  *(byte *)((int)in_ECX + 0x4a) = *(byte *)((int)in_ECX + 0x4a) & 0xf7 | (local_1d & 1) << 3;
  local_1e = *(byte *)((int)in_ECX + 0x4a) >> 4 & 1;
  local_32 = 1;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z
            ("DrawProjectileWeaponTrails",&local_1e,&local_32);
  *(byte *)((int)in_ECX + 0x4a) = *(byte *)((int)in_ECX + 0x4a) & 0xef | (local_1e & 1) << 4;
  local_1f = *(byte *)((int)in_ECX + 0x4a) >> 7;
  local_33 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z
            ("AlwaysIncludeInObjectStrikeScans",&local_1f,&local_33);
  *(byte *)((int)in_ECX + 0x4a) = *(byte *)((int)in_ECX + 0x4a) & 0x7f | local_1f << 7;
  local_20 = *(byte *)((int)in_ECX + 0x4b) & 1;
  local_34 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("DisableOcclusionTesting",&local_20,&local_34);
  *(byte *)((int)in_ECX + 0x4b) = *(byte *)((int)in_ECX + 0x4b) & 0xfe | local_20 & 1;
  local_21 = *(byte *)((int)in_ECX + 0x4b) >> 1 & 1;
  local_35 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("DrawAfterWater",&local_21,&local_35);
  *(byte *)((int)in_ECX + 0x4b) = *(byte *)((int)in_ECX + 0x4b) & 0xfd | (local_21 & 1) << 1;
  local_22 = *(byte *)((int)in_ECX + 0x4b) >> 2 & 1;
  local_36 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z
            ("ForceRenderedLastFrameOverride",&local_22,&local_36);
  *(byte *)((int)in_ECX + 0x4b) = *(byte *)((int)in_ECX + 0x4b) & 0xfb | (local_22 & 1) << 2;
  local_23 = *(byte *)((int)in_ECX + 0x4b) >> 3 & 1;
  local_37 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("AddToComboMultiplierOnHit",&local_23,&local_37)
  ;
  *(byte *)((int)in_ECX + 0x4b) = *(byte *)((int)in_ECX + 0x4b) & 0xf7 | (local_23 & 1) << 3;
  local_24 = *(byte *)((int)in_ECX + 0x4b) >> 4 & 1;
  local_38 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z
            ("GiveHeroStatChangesOnBeingHit",&local_24,&local_38);
  *(byte *)((int)in_ECX + 0x4b) = *(byte *)((int)in_ECX + 0x4b) & 0xef | (local_24 & 1) << 4;
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("GroupDef",in_ECX + 0x13);
  local_3c = 0x42800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("RenderFadeDistance",in_ECX + 0x14,&local_3c);
  local_40 = 1;
  ___Transfer_J_CPersistContext__QAEXPBDAAJABJ_Z("PersistenceFlags",in_ECX + 0x15,&local_40);
  local_44 = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("Health",in_ECX + 0x16,&local_44);
  local_48 = 0xbdcccccd;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("MinHealth",in_ECX + 0x17,&local_48);
  local_4c = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("MeshHeightOffset",in_ECX + 0x18,&local_4c);
  local_50 = 0x40000000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("MeshHeight",in_ECX + 0x19,&local_50);
  local_54 = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("MeshRadius",in_ECX + 0x1a,&local_54);
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("ApproxMaxMeshHeight",in_ECX + 0x1b,in_ECX + 0x19);
  ___Transfer_VCDefString___CPersistContext__QAEXPBDAAVCDefString___Z
            ("DefaultScriptName",in_ECX + 0x1c);
  ___Transfer_VCDefString___CPersistContext__QAEXPBDAAVCDefString___Z
            ("DefaultScriptData",in_ECX + 0x1d);
  if (9999 < in_ECX[0x15]) {
    __0CWideString__QAE_PB_W_Z(L"thing_base_def.cpp");
    local_8 = 0;
    __0CCharString__QAE_PBDJ_Z(&DAT_041534e3,0xffffffff);
    local_8._0_1_ = 1;
    __0CCharString__QAE_PBDJ_Z("PersistenceFlags<10000",0xffffffff);
    local_8._0_1_ = 2;
    uVar10 = 1;
    uVar8 = 0x3d;
    puVar7 = local_60;
    puVar4 = local_68;
    puVar5 = local_70;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar5,puVar4,puVar7,0x3d,1);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar5,puVar4,puVar7,uVar8,uVar10);
    local_55 = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 1;
    __1CCharString__QAE_XZ();
    local_8 = (uint)local_8._1_3_ << 8;
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    __1CWideString__QAE_XZ();
    if (local_55 != '\0') {
      BVar3 = IsDebuggerPresent();
      if (BVar3 != 0) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
  }
  cVar2 = _IsBinary_CPersistContext__QBE_NXZ();
  if (((cVar2 == '\0') && ((*(byte *)((int)in_ECX + 0x4a) & 1) != 0)) &&
     (*(char *)((int)in_ECX + 0x49) < '\0')) {
    __0CWideString__QAE_PB_W_Z(L"thing_base_def.cpp");
    local_8 = 3;
    puVar7 = local_80;
    (**(code **)(*in_ECX + 0x28))(puVar7,local_88);
    GetObject(puVar7);
    local_8._0_1_ = 4;
    __0CCharString__QAE_PBDJ_Z
              ("persist.IsBinary() || NOT IsLoadableGlobal || NOT InMapSearches",0xffffffff);
    local_8._0_1_ = 5;
    uVar9 = 1;
    uVar10 = 0x3e;
    puVar7 = local_7c;
    pcVar6 = " cannot be in map searches and global";
    __H_YI_AVCCharString__PBDABV0__Z(local_88);
    local_8._0_1_ = 6;
    uVar8 = __H_YI_AVCCharString__ABV0_PBD_Z(pcVar6);
    local_8._0_1_ = 7;
    puVar4 = local_a0;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar4,uVar8,puVar7,uVar10,uVar9);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar4,uVar8,puVar7,uVar10,uVar9);
    local_71 = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 6;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 5;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 4;
    __1CCharString__QAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,3);
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    __1CWideString__QAE_XZ();
    if (local_71 != '\0') {
      BVar3 = IsDebuggerPresent();
      if (BVar3 != 0) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
  }
  ExceptionList = local_10;
  return;
}



//=== CThingGroupDef @ 0191da25 ===

/* [ported from ego_r via strfp] */

void _Transfer_CThingGroupDef__UAEXAAVCPersistContext___Z(void)

{
  return;
}



//=== CThingVillageDef @ 0191d65d ===

/* [ported from ego_r via strfp] */

void _Transfer_CThingVillageDef__UAEXAAVCPersistContext___Z(undefined4 param_1)

{
  int in_ECX;
  
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  ___Transfer_VCEngineGraphic___CPersistContext__QAEXPBDAAVCEngineGraphic___Z
            ("Graphic",in_ECX + 0x78);
  return;
}



//=== CSoundThemeDef @ 0193430c ===

/* [ported from ego_r via strfp] */

void _Transfer_CSoundThemeDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("AtmosIndex",in_ECX + 0x48);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("MusicSet",in_ECX + 0x4c);
  ___Transfer_VCRGBColour___CPersistContext__QAEXPBDAAVCRGBColour___Z("EditColour",in_ECX + 0x44);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z(&DAT_0403c84c,in_ECX + 0x50);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("Range",in_ECX + 0x54);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("Granularity",in_ECX + 0x58);
  ___TransferMultiMap_JVCCharString___CPersistContext__QAEXPBDAAV__multimap_VCCharString__JU__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__J_std___3__std___Z
            ("AtmosGroupMap",in_ECX + 0x5c);
  ___Transfer__N_CPersistContext__QAEXPBDAA_N_Z("PlayOnlyWhenInside",in_ECX + 0x70);
  ___Transfer__N_CPersistContext__QAEXPBDAA_N_Z("PlayOnlyWhenOutside",in_ECX + 0x72);
  ___Transfer__N_CPersistContext__QAEXPBDAA_N_Z("PlayQuietlyWhenInside",in_ECX + 0x71);
  ___Transfer__N_CPersistContext__QAEXPBDAA_N_Z("PlayQuietlyWhenOutside",in_ECX + 0x73);
  return;
}



//=== CCameraModeDef @ 01938cb5 ===

/* [ported from ego_r via strfp] */

void _Transfer_CCameraModeDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined1 local_f;
  undefined1 local_e;
  undefined1 local_d;
  undefined4 local_c;
  undefined4 local_8;
  
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("CameraMode",in_ECX + 0x44);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("StringLengthXY",in_ECX + 0x48);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("CageRadius",in_ECX + 0x4c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("HeightOffsetZ",in_ECX + 0x50);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("LookOffsetZ",in_ECX + 0x54);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z(&DAT_0403e2ec,in_ECX + 0x58);
  local_8 = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("RollAngle",in_ECX + 0x5c,&local_8);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("TransitionInFrames",in_ECX + 0x60);
  local_c = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("LazyLookAtPosWeight",in_ECX + 100,&local_c);
  local_d = 1;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("RaiseUpWhenClose",in_ECX + 0x68,&local_d);
  local_e = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("StretchAroundHero",in_ECX + 0x69,&local_e);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("ZTargetBaseOffset",in_ECX + 0x6c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("ZTargetLookAtTargetOffset",in_ECX + 0x70);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("ZTargetStringLengthToObservedThing",in_ECX + 0x74);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z
            ("ZTargetStringLengthPerpendicularToObservedThing",in_ECX + 0x78);
  local_f = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z
            ("PerformExtendedOcclusionTests",in_ECX + 0x7c,&local_f);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("XAxisDisplacementCoupling",in_ECX + 0x80);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("XAxisVelocityDamping",in_ECX + 0x84);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("XAxisAttractorCoupling",in_ECX + 0x88);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("XAxisAttractorFallOff",in_ECX + 0x8c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("YAxisDisplacementCoupling",in_ECX + 0x90);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("YAxisVelocityDamping",in_ECX + 0x94);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("YAxisAttractorCoupling",in_ECX + 0x98);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("YAxisAttractorFallOff",in_ECX + 0x9c);
  ___Transfer__N_CPersistContext__QAEXPBDAA_N_Z("ProjectileCameraFOVZoom",in_ECX + 0xa0);
  ___Transfer__N_CPersistContext__QAEXPBDAA_N_Z("ProjectileCameraZoomToggle",in_ECX + 0xa1);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z
            ("ProjectileCameraDefaultTranslationalZoomOffset",in_ECX + 0xa4);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z
            ("ProjectileCameraInitialPhiOffsetForCamera",in_ECX + 0xa8);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z
            ("ProjectileCameraInitialHeightParamForCamera",in_ECX + 0xac);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("PhiRotationCouple",in_ECX + 0xb0);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("ThetaRotationCouple",in_ECX + 0xb4);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("DollyLength",in_ECX + 0xb8);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("NumberOfFramesToAverageInputsOver",in_ECX + 0xbc);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("DollyLengthBlendResistance",in_ECX + 0xc0);
  local_14 = 0x41200000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("ThetaAngleLimitUp",in_ECX + 0xc4,&local_14);
  local_18 = 0x42200000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("ThetaAngleLimitDown",in_ECX + 200,&local_18);
  local_1c = 0x3f000000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("SecondsToReset",in_ECX + 0xcc,&local_1c);
  local_20 = 0x3ccccccd;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z
            ("ViewHeroXYRotationMaxVelocity",in_ECX + 0xd4,&local_20);
  local_24 = 0x3ca3d70a;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z
            ("ViewHeroHeightParamMaxVelocity",in_ECX + 0xd8,&local_24);
  local_28 = 0x3fc00000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z
            ("ViewHeroXYOffsetMultiplier",in_ECX + 0xd0,&local_28);
  local_2c = 0x3f000000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z
            ("ViewHeroGroundHeightOffset",in_ECX + 0xdc,&local_2c);
  return;
}



//=== CHeroAbilityDef @ 019356a2 ===

/* [ported from ego_r via strfp] */

void _Transfer_CHeroAbilityDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  undefined4 uStack_8;
  
  uStack_8 = in_ECX;
  ___Transfer_W4EHeroAbility___CPersistContext__QAEXPBDAAW4EHeroAbility___Z("Ability",in_ECX + 0x44)
  ;
  ___Transfer_W4EHeroExperienceStatCategory___CPersistContext__QAEXPBDAAW4EHeroExperienceStatCategory___Z
            (&DAT_0403cef8,in_ECX + 0x48);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("IconGraphicIndex",in_ECX + 0x4c);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("IconEffectIndex",in_ECX + 0x50);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("IconActiveEffectIndex",in_ECX + 0x54);
  ___Transfer_VC2DVector___CPersistContext__QAEXPBDAAVC2DVector___Z
            ("IconEffectOffset",in_ECX + 0x58);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z(&DAT_0403ce98,in_ECX + 0x60);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("Description",in_ECX + 100);
  ___TransferVector_J_CPersistContext__QAEXPBDAAV__vector_JV__allocator_J_std___std___Z
            ("StaminaCost",in_ECX + 0x80);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("StatExperienceGainedOnUse",in_ECX + 0x94);
  ___TransferVector_J_CPersistContext__QAEXPBDAAV__vector_JV__allocator_J_std___std___Z
            ("ExperienceCostsToUpgrade",in_ECX + 0x98);
  ___TransferVector_J_CPersistContext__QAEXPBDAAV__vector_JV__allocator_J_std___std___Z
            ("LevDescription",in_ECX + 0x68);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("ABXYPriority",in_ECX + 0xac);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MoralityCostFactor",in_ECX + 0xb0);
  uStack_8 = CONCAT13(1,(undefined3)uStack_8);
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("Aggressive",in_ECX + 0xb4,(int)&uStack_8 + 3);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("MaxedOutDescription",in_ECX + 0x7c);
  ___Transfer_VCDefIndex___CPersistContext__QAEXPBDAAVCDefIndex___Z("DummyObject",in_ECX + 0xb8);
  return;
}



//=== CEngineThemeGroupDef @ 0191e286 ===

/* [ported from ego_r via strfp] */

void _Transfer_CEngineThemeGroupDef__UAEXAAVCPersistContext___Z(void)

{
  return;
}



//=== CMessageEventDef @ 01931245 ===

/* [ported from ego_r via strfp] */

void _Transfer_CMessageEventDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("MaxSeenRadius",in_ECX + 0x44);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("MaxHeardRadius",in_ECX + 0x48);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("Lifespan",in_ECX + 0x4c);
  ___Transfer_W4EMessageEventType___CPersistContext__QAEXPBDAAW4EMessageEventType___Z
            (&DAT_04032afc,in_ECX + 0x54);
  ___Transfer_W4EReactionSpeechType___CPersistContext__QAEXPBDAAW4EReactionSpeechType___Z
            ("ReactionSpeechType",in_ECX + 0x50);
  return;
}



//=== COpinionReactionMaskDef @ 0193b908 ===

/* WARNING: Removing unreachable block (ram,0x0193b95a) */
/* [ported from ego_r via strfp] */

void _Transfer_COpinionReactionMaskDef__UAEXAAVCPersistContext___Z(void)

{
  code *pcVar1;
  char cVar2;
  int *piVar3;
  BOOL BVar4;
  int in_ECX;
  undefined1 *puVar5;
  undefined1 *puVar6;
  char *pcStack_74;
  char *pcStack_70;
  wchar_t *pwStack_6c;
  undefined4 local_68;
  int local_64;
  undefined1 local_60 [12];
  undefined1 *local_54;
  undefined1 local_50 [8];
  undefined1 local_48 [8];
  char local_40 [10];
  char local_36;
  undefined1 local_35;
  undefined1 local_34 [12];
  undefined1 local_28 [12];
  undefined1 local_1c [12];
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03c1f098;
  local_10 = ExceptionList;
  local_35 = 1;
  pwStack_6c = (wchar_t *)&local_35;
  pcStack_70 = (char *)(in_ECX + 0x41);
  pcStack_74 = "ReactionEnabledDefault";
  ExceptionList = &local_10;
  local_64 = in_ECX;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z();
  pwStack_6c = (wchar_t *)(local_64 + 0x44);
  pcStack_70 = "ReactionEnabled";
  pcStack_74 = (char *)0x193b956;
  ___TransferMap__NW4EOpinionReactionType___CPersistContext__QAEXPBDAAV__map_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__std___Z
            ();
  pwStack_6c = (wchar_t *)local_1c;
  pcStack_70 = (char *)0x193b96e;
  _begin____Tree_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std___2_XZ
            ();
  local_8 = 0;
  pwStack_6c = (wchar_t *)local_1c;
  pcStack_70 = (char *)0x193b97e;
  __0___Tree_iterator_V___Tree_val_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std___std__QAE_ABV01__Z
            ();
  local_8._0_1_ = 1;
  pwStack_6c = (wchar_t *)local_28;
  pcStack_70 = (char *)0x193b991;
  _end____Tree_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std___2_XZ
            ();
  local_8._0_1_ = 2;
  do {
    pwStack_6c = (wchar_t *)local_28;
    pcStack_70 = (char *)0x193b9af;
    cVar2 = __9___Tree_const_iterator_V___Tree_val_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std___std__QBE_NABV01__Z
                      ();
    if (cVar2 == '\0') {
      local_8._0_1_ = 1;
      pwStack_6c = L"䗆ü䶍\xe8d0嗥￦䶃￼䶍\xe8e8嗙￦䶋擴ඉ";
      __1___Tree_iterator_V___Tree_val_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std___std__QAE_XZ
                ();
      local_8 = (uint)local_8._1_3_ << 8;
      pwStack_6c = L"䶃￼䶍\xe8e8嗙￦䶋擴ඉ";
      __1___Tree_iterator_V___Tree_val_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std___std__QAE_XZ
                ();
      local_8 = 0xffffffff;
      pwStack_6c = L"䶋擴ඉ";
      __1___Tree_iterator_V___Tree_val_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std___std__QAE_XZ
                ();
      ExceptionList = local_10;
      return;
    }
    pwStack_6c = L"䶍\xe8e8ꯀ￪㢃༒좍";
    __E___Tree_iterator_V___Tree_val_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std___std__QAEAAV01_XZ
              ();
    pwStack_6c = L"㢃༒좍";
    piVar3 = (int *)__C___Tree_iterator_V___Tree_val_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std___std__QBEPAU__pair___CBW4EOpinionReactionType___N_1_XZ
                              ();
    if (*piVar3 < 0x12) {
      pwStack_6c = 
      L"e:\\fabletlc\\fable tlc build repository\\bbbprojects\\fable1_5mainpc\\fablelib\\defs\\tc_opinion_of_hero_def.hpp"
      ;
      pcStack_70 = (char *)0x193b9e0;
      __0CWideString__QAE_PB_W_Z();
      local_8._0_1_ = 3;
      pwStack_6c = (wchar_t *)0xffffffff;
      pcStack_70 = 
      "Opinion reaction masks cannot be used to configure attitudes!  Use the opinion personality defs instead, dude!!!"
      ;
      pcStack_74 = (char *)0x193b9f3;
      __0CCharString__QAE_PBDJ_Z();
      local_8._0_1_ = 4;
      pwStack_6c = (wchar_t *)0xffffffff;
      pcStack_70 = "";
      pcStack_74 = (char *)0x193ba06;
      __0CCharString__QAE_PBDJ_Z();
      local_8._0_1_ = 5;
      pwStack_6c = (wchar_t *)0x3;
      pcStack_70 = (char *)0x1d;
      pcStack_74 = local_40;
      puVar6 = local_48;
      puVar5 = local_50;
      _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar5,puVar6);
      _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
      cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                        (puVar5,puVar6);
      local_36 = '\x01' - (cVar2 != '\x01');
      local_8._0_1_ = 4;
      pwStack_6c = (wchar_t *)0x193ba45;
      __1CCharString__QAE_XZ();
      local_8._0_1_ = 3;
      pwStack_6c = (wchar_t *)0x193ba51;
      __1CCharString__QAE_XZ();
      local_8._0_1_ = 2;
      pwStack_6c = (wchar_t *)0x193ba5d;
      __1CWideString__QAE_XZ();
      if (local_36 != '\0') {
        pwStack_6c = (wchar_t *)0x193ba6b;
        BVar4 = IsDebuggerPresent();
        if (BVar4 != 0) {
          pcVar1 = (code *)swi(3);
          (*pcVar1)();
          return;
        }
      }
      local_54 = (undefined1 *)&pcStack_74;
      __0___Tree_const_iterator_V___Tree_val_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std___std__QAE_ABV01__Z
                (local_1c);
      local_68 = _erase____Tree_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std___2_V___Tree_const_iterator_V___Tree_val_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std___2__Z
                           (local_60);
      pwStack_6c = (wchar_t *)0x193ba9b;
      __1___Tree_iterator_V___Tree_val_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std___std__QAE_XZ
                ();
    }
    pwStack_6c = (wchar_t *)local_34;
    pcStack_70 = (char *)0x193b9a3;
    __4___Tree_iterator_V___Tree_val_V___Tmap_traits_W4EOpinionReactionType___NU__less_W4EOpinionReactionType___std__V__allocator_U__pair___CBW4EOpinionReactionType___N_std___3__0A__std___std___std__QAEAAV01_ABV01__Z
              ();
  } while( true );
}



//=== CPlayerInventoryDef @ 0191f2c8 ===

/* [ported from ego_r via strfp] */

void _Transfer_CPlayerInventoryDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  ___TransferVector_J_CPersistContext__QAEXPBDAAV__vector_JV__allocator_J_std___std___Z
            (&DAT_04033844,in_ECX + 0x44);
  ___TransferVector_J_CPersistContext__QAEXPBDAAV__vector_JV__allocator_J_std___std___Z
            ("MaxInSlot",in_ECX + 0x58);
  ___TransferVector_J_CPersistContext__QAEXPBDAAV__vector_JV__allocator_J_std___std___Z
            ("InitialAmount",in_ECX + 0x6c);
  ___TransferVector__N_CPersistContext__QAEXPBDAAV__vector__NV__allocator__N_std___std___Z
            ("IsUsable",in_ECX + 0x80);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("InitialSelectedSlot",in_ECX + 0x9c);
  return;
}



//=== CPhysicsDef @ 01930fa6 ===

/* [ported from ego_r via strfp] */

void _Transfer_CPhysicsDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  undefined1 local_1d;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_8 = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("Diameter",in_ECX + 0x28,&local_8);
  local_c = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("Friction",in_ECX + 0x2c,&local_c);
  local_10 = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("Elasticity",in_ECX + 0x30,&local_10);
  local_14 = 0x16;
  ___Transfer_J_CPersistContext__QAEXPBDAAJABJ_Z("InteractionFlags",in_ECX + 0x3c,&local_14);
  local_18 = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("AirResistance",in_ECX + 0x34,&local_18);
  local_1c = 0x42480000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z(&DAT_0403b340,in_ECX + 0x38,&local_1c);
  local_1d = 1;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("PushableByHero",in_ECX + 0x40,&local_1d);
  return;
}



//=== CTargetingDef @ 0193b28d ===

/* [ported from ego_r via strfp] */

void _Transfer_CTargetingDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined1 local_5;
  
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z(&DAT_04032afc,in_ECX + 0x28);
  local_5 = 1;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("UseGlowTargeting",in_ECX + 0x2c,&local_5);
  local_c = 0x42b40000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("TargetArc",in_ECX + 0x30,&local_c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("TargetArcPCThirdPersonAimingMode",in_ECX + 0x34);
  ___Transfer__N_CPersistContext__QAEXPBDAA_N_Z("TargetingBasePosFromCamera",in_ECX + 0x38);
  ___Transfer__N_CPersistContext__QAEXPBDAA_N_Z("TargetingFacingDirFromCamera",in_ECX + 0x39);
  ___Transfer__N_CPersistContext__QAEXPBDAA_N_Z("TargetingPCFacingDirFromCamera",in_ECX + 0x3a);
  ___Transfer__N_CPersistContext__QAEXPBDAA_N_Z("RejectTargetsBehindIfRunning",in_ECX + 0x3b);
  ___Transfer__N_CPersistContext__QAEXPBDAA_N_Z("AcceptOnlyEnemiesBehind",in_ECX + 0x3c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("EnemyBehindTargetingRange",in_ECX + 0x40);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("FirstPersonTargetingAngleFalloff",in_ECX + 0x44);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("FirstPersonTargetingDistanceFalloff",in_ECX + 0x48);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("ThirdPersonTargetingAngleFalloff",in_ECX + 0x4c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("ThirdPersonTargetingDistanceFalloff",in_ECX + 0x50);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("ZLockTargetSelectAngleFalloff",in_ECX + 0x54);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("ZLockTargetSelectDistanceFalloff",in_ECX + 0x58);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z
            ("FirstPersonTargetingOverrideShortRangeFactor",in_ECX + 0x5c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z
            ("FirstPersonTargetingOverrideShortRangeFalloff",in_ECX + 0x60);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z
            ("ThirdPersonTargetingOverrideShortRangeFactor",in_ECX + 100);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z
            ("ThirdPersonTargetingOverrideShortRangeFalloff",in_ECX + 0x68);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z
            ("FirstPersonTargetingEnemyPreferenceFactor",in_ECX + 0x6c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z
            ("ThirdPersonTargetingEnemyPreferenceFactor",in_ECX + 0x70);
  ___TransferMap_MW4ETargetingType___CPersistContext__QAEXPBDAAV__map_W4ETargetingType__MU__less_W4ETargetingType___std__V__allocator_U__pair___CBW4ETargetingType__M_std___3__std___Z
            ("TargetingRanges",in_ECX + 0x74);
  local_10 = 0x3fa00000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("PreferPlayerWeight",in_ECX + 0x88,&local_10);
  local_14 = 0x40000000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z
            ("PreferNonCombatantsWeight",in_ECX + 0x8c,&local_14);
  local_18 = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("PreferLastAttackerWeight",in_ECX + 0x90,&local_18)
  ;
  local_1c = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z
            ("PreferCurrentBestEnemyWeight",in_ECX + 0x94,&local_1c);
  return;
}



//=== CWeaponDef @ 01ba53c5 ===

/* [ported from ego_r via strfp] */

void _Transfer_CWeaponDef__UAEXAAVCPersistContext___Z(void)

{
  undefined4 uVar1;
  int in_ECX;
  undefined1 local_51;
  undefined4 local_50;
  undefined1 local_4b;
  undefined1 local_4a;
  undefined1 local_49 [17];
  undefined4 local_38;
  undefined1 local_31;
  undefined4 local_30;
  undefined4 local_2c;
  undefined1 local_25;
  undefined4 local_24 [3];
  undefined4 local_18;
  undefined4 local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03c5f907;
  local_10 = ExceptionList;
  local_14 = 0;
  ExceptionList = &local_10;
  ___Transfer_W4EWeaponType___CPersistContext__QAEXPBDAAW4EWeaponType__ABW41__Z
            (&DAT_04032afc,in_ECX + 0x28,&local_14);
  ___Transfer_W4EWeaponClass___CPersistContext__QAEXPBDAAW4EWeaponClass___Z("Class",in_ECX + 0x2c);
  local_18 = 0;
  ___Transfer_K_CPersistContext__QAEXPBDAAKABK_Z("Property",in_ECX + 0x30,&local_18);
  uVar1 = __0CCharString__QAE_PBDJ_Z("SWORD",0xffffffff);
  local_8 = 0;
  ___Transfer_VCCharString___CPersistContext__QAEXPBDAAVCCharString__ABV1__Z
            ("AnimationGroup",in_ECX + 0x34,uVar1);
  local_8 = 0xffffffff;
  __1CCharString__QAE_XZ();
  local_24[0] = 0xffffffff;
  ___Transfer_J_CPersistContext__QAEXPBDAAJABJ_Z("SheatheObject",in_ECX + 0x3c,local_24);
  local_25 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z
            ("SheatheObjectAlwaysVisible",in_ECX + 0x40,&local_25);
  local_2c = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("AnimationSpeedVal",in_ECX + 0x44,&local_2c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("Damage",in_ECX + 0x48);
  local_30 = 0;
  ___Transfer_W4EDamageAttribute___CPersistContext__QAEXPBDAAW4EDamageAttribute__ABW41__Z
            ("DamageType",in_ECX + 0x4c,&local_30);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("Explosion",in_ECX + 0x50);
  ___Transfer_VCObjectAugmentationParticleSet___CPersistContext__QAEXPBDAAVCObjectAugmentationParticleSet___Z
            ("FlourishParticles",in_ECX + 0x54);
  ___Transfer_VCWeaponTrailGraphicSet___CPersistContext__QAEXPBDAAVCWeaponTrailGraphicSet___Z
            ("FlourishSpecialWeaponTrail",in_ECX + 0x90);
  ___TransferMap_VCWeaponTrailGraphicSet__W4EObjectAugmentationType___CPersistContext__QAEXPBDAAV__map_W4EObjectAugmentationType__VCWeaponTrailGraphicSet__U__less_W4EObjectAugmentationType___std__V__allocator_U__pair___CBW4EObjectAugmentationType__VCWeaponTrailGraphicSet___std___4__std___Z
            ("WeaponTrails",in_ECX + 0x98);
  local_31 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("UseAnalogueLoading",in_ECX + 0xac,&local_31);
  local_38 = 0xffffffff;
  ___Transfer_J_CPersistContext__QAEXPBDAAJABJ_Z("MissileLoadCarrySlot",in_ECX + 0xb0,&local_38);
  uVar1 = __0CCharString__QAE_PBDJ_Z("weapon_pos_a",0xffffffff);
  local_8 = 1;
  ___Transfer_VCCharString___CPersistContext__QAEXPBDAAVCCharString__ABV1__Z
            ("ProjectileWeaponHelperNameTip",in_ECX + 0xb4,uVar1);
  local_8 = 0xffffffff;
  __1CCharString__QAE_XZ();
  uVar1 = __0CCharString__QAE_PBDJ_Z("weapon_pos_b",0xffffffff);
  local_8 = 2;
  ___Transfer_VCCharString___CPersistContext__QAEXPBDAAVCCharString__ABV1__Z
            ("ProjectileWeaponHelperNameBase",in_ECX + 0xbc,uVar1);
  local_8 = 0xffffffff;
  __1CCharString__QAE_XZ();
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z(&DAT_04097e38,in_ECX + 0xc4);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z(&DAT_04097e30,in_ECX + 200);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("TargetingRange",in_ECX + 0xcc);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("TargetingFOV",in_ECX + 0xd0);
  ___TransferMap_JW4EObjectAugmentationType___CPersistContext__QAEXPBDAAV__map_W4EObjectAugmentationType__JU__less_W4EObjectAugmentationType___std__V__allocator_U__pair___CBW4EObjectAugmentationType__J_std___3__std___Z
            ("ArrowTrails",in_ECX + 0xd4);
  ___Transfer_VCFloatRange___CPersistContext__QAEXPBDAAVCFloatRange___Z
            ("ReloadSpeedRange",in_ECX + 0xe8);
  local_49[0] = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z
            ("ProjectileWeaponAutoReload",in_ECX + 0xf0,local_49);
  local_4a = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z
            ("ProjectileWeaponStartLoaded",in_ECX + 0xf1,&local_4a);
  local_4b = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z
            ("UseExtendedProjectileTrails",in_ECX + 0xf2,&local_4b);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("NextWeaponForBoss",in_ECX + 0xf4);
  local_50 = 0x42700000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("MeleeTargetingArc",in_ECX + 0xf8,&local_50);
  local_51 = 1;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("NeedsLoading",in_ECX + 0xfc,&local_51);
  ___Transfer_VCCharString___CPersistContext__QAEXPBDAAVCCharString___Z
            ("UnsheatheToCombatAnim",in_ECX + 0x100);
  ___Transfer_VCCharString___CPersistContext__QAEXPBDAAVCCharString___Z
            ("UnsheatheToNormalAnim",in_ECX + 0x108);
  ___Transfer_VCCharString___CPersistContext__QAEXPBDAAVCCharString___Z
            ("SheatheFromCombatAnim",in_ECX + 0x110);
  ___Transfer_VCCharString___CPersistContext__QAEXPBDAAVCCharString___Z
            ("SheatheFromNormalAnim",in_ECX + 0x118);
  ExceptionList = local_10;
  return;
}



//=== CContextSensitiveItemDef @ 0194b0a7 ===

/* [ported from ego_r via strfp] */

void _Transfer_CContextSensitiveItemDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  undefined4 local_8;
  
  local_8 = 0;
  ___Transfer_W4EContextSensitiveType___CPersistContext__QAEXPBDAAW4EContextSensitiveType__ABW41__Z
            (&DAT_04032afc,in_ECX + 0x28,&local_8);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("Priority",in_ECX + 0x2c);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("SlotIndex",in_ECX + 0x30);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("ExtraData1",in_ECX + 0x34);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("ExtraData2",in_ECX + 0x38);
  return;
}



//=== CHasNameDef @ 01bc19e6 ===

/* [ported from ego_r via strfp] */

void _Transfer_CHasNameDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("DefaultNameTag",in_ECX + 0x28);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z(&DAT_040a1c6c,in_ECX + 0x2c);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("Occupation",in_ECX + 0x30);
  return;
}



//=== CBuyableHouseDef @ 01b9d4f0 ===

/* [ported from ego_r via strfp] */

void _Transfer_CBuyableHouseDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  undefined1 local_15;
  undefined4 local_14;
  undefined1 local_d;
  undefined4 local_c;
  undefined1 local_6;
  undefined1 local_5;
  
  local_5 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("IsBuyable",in_ECX + 0x25,&local_5);
  local_6 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("InitiallyEmpty",in_ECX + 0x26,&local_6);
  ___TransferVector_J_CPersistContext__QAEXPBDAAV__vector_JV__allocator_J_std___std___Z
            ("Price",in_ECX + 0x28);
  ___TransferVector_J_CPersistContext__QAEXPBDAAV__vector_JV__allocator_J_std___std___Z
            (&DAT_04094e80,in_ECX + 0x3c);
  local_c = 2;
  ___Transfer_J_CPersistContext__QAEXPBDAAJABJ_Z("DaysBetweenRentPayments",in_ECX + 0x50,&local_c);
  local_d = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("IsScripted",in_ECX + 0x58,&local_d);
  local_14 = 6;
  ___Transfer_J_CPersistContext__QAEXPBDAAJABJ_Z("MaxRentBags",in_ECX + 0x54,&local_14);
  local_15 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z
            ("IsBuyableWithoutSignpost",in_ECX + 0x59,&local_15);
  return;
}



//=== CShopDef @ 01ba75db ===

/* [ported from ego_r via strfp] */

void _Transfer_CShopDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  undefined1 local_3d;
  undefined4 local_3c;
  undefined1 local_37;
  undefined1 local_36;
  undefined1 local_35;
  undefined4 local_34;
  undefined1 local_2d;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_8 = 0;
  ___Transfer_K_CPersistContext__QAEXPBDAAKABK_Z(&DAT_0403ce98,in_ECX + 0x28,&local_8);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("InitialHawkingPhrase",in_ECX + 0x2c);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("ReplyToRequestPhrase",in_ECX + 0x30);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("CustomerFollowPhrase",in_ECX + 0x34);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("ThankingPhrase",in_ECX + 0x38);
  ___Transfer_VCCharString___CPersistContext__QAEXPBDAAVCCharString___Z
            ("ShopkeeperMeshType",in_ECX + 0x3c);
  ___TransferVectorOfSubComponents_VCShopItemDef___CPersistContext__QAEXPBDAAV__vector_VCShopItemDef__V__allocator_VCShopItemDef___std___std___Z
            ("DefaultStock",in_ECX + 0x44);
  ___TransferVectorOfSubComponents_VCShopItemDef___CPersistContext__QAEXPBDAAV__vector_VCShopItemDef__V__allocator_VCShopItemDef___std___std___Z
            ("StockCategoryDefaults",in_ECX + 0x58);
  local_c = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("BuyPriceMultiplier",in_ECX + 0x6c,&local_c);
  local_10 = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("SellPriceMultiplier",in_ECX + 0x70,&local_10);
  local_14 = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z
            ("AttitudeBuyPriceMultiplierDefault",in_ECX + 0x74,&local_14);
  ___TransferMap_MW4EOpinionAttitudeType___CPersistContext__QAEXPBDAAV__map_W4EOpinionAttitudeType__MU__less_W4EOpinionAttitudeType___std__V__allocator_U__pair___CBW4EOpinionAttitudeType__M_std___3__std___Z
            ("AttitudeBuyPriceMultiplier",in_ECX + 0x78);
  local_18 = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z
            ("AttitudeSellPriceMultiplierDefault",in_ECX + 0x8c,&local_18);
  ___TransferMap_MW4EOpinionAttitudeType___CPersistContext__QAEXPBDAAV__map_W4EOpinionAttitudeType__MU__less_W4EOpinionAttitudeType___std__V__allocator_U__pair___CBW4EOpinionAttitudeType__M_std___3__std___Z
            ("AttitudeSellPriceMultiplier",in_ECX + 0x90);
  local_1c = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("MaxStockChangePerSell",in_ECX + 0xa4,&local_1c);
  local_20 = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("MaxStockChangePerBuy",in_ECX + 0xa8,&local_20);
  local_24 = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("MaxStockRevertPerDay",in_ECX + 0xac,&local_24);
  local_28 = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z
            ("PriceReductionFractionPerItem",in_ECX + 0xb0,&local_28);
  local_2c = 0x3f19999a;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z
            ("MaxPriceReductionFractionDueToStockCount",in_ECX + 0xb4,&local_2c);
  local_2d = 1;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z
            ("IsStockDisplayPermanent",in_ECX + 0xbc,&local_2d);
  local_34 = 0;
  ___Transfer_K_CPersistContext__QAEXPBDAAKABK_Z("PreferredShopkeeper",in_ECX + 0xb8,&local_34);
  local_35 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("IsTattooShop",in_ECX + 0xbe,&local_35);
  local_36 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("IsBarberShop",in_ECX + 0xbd,&local_36);
  local_37 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("IsTitleShop",in_ECX + 0xbf,&local_37);
  local_3c = 0x3f8ccccd;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z
            ("MinPriceMultiplierForItemToBeWanted",in_ECX + 0xc4,&local_3c);
  local_3d = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("AlwaysOpen",in_ECX + 0xc0,&local_3d);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MaxPriceToDisplay",in_ECX + 200);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MinTimeToSteal",in_ECX + 0xcc);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MaxTimeToSteal",in_ECX + 0xd0);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("MinGoldForDonation",in_ECX + 0xd4);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("MaxGoldForDonation",in_ECX + 0xd8);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MaxMoralityForDonation",in_ECX + 0xdc);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("LogarithmicPower",in_ECX + 0xe0);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("NumDaysToKeepSpecialItem",in_ECX + 0xe4);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("MinAmountBeforeSpecialItem",in_ECX + 0xe8);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("ProbabilityOfSpecialItemPerDay",in_ECX + 0xec);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("SpecialItemPriceMult",in_ECX + 0xf0);
  return;
}



//=== CPerceivedThingDef @ 01b9fdea ===

/* [ported from ego_r via strfp] */

void _Transfer_CPerceivedThingDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_8 = 0x42b40000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("FieldOfView",in_ECX + 0x28,&local_8);
  local_c = 0x41500000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("ExtendedSightRadius",in_ECX + 0x34,&local_c);
  local_10 = 0x41000000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("SightRadius",in_ECX + 0x30,&local_10);
  local_14 = 0x41200000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("SoundRadius",in_ECX + 0x38,&local_14);
  local_18 = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("SmellRadius",in_ECX + 0x3c,&local_18);
  local_1c = 0;
  ___Transfer_W4EPerceivedThingType___CPersistContext__QAEXPBDAAW4EPerceivedThingType__ABW41__Z
            (&DAT_04032afc,in_ECX + 0x40,&local_1c);
  local_20 = 0x41880000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("AwarenessTimer",in_ECX + 0x44,&local_20);
  local_24 = 0x40a00000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("GiveUpChaseRadius",in_ECX + 0x2c,&local_24);
  local_28 = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z
            ("DayTimeVisibilityMultiplier",in_ECX + 0x48,&local_28);
  local_2c = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z
            ("NightTimeVisibilityMultiplier",in_ECX + 0x4c,&local_2c);
  return;
}



//=== CSummonableCreatureDef @ 01baeb46 ===

/* [ported from ego_r via strfp] */

void _Transfer_CSummonableCreatureDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z(&DAT_0409b11c,in_ECX + 0x28);
  ___Transfer_VCDefString___CPersistContext__QAEXPBDAAVCDefString___Z("SummonedBrain",in_ECX + 0x2c)
  ;
  return;
}



//=== CAugmentationDef @ 0194e7ff ===

/* [ported from ego_r via strfp] */

void _Transfer_CAugmentationDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z(&DAT_0403ce98,in_ECX + 0x28);
  local_8 = 0;
  ___Transfer_W4EObjectAugmentationType___CPersistContext__QAEXPBDAAW4EObjectAugmentationType__ABW41__Z
            (&DAT_04032afc,in_ECX + 0x2c,&local_8);
  local_c = 0;
  ___Transfer_K_CPersistContext__QAEXPBDAAKABK_Z("InventoryGraphic",in_ECX + 0x30,&local_c);
  local_10 = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("DamageMultiplier",in_ECX + 0x34,&local_10);
  local_14 = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("ExperienceMultiplier",in_ECX + 0x38,&local_14);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("FrequencyToAddHealthInSeconds",in_ECX + 0x3c);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("HealthToAddPerIncrease",in_ECX + 0x40);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("FrequencyToAddStaminaInSeconds",in_ECX + 0x44);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("StaminaToAddPerIncrease",in_ECX + 0x48);
  return;
}



//=== CTavernGameDef @ 01bb53d1 ===

/* [ported from ego_r via strfp] */

void _Transfer_CTavernGameDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("Banter",in_ECX + 0x28);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("Greeting",in_ECX + 0x2c);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("OptionsInitial",in_ECX + 0x30);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("OptionsSubsequent",in_ECX + 0x34);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("Instructions",in_ECX + 0x38);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("InstructionsPC",in_ECX + 0x3c);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("Betting",in_ECX + 0x40);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z(&DAT_0409de3c,in_ECX + 0x44);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("ReactionWin",in_ECX + 0x48);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("ReactionLose",in_ECX + 0x4c);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("ReactionDraw",in_ECX + 0x50);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("ReactionWinNewBestScore",in_ECX + 0x54);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("FarewellInitial",in_ECX + 0x58);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("FarewellSubsequent",in_ECX + 0x5c);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("WinRoundPhrase",in_ECX + 0x60);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("OutOfTimePhrase",in_ECX + 100);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("NoMoney",in_ECX + 0x68);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("GreetingForward",in_ECX + 0x6c);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("OptionsForward",in_ECX + 0x70);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("OptionsBack",in_ECX + 0x74);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("OptionsAlternative",in_ECX + 0x78);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("InstructionsBack",in_ECX + 0x7c);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BettingForward",in_ECX + 0x80);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BettingBack",in_ECX + 0x84);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("ReactionForward",in_ECX + 0x88);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("FarewellForward",in_ECX + 0x8c);
  ___Transfer_VCDefString___CPersistContext__QAEXPBDAAVCDefString___Z("CameraName",in_ECX + 0x90);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BoxGraphicL",in_ECX + 0x94);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BoxGraphicC",in_ECX + 0x98);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BoxGraphicR",in_ECX + 0x9c);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("ClickToContinue",in_ECX + 0xa0);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("WinPhrase",in_ECX + 0xa4);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("LosePhrase",in_ECX + 0xa8);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("DrawPhrase",in_ECX + 0xac);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("NewGame",in_ECX + 0xb0);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BestScore",in_ECX + 0xb4);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("CurrentScore",in_ECX + 0xb8);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("RequiredScore",in_ECX + 0xbc);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("AdditionalInfo",in_ECX + 0xc0);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BlackjackBusted",in_ECX + 0xc4);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BlackjackSplit",in_ECX + 200);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BlackjackDouble",in_ECX + 0xcc);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BlackjackHit",in_ECX + 0xd0);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BlackjackStand",in_ECX + 0xd4);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BlackjackDealerTakesCard",in_ECX + 0xd8);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BlackjackSplitGUI",in_ECX + 0xdc);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BlackjackDoubleGUI",in_ECX + 0xe0);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BlackjackHitGUI",in_ECX + 0xe4);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("BlackjackStandGUI",in_ECX + 0xe8);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z(&DAT_0409db34,in_ECX + 0xec);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("PlayersMoney",in_ECX + 0xf0);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("TotalWinnings",in_ECX + 0xf4);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("Continue",in_ECX + 0xf8);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z(&DAT_0409db00,in_ECX + 0xfc);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z(&DAT_0409dafc,in_ECX + 0x100);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z(&DAT_0409daf8,in_ECX + 0x104);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("PrizeGiven",in_ECX + 0x108);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("MoneyBagGraphic",in_ECX + 0x10c);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("MinBet",in_ECX + 0x110);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("MaxBet",in_ECX + 0x114);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("BetIncrement",in_ECX + 0x118);
  ___Transfer_VCDefString___CPersistContext__QAEXPBDAAVCDefString___Z("ScoreFont",in_ECX + 0x11c);
  ___Transfer_VCDefString___CPersistContext__QAEXPBDAAVCDefString___Z("TargetFont",in_ECX + 0x120);
  ___Transfer_VCDefString___CPersistContext__QAEXPBDAAVCDefString___Z("StatsFont",in_ECX + 0x124);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("ScoreX",in_ECX + 0x128);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("ScoreY",in_ECX + 300);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("TargetX",in_ECX + 0x130);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("TargetY",in_ECX + 0x134);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("BestX",in_ECX + 0x138);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("BestY",in_ECX + 0x13c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("AdditionalX",in_ECX + 0x140);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("AdditionalY",in_ECX + 0x144);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z(&DAT_0409da2c,in_ECX + 0x148);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z(&DAT_0409da24,in_ECX + 0x14c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MoneyX",in_ECX + 0x150);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MoneyY",in_ECX + 0x154);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("WinningsX",in_ECX + 0x158);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("WinningsY",in_ECX + 0x15c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MainBetX",in_ECX + 0x160);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MainBetY",in_ECX + 0x164);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MainMoneyX",in_ECX + 0x168);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MainMoneyY",in_ECX + 0x16c);
  ___Transfer__N_CPersistContext__QAEXPBDAA_N_Z("BestScoreHigh",in_ECX + 0x170);
  ___TransferVectorOfSubComponents_VCPrizeScoreDef___CPersistContext__QAEXPBDAAV__vector_VCPrizeScoreDef__V__allocator_VCPrizeScoreDef___std___std___Z
            ("PrizeScores",in_ECX + 0x174);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("Prize",in_ECX + 0x188);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("PrizeRenown",in_ECX + 0x18c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MainGameScoreBoxX",in_ECX + 400);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MainGameScoreBoxY",in_ECX + 0x194);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MainGameScoreBoxWidthXbox",in_ECX + 0x198);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MainGameScoreBoxWidthPC",in_ECX + 0x19c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MainGameScoreBoxHeight",in_ECX + 0x1a0);
  ___Transfer__N_CPersistContext__QAEXPBDAA_N_Z("DisplayErrata",in_ECX + 0x1a4);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("PointerPhaseSpeed",in_ECX + 0x1a8);
  return;
}



//=== CDecapitationDef @ 01bb83a2 ===

/* [ported from ego_r via strfp] */

void _Transfer_CDecapitationDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("BodyParticleEffect",in_ECX + 0x28);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z(&DAT_0409e864,in_ECX + 0x2c);
  return;
}



//=== CShipDef @ 01bac1fc ===

/* [ported from ego_r via strfp] */

void _Transfer_CShipDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_8 = 1;
  ___Transfer_J_CPersistContext__QAEXPBDAAJABJ_Z("SightingFrequency",in_ECX + 0x28,&local_8);
  local_c = 0;
  ___Transfer_J_CPersistContext__QAEXPBDAAJABJ_Z("SightingOffset",in_ECX + 0x2c,&local_c);
  local_10 = 1;
  ___Transfer_J_CPersistContext__QAEXPBDAAJABJ_Z("DaysVisible",in_ECX + 0x30,&local_10);
  local_14 = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("Swell",in_ECX + 0x34,&local_14);
  local_18 = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("Pitch",in_ECX + 0x38,&local_18);
  local_1c = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z(&DAT_0409a3a4,in_ECX + 0x3c,&local_1c);
  local_20 = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("WaveLength",in_ECX + 0x40,&local_20);
  return;
}



//=== CShopItemDef @ 01ba72ce ===

/* [ported from ego_r via strfp] */

void _Transfer_CShopItemDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_8 = 0;
  ___Transfer_J_CPersistContext__QAEXPBDAAJABJ_Z(&DAT_04032afc,in_ECX + 0x28,&local_8);
  local_c = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("Quantity",in_ECX + 0x2c,&local_c);
  local_10 = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("PriceMultiplier",in_ECX + 0x30,&local_10);
  local_14 = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("MaxStock",in_ECX + 0x34,&local_14);
  local_18 = 0;
  ___Transfer_J_CPersistContext__QAEXPBDAAJABJ_Z("RestockPeriod",in_ECX + 0x38,&local_18);
  local_1c = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("RestockPeriodRandomness",in_ECX + 0x3c,&local_1c);
  local_20 = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("SalesPerDay",in_ECX + 0x40,&local_20);
  local_24 = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("SalesPerDayRandomness",in_ECX + 0x44,&local_24);
  return;
}



//=== CDegradableDef @ 01fea8a0 ===

/* [ported from ego_r via strfp] */

void _Transfer_CDegradableDef__UAEXAAVCPersistContext___Z(void)

{
  code *pcVar1;
  char cVar2;
  BOOL BVar3;
  undefined4 *puVar4;
  int in_ECX;
  undefined1 auStack_80 [4];
  undefined4 uStack_7c;
  undefined1 *puStack_78;
  undefined1 *puStack_74;
  char *pcStack_70;
  char *pcStack_6c;
  wchar_t *pwStack_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  int local_4c;
  undefined1 *local_44;
  undefined1 *local_40;
  undefined1 *local_38;
  undefined1 *local_34;
  byte local_2d;
  undefined1 local_2c [8];
  undefined1 local_24 [8];
  undefined1 local_1c [10];
  char local_12;
  undefined1 local_11;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03cd8e38;
  local_10 = ExceptionList;
  pwStack_68 = (wchar_t *)(in_ECX + 0x25);
  pcStack_6c = "Degradable";
  pcStack_70 = (char *)0x1fea8d2;
  ExceptionList = &local_10;
  local_4c = in_ECX;
  ___Transfer__N_CPersistContext__QAEXPBDAA_N_Z();
  pwStack_68 = (wchar_t *)(local_4c + 0x28);
  pcStack_6c = "GraphicType";
  pcStack_70 = (char *)0x1fea8e6;
  ___Transfer_W4EEngineGraphicType___CPersistContext__QAEXPBDAAW4EEngineGraphicType___Z();
  local_11 = 1;
  pwStack_68 = (wchar_t *)&local_11;
  pcStack_6c = (char *)(local_4c + 0x2c);
  pcStack_70 = "InitiallyBlocksNavigation";
  puStack_74 = (undefined1 *)0x1fea902;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z();
  pwStack_68 = (wchar_t *)(local_4c + 0x30);
  pcStack_6c = "Degradations";
  pcStack_70 = (undefined1 *)0x1fea916;
  ___TransferVector_VCDegradableInfo___CPersistContext__QAEXPBDAAV__vector_VCDegradableInfo__V__allocator_VCDegradableInfo___std___std___Z
            ();
  pwStack_68 = L"똏藐࿒岄\x01謀롅碃(蔏§";
  cVar2 = _IsParsingText_CPersistContext__QBE_NXZ();
  if (cVar2 != '\0') {
    if (*(int *)(local_4c + 0x28) == 0) {
      pwStack_68 = L"tc_degradable_def.cpp";
      pcStack_6c = (char *)0x1fea943;
      __0CWideString__QAE_PB_W_Z();
      local_8 = 0;
      pwStack_68 = (wchar_t *)0xffffffff;
      pcStack_6c = "";
      pcStack_70 = (char *)0x1fea959;
      __0CCharString__QAE_PBDJ_Z();
      local_8._0_1_ = 1;
      pwStack_68 = (wchar_t *)0xffffffff;
      pcStack_6c = "GraphicType != ENGINE_GRAPHIC_NULL";
      pcStack_70 = (char *)0x1fea96c;
      __0CCharString__QAE_PBDJ_Z();
      local_8._0_1_ = 2;
      pwStack_68 = (wchar_t *)0x1;
      pcStack_6c = (char *)0x57;
      pcStack_70 = local_1c;
      puStack_74 = local_24;
      puStack_78 = local_2c;
      uStack_7c = 0x1fea985;
      _GFGetSystemManager__YIPAVCSystemManager__XZ();
      uStack_7c = 0x1fea98c;
      _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
      uStack_7c = 0x1fea993;
      cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                        ();
      local_12 = '\x01' - (cVar2 != '\x01');
      local_8._0_1_ = 1;
      pwStack_68 = 
      L"䗆ü䶍\xe8e0硸ﾃ䗇￼\xffff跿\xe84d槨翹࿿営藲瓒＋怕뱀蔄瓀찁똏흅荐೬첋斉凐䶋莸チ擨蔢觿둅喋覴끕䗇ϼ"
      ;
      __1CCharString__QAE_XZ();
      local_8 = (uint)local_8._1_3_ << 8;
      pwStack_68 = 
      L"䗇￼\xffff跿\xe84d槨翹࿿営藲瓒＋怕뱀蔄瓀찁똏흅荐೬첋斉凐䶋莸チ擨蔢觿둅喋覴끕䗇ϼ"
      ;
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      pwStack_68 = 
      L"똏\xf255튅୴ᗿ䁠Ҽ삅Ŵ࿌䖶僗\xec83謌觌큥譑롍솃\xe830≤ﾅ䖉讴둕喉잰ﱅ\x03"
      ;
      __1CWideString__QAE_XZ();
      if (local_12 != '\0') {
        pwStack_68 = L"삅Ŵ࿌䖶僗\xec83謌觌큥譑롍솃\xe830≤ﾅ䖉讴둕喉잰ﱅ\x03";
        BVar3 = IsDebuggerPresent();
        if (BVar3 != 0) {
          pcVar1 = (code *)swi(3);
          (*pcVar1)();
          return;
        }
      }
    }
    pwStack_68 = (wchar_t *)(uint)local_2d;
    local_34 = (undefined1 *)&puStack_74;
    puStack_78 = (undefined1 *)&puStack_74;
    uStack_7c = 0x1fea9f6;
    local_54 = _end___vector_VCDegradableInfo__V__allocator_VCDegradableInfo___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCDegradableInfo__V__allocator_VCDegradableInfo___std___std___2_XZ
                         ();
    local_8 = 3;
    local_38 = auStack_80;
    local_50 = local_54;
    local_58 = _begin___vector_VCDegradableInfo__V__allocator_VCDegradableInfo___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCDegradableInfo__V__allocator_VCDegradableInfo___std___std___2_XZ
                         (auStack_80);
    local_8 = 0xffffffff;
    ___sort_V___Vector_iterator_V___Vector_val_VCDegradableInfo__V__allocator_VCDegradableInfo___std___std___std__VCIsHealth1GTHealth2_CDegradableDef___std__YIXV___Vector_iterator_V___Vector_val_VCDegradableInfo__V__allocator_VCDegradableInfo___std___std___0_0VCIsHealth1GTHealth2_CDegradableDef___Z
              ();
    pwStack_68 = *(wchar_t **)(local_4c + 0x28);
    pcStack_6c = (char *)0x1feaa38;
    puVar4 = (undefined4 *)__0CSetEngineGraphicType_CDegradableDef__QAE_W4EEngineGraphicType___Z();
    pwStack_68 = (wchar_t *)*puVar4;
    local_40 = (undefined1 *)&puStack_74;
    puStack_78 = (undefined1 *)&puStack_74;
    uStack_7c = 0x1feaa4f;
    local_60 = _end___vector_VCDegradableInfo__V__allocator_VCDegradableInfo___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCDegradableInfo__V__allocator_VCDegradableInfo___std___std___2_XZ
                         ();
    local_8 = 4;
    local_44 = auStack_80;
    local_5c = local_60;
    local_64 = _begin___vector_VCDegradableInfo__V__allocator_VCDegradableInfo___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCDegradableInfo__V__allocator_VCDegradableInfo___std___std___2_XZ
                         (auStack_80);
    local_8 = 0xffffffff;
    ___for_each_V___Vector_iterator_V___Vector_val_VCDegradableInfo__V__allocator_VCDegradableInfo___std___std___std__VCSetEngineGraphicType_CDegradableDef___std__YI_AVCSetEngineGraphicType_CDegradableDef__V___Vector_iterator_V___Vector_val_VCDegradableInfo__V__allocator_VCDegradableInfo___std___std___0_0V12__Z
              ();
  }
  ExceptionList = local_10;
  return;
}



//=== CBettingDef @ 01bc0388 ===

/* [ported from ego_r via strfp] */

void _Transfer_CBettingDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z(&DAT_0409da2c,in_ECX + 0x28);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z(&DAT_0409da24,in_ECX + 0x2c);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MoneyX",in_ECX + 0x30);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("MoneyY",in_ECX + 0x34);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("MinBet",in_ECX + 0x38);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("MaxBet",in_ECX + 0x3c);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("BetIncrement",in_ECX + 0x40);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z(&DAT_0409db34,in_ECX + 0x44);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("PlayersMoney",in_ECX + 0x48);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("Question",in_ECX + 0x4c);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("Continue",in_ECX + 0x50);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("Cancel",in_ECX + 0x54);
  return;
}



//=== CThingObjectDef @ 0191c3df ===

/* [ported from ego_r via strfp] */

void _Transfer_CThingObjectDef__UAEXAAVCPersistContext___Z(undefined4 param_1)

{
  char cVar1;
  int in_ECX;
  undefined1 local_e;
  undefined1 local_d;
  undefined1 local_c;
  undefined1 local_b;
  undefined1 local_a;
  byte local_9;
  byte local_8;
  byte local_7;
  byte local_6;
  byte local_5;
  
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  cVar1 = _IsBinary_CPersistContext__QBE_NXZ();
  if (cVar1 == '\0') {
    cVar1 = _IsSaving_CPersistContext__QBE_NXZ();
    if ((cVar1 == '\0') && (*(float *)(in_ECX + 0x58) == 0.0)) {
      *(undefined4 *)(in_ECX + 0x58) = 0x3f800000;
    }
  }
  ___Transfer_VCEngineGraphic___CPersistContext__QAEXPBDAAVCEngineGraphic___Z
            ("Graphic",in_ECX + 0x78);
  ___Transfer_M_CPersistContext__QAEXPBDAAM_Z("RotationTime",in_ECX + 0x88);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("Material",in_ECX + 0x8c);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("FloorMaterial",in_ECX + 0x90);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("OnDeathObject",in_ECX + 0x94);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("Property",in_ECX + 0x98);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("DieOffTimer",in_ECX + 0x9c);
  local_5 = *(byte *)(in_ECX + 0xa0) & 1;
  local_a = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("IsFragile",&local_5,&local_a);
  *(byte *)(in_ECX + 0xa0) = *(byte *)(in_ECX + 0xa0) & 0xfe | local_5 & 1;
  local_6 = *(byte *)(in_ECX + 0xa0) >> 1 & 1;
  local_b = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("IsClothing",&local_6,&local_b);
  *(byte *)(in_ECX + 0xa0) = *(byte *)(in_ECX + 0xa0) & 0xfd | (local_6 & 1) << 1;
  local_7 = *(byte *)(in_ECX + 0xa0) >> 2 & 1;
  local_c = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("IsSelfIlluminating",&local_7,&local_c);
  *(byte *)(in_ECX + 0xa0) = *(byte *)(in_ECX + 0xa0) & 0xfb | (local_7 & 1) << 2;
  local_8 = *(byte *)(in_ECX + 0xa0) >> 3 & 1;
  local_d = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("UseHighDetailQuadTree",&local_8,&local_d);
  *(byte *)(in_ECX + 0xa0) = *(byte *)(in_ECX + 0xa0) & 0xf7 | (local_8 & 1) << 3;
  local_9 = *(byte *)(in_ECX + 0xa0) >> 4 & 1;
  local_e = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("Electrocutable",&local_9,&local_e);
  *(byte *)(in_ECX + 0xa0) = *(byte *)(in_ECX + 0xa0) & 0xef | (local_9 & 1) << 4;
  return;
}



//=== CThingBuildingDef @ 0191ca19 ===

/* [ported from ego_r via strfp] */

void _Transfer_CThingBuildingDef__UAEXAAVCPersistContext___Z(undefined4 param_1)

{
  int in_ECX;
  undefined1 local_18;
  undefined1 local_17;
  undefined1 local_16;
  undefined1 local_15;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  byte local_8;
  byte local_7;
  byte local_6;
  byte local_5;
  
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("DieOffTimer",in_ECX + 0x78);
  ___Transfer_K_CPersistContext__QAEXPBDAAK_Z("SimBuildingDef",in_ECX + 0x7c);
  ___Transfer_VCEngineGraphic___CPersistContext__QAEXPBDAAVCEngineGraphic___Z
            ("Graphic",in_ECX + 0x80);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("WallMaterial",in_ECX + 0x90);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("FloorMaterial",in_ECX + 0x94);
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("CameraManagerSetIndex",in_ECX + 0xa4);
  local_c = 0;
  ___Transfer_W4EReverbEnvironmentType___CPersistContext__QAEXPBDAAW4EReverbEnvironmentType__ABW41__Z
            ("ReverbEnvironmentType",in_ECX + 0x98,&local_c);
  local_10 = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("ReverbEnvironmentLevel",in_ECX + 0x9c,&local_10);
  local_14 = 0x3f800000;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z("OutsideOcclusionLevel",in_ECX + 0xa0,&local_14);
  local_5 = *(byte *)(in_ECX + 0xa8) & 1;
  local_15 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("UseHighDetailQuadTree",&local_5,&local_15);
  *(byte *)(in_ECX + 0xa8) = *(byte *)(in_ECX + 0xa8) & 0xfe | local_5 & 1;
  local_6 = *(byte *)(in_ECX + 0xa8) >> 1 & 1;
  local_16 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("IsSelfIlluminating",&local_6,&local_16);
  *(byte *)(in_ECX + 0xa8) = *(byte *)(in_ECX + 0xa8) & 0xfd | (local_6 & 1) << 1;
  local_7 = *(byte *)(in_ECX + 0xa8) >> 2 & 1;
  local_17 = 0;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("IsPartOfHeroGuild",&local_7,&local_17);
  *(byte *)(in_ECX + 0xa8) = *(byte *)(in_ECX + 0xa8) & 0xfb | (local_7 & 1) << 2;
  local_8 = *(byte *)(in_ECX + 0xa8) >> 3 & 1;
  local_18 = 1;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("HasWeatherMask",&local_8,&local_18);
  *(byte *)(in_ECX + 0xa8) = *(byte *)(in_ECX + 0xa8) & 0xf7 | (local_8 & 1) << 3;
  return;
}



//=== CCombatSequenceDef @ 0194dc17 ===

/* [ported from ego_r via strfp] */

void _Transfer_CCombatSequenceDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  undefined1 local_9;
  undefined4 local_8;
  
  ___Transfer_J_CPersistContext__QAEXPBDAAJ_Z("ValidCombatZones",in_ECX + 0x44);
  ___Transfer_W4ECombatSequenceType___CPersistContext__QAEXPBDAAW4ECombatSequenceType___Z
            (&DAT_04032afc,in_ECX + 0x48);
  ___Transfer_W4ECombatSequenceInterruptionType___CPersistContext__QAEXPBDAAW4ECombatSequenceInterruptionType___Z
            ("InterruptionType",in_ECX + 0x4c);
  ___Transfer_W4ECombatSequenceIsValidType___CPersistContext__QAEXPBDAAW4ECombatSequenceIsValidType___Z
            ("IsValidType",in_ECX + 0x50);
  ___Transfer_W4ECombatSequenceOnStartModuleType___CPersistContext__QAEXPBDAAW4ECombatSequenceOnStartModuleType___Z
            ("OnStartType",in_ECX + 0x54);
  ___Transfer_W4ECombatSequenceOnStopModuleType___CPersistContext__QAEXPBDAAW4ECombatSequenceOnStopModuleType___Z
            ("OnStopType",in_ECX + 0x58);
  ___TransferVector_W4EActionRegisteredType___CPersistContext__QAEXPBDAAV__vector_W4EActionRegisteredType__V__allocator_W4EActionRegisteredType___std___std___Z
            ("ActionList",in_ECX + 0x5c);
  local_8 = 0;
  ___Transfer_M_CPersistContext__QAEXPBDAAMABM_Z
            ("SecondsToPredictTargetPosition",in_ECX + 0x70,&local_8);
  local_9 = 1;
  ___Transfer__N_CPersistContext__QAEXPBDAA_NAB_N_Z("PreferIfValid",in_ECX + 0x74,&local_9);
  return;
}



