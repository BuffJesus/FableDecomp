"""Two in-game Lua runtime errors from the 2026-09-20 teen play-through (user report: archery target dead,
Whisper fight unfinishable after the Guildmaster's third warning), pinned at the generated source.

* `GuildTraining/Entities/SkillTarget.lua`: retail 0x00D41D00 waits on
  `MsgIsHitByWithProjectileWeapon("SCRIPT_NAME_HERO", &damage)` and then tests `0.0 < damage`. The Forge binding
  returns the damage (number) or nil, so the lifter must bind the out slot to the call's result
  (`OUT_AS_RESULT`) instead of leaving it unassigned (`attempt to compare number with nil` on the first arrow).
* `GuildTraining.lua` CheckFriendlyAttacks (0x00D45060): the PreMeleeMaze lookup at the thread's start and the
  TryAcquire actor operand are ONE Ghidra slot (`auStack_a0`) that the typed export spreads over -0x90/-0x80/-0xa0;
  `restore_stack_operands` must keep them one object ("Retail TryAcquire requires an actor" otherwise).
"""
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
UNIT = ROOT / 'refs/script_recovery/lifted/GuildTraining'


def read(rel):
    return (UNIT / rel).read_text(encoding='utf-8')


class SkillTargetProjectileTests(unittest.TestCase):
    def test_damage_is_the_result_and_compared(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTraining/Entities/SkillTarget.lua')
            m = re.search(r'(\w+) = me:MsgIsHitByHeroWithProjectileWeapon\(\)', src)
            self.assertIsNotNone(m, stage)
            var = m.group(1)
            self.assertRegex(src, rf'0\.5 < {re.escape(var)}\b', stage)      # retail `fcomp qword [0x12316f0]` = 0.5
            self.assertNotIn('until me:MsgIsHitByHeroWithProjectileWeapon()', src, stage)


class FriendlyAttackActorTests(unittest.TestCase):
    def test_try_acquire_actor_is_the_maze_lookup(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTraining/GuildTraining.lua')
            body = re.search(r'^function CheckFriendlyAttacks\(quest\)\n.*?^end$', src, re.M | re.S).group(0)
            m = re.search(r'(\w+) = quest:GetThingWithScriptName\("PreMeleeMaze"\)', body)
            self.assertIsNotNone(m, stage)
            actor = m.group(1)
            acquires = re.findall(r'resources:TryAcquire\((\w+), (\w+), 4\)', body)
            self.assertTrue(acquires, stage)
            self.assertEqual(acquires[0][1], actor, (stage, acquires))     # the Maze acquire (the hero's follows)


if __name__ == '__main__':
    unittest.main()


class FriendlyAttackPunishmentTests(unittest.TestCase):
    """the third-warning punishment block (retail 0x00D45060 after GUILD_SEAL_FOURTH_WARNING): the conversation id
    is its own local (not folded onto the Maze resource), the BADHERO actor map carries HERO and MAZE, and the two
    resources released afterwards are the hero's and the Maze's (retail: ~Movie(aCStack_40), ~Movie(aCStack_50))"""

    def test_draft_block(self):
        src = read('draft/FSE/GuildTraining/GuildTraining.lua')
        body = re.search(r'^function CheckFriendlyAttacks\(quest\)\n.*?^end$', src, re.M | re.S).group(0)
        i = body.index('"CS_GUILD_BADHERO"')
        block = body[i - 1500:i + 700]
        conv = re.search(r'(\w+) = quest:AddNewConversation\(', block).group(1)
        hero_res = re.search(r'resources:SetActor\(\w+, "HERO", (\w+)\)', block)
        maze_res = re.search(r'resources:SetActor\(\w+, "MAZE", (\w+)\)', block)
        self.assertIsNotNone(hero_res, 'HERO actor lost')
        self.assertIsNotNone(maze_res, 'MAZE actor lost')
        self.assertNotIn(conv, (hero_res.group(1), maze_res.group(1)), 'conversation id folded onto a resource')
        released = re.findall(r'resources:ReleaseResource\((\w+)\)', block[block.index('"CS_GUILD_BADHERO"'):])
        self.assertIn(maze_res.group(1), released[:2], released)
        self.assertNotIn(conv, released)
        # (the hero resource release once spelled by the actor map's slot -- the export puts its destructor at -0x1c -- is
        # folded by fold_actor_map_releases since 2026-09-21, see BadHeroReleaseTests; a broader destructor-identity rule
        # had regressed nine other scripts in the 2026-09-20 audit)


class SkillTargetScoringTests(unittest.TestCase):
    """The archery targets never scored (user, 2026-09-20 late night: SkillScore 0 through three minutes of live
    SkillTarget Mains, no Lua error). Retail 0x00D41D00 scores by hit fraction against three DOUBLE constants
    (`fcomp qword ptr [0x1238010]` = 0.25, 0x12316f0 = 0.5, 0x129f0b0 = 0.75 -> +worth / +2 / +3 / +4) and writes
    the score through a register holding the field's address (`piVar1 = (int *)(master + 0xa4); *piVar1 += ..`).
    The converter read the constants as 4-byte floats (all 0.0) and left every `*piVar1` store as TODO(native)."""

    def test_ring_thresholds_are_doubles(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTraining/Entities/SkillTarget.lua')
            for k in ('0.25', '0.5', '0.75'):
                self.assertRegex(src, rf'{re.escape(k)} <= \w+|\w+ < {re.escape(k)}', (stage, k))

    def test_every_score_store_lands(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTraining/Entities/SkillTarget.lua')
            self.assertNotIn('*piVar1', src, stage)
            stores = re.findall(r'SetMasterGameState\("SkillScore", quest:GetMasterGameState\("SkillScore"\) ([-+] [^)]*\)?[^)\n]*)\)', src)
            self.assertGreaterEqual(len(stores), 5, (stage, stores))      # -1, +1, +worth, +worth*3, +iVar5 (<<1 / <<2)
            self.assertTrue(any('* 3' in s for s in stores), (stage, stores))
            self.assertTrue(any(s.strip() in ('+ -1', '- 1') for s in stores), (stage, stores))


class GuildmasterExperienceOrbTests(unittest.TestCase):
    """PreMelee Guildmaster 0x00D52E90 (journal 2026-09-20 'converter defect found while reading'): the XP orb comes
    back through a hidden-result slot and is copied into the polled thing by `CCountedPointer::operator=((.. *)(auStack_160
    + 4), &orb->field_0x4)` -- the array-slot spelling the handle fold did not match, so the cutscene-behaviour call got
    nil and the pick-up wait was skipped (ALARM followed PASSED at once)."""

    def test_orb_is_copied_set_up_and_polled(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTrainingPreMelee/Entities/TheRealGuildmaster.lua')
            self.assertNotIn('CCountedPointer', src, stage)
            m = re.search(r'(\w+) = quest:CreateExperienceOrb\(\w+, 1\)\n\s*(?:(\w+) = \1\n\s*)?quest:EntitySetCutsceneBehaviour\((\w+), ', src)
            self.assertIsNotNone(m, stage)
            orb = m.group(2) or m.group(1)
            self.assertEqual(m.group(3), orb, stage)
            self.assertRegex(src, rf'{re.escape(orb)} ~= nil and {re.escape(orb)}:IsAlive\(\)', stage)   # the pick-up wait polls the orb


class SkillTargetMovingDummyTests(unittest.TestCase):
    """The moving-dummies phase (SKILL_MOVE): retail 0x00D41D00 teleports the dummy one segment per frame to a
    C3DVector assembled from three float slots (`fStack_64/60/5c`, passed as `&fStack_64`; two of the four sites go
    through a `pPos` alias set in an if/else). `fold_stack_vector_builds` turns the operand into an inline table and
    EntityTeleportToPosition's position slot is tagged as a vector; before, every segment teleport was a TODO and the
    dummies never moved."""

    def test_every_segment_teleport_has_its_vector(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTraining/Entities/SkillTarget.lua')
            self.assertNotIn('TODO(native): quest:EntityTeleportToPosition', src, stage)
            calls = re.findall(r'quest:EntityTeleportToPosition\(me, ([^,]+), ', src)
            self.assertEqual(len(calls), 3, (stage, calls))     # three sites: the if/else segment pair shares one
            # every site carries its ANGLE (thing, pos, angle, b1, b2): run 5 crashed the moving round on a
            # four-operand call when the angle copy's slot spelling collided with the marker's z
            full = re.findall(r'quest:EntityTeleportToPosition\(me, (?:\{[^}]*\}|\w+), \w+, false, false\)', src)
            self.assertEqual(len(full), 3, (stage, full))
            for operand in calls:
                self.assertTrue(operand.startswith('{x = ') or re.fullmatch(r'\w+', operand), (stage, operand))
            self.assertGreaterEqual(src.count("{x = "), 4, stage)     # four vectors (two direct, two through pPos)


class WillGuildmasterActorMapTests(unittest.TestCase):
    """Will Guildmaster 0x00D5E0C0: every cutscene actor-map store lands. HERO's value was `&CStack_204` -- the hero's
    TryAcquire'd resource (`xStack_200`) seen through the decompiler's 4-byte ESP drift on a byte-split address
    (the bytes push the same `[esp+0x21c]` to both) -- and WHISPER's key was named 4 bytes apart between its
    ctor and its operator[] use, so both stores were TODOs and the WILL_CONTINUE / PLAY_WHISPER cutscenes ran
    with an incomplete actor map."""

    def test_every_actor_store_lands(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTrainingWill/Entities/TheRealGuildmaster.lua')
            self.assertNotIn('TODO(native): resources:SetActor', src, stage)
            self.assertNotIn('NUISystem::CComponent', src, stage)
            stores = re.findall(r'resources:SetActor\(\w+, "(\w+)", (\w+)\)', src)
            self.assertGreaterEqual(len([k for k, _ in stores if k == 'HERO']), 5, (stage, stores))
            self.assertEqual(len([k for k, _ in stores if k == 'WHISPER']), 2, (stage, stores))
            for _, value in stores:
                self.assertRegex(src, rf'{re.escape(value)} = resources:NewResource\(\)|resources:TryAcquire\({re.escape(value)}, |{re.escape(value)} = resources:', (stage, value))


class WoodsWillBanditResourcesTests(unittest.TestCase):
    """GuildTrainingWoodsWill 0x00D67890 holds one resource per WillBandit in a local `CArray<resource>` (zeroed
    begin/end/cap, `push_back(&arr, count, template)`, `arr + 0x10 * k` elements, the array destructor). Modelled
    as a Lua list of fresh resources (`fold_local_resource_arrays`): the acquire loop, the BAN1..3 actor-map
    stores and both releases were TODOs (the woods Will stage's bandits were never acquired)."""

    def test_bandit_list_is_built_acquired_stored_and_released(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTrainingWoodsWill/GuildTrainingWoodsWill.lua')
            self.assertNotIn('TODO(native)', src, stage)
            self.assertIn('resources:NewResource() end return t end)', src, stage)
            for k in ('BAN1', 'BAN2', 'BAN3'):
                self.assertRegex(src, rf'resources:SetActor\(\w+, "{k}", \w+\[\d \+ 1\]\)', (stage, k))
            self.assertRegex(src, r'resources:TryAcquire\(\w+\[[^\]]+\], \w+\[[^\]]+\], 4\)', stage)
            self.assertEqual(src.count('do resources:ReleaseResource(r) end'), 2, stage)


class FriendlyAttackMazeChecksTests(unittest.TestCase):
    """CheckFriendlyAttacks 0x00D45060 also asks whether the hero hit the Maze (`MsgIsHitBy` / `MsgIsHitByAnySpecialAbilityFrom`
    / `MsgIsHitBySpecialAbilityFrom(0xe)` on the Maze thing through its Info pointer, Ghidra's `uStack_9c` =
    `auStack_a0 + 4`): the object was canonicalised to another slot by the 2026-09-20 fix, so slot arithmetic lost the
    field and the three checks were TODOs. Also: the two SaveXP cutscene helpers share one bsim label
    (`RunSaveXPCutscene2`) and were never called."""

    def test_maze_hit_checks_and_helper_calls(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTraining/GuildTraining.lua')
            body = re.search(r'^function CheckFriendlyAttacks\(quest\)\n.*?^end$', src, re.M | re.S).group(0)
            self.assertNotIn('uStack_9c', body, stage)
            maze = re.search(r'(\w+) = quest:GetThingWithScriptName\("PreMeleeMaze"\)', body).group(1)
            for method in ('MsgIsHitByHero()', 'MsgIsHitByAnySpecialAbilityFromHero()', 'MsgIsHitByHeroSpecialAbility('):
                self.assertIn(f'{maze}:{method}', body, (stage, method))
            self.assertRegex(src, r'\n\s+RunSaveXPCutscene\(quest\)\s*\n', stage)      # (called from Main's stage chain)
            self.assertRegex(src, r'\n\s+RunSaveXPCutscene2\(quest\)\s*\n', stage)
            self.assertNotIn('RunSaveXPCutscene2__at', src, stage)


class BadHeroReleaseTests(unittest.TestCase):
    """CheckFriendlyAttacks' BADHERO cleanup released the ACTOR MAP as a resource (`ReleaseResource(actorMap)` -> "Invalid
    or released retail resource" in-game, 2026-09-21 resume 2): the export put the hero resource's destructor at the map's
    slot. `fold_actor_map_releases`: a release naming an actor map takes the nearest still-held resource above it."""

    def test_badhero_releases_the_hero_resource(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTraining/GuildTraining.lua')
            i = src.index('"CS_GUILD_BADHERO"')
            block = src[i:i + 700]
            maps = set(re.findall(r'(\w+) = resources:NewActorMap\(\)', src[i - 2500:i]))
            after = block[block.index('DestroyActorMap('):]
            released = re.findall(r'resources:ReleaseResource\((\w+)\)', after)[:2]     # the hero's and the Maze's
            self.assertEqual(len(released), 2, (stage, released))
            self.assertFalse(set(released) & maps, (stage, released, maps))


class SkillDisqualifiedFlagTests(unittest.TestCase):
    """Skill Guildmaster 0x00D5AE70: WON vs DISQUALIFIED is selected by the left-the-ring flag `cStack_215`, which
    Ghidra read as `uStack_21c._3_1_` (a timer id's top byte: `mov al, [esp+0x1b]` with one push outstanding = the flag
    at [esp+0x1f]). Lifted as an unassigned local, `nil == 0` sent every moving round to DISQUALIFIED (run 4)."""

    def test_selector_reads_the_flag(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTrainingSkill/Entities/TheRealGuildmaster.lua')
            i = src.index('"CS_GUILD_SKILL_DISQUALIFIED"')
            head = src[:i]
            # the selector is the `if <flag> == 0 then` that opens the WON branch (the grade switch sits inside it)
            sel = [v for v in re.findall(r'if (c_stk_215(?:_\d+)?) == 0 then', head[-6000:])]
            self.assertTrue(sel, stage)
            flag = sel[-1]
            self.assertRegex(head, rf'\n\s*{re.escape(flag)} = (?:1|true)\b', (stage, flag))    # set when the hero leaves the ring
            self.assertNotIn('uStack_21c._3_1_', src, stage)


class WillDummySpinTests(unittest.TestCase):
    """WillDummy 0x00D43450: a lightning hit spins the dummy (`fStack_a0 = angle + 0.25; SetFacingAngle(me, fStack_a0, true)`).
    The slot restoration respelled the by-value float as an object (`xStack_94`), the lifter refused the store, and the
    call passed nil -> `sol: no matching function call` on the first bolt (run 4, 2026-09-21)."""

    def test_spin_angle_is_stored_and_passed(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTraining/Entities/WillDummy.lua')
            self.assertNotIn('TODO(native)', src, stage)
            m = re.search(r'\n\s*(?:local )?(\w+) = angle \+ 0\.25\n\s*quest:EntitySetFacingAngle\(me, (\w+), true\)', src)
            self.assertIsNotNone(m, stage)
            self.assertEqual(m.group(1), m.group(2), stage)


class WillGuildmasterSpeakKeyTests(unittest.TestCase):
    """Will Guildmaster 0x00D5E0C0 (run 6, 2026-09-21: the WON subtitle printed "CS_GUILD_WILL_WON"): the two Speak
    sites after the play-with-Whisper question pass the text key as a bare .rdata address (`0x12d1148` /
    `0x12d1368`) the decompiler never resolved; the string slot must take that literal, not a stale temporary."""

    def test_speak_keys_are_the_rdata_strings(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTrainingWill/Entities/TheRealGuildmaster.lua')
            self.assertIn('me:Speak(me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_NO", ', src, stage)
            self.assertIn('me:Speak(me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_YES", ', src, stage)
            self.assertNotRegex(src, r'me:Speak\(me, "(?:CS_GUILD_WILL_WON|\$GRADE|WHISPER)"', stage)
            self.assertNotIn('0x12d1148', src, stage)


class ThingBoolOperandTests(unittest.TestCase):
    """Bool operands of CScriptThing slot calls (`_N` in the decorated name) must be Lua booleans: the sidecar's
    sol2 build has no safeties, so a `0` (truthy) SET `SetFriendsWithEverythingFlag` / `SetToKillOnLevelUnload`
    instead of clearing them. The Will Guildmaster's two drifted `auStack_1b4._0_4_` Data-field sites (the
    apprentice's SetToKillOnLevelUnload) also lift now (`__thing_valid` + thing call)."""

    def test_no_numeric_bool_operands(self):
        for rel in ('draft/FSE/GuildTrainingWill/Entities/TheRealGuildmaster.lua',
                    'draft/FSE/GuildTrainingSkill/Entities/TheRealGuildmaster.lua',
                    'draft/FSE/GuildTraining/Entities/CombatApprentice.lua',
                    'draft/FSE/GuildTrainingWoodsMelee/Entities/ScorpionHome.lua'):
            src = read(rel)
            self.assertNotRegex(src, r':(?:SetFriendsWithEverythingFlag|SetToKillOnLevelUnload)\((?:0|1|0x[01])\)', rel)

    def test_will_apprentice_kill_on_unload(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTrainingWill/Entities/TheRealGuildmaster.lua')
            self.assertEqual(len(re.findall(r'\w+:SetToKillOnLevelUnload\(false\)', src)), 3, stage)
            self.assertNotIn('0x118))(0)', src, stage)


class PreMeleeNagTimerTests(unittest.TestCase):
    """PreMelee Guildmaster 0x00D52E90: the "hit the dummy" nag timer is re-armed only when DummyHits CHANGES
    (`CStack_180` = the last count, init 0, compared with the parent field each frame). The export typed the
    field read as a by-value CCharString and the init was inlined as a temporary (`0 ~= DummyHits` forever)."""

    def test_last_count_is_a_local(self):
        for stage in ('draft', 'readable_converter'):
            src = read(f'{stage}/FSE/GuildTrainingPreMelee/Entities/TheRealGuildmaster.lua')
            m = re.search(r'if (\w+) ~= quest:GetStateInt\("DummyHits"\) then', src)
            self.assertIsNotNone(m, stage)
            self.assertIn(f'{m.group(1)} = quest:GetStateInt("DummyHits")', src, stage)
            self.assertRegex(src, rf'{re.escape(m.group(1))} = 0\b', stage)
            self.assertNotRegex(src, r'if 0 ~= quest:GetStateInt\("DummyHits"\)', stage)
