> ⚠ **SCOPE CORRECTION 2026-08-16.** The `UI_QUEST_SPRITE_CORE/OPTIONAL/VIGNETTE` sprites
> this doc reverses are the small **quest-TYPE SEAL / ORB MARKER** (32×32, Height/Width=0, an
> overlay used in *two* render paths incl. the HUD/list), **NOT** the quest card's main
> picture. The card **body** is `UI_QUEST_CARD_MODEL` (game.bin entry 7546, 200×200,
> `MeshType=5`, `GraphicIndex=0`) — a **3D card model** whose look comes from a mesh + its own
> textures (the mesh/graphics banks), not a flat UI-sprite GraphicIndex. So "custom card art"
> splits: (a) the **type seal/orb** = editable via the GraphicIndex path below (real, small);
> (b) the **card body/parchment** = a 3D-model job (mesh + texture pipeline), a separate/bigger
> effort. In retail every quest shares the same card model + one of 3 type seals; only the
> TEXT varies per quest. The GraphicIndex tooling/RE below is correct — it just customizes the
> SEAL, not the card face.

# Quest-Card ART binding — how the engine picks a card's picture (the TYPE SEAL sprite)

*RE'd 2026-08-16 (`quest-card-re-prereqs` workflow, render-path + data-correlation angles,
reconciled). Verdict: **PARTIAL** — the binding mechanism is SOLVED end-to-end; what's
constrained is *per-card* art (needs shared-art acceptance or an engine patch).*

## TL;DR
Quest cards do **not** carry their own art. `OBJECT_QUEST_CARD` has **no** `Graphic`/`modelId`
field and `CQuestCardDef` has **no** texture field. Instead the Logbook picks one of exactly
**three shared orb sprites** by a hard 3-way branch on the card's `IsCoreQuest`/`IsVignette`
flags. So a brand-new card automatically inherits a class-appropriate sprite; giving it a
**unique** picture is not a data-only operation.

## The chain (proven end-to-end)
1. **Render-time selection** — `CTCInventoryQuests::ConstructQuestList @0x0061B610`
   (PC twin `ConstructQuestListPC @0x0061E6D0`) picks a sprite by a 3-way branch on
   `CTCQuestCard::IsCore()` + a vignette byte (`ghidra_out/questcard_render_decomp.c:2368-2481`):
   - `IsCore` → `UI_QUEST_SPRITE_CORE`
   - else `!vignette` → `UI_QUEST_SPRITE_OPTIONAL`
   - else → `UI_QUEST_SPRITE_VIGNETTE`
2. Each name → `NUISystem::CManager::CreateComponent(name)` → instantiates the **CUIDef**
   widget of that name (frontend.bin entries 8487 / 8488 / 8489).
3. The CUIDef supplies the texture via `States[UI_STATE_INACTIVE].GraphicIndex`
   (`ui.def:14551-14581`): CORE → `HUD_ORB_QUEST_CORE`; OPTIONAL/VIGNETTE specialise.
4. Those resolve to **textures.big / GBANK_MAIN_PC** ids **5892 / 5894 / 5896**
   (`big_entries.csv:14399-14404`, `RetailHeaders/pc/textures.h:3289-3294`).
   (`HUD_ORB_QUEST_FEAT` 5893 and the `*_SMALL` variants 5895/5897 also exist but the
   card-list path does not use them.)

> Correction folded in: the `HUD_ORB_QUEST_*` string seen near the render fn feeding
> `CQuestManager::OnQuestStartScreenDisplayed` is **not** the texture path — that call is a
> pure `StringMap_FindNode` bool ("start screen already shown?"), not rendering. The real
> texture bind is `CUIDef.GraphicIndex`.

## Can a new card get new art from scratch?
| Path | Result | How |
|------|--------|-----|
| **Unique per-card art** | ✗ via data | No per-card `GraphicIndex`, no def texture field, no `SetQuestCardGraphic` API. Flag-selected only. |
| **Shared re-skin (all cards of a class)** | ✓ via data | Replace texture id 5892/5894/5896 in textures.big (`tools/texture_build.py`, `docs/TEXTURE_WRITER.md`). Up to 3 distinct looks by repointing the 3 CUIDef `GraphicIndex` values (needs a frontend.bin CUIDef **writer** — verify `forge defs encode` supports the UI class). |
| **True per-card art** | ✓ via engine patch | Add a texture-id field to `CQuestCardDef` and detour `ConstructQuestList`/`…PC` to read it instead of the fixed constant. ForgeFSE hook on `0x0061B610`/`0x0061E6D0`. Reconstructable now that the branch is decompiled. |

