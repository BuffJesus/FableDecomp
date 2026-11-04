"""Fresh VC71 parity gate: CCountedPointer<CNavConnectorNode>::DeleteData (0x00A82FA0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CNavConnectorNode@@QAE@XZ':0xa780b0,
 '??3@YAXPAX@Z':0xbfe9bc,
}
ROOTS=[
 dict(name='deletedata_cnavconnectornode',source=R/'rebuild/src/compiled/00/a8/CCountedPointer_NavConnectorDeleteData_00a82fa0.cpp',symbol='?DeleteData@?$CCountedPointer@VCNavConnectorNode@@@@CIXPAX@Z',address=0xa82fa0,size=23,instructions=10,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-nav-connector-delete-data.json',[],
  'CCountedPointer<CNavConnectorNode>::DeleteData (0x00A82FA0), exact after relocation')
 print('NATIVE_NAV_CONNECTOR_DELETE_DATA_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
