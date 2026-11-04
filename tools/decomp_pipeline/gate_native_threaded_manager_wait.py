"""Near wait root with real manager priority and UID lookup behavior."""
import json
from threaded_manager_wait_oracle import compare_wait,R,SOURCE,SYMBOL,ENTRY

ROOTS=[dict(name='wait',source=SOURCE,symbol=SYMBOL,address=ENTRY,size=181)]


def main():
    report=compare_wait()
    assert report['cases']==1440 and report['providers']==3 and report['missing_provider_checks']==3
    assert report['grade']['near_match']
    report['acceptance_grade']='NEAR_MATCH'
    (R/'rebuild/compile-gate/native-threaded-manager-wait.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_THREADED_MANAGER_WAIT_PASS cases=1440 providers=3 omissions=3 grade=NEAR_MATCH')


if __name__=='__main__':main()
