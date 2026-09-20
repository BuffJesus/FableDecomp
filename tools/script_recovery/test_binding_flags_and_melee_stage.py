"""Pins the 2026-09-20 afternoon converter fixes (retail bytes + the second residue audit).

* Entity binding flags: retail builds each `CEntityScriptBinding` as `{vtable, name, this, alloc, 1, flags}`
  (`puVar2[6] = 1;` before `CScriptBase::AddEntityScriptBinding`); the factory 0xE7ED60 forwards the word into
  `CActiveEntityScriptBase::Flags`, whose bit 0 makes `CScriptBase::OnScriptedEntityDeactivated` (0xCB88B0)
  keep the script alive across a level unload instead of marking it terminating. PreMelee's TheRealGuildmaster
  carries 1: retail's woods loop survives the Guild Woods trip and plays WOODSWON on the return (walkthrough
  26:34-26:40); without the flag the sidecar unwound Main on unload and the fresh Main replayed PUNCH.
  Orchard's MK_OFI_GWLL_WHIS2 and the Trader bindings are genuine zeros.
* GuildTrainingMelee TheRealGuildmaster (0x00D58490): the EH flag in an `int *` slot (`infoCounter & 1` on nil),
  the AddQuestInfoBarHealth colour whose address was loaded before its byte stores, the melee grade computed
  into a CCharString-typed slot (nil compare after the fight).
* TraderConflict: `MsgIsHitBySpecialAbilityFrom(p0, 0xe, HERO)` lifted with the receiver alias in the enum slot.
* Will's Guildmaster (0x00D5E0C0): the byte-split resource whose low byte prints as a literal.
"""
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
LIFTED = ROOT / 'refs/script_recovery/lifted'
GUILD = LIFTED / 'GuildTraining/readable_converter/FSE'
GUILD_DRAFT = LIFTED / 'GuildTraining/draft/FSE'
ORCHARD = LIFTED / 'OrchardFarm/readable/FSE'
TRADER = LIFTED / 'TraderConflict/readable/FSE'


def read(p):
    return p.read_text(encoding='utf-8')


class BindingFlagTests(unittest.TestCase):
    def test_guild_bindings_carry_the_keep_alive_flag(self):
        for quest, entities in {
            'GuildTrainingPreMelee': ('TheRealGuildmaster', 'PreMeleeDummy', 'PreMeleeWhisper'),
            'GuildTrainingWoodsMelee': ('ScorpionHome',),
            'GuildTrainingMelee': ('TheRealGuildmaster', 'MeleeOpponent', 'MeleeThunder'),
            'GuildTraining': ('AppleGirl', 'BirdKiller', 'HeroBed', 'PreMeleeMaze'),
            'GuildTrainingWoodsDeparture': ('ArtifactThief', 'FinalMaze'),
        }.items():
            text = read(GUILD / quest / f'{quest}.lua')
            for e in entities:
                self.assertRegex(text, r'AddEntityBinding\("' + e + r'", "[^"]+", 1\)', f'{quest}/{e}')

    def test_zero_flag_bindings_stay_bare(self):
        text = read(ORCHARD / 'OrchardFarmRaid/OrchardFarmRaid.lua')
        self.assertRegex(text, r'AddEntityBinding\("MK_OFI_GWLL_WHIS2", "[^"]+"\)')
        self.assertRegex(text, r'AddEntityBinding\("GuardTeamSpawn", "[^"]+", 1\)')
        text = read(TRADER / 'TraderConflictGood/TraderConflictGood.lua')
        self.assertRegex(text, r'AddEntityBinding\("BCMTrader", "[^"]+"\)')

    def test_oakvale_draft_is_untouched_by_the_flag_lift(self):
        # unit-only behaviour: the hand-reviewed Oakvale package keeps its two-argument bindings
        text = read(LIFTED / 'NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua')
        self.assertNotRegex(text, r'AddEntityBinding\([^)]*, 1\)')


