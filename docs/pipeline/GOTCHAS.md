# Gotchas (living list)

One line per solved problem. Moved out of `CLAUDE.md` on 2026-09-07 so the agent guide stays
short; **append here** when you solve something real. Newest at the bottom of each section.

## Ghidra / symbols
- Ghidra `X86FunctionPurgeAnalyzer` on huge binaries (165k fns) goes log-silent for hours in a
  quadratic progress-reporting loop — it's working, not hung; verify with jstack + CPU sampling,
  never kill (headless saves only after analysis completes).
- PDB names can contain whitespace ("dynamic initializer for 'x'") which Ghidra symbols reject —
  `ApplyNames.java` sanitizes `\s+`→`_` before `setName`.
- Preserve MSVC decorated names on the first PDB/BSim `setName` attempt so `DemangleAll.java` still
  works; only fall back to `SymbolUtilities.replaceInvalidChars(...)` after Ghidra rejects a name.
- RTTI vtable-slot ports beat low-confidence BSim guesses when slot counts align; preserve the
  compare TSV, then use `LabelApplyForce.java` and demangle.
- `analyzeHeadless.bat` script args: cmd.exe splits on `=`, so `name=0xaddr` arrives as TWO args —
  pass alternating `name addr` pairs instead.
- Ghidra DB after RTTI force pass: 49,082 functions, 40,187 named, 8,895 default-named. Bulk RTTI
  port source is `ghidra_out/labels_rtti_port.tsv`; conflict audit is `ghidra_out/rtti_port_compare.tsv`.
- Quest logic is compiled C++ (161-entry name→allocator table @ 0x00CD52D0, no quest VM) — see
  `docs/engine/QUEST_SCRIPTS.md#quest-binding-compiled-c-classes`. Trust FSE ASLR addresses over BSim names when they clash (0x00CB8110 is
  the CScriptBase ctor, not "CHeroMorphDef"; 0x00CBFAB8 is SetScriptActiveStatus).
- Manifest module labels are NOT trustworthy per file: Ghidra propagates one name over every
  byte-identical body (2,977 "CLandscapeBackgroundPatch::vector_deleting_destructor" rows are dtors of
  many classes). Use `tools/decomp_pipeline/label_trust.py` (address-unique `(module, leaf)` pairs)
  before attributing a function to a class.

## Decomp harness (`tools/decomp_pipeline`)
- `verify_and_land.py` decodes `&lt; &gt; &quot; &apos; &amp;` in payloads, rewrites `__thiscall`→
  `__fastcall`, and strips every line containing lowercase `static_assert`. Never write the literal
  `__thiscall` keyword (VC7.1 C4234); use `FABLE_STATIC_ASSERT` from `rebuild_abi.h` for size checks.
- Workflow `args` arrive in the script as a JSON **string**, not a parsed value — guard with
  `const items = typeof args === 'string' ? JSON.parse(args) : args`.
- EMBEDDED JUMP TABLES (switch >~4 dense cases) cannot be verified by `verify_and_land.py` (objdump
  splits the body at `$Lxxx` labels and elides the zeroed table). Use
  `tools/decomp_pipeline/verify_land_jumptable.py <land.json> <oracle.tsv> [--land]` (raw-COFF
  extraction). Proven on `CKeyRedefiner::GetSubTypeForAction` 0x557CA0 and `AreAllowedToCoexist` 0x5578A0.
- Manifest boundary OVER-CAPTURE: an oracle span `[addr, next_manifest_addr)` swallows int3 padding +
  the next function's head when the next fn isn't in the manifest. Fix with
  `tools/decomp_pipeline/trim_overcapture.py 0x<addr>` / `--oracle <tsv>`; then author the single fn.
  A VOID member forwarder gets tail-call-optimised to `jmp` by VC7.1 while retail kept `push;call;ret`
  (same-length DIFFER, not recoverable); value-returning and cdecl-cleanup forwarders recover byte-exact.
  Backlog: `rebuild/backlog/overcapture-recovery-worklist.tsv`.
