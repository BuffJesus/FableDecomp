# Oakvale Reborn — story

*Seed (user, 2026-09-19).* The birthday errands go on as in retail until a **Stranger** appears in
Oakvale. He offers the child hero great things — a sword — on one condition: **wipe out Oakvale.**

- **Accept** → the child hero does it himself: weapon in hand, villagers killable, guards hostile,
  a kill count on the HUD, the deeds counter going hard evil. The "raid" is the hero's own doing.
  Night falls on the burnt village (the retail PostAttack section); Theresa and Father confront him;
  Maze arrives. *(Gated on spike S6 — child-hero combat. Fallback: the Stranger "grows" the hero for
  the night.)*
- **Refuse** → the raid never happens. The Stranger takes another road to get the hero to the Guild
  (he takes Theresa / a different disaster — to be written). Different beats, same ending.

Both branches complete `Q_NewOakValeIntro` and hand off to `Q_GuildTraining` exactly as retail does.

## Beats — LOCKED 2026-09-19

User's calls on the draft: the Stranger is **never named**; **Father dies** on both roads; on the accept
road Father and Theresa are **protected** (the aftermath must confront the hero); **6 kills**; the **cold
open stays**. Consequence taken (flagged, reversible): on the refuse road the Stranger does himself what
the hero refused — kills Father, takes Theresa, burns Oakvale — so both roads share the retail section
swap and the dead-father stage; the choice is whether the village dies by *your* hand.

