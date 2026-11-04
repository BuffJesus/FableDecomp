"""native_compare.py <source.cpp> [--gs] [/flag ...] sym=hexaddr ...   (sym may be a substring of the mangled name)
Set QFE=1 to compile with the QFE-4035 toolset; set MSYS_NO_PATHCONV=1 when passing /I from Git Bash.
Compiles with canonical flags, reports size/reloc/byte match per symbol, shows an aligned diff on mismatch."""
import sys,re,difflib,tempfile
from pathlib import Path
R=Path(r'D:\Documents\FableTLC');sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
args=sys.argv[1:];src=Path(args.pop(0)).resolve()
flags=['/nologo','/c','/O2','/MT','/W3']
extra=[a for a in args if a.startswith('/')];args=[a for a in args if not a.startswith('/')]
if '--gs' in args:args.remove('--gs');flags.insert(3,'/GS')
flags+=extra
b=Path(tempfile.mkdtemp());obj=b/'t.obj'
import os
e=V.env()
if os.environ.get('QFE')=='1':
    V.VC=Path(r'D:\Tools\vc71-qfe4035');e['PATH']=str(V.VC/'bin')+';'+e['PATH'];print('[QFE-4035]')
r=V.cl(flags+['/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(src)],e,b)
if r.returncode:print(r.stdout+r.stderr);sys.exit(1)
c=Coff(obj);raw=EXE.read_bytes();md=Cs(CS_ARCH_X86,CS_MODE_32)
Z=bytes(4)
def norm(x):
    if (x.mnemonic=='call' or x.mnemonic.startswith('j')) and x.op_str.startswith('0x'):return x.mnemonic+' X'
    return re.sub(r'0x[0-9a-f]{5,8}','X',x.mnemonic+' '+x.op_str)
if not args:
    for s in c.symbols:
        if s.startswith('?') or s.startswith('_'):print(s)
for a in args:
    pat,addr=a.rsplit('=',1);addr=int(addr,16)
    names=[s for s in c.symbols if pat==s] or [s for s in c.symbols if pat in s and c.symbols[s][1]==0 and len(c.sections[c.symbols[s][0]][0])>0]
    if not names:print('NOSYM',pat);continue
    s=names[0];sec,off=c.symbols[s];built,rels=c.sections[sec];orig=read_virtual_data(raw,addr,len(built))
    bb=bytearray(built);oo=bytearray(orig)
    for site in rels:bb[site:site+4]=Z;oo[site:site+4]=Z
    ok=bb==oo
    print(('MATCH' if ok else 'DIFF'),s,hex(addr),'size',len(built),'relocs',len(rels),sorted(set(n for n,k in rels.values())))
    if ok:continue
    A=[norm(x) for x in md.disasm(bytes(built),0)]
    B=[]
    for x in md.disasm(read_virtual_data(raw,addr,len(built)+64),addr):
        B.append(norm(x))
        if x.mnemonic=='ret' and x.address+x.size>=addr+len(built)-2:break
    neq=0
    for tag,i1,i2,j1,j2 in difflib.SequenceMatcher(None,A,B,autojunk=False).get_opcodes():
        if tag=='equal':
            for k in range(i1,i2):print('    '+A[k])
        else:
            for k in range(max(i2-i1,j2-j1)):
                x=A[i1+k] if i1+k<i2 else '';y=B[j1+k] if j1+k<j2 else ''
                neq+=1;print('!!  %-45s | %s'%(x,y))
    print('  diff lines',neq,'of',len(B))
