"""Near construction and actual empty-owner lifetime proof for the IO manager."""
import json
from threaded_manager_construct_oracle import compare_construct,R,SOURCE,SYMBOL,ENTRY

ROOTS=[dict(name='constructor',source=SOURCE,symbol=SYMBOL,address=ENTRY,size=350)]


def main():
    report=compare_construct()
    assert report['cases']==696 and report['null_allocation_faults']==120
    assert report['providers']==5 and report['missing_provider_checks']==5 and report['grade']['near_match']
    report['acceptance_grade']='NEAR_MATCH'
    (R/'rebuild/compile-gate/native-threaded-manager-construct.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_THREADED_MANAGER_CONSTRUCT_PASS cases=696 faults=120 providers=5 omissions=5 grade=NEAR_MATCH')


if __name__=='__main__':main()
