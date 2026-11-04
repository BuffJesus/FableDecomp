"""Restore ordered-positive health flags only with uniform native comparison proof."""
import re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32

_PATTERN=re.compile(rb'\xd8\x1d(?P<address>.{4})\xc6\x44\x24(?P<offset>.)\x01\xdf\xe0\xf6\xc4\x41\x74\x05\xc6\x44\x24(?P=offset)\x00',re.S)

def restore(source,fn,rdata):
    calls=[c for c in fn.get('indirectCalls',[]) if c.get('slot')=='0x420']
    if not calls or source.count('quest:GetHealth(')!=len(calls):return source
    if not fn.get('bodyEndExclusive'):return source
    start=int(fn['address'],16);end=int(fn['bodyEndExclusive'],16)
    raw=rdata.bytes_at(start,end-start)
    if raw is None:return source
    instructions={i.address:i for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,start)}
    for call in calls:
        ins=instructions.get(int(call['site'],16))
        if ins is None or ins.mnemonic!='call' or '+ 0x420]' not in ins.op_str:return source
        match=_PATTERN.fullmatch(rdata.bytes_at(ins.address+ins.size,23) or b'')
        if not match or rdata.bytes_at(int.from_bytes(match['address'],'little'),4)!=bytes(4):return source
    # Two emitted shapes for the same one-byte flag. Consume the entire local
    # producer/consumer so a different use of the float cannot be rewritten.
    call=r'(?P<indent>[ \t]*)(?P<value>\w+) = quest:GetHealth\([^\n]*\)\n'
    explicit=(r'(?P=indent)(?P<flag>\w+) = true\n'
              r'(?P=indent)if (?P<condition>(?P=value) <= 0\.0) then\n'
              r'(?P=indent)    (?P=flag) = false\n(?P=indent)end\n')
    expression=r'(?P=indent)\w+ = not \((?P<condition>(?P=value) <= 0\.0)\)\n'
    matches=[]
    for shape in (explicit,expression):matches.extend(re.finditer('^'+call+shape,source,re.M))
    if len(matches)!=len(calls) or any('missing' in m[0] or m.groupdict().get('flag')==m['value'] for m in matches):return source
    for m in sorted(matches,key=lambda m:m.start(),reverse=True):
        a,b=m.span('condition');source=source[:a]+'not ('+m['value']+' > 0.0)'+source[b:]
    return source
