import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.prepare_resource_storage import bounded_storage


class ResourceStorageTests(unittest.TestCase):
    def test_actual_storage_fragment_bounds_allocations_and_rejects_stale_handles(self):
        compiler = shutil.which('g++')
        if not compiler:
            self.skipTest('g++ required for storage harness')
        with tempfile.TemporaryDirectory() as directory:
            executable = Path(directory) / 'storage.exe'
            subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                            str(Path(__file__).with_name('retail_resource_storage_harness.cpp')),
                            '-o', str(executable)], check=True, capture_output=True, text=True)
            subprocess.run([str(executable)], check=True, capture_output=True, text=True)

    def test_changed_bridge_rejects_instead_of_partial_patch(self):
        with self.assertRaisesRegex(ValueError, 'source anchor changed'):
            bounded_storage('class LuaRetailResources {};')
