"""Strip duplicate receivers backed by immutable entry-block entity pointer snapshots.

This recognizes pointer assignments, not counted CScriptThing value copies. It stops
at the first call/control transfer and rejects written or address-taken snapshots.
"""
import re
ME_RECEIVER = r'(?:\(CScriptThing(?:_bv)? \*\))?\(this \+ 8\)'

def find(text: str, me_receiver: str = ME_RECEIVER) -> set[str]:
    pointers=set(re.findall(r'^\s*CScriptThing(?:_bv)?\s+\*\s*(\w+)\s*;',text,re.M))
    if not pointers:
        return set()
    lines=text.splitlines(keepends=True)
    started=False
    aliases=set()
    snapshots=set()
    for index,line in enumerate(lines):
        stripped=line.strip()
        if not started:
            if stripped=='{':
                started=True
            continue
        if not stripped or stripped.startswith('//'):
            continue
        assignment=re.fullmatch(r'(\s*)(\w+) = (.*);(\s*)',line)
        if assignment:
            indent,target,value,tail=assignment.groups()
            is_me=re.fullmatch(me_receiver,value) is not None
            is_copy=value in aliases and target in pointers
            aliases.discard(target)
            if is_me or is_copy:
                aliases.add(target)
                if is_copy:
                    token=re.escape(target)
                    # A single direct write, no indirect write through &slot,
                    # increment or compound assignment anywhere in the function.
                    writes=re.findall(r'\b'+token+r'\s*(?:=(?!=)|[+*/%&|^\-]=|\+\+|--)',text)
                    if len(writes)==1 and not re.search(r'(?<!&)&\s*'+token+r'\b|(?:\+\+|--)\s*'+token+r'\b',text):
                        snapshots.add(target)
                continue
            # Constant stores may separate the pointer and its snapshot. Any
            # call, indirection read, or unknown expression closes this prefix.
            if re.fullmatch(r'(?:true|false|0x[\da-fA-F]+|\d+|&\w+)',value):
                continue
            break
        # Local declarations only; labels, branches, calls and braces close it.
        if re.fullmatch(r'\s*[\w:]+\s+\**\s*\w+(?:\s*\[\d+\])?\s*;\s*',line):
            continue
        break
    return snapshots

def restore(text: str, me_receiver: str = ME_RECEIVER) -> str:
    for name in sorted(find(text,me_receiver)):
        prefix=r'(CScriptThing::\w+\((?:'+me_receiver+r'|me),\s*)'
        duplicate=r'(?:\(void \*\))?'+re.escape(name)+r'(?:,\s*|(?=\)))'
        text=re.sub(prefix+duplicate,r'\1',text)
    return text
