# Active task list — decomp, frontend parity, and downstream tools

*Refreshed 2026-07-27 from the canonical rebuild and visual parity gates.*

## Current verified state

- Curated reconstruction: **5,051 / 5,051** VC7.1 compile + behavior PASS.
- Whole-project verified functional or matching C++: **4,736 / 49,568**
  (**9.55%**), including **2,705 byte-identical** functions (**5.46%**).
- Candidate retail parity: **4,718**
  (`2,689 EXACT + 2,029 RELOCATION_MATCH`), with `205 DIFFER` and
  `128 ORACLE_MISSING`; the whole-project totals additionally include 18
  independently lifted matching/functional functions.
- Auto-RE intake: **803 generated / 787 structural PASS**.
- Durable source/tests are address-sharded and indexed by `rebuild/ARTIFACT_INDEX.tsv`.
- Custom quest assets are staged offline: nine card defs, 36 custom English strings,
  and one childhood mysterious-NPC TNG addition. Nothing has been deployed to the live game.
- The Release visual checkpoint, retail `frontend.bin` layout oracle, and complete
  Options/Redefine/Quit interaction smoke pass. The scheduled auto-RE queue is
  exhausted apart from five cooldown-deferred hard targets and will retry them
  automatically.

Generated queues:

- `rebuild/backlog/promotion_queue.tsv` — ranked uncompiled candidates with semantic hazards.
- `rebuild/backlog/forgefse-binding-queue.tsv` — current binding reconstruction lane.
- `rebuild/backlog/functions.tsv` — canonical reconstruction gaps.

## P0 — frontend runtime and pixel parity

Completed in the current checkpoint:

1. Retail `frontend.bin` now gates the compiled list origins, row steps, child
   order, actions, table offsets, title rules, fonts, and Redefine mouse areas
   used by the Python sheet builders.
2. `CTable::Draw @ 0x00550DC0` ordinary/true transform propagation and generated
   child positioning are promoted into VC7.1 runtime code and behavior tests.
   Main-menu, Options, and Redefine hit regions use that transform chain.