## Implication for the content-first card vertical
The from-scratch card ships with the correct **class orb** (core/optional/vignette) chosen by
its `IsCoreQuest`/`IsVignette` flags — a real, non-placeholder look — without any art work.
Bespoke per-card art is a **separate ForgeFSE detour** feature (roadmap item), not a blocker
for the content-first card.

## Per-card art — implementation design (detour) — RE'd 2026-08-16

The exact selection branch (ghidra_out/questcard_render_decomp.c:2368-2481, fn
`CTCInventoryQuests::ConstructQuestList @0x0061B610`, PC twin `@0x0061E6D0`):

```c
ppVar51 = ppStack_428;                              // <- the CTCQuestCard* being rendered
if (CTCQuestCard::IsCore(ppStack_428)) {
    CreateComponent(mgr, "UI_QUEST_SPRITE_CORE");   // -> CUIDef 8487 -> tex 5892
} else {
    piVar27 = *(int**)(ppVar51 + 0xc);              // card inner object
    cVar44  = *(char*)((int)piVar27 + 0x59);        // vignette discriminator byte
    if (cVar44 == 0) CreateComponent(mgr, "UI_QUEST_SPRITE_OPTIONAL"); // 8488 -> 5894
    else             CreateComponent(mgr, "UI_QUEST_SPRITE_VIGNETTE"); // 8489 -> 5896
}
```

The sprite is 100% determined by the **CUIDef name string** passed to
`NUISystem::CManager::CreateComponent`. Per-card art = make that string card-specific.

**The card's identity at the branch:** the `CTCQuestCard*` (`ppStack_428`). Its owning quest
name is `ScriptQuestName` at card `+0x28` (see QUEST_CARD_EMPTY_FIX.md /
`GetActiveQuestCardFromScriptName`). That name is the join key to a ForgeFSE registry.

### The 3 required pieces (all native / in-game-iterative)
1. **Custom texture** into textures.big — READY today (`tools/texture_build.py`,
   docs/TEXTURE_WRITER.md; match the DXT format/dims of 5892 — TODO: confirm Info tail/mips/WxH).
2. **Custom CUIDef** in frontend.bin whose `States[UI_STATE_INACTIVE].GraphicIndex` = the new
   texture id (e.g. `UI_QUEST_SPRITE_CUSTOM_<X>`). GAP: needs a **frontend.bin CUIDef writer**
   (`forge defs encode` for the UI class — verify/implement; the decode side ships).
3. **ForgeFSE detour** at `0x0061B610` (+ `0x0061E6D0`): before the 3-way branch, read the
   card's `ScriptQuestName` (card+0x28), look it up in a ForgeFSE registry
   (`std::map<name, customCUIDefName>` populated from Lua, e.g.
   `Quest:SetQuestCardArt("<questName>", "UI_QUEST_SPRITE_CUSTOM_<X>")`); if found,
   `CreateComponent(mgr, customName)` and skip the stock branch; else fall through to retail.

### Recommended hook shape (safe, opt-in, matches ForgeFSE MapResourceAlias pattern)
Install a trampoline at the branch head but keep it **inert until a Lua opt-in** (like
`InstallMapResourceAliasHook` — installed always, disabled until `AliasMapResources`). The
detour trampoline: save regs → get card ptr (from the stack slot / EBX-relative it lives in at
that PC) → `name = *(CCharString*)(card+0x28)` → registry lookup → if hit, build the custom
component and jump past the stock selection; else execute the displaced instructions and
continue. Exact displaced-byte capture requires disassembling `0x0061B610` in the retail exe.

### Status
- Mechanism + selection branch + card identity: **CONFIRMED** (this doc).
- NOT YET IMPLEMENTED as a live patch: the trampoline is a **mid-function** hook whose only
  verification is in-game (crash-on-error), so it must be built against the disassembled
  displaced bytes and validated with the user's in-game loop — not shipped blind. Piece #2
  (frontend.bin CUIDef writer) is a prerequisite tooling task.

