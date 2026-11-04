"""Expose compound conditional-jump statements without changing Lua tokens.

Only physical whitespace changes. Semicolons remain at their original statement
boundaries; removing them can change Lua call parsing even across newlines.
"""
import re
from tools.script_recovery.readable_lua import tokens

NAME=r'[A-Za-z_]\w*'
CALL=NAME+r'(?:[.:]'+NAME+r')+\([^;\n]*\)'
PATTERN=re.compile(r'(?P<indent>[ \t]*)if (?P<condition>.+) then (?P<call>'+CALL+r'); (?P<assignment>'+NAME+r' = '+CALL+r'); goto (?P<label>'+NAME+r') end(?P<tail>[ \t]*(?:--[^\n]*)?)\n')
RETURN_JUMP=re.compile(r'(?P<indent>[ \t]*)if (?P<condition>.+) then (?P<terminal>do return[^\n]* end); goto (?P<label>'+NAME+r') end(?P<tail>[ \t]*(?:--[^\n]*)?)\n')
IGNORED={'space','comment','longcomment'}
HAZARDS={'_ENV','debug','getfenv','setfenv','load','loadstring','getlocal','setlocal','getupvalue','setupvalue','upvaluejoin'}

def code_tokens(source):
    return [(t.lastgroup,t[0]) for t in tokens(source) if t.lastgroup not in IGNORED]

def expand(source):
    stream=list(tokens(source))
    if {t[0] for t in stream if t.lastgroup=='identifier'} & HAZARDS:return source,[]
    if any(t.lastgroup in {'string','longstring','longcomment'} and '\n' in t[0] for t in stream):return source,[]
    # Match the token-masked structure, then preserve literal text by slicing
    # the raw line at exact token offsets. A string containing `;` or `then`
    # cannot manufacture a statement boundary or change the selected span.
    masked=''.join(re.sub(r'[^\n]',' ',t[0]) if t.lastgroup in {'string','longstring','comment','longcomment'} else t[0] for t in stream)
    raw=source.splitlines(True);structure=masked.splitlines(True)
    if len(raw)!=len(structure):return source,[]
    result=list(raw);proofs=[]
    for i,line in enumerate(structure):
        m=PATTERN.fullmatch(line) or RETURN_JUMP.fullmatch(line)
        if not m:continue
        # Comments/literals are never cut at a boundary or reinterpreted.
        r=raw[i];indent=m['indent'];deeper=indent+'    '
        condition=r[m.start('condition'):m.end('condition')]
        label=r[m.start('label'):m.end('label')]
        tail=r[m.start('tail'):m.end('tail')]
        if m.re is PATTERN:
            call=r[m.start('call'):m.end('call')]
            assignment=r[m.start('assignment'):m.end('assignment')]
            statements=deeper+call+';\n'+deeper+assignment+';\n'
        else:
            terminal=r[m.start('terminal'):m.end('terminal')]
            statements=deeper+terminal+';\n'
        replacement=(indent+'if '+condition+' then\n'+statements+deeper+'goto '+label+'\n'+indent+'end'+tail+'\n')
        if code_tokens(r)!=code_tokens(replacement):continue
        # Keep comments and quoted text exact, including their relative order.
        significant=lambda s:[(t.lastgroup,t[0]) for t in tokens(s) if t.lastgroup!='space']
        if significant(r)!=significant(replacement):continue
        result[i]=replacement
        proofs.append(dict(line=i+1,original=r.rstrip('\n'),expanded=replacement.rstrip('\n'),tokensExact=True,semicolonsRetained=True,kind='call-assignment-jump' if m.re is PATTERN else 'return-jump'))
    after=''.join(result)
    assert code_tokens(source)==code_tokens(after)
    return after,proofs
