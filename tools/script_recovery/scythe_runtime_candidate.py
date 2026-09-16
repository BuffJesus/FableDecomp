"""Wire the disabled Scythe draft to a pending, isolated runtime proposal."""
import hashlib
import json
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.scythe_converter import convert
from tools.script_recovery.scythe_cutscene_audit import audit
from tools.script_recovery.scythe_control import recover as recover_control
from tools.script_recovery.scythe_condition import recover as recover_condition
from tools.script_recovery.scythe_cleanup import recover as recover_cleanup
from tools.script_recovery.scythe_returned_things import recover as recover_things


def wire(source, rdata=None):
    witness = json.loads(Path(__file__).with_name('scythe_runtime_candidate_witness.json').read_text())
    checked = audit(rdata)
    if not checked['status'].startswith('checked'):
        raise ValueError('Native cutscene audit rejected: ' + repr(checked))
    if hashlib.sha256(source.encode()).hexdigest() != witness['sourceSha256'] or source.count(witness['block']) != 1:
        raise ValueError('Scythe Lua cutscene correspondence changed')
    result = source.replace(witness['block'], '                    -- Pending runtime integration: RunScytheOracleCutscene.\n'
                            '                    quest:RunScytheOracleCutscene(me)')
    LuaRuntime().execute('return function()\n' + result + '\nend')
    return result


def build():
    output = ROOT / 'work/scythe_converter/runtime_proposal/draft'
    report = convert(output)
    path = output / 'Entities/ScytheNearOracle.lua'
    controlled, control_evidence = recover_control(wire(path.read_text()))
    controlled, condition_evidence = recover_condition(controlled)
    controlled, cleanup_evidence = recover_cleanup(controlled)
    controlled, thing_evidence = recover_things('ScytheNearOracle',controlled)
    LuaRuntime().execute('return function()\n' + controlled + '\nend')
    path.write_text(controlled)
    main = report['entities']['ScytheNearOracle']['Main']
    main['preAdapterTodo'] = main['todo'][:]
    if len(main['todo']) != 5 or any(not row.startswith('unlifted: ') for row in main['todo'][2:]):
        raise ValueError('Scythe cutscene diagnostic correspondence changed')
    main['todo'] = ['pending runtime integration: RunScytheOracleCutscene']
    main['controlEvidence'] = control_evidence
    main['conditionEvidence'] = condition_evidence
    main['callbackErrorPolicy'] = cleanup_evidence
    main['returnedThingEvidence'] = thing_evidence
    main['todo'].append('pending runtime integration: FaceThingByScriptName')
    marker_path=output/'Entities/ScytheMarker.lua'
    marker_source,marker_evidence=recover_things('ScytheMarker',marker_path.read_text())
    LuaRuntime().execute('return function()\n'+marker_source+'\nend')
    marker_path.write_text(marker_source)
    marker=report['entities']['ScytheMarker']['Main']
    marker['returnedThingEvidence']=marker_evidence
    marker['todo'].append('pending runtime integration: SpawnScytheAndRemoveMarker')
    report['runtimeProposal'] = {'status': 'pending integration', 'capabilities': ['RunScytheOracleCutscene','FaceThingByScriptName','SpawnScytheAndRemoveMarker'],
        'registrationEnabled': False, 'nativeUnresolvedCallsReplaced': 3,
        'remaining': 'Runtime integration, borrowed-versus-distinct native resource equivalence, conditions and scheduler semantics require review.',
        'syntax': 'Lua 5.4 passed'}
    (output / 'COVERAGE.json').write_text(json.dumps(report, indent=2) + '\n')
    return report


if __name__ == '__main__':
    print(json.dumps(build()['runtimeProposal'], indent=2))