class MeleeStageTests(unittest.TestCase):
    def test_melee_guildmaster(self):
        for stage in (GUILD, GUILD_DRAFT):
            text = read(stage / 'GuildTrainingMelee/Entities/TheRealGuildmaster.lua')
            self.assertNotRegex(text, r'\w+ & 1 ~= 0', 'EH flag survived')
            self.assertNotRegex(text, r'AddQuestInfoBarHealth\([^,]+, (?!\{R = )', 'colour operand')
            self.assertRegex(text, r'AddQuestInfoBarHealth\([^,]+, \{R = 255, G = 255, B = 255, A = 255\}, "HUD_WHISPER_ICON", ')
            self.assertNotIn('TODO(native): xStack_1d4', text)
            self.assertNotIn('(CCharString)(float)', text)
            # the grade: (opponent before - after) - (hero before - after), compared against GUI_MeleeGrades
            self.assertRegex(text, r'= \(+\w+ - [^\n]+?\) - \(\w+ - [^\n]+?\)')

    def test_will_guildmaster_acquires_its_own_resource(self):
        text = read(GUILD / 'GuildTrainingWill/Entities/TheRealGuildmaster.lua')
        acquires = re.findall(r'TryAcquire\((\w+), ', text)
        self.assertGreaterEqual(len(set(acquires)), 4, acquires)

    def test_apple_cleanup_indexes_the_list(self):
        for stage in (GUILD, GUILD_DRAFT):
            text = read(stage / 'GuildTraining/GuildTraining.lua')
            self.assertNotRegex(text, r'RemoveThing\(\w+ \+ \w+', 'arithmetic on the apple list')


class InGameRun3Tests(unittest.TestCase):
    """The 13:45 v6 run: tattoo cards given as visible pickups at the teen transition, blocks stuck at 0/5."""

    def test_teen_transition_tattoo_gives_are_silent(self):
        # retail: GiveHeroObject(&name, -1, true) on GSI slot 0x1e4 (RunTutorials 0x00D45DD0 and Gameflow stage 0)
        for p in (GUILD / 'GuildTraining/GuildTraining.lua', LIFTED / 'Gameflow/readable/FSE/Gameflow/Gameflow.lua'):
            text = read(p)
            self.assertRegex(text, r'GiveHeroObject\("OBJECT_TATTOO_CARD_CHEST_CUSTOM_01", -1, true\)', str(p))
            self.assertNotRegex(text, r'GiveHeroObject\("OBJECT_TATTOO_CARD_\w+", -1\)', str(p))

    def test_melee_opponent_block_stage_has_no_flag_residue(self):
        # the EH flag borrowed the movie handle's slot (`movie & 1` on nil killed Main on the first block)
        for stage in (GUILD, GUILD_DRAFT):
            text = read(stage / 'GuildTrainingMelee/Entities/TheRealGuildmaster.lua'.replace('TheRealGuildmaster', 'MeleeOpponent'))
            self.assertNotRegex(text, r'\w+ & [12]\)? ~= 0')
            self.assertNotIn('unlifted', text)
            self.assertIn('IsPlayerCreatureBlocking()', text)
        will = read(GUILD / 'GuildTrainingWoodsWill/GuildTrainingWoodsWill.lua')
        self.assertNotRegex(will, r'scratchValue\d* & 2')


class TraderAbilityTests(unittest.TestCase):
    def test_special_ability_enum_is_passed(self):
        for f in ('TraderConflictEvil/Entities/TC_BanditFighter.lua', 'TraderConflictEvil/Entities/TC_Villager.lua',
                  'TraderConflictEvil/Entities/IsAGuard.lua'):
            text = read(TRADER / f)
            self.assertNotIn('MsgIsHitByHeroSpecialAbility(me)', text, f)
            self.assertRegex(text, r'MsgIsHitByHeroSpecialAbility\((?:0xe|14|HERO_ABILITY_\w+)\)', f)

    def test_friends_flag_is_a_bool(self):
        for stage in (GUILD, TRADER, ORCHARD):
            for p in stage.rglob('*.lua'):
                self.assertNotIn('SetFriendsWithEverythingFlag(me)', read(p), str(p))


if __name__ == '__main__':
    unittest.main()
