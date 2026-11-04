"""Drop dead literal staging only while a unique hoisted local is still nil.

Every incoming path must have no prior definition. A populated/unknown slot is
preserved regardless of whether its value is read, so retirement is not delayed.
No host API return-type assumption is used.
"""
from collections import Counter,deque
import re
from tools.script_recovery import readable_style as S
from tools.script_recovery.lua_binding_guard import ambiguous_bindings
from tools.script_recovery.readable_lua import tokens

HAZARDS={'_ENV','debug','getfenv','setfenv','load','loadstring','getlocal','setlocal','getupvalue','setupvalue','upvaluejoin'}
STORE=re.compile(r'[ \t]*(?P<name>[A-Za-z_]\w*) = (?P<value>false|true|0);?\n')

def incoming_definitions(flow,defs):
    incoming={i:set() for i in flow.graph}
    entry=next((i for i,line in enumerate(flow.structure) if line.strip()),None)
    if entry is None or entry not in incoming:return incoming
    incoming[entry].add(None);pending=deque([entry]);defs=set(defs)
    while pending:
        i=pending.popleft()
        output={i} if i in defs else incoming[i]
        for target in flow.graph[i]:
            if target not in incoming:return None
            before=len(incoming[target]);incoming[target].update(output)
            if len(incoming[target])!=before:pending.append(target)
    return incoming


def _binding_counts(stream):
    """Count initialized locals and formal parameters as well as bare locals."""
    code=[t for t in stream if t.lastgroup not in {'space','comment','longcomment','string','longstring'}]
    counts=Counter()
    for i,t in enumerate(code):
        if t[0]=='local':
            j=i+1
            if j<len(code) and code[j][0]=='function':
                if j+1<len(code):counts[code[j+1][0]]+=1
                continue
            while j<len(code) and code[j].lastgroup=='identifier':
                counts[code[j][0]]+=1;j+=1
                if j>=len(code) or code[j][0]!=',':break
                j+=1
        if t[0]=='function':
            j=i+1;implicit_self=False
            while j<len(code) and code[j][0]!='(':
                implicit_self=implicit_self or code[j][0]==':'
                j+=1
            if implicit_self:counts['self']+=1
            j+=1
            while j<len(code) and code[j][0]!=')':
                if code[j].lastgroup=='identifier':counts[code[j][0]]+=1
                j+=1
    return counts

def _safe_compound_prefixes(structure):
    """Limit the legacy Flow prefix projection to modeled simple effects.

    A loop/block/jump prefix can hide definitions or successors. Preserve the
    whole function rather than letting its text become a plain goto/return.
    """
    pattern=re.compile(r'^(?P<head>\s*(?:if .+ then )?)(?P<prefix>[^\n;]+); (?:return|goto \w+|break)(?: end)?$')
    name=r'[A-Za-z_]\w*'
    call=re.compile(name+r'(?:[.:]'+name+r')*\([^;\n]*\)')
    assignment=re.compile(name+r'\s*=(?!=)[^;\n]+')
    control={'if','then','else','elseif','while','repeat','until','for','function','local','goto','break','do','end','return'}
    for line in structure:
        match=pattern.fullmatch(line)
        if not match:continue
        prefix=match['prefix'].strip()
        identifiers=[t[0] for t in tokens(prefix) if t.lastgroup=='identifier']
        if re.fullmatch(r'do return(?:\s+.+)? end',prefix):
            middle=identifiers[2:-1]
            if not (set(middle)&control):continue
            return False
        if set(identifiers)&control:return False
        if not (call.fullmatch(prefix) or assignment.fullmatch(prefix)):return False
    return True

def prune(source):
    stream=list(tokens(source))
    ids=Counter(t[0] for t in stream if t.lastgroup=='identifier')
    if ids.keys() & HAZARDS or ids['function']!=1:return source,[]
    if any(t.lastgroup in {'string','longstring','longcomment'} and '\n' in t[0] for t in stream):return source,[]
    if ambiguous_bindings(source):return source,[]
    lines=source.splitlines(True)
    if not _safe_compound_prefixes(S._structure(lines)):return source,[]
    flow=S.Flow(lines)
    if not flow.ok:return source,[]
    # The supported graph uses one identity per label. Preserve nested-scope
    # label reuse rather than accepting a conflated graph.
    labels=[m[1] for line in flow.structure if (m:=re.fullmatch(r'\s*::(\w+)::\s*',line))]
    if len(set(labels))!=len(labels):return source,[]
    declared=Counter(S._declared_temporaries(flow.structure))
    bindings=_binding_counts(stream)
    proofs=[];deleted=set()
    for name,n in declared.items():
        if n!=1 or bindings[name]!=1 or name in flow.pinned:continue
        defs,reads=S._reads_and_defs(flow.structure,name)
        if ids[name]!=1+len(defs)+len(reads):continue
        incoming=incoming_definitions(flow,defs)
        if incoming is None:continue
        for d in defs:
            match=STORE.fullmatch(lines[d])
            if not match or match['name']!=name:continue
            if incoming[d]!={None}:continue
            if d in reads or S._read_reachable(flow,name,flow.graph[d],set(defs)):continue
            # Removing a statement separator must not join two adjacent Lua
            # calls through parenthesized/string/table argument sugar.
            following=''.join(lines[d+1:])
            next_token=next((t for t in tokens(following) if t.lastgroup not in {'space','comment','longcomment'}),None)
            if next_token is not None and next_token.lastgroup!='identifier' and next_token[0]!='::':continue
            deleted.add(d)
            proofs.append(dict(variable=name,line=d+1,value=match['value'],incoming='uninitialized local nil on every reachable path',unreadBeforeRedefinition=True,hostReturnContractRequired=False))
    return ''.join(line for i,line in enumerate(lines) if i not in deleted),proofs
