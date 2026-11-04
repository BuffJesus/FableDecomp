"""Fresh canonical async-distance acceptance; no cached scratch evidence."""
import hashlib
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SYMBOL = '??$__distance@U?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@_STL@@@_STL@@YAHABU?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@0@0ABUinput_iterator_tag@0@@Z'

def validate():
    test = ROOT / 'rebuild/tests/integration/NativeAsyncDistance_test.py'
    result = subprocess.run([sys.executable, str(test)], capture_output=True,
                            text=True, timeout=120)
    assert result.returncode == 0, result.stdout + result.stderr
    paths = [line[len('RECEIPT '):] for line in result.stdout.splitlines()
             if line.startswith('RECEIPT ')]
    assert len(paths) == 1, result.stdout
    report = json.loads(Path(paths[0]).read_text())
    assert report['status'] == 'NATIVE_ASYNC_DISTANCE_GATE_PASS'
    assert report['symbol'] == SYMBOL and report['exact']
    assert report['cases_per_mode'] == 1112
    assert report['actual_root_entries'] == {'original': 1112, 'compiled': 1112}
    assert report['links']['control']['returncode'] == 0
    assert report['links']['omission']['returncode'] != 0
    assert report['input_sha256_before'] == report['input_sha256_after']
    for path, digest in {**report['input_sha256_after'], **report['object_sha256']}.items():
        assert hashlib.sha256(Path(path).read_bytes()).hexdigest() == digest, path
    return paths[0]

if __name__ == '__main__':
    print('NATIVE_ASYNC_DISTANCE_ACCEPTANCE_PASS', validate())
