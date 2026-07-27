# Co-op revival — enable gate, replication protocol, CheckSync rebuild (2026-07-24)

RE'd via the Claude Workflow loop (`coop-revival-re`: 8 decode agents → synthesis → adversarial
verify). Verdict **PLAUSIBLE**: the three headline conclusions are byte-solid and survived refutation;
the synthesis also overstated some evidence, and those overclaims are **corrected inline below** — trust
this file, not the raw synthesis. Bytes from retail `Fable.exe`; cluster identity is a BSim port from
`ego_r.exe` + `FINDINGS.md` (same lineage — the CLAUDE.md ≥2-source bar is only weakly met for *names*,
but the *opcode facts* are directly from retail bytes).

---

## Historical Context (Public Sources)

Lionhead publicly described a local co-op system as early as 2003–2004. Key contemporary statements:

- **Peter Molyneux** (multiple interviews archived by The Fable Historian and analysed in Avarice’s 2021 cut-content video):
  - Up to **four players** on one machine.
  - Drop-in design: a friend plugs in their save/memory card; their Hero is imported into the host world with **no restart or configuration**.
  - Imported character becomes an **“Alter Ego”**.
  - Players can freely swap who is the main Hero and who is the Alter Ego (White button on Xbox).
  - Alter Ego can fight, gain experience, heal the main character, and trade items.
  - Scoring system: helpful actions increase score; higher score makes it easier to take over as the main character.
  - Described as “very cooperative but also a little bit competitive.”
  - Explicitly stated the feature had already been tested internally.

- **Dene Carter** also referenced multiplayer intentions in the same period.

These statements align extremely well with the surviving code and assets.

### Leftover Assets (Avarice cut-content research, 2021 + community RE)

| Asset / System                     | Notes                                                                 | Relation to runtime code                  |
|------------------------------------|-----------------------------------------------------------------------|-------------------------------------------|
| `CCoopSpiritDef`                   | Multiple definitions (5+ active + null placeholder)                  | Matches `CTCCoopSpirit`                   |
| Multiplayer orb / spirit effects   | Coloured orbs, particle emitters, lightning beam                     | Visuals for Alter Ego / spirit form       |
| Scoreboard system                  | Full text strings + UI remnants + `Fable.exe` references             | Matches Molyneux’s scoring mechanic       |
| Arena & Hero Centre leaderboards   | Unused score/leaderboard UI                                          | Competitive layer                         |
| Trading stubs                      | References to inter-player trading                                   | Explicitly mentioned by Molyneux          |
| Co-Op Spirit Player X entries      | Player-slot definitions                                              | Supports 4-player design                  |
| Xbox Live–style UI elements        | Unused online UI pieces                                              | Suggests networked expansion was considered |

Community consensus (fabletlcmod.com, 2000s–2010s): the system was a late-stage local co-op experiment that was cut for time. Full networking was never completed. That historical claim now needs a qualification: EgoMP publicly demonstrates independently controlled remote player creatures, but no public project is known here to have restored complete TLC co-op or gameplay/world-state replication.

---

## Modern Reimagining — design hypothesis grounded in TLC gameplay

*Fable: The Lost Chapters* is a third-person action-RPG built around a single Hero whose every action permanently shapes their body, reputation, and the world. The co-op system was clearly designed to plug into that existing fantasy rather than replace it.

### How TLC Actually Works (Relevant Systems)

- **One Hero, one story.** The player is the Hero of Oakvale. Quest progress, world state, and most major choices belong to that single character.
- **Combat is personal and multiplier-driven.** Melee, ranged, and Will (magic) feed a combat multiplier that multiplies experience. Flourish finishes, blocking, and timing matter.
- **Experience is split into Strength / Skill / Will orbs.** These are spent at the Guild to raise attributes that permanently change the Hero’s appearance and capabilities.
- **Alignment + Renown + Attractiveness/Scariness.** Good/evil deeds, trophies, clothing, hair, and tattoos all visibly morph the Hero and change how NPCs react.
- **Boasting and public performance.** Players can boast before quests for extra renown and gold, turning heroism into a public spectacle.
- **The world reacts to the Hero.** Villagers cheer, flee, or attack based on renown and alignment. Houses can be bought, trophies displayed, titles earned.

