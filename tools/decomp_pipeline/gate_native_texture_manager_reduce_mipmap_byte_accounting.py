import runpy
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
runpy.run_path(str(ROOT/'rebuild/tests/integration/NativeTextureManagerReduceMipmapByteAccounting_test.py'),run_name='__main__')
