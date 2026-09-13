# New Oakvale authored speech audit — 2026-09-12

The port invokes the installed native macro interpreter with the authored asset
name. It does not reimplement the speech verbs in Lua. The nine referenced
CCutsceneDef records contain 28 speech commands: 17 Speak, eight InteractiveSpeak,
two DialogadSpeak and one DialogSpeak. All are in the normal command vectors;
the skip vectors contain no speech commands. Empty vector elements are preserved.

Reproduce:

```powershell
python tools/script_recovery/audit_oakvale_macro_speech.py --output work/macro-speech-review-new.json
```

The archived evidence name is `native-macro-speech-operands-20260912.json`.
It records executable, installed script.bin, individual asset payload and Lua
source hashes, complete normal/skip command vectors, 29 exact instruction
assertions and native byte slices. Decoding begins at the verified interpreter
entry 0xCBFB7D. These are reviewed static slices, not full interpreter emulation.

## Native operands

Resource Speak uses six stack arguments: target, text, selection, listen,
soundIn2D, overFade. For `.Speak`, both actor-map and fallback-target paths
(calls 0xCC2813 and 0xCC289F) push listen=0, soundIn2D=1 and the parser's
black-screen-subtitle byte as overFade. The byte initializes to zero at
0xCBFC56. `EnableBlackScreenSubtitles` sets it to one at 0xCC5EF1; none of the
nine assets contains that command. Selection initializes to zero at 0xCC268C;
the parser recognizes random/norepeat/sequence as 1/2/3. The authored Speak
commands here omit that selection argument. Father and Theresa often target
the speaking actor itself; that is authored retail behavior.

`.DialogadSpeak` explicitly pushes 0,1,0 for listen/soundIn2D/overFade on both
target paths (calls 0xCC353A and 0xCC35BD). This includes Bully's apology.

`.InteractiveSpeak` creates its conversation with true,true (0xCC2F5B), then
queues each supplied line with the line boolean true (0xCC2F99 through
0xCC30B3). Father supplies an additional line in argument four; the parser
contains seven possible line slots. `.DialogSpeak` creates a conversation
with true,true when it cannot reuse an active conversation (0xCC32CA), then
queues a line with true (0xCC330A). These differ deliberately from native
direct-quest conversation creation false,false and line boolean false.
The direct-dialogue audit must not be used to change native macro defaults.

## Lua routing

Seven RunCutsceneWithSetup source call sites cover these nine names. Father
passes HERO/Father; Theresa's helper passes its caller's actor table; Bully
passes HERO/BRAT/BULLY. RUN1 passes only the authored `$BRATLINE` text replacement,
choosing the two native victim-thanks alternatives. The runtime separates string
input replacements from boolean flags and calls RunCutsceneMacro_Func with
name, actor map, flag map, input map, setup and skippable at LuaQuestState.cpp's
RunCutsceneWithSetup implementation. No speech command or speech flag is
rewritten there. This static routing check does not certify resource scheduling
or actor identity during every live streaming transition.

## What remains

Together with DIRECT_DIALOGUE_OPERAND_REVIEW and CONVERSATION_LINE_REVIEW,
this covers the inventoried New Oakvale speech routes. No additional audio
operand mismatch was found. The earlier four numeric-zero conversation fixes
remain justified by the actual sol conversion tests.

Matching input flags does not establish output gain, panning, backend behavior
or the exact sample heard. A new external capture still needs the affected
NPC/line and output-device context. Correlate sample identities and listener
movement with the recording; partial observer scans cannot prove absence.

Fade visibility remains separate. The retail early-skip branch intentionally
omits Bully's run and hides him after FadeOut/pause. A fade timer reaching its
opaque state does not establish that opaque pixels were drawn. Restored-v23
Father Escape and actual Bully fade rendering remain live acceptance checks.
Installed v23 and all game assets were left unchanged by this audit.
