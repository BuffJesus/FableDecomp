"""Remove reviewed compiler argument ghosts, preserving all Lua call operations."""
import hashlib
import re
from collections import Counter
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.readable_lua import tokens


def _code(source):
    """Mask Lua comments/literals without hiding code after a quoted `--`."""
    return ''.join(re.sub(r'[^\r\n]',' ',t[0]) if t.lastgroup in
                   ('comment','longcomment','string','longstring') else t[0] for t in tokens(source))


def restore(source,todo,fn,row,rdata,*,private_state_factory=None):
    if row is None:
        return source,list(todo),[]
    def require(ok,why):
        if not ok:raise ValueError(f'reviewed argument temporaries {fn["address"]}: {why}')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=rdata.bytes_at(start,end-start)
    require(raw is not None and hashlib.sha256(raw).hexdigest()==row['nativeSha256'],'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest()==row['sourceSha256'],'exported source changed')
    begin,limit=int(row['spanBegin'],16),int(row['spanEnd'],16)
    require(start<=begin<limit<=end and rdata.bytes_at(begin,limit-begin).hex()==row['spanBytes'],'argument instructions changed')
    indexed={i.address:i for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,start)}
    calls={int(call['site'],16):call for call in fn['indirectCalls']}
    for witness in row['calls']:
        site=int(witness['site'],16);ins=indexed.get(site);call=calls.get(site)
        require(ins is not None and ins.mnemonic=='call' and ins.bytes.hex()==witness['bytes'] and call is not None and
                call['slot'].lower()==witness['slot'].lower(),'argument call metadata changed')
    begin_text,end_text=row['scopeBegin'],row['scopeEnd']
    require(source.count(begin_text)==source.count(end_text)==1,'argument scope ambiguous')
    first=source.index(begin_text);last=source.index(end_text,first)
    scope=source[first:last]
    require(hashlib.sha256(scope.encode()).hexdigest()==row['luaScopeSha256'],'lifted argument scope changed')
    prefix=_code(source[:first])
    alias=row['uninitializedAlias'];require(re.fullmatch(r'[A-Za-z_]\w*',alias),'invalid alias')
    declarations=re.findall(r'(?m)^\s*local\s+([^\n=]+)',prefix)
    require(any(alias in [n.strip() for n in declaration.split(',')] for declaration in declarations) and
            len(re.findall(r'\b'+re.escape(alias)+r'\b',prefix))==1,'argument alias has an earlier use or write')
    updated=list(todo);recovered=[]
    expected=Counter(d for edit in row['edits'] for d in edit['diagnostics'])
    require(all(updated.count(d)==n for d,n in expected.items()),'diagnostic inventory changed')
    reads=[]
    for edit in row['edits']:
        old=edit['source'];require(scope.count(old)==1,'temporary block changed')
        if edit.get('kind')=='unusedPrivateStateRead':
            require(private_state_factory is not None and hashlib.sha256(private_state_factory.encode()).hexdigest()==row.get('privateStateFactorySha256'),'private state getter contract changed')
            require(not re.search(r'\b__native_entity_state\s*(?:[.:][A-Za-z_]\w*\s*)?=',_code(source)),'private state getter is rebound')
            match=re.fullmatch(r'\s*([A-Za-z_]\w*) = __native_entity_state:GetStateInt\("([A-Za-z0-9_]+)"\)\s*',old)
            require(match is not None,'not a supported private integer read')
            require(any(match[1] in [n.strip() for n in declaration.split(',')] for declaration in declarations),'private read target is not local')
            reads.append((old,match[1]));continue
        allowed={'if '+alias+' ~= nil then','end'}
        require(all(not line.strip() or line.strip().startswith('--') or line.strip() in allowed for line in old.splitlines()),'temporary edit would remove operations')
        control=[line.strip() for line in old.splitlines() if line.strip() and not line.strip().startswith('--')]
        require(control in ([],['if '+alias+' ~= nil then','end']),'temporary conditional is not empty and balanced')
        for diagnostic in edit['diagnostics']:
            updated.remove(diagnostic)
        scope=scope.replace(old,'',1);recovered.extend(edit['diagnostics'])
    for old,target in reads:
        # The getter is the generated private plain-table accessor. After the
        # empty scaffolding is gone,the target may be written but never read.
        current=source[:first]+scope+source[last:]
        position=current.index(old,first);tail=current[position+len(old):]
        clean=_code(tail)
        for occurrence in re.finditer(r'\b'+re.escape(target)+r'\b',clean):
            line_start=clean.rfind('\n',0,occurrence.start())+1
            require(re.match(r'\s*'+re.escape(target)+r'\s*=(?!=)',clean[line_start:]) is not None,'private read value is live')
            line_end=clean.find('\n',occurrence.end());line=clean[line_start:line_end if line_end>=0 else None]
            require(len(re.findall(r'\b'+re.escape(target)+r'\b',line))==1,'private read value is read by overwrite')
        edit=next(e for e in row['edits'] if e['source']==old)
        for diagnostic in edit['diagnostics']:updated.remove(diagnostic)
        recovered.extend(edit['diagnostics']);scope=scope.replace(old,'',1)
    require(recovered,'empty temporary inventory')
    result=source[:first]+scope+source[last:]
    invocation=re.compile(r'\b(?:quest|resources|me):[A-Za-z_]\w*\([^\n]*')
    require(invocation.findall(source)==invocation.findall(result),'call inventory changed')
    return result,updated,[{'nativeBegin':row['spanBegin'],'nativeEnd':row['spanEnd'],
                            'removedDiagnostics':len(recovered),'scope':'compiler argument setup;API calls andlater aliases retained',
                            'note':'Reviewed native/source/scoped-Lua evidence;empty-actor engine behavior remains a separate gate.'}]
