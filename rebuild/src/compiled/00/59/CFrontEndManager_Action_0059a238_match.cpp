// NUISystem::CFrontEndManager::Action @ 0x0059A238  (1486 bytes)
// Model as __fastcall free function: this in ecx (-> esi), dummy edx, one 4-byte
// stack arg [ebp+8] = CCountedPointer<CAction>&.
//
// GRADE (2026-07-31): BEHAVIOR-COMPLETE, not yet byte-identical. All ~30 case
// bodies are real (no stub shortcuts) -> strictly better than the graded slice
// at CFrontEndManager_Action_0059a238.cpp.
//
// *** Do NOT edit the shared lift/scripts/build_and_diff.ps1 flags to chase a
//     match (a prior agent changed /O2 /Oy -> /O1 /G7 and produced a bogus
//     "register-allocation mirror" conclusion; reverted). Use a SCRATCH build. ***
//
// PER-TU BUILD FLAG = /O2 /Oy- /G7  (verified faithful, NOT a hack):
//   - Retail has NO C++ exception machinery: scanning the 1486 bytes finds no
//     `push fs:[0]` (64ff35), no `mov fs:[0],esp` (648925), no EH-state stores
//     `mov [ebp-4],imm` (c745fc). The ebp frame is frame-pointer RETENTION, not EH.
//     (Corrects an earlier wrong "EH-driven" note.)
//   - Binary is NOT globally /Oy-: cmouse TU byte-matches under /O2 /Oy with NO
//     frame. But this TU keeps frames -- sibling CFrontEndManager::GotoProfileMenu
//     @0x00597b20 (same source file) has prologue 55 8b ec 51 51 53 56 57 8b f1.
//     So /Oy- is the correct per-TU flag here, not a global override.
//   - /G7 (Pentium-4 scheduling; orthogonal to opt-level, NOT the reverted /O1
//     cheat) is needed too: without it the byte-flag tests near the top don't
//     match. NOTE /G7's exact effect is `cmp [mem],al` (38 /r, reusing a zeroed
//     reg), not literally `cmp byte,0` (80 /7 ib) -- in-context it moves the first
//     divergence from 0x5 to 0x4d, which is the real signal.
//   - Dispatch is the retail nested RANGE if-tree, not a flat switch.
//
// STATE: relocation-masked IDENTICAL for bytes 0x00..0x4c (77 B) under /O2 /Oy- /G7,
// built len 1522 vs retail 1486. NOT yet a full match. First masked divergence @ 0x4d:
//   retail: 8b 45 08  8b 00  8b 08   ba dc 00 00 00  3b ca   (piVar4->EAX, id->ECX; mov edx,0xdc; cmp ecx,edx)
//   built : 8b 45 08  8b 08  8b 01   3d dc 00 00 00         (piVar4->ECX, id->EAX; cmp eax,0xdc; jle)
//
// 2026-07-31 LEVER PASS (ground truth):
//   Lever (a) MERGED COMPARE -- LANDED (partial). Restructured top split to the
//   retail three-way `if (id>0xdc){B/C} else if(id==0xdc){INVALID} else {A}`. The
//   spurious `cmp 0xdd; jge` is GONE; built now emits ONE `cmp 0xdc`. First divergence
//   stays at 0x4d because the delta is no longer the compare COUNT but the compare
//   FORM/polarity + register selection (see below). Length went 1511->1522 (block
//   reorder changed downstream jump encodings; expected, not a regression at 0x4d).
//   Lever (b) id-in-ECX -- NOT REACHED. Tried three source shapes (`*piVar4`,
//   `piVar4[0]`, id-first `**ppiVar4` then `*ppiVar4`). ALL produced BYTE-IDENTICAL
//   output: VC7.1 CSE collapses every derivation to `mov eax,[ebp+8]; mov ecx,[eax];
//   mov eax,[ecx]` with piVar4->ECX, id->EAX. The retail `ba dc; 3b ca` (0xdc
//   materialized in edx, cmp ecx,edx) needs id in ECX + piVar4 live in EAX -- a
//   whole-function allocator outcome invariant to these local rewrites.
//   VERDICT: earlier "different sub-build / not source-reachable" claim is
//   PARTLY-DISSOLVED: the merged-compare delta IS source-reachable (fixed); the
//   register/polarity delta was NOT flipped by the levers attempted here.
//
// REMAINING GAP -- narrower than a prior "3 unreachable deltas" note claimed:
//   (a) MERGED COMPARE is source-reachable: writing the top split as
//       `if (id > 0xdc) {region C} else if (id == 0xdc) {case dc} else {region A}`
//       yields ONE `cmp 0xdc` (jg/je share flags) instead of `cmp 0xdd; jge` + `cmp 0xdc`.
//       (Verified in a scratch probe; the grind pass used `if (id < 0xdd)` which
//       forced the extra `cmp 0xdd`.)  -> TRY THIS FIRST.
//   (b) id-in-ECX vs id-in-EAX: retail keeps piVar4 LIVE in EAX across the dispatch
//       (it re-derefs piVar4[1]/*piVar4 in cases 0x11/0xdc/0xd7/0x124), which pushes
//       id to ECX. The candidate lets piVar4 die (id = *piVar4 consumes it) so id
//       reuses EAX. LEVER (untried): keep the piVar4 variable live/used past the
//       switch so the allocator pins it in EAX.  The `mov edx,0xdc; cmp ecx,edx`
//       register-materialized pivot likely follows once id is in ECX.
//   The "needs a different VC7.1 sub-build (13.10.6030/SP1)" idea is a HYPOTHESIS,
//   NOT established -- levers (a)/(b) above were not exhausted. Do not treat the
//   match as a compiler boundary until they are.
//
// INFRA: the parity pipeline assumed one global /O2 /Oy; catalog_parity_audit.py now
// tries /Oy and /Oy- (per-TU). /G7 handling still TODO. SCRATCH build/diff only.
// Oracle: rebuild/oracles/cfrontendmanager_action.{expected.tsv,decomp.txt}.

