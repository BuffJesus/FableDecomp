import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path
from tools.script_recovery.prepare_man_resource_extension import prepare,METHODS


class RetailResourceActionsTests(unittest.TestCase):
    def test_changed_native_dispatch_rejects_proposal(self):
        class Changed:
            def bytes_at(self,address,size): return b'\x90'*size
        with tempfile.TemporaryDirectory() as directory:
            with self.assertRaisesRegex(ValueError,'Native MoveToPosition wrapper changed'):
                prepare(Path(directory)/'runtime',Path(directory)/'proposal',Changed())

    def test_candidate_forwards_native_arguments_without_implicit_policy(self):
        compiler=shutil.which('g++')
        if not compiler: self.skipTest('C++ forwarding harness requires g++')
        root=Path(__file__).parent
        with tempfile.TemporaryDirectory() as directory:
            executable=Path(directory)/'resource_actions.exe'
            subprocess.run([compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(root/'retail_resource_actions_harness.cpp'),'-o',str(executable)],check=True,capture_output=True,text=True)
            subprocess.run([str(executable)],check=True,capture_output=True,text=True)

    def test_preparation_is_read_only_and_rejects_existing_extension(self):
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory)
            runtime=root/'runtime'
            header=runtime/'FableScriptExtender/LuaRetailResources.h'
            header.parent.mkdir(parents=True)
            source='class LuaRetailResources {\n    unsigned NewThingFromResource(unsigned id) {\n    }\n};\n    type["NewThingFromResource"] = &LuaRetailResources::NewThingFromResource;\n'
            header.write_text(source)
            out=root/'proposal'
            prepare(runtime,out)
            self.assertEqual(header.read_text(),source)
            candidate=(out/'LuaRetailResources.h').read_text()
            for method in METHODS:
                self.assertIn(f'void {method}(',candidate)
                self.assertIn(f'type["{method}"] = &LuaRetailResources::{method};',candidate)
            header.write_text(candidate)
            with self.assertRaisesRegex(ValueError,'already contains'):
                prepare(runtime,out)
