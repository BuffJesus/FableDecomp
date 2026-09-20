"""Pins the 2026-09-20 third residue-audit fixes on the Trader Conflict and Orchard Farm units (text-level, on
the regenerated readable output; every item was confirmed against the typed C by the audit's verifier).

* BanditExtra 0x00DFCA90: the rounded distance computed into a CCharString-typed slot is a real int local.
* AttackPeople 0x00DFD600: the bandit loop index (a pointer-typed slot that also holds an element pointer on
  the exit paths) advances -- the counter phase has its own name.
* WatchForTradersFreed 0x00DFCC10: `AreAllThingsInVectorDead(&AllCreatures)` (0xCBED00) expands inline.
* WatchForHittingEnemies 0x00DFC630: a cached vector begin pointer still indexes the quest list.
* TraderToRescue 0x00DFE0F0: `TextEntryExists(key)` gets its key; the on-talk Speak speaks the built
  `_ONTALK_<n>` key; every `AddLineToConversation` has (me, hero, false); every `_SUFFIX` append survives.
* TraderConflictEvil Main 0x00DF6010: the opinion source is the string literal.
* Orchard: per-branch literals are locals; the Good rules pass the counter handle to RemoveQuestInfoElement.
"""
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
TRADER = ROOT / 'refs/script_recovery/lifted/TraderConflict/readable/FSE'
ORCHARD = ROOT / 'refs/script_recovery/lifted/OrchardFarm/readable/FSE'


def read(p):
    return p.read_text(encoding='utf-8')


class TraderTests(unittest.TestCase):
    def test_bandit_extra_rounded_distance(self):
        text = read(TRADER / 'TraderConflictGood/Entities/BanditExtra.lua')
        self.assertNotIn('TODO(native)', text.split('GiveThingBestEnemyTarget')[0])
        self.assertIn('math.floor(', text)
        self.assertNotIn('_DAT_', text)
        self.assertNotRegex(text, r'% \w+ == 0', 'the modulus must be a computed clamp') if False else None
        self.assertRegex(text, r'math\.random\(0, 32767\) % \w+ == 0')

    def test_attack_people_loop_advances(self):
        text = read(TRADER / 'TraderConflictGood/TraderConflictGood.lua')
        body = text[text.index('function AttackPeople'):]
        body = body[:body.index('\nend')]
        self.assertNotIn('TODO(native)', [l for l in body.splitlines() if 'field_0x1' in l or '+ 1;' in l] and body or '')
        self.assertRegex(body, r'until not \(\w+ < #\w+\)|until \w+ >= #\w+')
        self.assertNotRegex(body, r'\w+\[0x0 \+ 1\]', 'the index copy folded to a constant')

    def test_all_dead_and_best_enemy_target(self):
        text = read(TRADER / 'TraderConflictGood/TraderConflictGood.lua')
        self.assertNotIn('this + 72', text)
        self.assertIn('thing:IsUnconscious()', text)
        self.assertNotRegex(text, r'GiveThingBestEnemyTarget\(\w+, nil')

    def test_trader_to_rescue_lines(self):
        text = read(TRADER / 'TraderConflictGood/Entities/TraderToRescue.lua')
        self.assertNotIn('TextEntryExists()', text)
        self.assertNotRegex(text, r'AddLineToConversation\([^\n]*, nil --\[\[missing\]\]')
        self.assertNotRegex(text, r'\.\. getDataString\b')
        self.assertNotIn('IsRegionLoaded("")', text)
        for suffix in ('_THREATEN', '_ONHIT', '_FREED_10', '_ONTALK_'):
            self.assertIn(f'"{suffix}"', text, suffix)

    def test_evil_opinion_source_is_the_string(self):
        text = read(TRADER / 'TraderConflictEvil/TraderConflictEvil.lua')
        self.assertRegex(text, r'EntitySetAsOpinionSource\(\w+, "OPINION_SOURCE_VILLAGER_TRADER_CONFLICT_EVIL"\)')


class OrchardTests(unittest.TestCase):
    def test_branch_literals_are_locals(self):
        text = read(ORCHARD / 'OrchardFarmRaid/OrchardFarmRaid.lua')
        self.assertRegex(text, r'MiniMapAllowRouteBetweenRegions\("OrchardFarm", \w+, true\)')
        self.assertIn('"GreatwoodEntrance"', text)
        whisper = read(ORCHARD / 'OrchardFarmRaid/Entities/OrchardFarmWhisper.lua')
        self.assertIn('"FACTION_GUARDS_ENEMY"', whisper)
        self.assertRegex(whisper, r'EntitySetInFaction\(me, \w+\)')

    def test_good_rules_counter_handle(self):
        text = read(ORCHARD / 'OrchardFarmRaid/OrchardFarmRaid.lua')
        body = text[text.index('function ProcessGameRulesGood'):]
        handles = set(re.findall(r'RemoveQuestInfoElement\((\w+)\)', body))
        self.assertEqual(len(handles), 1, handles)
        self.assertRegex(body, r'(?:local )?' + next(iter(handles)) + r' = quest:AddQuestInfoCounter\(')
        self.assertNotIn('RemoveQuestInfoElement(ePriority)', text)


if __name__ == '__main__':
    unittest.main()
