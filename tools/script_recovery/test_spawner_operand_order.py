"""Q_GuildTraining `RunTutorials` (0x00D45DD0) spawns with the literals in the binding's slots.

Pins the silent operand rotation found by the 2026-09-20 residue audit: the AppleMarker vector's stack slot
(-0x54) is reused by Ghidra as the hidden by-value result of fifteen `GetThingWithScriptName` calls and as
the Guildmaster `TryAcquire` thing; `fold_local_thing_vectors` rewrote every bare `(CScriptThing *)xStack_54`
to `LOCALLIST_At(xStack_54, 0)` regardless of the vector's live range, the lifter then lost the hidden-result
slot, shed the real string operand and back-filled from its LIFO literal pool. Result: every Guild
`CreateCreature` / the PreMeleeDummy `CreateObject` had (scriptName, marker, defName) rotated into
(defName, position-source, scriptName) -- `GetThingWithScriptName("CREATURE_...")` is nil and `:GetPos()`
raises at GameState 5 (the first stage after the woods); two `AddLineToConversation` speaker/key swaps, an
`EntityTeleportToThing` swap and `TryAcquire(resource, appleMarker[0 + 1], 4)` came from the same cause.
Sidecar binding order (LuaQuestState.cpp): CreateObject / CreateCreature = (defName, position, scriptName).
"""
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
UNIT = ROOT / 'refs/script_recovery/lifted/GuildTraining'
STAGES = {
    'draft': UNIT / 'draft/FSE/GuildTraining/GuildTraining.lua',
    'readable_converter': UNIT / 'readable_converter/FSE/GuildTraining/GuildTraining.lua',
}
# (def name, marker script name, created script name) per retail site, from the typed C of 0x00D45DD0
SPAWNS = [
    ('OBJECT_STRAW_DUMMY_01', 'PreMeleeDummyMarker', 'PreMeleeDummy'),
    ('CREATURE_GUILD_EVIL_APPRENTICE_MALE', 'CombatApprenticeMarker', 'CombatApprentice'),
    ('CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE', 'M_MeleeOpponentStand', 'MeleeApprentice'),
    ('CREATURE_GUILD_EVIL_APPRENTICE_MALE', 'SkillApprenticeMarker', 'SkillApprentice'),
    ('CREATURE_GUILD_EVIL_APPRENTICE_MALE', 'BirdKillerMarker', 'BirdKiller'),
    ('CREATURE_GUILD_EVIL_APPRENTICE_MALE', 'WillApprenticeMarker', 'WillApprentice'),
    ('CREATURE_RIVAL_HERO_WHISPER_APPRENTICE', 'MeleeApprenticeMarker', 'MeleeApprentice'),
]
RE_SPAWN = re.compile(r'quest:(CreateObject|CreateCreature)\("([^"]*)",\s*([^,]+?),\s*"([^"]*)"')


class SpawnerOperandOrderTests(unittest.TestCase):
    def test_every_spawn_has_def_position_scriptname(self):
        for stage, path in STAGES.items():
            text = path.read_text(encoding='utf-8')
            calls = [(m.group(2), m.group(3), m.group(4)) for m in RE_SPAWN.finditer(text)]
            self.assertTrue(calls, stage)
            for defname, _pos, script in calls:
                self.assertTrue(defname.startswith(('OBJECT_', 'CREATURE_')) or defname == '',
                                f'{stage}: def slot holds {defname!r}')
                self.assertFalse(script.startswith(('OBJECT_', 'CREATURE_', 'M_', 'MK_')),
                                 f'{stage}: script-name slot holds {script!r}')
            for defname, marker, script in SPAWNS:
                self.assertRegex(text, r'quest:Create(?:Object|Creature)\("' + re.escape(defname) + r'",',
                                 f'{stage}: no spawn of {defname}')
                self.assertIn(f'"{script}"', text, stage)
            # the position source of every spawn is a marker lookup, never a def-name lookup
            self.assertNotRegex(text, r'GetThingWithScriptName\("(?:OBJECT_|CREATURE_)', stage)

    def test_neighbouring_operands(self):
        for stage, path in STAGES.items():
            text = path.read_text(encoding='utf-8')
            self.assertNotIn('AddLineToConversation(conversationId, "TheRealGuildmaster"', text, stage)
            self.assertNotRegex(text, r'TryAcquire\(\w+, \w+\[0 \+ 1\], 4\)', stage)
            self.assertNotRegex(text, r'EntityTeleportToThing\(quest:GetThingWithScriptName\("M_GuildmasterMarker"\)', stage)


if __name__ == '__main__':
    unittest.main()
