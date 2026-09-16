"""Fail-closed isolated recovery of the missing Rock Troll trigger Main."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.rock_trigger_program import BODY


def recover(data=None):
    data=data or RData()
    witness=json.loads(Path(__file__).with_name('rock_trigger_witness.json').read_text())
    source=(ROOT/'work/rock_trigger_converter/evidence/0x00EC42F0.c').read_text()
    if hashlib.sha256(source.encode()).hexdigest()!=witness['sourceSha256']:
        raise ValueError('Rock trigger source correspondence changed')
    for region in witness['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            raise ValueError('Rock trigger native bytes changed: '+region['name'])
    for address,value in witness['strings'].items():
        if data.string_at(int(address,16))!=value:raise ValueError('Rock trigger native literal changed')
    for call in witness['calls']:
        actual=read_call_window(data,0xec42f0,412,call['site'],
            {int(k):tuple(v) for k,v in call['known'].items()},argument_count=call['count'])
        if actual is None or json.loads(json.dumps(asdict(actual)))!=call['expected']:
            raise ValueError('Rock trigger call operands changed')
    return BODY,witness


def generate(output=None):
    output=Path(output or ROOT/'work/rock_trigger_converter/draft').resolve()
    if not output.is_relative_to((ROOT/'work/rock_trigger_converter').resolve()):
        raise ValueError('Rock trigger output must stay under work/rock_trigger_converter')
    body,witness=recover()
    text='-- Disabled single-function native candidate; see COVERAGE.json.\nfunction Main(quest, me)\n'+body+'end\n'
    LuaRuntime().execute('return function()\n'+text+'\nend')
    output.mkdir(parents=True,exist_ok=True)
    (output/'M_RTFERockTrollTrigger.lua').write_text(text)
    (output/'quests.lua').write_text('-- Registration disabled: isolated function and pending host capabilities.\n')
    report={'registrationEnabled':False,'function':'M_RTFERockTrollTrigger.Main','nativeAddress':'0x00EC42F0',
        'syntax':'Lua5.4 passed','nativeEvidence':witness,
        'requiredCapabilities':{
            'GetRockTrollTriggerProximity':'Read float at *(0143E90C)+72C on every poll; retail field-registration bytes name it RockTrollTriggerProximity. Do not hardcode installed5.0.',
            'RetailResources.CreateCreatureAtThingPosition':'Consume retained Thing id without exporting ownership. Native empty Data uses global vector143E8E0; otherwise Data vtable+18 position. Construct script then definition, create with false, destroy returned Thing then definition then script before state/remove operations.'},
        'remaining':['Only this Main is recovered; other port functions and activation remain incomplete.',
            'Scoped lookup/create/output lifetimes require the proposed resource consumer; generic Lua GetThing/position/creation would defer native destruction.',
            'Entity-host pre-entry cancellation guards must be reviewed against native condition-registration/first-frame timing; Lua-body tests do not certify host dispatch.',
            'Existing bound-alive registration and resource callback cleanup are reused; native engine scheduling and callback-error equivalence are not certified.']}
    (output/'COVERAGE.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(generate()['syntax'])
