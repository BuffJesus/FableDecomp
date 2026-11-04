"""Remove address-only cursors after a native-verified state-array rewrite."""
import re
from tools.script_recovery.readable_lua import tokens

def remove_mapped_state_cursor(source, name, offset):
    """Caller must prove the native cursor and replace every dereference first.

    This is native-address cleanup, not permission to erase arbitrary Lua math.
    Unknown remaining uses fail closed so a partial rewrite cannot hide them.
    """
    lines=source.splitlines(keepends=True)
    masked=''.join(re.sub(r'[^\n]',' ',t[0]) if t.lastgroup in
                   ('comment','longcomment','string','longstring') else t[0]
                   for t in tokens(source)).splitlines()
    init=re.compile(r'\s*'+re.escape(name)+r' = \(this \+ '+re.escape(hex(offset))+r'\)\s*')
    step=re.compile(r'\s*'+re.escape(name)+r' = '+re.escape(name)+r' \+ 1\s*')
    starts=[i for i,s in enumerate(masked) if init.fullmatch(s)]
    steps=[i for i,s in enumerate(masked) if step.fullmatch(s)]
    if len(starts)!=1 or len(steps)!=1 or starts[0]>=steps[0]:
        raise ValueError('mapped state cursor shape changed: '+name)
    owned=set(starts+steps)
    declarations=0
    for i,s in enumerate(masked):
        if i in owned or not re.search(r'\b'+re.escape(name)+r'\b',s):continue
        if re.fullmatch(r'\s*local [\w, ]+\s*',s):
            declarations+=1
            if declarations<=1:continue
        raise ValueError('mapped state cursor has an unrecovered use: '+name)
    for i in owned:
        # Preserve any source comment at the now-recovered address operation.
        comments=''.join(t[0] for t in tokens(lines[i]) if t.lastgroup in ('comment','longcomment'))
        lines[i]=re.match(r'\s*',lines[i])[0].replace('\n','')+comments+'\n' if comments else ''
    return ''.join(lines)
