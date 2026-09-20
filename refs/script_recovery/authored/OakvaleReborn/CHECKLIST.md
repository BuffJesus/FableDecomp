# Oakvale Reborn — in-game checklist

Every run: **New Game** (saves cache entities), disposable profile, then
`python tools/oakvale_reborn/grade_run.py --bundle <bundle>` (grades every log-decidable row below and lists
the eyes-only ones); the raw log is `<bundle>/NoviCompatibility/FableScriptExtender.log`. Listen to the script
first: `python tools/oakvale_reborn/table_read.py` → `work/oakvale_reborn/table_read.wav`. The install currently carries a
FableForge world stage (text.big, wad, wld, qst, stb); our overlay is a *layer* over it
(`<file>.ovrbak` backups + `work/oakvale_reborn/install_receipt.json`), never `forge unstage`.

## Spike S1 + S3 (bundle-spike-s1) — appended cutscene def + Lua camera beat

Build + install (game closed):

    python tools/oakvale_reborn/spike_s1.py
    python tools/oakvale_reborn/build_custom_intro.py all --lua work/oakvale_reborn/spike_s1/lua --tag spike-s1
    python tools/oakvale_reborn/build_custom_intro.py install
    python work/oakvale_reborn/bundle-spike-s1/local_test.py --game-dir "C:\Programs\Steam\steamapps\common\Fable The Lost Chapters" --launch --save-dir <saves>

Afterwards: `python tools/oakvale_reborn/build_custom_intro.py restore`.

| # | Look for | Pass |
|---|---|---|
| S1-a | log `OVR_SPIKE_S1: running appended macro CS_OVR_SPIKE` then `OVR_SPIKE_S1: CS_OVR_SPIKE returned` | both lines, no Lua error between |
| S1-b | on screen before the retail Father scene: fade, cut to the stand-up camera, Father says his first line, cut to `CAM_OVIF_SHOT2`, fade | the appended def *executed* (not silently skipped: the two log lines with nothing visible in between = engine did not resolve the name) |
| S1-c | retail `CS_OAKVALE_INTRO_FATHER` still plays fully after it | yes |
| S3 | log `OVR_SPIKE_S3: done ok=true err=nil`; letterbox appears, camera swings to a point beside the hero, then to `CAM_OVIF_SHOT2`, then back behind the hero | note which moves actually happened |
| gate | `grep -E "NOVI_AUTHORITY|Lua error|!!! "` — the authority line, zero errors | |

Fail branch for S1-b: the engine found no def → rewrite a dead retail def in place
(`forge script cutscene-set <root> CS_OAKVALEINTRO_BULLYRUNDUMMY <cs>`), which changes no names/counts.

## Spike S6 (bundle-spike-s6) — can the child hero fight? (Lua only, no overlay needed)

    python tools/oakvale_reborn/spike_s6.py            # or --grown for the TurnCreatureInto fallback
    python tools/oakvale_reborn/build_custom_intro.py bundle --lua work/oakvale_reborn/spike_s6/lua --tag spike-s6
    python work/oakvale_reborn/bundle-spike-s6/local_test.py --game-dir "<root>" --launch --save-dir <saves>

Play the Father intro, do one deed (or collect 3 gold), then attack a villager and walk up to a guard.

| # | Look for | Pass |
|---|---|---|
| S6-a | log `OVR_SPIKE_S6: armed, N killable creatures` | bindings did not throw; N > 0 |
| S6-b | the child draws the sword and swings (no T-pose, no crash, weapon visible in hand) | animations play on `CREATURE_HERO_CHILD` |
| S6-c | log `OVR_SPIKE_S6: kill 1 <def>` after a villager falls | villagers are mortal |
| S6-d | a guard turns hostile and fights the child; the hero cannot die (`SetOakvaleHeroKillable(false)` is retail) | guard fights back, no soft-lock |
| verdict | S6-b passes → `Stranger.GROWN_FOR_THE_NIGHT = false` stays; fails → set it `true` and rerun with `--grown` | |

## v1 (bundle-v1) — the locked beats: cold open, the watcher, the offer, both roads, Maze

    python tools/oakvale_reborn/build_custom_intro.py all --tag v1 --placeholder-vo   # pristine text defs cutscenes overlay bundle check (drop --placeholder-vo once the title VO exists)
    python tools/oakvale_reborn/build_custom_intro.py install
    python work/oakvale_reborn/bundle-v1/local_test.py --game-dir "<root>" --launch --save-dir <saves>
    ... afterwards: python tools/oakvale_reborn/build_custom_intro.py restore

