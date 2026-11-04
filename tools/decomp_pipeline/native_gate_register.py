"""native_gate_register.py gate_name ... (run after native_gate_generate.py; then symbol_index.py --only <TU stem>) : register native_exact_roots gates.
- symbol_index.py: dedicated providers, _verified_roots, companions (generated from gate ROOTS)
- gate_all.py: run(...) line expecting every root =MATCH (inserted after the drive-manager entry)
- function_overrides.tsv: one row per root (address, name, module, cc, evidence) if address absent
- ARTIFACT_INDEX.tsv: one row per source primary address if absent
Re-running it after regenerating a gate extends the TU's _verified_roots/companions and refreshes the
gate_all expectation (TUs indexed only through other providers are skipped with a warning).
Root dicts may carry 'name_override'=(name,module,cc) and 'reclaim'=False to skip claiming."""
import sys,importlib.util,re
from pathlib import Path
from collections import OrderedDict
R=Path('D:/Documents/FableTLC');T=R/'tools/decomp_pipeline';sys.path.insert(0,str(T))
def load(g):
    spec=importlib.util.spec_from_file_location('g_'+g,T/('gate_native_%s.py'%g));m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m
EOL={}
def rd(p):
    b=open(p,'rb').read();EOL[str(p)]=chr(13)+chr(10) if bytes([13,10]) in b else chr(10)
    return open(p,encoding='utf-8',newline=None).read()
def wr(p,s):open(p,'w',encoding='utf-8',newline=EOL.get(str(p),chr(10))).write(s)
def undec(sym):
    # ?Name@Class@@... -> Class::Name ; ??0Class@@ -> Class::Class ; ??_GClass -> Class::`scalar deleting destructor'
    m=re.match(r'\?\?0(\w+)@@',sym)
    if m:return m.group(1)+'::'+m.group(1),m.group(1)
    m=re.match(r'\?\?_G(\w+)@@',sym)
    if m:return m.group(1)+"::`scalar deleting destructor'",m.group(1)
    m=re.match(r'\?\?8(\w+)@@',sym)
    if m:return m.group(1)+'::operator==',m.group(1)
    m=re.match(r'\?(\w+)@(\w+)@@',sym)
    if m:return m.group(2)+'::'+m.group(1),m.group(2)
    m=re.match(r'\?(\w+)@\?\$(\w+)@',sym)
    if m:return m.group(2)+'<...>::'+m.group(1),m.group(2)
    if sym.startswith('??E'):return '_Rb_tree_iterator<...>::operator++','_STL'
    return sym,'_global'
gates=sys.argv[1:];files=OrderedDict();gate_lines=[];rows=[]
for g in gates:
    m=load(g)
    ok=' '.join(r['name']+'=MATCH' for r in m.ROOTS)
    src=rd(T/('gate_native_%s.py'%g));tag=re.search(r"print\('NATIVE_(\w+)_PASS'",src).group(1)
    gate_lines.append('        run("native %s", [PY, str(tools / "gate_native_%s.py")],\n            lambda o: "NATIVE_%s_PASS %s" in o),\n'%(g.replace('_',' '),g,tag,ok))
    for r in m.ROOTS:
        rel=str(Path(r['source']).relative_to(R/'rebuild/src/compiled')).replace('\\','/')
        if r.get('reclaim',True):files.setdefault(rel,[]).append(('%08x'%r['address'],r['symbol']))
        rows.append((r,rel,g))