- Shared engine headers (`rebuild/include/engine/<Class>.h`, generated from the PDB layouts by
  `gen_class_headers.py`) are the required way to model `this`; `retype_landed.py` converts landed files
  and re-proves parity. A `char`-vs-`bool` field type change DOES change bytes when the store comes from
  a differently typed parameter (0088e920 went MATCH→DIFFER) — the parity gate decides, never assume.
- Heredoc-fed Python that contains `\b`/`\w` regexes gets its backslashes eaten twice in this shell and
  writes real backspace bytes into the file. Write regex-bearing source with the Write/Edit tools.
- A file named `CON` (or any reserved device name) at the repo root makes every `git add`/`status`
  hang forever on Windows. Delete with `del "\\?\D:\Documents\FableTLC\CON"`; a killed git leaves a
  stale `.git/index.lock` to remove.

## Build / frontend
- WinLibs mingw64 g++ builds can die at startup with 0xC0000139 (entrypoint not found) from runtime
  DLL mismatches on PATH — link `-static -static-libgcc -static-libstdc++` (done in FableForge).
- Adding a D3D9 frontend texture (`visual_boot_d3d9.cpp`) requires registering it in BOTH the
  `FableInitialiseVisualD3D9` upload chain AND the `VisualRender2DAdapter`
  `RENDER2D_ADAPTER_ATTACH_TEXTURE` chain; miss the attach chain and the quads draw FLAT WHITE.
  Free frontend resource ids: 101-120 taken; 116-119 are WAVE; About=120, spooky=121/122.
- Frontend visual QA runs headlessly: `rebuild/build_bootstrap.ps1 -RetailFrontendBank <frontend.big>`,
  launch the VisualCheckpoint exe, synth-click via SetCursorPos+mouse_event, screenshot with
  Graphics.CopyFromScreen. `$PID` is read-only in PS. Recipe: `docs/pipeline/VISUAL_PARITY_STATUS.md`.
- Retail Fable.exe must be CLOSED while building: WinMain's real single-instance mutex fails the
  `FABLETLC_WINMAIN_BEHAVIOR` fixture (code=2).
- Profile-name font doubling (`AppendProfileNameText`, ENG_ARIAL): fixed by a half-texel UV inset in
  commit d0becd0 (2026-08-10, merged to main 2026-09-07). Re-verify visually after any Render2D change.

## Formats / data (see `docs/formats/`)
- Fable name hash = **crc0** = reflected CRC-32 poly 0xEDB88320, seed 0, NO final inversion
  (`CCharString::ComputeCRC32` 0x00404310). NOT `0xFFFFFFFF-crc32`, NOT zlib. A game.bin def APPEND
  resolves only if the names.bin CRC is crc0 AND the payload's self global-entry-index back-refs are
  retargeted (component sub-defs are SHARED). See `docs/formats/DEF_LOAD_CONTRACT.md`.
- 3DAF anim payload = `u32 decompSize` + ONE raw LZO1X stream; chunks are `[fourcc][u32 size]`, one
  XSEQ per bone track. EgoCore (`C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master`) is the
  whole-format answer key — check it BEFORE byte-RE; do NOT copy its `CDefStringTable::GetCRC` or
  `classIndex=0`. `docs/formats/BIG_ANIM_FORMAT.md` §9, `docs/engine/DEMON_DOOR_FACE.md`.
- Texture payloads: only MIP 0 is chunked-LZO; mips 1..n-1 are RAW. Info+24 (MipSize0) = mip-0 region
  size, 0 = all-raw. DXT3 Info tail is `02 08`. Writer `tools/texture_build.py`; `docs/formats/TEXTURE_WRITER.md`.
- Compiled-mesh `LODSizes[]` are byte SIZES; every retail 1-LOD type-1 entry appends an UNCOUNTED ghost
  LOD; material lists end with a `DegenerateTriangles` sentinel (STATIC meshes only). Skinned weights sum
  exactly 255, max 3 influences. `docs/formats/MESH_COMPOSE.md`.
- Save edits: any SAVED_ENTITIES cell edit must patch the 36-byte cell descriptor AND sectionLen AND
  chunk1_ulen, then re-sign. `tools/save_edit.py`; grammar in `docs/formats/SAVE_ENTITY_GRAPH.md` §9.5.