Overlay = `script.bin` (+7 `CS_OVR_*` defs, 602/602 round-trip), `game.bin` (+2 titles = 8 entries), `names.bin`,
`text.big` (+99 `TEXT_OVR_*`: 12 subtitle-only, 87 voiced incl. 72 title lines, 6 title groups), `Dialogue.lut` (+72 clips, silent
placeholders until the VO pass), `ScriptDialogue2.lut` (+15 ElevenLabs clips: George = Stranger, Lily = Theresa),
`scriptdialoguesnds2.bin`, `dialogue.big` (+14 lipsync curves).

| # | Look for | Pass |
|---|---|---|
| v1-0 | **cold open** before the Father: dawn, the hooded figure on the raid's cliff, two lines, fade; then the retail dream FMV + Father scene at noon (`SetTime 6` must not leak: clock reads 12 after) | log `OVR scene: macro CS_OVR_COLDOPEN done ok=true`; framing acceptable (else S2: new `CAM_OVR_*`) |
| v1-1 | after the Father scene and the highlighting box, the seed box: *"...you tell him no. Oakvale looks after its own."* | shown, dismisses |
| v1-2 | after the first deed / 3 gold: log `OVR stranger: created …`; core marker on him; walk past within ~5 m: one comment matching the deed (good/bad), later passes *"Come and talk to me…"*, never more often than 12 s | audio + subtitle + mouth on a **created** creature (S4 through `resources:Speak`) |
| v1-3 | talk to him (or walk past with the chocolates): `CS_OVR_OFFER` — square emptied, four lines on the Father-intro cameras, **the sword appears in his hand at the terms** and is gone after his answer, held wide shot; then **Take the sword?**; then his answer line under a Lua camera | `OVR scene: macro CS_OVR_OFFER done ok=true`, `lua offer-answer done ok=true` |
| v1-4 REFUSE | he fades out; errands finish; chocolates to Theresa; at the departure trigger `CS_OVR_REFUSE`: vision line (Lily), run to the fence, night, fire columns + a scream below, the square: Father dead, the Stranger with Theresa, two lines, both fade; **no FMV**; burnt village; `CS_OVR_AFTERMATH_GOOD` = retail dead-father scene with the new Maze line *"…I can promise you a sword."* (subtitle-only `InteractiveSpeak` — note whether it needs a click) → Guild | full run |
| v1-5 ACCEPT | `OVR massacre: begin`; sword in the child's hand; info box; counter 0/6; villagers die, guards fight, **Father and Theresa cannot be killed**; 6 → `night falls`; burnt village; `CS_OVR_AFTERMATH_EVIL` with *"Whoever gave you that blade wanted this seen."* → Guild | full run |
| v1-7 KILL | accept, then hit him with the sword within ~8 s: `OVR stranger: STRUCK by the gift`, his laughing line, he drops dead (`SetThingAsKilled`); the sword is gone from the hero's hands; info box; the three guards attack the child (hero cannot die) for ~40 s; `OVR hunted: night falls`; burnt village; `CS_OVR_AFTERMATH_KILLED` with *"You killed the messenger…"* → Guild. Also: **can a child's swing register as a weapon hit on him at all** (S6 again) | full run |
| v1-10 TITLE | after the massacre night: `OVR title: OBJECT_HERO_TITLE_BUTCHER_OF_OAKVALE`; the hero's title reads *Butcher of Oakvale* in the stats/title screen; in the Guild (and any town later) villagers greet with the Butcher lines (subtitle; audio is a silent placeholder until the VO pass); the title shop does NOT list it. Same for *Giftbreaker* on the killed road. Retail titles still buy/greet normally (the appended defs did not shift anything) | title shown + spoken |
| v1-6 | save + reload before the offer and after each answer: flags survive; the cold open does not replay after a mid-errand reload; a reload mid-massacre re-arms and resumes the count; a reload after the spawn does not create a second Stranger | yes |
| v1-8 | order attacks: give Theresa the chocolates **before** talking to him → the offer opens by itself on the hill (teleports both to the house); refuse there, walk back to Theresa → `CS_OVR_REFUSE`. Also: stand next to him 2 s without talking → offer opens | no way to reach night unasked |
| v1-9 | accept, then ignore the villagers for 3 min → `time is up, his fire does the rest`, night anyway; at night the sword is gone (`SWORD_SURVIVES_NIGHT=false`) | no stall |
| gate | `grep -E "NOVI_AUTHORITY|Lua error|!!! "` — the authority line, zero errors; every `OVR scene:` open has a matching `done ok=true` | |

Known placeholders: the Stranger wears the Snowspire prophet's hooded robe (`CREATURE_PROPHET_01`; if it has no
idle/walk or looks wrong, try `CREATURE_ASSASSIN`); all cameras/markers are retail (cold open and offer framing
unverified); in the offer he holds `OBJECT_HERO_SWORD_FIRST` (`HoldInHand` + `CS_HOLD_SWORD`) until the answer.
