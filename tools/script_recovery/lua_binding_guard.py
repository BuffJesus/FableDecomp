"""Conservative lexical guard for generated, line-oriented Lua transformations."""
import re
from tools.script_recovery.readable_lua import tokens

KEYWORDS=set('and break do else elseif end false for function goto if in local nil not or repeat return then true until while'.split())

def ambiguous_bindings(source):
    """Names used globally as well as declared locally must not be textually rewritten.

    Unsupported continued/compound local declarations fail closed. This is a
    preservation guard, not a general Lua parser or a variable renamer.
    """
    stream=[t for t in tokens(source) if t.lastgroup not in ('space','comment','longcomment','string','longstring')]
    candidates=set()
    for i,t in enumerate(stream):
        if t[0]=='local':
            j=i+1
            if j<len(stream) and stream[j][0]=='function':j+=1
            while j<len(stream) and stream[j].lastgroup=='identifier':
                candidates.add(stream[j][0]);j+=1
                if j==len(stream) or stream[j][0]!=',':break
                j+=1
        if t[0]=='function':
            j=i+1
            while j<len(stream) and stream[j][0]!='(':j+=1
            while j<len(stream) and stream[j][0]!=')':
                if stream[j].lastgroup=='identifier':candidates.add(stream[j][0])
                j+=1
    scopes=[set()];free=set();pending=[];loop_names=set();table_depth=0;paren_depth=0;deferred=[];scope_kinds=[];function_depth=0;table_functions=[]
    lines=source.splitlines(True);offset=0;cursor=0
    for line in lines:
        end=offset+len(line);ts=[]
        while cursor<len(stream) and stream[cursor].start()<end:
            ts.append(stream[cursor]);cursor+=1
        offset=end
        if not ts:continue
        pending=[];declaration_scope=None;declaration_parens=0;declaration_tables=0;pop_repeat=False;i=0
        while i<len(ts):
            t=ts[i];word=t[0];prev=ts[i-1][0] if i else '';nxt=ts[i+1][0] if i+1<len(ts) else ''
            if word=='local':
                # The emitter's array initializer contains an inline empty-table
                # local followed by a for loop. Its initializer has no reads.
                if (i+4<len(ts) and ts[i+1].lastgroup=='identifier'
                        and [x[0] for x in ts[i+2:i+5]]==['=','{','}']
                        and (i+5==len(ts) or ts[i+5][0] in (';','for','return','end'))):
                    scopes[-1].add(ts[i+1][0]);i+=5;continue
                if i!=0 or ';' in [x[0] for x in ts]:return candidates
                if nxt=='function':
                    if i+2>=len(ts):return candidates
                    scopes[-1].add(ts[i+2][0]);i+=1;continue
                declaration_scope=scopes[-1];declaration_parens=paren_depth;declaration_tables=table_depth
                i+=1
                while i<len(ts) and ts[i].lastgroup=='identifier':
                    pending.append(ts[i][0]);i+=1
                    if i==len(ts) or ts[i][0]!=',':break
                    i+=1
                continue
            if word=='function':
                j=i+1
                if j<len(ts) and ts[j].lastgroup=='identifier':
                    if not any(ts[j][0] in scope for scope in scopes):free.add(ts[j][0])
                while j<len(ts) and ts[j][0]!='(':j+=1
                if j==len(ts):return candidates
                params=set();j+=1
                while j<len(ts) and ts[j][0]!=')':
                    if ts[j].lastgroup=='identifier':params.add(ts[j][0])
                    j+=1
                if j==len(ts):return candidates
                scopes.append(params);scope_kinds.append('function');function_depth+=1;i=j+1;continue
            if word=='for':
                i+=1;loop_names=set()
                while i<len(ts) and ts[i].lastgroup=='identifier' and ts[i][0]!='in':
                    loop_names.add(ts[i][0]);candidates.add(ts[i][0]);i+=1
                    if i==len(ts) or ts[i][0]!=',':break
                    i+=1
                continue
            if word in ('end','else','elseif'):
                if len(scopes)==1:return candidates
                scopes.pop()
                if scope_kinds.pop()=='function':function_depth-=1
                if word=='else':scopes.append(set());scope_kinds.append('block')
            elif word in ('then','do','repeat'):
                scopes.append(loop_names if word=='do' else set());scope_kinds.append('block');loop_names=set()
            elif word=='until':pop_repeat=True
            elif word=='(':paren_depth+=1
            elif word==')':paren_depth-=1
            elif word=='{':table_depth+=1;table_functions.append(function_depth)
            elif word=='}':
                if not table_functions:return candidates
                table_depth-=1;table_functions.pop()
            elif t.lastgroup=='identifier' and word not in KEYWORDS:
                key=table_depth>0 and table_functions[-1]==function_depth and prev in ('{',',',';') and nxt=='='
                if prev not in ('.',':','goto','::') and nxt!='::' and not key:
                    if not any(word in scope for scope in scopes):free.add(word)
            i+=1
        if pending:
            deferred.append((declaration_scope,declaration_parens,declaration_tables,pending))
        still_pending=[]
        for scope,depth,tables,names in deferred:
            if scopes[-1] is scope and paren_depth==depth and table_depth==tables:
                scope.update(names)
            else:
                still_pending.append((scope,depth,tables,names))
        deferred=still_pending
        if pop_repeat:
            if len(scopes)==1:return candidates
            scopes.pop();scope_kinds.pop()
    if len(scopes)!=1 or deferred or paren_depth!=0 or table_depth!=0:return candidates
    return candidates & free
