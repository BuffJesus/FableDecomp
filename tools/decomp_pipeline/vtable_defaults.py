#!/usr/bin/env python3
"""Re-land trivial virtual members that early landings left as flat stand-ins.

Early landings wrote some vtable defaults as free functions (`void __stdcall
CBankFile_NotifyBankSize(int, int) {}`): byte-exact, but they define no class symbol,
so nothing can link against them. This tool finds them and stages genuine members.

    python tools/decomp_pipeline/vtable_defaults.py scan  --out work/.../scan.json
    python tools/decomp_pipeline/vtable_defaults.py stage --scan work/.../scan.json --stage work/.../vtable-defaults-N --limit 60

scan:  catalog sources that define no `Class::Member` although the manifest names the address
       as a member; retail body is one of: `ret n` (empty), `xor eax,eax; ret n` (zero),
       `mov al,imm; ret n` (bool), `mov eax,imm; ret n` (const). The address must sit in at
       least one RTTI vtable; the implementer for each (class, slot) comes from Ego_r.pdb
       (vtable_slot_methods), and the owner is the manifest's class when it is one of them.
stage: writes `Owner::Member` bodies against the shared header that defines Owner, compiles
       each one, and keeps it only when the object defines Ego_r's decorated name and its
       bytes equal retail's. Writes spec.json (for gate_vtable_defaults.py) and a fixture
       that calls every member through a derived probe (protected members included).
"""
import argparse,collections,csv,json,re,struct,subprocess,sys,tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1]
sys.path.insert(0,str(HERE));sys.path.insert(0,str(HERE/'crawl'));sys.path.insert(0,str(ROOT/'tools'))
import pefile,check_one,purity
from gate_game_component_abi import Coff
from pe_oracle import EXE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
IB=0x400000
ENTRY=re.compile(r"\[pscustomobject\]@\{(.*?)\n\s*\}",re.S);FIELD=re.compile(r"(\w+)\s*=\s*'([^']*)'")
MEMBER_DEF=re.compile(r"^[^/#\n]*\b\w+(?:<[^>\n]*>)?::~?\w+\s*\(",re.M)
def catalog():
    text=(ROOT/'rebuild/build_candidates.ps1').read_text(encoding='utf-8')
    return [dict(FIELD.findall(m.group(1))) for m in ENTRY.finditer(text)]
def rtti_vtables(pe):
    text=[s for s in pe.sections if s.Name.startswith(b'.text')][0];T0=IB+text.VirtualAddress;T1=T0+text.Misc_VirtualSize
    rd=[s for s in pe.sections if s.Name.startswith(b'.rdata')][0];R0=IB+rd.VirtualAddress;data=rd.get_data()
    dword=lambda a:pe.get_dword_at_rva(a-IB)
    out={}
    for off in range(0,len(data)-8,4):
        col=struct.unpack_from('<I',data,off)[0]
        if not (R0<=col<R0+len(data)-16):continue
        try:
            if dword(col)!=0 or dword(col+4)!=0:continue
            name=pe.get_string_at_rva(dword(col+12)+8-IB)
        except Exception:continue
        if not name.startswith((b'.?AV',b'.?AU')):continue
        slots=[]
        for k in range(512):
            if off+8+4*k>len(data):break
            v=struct.unpack_from('<I',data,off+4+4*k)[0]
            if not T0<=v<T1:break
            slots.append(v)
        if slots:out[R0+off+4]=(name[4:].decode('latin1').rsplit('@@',1)[0],slots)
    return out
def pattern(pe,a):
    md=Cs(CS_ARCH_X86,CS_MODE_32);ins=[]
    for i in md.disasm(pe.get_data(a-IB,24),a):
        ins.append(i)
        if i.mnemonic in('ret','jmp'):break
    if not ins or ins[-1].mnemonic!='ret':return None
    pop=int(ins[-1].op_str,16) if ins[-1].op_str else 0;size=sum(i.size for i in ins)
    if len(ins)==1:return dict(kind='empty',pop=pop,size=size)
    if len(ins)==2:
        m,o=ins[0].mnemonic,ins[0].op_str
        if m=='xor' and o=='eax, eax':return dict(kind='zero',pop=pop,size=size,value=0)
        if m=='mov' and re.match(r'al, (0x[0-9a-f]+|\d+)$',o):return dict(kind='bool',pop=pop,size=size,value=int(o[4:],0))
        if m=='mov' and re.match(r'eax, (0x[0-9a-f]+|\d+)$',o):return dict(kind='const',pop=pop,size=size,value=int(o[5:],0))
    return None
