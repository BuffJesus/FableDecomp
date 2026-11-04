"""Restore the BordelloClient paid encounter's incremented sex count."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_REG, X86_REG_EAX


OLD = ('__push4 = quest:GetNumberOfTimesHeroHasHadSex()\n'
       '                                    quest:SetNumberOfTimesHeroHasHadSex(__push4)')
NEW = ('__push4 = quest:GetNumberOfTimesHeroHasHadSex()\n'
       '                                    quest:SetNumberOfTimesHeroHasHadSex(__push4 + 1)')


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloClient sex count {fn["address"]}: {why}')

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
    for address, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(address, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {address}')
    for site, slot in (('0x00E45FA1', '0x368'), ('0x00E45FAC', '0x36c')):
        calls = [call for call in fn['indirectCalls'] if call['site'].lower() == site.lower()]
        require(len(calls) == 1 and calls[0]['slot'] == slot,
                f'native call changed at {site}')
    inc, push = indexed[0xE45FAA], indexed[0xE45FAB]
    require(inc.mnemonic == 'inc' and inc.operands[0].type == X86_OP_REG
            and inc.operands[0].reg == X86_REG_EAX
            and push.mnemonic == 'push' and push.operands[0].type == X86_OP_REG
            and push.operands[0].reg == X86_REG_EAX,
            'getter result no longer incremented and passed')
    require(source.count(OLD) == 1 and source.count('SetNumberOfTimesHeroHasHadSex') == 1,
            'lifted count call changed')
    return source.replace(OLD, NEW, 1), todo
