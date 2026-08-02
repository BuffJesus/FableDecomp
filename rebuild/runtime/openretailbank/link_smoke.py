import sys, subprocess, glob, re, os
sys.path.insert(0, r'D:/Documents/FableTLC/tools')
sys.path.insert(0, r'D:/Documents/FableTLC/tools/decomp_pipeline')
import verify_and_land as V
from pathlib import Path
e=V.env()
work=V.WORK_ROOT/'linksmoke'; 
import shutil; shutil.rmtree(work, ignore_errors=True); work.mkdir(parents=True, exist_ok=True)
ROOT=Path(r'D:/Documents/FableTLC')
# gather ring sources: anchor + 17 landed (from manifest) + 10 faithful
srcs=[ROOT/'rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp']
man=open(ROOT/'rebuild/runtime/openretailbank/link_manifest.tsv',encoding='utf-8').read().splitlines()[1:]
for ln in man:
    f=ln.split('\t')
    st,src=f[3],f[4]
    if st=='landed-byte-exact': srcs.append(ROOT/src)
    elif st=='faithful-behavior': srcs.append(ROOT/src)
objs=[]; failed=[]
for s in srcs:
    if not s.exists(): failed.append((str(s),'NOSRC')); continue
    o=work/(s.stem+'.obj'); 
    cp=V.cl(['/nologo','/c','/O2','/Oy','/W3',f'/Fo{o}',str(s)],e)
    if cp.returncode or not o.exists(): failed.append((s.name,'CC_FAIL: '+ (cp.stdout or '')[-300:]))
    else: objs.append(o)
print(f"compiled {len(objs)}/{len(srcs)} objs; {len(failed)} failed")
for n,why in failed: print('  FAIL', n, why[:200])
# attempt link into a DLL (noentry) to force unresolved-externals report
exe=work/'oab_ring.dll'
lk=subprocess.run([str(V.VC/'bin'/'link.exe'),'/nologo','/DLL','/NOENTRY','/FORCE:UNRESOLVED',
    f'/out:{exe}']+[str(o) for o in objs],capture_output=True,text=True,env=e)
un=sorted(set(re.findall(r'unresolved external symbol (\S+)', lk.stdout)))
print(f"\n=== unresolved externals: {len(un)} ===")
for u in un: print(' ', u)
open(r'C:/Users/Cornelio/AppData/Local/Temp/oab_unresolved.txt','w').write('\n'.join(un))