Working title **"The Stranger's Gift"**. One rule under every beat: the retail machinery keeps running
underneath (deeds, gold, Theresa's present, the Guild handoff), so each beat says what it *adds* and
what it *removes*. Voice budget: the Stranger and Theresa are ElevenLabs (George / Lily); Father, Maze
and the villagers are retail-voiced, so **new Father/Maze lines mean either a new voice for them or
subtitle-only** — every beat marks which.

### 0. Cold open — *the Stranger on the hill* (new `CS_OVR_COLDOPEN`, ~20 s)

Before the Father wakes the hero. Dawn over Oakvale from the Barrow Fields gate (`CAM_OIF_SHOT1`,
the camera retail uses for Theresa's hill). A hooded figure stands where the bandit archer will
stand in the retail raid (`MK_OIF_BANARCH`) and looks down at the village.

- STRANGER `TEXT_OVR_COLD_010`: "Every hero is born twice. The first time is nothing to do with them."
- STRANGER `TEXT_OVR_COLD_020`: "The second time, they choose."
- Fade to black → the retail Father scene, unchanged.

*Adds:* the promise that a choice is coming. *Removes:* nothing. *Risk:* the coldest possible
first impression if the voice is wrong — it is also the cheapest beat to cut.

### 1. The Father — retail, with one seed (retail `CS_OAKVALE_INTRO_FATHER` + one Lua line)

The birthday, the three gold, the present for Theresa: all retail. After the scene hands control back,
one **subtitle-only** Father line while the deed counter appears:

- FATHER `TEXT_OVR_DAD_010` (UNSPOKEN): "And if a traveller asks you for anything today, you tell him
  no. Oakvale looks after its own."

*Why:* the refusal is now the thing your father asked of you; the offer becomes a test of that.

### 2. The errands — retail deeds, with the Stranger watching (Lua, no new cutscene)

All five retail deeds stay (Bully/Victim, the teddy, the affair, the barrels, the sweets). After the
**first** deed the Stranger appears in the square (`MK_OVIT_SCARE2`), with a core quest marker.
He does not chase; he waits. If the hero walks past him he says one line, chosen by the deed done:

- after a good deed `TEXT_OVR_WATCH_GOOD_010`: "Kind. They'll remember that for a week, and forget it
  for a lifetime."
- after a bad deed `TEXT_OVR_WATCH_BAD_010`: "Good. You've already understood the first thing."
- second pass, any `TEXT_OVR_WATCH_020`: "Come and talk to me when you're done playing."

*Adds:* the errands now feel observed. *Mechanism:* `IsTalkedToByHero` opens the offer; the proximity
lines are `Scene.Say` with a cooldown. *Removes:* nothing.

### 3. The offer — `CS_OVR_OFFER` (the scene that matters, ~45 s)

Triggered by talking to him, **or** automatically when the hero has the chocolates and turns toward
Theresa's hill (so the offer can't be skipped). Letterbox; the square empties (`RemoveExtras`); he
lowers his hood — we never see a face we recognise.

- STRANGER `TEXT_OVR_OFFER_010`: "You. The birthday boy. I've watched you all morning."
- STRANGER `TEXT_OVR_OFFER_020`: "You spent it running errands for people who will die in this valley
  having done nothing. You are not like them. I can prove it."
- *He draws the sword and holds it out* (`HoldInHand OBJECT_HERO_SWORD_FIRST` + `CS_HOLD_SWORD`); it stays
  in his hand through the question and leaves it with his answer. He wears the Snowspire prophets' hooded
  robe (`CREATURE_PROPHET_01`) in every scene.
- STRANGER `TEXT_OVR_OFFER_030`: "A blade, and a name the whole of Albion will fear. All it costs is
  this village. Every soul in it, before the sun is down."
- STRANGER `TEXT_OVR_OFFER_040`: "Your father told you to say no. Didn't he."
- **Question** `TEXT_OVR_OFFER_QUESTION`: "Take the sword?"  [Yes] [No]

*Open question for you:* should the first "No" be final, or does he ask once more with a sweetener
(Theresa's safety) — a second question makes the refusal cost something too.

### 4a. Accept — *the long afternoon* (Lua `Stranger.Massacre` + `CS_OVR_TURN`)

- STRANGER `TEXT_OVR_OFFER_ACCEPT_010`: "Then let them see what you are." He is gone when the camera
  returns (`FadeOutAndKillEntity`).
- The sword is in the child's hand (S6) — or, fallback, the world blurs and the hero is grown for the
  night (`TurnCreatureInto`), which reads as *the gift working on him*.
- Info box `TEXT_OVR_MASSACRE_INFO`: "Nobody in Oakvale is safe tonight." Kill counter `TEXT_OVR_HUD_KILLS`
  "Oakvale" 0/6. Villagers are killable; they scream and flee (retail villager fear lines); the three
  guards attack on sight; Father and Theresa are **not** killable and cannot be targeted — they must
  survive for the aftermath.
- At 3 kills the Bully runs at the hero with a stick — the only villager who fights back (uses the
  retail `NOVI_Bully` hit machinery). At 6 kills: fade, `SetTime 23`, the PostAttack section swap —
  the burnt village is now *his* work — and `OverrideMusic(25)` (the retail raid music).
- `CS_OVR_TURN` (short, on the PostAttack stage): the hero alone in the square, the sword dropped
  (`Remove`), a single held camera. No lines.

*Removes:* Theresa's hill scene and the raid FMV. *Adds:* the S6 gate; morality to hard evil
(`GiveHeroMorality(-200)` plus the deed counter).

### 4c. Accept, then strike the giver — *the gift, returned* (Lua window + `Stranger.Hunted`)

Locked 2026-09-19: **the gift dies with the giver; the guards react.** After "Then let them see what you
are" the sword is in the child's hand and the Stranger stands a moment before he goes — eight seconds in
which he is the only mortal thing in Oakvale. Strike him and he goes down laughing:

- STRANGER `TEXT_OVR_KILLED_010`: "Good. That's the second thing."
- The blade is gone from the hero's hands (`RemoveAllHeroWeapons`); info box `TEXT_OVR_KILLED_INFO`: "The
  blade crumbles with him. The guards saw." The three guards turn on the child (the hero cannot die) and
  hunt him for forty seconds; no villager can be hurt. Then night — his fire was already set — the burnt
  village, Father dead, Theresa gone. Morality: a small evil hit (-50), not the massacre's.
- Aftermath `CS_OVR_AFTERMATH_KILLED`, Maze: "You killed the messenger. Whoever sent him will send another."

### 4b. Refuse — *someone always pays* (`CS_OVR_REFUSE`, replaces the raid FMV)

- STRANGER `TEXT_OVR_OFFER_REFUSE_010`: "A pity. There are other roads to the Guild." He leaves.
- The errands finish as retail: chocolates to Theresa on the hill. Then, in place of "Wait! There's
  something wrong…", Theresa's vision (her retail gift) is of the Stranger at the house:
  - THERESA `TEXT_OVR_REFUSE_010`: "He's at the house. He's with Father. Run!"
- They run; night falls (`SetTime 22`) and the sky goes orange — the Stranger has started the fire
  himself (`CreateEffect` fire on the retail bandit markers, `PlaySound`). At the square he waits with
  Theresa held by the arm; Father is on the ground between them.
  - STRANGER `TEXT_OVR_REFUSE_020`: "You said no. Someone always pays for a no."
  - STRANGER `TEXT_OVR_REFUSE_030`: "I'll keep her. You'll come looking. That's the road."
  - `FadeOut` on Theresa and the Stranger together — gone.
- Fade; the section swap to the burnt village (retail `AttackStuff`); `OverrideMusic(25)`.

*Removes:* the bandit raid, the FMV. *Keeps:* Father dead, Theresa taken, the burnt village — every
downstream script sees the retail world. *Adds:* the guilt lands on the refusal, not the raid.

### 5. Aftermath — Maze arrives (`CS_OVR_AFTERMATH_EVIL` / `_GOOD`)

Both roads: the burnt village, `OVI_DeadFather` at the house, Maze's retail arrival
(`CREATURE_RIVAL_HERO_MAZE` at `MK_OIF_HERO2`, retail voice) and his retail lines — "We must leave, it's
not safe here" … "Then give me your hand." One new **subtitle-only** Maze line before them:

- **EVIL**: Maze walks through the dead the hero made. `TEXT_OVR_AFTER_EVIL_010`: "Whoever gave you
  that blade wanted this seen. Come. The Guild will want to see it too."
- **GOOD**: `TEXT_OVR_AFTER_GOOD_010`: "The one who did this has taken your sister. I can't promise you
  her. I can promise you a sword."
- **KILLED** (4c): `TEXT_OVR_AFTER_KILLED_010`: "You killed the messenger. Whoever sent him will send another."
- All → the retail fade → `Q_GuildTraining`, adult transition unchanged.

### 6. Titles — what the villagers call you afterwards (locked 2026-09-19)

Two hero titles, **appended** to game.bin (not replacing retail ones): **Butcher of Oakvale** for the
massacre road and **Giftbreaker** for the gift returned; the refuse road earns nothing — the retail
titles are the reward for saying no. A title is an inventory item (OBJECT + CInventoryItemDef /
CStockItemDef / CHeroTitleDef sub-defs); villagers greet/comment with one fully voiced line per voice
type (AF1-3, AM1-3, CF1, CM1, INKEEP1, LEV1-3BDT1) in three categories, exactly as Arseface. Not
buyable, not shown in the title shop; granted at night by `GiveHeroTitle`. 36 lines each in
`manifest/intro.yaml` `titles:` (72 voiced lines, ~2,400 characters -- next month's ElevenLabs quota;
until then they ship with silent placeholder clips and subtitles).

### Mechanics this draft needs beyond v1 (so you can weigh the beats by cost)

| beat | needs | cost |
|---|---|---|
| 0 cold open | one new def, retail markers/camera, 2 VO lines | small |
| 1 Father seed | a subtitle-only line | trivial |
| 2 watcher | proximity `Say` with cooldown in `stranger.lua` | small |
| 3 offer | `CS_OVR_OFFER.cs`, 4 VO lines, maybe a sword prop marker (S2) | medium |
| 4a massacre | S6 verdict; Bully rush; Father/Theresa untargetable (`SetPlayerCreatureOnlyTarget`?) | medium, gated |
| 4b refuse | `CS_OVR_REFUSE.cs` rewrite on the retail Theresa-scene staging; 3 VO lines; fire effects on retail markers; the retail section swap stays | medium |
| 5 aftermath | HESDEADJIM clones with one inserted subtitle-only Maze line each | small |

## Edge cases handled (2026-09-19 review)

- **Theresa before the offer** — her departure now also requires `StrangerOfferMade`; and the offer is
  *forced* (hero free, no cutscene/conversation) once she has her chocolates, so nobody reaches the
  night without having been asked.
- **A body that can't be talked to** (the prophet may have no conversation brain) — the offer also opens
  by standing within 2.5 m for 2 s, by passing within 4 m with the sweets, or by the forced trigger.
- **Comments mid-scene** — the watcher only speaks when the hero is player-controlled, not in a movie,
  not in a conversation.
- **Reload after the answer** — `Routine` resumes the massacre (re-arms) or the hunt from the persisted
  flags; `MassacreKills` persists; an already-present `OVR_Stranger` is reused instead of duplicated.
- **Villagers that won't be caught** — after 180 s the night comes anyway ("his fire does the rest").
- **The sword into the Guild** — `SWORD_SURVIVES_NIGHT = false`: the gift goes with the night on every
  road, so `Q_GuildTraining` meets the retail hero. Flip it if you want him to keep it.
- **The square's extras** — `CS_OVR_OFFER` now returns them (`RemoveExtras FALSE,RETURN`, as retail
  pairs every `TRUE`).
- **The question with the world running** — `Scene.Ask` sits inside its own paused movie like retail
  entity questions.
- **The Bully rush** — at 3 kills `NOVI_Bully`, if still alive and present, gets `SetAttackHeroOnSight`.
- **The spawn point** — `MK_OVIT_SCARE2` is an unlocated Theresa-scene marker; he now appears 2.5 m from
  `NOVI_BookTrader`, who is in the square by definition.

Not handled (by choice): children (Bully, Victim,
TeddyGirl) may not be killable at all — the 6 come from the 15 adults; hitting Father/Theresa with the
sword triggers their retail "don't hit me" responses, which reads as intended.

## What the scaffold assumes today (2026-09-19, rewrite freely)

The v1 build (`FSE/OakvaleReborn/stranger.lua`, `scenes.lua`, `OVR_Theresa.lua`) plays the seed with
placeholders so every mechanism is exercised before the beats exist:

- The Stranger (`CREATURE_TRADER_01` at `MK_OVIT_SCARE2`) appears after the first deed or the first
  3 gold, with a core quest marker, and waits to be spoken to.
- The offer is a Lua beat (camera on him, `TEXT_OVR_OFFER_010/020`, the yes/no `TEXT_OVR_OFFER_QUESTION`).
- Accept: `OBJECT_HERO_SWORD_FIRST`, every villager killable, guards hostile, kill counter, six dead →
  night → the retail burnt-village section → `CS_OVR_AFTERMATH_EVIL`.
- Refuse: errands continue; Theresa's departure trigger runs `CS_OVR_REFUSE` (Theresa line, night falls,
  no raid, no FMV) → burnt-village section → `CS_OVR_AFTERMATH_GOOD`.  *The burnt village on the refuse
  road is a placeholder: the PostAttack section is the only night stage we have without new TNG sections.*
- Both aftermaths are clones of the retail `CS_OAKVALEINTRO_HESDEADJIM` until beat 5 is written.

Writing a beat = new `TEXT_OVR_*` lines in `manifest/intro.yaml`, a `CS_OVR_*.cs` (or a Lua beat in
`stranger.lua`), then `build_custom_intro.py all` — `cs_lint.py` catches unknown keys/markers/verbs.
