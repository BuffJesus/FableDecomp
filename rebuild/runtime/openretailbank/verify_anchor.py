import sys, subprocess, re
sys.path.insert(0, r'D:/Documents/FableTLC/tools')
sys.path.insert(0, r'D:/Documents/FableTLC/tools/decomp_pipeline')
import verify_and_land as V
from pathlib import Path
work=V.WORK_ROOT/'anchor'; work.mkdir(parents=True, exist_ok=True)
e=V.env()
src=Path(r'D:/Documents/FableTLC/rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp')
obj=work/'oab.obj'; obj.unlink(missing_ok=True)
cp=V.cl(['/nologo','/c','/O2','/Oy','/W3',f'/Fo{obj}',str(src)],e)
if cp.returncode or not obj.exists():
    print('COMPILE FAIL'); print(cp.stdout[-2000:]); sys.exit(1)
built,sec,sym=V.obj_text(obj,'OpenRetailBank')
retail=bytes.fromhex(open(r'C:/Users/Cornelio/AppData/Local/Temp/oab_anchor_oracle.tsv',encoding='utf-8-sig').read().splitlines()[1].split('\t')[2])
rel=V.obj_relocs(obj,sec)
print('symbol:',sym)
print('built len:',len(built),' retail len:',len(retail),' relocs:',len(rel))
if built==retail: print('PARITY: MATCH')
elif V.mask(retail,rel)==V.mask(built,rel): print('PARITY: RELOCATION_MATCH')
else:
    print('PARITY: DIFFER')
    # first diff offset
    m=V.mask(retail,rel); b=V.mask(built,rel)
    for i in range(min(len(m),len(b))):
        if m[i]!=b[i]: print('first masked diff at',hex(i),'retail',hex(retail[i]),'built',hex(built[i])); break
