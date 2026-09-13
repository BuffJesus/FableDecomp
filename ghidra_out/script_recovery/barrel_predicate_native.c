//=== CEntry @ 00c954a0 (seed 0x00C954C0) ===

/* [bsim sim=0.5269279816022729 <- ego_r]
   public: __thiscall CParticleMorphs::CEntry::CEntry(void) */

undefined4 __thiscall CParticleMorphs::CEntry::CEntry(CEntry *this)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *in_EDX;
  CEntry *pCVar3;
  
  pCVar3 = this;
  for (iVar2 = 0xb; iVar2 != 0; iVar2 = iVar2 + -1) {
    *(undefined4 *)pCVar3 = 0;
    pCVar3 = pCVar3 + 4;
  }
  *(undefined4 **)(this + 0xc) = in_EDX;
  *(undefined4 *)(this + 4) = in_EDX[1];
  *(undefined4 *)(this + 8) = in_EDX[1];
  *(undefined4 *)this = *in_EDX;
  uVar1 = Bitstream_ProcessCodes(0);
  *(undefined4 *)(this + 0x14) = uVar1;
  uVar1 = Waveform_Synthesize(0);
  *(undefined4 *)(this + 0x10) = uVar1;
  return 0;
}


// CALLEES of CEntry:
//   00c951b0  Waveform_Synthesize
//   00c94fd0  Bitstream_ProcessCodes

//=== CTCWound::CWound::Initialize @ 00c95580 (seed 0x00C955A0) ===

/* WARNING: Function: __chkstk replaced with injection: alloca_probe */
/* WARNING: Unable to track spacebase fully for stack */
/* autoname-t5 [high]: Caller: AudioSystem_Initialize; param is CWound*; initializes 11 fields (0xb
   * 4); calls Bitstream_ProcessCodes, Waveform_Synthesize; Huffman decode setup */

undefined4 CTCWound__CWound__Initialize(void)

