# Locked cutscene audio: first live backend evidence

**Overnight stop:** user is going to sleep. Current summary, verified build state
and remaining checklist are in [the overnight handoff](NEW_OAKVALE_NIGHT_HANDOFF_2026-09-12.md).
No further launch or automatic continuation until the user resumes.
The sections below preserve the investigation in order; V5 at the end establishes
the raw unsupported configuration. Earlier unknown-mode statements are historical.

User confirms the only reported issue after completing childhood is quiet speech
when the authored, locked cutscene camera faces the speaking NPC. Normal-volume
shots face the Hero or elsewhere. Headphones. No manual camera test is applicable.

Capture: work/new-oakvale-original-fse-20260912/local-test-ready/runs/
20260912-192855-005904/observer.jsonl. Archived statistics and relative records:
work/locked-cutscene-audio-findings-20260912.json. Father-start window records:
work/father-backend-relative-window-20260912.json.

## Observations

-7370 readable listener samples;750 complete emitter and backend snapshots.
-1345 decoded backend voice observations, all directional factors1 and cone
 parameters360/360/1. The proposed cone attenuation does not appear in these samples.
-215 decoded relative-voice observations ALL retain submitted backend position
 (0,0,0.5). Distance factor remains1 in these observations, so the recorded large
 distance itself is not proof of distance attenuation causing the quiet output.
-During the Father-start window, relative backend voices show distances near3600
 and local positions that change with listener orientation, while the listener
 is near world position3495,867,16.47 observations fall in this window. Voice
 identity is not yet a one-to-one mapping to exact Father dialogue keys.
-Backend source flags are120001/120002/120004 hexadecimal; bit40000 is absent.
 Native C41FD0 tests that bit to select the listener-relative copy path. Absent
 bit takes the world/listener subtraction path atC42079. This matches captured
 local geometry but is not by itself proof which earlier update was skipped.

## Native follow-up

C1C460 normally converts relative source coordinates to a point ahead of the
listener before callingC33FD0/C41320. Existing91-case native execution proves
the geometry when update gates permit it. The live records instead retain the
initial point near world origin. Need observe the missing proxy+19 gate.

C10ABD reads mixer+7BC as that proxy. C1C700 initializes proxy vtable12B3290
and gate+19=false. C1C738..74B enables it when the value returned throughC33590
is1,2or4. C33590 callsC3EA60, which reads an object at backend+188 and its+4,
or returns80000000 when absent. Do not infer the live value or its enum semantics
from Windows default stereo settings. It was not captured in this run.

Observer now reads proxy+19 with mixer/proxy vtable validation.17 relevant checks
pass, including0/1 gates, relocation and unknown-class rejection. Also fixed
the packaging path bug by supplying --proof and bundling the pinned proof JSON.
This avoids dependence on the repository-relative path after copying the observer.

Next capture package: work/new-oakvale-original-fse-20260912/local-test-audio-v3.
Preflight passed; all three binaries AND all quest Lua are byte-identical to the
successful childhood run. Only observer/launch-support files changed. No native
audio patch or audio configuration change was made. Launch timing requested from
the user. Record proxy gate during the same locked Father scene before proposing
a fix; final channel output remains unmeasured.

V3 additionally records the actual native mode getter input through143BB5C,
backend+188 and mode-object+4, including native80000000 fallback. A mode-read
failure is reported separately without discarding the successfully read proxy
gate. This avoids requiring another capture solely to learn the mode input.

Converter update checked during this investigation: husband question operand,
animation flag and cancellation-cleanup recoveries are useful port cross-checks.
No converter result establishes an audio fix. Converter-owned files unchanged.

## Offline follow-up: DirectSound mode and output selection

