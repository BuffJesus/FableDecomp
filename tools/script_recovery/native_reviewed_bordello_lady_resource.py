"""Keep BordelloLady's constructed resource for scripted-thing copies."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


ASSIGN = ('-- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= '
          '(resources:MemberResource("seh_Whore"),&xStack_1a8);')


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloLady resource {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    end = int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end - start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    instructions = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == end,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    for address, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(address, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {address}')
    calls = {call['site'].lower(): call for call in fn['calls']}
    for site, target in row['nativeCalls'].items():
        require(calls.get(site.lower(), {}).get('target', '').lower() == target.lower(),
                f'native call changed at {site}')
    getters = [call for call in fn['calls'] if call['target'].lower() == '0x007e7490']
    require([call['site'] for call in getters] == row['getterSites']
            and all(call['ecxStack'] + call['depth'] == 28 for call in getters),
            'GetScriptThing receiver changed')
    require(source.count('x_stk_188 = resources:NewResource()') == 1
            and source.count('x_stk_188 = nil') == 1
            and source.count(ASSIGN) == 2
            and source.count('resources:ScriptThing(xStack_1a0)') == 2
            and source.count('resources:ScriptThing(xStack_1a8)') == 1
            and todo.count(row['diagnostic']) == 2,
            'lifted resource uses changed')

    source = source.replace('x_stk_188 = resources:NewResource()',
                            'x_stk_188 = resources:NewResource()\n        local ladyResource = x_stk_188', 1)
    source = source.replace(ASSIGN,
                            'resources:AssignResource(resources:MemberResource("seh_Whore"), ladyResource)')
    source = source.replace('resources:ScriptThing(xStack_1a0)',
                            'resources:ScriptThing(ladyResource)')
    source = source.replace('resources:ScriptThing(xStack_1a8)',
                            'resources:ScriptThing(ladyResource)')
    todo = todo.copy()
    todo.remove(row['diagnostic'])
    todo.remove(row['diagnostic'])
    return source, todo