def class_of(name):
    m=re.match(r'\?\w+@(\w+)@@',name)
    if m:return m.group(1)
    if '::' in name:return name.rsplit('::',1)[0].split('<')[0]
    return None
def scan(a):
    rows={r['address']:r for r in csv.DictReader(open(ROOT/'rebuild/manifest/functions.tsv',encoding='utf-8',errors='ignore'),delimiter='\t')}
    pe=pefile.PE(str(EXE),fast_load=True);vts=rtti_vtables(pe);refs=collections.defaultdict(list)
    for vt,(cls,slots) in vts.items():
        for k,v in enumerate(slots):refs['%08x'%v].append((cls,k))
    retail_slots={}
    for vt,(cls,slots) in sorted(vts.items()):retail_slots.setdefault(cls,slots)
    cands=[]
    for e in catalog():
        if e.get('Grade')=='asm_bake' or 'Source' not in e:continue
        src=ROOT/'rebuild/src/compiled'/e['Source']
        if not src.is_file():continue
        text=src.read_text(encoding='utf-8',errors='replace')
        if MEMBER_DEF.search(text) or not purity.is_genuine(text):continue
        addr=e['Address'].lower();name=rows.get(addr,{}).get('name','')
        if not class_of(name) or addr not in refs:continue
        p=pattern(pe,int(addr,16))
        if p:cands.append(dict(address=addr,manifest_name=name,source=e['Source'],test=e.get('TestSource',''),refs=refs[addr],**p))
    classes=sorted({c for x in cands for c,_ in x['refs']})
    tmp=Path(tempfile.mkdtemp())/'slots.json';listing=tmp.with_name('classes.txt');listing.write_text('\n'.join(classes)+'\n')
    r=subprocess.run([sys.executable,str(HERE/'vtable_slot_methods.py'),'--classes-file',str(listing),'--out',str(tmp)],capture_output=True,text=True)
    assert not r.returncode,r.stderr[-2000:]
    slots=json.loads(tmp.read_text())
    for x in cands:
        # Trust an implementer where the RTTI class itself declares the slot in Ego_r,
        # or where the implementer's own retail vtable has this address at that slot:
        # an unconfirmed inherited resolution can be wrong where retail overrides a
        # slot that Ego_r does not (CEngine's engine-interface overrides).
        impl={}
        for c,k in x['refs']:
            s=slots.get(c,{}).get(str(k))
            if not (s and s['decorated']):continue
            own=retail_slots.get(s['owner'])
            if s['owner']==c or (own and k<len(own) and own[k]==int(x['address'],16)):impl[(s['owner'],s['name'])]=s
        want=class_of(x['manifest_name'])
        pick=[s for (o,n),s in impl.items() if o.split('::')[-1]==want] or sorted(impl.values(),key=lambda s:s['owner'])
        x['implementers']=sorted({o for o,_ in impl});x['pick']=pick[0] if pick else None
    Path(a.out).write_text(json.dumps(cands,indent=1))
    print('candidates',len(cands),'resolved',sum(1 for x in cands if x['pick']),'kinds',collections.Counter(x['kind'] for x in cands))
SIG=re.compile(r'^(?:(public|protected|private): )?virtual (.+?) __thiscall ([\w:<>, ]+)::(~?\w+)\((.*)\)\s*(const)?\s*$')
def cxx(t):
    t=re.sub(r'\b(class|struct|enum|union) ','',t).strip()
    return '' if t=='void' else t
def split_params(s):
    out=[];depth=0;cur=''
    for ch in s:
        if ch=='<':depth+=1
        elif ch=='>':depth-=1
        if ch==',' and depth==0:out.append(cur);cur=''
        else:cur+=ch
    if cur.strip():out.append(cur)
    return [cxx(p) for p in out if cxx(p)]