Native C3FDEF creates the object at backend+188 with vtable12B5BB0.
Its initialization C53260 calls imported CoCreateInstance with the exact
CLSID_DirectSound and IID_IDirectSound GUID bytes. Interface slot+20 in
C53340 is GetSpeakerConfig, verified against the installed Windows SDK
10.0.26100.0/um/dsound.h (interface order and GUIDs). It translates the low byte:

| DirectSound config | Native mode | Position gate | Initial channels |
|---|---:|---:|---:|
| Headphone (1) | 1 | enabled | 2 |
| Mono (2) | 2 | enabled | 6 |
| Quad (3) | 8 | disabled | 4 |
| Stereo (4) | 4 | enabled | 2 |
| Surround (5) | 80000000 hex | disabled | 6 |
| Legacy 5.1 (6) | 40 hex | disabled | 6 |
| Legacy 7.1 (7) | 80 hex | disabled | 6 |
| Configs 0, 8, 9, 255 | 80000000 hex | disabled | 6 |

The unusual mono result is the actual C3F110 initial branch, not an intended
speaker recommendation. Device-open failures can cause retries via C3F710 and
C3F280; this table does not assert the final live channel count.

Executed the native C53340 successful-query path and the constructor/selector
instruction slices with twelve synthetic COM results, including geometry bits.
All passed. Only GetSpeakerConfig was doubled. Harness:
work/runtime-20260912-video-comparison/prove_audio_mode.py; proof:
work/native-speaker-mode-proof-20260912.json.

Built and ran a read-only x86 helper using that same DirectSound interface.
Default device returned config1310724 (00140004 hex), low byte4 = stereo.
No SetSpeakerConfig, game launch, or audio setting change. This is a separate
process after game exit and does NOT establish the captured game's device/mode.
Evidence and existing-capture listener-gate counts:
work/directsound-mode-investigation-20260912.json.

Observer additionally records backend+1B8 selected mode and+1BC channel count,
with backend vtable12B5BC0 validation and separate partial-read reporting.
18 focused observer/analysis checks passed. New package:
work/new-oakvale-original-fse-20260912/local-test-audio-v4. Preflight passed;
26 binary/Lua files byte-identical to the successfully completed childhood run.
No runtime audio fix installed. User is not yet ready to launch.

Remaining discriminator: live proxy gate, translated DirectSound mode, actual
selected output mode/count, and voice geometry in the same Father scene.
The external stereo result prevents treating surround misconfiguration as a
confirmed cause. Channel output and the reported audible loss remain unproved.

## V4 live Father capture: position gate and six-channel output confirmed

User authorized launch and reported done. Run directory:
work/new-oakvale-original-fse-20260912/local-test-audio-v4/runs/20260912-195741-984877.
Game PID29984 subsequently exited. Archived original-final.log, addon-final.log,
and audio-review.json beside observer.jsonl. Father macro entered and returned.

2350 JSON records;204 complete gate observations ALL show:
- native mode80000000 hex (unrecognized/default sentinel);
- proxy+19=0, disabling C1C460 position updates;
- selected backend mode40 hex and six output channels.

52 relative-voice observations ALL retain submitted position(0,0,0.5), with
distance and directional factors1. This now establishes the gate responsible
for the missing relative-position transformation in this run. It also establishes
that the game selected six channels despite the separate default-device helper
reporting stereo. It does not yet establish the raw GetSpeakerConfig result,
why the sentinel was retained, the exact Father line for every voice sample,
or final audible channel levels. Do not label this an audio fix.

The startup module list includes Windows compatibility modules AcGenral and
AcXtrnal; their presence alone does not prove an audio shim caused the selection.
Next work: trace the raw configuration/initialization path or independently
reproduce its sentinel, then validate a narrowly scoped correction while keeping
the original FSE DLL unchanged. No configuration or runtime patch applied here.

## Exact native initialization against real Windows COM

The read-only helper now reproduces the retail DSSCL_PRIORITY step with its own
hidden window. DirectSound returns00140004 hex both before and after that step.

