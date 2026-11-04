"""Restore BordelloLady's native string personality override call."""
import hashlib
from pathlib import Path
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_REG_ESI


ROOT = Path(__file__).resolve().parents[2]
COMMENT = '-- TODO(native): EntitySetPersonalityOverride is not a ForgeFSE binding'
OLD = 'quest:EntitySetPersonalityOverride()'
NEW = 'quest:EntitySetPersonalityOverrideByString(me, "OPINION_PERSONALITY_EX_PROSTITUTE")'


def restore(source, todo, calls, fn, row, rdata):
    if row is None:
        return source, todo, calls

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloLady personality {fn["address"]}: {why}')

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
    for site, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {site}')
    native_calls = [call for call in fn['indirectCalls'] if call['slot'] == '0x9ac']
    require(len(native_calls) == 1 and native_calls[0]['site'].lower() == row['nativeCall'].lower(),
            'native personality call changed')
    require(all(X86_REG_ESI not in ins.regs_access()[1] for ins in instructions
                if 0xE400AE < ins.address < 0xE400D8),
            'self register changed before personality call')
    require(rdata.string_at(int(row['literalAddress'], 16)) == 'OPINION_PERSONALITY_EX_PROSTITUTE',
            'native personality string changed')

    binding = (ROOT / row['bindingSource']).read_bytes()
    require(hashlib.sha256(binding).hexdigest() == row['bindingSha256']
            and b'void LuaQuestState::EntitySetPersonalityOverrideByString(const std::shared_ptr<CScriptThing>& spThing, const std::string& personalityName)' in binding
            and b'EntitySetPersonalityOverrideByString_API(m_pGameInterface, pThing, fsName.get());' in binding,
            'Forge string binding changed')

    count = row['liftedCallCount']
    require(source.count(COMMENT) == count and source.count(OLD) == count
            and todo.count(row['diagnostic']) == count
            and calls.count('EntitySetPersonalityOverride') == count,
            'lifted personality calls changed')
    source, removed = re.subn(r'^[ \t]*' + re.escape(COMMENT) + r'\n', '', source, flags=re.M)
    require(removed == count, 'lifted personality comments changed')
    source = source.replace(OLD, NEW)
    for _ in range(count):
        todo.remove(row['diagnostic'])
    calls = ['EntitySetPersonalityOverrideByString' if call == 'EntitySetPersonalityOverride' else call
             for call in calls]
    return source, todo, calls
