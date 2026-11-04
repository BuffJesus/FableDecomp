"""Original save/reload/call windows; no full Main or native callee claim."""
import json
from pathlib import Path
import subprocess
import sys
import tempfile

ROOT=Path(__file__).resolve().parents[2]
def test_original_marker_save_reload_and_removal_call():
    # Keep Unicorn in its own process; other tests may host Lupa on Windows.
    with tempfile.TemporaryDirectory(prefix='codex_lua_assassin_native_',dir=ROOT/'work') as scratch:
        path=Path(scratch)/'result.json'
        assert path.resolve().is_relative_to((ROOT/'work').resolve())
        result=subprocess.run([sys.executable,str(ROOT/'tools/script_recovery/assassin_cleanup_native.py'),str(path)],
                              cwd=ROOT,capture_output=True,text=True,timeout=120)
        assert result.returncode==0,result.stdout+result.stderr
        data=json.loads(path.read_text(encoding='utf-8'))
        assert len(data['cases'])==9
        assert {c['reload'] for c in data['cases']}=={None,'0xe02f87','0xe02f8d'}
        for case in data['cases']:
            assert int(case['actor'],16)==int(case['owner'],16)+8
            assert case['flags']==[0,1]
