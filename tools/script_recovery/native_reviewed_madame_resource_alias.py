"""Keep Madame's controlled local resource across reused stack temporaries."""
import hashlib
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_MEM, X86_OP_REG, X86_REG_ECX, X86_REG_ESP


RECEIVERS = (
    'xStack_124', 'xStack_174', 'xStack_148', 'xStack_148',
    'xStack_174', 'xStack_174', 'xStack_174', 'xStack_174',
    'xStack_148_2', 'xStack_148_2', 'xStack_148_2', 'xStack_15c',
    'xStack_148_2', 'xStack_174', 'xStack_15c', 'xStack_148_2',
    'xStack_134', 'xStack_134', 'xStack_134', 'xStack_148_2',
    'xStack_174', 'xStack_174',
)
THING = re.compile(r'resources:ScriptThing\((xStack_[A-Za-z0-9_]+)\)')
SOURCE_CHANGES = (
    ('xStack_174 = resources:NewResource()',
     'xStack_174 = resources:NewResource()\n    local madameResource = xStack_174', 1),
    ('resources:PrepareResource(xStack_174)', 'resources:PrepareResource(madameResource)', 1),
    ('resources:TryAcquire(xStack_174, me, 4)', 'resources:TryAcquire(madameResource, me, 4)', 2),
    ('resources:AssignResource(resources:MemberResource("seh_Madam"), xStack_174)',
     'resources:AssignResource(resources:MemberResource("seh_Madam"), madameResource)', 1),
    ('resources:ReleaseResource(xStack_174)', 'resources:ReleaseResource(madameResource)', 2),
)


def restore(source, fn, row, rdata):
    if row is None:
        return source

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed Madame resource alias {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    calls = {call['site'].lower(): call for call in fn['calls']}
    ctor = int(row['constructorSite'], 16)
    require(calls.get(row['constructorSite'].lower(), {}).get('target', '').lower() == '0x0099a380',
            'resource constructor changed')

    def require_receiver(site, displacement):
        ins = indexed.get(site - 4)
        require(ins is not None and ins.mnemonic == 'lea'
                and ins.operands[0].type == X86_OP_REG
                and ins.operands[0].reg == X86_REG_ECX
                and ins.operands[1].type == X86_OP_MEM
                and ins.operands[1].mem.base == X86_REG_ESP
                and ins.operands[1].mem.disp == displacement,
                f'resource stack receiver changed at {site:08x}')

    require_receiver(ctor, 0x1C)
    native_getters = [call for call in fn['calls']
                      if call['target'].lower() == row['getterTarget'].lower()]
    require([call['site'] for call in native_getters] == row['getterSites']
            and len(native_getters) == len(RECEIVERS) == 22,
            'GetScriptThing site coverage changed')
    for call in native_getters:
        require(call['ecxStack'] + call['depth'] == 32,
                f'GetScriptThing frame slot changed at {call["site"]}')
        require_receiver(int(call['site'], 16), 0x20)
    assignment = calls.get('0x00e3bcd8')
    require(assignment is not None and assignment['target'].lower() == '0x008abd10',
            'parent resource copy changed')
    require(tuple(THING.findall(source)) == RECEIVERS,
            'lifted GetScriptThing receivers changed')
    require(source.count('local madameResource') == 0,
            'resource already lifted')
    for old, _new, count in SOURCE_CHANGES:
        require(source.count(old) == count, f'lifted resource use changed: {old}')
    for old, new, count in SOURCE_CHANGES:
        source = source.replace(old, new, count)
    source = THING.sub('resources:ScriptThing(madameResource)', source)
    require(source.count('resources:ScriptThing(madameResource)') == 22,
            'GetScriptThing replacement incomplete')
    return source