# symbol_index.py
p=T/'symbol_index.py';s=rd(p)
def primary(rel):return rel.rsplit('_',1)[1][:8]
new=[rel for rel in files if ("'%s'"%rel) not in s]
ded=''.join("        ('%s', '%s'),\n"%(primary(rel),rel) for rel in new)
a1="        ('009cb710', '00/9c/NGameText_CDataBank_GetLastModifiedTime_009cb710.cpp'),\n";assert s.count(a1)==1;s=s.replace(a1,a1+ded)
vr=''.join("        %r: (%s),\n"%(rel,', '.join(repr(sym)for a,sym in files[rel])+(',' if len(files[rel])==1 else '')) for rel in new)
a2="    _verified_roots = {\n";assert s.count(a2)==1;s=s.replace(a2,a2+vr)
comp=''.join("        %r: {%s},\n"%(rel,', '.join('%r: %r'%(sym,a) for a,sym in files[rel] if a!=primary(rel))) for rel in new if any(a!=primary(rel) for a,sym in files[rel]))
a3="    companions = {\n";assert s.count(a3)==1;s=s.replace(a3,a3+comp)
# Extend TUs that already have a _verified_roots entry (a gate grown with new roots).
import ast
ext=0
for rel in files:
    if rel in new:continue
    key='        %r: ('%rel
    if s.count(key)!=1:print('SKIP-EXTEND (no single _verified_roots line):',rel);continue
    i=s.index(key);j=s.index(chr(10),i);roots=ast.literal_eval(s[i+len(key)-1:j].rstrip(','))
    add=tuple(sym for a,sym in files[rel] if sym not in roots)
    if add:
        roots=tuple(roots)+add;s=s[:i]+'        %r: (%s),'%(rel,', '.join(repr(x) for x in roots)+(',' if len(roots)==1 else ''))+s[j:];ext+=len(add)
    want={sym:a for a,sym in files[rel] if a!=primary(rel)}
    ckey='        %r: {'%rel
    if s.count(ckey)==1:
        i=s.index(ckey);j=s.index(chr(10),i);comp=ast.literal_eval(s[i+len(ckey)-1:j].rstrip(','))
        if any(comp.get(k)!=v for k,v in want.items()):
            comp.update(want);s=s[:i]+'        %r: {%s},'%(rel,', '.join('%r: %r'%kv for kv in comp.items()))+s[j:]
    elif want:
        a3="    companions = {\n";s=s.replace(a3,a3+'        %r: {%s},\n'%(rel,', '.join('%r: %r'%kv for kv in want.items())))
wr(p,s);print('symbol_index: +%d files, +%d roots on existing files'%(len(new),ext))
# gate_all.py
p=T/'gate_all.py';s=rd(p)
a='        run("native drive manager", [PY, str(tools / "gate_native_drive_manager.py")],\n'
assert s.count(a)==1;i=s.index(a);j=s.index('\n',s.index('\n',i)+1)+1
add=''
for l,g in zip(gate_lines,gates):
    head='        run("native %s", [PY, str(tools / "gate_native_%s.py")],\n'%(g.replace('_',' '),g)
    if head in s:
        # A regenerated gate: refresh its expected PASS line.
        i=s.index(head);k=s.index('\n',i+len(head))+1;s=s[:i]+l+s[k:]
    elif 'gate_native_%s.py'%g not in s:add+=l
s=s[:j]+add+s[j:];wr(p,s);print('gate_all: +%d'%add.count('run('))
# function_overrides.tsv
p=R/'rebuild/corrections/function_overrides.tsv';s=rd(p);have=set(l.split('\t')[0] for l in s.splitlines())
out=[]
for r,rel,g in rows:
    a='%08x'%r['address']
    if a in have or not r.get('reclaim',True):continue
    name,mod=r.get('name_override',undec(r['symbol']))[:2] if 'name_override' not in r else r['name_override'][:2]
    out.append('\t'.join([a,name,mod,'__thiscall' if '@@Q' in r['symbol'] or '@@A' in r['symbol'] or '@@U' in r['symbol'] else '','','','','%dB exact (gate native-%s).'%(r['size'],g.replace('_','-'))]))
if out:
    if not s.endswith('\n'):s+='\n'
    wr(p,s+'\n'.join(out)+'\n')
print('overrides: +%d'%len(out))
# ARTIFACT_INDEX.tsv
p=R/'rebuild/ARTIFACT_INDEX.tsv';s=rd(p);have=set(l.split('\t')[0] for l in s.splitlines())
out=[]
for rel in files:
    a=primary(rel)
    if a in have:continue
    out.append('\t'.join([a,'','','rebuild/src/compiled/'+rel,'','','']));have.add(a)
if out:
    if not s.endswith('\n'):s+='\n'
    wr(p,s+'\n'.join(out)+'\n')
print('artifact_index: +%d'%len(out))