typedef unsigned long ulong;

// ---- cdecl / static callees (become masked relocations) ----------------------
extern "C" double __cdecl GFGetTime(void);
extern "C" void*  __cdecl SectionMgr_Get(void);                    // CTBaseSingleton<CCodeSectionManager>::Get
extern "C" void   __cdecl GameText_ConstructString(void* dst, void* src);
extern "C" char   __cdecl Compare_Unsigned16Array(int a, int b);

// ---- thiscall callees modeled as member functions (ecx=this, no edx) ---------
// Each is a real member call so VC7.1 emits `mov ecx,<this>; call` with NO edx setup.
struct Thk {
    void FileInstaller_Pause();
    void FileInstaller_Unpause();
    long* Map_op_bracket(ulong* key);
    void CCharString_ctor(const char* s, int n);
    void CCharString_dtor();
    void GameText_GetTextBySymbol(void* keyStr);
    void CTCTrophy_OnShowTrophyInitialise();
    void CWideString_assign(void* src);
    long RefreshAvailableSavedGamesForProfile();
    long RefreshAvailableProfiles();
    long RefreshAvailableProfilesForDelete();
    void ProfMgr_Save();
    void ProfMgr_DeleteProfile(void* wstr);
    void ProfMgr_SetDefaultValuesForNewProfile();
    void GotoPreviousScreen(int force);
    void GotoProfileMenu();
    void GotoNewProfileScreen();
    void GotoSpecificPreviousScreen(void* comp);
    void GotoNextScreen(void* comp, int force);
    void DoPressStart();
    void SetScoreboardOptionsFromProfile();
    void LoadProfile(void* wstr);
    void VKeyboard_Cancel();
    void VKeyboard_Confirm();
    void CCharString_AssignFromValue();
    void* CCharString_AssignFromData();
};
#define TH(p) ((Thk*)(void*)(p))

