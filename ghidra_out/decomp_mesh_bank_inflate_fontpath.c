INFO  DecompFuncs.java> ==================== CStreamingFontBank @ 009a3c20 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* WARNING: Function: __chkstk replaced with injection: alloca_probe */
/* [bsim sim=0.6838743910876047 <- ego_r]
   public: __thiscall CStreamingFontBank::CStreamingFontBank(class CStreamingFontDataBank *,unsigned
   long) */

CStreamingFontBank * __thiscall
CStreamingFontBank::CStreamingFontBank
          (CStreamingFontBank *this,CStreamingFontDataBank *param_1,ulong param_2)

{
  ushort uVar1;
  uint uVar2;
  ulong _Size;
  undefined4 *****pppppuVar3;
  void *pvVar4;
  CCharString *pCVar5;
  int *piVar6;
  int iVar7;
  uint uVar8;
  undefined4 *puVar9;
  int iVar10;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  short *psVar11;
  int *piVar12;
  undefined4 ****ppppuVar13;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CStreamingFontBank *pCVar14;
  CStreamingFontBank *local_104c;
  uchar local_1045;
  CMemoryDataInputStream local_1044 [4];
  uint local_1040;
  int *local_1038;
  int local_1030;
  undefined4 ****local_1028;
  undefined4 ****local_1024;
  undefined4 ****local_1020;
  undefined1 *local_101c;
  uint *local_1018;
  undefined4 local_1014;
  int local_1010 [1025];
  undefined4 uStack_c;
  
  uStack_c = 0x9a3c30;
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129ab38;
  CCharString::CCharString((CCharString *)(this + 8));
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129ab54;
  *(CStreamingFontDataBank **)(this + 0x10) = param_1;
  *(undefined4 *)(this + 0x302c) = 0;
  do {
    if (*(CStreamingFontBank **)(&DAT_013bda50 + *(int *)(this + 0x302c) * 4) == this) break;
    uVar2 = *(int *)(this + 0x302c) + 1;
    *(uint *)(this + 0x302c) = uVar2;
  } while (uVar2 < 8);
  if (*(int *)(this + 0x302c) == 8) {
    *(undefined4 *)(this + 0x302c) = 0;
    do {
      if (*(int *)(&DAT_013bda50 + *(int *)(this + 0x302c) * 4) == 0) break;
      uVar2 = *(int *)(this + 0x302c) + 1;
      *(uint *)(this + 0x302c) = uVar2;
    } while (uVar2 < 8);
    *(CStreamingFontBank **)(&DAT_013bda50 + *(int *)(this + 0x302c) * 4) = this;
  }
  _anon_FE46A155::InitializeStreamingFontTextureCache(*(CStreamingFontDataBank **)(this + 0x10));
  _anon_FE46A155::InitializeStreamingFontCacheEntries((StreamingFontCacheBufferData *)&DAT_013c5a98)
  ;
  _anon_FE46A155::InitializeStreamingFontCacheEntries((StreamingFontCacheBufferData *)&DAT_013c7ac8)
  ;
  pCVar14 = this + 0x2c;
  for (iVar10 = 0x400; iVar10 != 0; iVar10 = iVar10 + -1) {
    *(undefined4 *)pCVar14 = 0;
    pCVar14 = pCVar14 + 4;
  }
  pCVar14 = this + 0x102c;
  for (iVar10 = 0x800; iVar10 != 0; iVar10 = iVar10 + -1) {
    *(undefined4 *)pCVar14 = 0;
    pCVar14 = pCVar14 + 4;
  }
  _Size = CBankFile::GetEntryDataSize(*(CBankFile **)(this + 0x10),param_2);
  local_1024 = (undefined4 ****)0x0;
  local_1020 = (undefined4 ****)0x0;
  local_101c = (undefined1 *)0x0;
  if (_Size == 0) {
    local_1028 = (undefined4 *****)0x0;
  }
  else {
    local_1028 = malloc(_Size);
  }
  local_101c = (undefined1 *)((int)local_1028 + _Size);
  pppppuVar3 = (undefined4 *****)local_1028;
  for (uVar2 = _Size >> 2; uVar2 != 0; uVar2 = uVar2 - 1) {
    *pppppuVar3 = (undefined4 ****)0x0;
    pppppuVar3 = pppppuVar3 + 1;
  }
  for (uVar2 = _Size & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
    *(undefined1 *)pppppuVar3 = 0;
    pppppuVar3 = (undefined4 *****)((int)pppppuVar3 + 1);
  }
  local_1020 = (undefined4 ****)((int)local_1028 + _Size);
  pppppuVar3 = (undefined4 *****)local_1028;
  if (local_1020 == local_1028) {
    pppppuVar3 = &local_1024;
  }
  local_1024 = local_1028;
  CBankFile::ReadEntryData(*(CBankFile **)(this + 0x10),param_2,0,pppppuVar3,_Size);
  pppppuVar3 = (undefined4 *****)local_1024;
  if (local_1020 == local_1024) {
    pppppuVar3 = &local_1024;
  }
  CMemoryDataInputStream::CMemoryDataInputStream(local_1044,pppppuVar3,_Size);
  pvVar4 = ::operator_new(0x104);
  *(void **)(this + 0x14) = pvVar4;
  *(undefined4 *)(this + 0x18) = 0xffffffff;
  pCVar5 = (CCharString *)
           CMemoryDataInputStream::
           _ReadNullTerminatedString_CMemoryDataInputStream__UAE_AVCCharString__XZ(local_1044);
  CCharString::operator=((CCharString *)(this + 8),pCVar5);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_1028,
             extraout_EDX,unaff_EDI);
  if (local_1040 + 4 < 0x80000000) {
    if (local_1030 < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow
                ((CDataInputStream *)local_1044,(uchar *)&local_104c,4);
    }
    else {
      local_104c = (CStreamingFontBank *)*local_1038;
      local_1038 = local_1038 + 1;
      local_1030 = local_1030 + -4;
      local_1040 = local_1040 + 4;
    }
  }
  *(CStreamingFontBank **)(this + 4) = local_104c;
  if (local_1040 + 4 < 0x80000000) {
    if (local_1030 < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow
                ((CDataInputStream *)local_1044,(uchar *)&local_104c,4);
    }
    else {
      local_104c = (CStreamingFontBank *)*local_1038;
      local_1038 = local_1038 + 1;
      local_1030 = local_1030 + -4;
      local_1040 = local_1040 + 4;
    }
  }
  *(CStreamingFontBank **)(this + 0x20) = local_104c;
  if (local_1040 + 1 < 0x80000000) {
    if (local_1030 < 1) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)local_1044,&local_1045,1);
    }
    else {
      local_1045 = (uchar)*local_1038;
      local_1038 = (int *)((int)local_1038 + 1);
      local_1030 = local_1030 + -1;
      local_1040 = local_1040 + 1;
    }
  }
  this[0x24] = (CStreamingFontBank)(local_1045 != '\0');
  if (local_1040 + 4 < 0x80000000) {
    if (local_1030 < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow
                ((CDataInputStream *)local_1044,(uchar *)&local_104c,4);
    }
    else {
      local_104c = (CStreamingFontBank *)*local_1038;
      local_1038 = local_1038 + 1;
      local_1030 = local_1030 + -4;
      local_1040 = local_1040 + 4;
    }
  }
  *(CStreamingFontBank **)(this + 0x1c) = local_104c;
  if (local_1040 + 0x1004 < 0x80000000) {
    if (local_1030 < 0x1004) {
      CDataInputStream::ReadWithSrcChunkOverflow
                ((CDataInputStream *)local_1044,(uchar *)local_1010,0x1004);
    }
    else {
      piVar6 = local_1038;
      piVar12 = local_1010;
      for (iVar10 = 0x401; iVar10 != 0; iVar10 = iVar10 + -1) {
        *piVar12 = *piVar6;
        piVar6 = piVar6 + 1;
        piVar12 = piVar12 + 1;
      }
      local_1038 = local_1038 + 0x401;
      local_1030 = local_1030 + -0x1004;
      local_1040 = local_1040 + 0x1004;
    }
  }
  piVar6 = operator_new__(local_1010[0x400]);
  *(int **)(this + 0x28) = piVar6;
  if ((0 < local_1010[0x400]) && (local_1040 + local_1010[0x400] < 0x80000000)) {
    if (local_1030 < local_1010[0x400]) {
      CDataInputStream::ReadWithSrcChunkOverflow
                ((CDataInputStream *)local_1044,(uchar *)piVar6,local_1010[0x400]);
    }
    else {
      piVar12 = local_1038;
      for (uVar2 = (uint)local_1010[0x400] >> 2; uVar2 != 0; uVar2 = uVar2 - 1) {
        *piVar6 = *piVar12;
        piVar12 = piVar12 + 1;
        piVar6 = piVar6 + 1;
      }
      for (uVar2 = local_1010[0x400] & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
        *(uchar *)piVar6 = (uchar)*piVar12;
        piVar12 = (int *)((int)piVar12 + 1);
        piVar6 = (int *)((int)piVar6 + 1);
      }
      local_1038 = (int *)((int)local_1038 + local_1010[0x400]);
      local_1030 = local_1030 - local_1010[0x400];
      local_1040 = local_1040 + local_1010[0x400];
    }
  }
  local_104c = this + 0x2c;
  local_1028 = (undefined4 ****)0x0;
  do {
    ppppuVar13 = local_1028;
    iVar10 = local_1010[(int)local_1028 + 1] - local_1010[(int)local_1028];
    if (iVar10 != 0) {
      iVar7 = local_1010[(int)local_1028] + *(int *)(this + 0x28);
      *(int *)local_104c = iVar7;
      psVar11 = (short *)(iVar7 + 2);
      if ((*(byte *)(iVar7 + 1) & 0x80) != 0) {
        local_1014 = 0x104;
        _lzo1x_decompress_safe(psVar11,iVar10 + -2,*(undefined4 *)(this + 0x14),&local_1014,0);
        psVar11 = *(short **)(this + 0x14);
        *(undefined4 *****)(this + 0x18) = ppppuVar13;
      }
      uVar2 = 0;
      if (psVar11[1] != 0) {
        local_1018 = (uint *)(psVar11 + 2);
        do {
          if ((*local_1018 & 0xffffff) != 0) {
            uVar1 = (short)((int)ppppuVar13 << 6) + (short)uVar2 + *psVar11;
            uVar8 = (uint)(uVar1 >> 5);
            *(uint *)(this + uVar8 * 4 + 0x102c) =
                 *(uint *)(this + uVar8 * 4 + 0x102c) | 1 << ((byte)uVar1 & 0x1f);
            ppppuVar13 = local_1028;
          }
          local_1018 = local_1018 + 1;
          uVar2 = uVar2 + 1;
        } while (uVar2 < (ushort)psVar11[1]);
      }
    }
    local_1028 = (undefined4 ****)((int)ppppuVar13 + 1);
    local_104c = local_104c + 4;
  } while (local_1028 < 0x400);
  puVar9 = ::operator_new(8);
  if (puVar9 == (undefined4 *)0x0) {
    puVar9 = (undefined4 *)0x0;
  }
  else {
    *puVar9 = 0;
    puVar9[1] = 0;
  }
  *(undefined4 **)(this + 0x3030) = puVar9;
  CChunkedFileChunk::~CChunkedFileChunk((CChunkedFileChunk *)local_1044);
  if ((undefined4 *****)local_1024 != (undefined4 *****)0x0) {
    free(local_1024);
  }
  return this;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== GlyphLookup @ 009a1140 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [bsim sim=0.8113649261134748 <- ego_r]
   protected: struct CStreamingGlyphLookup * __thiscall CStreamingFontBank::GlyphLookup(unsigned
   short)const  */