{
  char cVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  void *pvVar4;
  int iVar5;
  int *piVar6;
  uint uVar7;
  uint *puVar8;
  CWound *in_ECX;
  int iVar9;
  int iVar10;
  uint uVar11;
  undefined4 *in_EDX;
  int iVar12;
  CWound *pCVar13;
  size_t asStack_40 [5];
  int local_14;
  undefined4 *local_10;
  uint local_c;
  int local_8;
  
  pCVar13 = in_ECX;
  for (iVar9 = 0xb; iVar9 != 0; iVar9 = iVar9 + -1) {
    *(undefined4 *)pCVar13 = 0;
    pCVar13 = pCVar13 + 4;
  }
  iVar9 = in_EDX[1];
  iVar12 = 0;
  if (0 < iVar9) {
    piVar6 = (int *)in_EDX[2];
    do {
      if (0 < *piVar6) {
        iVar12 = iVar12 + 1;
      }
      piVar6 = piVar6 + 1;
      iVar9 = iVar9 + -1;
    } while (iVar9 != 0);
  }
  *(undefined4 *)(in_ECX + 4) = in_EDX[1];
  *(int *)(in_ECX + 8) = iVar12;
  *(undefined4 *)in_ECX = *in_EDX;
  asStack_40[3] = 0xc955cd;
  asStack_40[4] = iVar12;
  puVar2 = (undefined4 *)Bitstream_ProcessCodes();
  asStack_40[4] = 0xc955e7;
  iVar9 = iVar12 * -4;
  piVar6 = (int *)(&stack0xffffffd4 + iVar12 * -8);
  if (puVar2 == (undefined4 *)0x0) {
    asStack_40[4 - iVar12] = 0xc955f7;
    CTCWound::CWound::Kill(in_ECX,*(CTCTextureDecal **)(&stack0xffffffd4 + iVar9));
    return 0xffffffff;
  }
  if (0 < iVar12) {
    local_10 = puVar2;
    local_c = iVar12;
    do {
      asStack_40[4 - iVar12] = 0xc9561d;
      uVar3 = Bit_ReverseBits();
      *local_10 = uVar3;
      *(undefined4 **)(&stack0xffffffd4 + (iVar9 - (int)puVar2) + (int)local_10) = local_10;
      local_10 = local_10 + 1;
      local_c = local_c + -1;
    } while (local_c != 0);
  }
  asStack_40[4 - iVar12] = (size_t)Uint_Compare;
  asStack_40[3 - iVar12] = 4;
  asStack_40[2 - iVar12] = iVar12;
  asStack_40[1 - iVar12] = (size_t)(&stack0xffffffd4 + iVar9);
  asStack_40[-iVar12] = 0xc95649;
  qsort((void *)asStack_40[1 - iVar12],asStack_40[2 - iVar12],asStack_40[3 - iVar12],
        (_PtFuncCompare *)asStack_40[4 - iVar12]);
  asStack_40[4 - iVar12] = 0xc9565a;
  asStack_40[iVar12 * -2 + 4] = iVar12 * 4;
  asStack_40[iVar12 * -2 + 3] = 0xc95667;
  pvVar4 = malloc(asStack_40[iVar12 * -2 + 4]);
  *(void **)(in_ECX + 0x14) = pvVar4;
  iVar5 = 0;
  if (0 < iVar12) {
    do {
      *(int *)(&stack0xffffffd4 +
              (*(int *)(&stack0xffffffd4 + iVar5 * 4 + iVar9) - (int)puVar2 >> 2) * 4 + iVar12 * -8)
           = iVar5;
      iVar5 = iVar5 + 1;
    } while (iVar5 < iVar12);
    if (0 < iVar12) {
      local_14 = iVar12;
      do {
        *(undefined4 *)(*(int *)(in_ECX + 0x14) + *piVar6 * 4) =
             *(undefined4 *)(((int)puVar2 - (int)(&stack0xffffffd4 + iVar12 * -8)) + (int)piVar6);
        piVar6 = piVar6 + 1;
        local_14 = local_14 + -1;
      } while (local_14 != 0);
    }
  }
  asStack_40[iVar12 * -2 + 4] = (size_t)puVar2;
  asStack_40[iVar12 * -2 + 3] = 0xc956c1;
  free((void *)asStack_40[iVar12 * -2 + 4]);
  asStack_40[iVar12 * -2 + 4] = (size_t)(&stack0xffffffd4 + iVar12 * -8);
  asStack_40[iVar12 * -2 + 3] = 0xc956d4;
  uVar3 = Waveform_Synthesize();
  *(undefined4 *)(in_ECX + 0x10) = uVar3;
  asStack_40[iVar12 * -2 + 4] = iVar12 * 4;
  asStack_40[iVar12 * -2 + 3] = 0xc956e1;
  pvVar4 = malloc(asStack_40[iVar12 * -2 + 4]);
  *(void **)(in_ECX + 0x18) = pvVar4;
  iVar5 = 0;
  iVar9 = 0;
  if (0 < (int)in_EDX[1]) {
    do {
      if (0 < *(int *)(in_EDX[2] + iVar9 * 4)) {
        *(int *)(*(int *)(in_ECX + 0x18) + *(int *)(&stack0xffffffd4 + iVar5 * 4 + iVar12 * -8) * 4)
             = iVar9;
        iVar5 = iVar5 + 1;
      }
      iVar9 = iVar9 + 1;
    } while (iVar9 < (int)in_EDX[1]);
  }
  asStack_40[iVar12 * -2 + 4] = iVar5;
  asStack_40[iVar12 * -2 + 3] = 0xc95717;
  pvVar4 = malloc(asStack_40[iVar12 * -2 + 4]);
  *(void **)(in_ECX + 0x1c) = pvVar4;
  iVar9 = 0;
  local_10 = (undefined4 *)0x0;
  local_8 = 0;
  if (0 < (int)in_EDX[1]) {
    do {
      if (0 < *(int *)(in_EDX[2] + local_8 * 4)) {
        *(undefined1 *)
         (*(int *)(&stack0xffffffd4 + iVar9 * 4 + iVar12 * -8) + *(int *)(in_ECX + 0x1c)) =
             *(undefined1 *)(in_EDX[2] + local_8 * 4);
        iVar9 = iVar9 + 1;
      }
      local_8 = local_8 + 1;
      local_10 = (undefined4 *)iVar9;
    } while (local_8 < (int)in_EDX[1]);
  }
  puVar2 = local_10;
  asStack_40[iVar12 * -2 + 4] = 0xc95765;
  iVar9 = Bitcount();
  *(int *)(in_ECX + 0x24) = iVar9 + -4;
  if (iVar9 + -4 < 5) {
    *(undefined4 *)(in_ECX + 0x24) = 5;
  }
  if (8 < *(int *)(in_ECX + 0x24)) {
    *(undefined4 *)(in_ECX + 0x24) = 8;
  }
  iVar9 = 1 << ((byte)*(undefined4 *)(in_ECX + 0x24) & 0x1f);
  asStack_40[iVar12 * -2 + 4] = 4;
  asStack_40[iVar12 * -2 + 3] = iVar9;
  asStack_40[iVar12 * -2 + 2] = 0xc9579c;
  pvVar4 = calloc(asStack_40[iVar12 * -2 + 3],asStack_40[iVar12 * -2 + 4]);
  iVar5 = 0;
  *(void **)(in_ECX + 0x20) = pvVar4;
  *(undefined4 *)(in_ECX + 0x28) = 0;
  if (0 < (int)local_10) {
    do {
      iVar10 = (int)*(char *)(*(int *)(in_ECX + 0x1c) + iVar5);
      if (*(int *)(in_ECX + 0x28) < iVar10) {
        *(int *)(in_ECX + 0x28) = iVar10;
      }
      cVar1 = *(char *)(*(int *)(in_ECX + 0x1c) + iVar5);
      if ((int)cVar1 <= *(int *)(in_ECX + 0x24)) {
        asStack_40[iVar12 * -2 + 4] = 0xc957d5;
        uVar7 = Bit_ReverseBits();
        iVar10 = 0;
        if (0 < 1 << ((char)*(undefined4 *)(in_ECX + 0x24) - cVar1 & 0x1fU)) {
          do {
            *(int *)(*(int *)(in_ECX + 0x20) +
                    (iVar10 << (*(byte *)(iVar5 + *(int *)(in_ECX + 0x1c)) & 0x1f) | uVar7) * 4) =
                 iVar5 + 1;
            iVar10 = iVar10 + 1;
          } while (iVar10 < 1 << ((char)*(undefined4 *)(in_ECX + 0x24) -
                                  *(char *)(iVar5 + *(int *)(in_ECX + 0x1c)) & 0x1fU));
        }
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < (int)local_10);
  }
  uVar3 = *(undefined4 *)(in_ECX + 0x24);
  local_c = 0;
  local_10 = (undefined4 *)0x0;
  local_8 = 0;
  if (0 < iVar9) {
    do {
      uVar7 = local_8 << (0x20U - (char)*(undefined4 *)(in_ECX + 0x24) & 0x1f);
      asStack_40[iVar12 * -2 + 4] = 0xc95866;
      iVar5 = Bit_ReverseBits();
      if (*(int *)(*(int *)(in_ECX + 0x20) + iVar5 * 4) == 0) {
        iVar10 = local_c + 1;
        if (iVar10 < (int)puVar2) {
          puVar8 = (uint *)(*(int *)(in_ECX + 0x14) + 4 + local_c * 4);
          do {
            if (uVar7 < *puVar8) break;
            local_c = local_c + 1;
            puVar8 = puVar8 + 1;
            iVar10 = iVar10 + 1;
          } while (iVar10 < (int)puVar2);
        }
        if ((int)local_10 < (int)puVar2) {
          puVar8 = (uint *)(*(int *)(in_ECX + 0x14) + (int)local_10 * 4);
          do {
            if (uVar7 < (*puVar8 & -2 << (0x1fU - (char)uVar3 & 0x1f))) break;
            local_10 = (undefined4 *)((int)local_10 + 1);
            puVar8 = puVar8 + 1;
          } while ((int)local_10 < (int)puVar2);
        }
        uVar11 = (int)puVar2 - (int)local_10;
        uVar7 = local_c;
        if (0x7fff < local_c) {
          uVar7 = 0x7fff;
        }
        if (0x7fff < uVar11) {
          uVar11 = 0x7fff;
        }
        *(uint *)(*(int *)(in_ECX + 0x20) + iVar5 * 4) = (uVar7 | 0xffff0000) << 0xf | uVar11;
      }
      local_8 = local_8 + 1;
    } while (local_8 < iVar9);
  }
  return 0;
}


// CALLEES of CTCWound::CWound::Initialize:
//   EXTERNAL:00000023  calloc
//   00c94ee0  Bitcount
//   00c951b0  Waveform_Synthesize
//   00c95440  Kill
//   EXTERNAL:00000025  qsort
//   EXTERNAL:00000063  malloc
//   EXTERNAL:00000062  free
//   00bfea30  __chkstk
//   00c954f0  Bit_ReverseBits
//   00c94fd0  Bitstream_ProcessCodes

//=== Waveform_FindClosestFrequencyBin @ 00c95a10 (seed 0x00C95A70) ===

/* autoname-t5 [medium]: Calls FUN_00c95910; binary search pattern on sorted floats; returns best
   frequency match index */

int Waveform_FindClosestFrequencyBin(int param_1)

{
  int iVar1;
  int *in_ECX;
  float *pfVar2;
  int iVar3;
  int *piVar4;
  int in_EDX;
  int iVar5;
  int iVar6;
  float *pfVar7;
  int iVar8;
  int iVar9;
  float10 fVar10;
  int local_24;
  float local_20;
  
  iVar8 = in_ECX[3];
  piVar4 = *(int **)(iVar8 + 0x28);
  iVar1 = *in_ECX;
  if (piVar4 != (int *)0x0) {
    local_24 = 0;
    if (0 < iVar1) {
      iVar8 = piVar4[3];
      iVar9 = *piVar4;
      iVar5 = iVar8 >> 1;
      pfVar7 = (float *)(in_EDX + (iVar1 + -1) * param_1 * 4);
      local_20 = (float)iVar1;
      do {
        if (*(float *)(iVar9 + iVar5 * 4) <= *pfVar7) {
          iVar6 = iVar5 + 1;
          iVar3 = iVar8 + -1;
          if (3 < iVar3 - iVar6) {
            pfVar2 = (float *)(iVar9 + 8 + iVar6 * 4);
            do {
              if (*pfVar7 < pfVar2[-2]) goto LAB_00c95b96;
              if (*pfVar7 < pfVar2[-1]) {
                iVar6 = iVar6 + 1;
                goto LAB_00c95b96;
              }
              if (*pfVar7 < *pfVar2) {
                iVar6 = iVar6 + 2;
                goto LAB_00c95b96;
              }
              if (*pfVar7 < pfVar2[1]) {
                iVar6 = iVar6 + 3;
                goto LAB_00c95b96;
              }
              iVar6 = iVar6 + 4;
              pfVar2 = pfVar2 + 4;
            } while (iVar6 < iVar8 + -4);
          }
          if (iVar6 < iVar3) {
            pfVar2 = (float *)(iVar9 + iVar6 * 4);
            do {
              if (*pfVar7 < *pfVar2) break;
              iVar6 = iVar6 + 1;
              pfVar2 = pfVar2 + 1;
            } while (iVar6 < iVar3);
          }
        }
        else {
          iVar6 = iVar5;
          if (3 < iVar5) {
            pfVar2 = (float *)(iVar9 + -8 + iVar5 * 4);
            do {
              if (pfVar2[1] <= *pfVar7) goto LAB_00c95b96;
              if (*pfVar2 <= *pfVar7) {
                iVar6 = iVar6 + -1;
                goto LAB_00c95b96;
              }
              if (pfVar2[-1] <= *pfVar7) {
                iVar6 = iVar6 + -2;
                goto LAB_00c95b96;
              }
              if (pfVar2[-2] <= *pfVar7) {
                iVar6 = iVar6 + -3;
                goto LAB_00c95b96;
              }
              iVar6 = iVar6 + -4;
              pfVar2 = pfVar2 + -4;
            } while (3 < iVar6);
          }
          if (0 < iVar6) {
            pfVar2 = (float *)(iVar9 + -4 + iVar6 * 4);
            do {
              if (*pfVar2 <= *pfVar7) break;
              iVar6 = iVar6 + -1;
              pfVar2 = pfVar2 + -1;
            } while (0 < iVar6);
          }
        }
LAB_00c95b96:
        pfVar7 = pfVar7 + -param_1;
        local_24 = piVar4[2] * local_24 + *(int *)(piVar4[1] + iVar6 * 4);
        local_20 = (float)((int)local_20 + -1);
      } while (local_20 != 0.0);
    }
    iVar8 = in_ECX[3];
    if (0 < *(int *)(*(int *)(iVar8 + 8) + local_24 * 4)) {
      return local_24;
    }
  }
  iVar9 = in_ECX[4];
  iVar5 = in_ECX[1];
  iVar6 = -1;
  local_20 = 0.0;
  if (0 < iVar5) {
    piVar4 = *(int **)(iVar8 + 8);
    iVar8 = 0;
    do {
      if ((0 < *piVar4) &&
         ((fVar10 = (float10)Audio_ComputeL2Norm(iVar1,iVar9,in_EDX), iVar6 == -1 ||
          (fVar10 < (float10)local_20)))) {
        local_20 = (float)fVar10;
        iVar6 = iVar8;
      }
      iVar9 = iVar9 + iVar1 * 4;
      iVar8 = iVar8 + 1;
      piVar4 = piVar4 + 1;
    } while (iVar8 < iVar5);
  }
  return iVar6;
}


// CALLEES of Waveform_FindClosestFrequencyBin:
//   00c95910  Audio_ComputeL2Norm

//=== Waveform_FindClosestFrequencyBin @ 00c95a10 (seed 0x00C95B10) ===

/* autoname-t5 [medium]: Calls FUN_00c95910; binary search pattern on sorted floats; returns best
   frequency match index */

int Waveform_FindClosestFrequencyBin(int param_1)

{
  int iVar1;
  int *in_ECX;
  float *pfVar2;
  int iVar3;
  int *piVar4;
  int in_EDX;
  int iVar5;
  int iVar6;
  float *pfVar7;
  int iVar8;
  int iVar9;
  float10 fVar10;
  int local_24;
  float local_20;
  
  iVar8 = in_ECX[3];
  piVar4 = *(int **)(iVar8 + 0x28);
  iVar1 = *in_ECX;
  if (piVar4 != (int *)0x0) {
    local_24 = 0;
    if (0 < iVar1) {
      iVar8 = piVar4[3];
      iVar9 = *piVar4;
      iVar5 = iVar8 >> 1;
      pfVar7 = (float *)(in_EDX + (iVar1 + -1) * param_1 * 4);
      local_20 = (float)iVar1;
      do {
        if (*(float *)(iVar9 + iVar5 * 4) <= *pfVar7) {
          iVar6 = iVar5 + 1;
          iVar3 = iVar8 + -1;
          if (3 < iVar3 - iVar6) {
            pfVar2 = (float *)(iVar9 + 8 + iVar6 * 4);
            do {
              if (*pfVar7 < pfVar2[-2]) goto LAB_00c95b96;
              if (*pfVar7 < pfVar2[-1]) {
                iVar6 = iVar6 + 1;
                goto LAB_00c95b96;
              }
              if (*pfVar7 < *pfVar2) {
                iVar6 = iVar6 + 2;
                goto LAB_00c95b96;
              }
              if (*pfVar7 < pfVar2[1]) {
                iVar6 = iVar6 + 3;
                goto LAB_00c95b96;
              }
              iVar6 = iVar6 + 4;
              pfVar2 = pfVar2 + 4;
            } while (iVar6 < iVar8 + -4);
          }
          if (iVar6 < iVar3) {
            pfVar2 = (float *)(iVar9 + iVar6 * 4);
            do {
              if (*pfVar7 < *pfVar2) break;
              iVar6 = iVar6 + 1;
              pfVar2 = pfVar2 + 1;
            } while (iVar6 < iVar3);
          }
        }
        else {
          iVar6 = iVar5;
          if (3 < iVar5) {
            pfVar2 = (float *)(iVar9 + -8 + iVar5 * 4);
            do {
              if (pfVar2[1] <= *pfVar7) goto LAB_00c95b96;
              if (*pfVar2 <= *pfVar7) {
                iVar6 = iVar6 + -1;
                goto LAB_00c95b96;
              }
              if (pfVar2[-1] <= *pfVar7) {
                iVar6 = iVar6 + -2;
                goto LAB_00c95b96;
              }
              if (pfVar2[-2] <= *pfVar7) {
                iVar6 = iVar6 + -3;
                goto LAB_00c95b96;
              }
              iVar6 = iVar6 + -4;
              pfVar2 = pfVar2 + -4;
            } while (3 < iVar6);
          }
          if (0 < iVar6) {
            pfVar2 = (float *)(iVar9 + -4 + iVar6 * 4);
            do {
              if (*pfVar2 <= *pfVar7) break;
              iVar6 = iVar6 + -1;
              pfVar2 = pfVar2 + -1;
            } while (0 < iVar6);
          }
        }
LAB_00c95b96:
        pfVar7 = pfVar7 + -param_1;
        local_24 = piVar4[2] * local_24 + *(int *)(piVar4[1] + iVar6 * 4);
        local_20 = (float)((int)local_20 + -1);
      } while (local_20 != 0.0);
    }
    iVar8 = in_ECX[3];
    if (0 < *(int *)(*(int *)(iVar8 + 8) + local_24 * 4)) {
      return local_24;
    }
  }
  iVar9 = in_ECX[4];
  iVar5 = in_ECX[1];
  iVar6 = -1;
  local_20 = 0.0;
  if (0 < iVar5) {
    piVar4 = *(int **)(iVar8 + 8);
    iVar8 = 0;
    do {
      if ((0 < *piVar4) &&
         ((fVar10 = (float10)Audio_ComputeL2Norm(iVar1,iVar9,in_EDX), iVar6 == -1 ||
          (fVar10 < (float10)local_20)))) {
        local_20 = (float)fVar10;
        iVar6 = iVar8;
      }
      iVar9 = iVar9 + iVar1 * 4;
      iVar8 = iVar8 + 1;
      piVar4 = piVar4 + 1;
    } while (iVar8 < iVar5);
  }
  return iVar6;
}


// CALLEES of Waveform_FindClosestFrequencyBin:
//   00c95910  Audio_ComputeL2Norm

//=== FFT_Radix4Stage @ 00c99060 (seed 0x00C991A0) ===

/* autoname-t5 [medium]: Butterfly pattern with 4-way unrolling; radix-4 FFT stage kernel */

void FFT_Radix4Stage(int param_1,int param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  int in_EAX;
  float *pfVar5;
  float *in_ECX;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  float *pfVar10;
  uint uVar11;
  float *pfVar12;
  uint in_EDX;
  int iVar13;
  float *pfVar14;
  float *pfVar15;
  float *local_20;
  float *local_1c;
  float *local_14;
  float *local_10;
  int local_c;
  int local_8;
  int local_4;
  
  iVar6 = in_EDX * param_1;
  iVar13 = 0;
  iVar7 = 0;
  local_20 = (float *)0x0;
  if (3 < param_1) {
    local_10 = (float *)((param_1 - 4U >> 2) + 1);
    local_20 = (float *)((int)local_10 * 4);
    pfVar5 = in_ECX + iVar6;
    do {
      iVar8 = iVar13 + -1 + in_EDX * 2;
      in_ECX[iVar7] = *(float *)(in_EAX + iVar8 * 4) + *(float *)(in_EAX + iVar13 * 4);
      iVar7 = iVar7 + in_EDX;
      *pfVar5 = *(float *)(in_EAX + iVar13 * 4) - *(float *)(in_EAX + iVar8 * 4);
      pfVar5 = pfVar5 + in_EDX;
      iVar13 = iVar7 * 2 + -1 + in_EDX * 2;
      in_ECX[iVar7] = *(float *)(in_EAX + iVar13 * 4) + *(float *)(in_EAX + iVar7 * 8);
      iVar8 = iVar7 + in_EDX;
      *pfVar5 = *(float *)(in_EAX + iVar7 * 8) - *(float *)(in_EAX + iVar13 * 4);
      pfVar5 = pfVar5 + in_EDX;
      iVar13 = iVar8 * 2 + -1 + in_EDX * 2;
      in_ECX[iVar8] = *(float *)(in_EAX + iVar13 * 4) + *(float *)(in_EAX + iVar8 * 8);
      iVar9 = iVar8 + in_EDX;
      *pfVar5 = *(float *)(in_EAX + iVar8 * 8) - *(float *)(in_EAX + iVar13 * 4);
      iVar13 = iVar9 * 2 + -1 + in_EDX * 2;
      in_ECX[iVar9] = *(float *)(in_EAX + iVar13 * 4) + *(float *)(in_EAX + iVar9 * 8);
      iVar7 = iVar9 + in_EDX;
      pfVar5[in_EDX] = *(float *)(in_EAX + iVar9 * 8) - *(float *)(in_EAX + iVar13 * 4);
      pfVar5 = pfVar5 + in_EDX + in_EDX;
      local_10 = (float *)((int)local_10 + -1);
      iVar13 = iVar7 * 2;
    } while (local_10 != (float *)0x0);
  }
  if ((int)local_20 < param_1) {
    local_10 = in_ECX + iVar6 + iVar7;
    iVar8 = param_1 - (int)local_20;
    do {
      iVar9 = iVar13 + -1 + in_EDX * 2;
      in_ECX[iVar7] = *(float *)(in_EAX + iVar9 * 4) + *(float *)(in_EAX + iVar13 * 4);
      iVar7 = iVar7 + in_EDX;
      *local_10 = *(float *)(in_EAX + iVar13 * 4) - *(float *)(in_EAX + iVar9 * 4);
      local_10 = local_10 + in_EDX;
      iVar8 = iVar8 + -1;
      iVar13 = iVar7 * 2;
    } while (iVar8 != 0);
  }
  if (1 < (int)in_EDX) {
    if (in_EDX != 2) {
      iVar13 = 0;
      local_8 = 0;
      if (0 < param_1) {
        local_14 = in_ECX + iVar6;
        local_4 = param_1;
        local_10 = in_ECX;
        do {
          if (2 < (int)in_EDX) {
            local_1c = local_10;
            local_20 = local_14;
            local_c = (in_EDX - 3 >> 1) + 1;
            pfVar5 = (float *)(in_EAX + iVar13 * 4);
            pfVar12 = (float *)(param_2 + 4);
            pfVar14 = (float *)(in_EAX + (iVar13 + in_EDX * 2) * 4);
            do {
              pfVar15 = pfVar14 + -2;
              pfVar10 = pfVar5 + 2;
              local_1c[1] = pfVar14[-3] + pfVar5[1];
              fVar1 = pfVar5[1];
              fVar2 = pfVar14[-3];
              local_1c[2] = *pfVar10 - *pfVar15;
              fVar3 = *pfVar10;
              fVar4 = *pfVar15;
              local_20[1] = (fVar1 - fVar2) * pfVar12[-1] - (fVar3 + fVar4) * *pfVar12;
              local_20[2] = (fVar1 - fVar2) * *pfVar12 + (fVar3 + fVar4) * pfVar12[-1];
              local_c = local_c + -1;
              pfVar5 = pfVar10;
              pfVar12 = pfVar12 + 2;
              pfVar14 = pfVar15;
              local_20 = local_20 + 2;
              local_1c = local_1c + 2;
            } while (local_c != 0);
          }
          local_14 = local_14 + in_EDX;
          local_8 = local_8 + in_EDX;
          local_10 = local_10 + in_EDX;
          iVar13 = local_8 * 2;
          local_4 = local_4 + -1;
        } while (local_4 != 0);
      }
      uVar11 = in_EDX & 0x80000001;
      if ((int)uVar11 < 0) {
        uVar11 = (uVar11 - 1 | 0xfffffffe) + 1;
      }
      if (uVar11 == 1) {
        return;
      }
    }
    iVar13 = in_EDX - 1;
    local_20 = (float *)0x0;
    iVar7 = iVar13;
    if (3 < param_1) {
      param_2 = (param_1 - 4U >> 2) + 1;
      local_20 = (float *)(param_2 * 4);
      pfVar5 = in_ECX + iVar6 + iVar13;
      do {
        fVar1 = *(float *)(in_EAX + iVar13 * 4);
        in_ECX[iVar7] = fVar1 + fVar1;
        fVar1 = *(float *)(in_EAX + 4 + iVar13 * 4);
        *pfVar5 = -(fVar1 + fVar1);
        pfVar5 = pfVar5 + in_EDX;
        iVar13 = iVar13 + in_EDX * 2;
        fVar1 = *(float *)(in_EAX + iVar13 * 4);
        in_ECX[iVar7 + in_EDX] = fVar1 + fVar1;
        iVar7 = iVar7 + in_EDX + in_EDX;
        fVar1 = *(float *)(in_EAX + 4 + iVar13 * 4);
        *pfVar5 = -(fVar1 + fVar1);
        pfVar5 = pfVar5 + in_EDX;
        iVar13 = iVar13 + in_EDX * 2;
        fVar1 = *(float *)(in_EAX + iVar13 * 4);
        in_ECX[iVar7] = fVar1 + fVar1;
        iVar7 = iVar7 + in_EDX;
        fVar1 = *(float *)(in_EAX + 4 + iVar13 * 4);
        *pfVar5 = -(fVar1 + fVar1);
        iVar13 = iVar13 + in_EDX * 2;
        fVar1 = *(float *)(in_EAX + iVar13 * 4);
        in_ECX[iVar7] = fVar1 + fVar1;
        iVar7 = iVar7 + in_EDX;
        fVar1 = *(float *)(in_EAX + 4 + iVar13 * 4);
        pfVar5[in_EDX] = -(fVar1 + fVar1);
        pfVar5 = pfVar5 + in_EDX + in_EDX;
        iVar13 = iVar13 + in_EDX * 2;
        param_2 = param_2 + -1;
      } while (param_2 != 0);
    }
    if ((int)local_20 < param_1) {
      pfVar5 = (float *)(in_EAX + iVar13 * 4);
      pfVar12 = in_ECX + iVar7;
      pfVar14 = in_ECX + iVar7 + iVar6;
      param_1 = param_1 - (int)local_20;
      do {
        *pfVar12 = *pfVar5 + *pfVar5;
        pfVar12 = pfVar12 + in_EDX;
        pfVar10 = pfVar5 + 1;
        pfVar5 = pfVar5 + in_EDX * 2;
        *pfVar14 = -(*pfVar10 + *pfVar10);
        pfVar14 = pfVar14 + in_EDX;
        param_1 = param_1 + -1;
      } while (param_1 != 0);
    }
  }
  return;
}


// CALLEES of FFT_Radix4Stage:

//=== CTriangleList @ 00db7d00 (seed 0x00DB7D00) ===

/* [bsim sim=0.8386630707597903 <- ego_r]
   public: __thiscall CTriangleStripifier::CTriangleList::CTriangleList(void) */

CTriangleList * __thiscall CTriangleStripifier::CTriangleList::CTriangleList(CTriangleList *this)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int in_EDX;
  undefined4 in_stack_00000004;
  CScriptThing *in_stack_00000008;
  
  puVar2 = ::operator_new(0x1c);
  if (puVar2 == (undefined4 *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    puVar2[1] = *(undefined4 *)(in_EDX + 0x40);
    *puVar2 = &PTR__vector_deleting_destructor__012c3224;
    CScriptThing::CScriptThing((CScriptThing *)(puVar2 + 2),in_stack_00000008);
    *puVar2 = &PTR__vector_deleting_destructor__012d94f0;
    puVar2[5] = in_EDX;
    puVar2[6] = in_stack_00000004;
  }
  *(undefined4 **)this = puVar2;
  if (puVar2 != (undefined4 *)0x0) {
    puVar2 = ::operator_new(0xc);
    if (puVar2 != (undefined4 *)0x0) {
      uVar1 = *(undefined4 *)this;
      *puVar2 = 1;
      puVar2[1] = &LAB_00cdee00;
      puVar2[2] = uVar1;
      *(undefined4 **)(this + 4) = puVar2;
      return this;
    }
    *(undefined4 *)(this + 4) = 0;
    return this;
  }
  *(undefined4 *)(this + 4) = 0;
  return this;
}


// CALLEES of CTriangleList:
//   004abe90  CScriptThing
//   00bfea1a  operator_new

