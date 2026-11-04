"""Restore BordelloLady's own thing and home position in its opening setup."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


REPLACEMENTS = (
    ('quest:SetCreatureBrain(nil --[[missing]], "BRAIN_PASSIVE_OVERRIDE")',
     'quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")'),
    ('        r1 = nil\n', ''),
    ('quest:SetWanderCentrePoint(nil --[[missing]], nil --[[missing]])',
     'quest:SetWanderCentrePoint(me, r1)'),
    ('quest:SetWanderMinDistance(nil --[[missing]], 0)',
     'quest:SetWanderMinDistance(me, 0)'),
    ('quest:SetWanderMaxDistance(nil --[[missing]], 16.0)',
     'quest:SetWanderMaxDistance(me, 16.0)'),
    ('quest:SetScriptingStateGroup(nil --[[missing]], 0x0)',
     'quest:SetScriptingStateGroup(me, 4)'),
)


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloLady setup {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    instructions = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    for site, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {site}')

    # The preserved r1 is the position returned by me:GetHomePos. A
    # decompiled temporary cleanup had incorrectly cleared it before use.
    positions = []
    for old, _ in REPLACEMENTS:
        require(source.count(old) == 1, f'lifted operand changed: {old}')
        positions.append(source.index(old))
    require(positions == sorted(positions)
            and source.count('        r1 = me:GetHomePos()\n') == 1
            and source.index('        r1 = me:GetHomePos()\n') < positions[1],
            'lifted setup order changed')
    for diagnostic in row['diagnostics']:
        require(todo.count(diagnostic) == 1, f'diagnostic changed: {diagnostic}')
    for old, new in REPLACEMENTS:
        source = source.replace(old, new, 1)
    for diagnostic in row['diagnostics']:
        todo.remove(diagnostic)
    return source, todo
