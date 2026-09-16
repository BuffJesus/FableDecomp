"""Recover Rock Troll root Main independently of canonical/shared converters."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT,CLUSTERS,RData
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.rock_root_program import BODY


def recover(data=None):
    data=data or RData()
    witness=json.loads(Path(__file__).with_name('rock_root_witness.json').read_text())
    cluster=json.loads((CLUSTERS/'V_RockTrollFirstEncounter.json').read_text())
    source=next(fn['decompile'] for fn in cluster['lifecycle'] if fn['role']=='Main')
    if hashlib.sha256(source.encode()).hexdigest()!=witness['sourceSha256']:
        raise ValueError('Rock root source correspondence changed')
    for region in witness['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            raise ValueError('Rock root native bytes changed: '+region['name'])
    for address,value in witness['strings'].items():
        actual='' if value=='' and data.bytes_at(int(address,16),1)==b'\0' else data.string_at(int(address,16))
        if actual!=value:raise ValueError('Rock root native literal changed')
    for call in witness['calls']:
        actual=read_call_window(data,0xec3d40,0x3e2,call['site'],
            {int(k):tuple(v) for k,v in call['known'].items()},argument_count=call['count'])
        if actual is None or json.loads(json.dumps(asdict(actual)))!=call['expected']:
            raise ValueError('Rock root native call operands changed')
    return BODY,witness


def generate(output=None):
    output=Path(output or ROOT/'work/rock_trigger_converter/root_candidate').resolve()
    if not output.is_relative_to((ROOT/'work/rock_trigger_converter').resolve()):
        raise ValueError('Rock root candidate must stay under work/rock_trigger_converter')
    body,witness=recover()
    text='-- Disabled root Main candidate; see COVERAGE.json.\nfunction Main(quest)\n'+body+'end\n'
    LuaRuntime().execute('return function()\n'+text+'\nend')
    output.mkdir(parents=True,exist_ok=True)
    (output/'RockTrollFirstEncounter.lua').write_text(text)
    (output/'quests.lua').write_text('-- Disabled: isolated root Main, scoped-message and entry-policy integration pending.\n')
    report={'registrationEnabled':False,'function':'RockTrollFirstEncounter.Main','nativeAddress':'0x00EC3D40',
        'syntax':'Lua5.4 passed','nativeEvidence':witness,
        'requiredCapability':{'WithRegionLoadedMessage':'Construct one native CString; Poll reuses it and returns raw MsgOnRegionLoaded bool, including true+empty. Destroy after callback completes/cancels, following generators and both deactivations on success.'},
        'stateContract':{'MissionOver':'native byte+4A, original PDB role; other entity writers need matching state binding.',
            'RockTrollHealthBarID':'native full32-bit+50; root Init/OnPersist and actor producers must use Int, not canonical Bool.',
            'Activated':'native byte+4D persisted under that name.'},
        'remaining':['WatchForRegionExit helper and both actor Mains remain separate incomplete converter work.',
            'Root Init/OnPersist are not emitted by this bounded recovery; whole-port state wiring remains pending.',
            'Scoped region-message capability required; current string-or-nil poll wrapper is not equivalent.',
            'Existing trigger candidate/capabilities remain separately staged; native host entry/scheduling and teardown remain integration gates.']}
    (output/'COVERAGE.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(generate()['syntax'])
