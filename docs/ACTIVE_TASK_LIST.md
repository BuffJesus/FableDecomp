# Active task list — decomp, frontend parity, and downstream tools

*Refreshed 2026-07-29 from the canonical rebuild and visual parity gates.*

## Current verified state

- Curated reconstruction: **5,355 / 5,355** VC7.1 compile + behavior PASS.
- Whole-project verified functional or matching C++: **5,205 / 49,568**
  (**10.50%**), including **2,900 byte-identical** functions (**5.85%**).
- Candidate retail parity: **5,187**
  (`2,884 EXACT + 2,303 RELOCATION_MATCH`), with `40 DIFFER` and
  `128 ORACLE_MISSING`; the whole-project totals additionally include 18
  independently lifted matching/functional functions.
- Auto-RE intake: **840 generated / 825 structural PASS**.
- GFMain evidence/control-flow closure: **257 / 257 direct calls** and
  **10 / 10 authored phases**; the separate modern C++23 startup pass remains
  **0 / 10 phases**.
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
13. `CTable::ConstructSpritesToDraw` now crosses from pure plans into owned
    generated components. Definition-bearing sprite prototypes are cloned in
    the recovered corner/horizontal/vertical order, initialised, and receive
    state-0 position/zoom writes. Explicit `CCountedPointer` controls prove the
    retail local-create/vector-copy/local-release sequence, final vector
    ownership, complete destruction, and capacity-failure rollback. The
    20-component fixture and full VC7.1 Release bootstrap pass.
14. The first owned component family now reaches the live Render2D queue.
    Options and all four detail headers construct three definition-122
    `UI_TEXTBOX_MIDDLE` children, map them to the pristine 8x64 retail sprite,
    and emit clipped/scaled quads plus ordinary texture/state records through
    the existing batch and draw-list adapters. Binding/capacity failures are
    transactional. The retained baked sheet and the no-rule component sheet
    recompose pixel-identically across all eight frames; full Release,
    subscreen interaction, and maximized aspect-fit smokes pass.
15. The Options selected-row `UI_BUTTON` table is live. Definitions
    129/130/131 bind to separate pristine `TS_BUTTON_L/R/M` textures, the
    middle child carries the recovered 19-tile zoom, and the owned three-child
    table follows y=`143 + 30*selection` behind the still-baked row text.
    Multi-definition record emission is focused-test covered. Removing both
    this ornament and the title rule from the component sheet still recomposes
    all eight oracle frames pixel-identically. The live D3D9 adapter now
    recognizes the title segment plus all three `TS_BUTTON_L/M/R` texture
    handles; previously those valid Render2D records attached null and made
    the runtime highlight disappear. Retail, BuffJesus, and full subscreen
    smokes now require a visible off-texture highlight-pixel delta.
16. Main-menu selection tables are live in both public variants. Rows 1-6
    reuse the owned 280-pixel vector; Continue Game uses a second owned
    `UI_BUTTON_BIG` vector with the recovered 400-pixel span and 34-tile
    middle zoom. Retail and BuffJesus component sheets omit the ornament and
    recompose all seven baked frames pixel-identically. Both main-menu smokes,
    the complete subscreen smoke, maximized scaling, and the Release bootstrap
    pass.
17. Main-menu labels are seven independent Render2D rows rather than text
    flattened into every selection frame. Retail and BuffJesus component
    atlases retain title-only left frames and one transparent row canvas per
    list child; recomposition is pixel-identical across all 14 frames. The
    retail 0x24-byte `CUIState` layout is compile-locked, and a focused
    `DoRecomputeOffsets` adapter writes the proven position/alpha state masks,
    live state-1 values, and `ScrollingChildrenStates`-style snapshots with
    transactional capacity rejection. The adapter is behavior-gated
    integration code, not whole-function byte parity.
18. The seven live row quads now consume their exact five-state `CUIState`
    maps for position and colour. Main-menu and Options Up/Down input uses the
    recovered frontend-list scroll decision path, including wrap, old/new
    states 4/3, and unchanged-child rejection at a blocked boundary. Retail
    definitions lock both lists to `Wrapping=true`, `Scrolling=false`,
    `AlphaOffset=0`, and `PositionOffsetY=30`; text states 3/4 are immediately
    visible at x=120 with `UpdateTime=-1`. Therefore navigation correctly moves
    only the selected ornament rather than inventing a row translation/fade.
    Retail, BuffJesus, complete subscreen (including Options Down/Up), and
    maximized 2560x1369 smokes pass.
19. Options labels are four independent Render2D row canvases rather than text
    repeated in the four selection frames. The component sheet expands from
    1024x3840 to 1664x3840: its original frame/control/key atlas coordinates
    remain unchanged and x=1024..1664 holds the four row canvases. All four
    quads consume their exact decoded five-state position/colour maps.
    Recomposition against the retained 1024x3840 baked oracle is
    pixel-identical for every Options and detail frame. The full VC7.1 build,
    15 focused subscreen tests, retail interaction, BuffJesus main-menu, and
    maximized smokes pass.
