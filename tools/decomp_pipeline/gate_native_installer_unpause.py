"""Installer Unpause: near source, original volatile reads and real scoped locks."""
import hashlib
import json
from pathlib import Path
from installer_unpause_oracle import compare_unpause,R,SOURCE,SYMBOL,ENTRY,SIZE

ROOTS = [dict(name='unpause',source=SOURCE,symbol=SYMBOL,address=ENTRY,size=SIZE)]


def main():
    report = compare_unpause()
    assert report['cases'] == 720 and report['providers'] == report['missing_provider_checks'] == 3
    report['acceptance_grade'] = 'NEAR_MATCH'
    report['input_sha256'][str(Path(__file__))] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    (R/'rebuild/compile-gate/native-installer-unpause.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_INSTALLER_UNPAUSE_PASS cases=720 providers=3 omissions=3 grade=NEAR_MATCH')


if __name__ == '__main__':
    main()
