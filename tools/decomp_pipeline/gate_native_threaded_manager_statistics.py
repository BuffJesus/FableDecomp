"""Fresh exact statistics getters with independent original/rebuilt behavior."""
from pathlib import Path
import native_exact_roots
from threaded_statistics_oracle import compare_statistics

R = Path(__file__).resolve().parents[2]
ADDRESS = {'__imp__EnterCriticalSection@4':0x143fe0c,'__imp__LeaveCriticalSection@4':0x143fe10}
ROOTS = []
for method,address,size,instructions in (
    ('GetTotalObjectsPendingCount',0x98e390,117,49),
    ('GetTotalBytesPendingCount',0x98e410,118,48),
    ('GetTotalObjectsFinishedCount',0x98e490,117,49),
    ('GetTotalBytesFinishedCount',0x98e510,118,48)):
    ROOTS.append(dict(name=method,source=R/'rebuild/src/compiled/00/98'/('CThreadedFileManager_%s_%08x.cpp'%(method,address)),
                      symbol='?'+method+'@CThreadedFileManager@@QBEJXZ',address=address,size=size,
                      instructions=instructions,relocations=2,finished='Finished' in method,bytes_total='Bytes' in method))


def main():
    behavior = compare_statistics(ROOTS,ADDRESS)
    results = native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-threaded-manager-statistics.json',[behavior],
                                    'Four manager statistics getters; lock APIs controlled, no worker or IO claim.')
    print('NATIVE_THREADED_MANAGER_STATISTICS_PASS cases=%d roots=%d omissions=%d'%
          (behavior['cases'],len(results),behavior['missing_provider_checks']))


if __name__ == '__main__':
    main()
