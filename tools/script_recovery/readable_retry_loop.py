"""Structure the emitter's complete two-history random retry/return shape.

Preserve all reachable calls, comparisons, field writes, return expressions and
local scopes. Reject partial matches, comments, closures and introspection.
Only graph-proven unreachable trailing frame/termination calls are removed.
"""
import re
from tools.script_recovery import readable_style as S
from tools.script_recovery.lua_binding_guard import ambiguous_bindings
from tools.script_recovery.readable_lua import tokens

NAME=r'[A-Za-z_]\w*'
HAZARDS={'_ENV','debug','getfenv','setfenv','load','loadstring','getlocal','setlocal','getupvalue','setupvalue','upvaluejoin'}

def restructure(source):
    stream=list(tokens(source));ids={t[0] for t in stream if t.lastgroup=='identifier'}
    if ids & HAZARDS or any(t.lastgroup in {'comment','longcomment'} for t in stream):return source,None
    if any(t.lastgroup in {'string','longstring'} and '\n' in t[0] for t in stream):return source,None
    if sum(t[0]=='function' and t.lastgroup=='identifier' for t in stream)!=1:return source,None
    if ambiguous_bindings(source):return source,None
    lines=source.splitlines(True);at=[i for i,l in enumerate(lines) if l.strip()];code=[lines[i].strip() for i in at]
    if len(code)!=30:return source,None
    header=re.fullmatch(r'(?P<header>(?:local )?function '+NAME+r'\((?P<args>[\w, ]*)\))',code[0])
    initial=re.fullmatch(r'local (?P<predicate>'+NAME+r') = (?P<receiver>'+NAME+r'):IsActiveThreadTerminating\(\)',code[1])
    draw=re.fullmatch(r'local (?P<value>'+NAME+r') = (?P<receiver>'+NAME+r'):RetailRandModulo\((?P<modulus>[1-9]\d*)\)',code[7])
    if not header or not initial or not draw or draw['receiver']!=initial['receiver']:return source,None
    predicate=initial['predicate'];receiver=initial['receiver'];value=draw['value']
    params={p.strip() for p in header['args'].split(',')}
    if receiver not in params or len({predicate,receiver,value})!=3 or predicate in params or value in params:return source,None
    retry=re.fullmatch(r'if not \(\('+re.escape(value)+r' ~= (?P<first>'+NAME+r')\) and \('+re.escape(value)+r' ~= (?P<second>'+NAME+r')\)\) then',code[8])
    if not retry:return source,None
    first,second=retry['first'],retry['second']
    if first==second or {first,second} & (params|{predicate,value}):return source,None
    top=re.fullmatch(r'::(?P<label>'+NAME+r')::',code[6])
    continuation=re.fullmatch(r'::(?P<label>'+NAME+r')::',code[27])
    frame=re.fullmatch(re.escape(receiver)+r':NewScriptFrame\((?P<thing>'+NAME+r')\);',code[9])
    if not top or not continuation or top['label']==continuation['label'] or not frame or frame['thing'] not in params:return source,None
    term=receiver+':IsActiveThreadTerminating()';frame_call=code[9].removesuffix(';')
    default=re.fullmatch(r'return (?P<value>.+)',code[4])
    result=re.fullmatch(r'do return (?P<value>.+) end;',code[20])
    if not default or not result:return source,None
    default_expr=default['value'];result_expr=result['value']
    default_tokens=[t for t in tokens(default_expr) if t.lastgroup!='space']
    if len(default_tokens)!=1 or not (default_tokens[0].lastgroup in {'string','number'} or default_expr in {'nil','false','true'}):return source,None
    expected={2:'repeat',3:'if '+predicate+' then',5:'end',10:predicate+' = '+term+';',11:'goto '+continuation['label'],12:'end',13:'if '+term+' then',14:'do return '+default_expr+' end;',15:'goto '+top['label'],16:'end',17:second+' = '+first,18:first+' = '+value,19:'if not '+term+' then',21:'goto '+top['label'],22:'end',23:'do return '+default_expr+' end',24:'goto '+top['label'],25:frame_call,26:predicate+' = '+term,28:'until false',29:'end'}
    if any(code[i]!=text for i,text in expected.items()):return source,None
    flow=S.Flow(lines)
    if not flow.ok:return source,None
    reachable=set();pending=[at[0]]
    while pending:
        i=pending.pop()
        if i in reachable:continue
        reachable.add(i);pending.extend(flow.graph.get(i,()))
    if any(at[i] in reachable for i in [15,21,24,25,26]):return source,None
    # Keep the source expression's parentheses: they can force one return
    # value from a host function that supplies several values.
    indent=lines[at[1]][:len(lines[at[1]])-len(lines[at[1]].lstrip())]
    if not indent or indent.strip():return source,None
    accepted='('+value+' ~= '+first+') and ('+value+' ~= '+second+')'
    output=[code[0],indent+code[1],indent+'while not '+predicate+' do',indent*2+code[7],indent*2+'if '+accepted+' then',indent*3+'if '+term+' then return '+default_expr+' end',indent*3+code[17],indent*3+code[18],indent*3+'if not '+term+' then return '+result_expr+' end',indent*3+'return '+default_expr,indent*2+'end',indent*2+frame_call,indent*2+predicate+' = '+term,indent+'end',indent+'return '+default_expr,'end']
    trailing=''.join(lines[at[-1]+1:])
    after='\n'.join(output)+'\n'+trailing
    proof=dict(kind='structured_random_retry',function=code[0],receiver=receiver,predicate=predicate,draw=value,history=[first,second],modulus=int(draw['modulus']),returnExpression=result_expr,returnExpressionExact=True,unreachableRemoved=[dict(line=at[i]+1,statement=code[i]) for i in [15,21,24,25,26]],reachableCallsAndStateOrderPreserved=True,sourceLines=len(lines),resultLines=len(after.splitlines()),hostTypeContractRequired=False)
    return after,proof
