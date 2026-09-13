import hashlib,json,tempfile,unittest
from pathlib import Path
from tools.script_recovery.novi_local_test import validate

class PreflightTests(unittest.TestCase):
    def setUp(self):
        self.temp=tempfile.TemporaryDirectory();self.addCleanup(self.temp.cleanup)
        root=Path(self.temp.name);self.bundle=root/'bundle';self.game=root/'game'
        self.bundle.mkdir();(self.game/'data/Levels').mkdir(parents=True)
        (self.bundle/'FableScriptExtender.dll').write_bytes(b'unchanged baseline fixture')
        (self.game/'Fable.exe').write_bytes(b'game fixture')
        (self.game/'data/Levels/FinalAlbion.qst').write_text('// no duplicate authority\n')
        self.manifest={'files':{'FableScriptExtender.dll':hashlib.sha256(b'unchanged baseline fixture').hexdigest()},
                       'game_exe_sha256':hashlib.sha256(b'game fixture').hexdigest()}
        self.write()
    def write(self):(self.bundle/'manifest.json').write_text(json.dumps(self.manifest))
    def test_valid_is_read_only(self):
        before={str(p):p.read_bytes() for p in Path(self.temp.name).rglob('*') if p.is_file()}
        validate(self.bundle,self.game)
        self.assertEqual(before,{str(p):p.read_bytes() for p in Path(self.temp.name).rglob('*') if p.is_file()})
    def test_changed_original_rejected(self):
        (self.bundle/'FableScriptExtender.dll').write_bytes(b'changed')
        with self.assertRaisesRegex(ValueError,'Payload hash mismatch'):validate(self.bundle,self.game)
    def test_changed_game_rejected(self):
        (self.game/'Fable.exe').write_bytes(b'changed')
        with self.assertRaisesRegex(ValueError,'Unsupported'):validate(self.bundle,self.game)
    def test_duplicate_quest_rejected(self):
        (self.game/'data/Levels/FinalAlbion.qst').write_text('  AddQuest("NewOakValeIntro", TRUE);\n')
        with self.assertRaisesRegex(ValueError,'Duplicate'):validate(self.bundle,self.game)
    def test_manifest_cannot_escape_bundle(self):
        self.manifest['files']={'../game/Fable.exe':self.manifest['game_exe_sha256']};self.write()
        with self.assertRaisesRegex(ValueError,'Payload hash mismatch'):validate(self.bundle,self.game)
if __name__=='__main__':unittest.main()
