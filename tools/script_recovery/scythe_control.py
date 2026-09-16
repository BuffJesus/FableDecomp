"""Checked Scythe acquisition collapse and deterministic owned/borrowed release."""
import hashlib
import json
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


def recover(source, rdata=None):
    rdata = rdata or RData()
    root = Path(__file__).parent
    w = json.loads((root / 'scythe_control_witness.json').read_text())
    raw = rdata.bytes_at(int(w['address'], 16), w['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != w['nativeSha256']:
        raise ValueError('Scythe control native bytes changed')
    destructor = w['destructor']
    data = rdata.bytes_at(int(destructor['address'], 16), destructor['size'])
    if data is None or hashlib.sha256(data).hexdigest() != destructor['sha256']:
        raise ValueError('Scythe control destructor changed')
    if hashlib.sha256(source.encode()).hexdigest() != w['sourceSha256']:
        raise ValueError('Scythe control Lua correspondence changed')
    if hashlib.sha256((root / 'scythe_control_host_snapshot.inc').read_text().encode()).hexdigest() != w['hostSnapshotSha256']:
        raise ValueError('Scythe checked host acquisition changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, int(w['address'], 16)))
    if not check_single_resource_lifetime(instructions, {int(a,16):tuple(e) for a,e in w['events'].items()}):
        raise ValueError('Scythe control native lifetime changed')
    old = '''        cVar6 = me:AcquireControl(4)
        while not cVar6 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00e2a8ee end
            cVar6 = me:AcquireControl(4)
        end'''
    tail = '        ::LAB_00e2a8ee::\n    end\nend\n'
    if source.count(old) != 1 or source.count(tail) != 1:
        raise ValueError('Scythe control block correspondence changed')
    source = source.replace(old, '''        if not me:AcquireControl(4) then return end
        local function controlled_body()''')
    source = source.replace(tail, '''        end
        local ok, failure = pcall(controlled_body)
        me:ReleaseControl()
        if not ok then error(failure, 0) end
    end
end
''')
    return source, dict(w, status='recovered with checked host acquisition semantics')
