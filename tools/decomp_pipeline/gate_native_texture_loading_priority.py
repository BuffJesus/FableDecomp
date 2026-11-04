"""Exact original writable signed-long storage, not loader behavior coverage."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data

ROOT=Path(__file__).resolve().parents[2]
source=ROOT/'rebuild/src/compiled/01/39/Global_TextureLoadingPriority_01396c64.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeTextureLoadingPriority_test.cpp'
work=Path(tempfile.mkdtemp(prefix='native-texture-loading-priority-'))
env=V.env()
obj=work/'priority.obj'
fo=work/'fixture.obj'
for src,out in ((source,obj),(fixture,fo)):
    r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(src)],env,work)
    assert not r.returncode,r.stdout+r.stderr
coff=Coff(obj)
symbol='?TextureLoadingPriority@@3JA'
section,offset=coff.symbols[symbol]
data,relocs=coff.sections[section]
assert offset==0 and len(data)==4 and not relocs
original=read_virtual_data(EXE.read_bytes(),0x1396c64,4)
assert data==original==b'\x03\x00\x00\x00'
exe=work/'fixture.exe'
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),str(fo)]
omission=subprocess.run(cmd,env=env,capture_output=True,text=True)
diagnostic=omission.stdout+omission.stderr
assert omission.returncode and symbol in diagnostic
assert 'LNK2001' in diagnostic or 'LNK2019' in diagnostic
linked=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True)
assert not linked.returncode,linked.stdout+linked.stderr
run=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
assert not run.returncode,(run.returncode,run.stdout,run.stderr)
assert 'NATIVE_TEXTURE_LOADING_PRIORITY_PASS cases=4 initial=3 writable=1 signed=1' in run.stdout
report=dict(address='01396c64',symbol=symbol,bytes=4,grade='DATA_MATCH',
    initial_signed_long=3,cases=4,omission_checks=1,result=run.stdout.strip(),
    original_data_sha256=hashlib.sha256(original).hexdigest(),
    source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()
                   for p in (source,fixture,Path(__file__))},
    boundary='Original4B signedlong global storage with VC7.1 symbol/data proof and mandatory source omission. Startup3 and writable0/3/80000000/FFFFFFFF signed/raw reads. No loader,async read,thread,GPU or game-launch behavior claim.')
(ROOT/'rebuild/compile-gate/native-texture-loading-priority.json').write_text(
    json.dumps(report,indent=2)+'\n',encoding='utf-8')
print(run.stdout.strip())
print('NATIVE_TEXTURE_LOADING_PRIORITY_GATE_PASS')
