"""Fold a complete nested query with private Boolean construction bookkeeping.

Keep declarations and lookup scopes. Match every assignment and empty test;
resource cleanup or any unknown statement makes the complete shape refuse.
"""
import re
from tools.script_recovery.readable_lua import tokens
from tools.script_recovery.readable_initial_literals import _binding_counts
from tools.script_recovery.lua_binding_guard import ambiguous_bindings
NAME=r'[A-Za-z_]\w*'
HAZARDS={'_ENV','debug','getfenv','setfenv','load','loadstring','getlocal','setlocal','getupvalue','setupvalue','upvaluejoin'}

def fold(source):
    stream=tokens(source)
    last=next((t for t in reversed(stream) if t.lastgroup not in {'space','comment','longcomment'}),None)
    if last is None or last[0]!='end':return source,None
    core=source[:last.end()];suffix=source[last.end():];stream=tokens(core)
    ids=[t[0] for t in stream if t.lastgroup=='identifier']
    if ids.count('function')!=1 or set(ids)&HAZARDS or ambiguous_bindings(source):return source,None
    if any(t.lastgroup in {'comment','longcomment'} for t in stream):return source,None
    lines=core.splitlines();code=[line.strip() for line in lines if line.strip()]
    if len(code)<20:return source,None
    header=re.fullmatch(r'(?:local )?function '+NAME+r'\((?P<args>[\w, ]*)\)',code[0])
    if not header:return source,None
    params={x.strip() for x in header['args'].split(',')};bindings=_binding_counts(stream)
    cursor=1;declarations=[];declared=[]
    while cursor<len(code):
        match=re.fullmatch(r'local ('+NAME+r'(?:, '+NAME+r')*)',code[cursor])
        if not match:break
        declarations.append(code[cursor]);declared+=match[1].split(', ');cursor+=1
    if not declared or len(set(declared))!=len(declared) or set(declared)&params:return source,None
    flags=[];start=cursor
    while cursor<len(code):
        match=re.fullmatch(r'('+NAME+r') = false',code[cursor])
        if not match:break
        flags.append(match[1]);cursor+=1
    n=len(flags)+1
    if not 2<=n<=16 or len(set(flags))!=len(flags) or not set(flags)<=set(declared):return source,None
    results=set(declared)-set(flags)
    if len(results)!=1 or any(bindings[x]!=1 for x in declared):return source,None
    result=next(iter(results));queries=[];receiver=None
    for depth in range(n):
        if depth:
            expected=[flag+' = '+('true' if j<depth else 'false') for j,flag in enumerate(flags)]
            if code[cursor:cursor+len(expected)]!=expected:return source,None
            cursor+=len(expected)
        if cursor>=len(code):return source,None
        getter=re.fullmatch(r'local (?P<hero>'+NAME+r') = (?P<receiver>'+NAME+r'):GetHero\(\)',code[cursor])
        if not getter or getter['receiver'] not in params:return source,None
        hero=getter['hero']
        if receiver is None:receiver=getter['receiver']
        if getter['receiver']!=receiver or hero in params|set(declared) or bindings[hero]!=1 or hero in [q['hero'] for q in queries]:return source,None
        getter_line=code[cursor];cursor+=1
        if depth==n-1:
            if cursor>=len(code) or code[cursor]!=result+' = false':return source,None
            cursor+=1
        if cursor>=len(code):return source,None
        call=receiver+':IsWearingHairstyle('+hero+', '
        pattern=r'if not (?P<query>'+re.escape(call)+r'.+\)) then'
        if depth==n-1:pattern+=r' goto (?P<label>'+NAME+r') end'
        condition=re.fullmatch(pattern,code[cursor])
        if not condition:return source,None
        query=condition['query'];literal=query[len(call):-1];ts=[t for t in tokens(literal) if t.lastgroup!='space']
        if len(ts)!=1 or ts[0].lastgroup!='string' or '\n' in literal:return source,None
        queries.append(dict(hero=hero,getter=getter_line,query=query,literal=literal));cursor+=1
        if depth==n-1:label=condition['label']
    expected=['end']*(n-1)+[result+' = true','::'+label+'::']
    for flag in reversed(flags):expected+=['if '+flag+' then','end']
    expected+=['return '+result,'end']
    if code[cursor:]!=expected:return source,None
    first=next((l for l in lines[1:] if l.strip()),'');indent=first[:len(first)-len(first.lstrip())]
    if not indent:return source,None
    out=[code[0]]+[indent+d for d in declarations]+[indent+result+' = true']
    for depth,q in enumerate(queries):
        prefix=indent*(depth+1);out.append(prefix+q['getter'])
        out.append(prefix+(result+' = not not '+q['query'] if depth==n-1 else 'if not '+q['query']+' then'))
    out += [indent*level+'end' for level in range(n-1,0,-1)]+[indent+'return '+result,'end']
    after='\n'.join(out)+suffix
    proof=dict(kind='nested_boolean_query_chain',function=code[0],queryCount=n,queries=queries,privateFlags=flags,removedBooleanStores=n*(n-1),removedEmptyTests=n-1,removedGotos=1,localDeclarationsRetained=True,lookupScopesAndOrderRetained=True,returnBooleanAndArityPreserved=True,noHostReturnTypePremise=True,scalarProof='Private unique Boolean bindings,only literal assignments and empty condition tests;no API use,capture or reflection')
    return after,proof