CStreamingGlyphLookup * __thiscall
CStreamingFontBank::GlyphLookup(CStreamingFontBank *this,ushort param_1)

{
  CStreamingFontBank CVar1;
  int iVar2;
  ushort uVar3;
  ushort *puVar4;
  ulong uVar5;
  uint uVar6;
  CStreamingGlyph *pCVar7;
  uint uVar8;
  int iVar9;
  ushort *puVar10;
  short *psVar11;
  undefined4 *puVar12;
  CEngineInternalPrimitiveBase *this_00;
  uint uVar13;
  int *piVar14;
  int iVar15;
  ulong unaff_ESI;
  CEnginePrimitive *unaff_EDI;
  uint uVar16;
  int local_c;
  CStreamingFontBank *local_8;
  uint local_4;
  
  uVar3 = param_1;
  _param_1 = (uint)param_1;
  if ((*(uint *)(this + (uint)(uVar3 >> 5) * 4 + 0x102c) & 1 << ((byte)uVar3 & 0x1f)) == 0) {
    do {
      if (param_1 == 0x25a1) {
        return (CStreamingGlyphLookup *)0x0;
      }
      _param_1 = 0x25a1;
    } while (((byte)this[0x14e0] & 2) == 0);
  }
  local_4 = _param_1 & 0xff;
  uVar16 = 0;
  iVar15 = local_4 * 0x80;
  puVar4 = &DAT_013bda70 + local_4 * 0x40;
  puVar10 = puVar4;
  do {
    if ((puVar10[1] == param_1) && (((byte)*puVar10 & 0xf) == *(uint *)(this + 0x302c))) break;
    uVar16 = uVar16 + 1;
    puVar10 = puVar10 + 8;
  } while (uVar16 < 8);
  if (uVar16 == 8) {
    uVar16 = 0;
    psVar11 = &DAT_013bda72 + local_4 * 0x40;
    do {
      if (*psVar11 == 0) break;
      uVar16 = uVar16 + 1;
      psVar11 = psVar11 + 8;
    } while (uVar16 < 8);
    local_8 = this;
    if (uVar16 == 8) {
      uVar16 = 0;
      do {
        puVar12 = &DAT_013c7ac8;
        if (-1 < *(int *)(puVar4 + 6)) {
          puVar12 = &DAT_013c5a98;
        }
        this_00 = (CEngineInternalPrimitiveBase *)puVar12[6];
        if (((*(uint *)(this_00 + (uint)(*puVar4 >> 4) * 0x10 + 8) ^ *(uint *)(puVar4 + 2)) &
            0xffffff) != 0) break;
        puVar12 = &DAT_013c7ac8;
        if (-1 < *(int *)(puVar4 + 0xe)) {
          puVar12 = &DAT_013c5a98;
        }
        this_00 = (CEngineInternalPrimitiveBase *)puVar12[6];
        if (((*(uint *)(this_00 + (uint)(puVar4[8] >> 4) * 0x10 + 8) ^ *(uint *)(puVar4 + 10)) &
            0xffffff) != 0) {
          uVar16 = uVar16 + 1;
          break;
        }
        puVar12 = &DAT_013c7ac8;
        if (-1 < *(int *)(puVar4 + 0x16)) {
          puVar12 = &DAT_013c5a98;
        }
        this_00 = (CEngineInternalPrimitiveBase *)puVar12[6];
        if (((*(uint *)(this_00 + (uint)(puVar4[0x10] >> 4) * 0x10 + 8) ^ *(uint *)(puVar4 + 0x12))
            & 0xffffff) != 0) {
          uVar16 = uVar16 + 2;
          break;
        }
        puVar12 = &DAT_013c7ac8;
        if (-1 < *(int *)(puVar4 + 0x1e)) {
          puVar12 = &DAT_013c5a98;
        }
        this_00 = (CEngineInternalPrimitiveBase *)puVar12[6];
        if (((*(uint *)(this_00 + (uint)(puVar4[0x18] >> 4) * 0x10 + 8) ^ *(uint *)(puVar4 + 0x1a))
            & 0xffffff) != 0) {
          uVar16 = uVar16 + 3;
          break;
        }
        uVar16 = uVar16 + 4;
        puVar4 = puVar4 + 0x20;
      } while (uVar16 < 8);
      if (uVar16 == 8) {
        uVar16 = 0;
        uVar5 = CEngineInternalPrimitiveBase::AddChildPrimitive(this_00,unaff_EDI,unaff_ESI);
        local_c = *(int *)(*(int *)(uVar5 + 0x60) + 0x25c);
        uVar6 = 0;
        piVar14 = (int *)(iVar15 + 0x13bda8c);
        uVar8 = 2;
        do {
          puVar12 = &DAT_013c7ac8;
          if (-1 < piVar14[-4]) {
            puVar12 = &DAT_013c5a98;
          }
          uVar13 = local_c - *(int *)((uint)(*(ushort *)(piVar14 + -7) >> 4) * 0x10 + 4 + puVar12[6]
                                     );
          if (uVar6 <= uVar13) {
            uVar16 = uVar8 - 2;
            uVar6 = uVar13;
          }
          puVar12 = &DAT_013c7ac8;
          if (-1 < *piVar14) {
            puVar12 = &DAT_013c5a98;
          }
          uVar13 = local_c - *(int *)((uint)(*(ushort *)(piVar14 + -3) >> 4) * 0x10 + 4 + puVar12[6]
                                     );
          if (uVar6 <= uVar13) {
            uVar16 = uVar8 - 1;
            uVar6 = uVar13;
          }
          puVar12 = &DAT_013c7ac8;
          if (-1 < piVar14[4]) {
            puVar12 = &DAT_013c5a98;
          }
          uVar13 = local_c - *(int *)((uint)(*(ushort *)(piVar14 + 1) >> 4) * 0x10 + 4 + puVar12[6])
          ;
          if (uVar6 <= uVar13) {
            uVar6 = uVar13;
            uVar16 = uVar8;
          }
          puVar12 = &DAT_013c7ac8;
          if (-1 < piVar14[8]) {
            puVar12 = &DAT_013c5a98;
          }
          uVar13 = local_c - *(int *)((uint)(*(ushort *)(piVar14 + 5) >> 4) * 0x10 + 4 + puVar12[6])
          ;
          if (uVar6 <= uVar13) {
            uVar16 = uVar8 + 1;
            uVar6 = uVar13;
          }
          uVar13 = uVar8 + 2;
          piVar14 = piVar14 + 0x10;
          uVar8 = uVar8 + 4;
        } while (uVar13 < 8);
        if (uVar16 == 8) {
          return (CStreamingGlyphLookup *)0x0;
        }
      }
    }
    pCVar7 = FindGlyph(local_8,param_1);
    uVar6 = (*(uint *)pCVar7 & 0xffffff) - 1;
    uVar8 = uVar6 / DAT_0138e068;
    uVar6 = uVar6 % DAT_0138e068;
    if (DAT_0138e06c != uVar8) {
      local_c = DAT_0138e068 * 8;
      iVar15 = *(int *)(DAT_013bda34 + uVar8 * 4);
      _lzo1x_decompress_safe
                (iVar15 + DAT_013bda38,*(int *)(DAT_013bda34 + 4 + uVar8 * 4) - iVar15,DAT_013bda3c,
                 &local_c,0);
      DAT_0138e06c = uVar8;
    }
    CVar1 = local_8[0x302c];
    iVar15 = uVar16 + local_4 * 8;
    iVar9 = iVar15 * 0x10;
    (&DAT_013bda72)[iVar15 * 8] = param_1;
    iVar2 = DAT_013bda3c;
    (&DAT_013bda70)[iVar15 * 8] = (byte)CVar1 & 0xf;
    *(undefined4 *)(iVar9 + 0x13bda74) = *(undefined4 *)pCVar7;
    *(undefined4 *)(iVar9 + 0x13bda78) = *(undefined4 *)(iVar2 + uVar6 * 8);
    *(undefined4 *)(iVar9 + 0x13bda7c) = *(undefined4 *)(iVar2 + 4 + uVar6 * 8);
  }
  return (CStreamingGlyphLookup *)(&DAT_013bda70 + (uVar16 + local_4 * 8) * 8);
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== FindGlyph @ 009a4160 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [bsim sim=0.9331232507858808 <- ego_r]
   protected: class CStreamingGlyph const * __thiscall CStreamingFontBank::FindGlyph(unsigned
   short)const  */

CStreamingGlyph * __thiscall CStreamingFontBank::FindGlyph(CStreamingFontBank *this,ushort param_1)

{
  ushort *puVar1;
  ushort uVar2;
  uint uVar3;
  ushort *puVar4;
  uint uVar5;
  
  uVar2 = param_1;
  _param_1 = (uint)param_1;
  uVar3 = _param_1;
  if (uVar2 == 0xa0) {
    uVar3 = 0x20;
  }
  puVar4 = *(ushort **)(this + 0x14);
  uVar5 = uVar3 >> 6;
  if (*(uint *)(this + 0x18) != uVar5) {
    puVar1 = *(ushort **)(this + uVar5 * 4 + 0x2c);
    if (puVar1 == (ushort *)0x0) {
      return (CStreamingGlyph *)0x0;
    }
    if ((*puVar1 & 0x8000) == 0) {
      puVar4 = puVar1 + 1;
    }
    else {
      _param_1 = 0x104;
      _lzo1x_decompress_safe(puVar1 + 1,*puVar1 & 0x7fff,puVar4,&param_1,0);
      *(uint *)(this + 0x18) = uVar5;
    }
  }
  uVar3 = (uVar3 & 0x3f) - (uint)*puVar4;
  if ((uVar3 < puVar4[1]) && ((*(uint *)(puVar4 + uVar3 * 2 + 2) & 0xffffff) != 0)) {
    return (CStreamingGlyph *)(puVar4 + uVar3 * 2 + 2);
  }
  return (CStreamingGlyph *)0x0;
}

 (GhidraScript)  
INFO  REPORT: Save succeeded for processed file: /Fable.exe (HeadlessAnalyzer)  
