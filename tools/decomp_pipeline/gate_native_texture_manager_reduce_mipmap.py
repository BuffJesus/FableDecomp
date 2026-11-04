"""Run the native/compiler/independent-model mip reduction proof."""
import runpy
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
runpy.run_path(str(ROOT/'rebuild/tests/integration/NativeTextureManagerReduceMipmap_test.py'),run_name='__main__')
