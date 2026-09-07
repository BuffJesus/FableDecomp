# Fable: The Lost Chapters — Reported and Unfixed Bugs

> Working backlog for **FableDecomp**.
> This document separates confirmed defects, community reports, exploits, and behaviours that still require verification.

## Status Legend

| Label | Meaning |
|---|---|
| `Confirmed` | Documented and reproducible in Fable TLC |
| `Reported` | Reported by players or community sources; reproduction still required |
| `Investigate` | Suspected defect, engine quirk, or undesirable original behaviour |
| `Exploit` | Repeatable logic flaw that benefits the player |
| `Design Ambiguity` | Possibly intentional, but undesirable or inconsistent |

---

## Camera, Movement, and Input

- [ ] **Camera wobble during diagonal movement** — `Confirmed`
  - The camera rocks laterally while the Hero moves forward and turns.
  - Appears tied to player lean values such as:
    - `LeanAngleTurnFactor`
    - `LeanAngleLimitAngle`
    - `LeanAngleStepPerFrame`

- [ ] **Frame-dependent mouse camera response** — `Reported`
  - Mouse movement may feel different depending on frame rate.

- [ ] **Mouse camera acceleration or angular-speed cap** — `Reported`
  - Fast mouse movement does not always produce proportional camera rotation.

- [ ] **Camera micro-stutter at high mouse polling rates** — `Reported`

- [ ] **Player lean affects the camera too strongly** — `Design Ambiguity`

- [ ] **Camera collision snaps near walls** — `Investigate`

- [ ] **Camera retains an undesirable pitch after targeting** — `Investigate`

- [ ] **Target selection chooses an unintended enemy** — `Reported`

- [ ] **Target lock becomes unstable around multiple enemies** — `Reported`

- [ ] **Hero movement changes direction abruptly when lock-on ends** — `Investigate`

- [ ] **Movement behaviour changes at high frame rates** — `Investigate`

- [ ] **Rolling can intersect doors, props, or uneven geometry** — `Reported`

---

## Saving and Profile State

- [ ] **Hero Save duplicates quest-obtainable items** — `Exploit`

- [ ] **Hero Save can produce invalid quest state** — `Reported`

- [ ] **World Save remains unavailable after a quest ends** — `Reported`

- [ ] **Guild Seal is not restored after temporary quest removal** — `Reported`

- [ ] **New profiles inherit dialogue state from another profile** — `Reported`

- [ ] **Profile state persists until the executable is restarted** — `Investigate`

- [ ] **Save reports success despite incomplete or unusable data** — `Reported`

- [ ] **Save load returns the Hero to an unexpectedly early checkpoint** — `Design Ambiguity`

- [ ] **Autosave or transition save can crash or fail** — `Reported`

- [ ] **Quest-restricted time state remains active after reload** — `Investigate`

---

## Quests and Progression

- [ ] **White Balverine quest fails to restore Knothole Glade residents** — `Reported`
  - Villagers or shopkeepers may remain absent after the quest.

- [ ] **Quest marker points toward the wrong region after teleporting or loading** — `Reported`

- [ ] **Quest marker remains after objective completion** — `Reported`

- [ ] **Objective trigger fails when entered from an unusual direction** — `Reported`

- [ ] **Escort NPC stops following the Hero** — `Reported`

- [ ] **Escort NPC becomes stuck behind a door or region transition** — `Reported`

- [ ] **Escort NPC attacks unreachable enemies indefinitely** — `Reported`

- [ ] **Quest NPC dies or disappears without triggering quest failure** — `Investigate`

- [ ] **Cutscene fails to begin because an NPC is out of position** — `Reported`

- [ ] **Cutscene ends without restoring player control** — `Reported`

- [ ] **Quest barrier remains after completion or abandonment** — `Reported`

- [ ] **Quest enemy spawns outside the playable area** — `Reported`

- [ ] **Arena Hero Save corrupts money or reward state** — `Reported`

- [ ] **Arena round cannot complete after an enemy becomes unreachable** — `Reported`

- [ ] **Door or Cullis Gate interaction interrupts quest-state restoration** — `Investigate`

- [ ] **Soul-collection objective marker becomes confused after teleporting or loading** — `Reported`

---

## NPCs, Navigation, and World Simulation

- [ ] **NPCs become trapped by opening doors** — `Reported`

- [ ] **Hero becomes trapped between an NPC and a door** — `Reported`

- [ ] **Force Push moves enemies outside valid navigation** — `Exploit`

- [ ] **Enemies remain alive but unreachable inside scenery** — `Reported`

- [ ] **Enemies fall through terrain or region boundaries** — `Reported`

- [ ] **NPC pathfinding repeatedly walks into doors or corners** — `Reported`

- [ ] **Followers fail to use region exits** — `Reported`

- [ ] **NPC fails to resume its schedule after combat** — `Reported`

