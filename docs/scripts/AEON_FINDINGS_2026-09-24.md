# Notes for Aeon — 2026-09-24

Since the 2026-09-22 share. The first section is runtime (FSE-level) bugs your FSE very likely shares; the
patches that fix them in our sidecar are in `sidecar_patches/` (plain `git diff`s of our ForgeFSE fork).

---

## 1. FSE runtime bugs we hit in-game (probably in yours too)

| # | symptom | cause | patch |
|---|---|---|---|
| 1 | An entity script starts a thread on its **parent quest** with an argument (`Quest:CreateThread("WatchForPickpocketing", {args = {Me}})`); the thread gets an un-indexable userdata | Entity scripts run in their **own** `sol::state`. The `sol::object` args are registry refs into the entity's state; the quest host resumes the thread in its state, where that index names something else | `novi-zzzzzzz-thread-args-cross-state.patch`: rebuild each arg in the quest's state (primitives by value, a thing as an owning CScriptThing copy — retail passes it by value, and `Me` is a non-owning handle the entity host can outlive) |
| 2 | `SetQuestAsFailed(..., true)` shows a blank Quest Failed screen with a dead Reload button (Orchard Evil) | FSE passed `CWideString{nullptr}` and hard-coded `useWideMessage = true` | `novi-zzzz-quest-failed-message.patch`: construct a real retail CWideString (ctor 0x99B6B0 / dtor 0x99B510) from the message and pass the caller's flag |
| 3 | `thing:IsEqualTo(other)` crashes | retail `CGameScriptThing::IsEqualTo` (0x8D46A0) compares `+0xC` of both **implementation** objects; FSE handed it the CScriptThing wrapper, read past its end | `novi-zzzzz-isequalto-implementation.patch`: pass `other.pImp.Data` |
| 4 | `CreateEffectAtPos` / `CreateEffectOnThing` always return nil | the Lua lambdas called the method and dropped its result | `novi-zzzzzz-create-effect-result.patch` |

Your crash (GetPos in `Init` before entities initialise) is the same family as #1: things are only
safe to touch once the host that owns them is running.

## 2. SCRIPT_DEF (CScriptDef) offsets: the boast block

If you read boast wagers/rewards through global game data: the leading CScriptDef block (up to +0x254,
every quest boast) sits at **PDB offset − 4**, not − 64. Verified live on the boast podium: Orchard Evil
(five boasts) and Trader Escort (No Protection 200/400 and Protect Traders 100/400 read on screen; Without
A Scratch is 200/1000 by the table, not yet looked at) show exactly retail's numbers. The 64-byte model holds from the 0xd64 anchor on; the middle
(0x258..0xd60) is unproven, so our readables keep those offsets numeric.

## 3. What has been played in-game since the last zip

* **Orchard Farm Good**: completed end to end (handoff, Whisper, completion), 0 Lua errors.
* **Orchard Farm Evil**: played to its failure path (the fail screen is what surfaced bug #2); success
  not yet seen. Boasts: podium, `AddBoast`, `IsBoastTaken`, Quest Start lines and the "Boast Failed"
  notice all verified; a *won* boast's payout line is still unseen.
* **Trader Escort** (new unit in this zip): card -> Take Quest and Boast -> Darkwood intro cutscene ->
  Quest Start -> follow tutorial -> Darkwood1 -> 2 (flesh-eating balverine scene + fight) -> 3 -> 4, all
  three traders following, 0 Lua errors. It stopped in Darkwood4 at the camp-trader greeting (a
  converter bug, fixed in this zip: every trader took the greeting branch instead of only `TraderToTalk`).
  Rock troll onward is untested.

## 4. Converter-side fixes you might see if you diff against your ports

* Surprise balverines were created with nil def/position (an exporter stack-depth bug around a
  zero-argument vtable call on a vector element) — now `CreateCreature("CREATURE_BALVERINE_EASY",
  markers[i]:GetPos(), "SurpriseBalverine")`, matching retail.
* A cutscene started twice (`StartMovie` + a stray `StartMovieSequence`) parks the thread forever.
* VC7.1's temporary-destruction flag word can be relayed through a register mid-function; lifted naively
  it becomes bit tests on a nil local.
