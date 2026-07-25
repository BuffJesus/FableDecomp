# Co-op revival — enable gate, replication protocol, CheckSync rebuild (2026-07-24)

RE'd via the Claude Workflow loop (`coop-revival-re`: 8 decode agents → synthesis → adversarial
verify). Verdict **PLAUSIBLE**: the three headline conclusions are byte-solid and survived refutation;
the synthesis also overstated some evidence, and those overclaims are **corrected inline below** — trust
this file, not the raw synthesis. Bytes from retail `Fable.exe`; cluster identity is a BSim port from
`ego_r.exe` + `FINDINGS.md` (same lineage — the CLAUDE.md ≥2-source bar is only weakly met for *names*,
but the *opcode facts* are directly from retail bytes).

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
+0x03  u8  payloadLen= CGameEvent+0x25   (0..255)
+0x04  u8[payloadLen] payload            (from CGameEvent+0x05, rep movsd+movsb)
```
"Compress" = pack dense (no compression, no endian swap). Producer returns `payloadLen+4` (records
concatenate). Consumer sets id=`hdr&0x7FFF`, `+0x27`=flag, `+0x25=0 then +=len`, and `+0x26=1`
(pending/dirty apply flag). A `CGameEventPackageSet` frames N records; its own compress/init are
`0x9F19A0`/`0x9F1AC0` (per FINDINGS, not decoded here).

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