- [ ] **Shopkeeper remains frightened or hostile indefinitely** — `Reported`

- [ ] **Shopkeeper fails to reopen after the town returns to normal** — `Reported`

- [ ] **NPC conversation becomes permanently unavailable** — `Reported`

- [ ] **NPC dialogue repeats too frequently** — `Investigate`

- [ ] **NPC speaks dialogue for an event that has not happened** — `Reported`

- [ ] **NPC reaction conflicts with current morality or renown** — `Investigate`

- [ ] **NPC or creature floats above sloped ground** — `Reported`

- [ ] **NPC movement animation continues while blocked** — `Reported`

- [ ] **Chickens can be kicked onto roofs or outside town limits** — `Exploit`

- [ ] **Child appears unexpectedly after divorce** — `Reported`

- [ ] **Time remains frozen after a quest or scripted sequence** — `Reported`

- [ ] **Day and night schedules stop updating after time is frozen** — `Investigate`

---

## Combat

- [ ] **Roll grants complete damage immunity for too much of the animation** — `Design Ambiguity`

- [ ] **Enflame grants extended invulnerability** — `Design Ambiguity`

- [ ] **Physical Shield prevents combat multiplier loss** — `Exploit`

- [ ] **Force Push repeatedly scores hits on dead bodies** — `Exploit`

- [ ] **Dead bodies award additional experience or combat multiplier** — `Exploit`

- [ ] **Time Control causes enemies to lose awareness incorrectly** — `Exploit`

- [ ] **Area damage harms civilians without reliably registering a crime** — `Exploit`

- [ ] **Splash damage bypasses friendly-fire or ownership checks** — `Investigate`

- [ ] **Attacks connect through walls or closed doors** — `Reported`

- [ ] **Projectiles pass through close targets** — `Reported`

- [ ] **Projectiles collide with invisible or outdated collision geometry** — `Reported`

- [ ] **Enemy attacks continue tracking after animation commitment** — `Investigate`

- [ ] **Enemy attack damages the Hero despite visibly missing** — `Reported`

- [ ] **Blocking direction disagrees with incoming attack direction** — `Investigate`

- [ ] **Knockdown leaves an actor temporarily untargetable** — `Reported`

- [ ] **Enemy remains invulnerable after a spawn or scripted animation** — `Reported`

- [ ] **Enemy health reaches zero without triggering death** — `Reported`

- [ ] **Execution or flourish targets the wrong actor** — `Reported`

- [ ] **Flourish input remains buffered too long** — `Investigate`

- [ ] **Spell charging becomes stuck after interruption** — `Reported`

- [ ] **Spell effect remains active after the caster dies** — `Reported`

- [ ] **Summoned creature AI becomes inactive or incorrectly hostile** — `Reported`

- [ ] **Multi-hit spell damage changes with frame rate** — `Investigate`

---

## Economy, Shops, and Inventory

- [ ] **Bulk merchant pricing uses one price for the entire transaction** — `Exploit`

- [ ] **Merchant supply and demand are not recalculated per item** — `Exploit`

- [ ] **Buyback or resale price exceeds purchase cost under repeatable conditions** — `Exploit`

- [ ] **Hero Save duplicates merchant or quest inventory** — `Exploit`

- [ ] **Inventory quantity becomes incorrect after an interrupted transaction** — `Investigate`

- [ ] **Item remains equipped after being removed from inventory** — `Reported`

- [ ] **Weapon or clothing bonuses remain after unequipping** — `Reported`

- [ ] **Augmentation effects fail to update immediately** — `Reported`

- [ ] **Frying Pan hidden augmentation slot becomes unavailable** — `Reported`

- [ ] **Quest item remains after its quest is completed** — `Reported`

- [ ] **Required quest item can be sold or discarded** — `Investigate`

- [ ] **Shop menu selects the wrong item after sorting or changing categories** — `Reported`

- [ ] **Inventory list cannot scroll back to its first entry** — `Reported`

- [ ] **Item description or statistic does not match its actual effect** — `Reported`

- [ ] **Stack count overflows or displays an invalid value** — `Investigate`

- [ ] **Gold value overflows or becomes extreme after save manipulation** — `Reported`

---

## UI and Menus

- [ ] **Mouse hitboxes do not align with menu graphics** — `Reported`

- [ ] **Hovered menu entry differs from the entry that activates** — `Reported`

- [ ] **List selection skips the first or last item** — `Reported`

- [ ] **Scroll position persists incorrectly between menu categories** — `Investigate`

- [ ] **Tooltip remains visible after closing a menu** — `Reported`

- [ ] **HUD element remains hidden after a cutscene** — `Reported`

- [ ] **Target indicator remains attached to a dead or unloaded actor** — `Reported`

- [ ] **Quest text is clipped or extends outside its panel** — `Reported`

- [ ] **Incorrect button prompt is shown for the active input device** — `Investigate`

