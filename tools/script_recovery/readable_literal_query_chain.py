"""Fold the complete nested literal-query constructor-flag shape.

Keep every local declaration and nested hero-handle scope, and every ordered
lookup/query. Only closed integer flag staging and Boolean materialization move.
The final query is explicitly truth-coerced, never returned as an arbitrary value.
"""
import re
from tools.script_recovery import readable_style as S
from tools.script_recovery.lua_binding_guard import ambiguous_bindings
from tools.script_recovery.readable_initial_literals import _binding_counts
from tools.script_recovery.readable_lua import tokens

NAME=r'[A-Za-z_]\w*'
HAZARDS={'_ENV','debug','getfenv','setfenv','load','loadstring','getlocal','setlocal','getupvalue','setupvalue','upvaluejoin'}

def fold(source):
    whole=list(tokens(source))
    last=next((t for t in reversed(whole) if t.lastgroup not in {'space','comment','longcomment'}),None)
    if last is None or last[0]!='end':return source,None
    suffix=source[last.end():];core=source[:last.end()]
    stream=list(tokens(core));ids=[t[0] for t in stream if t.lastgroup=='identifier']
    if set(ids)&HAZARDS or ids.count('function')!=1:return source,None
    if any(t.lastgroup in {'comment','longcomment'} or t.lastgroup in {'string','longstring'} and '\n' in t[0] for t in stream):return source,None
    if ambiguous_bindings(source):return source,None
    lines=core.splitlines(True);at=[i for i,l in enumerate(lines) if l.strip()];code=[lines[i].strip() for i in at]
    if len(code)<20:return source,None
    header=re.fullmatch(r'(?:local )?function '+NAME+r'\((?P<args>[\w, ]*)\)',code[0])
    declaration=re.fullmatch(r'local (?P<flags>'+NAME+r'), (?P<scratch>'+NAME+r'), (?P<result>'+NAME+r')',code[1])
    if not header or not declaration:return source,None
    params={p.strip() for p in header['args'].split(',')}
    flags,scratch,result=[declaration[k] for k in ['flags','scratch','result']]
    if len({flags,scratch,result})!=3 or {flags,scratch,result}&params:return source,None
    bindings=_binding_counts(stream)
    if any(bindings[n]!=1 for n in [flags,scratch,result]):return source,None
    cursor=2;queries=[];receiver=None
    while cursor+2<len(code):
        number=len(queries)+1
        if code[cursor]!=flags+' = '+str((1<<number)-1):break
        getter=re.fullmatch(r'local (?P<hero>'+NAME+r') = (?P<receiver>'+NAME+r'):GetHero\(\)',code[cursor+1])
        if not getter or getter['receiver'] not in params:break
        if receiver is None:receiver=getter['receiver']
        if getter['receiver']!=receiver:break
        hero=getter['hero']
        if hero in params|{flags,scratch,result} or bindings[hero]!=1 or hero in [q['hero'] for q in queries]:break
        call=receiver+':IsWearingHairstyle('+hero+', '
        if code[cursor+2]==result+' = false':
            if cursor+3>=len(code):break
            condition=re.fullmatch(r'if not (?P<query>'+re.escape(call)+r'.+\)) then goto (?P<label>'+NAME+r') end',code[cursor+3])
            if not condition:break
            query=condition['query'];literal=query[len(call):-1]
            literal_tokens=[t for t in tokens(literal) if t.lastgroup!='space']
            if len(literal_tokens)!=1 or literal_tokens[0].lastgroup!='string':break
            queries.append(dict(hero=hero,getter=code[cursor+1],query=query,literal=literal,final=True,label=condition['label']))
            cursor+=4;break
        condition=re.fullmatch(r'if not (?P<query>'+re.escape(call)+r'.+\)) then',code[cursor+2])
        if not condition:break
        query=condition['query'];literal=query[len(call):-1]
        literal_tokens=[t for t in tokens(literal) if t.lastgroup!='space']
        if len(literal_tokens)!=1 or literal_tokens[0].lastgroup!='string':break
        queries.append(dict(hero=hero,getter=code[cursor+1],query=query,literal=literal,final=False))
        cursor+=3
    n=len(queries)
    if not 2<=n<=16 or not queries[-1].get('final'):return source,None
    label=queries[-1]['label']
    expected=['end']*(n-1)+[result+' = true','::'+label+'::']
    for bit in range(n-1,0,-1):
        target=scratch if bit==1 else flags
        expected+=['if ('+flags+' & '+str(1<<bit)+') ~= 0 then',target+' = '+flags+' & '+str(255^(1<<bit)),'end']
    expected+=['return '+result,'end']
    if code[cursor:]!=expected:return source,None
    # The source clears uint8 constructor flags; a longer pattern would require
    # a different mask domain. Preserve it rather than invent integer facts.
    if n>8:return source,None
    flow=S.Flow(lines)
    if not flow.ok:return source,None
    indent=lines[at[2]][:len(lines[at[2]])-len(lines[at[2]].lstrip())]
    if not indent:return source,None
    output=[code[0],indent+code[1],indent+result+' = true']
    for i,q in enumerate(queries):
        level=indent*(i+1)
        output.append(level+q['getter'])
        if i==n-1:output.append(level+result+' = not not '+q['query'])
        else:output.append(level+'if not '+q['query']+' then')
    for level in range(n-1,0,-1):output.append(indent*level+'end')
    output+=[indent+'return '+result,'end']
    after='\n'.join(output)+suffix
    proof=dict(kind='nested_literal_query_boolean',function=code[0],receiver=receiver,queries=queries,queryCount=n,localDeclarationsRetained=True,lookupScopesAndOrderRetained=True,noLookupHoisted=True,returnBooleanAndArityPreserved=True,noHostReturnTypePremise=True,removedConstructorFlagStores=n,removedIntegerMaskGroups=n-1,removedGotos=1,sourceLines=len(lines),resultLines=len(after.splitlines()),scalarProof='flags:initial nil then exact uint8 constants/AND only;scratch:initial nil then private integer mask;predicate:only true/false;no API use,capture,reflection or other binding')
    return after,proof