### Works TODAY without the detour (data-only)
- **Shared re-skin** of any card class: overwrite texture id 5892 (core) / 5894 (optional) /
  5896 (vignette) in `textures.big` GBANK_MAIN_PC via `tools/texture_build.py` (numeric-id
  target; match the existing DXT format/dims — TODO confirm). Every card of that class then
  shows the new art. This is the cleanest path because it swaps the texture the `GraphicIndex`
  already points at — it needs NO CUIDef edit, so it sidesteps the RE gap below.
- **3 distinct looks** by class: author cards with the desired `IsCoreQuest`/`IsVignette` flags
  (from-scratch CLI supports both) → each renders its class orb → re-skin the 3 orb textures
  independently → 3 authorable looks, zero native code.

### The CUIDef location — SOLVED 2026-08-16 (it's game.bin, not frontend.bin)
The quest-sprite CUIDefs are **normal top-level `game.bin` entries** (def class `UI` =
`CUIDef`), NOT in frontend.bin and NOT a nested UI-tree — the earlier "frontend.bin UI-tree"
worry was a wrong premise (retail `frontend.bin` holds only 811 dialog/table/generic-sprite
UI defs; the quest UI lives in game.bin):

| Entry | Name | indexInDefinition | size |
|------:|------|------:|-----:|
| 8487 | `UI_QUEST_SPRITE_CORE` | 2205 | 1355 B |
| 8488 | `UI_QUEST_SPRITE_OPTIONAL` | 2206 | 1355 B |
| 8489 | `UI_QUEST_SPRITE_VIGNETTE` | 2207 | 1355 B |

Each decodes **clean** as a `CUIDef` (109 fields, leftover 0). The three are **byte-identical
except one dword**: the texture id in `States[0].GraphicIndex`, which equals
**5892 / 5894 / 5896** (the textures.big GBANK_MAIN_PC orb ids) respectively — byte-verified.

**Wire location (tag-anchored, robust):** inside the `States` field value
(`crc0("States")=0x87ACD3D8`) → `[u32 count=5]` → each `CUIStateDef` element is itself
crc0-tag-anchored, so the first element is `crc0("GraphicIndex")=0x38E36902` then the
`u32` texture id. In these retail entries the value sits at payload offset **103**, but the
correct/robust way to find it is: locate the `States` tag, skip the count, then find
`crc0("GraphicIndex")` and overwrite the following `u32`. (`crc0("PositionX")=0x1EDB8A31`
follows, confirming the element layout.)

**CONSEQUENCE — both CUIDef art paths are now unblocked as plain game.bin def edits:**
- **3 distinct looks (data-only):** set each orb CUIDef's `States[0].GraphicIndex` to a custom
  texture id (inject the texture via `texture_build.py`), or just re-skin 5892/5894/5896.
  No frontend.bin work, no new RE. Needs a small `forge` States-aware GraphicIndex setter
  (`defs set-field` only reaches top-level tags; this is nested inside `States`).
- **True per-card art (detour):** the custom CUIDef the detour hands `CreateComponent` is just
  a **new `UI`/CUIDef entry appended to game.bin** (clone entry 8487, set GraphicIndex to the
  custom texture, name it `UI_QUEST_SPRITE_<X>`) via the DEF_LOAD_CONTRACT append (crc0 +
  index) forge already supports. The remaining native piece is the ForgeFSE trampoline
  (card+0x28 → registry → custom CUIDef name), still in-game-iterative.

## Verified facts
- Vignette discriminator = raw byte at `*(card+0xc)+0x59` (CONFIRMED from the decomp, not a
  named IsVignette() call) — the mapping is exactly `{IsCore, thatByte}`.
- (Still TODO) DXT format/dimensions of 5892/5894/5896 for byte-exact custom replacements.

## Evidence
- `ghidra_out/questcard_render_decomp.c:2368-2481`, `ghidra_out/quest_card_decomp.txt:8744-8754`.
- `RetailHeaders/pc/textures.h:3289-3294`, `big_entries.csv:14399-14404`,
  `compiled_game_entries.csv:10194-10196`, `ui.def:14551-14581`.
- `docs/QUEST_CARD_SYSTEM.md`, `docs/TEXTURE_WRITER.md`, `docs/FRONTEND_FORMAT.md`.
