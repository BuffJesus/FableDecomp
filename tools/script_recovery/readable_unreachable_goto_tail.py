"""Remove only adjacent executable leaves after an unconditional standalone goto.

No shared CFG projection is used. A label, declaration, control statement,
annotation or unknown syntax stops the scan. Every declaration/scope stays put.
"""
import bisect,re
from tools.script_recovery.readable_lua import tokens
NAME=re.compile(r'[A-Za-z_][A-Za-z0-9_]*\Z')
HAZARDS={'_ENV','debug','getfenv','setfenv','load','loadstring','getlocal','setlocal','getupvalue','setupvalue','upvaluejoin'}
CONTROL={'function','if','then','else','elseif','end','do','for','while','repeat','until','goto','break','return','local','in'}
RESERVED=CONTROL|{'and','or','not','true','false','nil'}

def _call(ts):
    if not ts or ts[0].lastgroup!='identifier' or ts[0][0] in RESERVED:return False
    i=1
    while i+1<len(ts) and ts[i][0]=='.' and ts[i+1].lastgroup=='identifier':i+=2
    if i+1<len(ts) and ts[i][0]==':' and ts[i+1].lastgroup=='identifier':i+=2
    if i>=len(ts) or ts[i][0]!='(' or ts[-1][0]!=')':return False
    if any(t.lastgroup=='identifier' and (t[0] in RESERVED or not NAME.fullmatch(t[0])) for t in ts[:i]):return False
    args=ts[i+1:-1]
    if not args:return True
    groups=[[]]
    for t in args:
        if t[0]==',':groups.append([])
        else:groups[-1].append(t)
    for group in groups:
        if not group:return False
        raw=group[0].string[group[0].start():group[-1].end()]
        if len(group)==1 and group[0].lastgroup=='string' and '\\' not in raw:continue
        if raw in {'true','false','nil'} or re.fullmatch(r'-?[0-9]+',raw):continue
        if any(t.lastgroup=='identifier' and t[0] in RESERVED for t in group):return False
        if not re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*(?:\s*\.\s*[A-Za-z_][A-Za-z0-9_]*)*',raw):return False
    return True

def prune(source):
    stream=tokens(source);ids=[t[0] for t in stream if t.lastgroup=='identifier']
    if ids.count('function')!=1 or set(ids)&HAZARDS:return source,None
    if any(t.lastgroup!='space' and any(c in t[0] for c in '\r\n') for t in stream):return source,None
    lines=source.splitlines(True);starts=[];pos=0
    for line in lines:starts.append(pos);pos+=len(line)
    by_line=[[] for _ in lines]
    for t in stream:
        if t.lastgroup!='space':by_line[bisect.bisect_right(starts,t.start())-1].append(t)
    dropped=set();proofs=[];origin=None
    for i,ts in enumerate(by_line):
        if not ts:continue
        # Keep comments intact; they also conservatively end the candidate tail.
        if any(t.lastgroup in {'comment','longcomment'} for t in ts):origin=None;continue
        values=[t[0] for t in ts]
        is_goto=len(values)==2 and values[0]=='goto' and ts[1].lastgroup=='identifier' and NAME.fullmatch(values[1])
        if is_goto:
            # Keep every jump and its target diagnostics, even when unreachable.
            origin=dict(line=i+1,label=values[1])
            continue
        if origin is None:continue
        next_code=next((xs for xs in by_line[i+1:] if xs and not all(t.lastgroup in {'comment','longcomment'} for t in xs)),[])
        valid_return=values==['return'] and next_code and next_code[0][0] in {'end','else','elseif','until'}
        kind='unreachable_call' if _call(ts) else 'unreachable_return' if valid_return else None
        if kind:
            dropped.add(i);proofs.append(dict(line=i+1,statement=lines[i].strip(),afterUnconditionalGoto=origin,kind=kind))
        else:origin=None
    if not dropped:return source,None
    after=''.join(line for i,line in enumerate(lines) if i not in dropped)
    return after,dict(kind='adjacent_unreachable_goto_leaves',removed=proofs,noSharedCFGProjection=True,declarationsAndScopesRetained=True,labelsAndCommentsRetained=True,noHostTypeOrOwnershipPremise=True)