An additional x86 probe copies the pinned native initialization code/data into
bounds-checked reserved memory at its original virtual addresses, binds only the
three required ole32 imports, and explicitly calls C53260, followed by C53310
cleanup. It never executes the game entry point, TLS callbacks or gameplay.
Result: native initialization succeeds, retains its COM object, and selects
mode4 (stereo). Evidence: work/retail-audio-init-real-com-20260912.json.
Source: work/runtime-20260912-video-comparison/execute_retail_audio_init.cpp.

This narrows the difference to the live game's initialization context. It does
not explain that difference. An optional diagnostic for an isolated sidecar
records the actual HRESULT and raw DWORD immediately after the native
GetSpeakerConfig call atC53352. Failed HRESULT means the DWORD is not valid.
It replays the original TEST/JGE/failure-MOV and preserves general registers,
flags and floating-point/SIMD state around logging; no forced mode or changed
speech operands. Source: work/runtime-20260912-video-comparison/NoviAudioQueryTrace.inl.

Eight compiled-hook/native-branch comparisons pass across success/failure and
flag states, with a callback double that deliberately clobbers registers and
floating-point/SIMD data. Proof: work/audio-query-hook-check-20260912-c/proof.json.
Limit: Unicorn FXSAVE instruction-pointer metadata bytes8..15 are excluded after
observing its FNINIT/FXRSTOR behavior; floating-point values/control/status and
SIMD values are compared. This is not a full hardware or gameplay proof.

The installed original Readme.rtf also documents quiet/intermittent dialogue
with headphones/two speakers and recommends desktop stereo. That primary
historical guidance is consistent with the routing lead but does not explain
why this game run selected six channels while the probes select stereo.

Diagnostic candidate now assembled and preflight passed:
work/new-oakvale-original-fse-20260912/local-test-audio-v5.
Built from a fresh copy of the successful sidecar source, adding only the
audio-query trace include/install call. Diagnostic add-on SHA256:
be814f1a6b8311ccce193267803491d91aa7aace1db143269c269325093987b0.
25 original-FSE/launcher/Lua files are byte-identical to v4. Installed v23 files
are untouched. Unlike v4, v5 contains an in-process diagnostic hook; no claim
that it is an external-only observer or identical add-on DLL.

The actual optimized candidate DLL's exported hook also passed all eight native
machine-state comparisons: work/audio-query-candidate-binary-check-20260912/proof.json.
Same FIP metadata limitation applies. No live validation yet.
Next authorized launch needs only reach the main menu for the initialization
result; inspect [NoviAudioQuery] before requesting another full Father scene.

## V5 main-menu capture: raw unsupported 7.1 config confirmed

User authorized launch then reported main menu. PID24808, run:
work/new-oakvale-original-fse-20260912/local-test-audio-v5/runs/20260912-202301-591016.
Native post-query trace: tick45090750, object015550A8, HRESULT00000000,
raw00000008, rawValid1. SDK names config8 DSSPEAKER_7POINT1_SURROUND.
Native C53340 only handles values1..7 and sends8 to its unknown sentinel;
the previously proved selector maps that sentinel to six channels and disables
relative position updates. This establishes the previously missing raw input.

Archived addon-menu.log/original-menu.log, startup-result.json, main-menu-com.json
and main-menu-modules.json. Read-only inspection finds the retained COM object's
GetSpeakerConfig pointer and vtable both belong to the system SysWOW64/dsound.dll,
not a game-local replacement. Windows compatibility DLLs are present but remain
an unproved cause. External helper STILL reports00140004 (stereo) while Fable
runs; all three Windows default endpoint roles report Realtek stereo, mask3.

The user was told the capture is complete and they can close the game. No runtime
correction yet. Need explain per-game DirectSound context or provide a narrowly
scoped, explicitly selected stereo compatibility path and verify audible output;
mapping8 to stereo unconditionally would misrepresent actual 7.1 systems.