- Dialogue join: `data\Defs\<bank>snds.bin` = sorted {crc0("SND_"+entryName), soundID}; soundID == .lut
  clip Index == dialogue.big LIPSYNC id. `tools/dialogue_pipeline.py`, `docs/formats/DIALOGUE_PIPELINE.md`.
- Chest facts: `CChestDef::Transfer` is retail 0x004DE204; `OpenerObject` +0x34, `OpenersRequired`
  +0x38; rewards in `CContainerRewardHeroDef::ObjectFamilies` +0x28.
- New-map registration REQUIRES a FinalAlbion_RT.stb common-header chunk (unchecked map lookup in
  OpenRetailStaticMap 0xB41E50 → CTD 0xA2428A). WER Application-log fault offsets +0x400000 = Ghidra VA.
- Region vector is hard-capped at 142 (owner must be ≤141); the ForgeTest crash at 0x7dd1d3 is
  `CEngineLandscapeMap::OpenStaticMap` zero-filling `malloc(field_04)` with no null check.
- Custom terrain textures: foreground triples are GBANK_MAIN_PC entry IDs in textures.big (156
  `UNASSIGNED_*` 512² slots); pipeline `texture_build.py replace` + `forge stb settex`.

## Modding toolchain (lives in FableForge / ForgeFSE now — `docs/modding/README.md`)
- Active build target since 2026-07-18: FableForge (`D:\Code\FableForge`); FQT is a donor, not the target.
- WLD/BWD authoring is `forge wld compile` / `forge world add-level|install-level|attach-map` /
  `forge stb settex`; `forge validate` cross-checks BWD↔WLD↔STB.
- Do not activate a quest during ForgeTest terrain teleports (quest-region abandon modal blocks
  validation). Wasp Menace renders correctly through ForgeFSE, so the base quest API works.
- Mario rig (`work/mario_hero/stage_bindaxis4`): parent-relative ANIM translations stretch the
  segments; mesh-only rest/inverse-bind edits are insufficient. Parked.
- ForgeFSE canonical fork is `D:\Code\ForgeFSE-retail-shadow`; `D:\Code\ForgeFSE` is stale.
- `validate_tooling_sdk.py` FAILs with "mirror drift" whenever `export_fse_native_overlay.py` / `gen_fable_engine_header.py` ran with `--output`/`--no-mirrors`; the mirrors are plain byte copies, so re-run the generator in default mode (or copy the canonical file) before trusting the gate.
- ForgeFSE `LuaEntityAPI` nested control: never request a second `StartScriptingEntity` resource for an actor the same VM already controls; the engine treats Forge handles as foreign owners and never grants it (Affair Wife hang, 2026-09-11). Nested acquires must reuse the live handle; a re-acquire at the held priority is idempotent (retail Main loops re-call StartScriptingEntity on the same resource every iteration), only a priority change counts a depth level; `audit_forgefse_control_abi.py` pins this.
- Installed `FinalAlbion.qst` may still carry `AddQuest("NewOakValeIntro", TRUE)` from the additive-quest era; under the identity-preserving override it is a competing authority (`NOVI_AUTHORITY ... legacy=true`). Check it before every single-authority run.
- Retail colour immediates are written as B,G,R,A memory bytes (`CRGBColour`); read them in memory order, never as RGBA (Bully bar `00 00 FF FF` is red). Hand-transcribed byte strings in snapshot exporters must be checked against a capstone disassembly of the installed exe.