INTEGRAL={'bool','char','signed char','unsigned char','short','unsigned short','int','unsigned int','long','unsigned long','__int64','unsigned __int64','wchar_t'}
def header_for(cls):
    pat=re.compile(r'^\s*(?:class|struct)\s+'+re.escape(cls)+r'\b[^;{]*\{',re.M);hits=[]
    for h in sorted((ROOT/'rebuild/include').rglob('*.h')):
        rel=h.relative_to(ROOT/'rebuild/include').as_posix()
        if rel.startswith(('candidates/','engine/_quarantine/','test_support/')):continue
        if pat.search(h.read_text(encoding='utf-8',errors='replace')):hits.append(rel)
    hits.sort(key=lambda r:(r.startswith('engine/'),r))
    return hits[0] if hits else None
def body(x,ret):
    if x['kind']=='empty':return '\n{\n}\n' if ret=='void' else None
    if ret=='void':return None
    v=x['value']
    if ret=='bool':
        if x['kind']=='const':return None
        return '\n{\n    return %s;\n}\n'%('true' if v&0xff else 'false')
    if ret.endswith('*'):
        return '\n{\n    return 0;\n}\n' if v==0 else None
    if ret in INTEGRAL:
        if x['kind']=='bool':return None
        return '\n{\n    return %s;\n}\n'%(hex(v) if v>9 else str(v))
    return None
def stage(a):
    cands=json.loads(Path(a.scan).read_text());S=Path(a.stage);S.mkdir(parents=True,exist_ok=True)
    env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];w=Path(tempfile.mkdtemp())
    pe=pefile.PE(str(EXE),fast_load=True);spec=[];skipped=collections.Counter();hdr_cache={};seen=set()
    done={p.stem.rsplit('_',1)[-1] for p in Path(ROOT/'work/codex-native-startup').glob('vtable-defaults-*/*_0*.cpp')}
    for x in cands:
        if len(spec)>=a.limit:break
        if x['address'] in done:skipped['already_staged']+=1;continue
        s=x['pick']
        if not s:skipped['no_implementer']+=1;continue
        m=SIG.match(s['signature'])
        if not m:skipped['signature']+=1;continue
        access,ret,owner,meth,params,const=m.groups();ret=cxx(ret) or 'void'
        if owner!=s['owner'] or '<' in owner:skipped['owner_form']+=1;continue
        if s['decorated'][0] in seen:skipped['duplicate']+=1;continue
        if access=='private':skipped['private']+=1;continue
        ps=split_params(params)
        if any(('&' not in p and '*' not in p and p not in INTEGRAL and p not in ('float','double')) for p in ps):skipped['by_value_param']+=1;continue
        b=body(x,ret)
        if b is None:skipped['body']+=1;continue
        if owner not in hdr_cache:hdr_cache[owner]=header_for(owner.split('::')[-1])
        hdr=hdr_cache[owner]
        if not hdr:skipped['no_header']+=1;continue
        short=owner.split('::')[-1]
        fname='%s_%s_%s.cpp'%(short,meth.lstrip('~'),x['address'])
        what={'empty':'does nothing','zero':'returns 0','bool':'returns %s'%('true' if x.get('value',0)&0xff else 'false'),'const':'returns %s'%(hex(x.get('value',0)))}[x['kind']]
        others=[o for o in x['implementers'] if o!=owner]
        text=('// %s::%s @ 0x%s (Ego_r %s; vtable slot of %s):\n// the default %s.%s\n#include "%s"\n\n%s %s::%s(%s)%s%s')%(
            owner,meth,x['address'].upper(),s['decorated'][0],', '.join(sorted({c for c,_ in x['refs']}))[:120],what,
            ('\n// Byte-identical bodies (/OPT:ICF) also serve: %s.'%', '.join(others)[:200]) if others else '',
            hdr,ret,owner,meth,', '.join(ps),' const' if const else '',b)
        src=w/fname;src.write_text(text);obj=w/(fname[:-4]+'.obj')
        r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(src)],env,w)
        if r.returncode:skipped['compile']+=1;continue
        c=Coff(obj);sym=s['decorated'][0]
        if sym not in c.symbols:skipped['decoration']+=1;continue
        sec,off=c.symbols[sym];bts,rel=c.sections[sec]
        if off or rel or bytes(bts)!=pe.get_data(int(x['address'],16)-IB,len(bts)):skipped['bytes']+=1;continue
        (S/fname).write_text(text);seen.add(sym)
        spec.append(dict(address=x['address'],file=fname,symbol=sym,owner=owner,method=meth,ret=ret,params=ps,const=bool(const),
                         access=access or 'public',kind=x['kind'],value=x.get('value'),pop=x['pop'],bytes=len(bts),header=hdr,
                         old_source=x['source'],old_test=x['test']))
    (S/'spec.json').write_text(json.dumps(spec,indent=1))
    print('staged',len(spec),'skipped',dict(skipped))