Any co-op design that ignored these systems would have felt bolted-on. The surviving code and Molyneux’s statements suggest Lionhead tried to *extend* them.

### Reconstructed “Alter Ego” Fantasy

**Core idea**
A second (or third/fourth) player does not start a parallel campaign. They import their own Hero into the *host’s* world as a living **Alter Ego** — a second expression of heroism that can fight, earn, and support, but does not own the story.

**How it would have played in practice**

1. **Drop-in**
   Friend plugs in controller or loads their save. Their Hero appears as a `CTCCoopSpirit` near the Main Hero. Appearance, equipment, and base stats come from the imported save. No lobby, no reload.

2. **Main Hero vs Alter Ego**
   - Only the Main Hero advances gold quests, opens Demon Doors tied to story, and triggers major world changes.
   - Alter Egos can fight, collect experience orbs, heal the Main Hero, open chests, and interact with most of the world.
   - A single button (White) swaps roles. The previous Main becomes an Alter Ego and vice-versa. This is why `IsMultiplayerGameActive` only checks for a seated non-main player — identity is fluid by design.

3. **Experience & Progression**
   Experience orbs collected by an Alter Ego still feed the *Main Hero’s* Strength/Skill/Will pools (or a shared pool). This keeps the single-Hero progression model intact while letting secondary players contribute power.
   Renown gains from quests and trophies remain tied to the Main Hero, but Alter Egos can earn a separate **co-op score**.

4. **The Score System (the competitive heart)**
   Helpful actions raise co-op score:
   - Healing or protecting the Main Hero
   - Landing the killing blow on tough enemies
   - Completing side objectives while the Main Hero handles the critical path
   - Successful trades of rare items

   Higher score shortens the cooldown (or raises the chance) of seizing Main Hero status.
   Leftover Arena and Hero Centre scoreboard UI implies this score was meant to be public and persistent — a light rivalry over who is the better companion.

5. **Alignment & Appearance Tension**
   Because Alter Egos are imported Heroes, they bring their own alignment and appearance. A pure good Main Hero fighting alongside a heavily corrupted Alter Ego (or vice-versa) would have created immediate visual and social contrast — villagers reacting differently to each body. This is pure *Fable*.

6. **Trading**
   Items can move between Main Hero and Alter Ego inventories. Since the Alter Ego comes from another save, this is genuine cross-save trading — something almost no single-player RPG of the era offered.

7. **Visual Language**
   The multiplayer orb effects, coloured particles, and lightning-beam remnants suggest Alter Egos were not fully opaque physical bodies. A semi-ethereal or “bound spirit” look would have made it visually clear who currently owns the story while still allowing solid combat interaction.

### What a Session Would Have Felt Like

Two friends on a couch. One is currently the Main Hero, chasing a gold quest. The other is an Alter Ego, scoring points by protecting them, finishing enemies, and occasionally demanding a role swap after a big contribution. Between fights they trade a legendary weapon or a morph potion. At the Arena the scoreboard shows who has been the better companion this week. Alignment differences make NPCs treat them differently. The story still belongs to one Hero, but heroism itself has become a shared, slightly competitive performance.

This design respects every major TLC system (single story owner, experience orbs, renown, alignment morphing, boasting, public reputation) while delivering the “cooperative but a little competitive” fantasy Molyneux described.

### Why It Was Cut
Making the above feel polished required robust save import, fair scoring, reliable replication, clear visual distinction for spirits, and desync handling. Surviving retail code and assets are consistent with parts of this fantasy, but do not prove the complete design above. EgoMP adds no evidence for save import, score-driven role swaps, shared experience, trading, quest ownership, or spirit presentation.

---

## EgoMP comparison (2026-07-26)