- [ ] **Menu input repeats too rapidly at high frame rates** — `Investigate`

- [ ] **Resolution change breaks UI scaling or mouse mapping** — `Reported`

- [ ] **Alt-Tab breaks cursor confinement** — `Reported`

- [ ] **Cursor remains visible during gameplay** — `Reported`

- [ ] **Brightness or gamma changes do not apply consistently** — `Reported`

---

## Graphics, Animation, and Audio

- [ ] **Character animation foot sliding** — `Investigate`

- [ ] **Movement animation speed does not match actual movement speed** — `Investigate`

- [ ] **Weapon uses an incorrect transform during animation** — `Reported`

- [ ] **Clothing or hair clips heavily during common animations** — `Confirmed`

- [ ] **Facial animation fails to return to neutral** — `Reported`

- [ ] **Actor remains in combat stance after combat ends** — `Reported`

- [ ] **Particle effect remains after its actor or spell unloads** — `Reported`

- [ ] **Light or effect remains active after its source is destroyed** — `Reported`

- [ ] **Region lighting changes incorrectly after loading** — `Reported`

- [ ] **Shadow or character rendering flickers at modern resolutions** — `Reported`

- [ ] **Water or transparency renders incorrectly on modern hardware** — `Reported`

- [ ] **Audio line plays twice or overlaps itself** — `Reported`

- [ ] **Ambient audio from the previous region continues after transition** — `Reported`

- [ ] **Music fails to return after combat or a cutscene** — `Reported`

- [ ] **Music layers stack or play simultaneously** — `Reported`

- [ ] **Sound effect continues indefinitely after interruption** — `Reported`

- [ ] **Subtitle does not match the spoken dialogue** — `Confirmed`

- [ ] **Dialogue is inaudible because its emitter is misplaced** — `Reported`

---

## Stability and Modern-PC Compatibility

- [ ] **Crash during region transition** — `Reported`

- [ ] **Crash while saving or immediately after saving** — `Reported`

- [ ] **Crash when loading particular profiles** — `Reported`

- [ ] **Crash after repeated profile switching** — `Investigate`

- [ ] **Crash or rendering failure after Alt-Tab** — `Reported`

- [ ] **Incorrect behaviour at very high frame rates** — `Investigate`

- [ ] **Timing-dependent scripts complete too quickly or too slowly** — `Investigate`

- [ ] **Physics or animation behaviour changes with frame rate** — `Investigate`

- [ ] **Game fails to restore display mode after a crash** — `Reported`

- [ ] **Input focus is lost after returning from another window** — `Reported`

- [ ] **Configuration changes are not retained reliably** — `Reported`

---

## Recommended Initial Investigation Milestone

These issues are good candidates for the first focused FableDecomp bug milestone:

1. Camera wobble during diagonal movement
2. Frame-dependent mouse input
3. Mouse angular-speed cap
4. Camera collision snapping
5. Cross-profile dialogue leakage
6. Guild Seal restoration failure
7. World Save remaining disabled
8. Hero Save quest-state corruption
9. White Balverine villagers not returning
10. Arena save or gold corruption
11. Quest enemies becoming unreachable
12. Escort NPC navigation failure
13. Merchant bulk-pricing exploit
14. Incorrect menu mouse hitboxes
15. Frame-rate-dependent scripts and animations

---

## Reproduction Template

```markdown
## Summary

Brief description of the bug.

## Classification

- Status: Confirmed / Reported / Investigate / Exploit / Design Ambiguity
- System:
- Severity:
- Reproduction rate:

## Environment

- Game: Fable: The Lost Chapters
- Platform: PC / Original Xbox
- Executable version:
- Region:
- Active quest:
- Save file:
- Resolution:
- Frame rate:
- V-Sync:
- Input device:

## Reproduction Steps

1.
2.
3.

## Expected Result

Describe what should happen.

## Actual Result

Describe what happens instead.

## Additional Evidence

- Screenshots:
- Video:
- Save file:
- Logs:
- Community reports:
- Original Xbox behaviour:
- Fable Anniversary behaviour:

## Suspected Code

- Module:
- Function:
- Address:
- Related symbols:
```

---

## Verification Rules

Before marking an issue as `Confirmed`:

1. Reproduce it on an unmodified PC installation of Fable TLC.
2. Record the executable version or checksum.
3. Test with a clean profile where practical.
4. Repeat the test at least three times.
5. Check whether frame rate affects the result.
6. Check whether the issue also occurs on the original Xbox release.
7. Separate intended Hero Save behaviour from actual save corruption.
8. Attach a save file or video when the reproduction setup is complicated.

## Notes

This list intentionally includes:

- Genuine programming defects
- Modern-PC compatibility problems
- Progression and save-state failures
- AI and navigation failures
- Repeatable exploits
- Original behaviours that may be undesirable but intentional

Items should be split into individual GitHub issues only after they have sufficient reproduction information.
