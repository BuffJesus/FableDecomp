import tempfile
import unittest
from pathlib import Path
from tools.script_recovery.build_readable_new_oakvale import build, RAW
from tools.script_recovery.readable_post_attack_cutscene import lower
from tools.script_recovery.test_post_attack_cutscene import lua_trace, native_trace
from tools.script_recovery.lift_native_lua import RData


class PostAttackReadableTests(unittest.TestCase):
    def test_emitted_scope_matches_original_movie_caller(self):
        with tempfile.TemporaryDirectory() as directory:
            report = build(Path(directory))
            source = (Path(directory)/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
            self.assertTrue(report['syntax']['ok'])
            self.assertEqual(report['postAttackCutscene']['implementation'], 'playPostAttackDadCutscene')
            self.assertEqual(source.count('playPostAttackDadCutscene(quest, resources)'), 2)
            self.assertNotIn('RunCutsceneMacro_Func();', source[source.index('function PostAttackStuff('):source.index('function ManageQuestCoreMarkers(')])
            for hero in (0, 0x200c00):
                for acquired in (0, 1):
                    events, error = lua_trace(hero, acquired, source=source+'\nreturn playPostAttackDadCutscene')
                    self.assertIsNone(error)
                    self.assertEqual(events, native_trace(hero, acquired))

    def test_changed_source_native_and_literal_rejected(self):
        source = (RAW/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
        with self.assertRaises(ValueError): lower(source.replace('ppVar9 = 0x0', 'ppVar9 = 0x1'))
        class Changed(RData):
            def bytes_at(self, address, size):
                raw = super().bytes_at(address, size)
                if address <= self.changed < address + size:
                    raw = bytearray(raw); raw[self.changed-address] ^= 1; raw = bytes(raw)
                return raw
        for address in (0xdbedc8, 0xdbee6a, 0xdbeea5, 0x122d70e, 0x1260f0c + 0x20):
            data = Changed(); data.changed = address
            with self.assertRaises(ValueError): lower(source, data)


if __name__ == '__main__': unittest.main()