20. Keyboard Enter now dispatches the selected recovered routes instead of
    requiring a synthetic mouse release: press-start action 229, main-menu
    Options/Quit actions 297/314, and Options actions 9/13/12/283. Mouse and
    keyboard share the same transition helpers, keeping detail initialization,
    saved-value snapshots, and D3D state changes aligned. The complete
    subscreen smoke uses Enter for press-start, Options entry, and every detail
    destination; retail and BuffJesus report `keys=enter-up-wrap-down`.
21. Frontend navigation now uses the untouched retail `Frontend.lug` audio
    rather than a synthetic placeholder. `CFrontEndList::ScrollUp/ScrollDown`
    selects `UI_MISC_THINGS_DEF.SoundUpDown` (`CS_GUI_2`, sample 3) after a
    move and `SoundError` (`CS_GUI_5`, sample 7) at a blocked boundary.
    Recovered `CFrontEndManager::Update` transition fields select
    `SoundBack` (`CS_GUI_6`, sample 4) and `SoundForward` (`CS_GUI_7`,
    sample 5). The build extracts and embeds all four RIFFs byte-for-byte,
    the executable resource audit matches their SHA-256 values, and the full
    VC7.1 build plus interaction smokes pass.
22. The first frontend-list bodies now have real object-code proof rather than
    adapter-only evidence. `CFrontEndList::InitialiseOffsets @ 0x0054C480` is
    exact 57/57 bytes with no relocations, and `ScrollUp @ 0x0054C4C0` is
    relocation-normalized 834/834 bytes with zero non-relocation differences.
    Its focused fixture covers reject/wrap, sound selection, states, colours,
    positions, alpha math, and intrusive-reference release. `ScrollDown
    @ 0x0054C810` now has equivalent behavior coverage and a complete
    relocation-normalized 977/977-byte match. Its ABI-explicit recovered body
    also locks the retail x87-truncated alpha ramp (`190,255,190,126,62`).
23. The initial frontend-manager transition/profile seam now has canonical
    object-code proof. `LaunchVirtualKeyboard @ 0x00596917` is a
    relocation-normalized 99/99-byte match, including lookup/routing,
    allocation/construction/storage order, and retail's null-allocation
    behavior. `CreateNewProfile @ 0x0059697A` is a 238/238-byte match covering
    no-space rejection, name-result semantics, defaults, both inversion
    branches, save, menu-definition update, and profile-name propagation.
    Neither body invents direct sound/refcount work; keyboard routing delegates
    that boundary to the already matched `GotoNextScreen`.
    `CreateFrontEndButtonComponent @ 0x00596A68` adds a 614/614-byte match for
    default/custom definition routing, component composition, action setup,
    state widths, and counted-pointer release order. `RefreshAvailableProfiles
    @ 0x00596CCE` has a behavior- and ownership-proven fixture, but remains an
    honest 329/824-byte `DIFFER` until its inlined string/action/allocator
    temporary-lifetime graph is recovered.
    `RefreshAvailableProfilesForDelete` 546/546, `LoadProfile` 130/130,
    `DoPressStart` 227/227, `AddPrefixToTitleBar` 75/75, and
    `CanAcceptOptionChanges` 56/56 further cover delete-list ownership,
    press-start routing, profile publication, title propagation, and the
    option-acceptance gate.
24. Redefine Keys now uses the two different retail row primitives. The
    280-pixel action-name side remains the rounded `FE_SLOT_TEST_L/M/R` table;
    the 220-pixel key-value side is the repeated
    `FE_OPTIONS_HORIZONTAL_BAR_SPRITE` selected by
    `UI_OPTIONS_HORIZONTAL_BAR`. `CText::Draw @ 0x0054EF00` proves both
    `ENG_ARIAL_12` children use their serialized `(0,3)`/`(380,3)` top
    origins: alignment changes x only, y is rounded and forwarded unchanged,
    and the downstream font call receives flags zero. The renderer and focused
    tests now lock that code-derived geometry instead of aesthetically
    centering the text.
25. Continue Game's next screen and dynamic rows are recovered and live in the
    checkpoint. Action 66
    refreshes saves and enters used key `0x08`,
    `UI_FRONTEND_PROFILE_SAVED_GAMES_MENU`. The 1,335-byte refresh body
    replaces `UI_FRONTEND_LIST_FOR_SAVES`, attaches the scrolling text and
    screenshot viewports, then builds autosave-first/ascending-manual rows at
    30-pixel intervals. Each row carries the original filename and chooses
    action `0x11` or invalid-save action `0xDC` from the primary/companion
    validity checks. The runnable now enters the saved-games screen, renders
    the decoded title/list/file-information/Back structure, moves the
    ornamental highlight through the four sample rows with keyboard or mouse,
    and returns through action 86. The 12 logical frames are packed into a
    1664x3840 D3D9-safe atlas; save frames occupy x=1024..1664 at y=1920..3840.
    The full Release build and pixel-level subscreen smoke prove activation,
    highlight movement, and Back. Enter on a row deliberately does not cross
    the still-unrecovered main-game/world-load boundary.

