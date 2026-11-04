"""Rejoin Magicman's first beer movie task loop and native cleanup."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32

OLD_LOOP = '''                                                iVar21 = me:IsPerformingScriptTask()
                                                if iVar21 then
                                                    -- LAB_00e428b0: (native jump target)
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if not bVar5 then goto LAB_00e428c7 end
                                                    quest:PauseAllNonScriptedEntities(0x126008c)
                                                    this_00 = xStack_330
                                                    -- LAB_00e4485d: (native jump target)
                                                    goto LAB_00e44862
                                                end
                                                -- LAB_00e428d4: (native jump target)
'''
NEW_LOOP = '''                                                while me:IsPerformingScriptTask() do
                                                    quest:NewScriptFrame(me)
                                                    if quest:IsActiveThreadTerminating() then
                                                        quest:PauseAllNonScriptedEntities(0x126008c)
                                                        resources:DestroyMovie(xStack_330)
                                                        goto LAB_00e44862
                                                    end
                                                end
'''
OLD_TAIL = '''    ::LAB_00e428c7::
    iVar21 = me:IsPerformingScriptTask()
    if not iVar21 then return end  -- TODO(native): goto LAB_00e428d4
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then goto LAB_00e428c7 end
    quest:PauseAllNonScriptedEntities(0x126008c)
    this_00 = xStack_330
    -- LAB_00e4485d_c34: (native jump target)
    goto LAB_00e44862
'''


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed Magicman task loop {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    decompile = fn['decompile']
    require(hashlib.sha256(decompile.encode()).hexdigest() == row['sourceSha256']
            and decompile.count('code_r0x00e428c7:') == 1
            and decompile.count('LAB_00e428d4:') == 1
            and decompile.count('LAB_00e4485d:') == 1,
            'exported source changed')
    instructions = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    for site, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {site}')
    direct = {c['site'].lower(): c for c in fn['calls']}
    member_release = direct.get(row['memberRelease'].lower())
    base_release = direct.get(row['baseRelease'].lower())
    require(member_release is not None and base_release is not None
            and member_release['target'].lower() == '0x006e7ab0'
            and member_release['ecxStack'] == -788
            and base_release['target'].lower() == '0x0099a430',
            'native movie cleanup changed')

    require(source.count(OLD_LOOP) == 1 and source.count(OLD_TAIL) == 1
            and source.count('LAB_00e428c7') == 3
            and todo.count(row['diagnostic']) == 1,
            'lifted loop changed')
    source = source.replace(OLD_LOOP, NEW_LOOP, 1).replace(OLD_TAIL, '', 1)
    todo.remove(row['diagnostic'])
    return source, todo
