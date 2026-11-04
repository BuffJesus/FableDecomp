"""Recognize Bordello's reviewed paused cutscene skip sidecar binding."""
import hashlib
from pathlib import Path
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


ROOT = Path(__file__).resolve().parents[2]
COMMENT = '-- TODO(native): SetCutsceneSkippableWhilePaused is not a ForgeFSE binding'
DIAGNOSTIC = 'SetCutsceneSkippableWhilePaused: not in FSE manifest'


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed Bordello paused skip binding {fn["address"]}: {why}')

    patch = ROOT / row['bindingPatch']
    require(patch.is_file() and hashlib.sha256(patch.read_bytes()).hexdigest() == row['bindingPatchSha256'],
            'sidecar binding patch changed')
    patch_text = patch.read_text(encoding='utf-8')
    require('questState_type["SetCutsceneSkippableWhilePaused"] = &LuaQuestState::SetCutsceneSkippableWhilePaused;' in patch_text
            and 'SetCutsceneSkippableWhilePaused_API(m_pGameInterface, bIsSkippable);' in patch_text,
            'sidecar registration or forwarder changed')
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
    calls = [call['site'].lower() for call in fn['indirectCalls'] if call['slot'] == '0x610']
    require(calls == ['0x00e3e9fa', '0x00e3ea54'], 'paused skip call inventory changed')
    require(source.count(COMMENT) == 2 and todo.count(DIAGNOSTIC) == 2
            and source.count('quest:SetCutsceneSkippableWhilePaused(true)') == 1
            and source.count('quest:SetCutsceneSkippableWhilePaused(false)') == 1,
            'lifted paused skip calls changed')
    for _ in range(2):
        todo.remove(DIAGNOSTIC)
    updated, count = re.subn(r'(?m)^[ \t]*-- TODO\(native\): SetCutsceneSkippableWhilePaused is not a ForgeFSE binding\n',
                             '', source)
    require(count == 2, 'reviewed TODO comments changed')
    return updated, todo