Visual acceptance note: the saved-games and keybind screens are functional
checkpoints, not presentation-parity sign-off. User review still finds the
current screenshot visibly incorrect. Tomorrow's first UI task is a
state-matched retail/reconstruction capture and alpha-aware diff, followed by
evidence-based corrections to text scale/baselines, highlight span/placement,
and any metadata/background differences the diff identifies. Do not describe
the initial frontend as fully matched until that comparison passes.

Next work, in order:

1. Extend the live WinMM controller bridge beyond initial navigation. POV and
   analogue Up/Down plus button-1/Start accept and button-2/Back now enter the
   same recovered list-scroll/action routes as keyboard input. Detail screens
   now wrap row focus and apply Left/Right through the same live profile-value
   path as mouse controls. Held direction input now mirrors the recovered
   `CFrontendGameComponent::ChangeSelection @ 0x00494380` state machine:
   immediate direction changes, a 500 ms initial delay, then 100 ms repeats.
   Implement the remaining main-menu actions; connect validated save-row action
   `0x11` to the recovered main-game load chain only after that ownership
   boundary is ready. Recover mouse-hover and remaining
   action sound dispatch separately before enabling it.
2. Capture identical retail and reconstructed Saved Games and Redefine Keys
   states. Produce alpha-aware image diffs and correct measured text,
   highlight, metadata, and background discrepancies.
3. Extend `CKeyRedefiner` beyond the first visible page: decode the full action
   list, retail `IsRedefinableKey` filtering, action-pair coexistence table, and
   profile persistence rather than keeping the compact key-name atlas as the
   final renderer.
4. Replace remaining detail-screen/helper constants with decoded definition
   records, then gate every runtime hitbox against those same records.
5. Move from precomposed 640×480 sheets toward live retail component rendering:
   sprite tables, text components, state transitions, alpha/color inheritance,
   and UI scaling should be emitted through the recovered Render2D path.
6. Replace the remaining dependency boundaries inside the now-callable full
   GFMain coordinator. Phase 10's `CWideString::operator!=` and complete
   registry constructor/write/destructor closure are now exact, independently
   behavior-gated leaves. The former render-manager startup-line call is now
   correctly identified and exact as `CUserProfileManager::SetProfileName`,
   its counted singleton getter is exact as well, and the former destructor
   collision is now exact `GFUninitialise`. `CGame::Play` and its ownership
   graph are exact as well, alongside the full `GFHandleSystemInitError`
   dispatcher. All ten phases execute in Stage 3; Phase 10 is now 21/21
   direct calls proven. Phase 7's final ownership-heavy `GetWindowTitle` leaf
   is exact too, raising that phase to 15/15. Exact CIME initialization raises
   Phase 9's exact CIME and LUG-to-MET coordinators raise it to 10/11 and
   GFMain to 218/257 overall after exact Phase 8 configuration cleanup.
   `CSystemManager::Initialise` is Phase 9's final edge.
   The frontend checkpoint remains a fast visual gate.

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

## P1 — pilot a Unicorn retail behavior oracle

Adopt Unicorn only as an optional, local function-level x86 oracle. It must
remain outside the canonical build and cannot change parity totals by itself.
The full design and upstream evidence are in
`docs/UNICORN_EMULATION_PLAN.md`.

Current opportunity:

- 2,390 `MATCH` functions are 2–64 bytes with zero recorded relocations,
  providing a broad known-good harness-validation pool.
- 174 `DIFFER` functions are at most 128 retail bytes, providing a bounded
  second-stage trace-diagnosis pool.
- CPU-side frontend geometry, billboard generation, particle batching, shader
  constant selection, and D3D state arguments can be traced behind registered
  stubs; actual D3D/shader output cannot.

Pilot order:

1. Create a separate Python 3.13 environment and pin `unicorn==2.1.4`; do not
   vendor or link it into reconstructed binaries.
2. Implement `tools/unicorn_oracle.py` around authoritative oracle rows,
   original retail virtual addresses, explicit stack/heap/object fixtures,
   mapped call stubs, a return sentinel, and hard instruction/time bounds.
3. Prove deterministic register, stack-delta, block, memory-write, call-trace,
   and fault JSON on five known exact leaves.
4. Diagnose ten same-length, sub-128-byte `DIFFER` rows. Preserve
   `UNSUPPORTED` separately from `MISMATCH`; feed only proven semantic/ABI
   corrections into the normal author/refine loop.
5. Trace one frontend geometry helper and one particle/RSA/render-state helper
   behind engine/D3D stubs.

Acceptance: repeated outputs are deterministic; existing behavior fixtures
agree; deliberate bad fixtures fail; at least one trace replaces a guessed
contract or repairs a real residue; retail bytes/snapshots remain local; and
every promoted function still passes VC7.1 compile, behavior, and retail-byte
comparison.

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
