"""Remove bare locals whose every occurrence is a declaration or nil-only write.

Never removes a clear of a potentially populated slot. This is conservative
token/lexical evidence on one generated function, not general Lua evaluation.
"""
from collections import Counter,defaultdict
import re
from tools.script_recovery.readable_lua import tokens
from tools.script_recovery.lua_binding_guard import ambiguous_bindings

NAME=r'[A-Za-z_]\w*'
NAMES=NAME+r'(?:[ \t]*,[ \t]*'+NAME+r')*'
DECLARE=re.compile(r'(?m)^(?P<indent>[ \t]*)local (?P<names>'+NAMES+r')[ \t]*\n')
CLEAR=re.compile(r'(?m)^(?P<indent>[ \t]*)(?P<names>'+NAMES+r')[ \t]*=[ \t]*(?P<values>nil(?:[ \t]*,[ \t]*nil)*)[ \t]*\n')
OPAQUE={'space','comment','longcomment','string','longstring'}

def prune(source):
    stream=tokens(source)
    # An implicit environment or introspective execution is outside this proof.
    identifiers={t[0] for t in stream if t.lastgroup=='identifier'}
    if identifiers & {'_ENV','debug','getfenv','setfenv','load','loadstring'}:
        return source,[]
    if any(t.lastgroup in {'string','longstring','longcomment'} and '\n' in t[0] for t in stream):
        return source,[]
    if ambiguous_bindings(source):return source,[]
    code=''.join(re.sub(r'[^\n]',' ',t[0]) if t.lastgroup in {'comment','longcomment','string','longstring'} else t[0] for t in stream)
    declarations=[];clears=[];declared=Counter();allowed=defaultdict(set);writes=defaultdict(list)
    def names(match):return [n.strip() for n in match['names'].split(',')]
    def positions(match):
        return [(m[0],match.start('names')+m.start()) for m in re.finditer(NAME,match['names'])]
    for m in DECLARE.finditer(code):
        if source[m.start():m.end()]!=m[0] or code[m.end():].lstrip().startswith((',','=')):continue
        declarations.append(m)
        for name,at in positions(m):declared[name]+=1;allowed[name].add(at)
    for m in CLEAR.finditer(code):
        targets=names(m)
        if source[m.start():m.end()]!=m[0] or len(targets)!=len(set(targets)) or len(targets)!=len(m['values'].split(',')):continue
        # A following comma may continue the RHS list, and an operator may
        # continue the final nil expression. Only a complete physical statement
        # followed by an unambiguous statement boundary is supported.
        following=code[m.end():].lstrip()
        first=next(iter(tokens(following)),None)
        if first is not None and (first[0] in {',','.','[','(',':','+','-','*','/','//','%','^','&','|','~','<','>','==','~=','<=','>=','..','and','or'}):continue
        clears.append(m)
        for name,at in positions(m):allowed[name].add(at);writes[name].append(m.start())
    occurrences=defaultdict(set)
    for t in stream:
        if t.lastgroup=='identifier':occurrences[t[0]].add(t.start())
    dead={name for name,n in declared.items() if n==1 and writes[name] and occurrences[name]==allowed[name]}
    if not dead:return source,[]
    edits=[]
    for m in declarations+clears:
        old=names(m);new=[n for n in old if n not in dead]
        if old==new:continue
        replacement=''
        if new:
            replacement=m['indent']+('local ' if m.re is DECLARE else '')+', '.join(new)
            if m.re is CLEAR:replacement+=' = '+', '.join(['nil']*len(new))
            replacement+='\n'
        edits.append((m.start(),m.end(),replacement))
    edits.sort()
    assert all(a[1]<=b[0] for a,b in zip(edits,edits[1:]))
    out=source
    for a,b,text in reversed(edits):out=out[:a]+text+out[b:]
    return out,[dict(name=n,nilWrites=writes[n],allTokenOccurrencesAccountedFor=True,nonNilWrites=0,reads=0) for n in sorted(dead)]