def fixture(a):
    S=Path(a.stage);spec=json.loads((S/'spec.json').read_text());name=a.name
    heads=sorted({x['header'] for x in spec})
    lines=['// Behaviour fixture for trivial virtual defaults re-landed as genuine members',
           '// (tools/decomp_pipeline/vtable_defaults.py). Each member is the real compiled',
           '// provider, called non-virtually on scratch storage with dummy arguments; it',
           '// must return its constant and leave the object and the arguments untouched.']
    lines+=['#include "%s"'%h for h in heads]+['#include <stdio.h>','#include <string.h>','',
           'static union { double align; unsigned char bytes[4096]; } g_object, g_objectBefore;',
           'static union { double align; unsigned char bytes[4096]; } g_scratch, g_scratchBefore;',
           'static int g_fail;','','int main()','{','    int cases = 0;','    for (int seed = 0; seed < 3; ++seed)','    {']
    for i,x in enumerate(spec):
        args=[]
        for ptype in x['params']:
            if ptype.endswith('&'):args.append('*reinterpret_cast<%s*>(&g_scratch)'%ptype[:-1].strip())
            elif ptype.endswith('*'):args.append('(%s)0'%ptype)
            elif ptype=='bool':args.append('seed != 0')
            elif ptype in('float','double'):args.append('0.5f * seed')
            else:args.append('(%s)(seed + 7)'%ptype)
        call='reinterpret_cast<%s%s*>(&g_object)->%s::%s(%s)'%('const ' if x['const'] else '',x['owner'],x['owner'],x['method'],', '.join(args))
        lines.append('        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + %d); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }'%i)
        lines.append('        g_objectBefore = g_object; g_scratchBefore = g_scratch;')
        if x['kind']=='empty':
            lines.append('        %s;'%call)
        else:
            want={'zero':'0','bool':'true' if (x['value'] or 0)&0xff else 'false'}.get(x['kind'],hex(x['value'] or 0))
            lines.append('        if (!(%s == %s)) { printf("VTABLE_DEFAULTS_FAIL %d value\\n"); g_fail = 1; }'%(call,want,i))
        lines.append('        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL %d state\\n"); g_fail = 1; }'%i)
        lines.append('        ++cases;')
    lines+=['    }','    if (g_fail) return 1;','    printf("VTABLE_DEFAULTS_BEHAVIOR_PASS scenarios=%d\\n", cases);','    return 0;','}']
    out=S/(name+'_test.cpp');out.write_text('\n'.join(lines)+'\n');print('fixture',out,3*len(spec),'scenarios')
def main():
    ap=argparse.ArgumentParser(description=__doc__);sub=ap.add_subparsers(dest='cmd',required=True)
    s=sub.add_parser('scan');s.add_argument('--out',required=True)
    t=sub.add_parser('stage');t.add_argument('--scan',required=True);t.add_argument('--stage',required=True);t.add_argument('--limit',type=int,default=60)
    f=sub.add_parser('fixture');f.add_argument('--stage',required=True);f.add_argument('--name',required=True)
    a=ap.parse_args();dict(scan=scan,stage=stage,fixture=fixture)[a.cmd](a)
if __name__=='__main__':main()
