# New Oakvale direct dialogue operand audit ? September 12, 2026

Bounded audit complete: 102 native Speak calls and 18 native conversation creations
across the 13 reviewed speaking entities. Direct speech always passes listen=false,
soundIn2D=true, overScreenFade=false. Conversation creation always passes false,false.
This supports the earlier four Guard/Villager numeric-zero fixes; no additional
speech-flag change is justified. It is not a measured audible-output result.

## Coverage and evidence

`tools/script_recovery/audit_new_oakvale_speech_operands.py` traverses the functions
named in the reviewed entity inventories, including the four-way Barrel Man jump
at DB5526 (bounds DB551D, table DB6B24). It rejects unknown indirect branches,
nonconstant flag operands, calls consuming unreviewed arguments, and alternate
entries into argument slices. Callee-saved register constants are joined across
all CFG predecessors; volatile registers become unknown at calls. Six tests cover
conflicting joins, partial writes, loop convergence and unknown-jump rejection.
This is static x86 analysis with the standard native callee-saved ABI assumption,
not execution of every quest branch. Every speech slice forwards the immediately
preceding CGSI+118 Hero result as its target. Bytes, addresses, keys and constants
are archived in `native-dialogue-operands-complete-20260912.json`.

| Entity | Native Speak | Native conversations | Selection methods |
|---|---:|---:|---|
| NOVI_AffairMan | 8 | 2 | 0 |
| NOVI_AffairWife | 6 | 3 | 0 |
| NOVI_AffairWoman | 2 | 0 | 0 |
| NOVI_BarrelMan | 7 | 2 | 0 |
| NOVI_BarrelThug | 6 | 2 | 0, 2 |
| NOVI_BookTrader | 7 | 1 | 0 |
| NOVI_Bully | 9 | 2 | 0 |
| NOVI_Guard | 19 | 2 | 0 |
| NOVI_LiveFather | 10 | 0 | 0 |
| NOVI_TeddyGirl | 12 | 1 | 0 |
| NOVI_Theresa | 8 | 0 | 0 |
| NOVI_Victim | 6 | 1 | 0, 2 |
| NOVI_Villager | 2 | 2 | 1 |

The 18 native conversation instructions map to 17 Lua creation sites: Barrel
Thug shares one Lua helper between its two native creation paths. Both native
paths have the same false,false flags. The source uses method 2 for Barrel Thug
SCRMSG_WELLDONE/SCRMSG_TEMPT and Victim EVIL_BROS, matching DB7289/DB7345/DBDBDA.
Villager uses method 1; the other recovered direct calls use method 0.

## Runtime conversion checks

Clean runner result: `work/runtime-dialogue-operands-final-20260912/result.json`.
The actual three-overload Speak registration is extracted verbatim from
LuaManager.cpp and compiled with the current vendor sol/Lua and enum header.
All 13 source call expressions plus both method-2 variants pass, preserving
borrowed speaker, owned Hero target, text key, selection and 010 flags. The
Speak_Blocking body forwards these values unchanged to expert vtable+34; its
engine speech task and audio output are outside this test. A fixture changing
soundIn2D to false fails with exit 1 (speech-operands-negative.log). All 17
conversation-creation expressions still pass the real C++ bool conversion check.
No runtime or installed-file change was made for this audit.

## Decompiler correction and remaining boundary

Native resource thunk 7E7390 forwards to vtable+34 and returns 0x18 bytes on the
empty-resource path: six arguments. The archived helper decompile incorrectly
labels/types it like a seven-argument AskHeroQuestion. Do not treat that inferred
signature or its extra decompiler argument as authoritative. Source inventories
owned by the converter were not edited.

AddLineToConversation operands, macro speech operands, live emitter/listener
correlation and measured loudness remain separate checks. The exact affected
NPC/line and a fresh external capture are still unavailable. Native script2D
descriptor/listener-transform proofs already exist; they do not establish gain
or routing on the user?s physical audio output.