- Git Bash heredoc / `python -c` patches mangle backslashes: a regex replacement `\1` lands as a literal 0x01 byte. Write patch scripts to a file and run them (2026-09-16).
- Debug-PDB (Ego_r) class offsets → retail: quest classes −0x14, entities 0, and −4 per STL container member preceding the field (debug iterator pointer); CScriptThing is 12 bytes in both (2026-09-16).
- Ghidra `__thiscall` FunctionDefinitions need `this` as an explicit first parameter or every argument shifts one slot (2026-09-16).
- Ghidra provenance/stack tracking: stack keys must be entry-relative (`disp - espDelta`), and a mid-function `pop/add esp/ret` epilogue must not carry its depth into the next block — otherwise every parameter read after an early return is shifted by one (Orchard `MakeTeamMemberComment`).
- Retail passes `CScriptThing` BY VALUE to many GSI slots (`sub esp,0xc; mov ecx,esp; push src; call 0x4ABE90`) even where FSE's typedef says `CScriptThing *`; type those sites as 12-byte structs or the decompiler misattributes every pushed immediate.
- A CScriptThing's Data pointer (+4) shares the CScriptThing slot layout; `(**(**(int **)(obj+off+4) + 0x12c))()` is `thing:IsAlive()` on the member at `off`.
- Ghidra prints some namespaced call labels as `Ns__Fn` in C output while the call list says `Ns::Fn`; match both.
- Ghidra's stack-variable names drift after callee-cleaned vtable calls (it never learns the purge), so one slot prints as `auStack_a8`, `&uStack_b8` and `auStack_b0 + 4` in one function; the typed export now records true entry-relative slots per call site (`ecxStack/edxStack/pushedStack`) and `restore_stack_operands` renames by (slot, lifetime) — never trust two Ghidra stack names to be different objects, nor one name to be one object (2026-09-16).
- Ghidra `Instruction.getOperandType()` is a flag set: a register written by LEA is `REGISTER|ADDRESS` (0x2200), so `== OperandType.REGISTER` silently fails; use `OperandType.isRegister(t)` and check the single `Register` object (2026-09-16).
- Exact stack purge: `Function.getStackPurgeSize()` for direct callees (clamp: values > 0x40 are Ghidra "unknown"), the resolved slot prototype's stack-parameter bytes for vtable calls (by-value CScriptThing = 12); VC7.1 pushes callee-saved registers lazily inside the body (`push ebp` at the first call) — only the registers the epilogue pops are saves; `lea esp,[esp]` is padding, not a frame reset (2026-09-16).
- Patch scripts: a heredoc (`python - <<'EOF'`) still mangles backslash escapes (`\n`, `\)`, `\w`) inside Python string literals on this shell (this very line was mangled once), and a `r"""..."""` patch body cannot contain a `"""` docstring — write patch scripts with the Write tool and use `'''` for embedded docstrings (2026-09-16).
- Appended `GBANK_MAIN_PC` texture entries load without crashing but are NOT found by `MiniMapGraphic` name (RetailHeaders `textures.h` does not fix it); retail resolves bank symbols some other way -- replace an unreferenced retail slot instead; investigation steps in `D:\Code\AlbionAtlas\docs\PLAN.md` (2026-09-16).
- The engine reads `FinalAlbion.bwd` from three places (root, `data/Levels`, `data/Levels/FinalAlbion`) -- write all three; region names are cached in saves (new game to see a rename).
- The LEV walkable byte is inert at runtime; creatures follow the nav quadtree in the LEV's navigation sections (in-game proven 2026-09-16).
- Ghidra prints out-of-line blocks (shared cleanups behind `LAB_x:`) after the function's return: the k-th printed call is NOT the k-th call site by address. Pair through the decompiler token stream (`callOrder` in ExportTypedTranslationUnit; `_text_order_sites`).
- `__ftol2` (0xBFEA70) takes its operand in ST0: without a custom-storage `float10` parameter Ghidra prints `uVar = __ftol2();` and the value is gone.
- `infer_helper_prototypes.py --unit` must run after `ghidra_typing_spec.py --unit` — the spec generator overwrites typing_spec.json without the 31 inferred helpers (helpers typed 218 -> 187 silently).
- CCountedPointer/`CScriptThing` slot names: Ghidra names a thing's Data member (`piStack_14`) separately from the object (`auStack_18`); after canonicalisation both are the object, so a rule that rewrites `*P + SLOT` to `*(int *)P + SLOT` must be accepted by the later stack-thing vcall rule.
- Never `python - <<'EOF'` a patch containing regex backslashes: bash heredocs keep them but the Python source then has `\(`/`\w` in non-raw strings and the asserts fail (or worse, silently mangle). Write the patch file with the Write tool (or use Edit).
- A Lua boolean compared with a number is never equal: `c = !(x != 0); if (c != '\0')` must lift to `if c`, not `if c ~= 0` (always true). Type `not`/comparison values as bool in the lifter, and take `bool __thiscall` from the bsim signature over Ghidra's `int`.
- Ghidra prints a retyped class member without its class (`MakeTeamMemberComment(` for `NScript::...::MakeTeamMemberComment`); pair call sites by the bare member name too, else its stack operands never restore (`"FETCHING" + 4` = a string at the wrong slot).
- FSE `quest:NewScriptFrame(me)` returns `not terminating` of the entity host (same predicate as `quest:IsActiveThreadTerminating()` in an entity VM) for lifetime-None quests, but ALWAYS true under nativeLifetime NewOakValeIntro — fold the frame+check idiom only for units (`build_readable_unit.py --frame-keeps-checks` otherwise).
- Readable-style rewrites must go through the flow graph: `if not T() then BODY end` is only `if T() then return end; BODY` when the block's end reaches nothing but `end`/`return` lines; a temporary moves to its read only along straight-line statements that do not touch it (and only past queries when its value is a call).
- Heredoc-mangled regexes can leave a literal BACKSPACE byte (0x08) in a `.py` where `\b` was meant: the pattern silently never matches (`native_goto_scopes.py` / `smoke_run_unit.py` shipped that way for weeks; `fold_local_thing_vectors` end-slot rule too). Scan with `[c for c in bytes if c < 32 and c not in (9,10,13)]` after any shell-scripted patch; edit tools only with the Write/Edit tools.
- Ghidra's `PATTERNS` blanket rule "`(**(code **)(*piVarN + OFF))(` is a GSI vcall" is wrong once the register walks a vector of things (`piVar6 = *this_00; ... (*piVar6 + 8)` is `GetDefName` on an element, not `GSI->Error`): a register is the interface only while its latest definition (text order, copies followed) is an interface load — `_is_gsi_alias_at`.
- `RE_CAST` in the lifter ate `(CVar5)` as a type cast (`[A-Z]\w+` matches any capitalised register): `LIST_At((CVar5) / 0xc)` became `LIST_At( / 0xc)`. Register names (`\w*Var\d+`) are never casts.
- The typed export's per-site `pushedStack` is attributed by proximity, not by callee purge: a `push 15.0` for a later direct call lands on the `GetHero` vtable site before it. Count operands from the signature (manifest params / decorated CScriptThing vtable name, +1 for a by-value string/vector result — a CScriptThing result travels in eax) and skip nested calls' pushes when reading operands back from the machine code (`recover_dropped_operands`).
- Site `depth` includes the pushed operand itself: a `lea reg,[esp+N]; push reg` slot is entry-relative `-depth + 4*(pushes after the lea, this one included) + N` (Orchard `IsBeingCarriedBy` string temp: -76, not -80).
- The decompiler's text order is NOT address order in restructured functions (TraderToRescue's `_INTRO` AppendCString is first in text, last in address): pair printed calls with sites only through `callOrder` (`_text_order_sites`), never by position among same-label calls. Any rewrite that deletes a printed call head (e.g. a CCharString temp constructor) breaks that pairing for the whole function.
- Ghidra names the Lua-binding signature, not the native one: `AddLineToConversation` pushes (id, text, showSubtitle, speaker, listener) natively while the binding is (id, text, speaker, listener, showSubtitle) — recovered operands are placed by type, never by position.
- Retail `CScriptDef` (the global game data at `DAT_0143e90c`, script.bin SCRIPT_DEF entry 597) is the Ego_r PDB layout minus its 64-byte header, with `vector<float>` / `<long>` / `<CDefString>` members 4 bytes smaller (12 vs 16); `vector<CCharString>` and the struct vectors keep 16. Anchors: OVI_MoralityChangePerDeed 0xd64, GUI_MeleeBeetles 0xf10, AmbushScamRenown 0x270. `tools/script_recovery/script_def_offsets.py` writes the table; readable output names every read (`SCRIPT_DEF.TCE_GuardRangeLow`).
- Ghidra gives one register name to two values when the compiler reloads a spilled local into the same register (AppleGirl: timer id `ebx` reloaded from `[esp+0x10]` at the loop head, conversation id also `iVar4`): a draft that keeps the register name reads the wrong value from the second iteration. Timer calls go through the stack copy made right after `RegisterTimer` (lowering); watch for the shape on other spilled ids.
- Ghidra hoists a call's register set-up above an inlined construction (`pScriptObject = local_10;` before `local_10[0] = &PTR_vtable`): after the vtable store becomes `local_10 = NewResource()` the alias must be moved below it (`hoist_object_aliases`) or the lifter drops it as a pre-definition read.
- `_text_order_sites` bails for the whole function when one printed vtable head is missed: check `RE_VCALL_HEAD_ANY` covers every Ghidra spelling (`(*(code *)(*(int **)(x + 0xc))[1])()` was missing) before blaming `callOrder`.
- `unwrap_statements` re-joins Ghidra's wrapped expressions with a space, so any lowering rule that matches a reassembly by exact string (`CONCAT13(uVar19,CONCAT12(...))`) silently matches only the unwrapped sites: `fold_byte_split_pointers` folded one acquire per function for weeks. Match those with a `\s*`-tolerant regex, and let the statement separators inside a multi-line pattern be `\s*` too (a deeply indented body leaves `>> 0x10)\n    ;`).
- Ghidra spells one stack slot two ways in the same body: by its declaration (`ppuStack_f8`) and, where the slot is only ever taken by address, as a raw frame offset (`&stack0xffffff04`). A local `X_f8` is at entry-SP - (0xf8 + 4), so the two meet — `resolve_stack_offset_names`. Un-resolved, the object's construction, acquire and destructor land on two different names.
- The inlined resource destructor does not always have a vtable-reset line: where the object is already the base, only the member zero-stores precede the base dtor call. `canonicalise_stack_objects` then re-spells that member store under the object's own name, so `resource = 0` reaches the lifter and every later use comes out `nil` (that was `TryAcquire(0, me, 4)`). Fold that shape only for an object the function actually acquired.
- A chunk handed to `split_hoisted_locals` may itself begin `local function __resource_main(...)` (the Oakvale husband candidate): a closure scan that does not skip the chunk's own header blanks the whole body, and the splitter silently stops splitting. The Oakvale readable gate (`build_readable_new_oakvale.py`) is what catches this — the draft gate does not.
- The local-splitter's Lua 200-local budget must be spent on the *widest* splits first. A register Ghidra reused 44 ways is the one that reads as noise unsplit, and each version earns its own role name; declaration order or narrowest-first both leave hundreds more `scratchValue` (GuildTraining: 2467 / 2324 / 1927 semantic names for widest / declaration / narrowest).
- Run the role renamer twice: the style folds drop assignments, so a slot whose five draft assignments disagreed (-> `scratchValue`) often has one survivor afterwards. Put the renamer's own fallback spelling in `GENERATED` so the second pass sees it, and make the fallback refuse to renumber its own names or every untouched `scratchValue` shifts index.
- When a temporary's assignments say nothing, the API that consumes it does (`use_role`): `DeregisterTimer(X)` is a timer id, `AddPersonToConversation(X, ..)` a conversation id, `TryAcquire(X, hero, ..)` hero's control. This use-side rule removed ~3x more `scratchValue` than any new right-hand-side pattern.
- Count `scratchValue` with `--include='*.lua'`: a bare `grep -r` over the lifted directory also reads READABLE_REPORT.json and roughly doubles the number.
- `Path.read_text()` translates CRLF to LF, so round-tripping a CRLF doc through `read_text`/`write_text` silently rewrites every line ending. Patch docs with `read_bytes`/`write_bytes`.
- A retail cleanup epilogue is rarely straight-line at one indent: the native jump target sits inside an `if` and the cleanup continues after that block's `end`. Walking out through enclosing if/do blocks (never a loop `end` — that is a back edge) found 8 more regions and stopped 27 early exits from dropping their releases. But such a region must NOT be rewritten in place: the `end`s it crosses belong to those blocks, and deleting them unbalances the function (two files failed `fileSyntaxPassed` before that guard).
- An epilogue that finishes with `goto FLOW_after_lab_x` really ends in that label's block; take its statements into the region so the call sites reproduce the whole thing. A region that delegates to another region must take nothing, or the shared tail is inlined twice and the same resource is released twice.
- `-- TODO(native): goto LAB_x` is not one problem: most of the remaining sites target a region with an EMPTY body (nothing to clean up, only the jump is unexpressible). Count the non-empty ones before treating goto residue as a leak.
