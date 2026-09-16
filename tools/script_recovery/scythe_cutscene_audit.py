"""Compare native Scythe cutscene operands with the audited host adapter."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from tools.script_recovery.lift_native_lua import ROOT, RData
from tools.script_recovery.native_call_setup_ir import read_call_window


def audit(rdata=None, host_source=None):
    rdata = rdata or RData()
    witness = json.loads(Path(__file__).with_name('native_scythe_cutscene_audit_witness.json').read_text())
    if host_source is None:
        host_source = (ROOT / witness['hostSourcePath']).read_text()
    if hashlib.sha256(host_source.encode()).hexdigest() != witness['hostSourceSha256']:
        return {'status': 'rejected', 'reason': 'audited host source changed'}
    base, size = int(witness['address'], 16), witness['size']
    raw = rdata.bytes_at(base, size)
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['nativeSha256']:
        return {'status': 'rejected', 'reason': 'native cutscene helper changed'}
    for block in witness['inputSubstitution']['blocks']:
        data = rdata.bytes_at(int(block['address'], 16), block['size'])
        if data is None or hashlib.sha256(data).hexdigest() != block['sha256']:
            return {'status': 'rejected', 'reason': 'native input substitution changed'}
    for string in witness['inputSubstitution']['strings']:
        if rdata.string_at(int(string['address'], 16)) != string['value']:
            return {'status': 'rejected', 'reason': 'native substitution literal changed'}
    for call in witness['calls']:
        setup = read_call_window(rdata, base, size, call['site'], argument_count=call['count'])
        if setup is None:
            return {'status': 'rejected', 'reason': 'native cutscene operands unavailable'}
        actual = json.loads(json.dumps(asdict(setup)))
        if any(actual.get(k) != v for k, v in call['expected'].items()):
            return {'status': 'rejected', 'reason': 'native cutscene operands changed'}
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, base))
    acquisition = int(witness['nativeAcquisition']['site'], 16)
    next_call = int(witness['nativeAcquisition']['nextCall'], 16)
    # The native acquisition bool is dead before the next unknown call.
    # This differs from the host adapter's explicit retry loop.
    for instruction in instructions:
        if acquisition < instruction.address < next_call:
            read, _ = instruction.regs_access()
            if (any(instruction.reg_name(reg) in ('eax', 'ax', 'al', 'ah', 'eflags') for reg in read)
                    or instruction.mnemonic.startswith(('j', 'ret', 'loop'))):
                return {'status': 'rejected', 'reason': 'native acquisition result has a new consumer'}
    return dict(witness, status='checked; adapter requires a fidelity decision',
                runtimeChanged=False, nativeAcquisitionResultIgnored=True)


if __name__ == '__main__':
    report = audit()
    output = ROOT / 'work/scythe_converter/CUTSCENE_ADAPTER_REVIEW.json'
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({'status': report['status'], 'output': str(output)}, indent=2))