3. Screen headers resolve to the full 640-pixel title-rule center. Their table
   now uses the exact `UI_TABLE_TITLE_WHOLE` mapping: all three horizontal
   roles reference `UI_TEXTBOX_MIDDLE` (#122), producing one continuous retail
   gold-edged/blue-filled rule instead of substituted selected-button pieces.
   The build still prefers the pristine installed `frontend.big`.
4. Plain `--buff-jesus` includes the three retail boot movies; the explicit
   `--skip-boot-videos` developer override and retail/BuffJesus smokes pass.
5. The horizontal/vertical `CTable` line geometry is promoted from retail and
   the PDB-backed FableWin donor: primary placement, axis-specific zoom/repeat
   count, resource-key filtering, cursor advancement, and truncated-output
   accounting have VC7.1 behavior coverage. The installed-retail font test also
   proves the Redefine key-label alpha centre equals its right-slot alpha centre.
6. `CKeyRedefiner` is no longer hover-only in the live checkpoint. PDB-backed
   vtable/body parity identifies `OnLeftClicked @ 0x00557850`,
   `OnLeftUnclicked @ 0x00557AF0`, `ChangeState @ 0x00557C10`, and corrects
   `OnUnhovered @ 0x00557880`. Clicking a row enters the yellow retail
   `PRESS CONTROL` state; keyboard/three-button mouse capture, Escape
   cancellation, duplicate clearing, and Apply/Cancel snapshots are exercised
   by both retail-text and BuffJesus live smokes. Movement action 60 now
   follows `CRedefinerList::RefreshScriptThings @ 0x00556A40`: it expands into
   four W/S/A/D Forward/Back/Left/Right children before the remaining actions,
   rather than occupying one composite display row. Reset actions 284/311 now
   switch those children between arrows and WASD, and both reset controls use
   their complete centered retail ON tables on hover.
7. The exact high-level `ConstructSpritesToDraw` event order is now promoted:
   corners 0/1/2/3, horizontal top/interior/bottom using 4+9, 4+12, 5+8,
   then vertical left/interior/right using 6+10, 6+12, 7+11. Missing optional
   tee/cross components remain null, and missing primary edge components
   suppress their entire separator family. A VC7.1 fixture covers the full
   route, sparse maps, offsets, repeat counts, and truncated output.
8. The exact `ConstructSpritesToDraw` coordinate slice is promoted from retail
   x87 instructions and cross-checked against Ego R: tiled width/height are
   inner extents, corner sizes move the four edge cursors, normalized separator
   tile indices reconstruct interior positions, and repeat/resource offsets
   truncate exactly like retail. Bounded VC7.1 fixtures cover corner anchors,
   line starts, interior rows/columns, and logical generated-child counts.
   The live title rule now uses that inner-span-plus-corners composition before
   the 640-pixel retail viewport clips it.
9. `CList::SetSelectedChild @ 0x005360B1` is promoted from retail and both PDB
   donors. It stores every requested index, leaves visible/invalid selections
   stationary, finds the first contiguous visible window, and emits the exact
   signed 25-pixel-per-row correction for a valid selection outside that
   window. VC7.1 coverage includes above/below, visible, and invalid requests.
10. Frontend lists are proven not to add a hidden centering offset:
    `CFrontEndList::InitialiseOffsets @ 0x0054C480` ignores child count, writes
    interpolation scalar `1.0`, and applies `(0,0)`. Retail, Ego R, and
    FableWin bodies agree, and the VC7.1 fixture locks the override.
11. Frontend list navigation is promoted from retail
    `ScrollUp @ 0x0054C4C0` / `ScrollDown @ 0x0054C810` and cross-checked
    against both PDB donors. Zero/singleton lists and stop-at-end boundaries
    request the invalid action; successful moves wrap when allowed, transition
    the old/new children through states 4/3, rotate row positions by exactly
    `+30`/`-30` pixels, and recompute the optional distance alpha falloff with
    the retail x87 truncation. VC7.1 coverage includes rejection, wrap, state,
    motion, and bounded alpha output.
12. Generic `CList::InitialiseOffsets @ 0x00536B4F` and
    `DoRecomputeOffsets @ 0x0053C332` are promoted and cross-checked against
    Ego R/FableWin. Odd/even centre selection, initial x/y displacement,
    optional zero-origin behavior, states 0/1/4/5/6 position writes, states
    1/4/5 alpha writes, centre reflection, and bounded child output now have
    VC7.1 coverage. The fixture deliberately locks the stored-float alpha
    residue (`126,191,255,191,127`) rather than a symmetric rounded ideal.

Next work, in order:

1. Finish `CTable::ConstructSpritesToDraw @ 0x00551EA0`: replace the promoted
   pure planners with real component cloning, `CCountedPointer` increment/
   release behavior, state-map writes, and the final generated-child vector.
   Corner placement, edge cursors, and logical child counts are now covered.
   The exact line-builder addresses
   are now correctly named `ConstructHorizontalLine @ 0x005518E0` and
   `ConstructVerticalLine @ 0x00551BC0` in Ghidra.
2. Continue `CList` keyboard/controller navigation by replacing the pure
   planners with real `CUIState` map writes and the scrolling-state vector.
   `SetSelectedChild`, both initial-offset variants, `DoRecomputeOffsets`, and
   frontend `ScrollUp/ScrollDown` decisions are covered. Next promote
   disabled-child handling, the per-child state rotation, and timed selection
   animation into the live renderer.
3. Extend `CKeyRedefiner` beyond the first visible page: decode the full action
   list, retail `IsRedefinableKey` filtering, action-pair coexistence table, and
   profile persistence rather than keeping the compact key-name atlas as the
   final renderer.
4. Replace remaining detail-screen/helper constants with decoded definition
   records, then gate every runtime hitbox against those same records.
5. Move from precomposed 640×480 sheets toward live retail component rendering:
   sprite tables, text components, state transitions, alpha/color inheritance,
   and UI scaling should be emitted through the recovered Render2D path.
6. Capture retail and reconstructed frames at identical states and add
   alpha-aware image diffs for text baselines, highlight centers, title rules,
   helper buttons, and 4:3/aspect-fit scaling.
7. Continue the runnable boundary through the remaining GFMain Phase 3 calls
   while keeping the frontend checkpoint available as a fast visual gate.

Acceptance: decoded layout values and runtime hit regions share one oracle;
headers and row text are centered by recovered transforms/metrics; every visible
state has a deterministic screenshot gate; and the authored checkpoint clearly
distinguishes exact retail data from still-approximate runtime behavior.

## P0 — particles, RSA lighting, sky, and shadows

The viewer-parity lane is now grounded in retail asset and runtime evidence:

1. `tools/report_particle_rendering.py` joins selected `effects.big` emitters
   to `textures.big` descriptors, preserving exact component path, real versus
   allocated dimensions, UV crop, aspect, crossed-sprite count/angles,
   orientation inputs, colours, decoded PDB sprite flags, and exact normal-pass
   D3D9 blend state.
2. The candle-flame defect has a concrete contract: texture 4422 is 32x64,
   the quad aspect is 2.0, and two crossed planes are emitted at base-angle
   offsets 0.125/0.375 turns. The planes are not animation frames.
3. `tools/report_environment_lookup.py` samples the installed 190x21
   `lighting_colours.tga` with the retail `(byte + 0.5) / 255` conversion.
   All rows, including diffuse/ambient/backlight and sky/cloud gradients, are
   decoded from the live `ENVIRONMENT` definition.
4. `docs/PARTICLE_LIGHTING_VIEWER_HANDOFF.md` records the two sprite paths,
   ten-quad batching, padded-texture UV behavior, environment columns,
   lighting SIMD layout, layered sky inputs, and shadow-fade triple.
5. `EEngineSpriteFlag` is recovered through the Ego R PDB. Zero is centered
   `2D_FACE_ME`; the `3D_FACE_ME`, alignment, rotation, lighting, modulation,
   and Z-buffer bits are now decoded in every generated effect report.
6. The batched sprite normal-pass blend tuples are recovered: additive is
   `ONE/ONE`, add-smooth is `ONE/INVSRCCOLOR`, and ordinary alpha is
   `SRCALPHA/INVSRCALPHA`; ADD/SUB/REVSUB blend-op state is also exact.
7. `tools/dump_shader_asm.py` extracts the original shader-model 1.1 token
   streams from `shaders.big` and disassembles them with payload hashes.
   Billboard, crossed, 3D-face-me, stipple, displacement, and ordinary sprite
   shaders are dumped in `work/particle_shaders.asm`.
8. The outer- and inner-sky shader programs are dumped in
   `work/sky_shaders.asm`. The exact two-texture/gradient-alpha outer blend and
   four-texture, two-layer inner/cloud composition are now documented.

Next work, in order:

1. Map names onto every now-disassembled particle shader constant register and
   recover the separate RSA mesh-normal formula.
2. Recover the signed blend-op shader behavior and the special depth/pass
   overrides around the now-exact normal colour pass.
3. Attach semantic names to the inner-sky/cloud constant registers and trace
   each texture stage back to its upper/lower theme field; the GPU composition
   itself is now exact.
4. Promote shadow buffer allocation, depth packing, transforms, and scene
   eligibility into a small documented renderer contract/test fixture.
5. Add effect-specific golden reports for flame, statue waterfall, smoke, and
   self-illuminated particles, then compare viewer captures by camera angle.

## P0 — review the current Quest-wrapper tail

The organized-tree full build and parity refresh are complete. Review these generated wrappers
before any promotion:

1. Reconstruct the failed lookup paths in `DisplayTutorial @ 0x0089E710`,
   `EnableGuards @ 0x00896270`, `EnableVillagerDefTypes @ 0x008962D0`, and
   `GetNumberOfItemsOfTypeInInventory @ 0x00897190`; each currently dereferences an end sentinel.
2. Correct the unrelated-object fallback in the inventory wrapper and the implausible lip-sync
   container identity in `ClearGossip @ 0x008AA010`.
3. Verify the raw slot and null behavior in `GetWaterHeightAtPosition @ 0x0088DF60`.
4. Promote only through VC7.1 compile, focused behavior, signature audit, and retail comparison.

## P0 — make the secret hunt playable

### Registration

1. Register the persistent parent quest `SecretHunt`.
2. Register `MysteryHunt1` through `MysteryHunt9` in ForgeFSE and
   `data/Levels/FinalAlbion.qst`; the companion script currently activates these names.
3. Bind the childhood entity script to `MysteriousChildhoodNPC`.
4. Keep the card, text, TNG, and Lua files in one deployable/rollback unit:
   - `data/CompiledDefs/game.bin`
   - `data/lang/English/text.big`
   - `data/Levels/FinalAlbion/StartOakValeEast.tng`
   - `FSE/SecretHunt/...`

### Region probe

Log `Quest:GetRegionName()` in the intended maps before finalizing the hunt table.
`Greatwood`, `Witchwood`, `Darkwood`, `HookCoast`, and `Oakvale` are plausible;
`Graveyard`, `HobbeCave`, and `Snowspire` are explicitly unconfirmed.

### Runtime smoke order

1. Start a new childhood game and find the NPC at `M_BarrelManHiddenPos`.
2. Confirm talk UI renders the new `TEXT_SECRET_HUNT_NPC_*` strings.
3. Accept the secret; confirm no quest card is created during childhood.
4. Save/reload as a child and confirm the NPC uses the reminder line. This gates
   `OnPersist`/`PersistTransferBool`.
5. Cross the time-skip and confirm `CanOfferAdultSecretHunt()` is true.
6. Activate `MysteryHunt1`; verify F9 shows:
   - title `The Footprint That Vanished`;
   - its custom summary and objective;
   - 200 gold / 50 renown.
7. Complete/deactivate Hunt 1 and activate Hunt 2 to prove distinct cards cycle.
8. Restore all backups after the smoke.

Acceptance: childhood discovery survives save and time-skip, the adult card appears in
F9 with custom text/rewards, and retail restoration hashes match.

## P0 — high-value fse2 decomp candidates

These directly support quest/NPC/runtime features. Run them before general length order.

| Address | Bytes | Function | Why next |
|---|---:|---|---|
| `00896F60` | 97 | `SetDeathRecoveryMarkerName` | save/death state wrapper |
| `00896DF0` | 104 | `KickOffCreditsScreen` | screen-flow wrapper |
| `00894300` | 111 | `MsgIsQuestionAnsweredYesOrNo` | mysterious-NPC interaction |
| `0089E4C0` | 120 | `AddScreenTitleMessage` | custom quest messaging |
| `00891220` | 125 | `OverrideMusic` | pairs with verified `StopOverrideMusic` |
| `00896EE0` | 125 | `GetDeathRecoveryMarkerName` | save/death state wrapper |
| `008913F0` | 148 | `AddQuestCard` | central custom-card ABI |
| `0089AD90` | 155 | `IsQuestStartScreenActive` | quest UI state |

Gate for every address:

1. validate the oracle boundary;
2. author C++03-compatible source and a concrete behavior test;
3. require `check_one.py` `MATCH/RELOCATION_MATCH + PASS`;
4. require independent `verify_and_land.py` agreement;
5. land only after both gates.

## P1 — short fse2 candidates

After the P0 product targets, work the short snapshot-backed candidates:

| Address | Bytes | Function |
|---|---:|---|
| `00891610` | 97 | `GetMostRecentValidUsedTarget` |
| `008A8E40` | 98 | `EntityDecapitate` |
| `0089D7C0` | 99 | `SetAbilityAvailability` |
| `00893F80` | 100 | `MsgOnHeroCastSpell` |
| `008979C0` | 101 | `IsEntityMarriedToHero` |
| `00897A30` | 101 | `IsEntityMarriable` |
| `00899770` | 101 | `GetHeroRoughExperienceLevel` |
| `008A8EB0` | 102 | `EntitySetAttackThingImmediately` |
| `008A1C50` | 104 | `RepopulateVillage` |
| `0089C8A0` | 104 | `IsDeedWitnessed` |
| `008974F0` | 105 | `IsEntityWieldingWeapon` |
| `00898A50` | 105 | `GiveHeroTutorial` |
| `0089A000` | 105 | `GetHeroMoralityCategory` |
| `00894490` | 106 | `RemoveDeadCreature` |
| `008A0FD0` | 107 | `SetReadableObjectTextTag` |
| `00899390` | 109 | `GetHeroStrengthLevel` |
| `00899400` | 109 | `GetHeroSkillLevel` |
| `00899470` | 109 | `GetHeroWillLevel` |
| `00893EC0` | 110 | `MsgOnHeroSlept` |

Full fse2 ordering, including the 46 larger functions, is in
`rebuild/backlog/fse2_remaining_ranked.tsv`.

### Explicit hold

`008997E0 GetHeroExperienceAvailableToSpend` is behavior `PASS` but best parity is
`DIFFER(101v97)`. It is a documented register-allocation/spill residue. Do not blind
regrind it; retry only with a new liveness/register hypothesis or a different compiler build.

## P1 — clear small stalled batches

These are compact cleanup pools after fse2:

1. batch13: **6 remaining**.
   - `00456389 OnReadFinished` (36 B)
   - `004549CE vector_deleting_destructor` (50 B)
   - `00456010 ~CPatchTesselationEdgeStrip` (61 B)
   - `00455008 UpdateShadowScene` (65 B)
   - `0045641A OnDie` (67 B)
   - `00454AEF CopyBackBufferToTexture` (93 B)
2. batch12: **16 remaining**, including eight 30-byte
   `CopyBackBufferToTexture` variants.
3. batch11: **26 remaining**, starting with
   `0044C1CD CTCNoiseDef::GetSizeofClass` (15 B) and
   `0044C12F CInventoryItemDef::GetSizeofClass` (23 B).

These are previous non-wins. Validate boundaries before authoring; many tiny failures are
merged-oracle or signature problems rather than difficult source.

## P2 — ForgeFSE binding retries

fse1 has **69** unlanded binding targets. Start with the smallest:

- `0089CD50 CanThingBe_Smelled_ByOtherThing` (20 B)
- `0088E130 GiveHeroWeapon` (23 B)
- `008CFE30 CGameScriptThing::GetHomePos` (24 B)
- `0088F480 ApplyScriptBrush` (25 B)
- `0088E3D0 MiniMapAllowRouteBetweenRegions` (33 B)
- `008A9E30 AddGossipVillage` (34 B)
- `008A9DD0 AddNewRumourToCategory` (34 B)
- `0088F9B0 GetGuildSealRecallPos` (36 B)
- `00891550/80/B0/E0` boast wrappers (36 B)

Because this is a retry lane, record the prior mismatch class before spending another
authoring pass. Prefer oracle repair, ABI correction, or annotated-diff-driven changes.

## P2 — harden FableForge custom text

1. Review and isolate the text-port changes from the already-dirty FableForge worktree.
2. Commit the port as a coherent unit:
   - BIG magic preservation and type histogram;
   - text encode/upsert/import;
   - quest-card custom-text flags;
   - tests and `docs/CUSTOM_QUEST_CARD_TEXT.md`.
3. Add a mod-package/deploy command that treats `game.bin + text.big + TNG + Lua` atomically.
4. Add a rollback manifest containing original and staged hashes.
5. Add additional language manifests only after English runtime validation.

Acceptance: FableForge remains 7/7 tests passing, an imported archive reopens, all untouched
retail records remain byte-identical, and no command edits the install without `--in-place`.

## P3 — broad backlog

Current remaining staged supply:

- batch14: 292
- batch16: 205
- batch17: 125
- batch15: 108
- batch3: 58
- batch4: 46
- batch5: 59
- retry1: 48

Do not launch these ahead of the product-facing fse queues and small batch11–13 cleanup.
For `retry1` and the old batch3–5 residue, repair merged/over-length oracle rows before
another agent wave.

## Resume rules

- One authoring lane at a time unless rate limits are demonstrably stable.
- Never count self-reported wins; independent VC7.1 verification is mandatory.
- Keep behavior-only `DIFFER` functions out of byte-match totals.
- Preserve automation-owned dirty files and commit scoped changes only.
- Update the three generated TSVs after every landing wave so completed addresses disappear.
