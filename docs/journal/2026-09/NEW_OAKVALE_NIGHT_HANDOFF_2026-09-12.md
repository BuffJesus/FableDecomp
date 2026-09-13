# New Oakvale: overnight handoff, September 12

User explicitly stopped for sleep. Work is paused. Do not launch the game,
continue a marathon, or request playtesting until the user returns. Game was
closed at this handoff's process check. Preserve all uncommitted work and the
converter session's ownership. No commits, resets, stashes or publication.

## Current outcome

The user completed the entire childhood on the isolated original-FSE/add-on
candidate and reported retail-like behavior except quiet speech in cutscenes.
Father Escape works again; Bully disappearing when Escape skips the scene was
compared with retail and accepted by the user. Do not revive the earlier forced
run-off workaround or change retail skip/reward ordering.

Audio means **the locked, authored camera faces the speaking NPC**, especially
Father, and his speech becomes extremely quiet. Shots facing Hero or away from
Father sound normal. User wears headphones. This is not a movable-camera test.

## Confirmed audio failure path

1. V5 captured the actual native DirectSound query returning success, raw config
   `8` (`DSSPEAKER_7POINT1_SURROUND`), at tick45090750.
2. Native C53340 recognizes only configurations1..7. Config8 becomes sentinel
   `0x80000000`. Pinned native execution tests reproduce this mapping.
3. The native selector maps that sentinel to mode`0x40`, six channels. The
   C1C700 proxy gate disables C1C460 relative-position updates.
4. V4 directly captured204 identical mode/gate/channel observations and52 relative
   voice observations retaining submitted position(0,0,0.5). Distance/cone factors
   remained1. This proves the routing/position failure path, not an audible fix.
5. A separate DirectSound probe reports stereo (`0x00140004`), including while
   Fable runs. Retail's exact C53260 initialization instructions, executed in an
   inert standalone probe against real Windows COM, also select stereo.

The per-game reason for the different DirectSound result is unresolved. System
SysWOW64/dsound.dll owns the live COM query function. Windows compatibility DLLs
are loaded but are not a proven cause. Do not unconditionally map actual7.1
systems to stereo or claim Windows default settings alone explain this run.

Detailed native addresses, evidence and limitations:
[locked-cutscene audio journal](LOCKED_CUTSCENE_AUDIO_2026-09-12.md).

## Builds and rollback

Installed game: `C:/Programs/Steam/steamapps/common/Fable The Lost Chapters`.
All25 deployment-v23 files still match the manifest; duplicate New Oakvale
activation is absent. Both installed DLLs:
`c3900b3db85423e1a7e4035825b265225e4892eb26522b92580aa2a055539d31`.
Audit: `work/new-oakvale-night-handoff-audit-20260912.json`.

Local candidates live under `work/new-oakvale-original-fse-20260912/`:

| Candidate | What it establishes |
|---|---|
| `local-test-ready` | Full childhood completed; no native audio hook |
| `local-test-audio-v4` | External observer captured mode/gate/six channels; same binaries/Lua as successful childhood |
| `local-test-audio-v5` | Main-menu raw-query diagnostic; changed add-on, unchanged original FSE/launcher/Lua |

Original FSE is locally compiled, source-unmodified upstream commit
`b2432e82b2ef1e5fd50b8fac2a8809bd2bd107d6`, not a maintainer release binary.
DLL hash: `36ffb32ce4f1689d08cd5deef37829760d9f2a840aa9e801e79c86135fdb95ea`.
V5 diagnostic add-on hash:
`be814f1a6b8311ccce193267803491d91aa7aace1db143269c269325093987b0`.
Successful childhood add-on hash:
`74e2b482d12196be91c743152becf20243db43b5089a0e9df6c6f8108ded5755`.

No installed DLL/config changes are needed to use these candidates. Normal
installed FSE_Launcher.exe returns to v23. Saves are shared and backed up per run;
closing a candidate does not undo saved progress. No public distribution yet.

## Preserved captures

- Childhood: `local-test-ready/runs/20260912-192855-005904/`;
  `childhood-review.json`, original/add-on final logs, observer, save backup.
- Father mode/gate: `local-test-audio-v4/runs/20260912-195741-984877/`;
  `audio-review.json`, final logs, observer and backup.
- Raw startup query: `local-test-audio-v5/runs/20260912-202301-591016/`;
  `startup-result.json`, menu/final logs, COM/module snapshots, observer and backup.

Paths above are relative to the local-candidate parent directory. V5 did not
repeat Father or full childhood. Do not describe it as a validated audio fix.

## Resume checklist

- [x] Preserve successful childhood and original FSE bytes.
- [x] Confirm raw native configuration, mode translation, live channel count and
  position-update gate; archive evidence.
- [x] Build V5 and pass preflight; compare actual optimized hook against eight
  native success/failure/flag cases. FXSAVE FIP metadata bytes8..15 are excluded
  because of emulator behavior; logger body is doubled. No full hardware proof.
- [x] Run18 observer/analysis checks and12 native mode/selector cases.
- [ ] Explain the game-specific7.1 report: selected device/context, configuration
  writes, or compatibility behavior. Read-only module ownership alone is insufficient.
- [ ] Choose a narrow correction with explicit scope, keeping original FSE
  unchanged. No blind global volume gain, changed speech flags or universal
  stereo remapping. No correction has been installed.
- [ ] Test correction offline, then coordinate a Father listening comparison.
  Confirm output mode, transformed positions, audible result and Escape.
- [ ] Verify save/reload and coexistence with other original-FSE custom quests
  before distribution. Full childhood completion does not establish those gates.
- [ ] Fix the sidecar's legacy quest-file self-heal path assumption separately;
  it derives a nonexistent `NoviCompatibi/data/...` path. Preflight already checks
  activation. The warning did not demonstrate an installed-file modification.
- [ ] Resume the next supported script port after audio priority is resolved.
  MeetSister/MazeResearch/GuardianTrophyDealerInfo staged work remains disabled;
  GuardianSister entity Main is incomplete. See the extended marathon journal.

Converter-owned files remain untouched. Recent husband operand and cancellation
recoveries are useful port cross-checks, not an audio solution. Re-read the
converter's own handoff on return; do not assume tonight's snapshot is its latest.

V5 launch command, **only when the user requests it**:

```powershell
python work/new-oakvale-original-fse-20260912/local-test-audio-v5/local_test.py --game-dir "C:/Programs/Steam/steamapps/common/Fable The Lost Chapters" --launch
```

The needed V5 main-menu result is already captured. Do not request the same
diagnostic again without a new question that it can resolve.