// vtable-indirect thiscall modeled as virtual calls on a 145-slot class (exact retail
// codegen: mov ecx,obj; mov eax,[obj]; call [eax+slot*4]; no edx, no thunk).
struct Eng {
  virtual int v0();
  virtual int v1();
  virtual int v2();
  virtual int v3();
  virtual int v4();
  virtual int v5();
  virtual int v6();
  virtual int v7();
  virtual int v8();
  virtual int v9();
  virtual int v10();
  virtual int v11();
  virtual int v12();
  virtual int v13();
  virtual int v14();
  virtual int v15();
  virtual int v16();
  virtual int v17();
  virtual int v18();
  virtual int v19();
  virtual int v20();
  virtual int v21();
  virtual int v22();
  virtual int v23();
  virtual int v24();
  virtual int v25();
  virtual int v26();
  virtual int v27();
  virtual int v28();
  virtual int v29();
  virtual int v30();
  virtual int v31();
  virtual int v32();
  virtual int v33();
  virtual int v34();
  virtual int v35();
  virtual int v36();
  virtual int v37();
  virtual int v38();
  virtual int v39();
  virtual int v40();
  virtual int v41();
  virtual int v42();
  virtual int v43();
  virtual int v44();
  virtual int v45();
  virtual int v46();
  virtual int v47();
  virtual int v48();
  virtual int v49();
  virtual int v50();
  virtual int v51();
  virtual int v52();
  virtual int v53();
  virtual int v54();
  virtual int v55();
  virtual int v56();
  virtual int v57();
  virtual int v58();
  virtual int v59();
  virtual int v60();
  virtual int v61();
  virtual int v62();
  virtual int v63();
  virtual int v64();
  virtual int v65();
  virtual int v66();
  virtual int v67();
  virtual int v68();
  virtual int v69();
  virtual int v70();
  virtual int v71();
  virtual int v72();
  virtual int v73();
  virtual int v74();
  virtual int v75();
  virtual int v76();
  virtual int v77();
  virtual int v78();
  virtual int v79();
  virtual int v80();
  virtual int v81();
  virtual int v82();
  virtual int v83();
  virtual int v84();
  virtual int v85();
  virtual int v86();
  virtual int v87();
  virtual int v88();
  virtual int v89();
  virtual int v90();
  virtual int v91();
  virtual int v92();
  virtual int v93();
  virtual int v94();
  virtual int v95();
  virtual int v96();
  virtual int v97();
  virtual int v98();
  virtual int v99();
  virtual int v100();
  virtual int v101();
  virtual int v102();
  virtual int v103();
  virtual int v104();
  virtual int v105();
  virtual int v106();
  virtual int v107();
  virtual int v108();
  virtual int v109();
  virtual int v110();
  virtual int v111();
  virtual int v112();
  virtual int v113();
  virtual int v114();
  virtual int v115();
  virtual int v116();
  virtual int v117();
  virtual int v118();
  virtual int v119();
  virtual int v120();
  virtual int v121();
  virtual int v122();
  virtual int v123();
  virtual int v124();
  virtual int v125();
  virtual int v126();
  virtual int v127();
  virtual int v128();
  virtual int v129();
  virtual int v130();
  virtual int v131();
  virtual int v132();
  virtual int v133();
  virtual int v134();
  virtual int v135();
  virtual int v136();
  virtual int v137();
  virtual int v138();
  virtual int v139();
  virtual int v140();
  virtual int v141();
  virtual int v142();
  virtual int v143();
  virtual int v144(void*, int);
};



// ---- globals (masked relocations) --------------------------------------------
extern "C" int   DAT_013ca818;   // installer-active flag
extern "C" void* DAT_013b871c;   // game/singleton root
extern "C" char  DAT_0122d820[]; // string data (case 0xd7)