EgoMP was audited at commit
[`816e58f9a0acd099ac8289b6a125bbf95c590b4b`](https://github.com/98thrxse/egomp/tree/816e58f9a0acd099ac8289b6a125bbf95c590b4b).
It changes how this retail archaeology should be used for the modern
multiplayer goal.

The two bodies of evidence cover different layers:

| Topic | Retail co-op evidence in this file | EgoMP evidence | Combined conclusion |
|---|---|---|---|
| Four-player shape | `CPlayerManager` scans four slots and excludes the main player. | SLikeNet session is explicitly limited to four peers. | Four players is a well-corroborated target, but neither source proves four complete simultaneous gameplay actors. |
| Extra actors | `CTCCoopSpirit` assets/classes suggest the historical Alter Ego presentation. Seating mechanics remain unproven. | Creates extra `CPlayer` slots and attaches remote `CThingPlayerCreature` objects. | First prove two ordinary player creatures offline. Treat CoopSpirit presentation as a later archaeology/visual layer, not the spawn foundation. |
| Multiplayer-active gate | `IsMultiplayerGameActive` is real data-driven code, not a constant stub. | EgoMP forcibly returns false because allowing residual native multiplayer behavior crashes. | The retail predicate is genuine, but satisfying it is not currently safe. “No patch needed” only describes the function body, not a viable activation strategy. |
| Network update gate | `CNetworkClient+0x2662` gates the dormant event-package update path. | Does not use `CNetworkClient`, its package protocol, or this gate. It polls a separate SLikeNet peer after game update. | `+0x2662` is one gate inside an incomplete retail pipeline, not a general multiplayer enable switch. Keep it off in the first modern prototype. |
| Player lifecycle | Retail object graph and package sinks are recovered, but remote creation/teardown is not established here. | Demonstrates player-slot allocation, creature creation/attachment, roster fan-out, disconnect teardown, and lifecycle hooks after `PostInit`/`Update` and before `Shutdown`. | EgoMP is the stronger feasibility proof for the first ghost-player lifecycle. Verify every address/layout independently. |
| Region loading | Retail `CWorld` package/application paths exist; a co-op transition protocol is not recovered here. | Joining client requests the host position's region and waits until loading ends before announcing its player. | A region-ready barrier is mandatory. EgoMP only proves initial join ordering, not later synchronized transitions. |
| Event format | `CGameEvent` is a byte-exact 4-byte header plus a maximum 32-byte payload; package-set sequencing is partly recovered. | Uses seven unrelated custom messages and does not serialize `CGameEvent`. | The modern `GameEvent` codec can become a reliable gameplay-event payload, but needs a new versioned network envelope and must not be assumed wire-compatible with EgoMP. |
| Ordering and sync | Retail package sequence/high-water fields and a gutted `CheckSync` remain. | Reliable ordered control messages and unreliable sequenced transforms, but no tick, interpolation, rollback, checksum, or recovery. | Preserve the retail monotonic/idempotent lesson. Implement modern ticks, snapshots, and diagnostics outside the 136-byte retail stub before considering an in-place `CheckSync` patch. |
| Authority | Historical local co-op likely shared one process/world; authority semantics are not established by the surviving bytes. | Host assigns membership, but clients author transforms and can claim another `networkId`; host relays without sender ownership validation. | Use host-authoritative identity and simulation/validation. EgoMP's relay is a milestone prototype, not the authority model. |
| Gameplay replication | Retail event dispatch suggests a broad event system, but producer meanings and co-op ownership rules are incomplete. | Replicates only player create/destroy, position/acceleration, and facing. | Score, role swap, combat, health, inventory, quests, morality, NPCs, and persistence remain unimplemented hypotheses. |

### What EgoMP corroborates

- Multiple engine player/creature instances are feasible.
- A `network ID -> stable replication record -> engine player/creature`
  adapter is a practical boundary.
- `ResolveMovementAcceleration` and `ResolveFacingDirection` are promising
  motion seams.
- Network polling can be ordered around `CMainGameComponent` lifecycle.
- A joining player must not activate before the destination region is ready.
- Its declared `CGameEvent` fields independently agree with the recovered
  retail `0x28` object layout, although EgoMP does not use that object as its
  network wire protocol.

Useful independently verified leads from the prototype are:

| Seam | EgoMP address/usage |
|---|---|
| Player creation | `CPlayerManager::CreatePlayer @ 0x0044A1A0` |
| Player lookup | `0x004498C0`; retail also contains the separate byte-identical `0x00449910` body used by the co-op predicate |
| Creature attachment | `CPlayer::SetControlledCreature @ 0x00487CF0` |
| Player-creature creation | `CThingPlayerCreature::Create @ 0x006AC910` |
| Movement/facing | `ResolveMovementAcceleration @ 0x006AB770`, `ResolveFacingDirection @ 0x006AB820` |
| Position | `CThingPlayerCreature::SetPosition @ 0x006B0C10` |
| Initial region load | `CWorld::SetAsLoadingRegion @ 0x0049E2C0`, `UpdateRegionLoad @ 0x004A3740`, `CWorldMap::PostRegionLoad @ 0x005064C0` |
| Co-op residue only | `CTCCoopSpirit` constructor `0x00670050`, `GetScore @ 0x0066FB20`; neither is used by EgoMP's remote-player path |

These are factual leads, not imported implementations. The key source areas
are EgoMP's
[`CGameEvent` layout](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/SDK/Fable/GameEvent.h#L9-L17),
[`CPlayerManager` hooks](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/SDK/Fable/PlayerManager.cpp#L15-L35),
[`player lifecycle`](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/Multiplayer/Fable/NetPlayerManager/NetPlayerManagerLifecycle.cpp#L27-L55),
and
[`motion application`](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/Multiplayer/Fable/NetPlayerManager/NetPlayerManagerMotion.cpp#L3-L60).

### What EgoMP does not corroborate

EgoMP contains no evidence for historical Alter Ego role swapping, co-op score,
shared experience, trading, spirit presentation, quest ownership, or the
meaning of the two unknown `CheckSync` words. It also does not demonstrate
that `InitialiseAsLocal`, `+0x2662`, `IsMultiplayerGameActive`, or the retail
`CGameEventPackageSet` are safe to enable. Those remain separate retail
archaeology questions.

### Two tracks, kept deliberately separate

1. **Retail archaeology track:** continue recovering player seating,
   `CNetworkClient` initialization, package producers, event dispatch,
   `CheckSync` producer fields, CoopSpirit behavior, and crash dependencies.
   Do not enable the dormant path merely because its gates are known.
2. **Modern multiplayer track:** first create two local player creatures
   offline; then add stable entity IDs, deterministic loopback transport,
   versioned executable/content negotiation, host-authoritative ownership,
   ticked/interpolated ghost motion, and an explicit region-ready state
   machine. Add gameplay replication in layers afterward.

This separation preserves the byte-backed retail findings without making the
modern multiplayer milestone depend on completing every abandoned Lionhead
network subsystem. The fuller EgoMP protocol/security audit is in
[`EGOMP_MULTIPLAYER_AUDIT.md`](EGOMP_MULTIPLAYER_AUDIT.md).

## 1. Enable gate — byte `[CNetworkClient+0x2662]` (OPCODE-PROVEN, high confidence)
`CNetworkClient::Update` (`0x004AE9D0`) opens `mov al,[ecx+0x2662]; test al,al; je <ret>` — if the byte is
**0, Update is a total no-op**. This is a gate inside the dormant retail package pipeline, not a safe
standalone multiplayer switch. `CNetworkClient::InitialiseAsLocal` (`0x004AE940`) is what sets it: on its
base-init precondition returning `AL==1`, it writes `+0x2660=1` (master "client up"), **`+0x2662=1` (the
gate)**, clears `+0x2661/+0x2664/+0x2668/+0x266C` (replication bookkeeping), and stores `arg0 → +0x2678`
(a `CMainGameComponent*` back-pointer).

`UpdateFromEventPackageSet` does `lea ecx,[esi+0x13AB8]; call 0x4AE9D0` → **the CNetworkClient is embedded
at `CMainGameComponent+0x13AB8`**, and `+0x2662` throttles the live per-frame apply loop. It is a
necessary retail-pipeline state, but not "one flag from life": EgoMP's working
prototype avoids this client entirely, and enabling residual native
multiplayer behavior is observed to crash.

- ⚠ **CORRECTION (verifier):** the InitialiseAsLocal base-init precondition target is **UNKNOWN**
  (relocation-masked). The previously cited `0x4EBA10` is a **flat-disassembler artifact**, not a resolved
  address (`e8 08 ba 4e 00` at `0x4AE940+0x3` → base0 render `0x4EBA10`, real target masked). `FINDINGS.md`
  inherited the same phantom. Do not treat `0x4EBA10` as real.

## 2. `IsMultiplayerGameActive` (`0x00449D20`) — data-driven, NOT a stub, unsafe to activate yet
Scans `std::vector<CPlayer*>` at `this+0x0C..0x10`, loops 4 slots (`cmp edi,4; jl`), finds the element whose
`[+0x28]==edi`, fetches it via `CPlayerManager::GetPlayer` (`0x00449910`, rel32-confirmed), **excludes the
main player** (`cmp player[+0x28], this+0x1C; je skip`), then `test byte[accessor+0x91],1; je RETURN_TRUE`.
Returns true when a non-main player is seated with `[+0x91] bit0 CLEAR`. Independent of `+0x2662`.
- EgoMP deliberately keeps this result false because the residual retail
  multiplayer path crashes when activated. The byte-backed predicate needs no
  implementation patch, but the systems it gates remain incomplete.
- Unconfirmed: the `+0x91` accessor (`0x00487DC0`, rel32-solid but BSim-misnamed `_Cons_val`) and the exact
  meaning of `[+0x91] bit0` (hypothesized "not-yet-active", since CLEAR⇒active).

## 3. Replication wire format — `CGameEvent` (BYTE-EXACT both directions)
`CompressIntoBuffer` (`0x009F1810`) / `InitFromCompressedBuffer` (`0x009F1870`) are exact inverses
(`__thiscall`, `ret 4`). Little-endian, total size `4 + payloadLen`:
```
+0x00  u16 header    = (id & 0x7FFF) | (flag ? 0x8000 : 0)   // id is effectively 15-bit
+0x02  u8  subField  = CGameEvent+0x04
+0x03  u8  payloadLen= CGameEvent+0x25   (encoded as u8; safe object capacity is 0..32)
+0x04  u8[payloadLen] payload            (from CGameEvent+0x05, rep movsd+movsb)
```
"Compress" = pack dense (no compression, no endian swap). Producer returns `payloadLen+4` (records
concatenate). Consumer sets id=`hdr&0x7FFF`, `+0x27`=flag, `+0x25=0 then +=len`, and `+0x26=1`
(pending/dirty apply flag). A `CGameEventPackageSet` frames N records; its own compress/init are
`0x9F19A0`/`0x9F1AC0` (per FINDINGS, not decoded here).

**Safety correction (2026-07-25):** although the length field can encode 0..255, the proven
`CGameEvent` layout is only `0x28` bytes and its payload occupies `+0x05..+0x24` (32 bytes).
The retail deserializer performs no bound check, so a length above 32 would overwrite adjacent
fields/object memory. Modern readers must reject it. The C++23 proof-of-concept codec under
`rebuild/modern/multiplayer/` enforces that capacity and tests the exact wire bytes.

EgoMP's `CGameEvent` declaration independently corroborates these member
offsets, but its network implementation uses unrelated custom SLikeNet
messages. It therefore validates the object layout, not the retail codec,
package framing, or network safety. Never pass untrusted network lengths to
the retail unchecked deserializer.

## 4. Apply chain (all LIVE except CheckSync)
`UpdateFromEventPackageSet` (`0x0041726D`) — iterates the package set; **sequence gate: apply a package
only if `package[+0] > this+0x4C`** (last-applied cursor; `jle` skips stale → monotonic/idempotent).
On apply: `this+0x4C := seq`; `this+0x48 := max(hw,seq)`; caught-up byte `this+0x161E1 = 1` iff
`+0x4C==+0x48`; forwards the package to `CWorld` (`this+0x24` → **`0x0049DFB0 CWorld::Update`** —
⚠ corrected from the synthesis's wrong `0x0049E0B0`) and `CDisplayEngine` (`this+0x28`; sink
`0x00434A60 ConfirmFeedbackGameEventPackage`, finalize `0x00434F60 WorldUpdate`); and pokes the embedded
CNetworkClient (`+0x13AB8` → `CNetworkClient::Update`).
→ `ProcessEventPackage` (`0x00416670`) iterates a package's **events** → `ProcessEvent` (`0x00415FE0`,
the per-event-type dispatch).
- ⚠ **CORRECTION (verifier):** the loop accessors (`0x9F16E0/0x9F16D0` for events, `0x9F1750/0x9F1730`
  for packages) were falsely claimed "verified as GetCount/GetAt against engine_api.tsv" — the TSV
  actually BSim-mislabels them (DrawGetEnvironment, etc.). They ARE the count/at iterators structurally,
  but there is no TSV name confirming it. Also note these are **two different container levels**: a Set of
  packages (element[+0]=SEQUENCE) vs a package's events (element[+0]=TAG).

## 5. `CheckSync` (`0x004165E8`) — GUTTED / STUBBED (dataflow-proven; the one piece to REBUILD)
Reads three remote u32s and the local world checksum, then **discards all of it** — no `cmp`/`jcc` on any
sync value, no desync flag, no report call. Specifically: zeroes 3 accumulators (never read); calls
`PeekWorldChecksum` (`0x00416392`) then overwrites EAX before use (discarded); for each **event with
tag==1** (`cmp [event+0],1`) does `CEventPacket::CopyToBuffer` + **three `CircularBuffer_Read(&x,4)`** into
locals that are never read back; `leave; ret 4`.

`PeekWorldChecksum` (`0x00416392`, oracle-confirmed): `cmp [ecx+0x1611A],dl; je → mov ecx,[ecx+0x24]; jmp
0x0049E200` (**tail-jump to `CWorld::GetChecksum`**). So `this+0x24 = CWorld*`; `this+0x1611A` is a
suppress-checksum byte (nonzero ⇒ returns literal 1).

### Archaeology rebuild spec

An experimental parity implementation can still be authored as an oracle
`.cpp`, byte-exact into the 136-byte slot, keeping the /GS prologue/epilogue
`0x139C8A8` + `__security_check_cookie 0x00BFE9F9`:
For each tag==1 event: read the 3 remote u32s (`r0,r1,r2`) as now; compute local `l0 =
CWorld::GetChecksum(this+0x24)`, and `l1,l2` = the two companion counters; if any differ, latch a desync
flag / raise the sync-error path.
- ⚠ **Field identities of `r1,r2` are SPECULATION** (not high-confidence): only `l0`=world checksum is
  grounded. `l1,l2` are *hypothesized* to be `this+0x4C` (last-applied seq) and `this+0x48` (high-water),
  but the tag-1 sync-event **producer** (which fills the three u32s) was not in this cluster and must be
  found to pin them. The desync **reaction** the retail debug build had is fully removed — a rebuild must
  invent a plausible one (candidate sinks: `+0x161E1` caught-up byte, `+0x1613C`).

For the modern multiplayer track, do not make the 136-byte retail slot the
primary synchronization design. First recover the tag-1 producer, then model
checksums, tick/sequence diagnostics, mismatch reporting, and recovery in
ordinary tested code. An in-place parity experiment can follow once the three
field meanings are grounded.

## Retail revival experiment (ordered) — with the verifier's crash caveats
1. **Recover seating before enabling it** — identify exactly how a non-main
   `CPlayer*` enters the four-slot manager and how its creature is attached.
   EgoMP demonstrates this with ordinary `CThingPlayerCreature` instances; it
   does not prove that spawning a `CTCCoopSpirit` performs the seating step.
2. **Proper enable via `InitialiseAsLocal`** — prefer this over a raw poke: it sets `+0x2660/+0x2662=1`
   AND stores `+0x2678=arg0`. ⚠ **Raw-poking `+0x2662=1` alone can CTD**: Update's forwarder entry points
   deref `[ecx+0x2678]`; if InitialiseAsLocal hasn't stored `+0x2678`, that's a stale/null deref. Do not
   NOP the unresolved base-init guard: recover its target and preconditions before testing this path in
   an isolated harness.
3. **Confirm the pipeline moves** — watch `+0x4C`/`+0x48` advance as events apply.
4. **Recover the tag-1 producer, then rebuild CheckSync** (§5) so the three
   compared fields are known before any desync policy is invented.
5. Only then diff `InitialiseAsNetworkClient/Host` (near `0x4AE940`) to learn the host/client mode-byte
   pattern before wiring a transport (LSocket / Steamworks P2P / GameNetworkingSockets).

This experiment is not the critical path for the first modern ghost-player
milestone. That milestone should use offline two-creature proof, loopback, and
host-authoritative sidecar replication while the retail pipeline continues to
be recovered independently.

*Artifacts: `work/coop_re/` (bundles, `checksync_md.txt`, `summary.txt`). Full synthesis + verify in the
task output journal. Supersedes/refines the co-op section of `FINDINGS.md` (which shares the `0x4EBA10`
phantom).*
