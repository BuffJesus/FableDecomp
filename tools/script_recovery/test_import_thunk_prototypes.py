import unittest
import contextlib
import io
import json
import tempfile
from pathlib import Path
from unittest import mock

from tools.script_recovery import infer_helper_prototypes as inference
from tools.script_recovery.infer_helper_prototypes import purge, REVIEWED, discard_unproven_inferred
from tools.script_recovery.lift_native_lua import RData


class BytesImage:
    def __init__(self, blocks):
        self.blocks = blocks

    def bytes_at(self, address, size):
        return self.blocks.get(address, b'')[:size]


class ImportThunkPrototypeTests(unittest.TestCase):
    def test_import_does_not_inherit_adjacent_ret(self):
        image = BytesImage({0x1000: bytes.fromhex('ff2500200000cccc c21000')})
        self.assertIsNone(purge(image, 0x1000)[0])

    def test_direct_thunk_still_follows_its_target(self):
        image = BytesImage({0x1000: bytes.fromhex('e9fb0f0000'),
                            0x2000: bytes.fromhex('8bc1 c20800')})
        self.assertEqual(purge(image, 0x1000)[:3], (8, True, False))

    def test_direct_thunk_cycle_is_unproven(self):
        image = BytesImage({0x1000: bytes.fromhex('e9fbffffff')})
        self.assertIsNone(purge(image, 0x1000)[0])

    def test_retail_stricmp_import_and_caller_cleanup(self):
        image = RData()
        self.assertEqual(image.bytes_at(0xBFEBA8, 6), bytes.fromhex('ff25ec004401'))
        self.assertEqual(image.bytes_at(0xECE4FF, 10), bytes.fromhex('50 51 e8a206d3ff 83c408'))
        self.assertIsNone(purge(image, 0xBFEBA8)[0])
        spec = REVIEWED[0xBFEBA8]
        self.assertEqual((spec['cc'], len(spec['params'])), ('__cdecl', 2))

    def test_stale_guesses_are_removed_but_reviewed_imports_survive(self):
        image = BytesImage({0x1000: bytes.fromhex('ff2500200000 c21000'),
                            0x2000: bytes.fromhex('c20400'),
                            0x3000: bytes.fromhex('ff2500200000')})
        helpers = {'0x1000': {'source': 'inferred ret 16 (ecx=False)'},
                   '0x2000': {'source': 'inferred ret 4 (ecx=True)'},
                   '0x3000': {'source': 'reviewed CRT import'}}
        self.assertEqual(discard_unproven_inferred(helpers, image), ['0x1000'])
        self.assertEqual(set(helpers), {'0x2000', '0x3000'})

    def test_sick_child_parameter_destruction_and_purge(self):
        image = RData()
        self.assertEqual(image.bytes_at(0xECE7D4, 24),
                         bytes.fromhex('8d4c2438 e80303adff 8d4c243c e8fa02adff 83c434 c20800'))
        spec = REVIEWED[0xECE460]
        self.assertEqual([p['type'] for p in spec['params']], ['CCharString', 'CCharString'])

    def test_rebuild_repairs_existing_spec_and_adds_reviewed_unit_helper(self):
        with tempfile.TemporaryDirectory() as temp:
            evidence = Path(temp)
            existing = {'helpers': {'0xbfeba8': {'source': 'inferred ret 16', 'params': []},
                                    '0x99eae0': {'typedef': 'tCCharString_Destructor'}}}
            (evidence / 'typing_spec.json').write_text(json.dumps(existing))
            calls = [{'target': '0x00BFEBA8', 'currentName': '_stricmp'},
                     {'target': '0x00ECE460', 'currentName': 'Script_InitializeWitchCharacter'}]
            (evidence / 'translation_unit.json').write_text(json.dumps({'functions': [{'calls': calls}]}))
            unit = {'evidence': evidence, 'lo': 0xEC5420, 'hi': 0xED39A0}
            with mock.patch.object(inference, 'script_unit', return_value=unit), \
                    mock.patch.object(inference, 'Image', return_value=RData()), \
                    mock.patch('sys.argv', ['infer_helper_prototypes', '--unit', 'sick_child']), \
                    contextlib.redirect_stdout(io.StringIO()):
                inference.main()
            result = json.loads((evidence / 'typing_spec.json').read_text())['helpers']
            self.assertEqual(result['0xbfeba8'], REVIEWED[0xBFEBA8])
            self.assertEqual(result['0xece460'], REVIEWED[0xECE460])
            self.assertEqual(result['0x99eae0'], existing['helpers']['0x99eae0'])


if __name__ == '__main__':
    unittest.main()