// ---- byte accessors ----------------------------------------------------------
#define B(o)  (*(unsigned char*)((char*)mgr + (o)))
#define U(o)  (*(ulong*)((char*)mgr + (o)))
#define P(o)  (*(void**)((char*)mgr + (o)))
#define D(o)  (*(double*)((char*)mgr + (o)))

extern "C" void __fastcall
CFrontEndManager_Action_0059a238(void* mgr /*ecx->esi*/, void* /*edx*/, void* param_1 /*[ebp+8]*/)
{
    // 5 stack temps = sub esp,0x14 : local_18/14 (CCharString), local_10 (CWideString ptr),
    // local_c/8 (CCharString).  Declared so their dtors emit LIFO in the 0xd7 path.
    char local_8[4], local_c[4], local_18[4], local_14[4];
    void* volatile local_10;

    long* plVar7;              // op[] result, shared tail
    char CVar11 = 0;           // tail bool -> lives in ebx (xor ebx,ebx), 0 in most paths

    D(0xd0) = GFGetTime();

    if (B(0xd8) == 0 && B(0xd9) == 0 && DAT_013ca818 != 0) {
        void* mn = SectionMgr_Get();
        TH(mn)->FileInstaller_Pause();
        B(0xd8) = 1;
    }

    int* piVar4 = *(int**)param_1;             // action ptr (retail keeps this in eax)
    int iVar1 = *piVar4;                         // action->id (retail: ecx)

    // Dispatch is the retail nested range-search if-tree (decomp lines 47-327), NOT a
    // flat switch: MSVC otherwise builds its own tree pivoting at 0x42. Mirroring the
    // decomp's `if (iVar1 < 0xdd) {...} else if (iVar1 < 0x128) {...} else {...}` split
    // reproduces the retail pivot at 0xdd/0xdc.
    if (iVar1 > 0xdc) {
        if (iVar1 < 0x128) {
        if (iVar1 == 0x127) {
            if (P(0x60) == 0) return;
            TH(P(0x60))->VKeyboard_Cancel();
            return;
        }
        if (iVar1 == 0xe5) {
            TH(mgr)->DoPressStart();
            return;
        }
        if (iVar1 == 0xfa) {
            long lVar6 = TH(mgr)->RefreshAvailableProfilesForDelete();
            if (lVar6 == 0) *(ulong*)&param_1 = 0xc;
            else            *(ulong*)&param_1 = 0x15;
        }
        else {
            if (iVar1 != 0x11b) {
                if (iVar1 != 0x124) {
                    if (iVar1 == 0x125) {
                        void* mn = SectionMgr_Get();
                        TH(mn)->ProfMgr_SetDefaultValuesForNewProfile();
                        TH(mgr)->GotoNewProfileScreen();
                        return;
                    }
                    if (iVar1 != 0x126) return;
                    if (P(0x60) == 0) return;
                    TH(P(0x60))->VKeyboard_Confirm();
                    return;
                }
                piVar4 = (int*)piVar4[1];
                if (*(int*)piVar4 != (int)piVar4)
                    TH(mgr)->LoadProfile((void*)(*(int*)(piVar4[1] + 8) + 8));
                *(ulong*)&param_1 = 0;
                plVar7 = TH((char*)mgr + 0x54)->Map_op_bracket((ulong*)&param_1);
                CVar11 = 1;
                goto LAB_0059a768;
            }
            *(ulong*)&param_1 = 0x16;
        }
        }
        else {
        if (iVar1 == 0x128) {
            ((Eng*)(DAT_013b871c))->v4();
            return;
        }
        if (iVar1 == 0x129)       { *(ulong*)&param_1 = 0x18; }
        else if (iVar1 == 0x138)  {
            TH(mgr)->SetScoreboardOptionsFromProfile();
            *(ulong*)&param_1 = 0x19;
        }
        else if (iVar1 == 0x13a)  { *(ulong*)&param_1 = 0x1a; }
        else if (iVar1 == 0x141)  { *(ulong*)&param_1 = 0x1c; }
        else {
            if (iVar1 != 0xbd1) return;
            *(ulong*)&param_1 = 0x14;
        }
        }
    }
    else if (iVar1 == 0xdc) {                          // INVALID_SAVE
            if (*(int**)piVar4[1] != (int*)piVar4[1]) {
                *(ulong*)&param_1 = 0x10;
                plVar7 = TH((char*)mgr + 0x54)->Map_op_bracket((ulong*)&param_1);
                iVar1 = *(int*)*plVar7;
                TH(local_c)->CCharString_ctor("UI_INVALID_SAVE_TEXT", -1);
                piVar4 = (int*)((Eng*)((void*)iVar1))->v3();
                iVar1 = *(int*)(*(int**)(*(int*)param_1 + 4) + 2);
                if (piVar4 != 0 && iVar1 != 0) {
                    TH(local_c)->CCharString_ctor("TEXT_GUI_MENU_INVALID_SAVE", -1);
                    TH(*(void**)((char*)DAT_013b871c + 0x60))->GameText_GetTextBySymbol(&param_1);
                    GameText_ConstructString(&param_1, (void*)(iVar1 + 8));
                    ((Eng*)((void*)piVar4))->v144(&param_1, 1);
                    TH(&param_1)->CCharString_dtor();
                }
            }
            *(ulong*)&param_1 = 0x10;
    }
    else {
        if (iVar1 < 0x12) {
            if (iVar1 == 0x11) {
                piVar4 = (int*)piVar4[1];
                if (*(int*)piVar4 == (int)piVar4) return;
                iVar1 = *(int*)(piVar4[1] + 8);
                TH((char*)mgr + 0x20)->CTCTrophy_OnShowTrophyInitialise();
                piVar4 = (int*)0;                // *extraout_EAX residue
                P(0xa8) = piVar4;
                ((Eng*)((void*)piVar4))->v48();
                TH((char*)mgr + 0xac)->CWideString_assign((void*)(iVar1 + 8));
                return;
            }
            if (iVar1 == 9)       { *(ulong*)&param_1 = 1; }
            else if (iVar1 == 0xb){ *(ulong*)&param_1 = 3; }
            else if (iVar1 == 0xc){ *(ulong*)&param_1 = 4; }
            else if (iVar1 == 0xd){ *(ulong*)&param_1 = 5; }
            else {
                if (iVar1 != 0xe) {
                    if (iVar1 == 0xf) {
                        if (B(0xd8) != 0 && DAT_013ca818 != 0) {
                            void* mn = SectionMgr_Get();
                            TH(mn)->FileInstaller_Unpause();
                            B(0xd8) = 0;
                            B(0xd9) = 1;
                        }
                        piVar4 = *(int**)((char*)mgr + 0x1c);
                        ((Eng*)((void*)piVar4))->v4();
                        *(char*)((int)piVar4 + 0x29) = 1;
                        return;
                    }
                    if (iVar1 != 0x10) return;
                    TH(mgr)->GotoProfileMenu();
                    return;
                }
                *(ulong*)&param_1 = 6;
            }
        }
        else if (iVar1 == 0x42) {
            long lVar6 = TH(mgr)->RefreshAvailableSavedGamesForProfile();
            if (lVar6 == 0 && U(0xbc) == 0) {
                B(0x64) = 1;
                void* mn = SectionMgr_Get();
                TH(mn)->ProfMgr_Save();
                TH((char*)mgr + 0x20)->CTCTrophy_OnShowTrophyInitialise();
                piVar4 = (int*)0;
                P(0xa4) = piVar4;
                ((Eng*)((void*)piVar4))->v48();
                return;
            }
            *(ulong*)&param_1 = 8;
        }
        else if (iVar1 == 0x43) {
            *(ulong*)&param_1 = 9;
        }
        else {
            if (iVar1 == 0x56) {
                TH(mgr)->GotoPreviousScreen(0);
                return;
            }
            if (iVar1 == 0xd6) {
                char confirmed = 0;
                void* mn0 = SectionMgr_Get();
                if (*(char*)((char*)mn0 + 0xc) == 0) {
                    void* mn1 = SectionMgr_Get();
                    int a = *(int*)((char*)mn1 + 8);
                    if (a != *(int*)((char*)mgr + 0xb0)) {
                        if (a == 0 || *(int*)((char*)mgr + 0xb0) == 0)
                            confirmed = 0;
                        else
                            confirmed = Compare_Unsigned16Array(a, *(int*)((char*)mgr + 0xb0));
                        if (confirmed == 0) goto LAB_0059a4f1;
                    }
                    confirmed = 1;
                }
            LAB_0059a4f1:;
                void* mn2 = SectionMgr_Get();
                TH(mn2)->ProfMgr_DeleteProfile((char*)mgr + 0xb0);
                long lVar6 = TH(mgr)->RefreshAvailableProfiles();
                if (confirmed == 0 && lVar6 != 0) {
                    *(ulong*)&param_1 = 7;
                    plVar7 = TH((char*)mgr + 0x54)->Map_op_bracket((ulong*)&param_1);
                    TH(mgr)->GotoSpecificPreviousScreen((void*)*plVar7);
                    return;
                }
                *(ulong*)&param_1 = 0x14;
                plVar7 = TH((char*)mgr + 0x54)->Map_op_bracket((ulong*)&param_1);
                TH(mgr)->GotoNextScreen((void*)*plVar7, 1);
                if (lVar6 < 1) return;
                ulong saved = U(0x98);
                *(ulong*)&param_1 = 7;
                plVar7 = TH((char*)mgr + 0x54)->Map_op_bracket((ulong*)&param_1);
                TH(mgr)->GotoNextScreen((void*)*plVar7, 0);
                U(0x98) = saved;
                return;
            }
            if (iVar1 == 0xd7) {
                piVar4 = (int*)piVar4[1];
                if (*(int*)piVar4 == (int)piVar4) return;
                local_10 = (void*)(*(int*)(piVar4[1] + 8) + 8);
                TH((char*)mgr + 0xb0)->CWideString_assign((void*)local_10);
                *(ulong*)&param_1 = 0xa;
                plVar7 = TH((char*)mgr + 0x54)->Map_op_bracket((ulong*)&param_1);
                iVar1 = *(int*)*plVar7;
                TH(local_c)->CCharString_ctor("UI_TEXT_DELETE_PROFILE_MENU_TITLE", -1);
                piVar4 = (int*)((Eng*)((void*)iVar1))->v3();
                if (piVar4 != 0) {
                    TH(local_8)->CCharString_ctor("TEXT_GUI_MENU_DELETE_PROFILE", -1);
                    TH(*(void**)((char*)DAT_013b871c + 0x60))->GameText_GetTextBySymbol(&param_1);
                    iVar1 = *piVar4;
                    void* pCVar9 = (void*)local_10;
                    TH((void*)DAT_0122d820)->CCharString_AssignFromValue();
                    void* uVar5 = TH(pCVar9)->CCharString_AssignFromData();
                    ((Eng*)((void*)iVar1))->v144(uVar5, 1);
                    TH(local_14)->CCharString_dtor();
                    TH(local_18)->CCharString_dtor();
                    TH(&param_1)->CCharString_dtor();
                }
                *(ulong*)&param_1 = 0xa;
                plVar7 = TH((char*)mgr + 0x54)->Map_op_bracket((ulong*)&param_1);
                CVar11 = 0;
                goto LAB_0059a768;
            }
            if (iVar1 != 0xdb) return;
            *(ulong*)&param_1 = 0xf;
        }
    }

    plVar7 = TH((char*)mgr + 0x54)->Map_op_bracket((ulong*)&param_1);
    CVar11 = 0;
LAB_0059a768:
    TH(mgr)->GotoNextScreen((void*)*plVar7, CVar11);
    return;
}
