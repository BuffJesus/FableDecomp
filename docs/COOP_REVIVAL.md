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

Community consensus (fabletlcmod.com, 2000s–2010s): the system was a late-stage local co-op experiment that was cut for time. Full networking was never completed. No public mod ever restored live multi-player control of independent characters in the same world.

---

## Modern Reimagining — Grounded in Actual TLC Gameplay

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
Making the above feel polished required robust save import, fair scoring, reliable replication, clear visual distinction for spirits, and desync handling. The code left behind is the skeleton of exactly this fantasy; the missing pieces were the ones that take the longest to finish.

---

## 1. Enable gate — byte `[CNetworkClient+0x2662]` (OPCODE-PROVEN, high confidence)
`CNetworkClient::Update` (`0x004AE9D0`) opens `mov al,[ecx+0x2662]; test al,al; je <ret>` — if the byte is
**0, Update is a total no-op**. `CNetworkClient::InitialiseAsLocal` (`0x004AE940`) is what sets it: on its
base-init precondition returning `AL==1`, it writes `+0x2660=1` (master "client up"), **`+0x2662=1` (the
gate)**, clears `+0x2661/+0x2664/+0x2668/+0x266C` (replication bookkeeping), and stores `arg0 → +0x2678`
(a `CMainGameComponent*` back-pointer).

`UpdateFromEventPackageSet` does `lea ecx,[esi+0x13AB8]; call 0x4AE9D0` → **the CNetworkClient is embedded
at `CMainGameComponent+0x13AB8`**, and `+0x2662` throttles the live per-frame apply loop. That is the
"one flag from life."

- ⚠ **CORRECTION (verifier):** the InitialiseAsLocal base-init precondition target is **UNKNOWN**
  (relocation-masked). The previously cited `0x4EBA10` is a **flat-disassembler artifact**, not a resolved
  address (`e8 08 ba 4e 00` at `0x4AE940+0x3` → base0 render `0x4EBA10`, real target masked). `FINDINGS.md`
  inherited the same phantom. Do not treat `0x4EBA10` as real.

## 2. `IsMultiplayerGameActive` (`0x00449D20`) — data-driven, NOT a stub, no patch needed
Scans `std::vector<CPlayer*>` at `this+0x0C..0x10`, loops 4 slots (`cmp edi,4; jl`), finds the element whose
`[+0x28]==edi`, fetches it via `CPlayerManager::GetPlayer` (`0x00449910`, rel32-confirmed), **excludes the
main player** (`cmp player[+0x28], this+0x1C; je skip`), then `test byte[accessor+0x91],1; je RETURN_TRUE`.
Returns true when a non-main player is seated with `[+0x91] bit0 CLEAR`. Independent of `+0x2662`.
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

### Rebuild spec (author as an oracle `.cpp`, byte-exact into the 136-byte slot, keep the /GS
prologue/epilogue `0x139C8A8` + `__security_check_cookie 0x00BFE9F9`):
For each tag==1 event: read the 3 remote u32s (`r0,r1,r2`) as now; compute local `l0 =
CWorld::GetChecksum(this+0x24)`, and `l1,l2` = the two companion counters; if any differ, latch a desync
flag / raise the sync-error path.
- ⚠ **Field identities of `r1,r2` are SPECULATION** (not high-confidence): only `l0`=world checksum is
  grounded. `l1,l2` are *hypothesized* to be `this+0x4C` (last-applied seq) and `this+0x48` (high-water),
  but the tag-1 sync-event **producer** (which fills the three u32s) was not in this cluster and must be
  found to pin them. The desync **reaction** the retail debug build had is fully removed — a rebuild must
  invent a plausible one (candidate sinks: `+0x161E1` caught-up byte, `+0x1613C`).

## Revival plan (ordered) — with the verifier's crash caveats
1. **Seat a co-op player first** — spawn/seat a `CTCCoopSpirit` so a non-main `CPlayer*` exists (satisfies
   IsMultiplayerGameActive). ⚠ *Unproven* that a CoopSpirit (a `CThing`) is inserted into CPlayerManager's
   `CPlayer*` vector with the right `+0x28`/`+0x91` — the data condition may need more than "spawn a spirit."
2. **Proper enable via `InitialiseAsLocal`** — prefer this over a raw poke: it sets `+0x2660/+0x2662=1`
   AND stores `+0x2678=arg0`. ⚠ **Raw-poking `+0x2662=1` alone can CTD**: Update's forwarder entry points
   deref `[ecx+0x2678]`; if InitialiseAsLocal hasn't stored `+0x2678`, that's a stale/null deref. If the
   masked base-init precondition blocks the commit, NOP its `cmp al,1; jne` guard so local mode always commits.
3. **Confirm the pipeline moves** — watch `+0x4C`/`+0x48` advance as events apply.
4. **Rebuild CheckSync** (§5) so desyncs are detected before any real transport.
5. Only then diff `InitialiseAsNetworkClient/Host` (near `0x4AE940`) to learn the host/client mode-byte
   pattern before wiring a transport (LSocket / Steamworks P2P / GameNetworkingSockets).

*Artifacts: `work/coop_re/` (bundles, `checksync_md.txt`, `summary.txt`). Full synthesis + verify in the
task output journal. Supersedes/refines the co-op section of `FINDINGS.md` (which shares the `0x4EBA10`
phantom).*
