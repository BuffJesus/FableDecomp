"""VC7.1 linker diagnostics must retain C fastcall startup dependencies."""
import sys
import unittest
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import link_closure as closure

class LinkerSymbols(unittest.TestCase):
    def test_native_startup_diagnostics(self):
        diagnostics = """Global_WinMain.obj : error LNK2019: unresolved external symbol @GFMain@12 referenced in function _WinMain@16
seam_main.obj : error LNK2019: unresolved external symbol __imp__GetModuleHandleA@4 referenced in function _main
Global_WinMain.obj : error LNK2019: unresolved external symbol "void __fastcall MicroThread::SetStack(unsigned char *,unsigned int)" (?SetStack@MicroThread@@YIXPAEI@Z) referenced in function _WinMain@16"""
        found = set(closure.UNRESOLVED.findall(diagnostics)) | set(closure.UNRESOLVED_C.findall(diagnostics))
        self.assertEqual(found, {"@GFMain@12", "__imp__GetModuleHandleA@4", "?SetStack@MicroThread@@YIXPAEI@Z"})

if __name__ == "__main__":
    unittest.main()
