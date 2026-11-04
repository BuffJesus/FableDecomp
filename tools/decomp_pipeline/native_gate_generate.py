"""native_gate_generate.py <gate_name> <TAG> <source.cpp> [--gs] <description> name:sym=addr ...
Compiles, derives each relocation target from retail bytes, cross-checks targets against
rebuild/build/symbol_index.tsv where the symbol is indexed, and writes
tools/decomp_pipeline/gate_native_<gate_name>.py. Prints the gate_all line."""
import sys,struct,tempfile,csv
from pathlib import Path
R=Path(r'D:\Documents\FableTLC');sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
a=sys.argv[1:];gname=a.pop(0);tag=a.pop(0);src=Path(a.pop(0)).resolve()
gs='--gs' in a
if gs:a.remove('--gs')
desc=a.pop(0)
FL=['/nologo','/c','/O2','/GS','/MT','/W3'] if gs else ['/nologo','/c','/O2','/MT','/W3']
b=Path(tempfile.mkdtemp());obj=b/'t.obj'
r=V.cl(FL+['/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(src)],V.env(),b)
if r.returncode:print(r.stdout+r.stderr);sys.exit(1)
c=Coff(obj);raw=EXE.read_bytes();md=Cs(CS_ARCH_X86,CS_MODE_32)
idx={}
for row in csv.reader(open(R/'rebuild/build/symbol_index.tsv',encoding='utf-8'),delimiter='\t'):
    if len(row)>1:idx.setdefault(row[0],set()).add(int(row[1],16) if row[1][:1].isalnum() and all(ch in '0123456789abcdefABCDEF' for ch in row[1]) else -1)
address={};roots=[];bad=False
for spec in a:
    nm,rest=spec.split(':',1);sym,ad=rest.rsplit('=',1);ad=int(ad,16)
    sec,off=c.symbols[sym];assert off==0;built,rels=c.sections[sec]
    orig=read_virtual_data(raw,ad,len(built));bb=bytearray(built);oo=bytearray(orig)
    for site,(n,k) in rels.items():
        bb[site:site+4]=b'\0'*4;oo[site:site+4]=b'\0'*4
        addend=struct.unpack_from('<i',built,site)[0]
        if k==20:t=(ad+site+4+struct.unpack_from('<i',orig,site)[0]-addend)&0xffffffff
        else:t=(struct.unpack_from('<I',orig,site)[0]-addend)&0xffffffff
        if n in address and address[n]!=t:print('CONFLICT',n,hex(address[n]),hex(t));bad=True
        address[n]=t
        if n in idx and t not in idx[n]:print('INDEX-MISMATCH',n,hex(t),[hex(x) for x in idx[n]])
    if bb!=oo:print('DIFF',nm);bad=True
    ni=len(list(md.disasm(orig,ad)))
    roots.append((nm,sym,ad,len(built),ni,len(rels)))
if bad:sys.exit(1)
# A derived (unindexed) callee is pinned to whatever retail calls at that site, so a swapped
# same-signature call still passes. Warn when the manifest/Ghidra names at the target disagree.
import re,glob
known={}
for f in [R/'rebuild/manifest/functions.tsv']+[Path(p) for p in glob.glob(str(R/'ghidra_out/labels_*.tsv'))]:
    for row in csv.reader(open(f,encoding='utf-8',errors='replace'),delimiter='\t'):
        if len(row)>1 and re.match(r'^(0x)?[0-9a-fA-F]{6,8}$',row[0]):known.setdefault(int(row[0],16),set()).add(row[1])
for n,t in address.items():
    m=re.match(r'\?([A-Za-z_][A-Za-z0-9_]*)@[A-Za-z_]',n)
    if n in idx or not m or t not in known:continue
    # A callee defined in this same object is pinned by its own root, not by guesswork.
    if n in c.symbols and c.symbols[n][1]==0 and len(c.sections[c.symbols[n][0]][0])>0:continue
    if not any(m.group(1) in k for k in known[t]):print('NAME-CHECK',n,hex(t),'labels:',sorted(known[t])[:4])
rel=src.relative_to(R).as_posix()
L=['"""Fresh VC71 parity gate: %s."""'%desc.replace(chr(92),chr(92)*2),'import sys','from pathlib import Path',
"R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))",'import native_exact_roots','ADDRESS={']
for n,t in sorted(address.items(),key=lambda x:x[1]):
    note='' if n in idx else '  # derived from retail site; not in symbol_index'
    L.append(" %r:0x%x,%s"%(n,t,note))
L.append('}')
L.append("F=R/%r"%rel)
if gs:L.append("GS=['/nologo','/c','/O2','/GS','/MT','/W3']")
L.append('ROOTS=[')
for nm,sym,ad,sz,ni,nr in roots:
    L.append(" dict(name=%r,source=F,symbol=%r,address=0x%x,size=%d,instructions=%d,relocations=%d%s),"%(nm,sym,ad,sz,ni,nr,',flags=GS' if gs else ''))
L.append(']')
L+=['def main():',
" results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-%s.json',[],"%gname.replace('_','-'),
"  %r)"%desc,
" print('NATIVE_%s_PASS',' '.join(r['name']+'='+r['grade'] for r in results))"%tag,
"if __name__=='__main__':main()",'']
out=R/('tools/decomp_pipeline/gate_native_%s.py'%gname);out.write_text('\n'.join(L),encoding='utf-8')
print('WROTE',out)
print('        run("native %s", [PY, str(tools / "gate_native_%s.py")],'%(gname.replace('_',' '),gname))
print('            lambda o: "NATIVE_%s_PASS %s" in o),'%(tag,' '.join(nm+'=MATCH' for nm,*_ in roots)))
for nm,sym,ad,*_ in roots:print('ROOT\t%s\t%08x\t%s'%(rel,ad,sym))
