"""Near-parity and actual shutdown/tree ownership proof of manager destruction."""
import json
from pathlib import Path
from threaded_manager_destroy_oracle import compare_destroy,R,SOURCE,SYMBOL,ENTRY

ROOTS=[dict(name='destructor',source=SOURCE,symbol=SYMBOL,address=ENTRY,size=214)]


def main():
    report=compare_destroy()
    assert report['cases']==1440 and report['providers']==5 and report['missing_provider_checks']==5
    assert report['grade']['near_match'] and report['grade']['opcode_similarity']>=0.9
    report['acceptance_grade']='NEAR_MATCH'
    (R/'rebuild/compile-gate/native-threaded-manager-destroy.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_THREADED_MANAGER_DESTROY_PASS cases=1440 providers=5 omissions=5 grade=NEAR_MATCH')


if __name__=='__main__':main()
