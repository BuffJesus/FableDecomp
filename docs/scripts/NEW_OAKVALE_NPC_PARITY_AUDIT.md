# New Oakvale NPC parity audit

Status: static retail baseline complete; runtime control-ownership verification remains open.

This audit separates authored world state from quest-script behavior. It is intentionally based on
the installed retail TNG records, the decoded retail `script.bin` cutscenes, and native entity
functions in `refs/script_recovery/native_clusters/Q_NewOakValeIntro.json`. Visual memory is not used
as a source for positions, schedules, or timing.

## Reproducible placement baseline

Run:

```powershell
python tools/script_recovery/audit_new_oakvale_npc_baseline.py `
  --output refs/script_recovery/new_oakvale_intro/retail_tng_npc_baseline.json
```

The current retail inputs and their SHA-256 hashes are recorded in that JSON. The extractor finds 33
`NOVI_*`/`OVI_DeadFather` things: nine scripted barrels, eight generic villagers, three guards, one
each of the ten named living characters, the dead father, and `NOVI_BlockingGate`. For every record it
copies the local position, authored world-space initial position, facing vector, definition, health,
brain override, information/AI flags, follow permission, targetability, forced attitude/enemy and
hit-tolerance state, faction/friend flags, courtship flags, and persistence flags.

The two coordinate forms are not interchangeable. `Position*` is local to the TNG region; the
`InitialPos*` values already contain the region offset. Lua must continue to use the engine thing and
its `GetHomePos()` result rather than embedding either set as guessed coordinates.

## Quest registration cross-check

Native `CQ_NewOakValeIntroScript::Main` at `0x00DABAC0` registers exactly the 16 entity script classes
listed in the reconstructed `quests.lua`. This relationship is deliberate:

- `NOVI_BlockingGate` exists in TNG but is not one of the native quest's 16 entity bindings. Adding a
  Lua host for it would diverge from retail.
- `NOVI_CreatedBeetle` is one of the 16 bindings but has no initial TNG instance because barrels create
  beetles dynamically.
- Repeated TNG script names are expected. One binding applies `NOVI_Guard` to all three guards,
  `NOVI_Villager` to all eight villagers, and `NOVI_Barrel` to all nine barrels.

## Behavior/ownership matrix

| Script | Authored starting behavior | Native quest changes | Lua parity result |
|---|---|---|---|
| `NOVI_Villager` (8) | Oakvale unemployed definition; authored position/facing; no brain override | No blanket movement or brain change. Script takes priority-4 control only for hit/talk reactions; intermittent speech takes no control | Exact by static audit; do not add a schedule or teleport |
| `NOVI_Guard` (3) | BS guard definition at three distinct posts | Init sets wander centre to each guard's own home, range 0..6, scripting group 4, weapons sheathed. Priority-4 control is event-scoped to chase/lecture or hit response | Exact by static audit; a shared fixed position would be wrong |
| `NOVI_LiveFather` | Authored beside the house | Retail intro cutscene teleports Father and Hero to `MK_OVI_ID_DAD`/`MK_OVI_ID_HERO`; later interaction movement remains native AI unless explicitly controlled | Lua invokes the shipped cutscene and does not synthesize coordinates |
| `NOVI_Theresa` | Authored away from the starting steps | Retail cutscenes own placement/actions; after chocolates she removes all living `NOVI_Guard` instances before the raid | Lua mirrors the native all-instance guard cleanup |
| `NOVI_Bully` / `NOVI_Victim` | Authored face-to-face in East Oakvale | Both use sustained scripted control for their encounter. `BULLYRUN1` owns the bully's run to `MK_OIBR_BULLY2` and visibility change | Static port matches; visible run remains a Forge cutscene actor/control seam |
| `NOVI_TeddyGirl` | Authored near the house | Stays authored until interaction; ruined-teddy branch runs to the affair wife and removes her only after she is off-screen | Exact by static audit |
| Affair man/woman/wife | Authored man and woman together; wife elsewhere | Man/woman retain scripted control for their interaction loop. Wife retains her ordinary state until discovery, then runs to the husband's live position and argues | Exact by static audit; no invented patrols |
| `NOVI_BookTrader` | Trader definition at authored stall/home | Script retains control, returns to engine home when over 2 m away, and faces Theresa | Exact `GetHomePos()` path; no coordinates embedded |
| `NOVI_BarrelMan` | Warehouse-worker definition at authored home | Sets `BRAIN_PASSIVE_OVERRIDE`, 0..1 home wander, then marker/hero-relative movement; restores `BRAIN_GOOD_VILLAGER_BASE` after judgment | Exact by static audit; return crash is below this policy layer |
| `NOVI_BarrelThug` | Authored in East Oakvale | Teleports to `MK_NOVI_ID_BARRELBOY_START` only when Barrel Man leaves the hero in charge | Exact by static audit |
| `OVI_DeadFather` | Separate authored dead-father thing | Post-raid script teleports him to `MK_OVI_ID_DAD_DEAD`, plays the recovered pose, then removes his marker when found | Exact by static audit |

## Cleanup and restoration

Retail does not define a blanket “restore every NPC” operation. Cleanup is branch-specific:

- scripted-control resources are released at the recovered native boundaries;
- generic villagers and guards otherwise retain their engine AI;
- the affair woman, Teddy Girl's ruined branch, Bully, created beetles, raid guards, and dead-father
  marker are removed only by their explicit native branches;
- Barrel Man alone has an explicit quest brain transition from passive to normal villager;
- entity-local constructor fields are not persisted by retail, while the quest transfers only its
  recovered parent fields.

Adding generic reset teleports, new schedules, or entity persistence would therefore hide runtime
ownership bugs and make the port less accurate.

## Remaining evidence gate

The static audit finds no evidence-backed Lua correction to initial placement or ambient schedules.
The remaining observed village differences must be attributed more narrowly with runtime evidence:

1. record the bound thing UID/script name, home/current position, and active control resource at host
   start;
2. record every control acquire/release transition and native cutscene actor mapping;
3. compare the same NPC at the same quest-state boundary in direct retail and Forge runs;
4. correct only the first diverging state transition.

This is especially important for the Bully/Victim pair and the continuously controlled affair/trader
actors. Their lack of ordinary wandering is native script behavior; a Forge resource that fails to
release or a cutscene actor that fails to bind is not.
