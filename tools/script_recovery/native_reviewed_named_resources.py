"""Native witnesses for entity speech through separately acquired resources."""
import hashlib
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_MEM, X86_OP_REG, X86_REG_ECX, X86_REG_ESP

from tools.script_recovery.native_named_resource_receivers import named_resources


def reviewed_resources(source, fn, row, rdata):
    if row is None:
        return set()

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed named resources {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    raw = rdata.bytes_at(start, int(fn['bodyEndExclusive'], 16) - start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'], 'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'], 'export changed')
    normalized = source.replace('\r\n', '\n').replace('\r', '\n')
    require(hashlib.sha256(normalized.encode()).hexdigest() == row['loweredSha256'], 'lowered source changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + len(raw), 'incomplete disassembly')
    by_site = {i.address: i for i in instructions}
    previous = {b.address: a for a, b in zip(instructions, instructions[1:])}
    witnessed = {int(address, 16) for address in row['nativeInstructions']}
    for address, encoded in row['nativeInstructions'].items():
        ins = by_site.get(int(address, 16))
        require(ins is not None and ins.bytes.hex() == encoded, 'instruction changed')
    for address, encoded in row['helperBytes'].items():
        helper = rdata.bytes_at(int(address, 16), len(encoded) // 2)
        require(helper is not None and helper.hex() == encoded, 'resource helper changed')
    direct = {int(c['site'], 16): c for c in fn['calls']}
    indirect = {int(c['site'], 16): c for c in fn['indirectCalls']}
    objects = set()
    covered = set()
    require(row['resources'], 'empty resource evidence')
    for resource in row['resources']:
        name = resource['object']
        require(re.fullmatch(r'xStack_[0-9a-f]+', name), 'invalid resource object')
        slot, frame = resource['entrySlot'], resource['frameBytes']
        require(slot == -int(name.rsplit('_', 1)[1], 16) and frame > 0, 'resource stack identity changed')
        acquire = indirect.get(resource['acquireSite'])
        require(resource['acquireSite'] in witnessed and acquire is not None
                and acquire.get('slot') == '0x20' and acquire['depth'] == frame + 12
                and len(acquire.get('pushedStack', [])) == 3 and acquire['pushedStack'][1] == slot,
                'acquisition receiver changed')
        require(resource['speakSites'] and resource['taskSites'], 'incomplete speech evidence')
        for sites, target, extra in ((resource['speakSites'], 0x7e7390, 24), (resource['taskSites'], 0x7e7450, 0)):
            require(hex(target) in row['helperBytes'], 'missing helper witness')
            for site in sites:
                require(site not in covered, 'duplicate call evidence')
                covered.add(site)
                call = direct.get(site)
                require(site in witnessed and call is not None and int(call['target'], 16) == target
                        and call.get('depth') == frame + extra and call.get('ecxStack') == slot,
                        'speech/task receiver changed')
                ins = previous.get(site)
                require(ins is not None and ins.mnemonic == 'lea' and len(ins.operands) == 2,
                        'missing receiver address')
                dest, operand = ins.operands
                require(dest.type == X86_OP_REG and dest.reg == X86_REG_ECX
                        and operand.type == X86_OP_MEM and operand.mem.base == X86_REG_ESP
                        and operand.mem.index == 0 and operand.mem.disp - frame - extra == slot,
                        'native receiver address changed')
        require(name not in objects, 'duplicate resource evidence')
        objects.add(name)
    require(named_resources(source) == objects, 'named-resource inventory changed')
    require(covered == {site for site, call in direct.items() if int(call['target'], 16) in (0x7e7390, 0x7e7450)},
            'speech/task call inventory changed')
    return objects
