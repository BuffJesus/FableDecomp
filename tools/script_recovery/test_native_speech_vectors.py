import re
import unittest

from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_speech_vectors import recover_vectors, vector_prelude, ROOT
from tools.script_recovery.lift_native_lua import Lifter, RData, load_manifest


class SpeechVectorTests(unittest.TestCase):
    def test_native_tables_match_port_order_and_reject_changed_bytes(self):
        vectors = recover_vectors(RData().bytes_at)
        self.assertEqual(sum(map(len, vectors.values())), 42)
        lua = LuaRuntime()
        source = (ROOT / 'refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/villager_speech.lua').read_text()
        port = lua.execute(source)
        for offset, name in ((0x9c, 'MaleGood'), (0xa8, 'MaleBad'), (0xb4, 'MaleBoth'), (0xc0, 'MaleNeither'),
                             (0xcc, 'FemaleGood'), (0xd8, 'FemaleBad'), (0xe4, 'FemaleBoth'), (0xf0, 'FemaleNeither')):
            self.assertEqual(vectors[hex(offset)], [port[name][i] for i in range(1, len(port[name]) + 1)])
        with self.assertRaises(ValueError):
            recover_vectors(lambda *_: b'changed')

    def test_actual_native_selection_pairs_produce_every_expected_line(self):
        vectors = recover_vectors(RData().bytes_at)
        native = (ROOT / 'ghidra_out/script_recovery/native_decompiles/0x00DADF80.c').read_text()
        pairs = re.findall(r'iVar12 = GetVillagerSpeechIndex\s*\([^;]+;\s*iVar8 = [^;]+;', native)
        self.assertEqual(len(pairs), 8)
        for pair in pairs:
            offset = re.search(r'iVar8 = .*\+ (0x[0-9a-f]+)\);', pair)[1]
            lifter = Lifter(load_manifest(), {}, 'quest', True, '', RData(), static_vectors=vectors)
            lifter.helper_names = {'GetVillagerSpeechIndex'}
            lifter.helper_parameters = {'GetVillagerSpeechIndex': ['count']}
            lifter.helper_return_kinds = {'GetVillagerSpeechIndex': 'number'}
            source = '{\n' + pair + '\nCCharString::operator=(aCStack_6c,(CCharString *)(iVar8 + iVar12 * 4));\nGSI->DisplayGameInfo(aCStack_6c);\n}'
            body = '\n'.join(lifter.lift('Main', source))
            self.assertEqual(lifter.todo, [])
            for index, expected in enumerate(vectors[offset]):
                lua, calls, counts = LuaRuntime(), [], []
                lua.globals().GetVillagerSpeechIndex = lambda _q, _me, count: (counts.append(count) or index)
                q = lua.table_from({'DisplayGameInfo': lambda _q, key: calls.append(key)})
                lua.execute(vector_prelude(vectors) + '\nreturn function(quest,me)\n' + body + '\nend')(q, 'villager')
                self.assertEqual(counts, [len(vectors[offset])])
                self.assertEqual(calls, [expected])


if __name__ == '__main__':
    unittest.main()
