"""Restore Magicman's two reviewed conversation-line calls."""
import hashlib
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32

ROOT = Path(__file__).resolve().parents[2]


def restore(source, todo, calls, fn, row, rdata):
    if row is None:
        return source, todo, calls

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed Magicman conversation {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    decompile = fn['decompile']
    require(hashlib.sha256(decompile.encode()).hexdigest() == row['sourceSha256']
            and decompile.count(row['rawCall']) == 2,
            'exported source changed')
    instructions = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    for site, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {site}')
    indirect = {c['site'].lower(): c for c in fn['indirectCalls']}
    for site in row['sites']:
        call = indirect.get(site.lower())
        require(call is not None and call['slot'] == '0x5b8',
                f'native conversation call changed at {site}')

    binding = (ROOT / row['bindingSource']).read_bytes()
    require(hashlib.sha256(binding).hexdigest() == row['bindingSha256']
            and b'void LuaQuestState::AddLineToConversation(int conversationID, const std::string& textKey, sol::object speaker,' in binding
            and b'AddLineToConversation_API(m_pGameInterface, conversationID, fsKey, bShowSubtitle, pSpeaker, pListener);' in binding,
            'Lua binding order changed')

    require(len(row['patterns']) == 2, 'lifted call coverage changed')
    for entry in row['patterns']:
        for assignment in entry['assignments']:
            require(source.count(assignment) == 1, 'lifted conversation operands changed')
        for statement in entry['remove'] + [entry['call']]:
            require(source.count(statement) == 1, 'lifted conversation call changed')
            diagnostic = 'unlifted: ' + statement.removeprefix('-- TODO(native): ')
            require(todo.count(diagnostic) == 1, 'conversation diagnostic changed')
        for statement in entry['remove']:
            source = source.replace('                    ' + statement + '\n', '', 1)
            todo.remove('unlifted: ' + statement.removeprefix('-- TODO(native): '))
        source = source.replace(entry['call'], entry['replacement'], 1)
        todo.remove('unlifted: ' + entry['call'].removeprefix('-- TODO(native): '))
        calls.append('AddLineToConversation')
    return source, todo, calls
