"""Inventory emitted calls and generate checks against actual Lua registrations."""
import hashlib
import json
import re
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT

RECEIVERS={'quest':'Quest','resources':'RetailResources','me':'CScriptThing',
           'guards':'RetailTheresaGuardVector','speechLists':'RetailVillagerSpeechLists',
           'barrels':'RetailBarrelWatchSnapshot','argumentKey':'RetailWifeArgumentKey','timer':'RetailOwnedTimer'}
LOCAL_STATE={'state','__native_entity_state'}
# Preserve line positions while removing Lua comments and string literals.
TOKENS=re.compile(r'--\[(=*)\[.*?\]\1\]|--[^\n]*|\[(=*)\[.*?\]\2\]|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',re.S)


def inventory(package=ROOT/'work/oakvale_final_readable'):
    package=Path(package);rows=[];unresolved=[];inputs={};required={}
    for path in sorted((package/'FSE').rglob('*.lua')):
        source=path.read_text();inputs[str(path)]=hashlib.sha256(path.read_bytes()).hexdigest()
        clean=TOKENS.sub(lambda match:''.join('\n' if c=='\n' else ' ' for c in match.group()),source)
        # Dot references include method values passed to pcall, not just colon calls.
        for match in re.finditer(r'\b([A-Za-z_]\w*)\s*([:.])\s*([A-Za-z_]\w*)',clean):
            receiver,operator,method=match.groups()
            if operator=='.' and receiver not in RECEIVERS|{name:None for name in LOCAL_STATE}:continue
            row=dict(path=str(path.relative_to(package)),line=clean.count('\n',0,match.start())+1,receiver=receiver,method=method)
            if receiver in RECEIVERS:
                row['type']=RECEIVERS[receiver];required.setdefault(row['type'],set()).add(method)
            elif receiver in LOCAL_STATE and re.fullmatch(r'(Get|Set)State(Bool|Int|Float|String)',method):
                row['type']='Lua entity state';row['scope']='Lua-local helper, outside native registration inventory'
            else:unresolved.append(row)
            rows.append(row)
    required={name:sorted(methods) for name,methods in sorted(required.items())}
    out=ROOT/'work/oakvale_api_inventory';out.mkdir(parents=True,exist_ok=True)
    script=[]
    for name,methods in required.items():
        for method in methods:
            key=json.dumps(name);member=json.dumps(method)
            script.append(f'assert(type(_G[{key}][{member}]) == "function", {json.dumps(name+":"+method+" missing registration")})')
    report_path=package/'READABILITY_REPORT.json'
    inputs[str(report_path)]=hashlib.sha256(report_path.read_bytes()).hexdigest()
    binding_names=json.loads(report_path.read_text())['mainBindings']['bindingNames']
    script.append('__oakvale_binding_names={'+','.join(map(json.dumps,binding_names))+'}')
    script.append(f'print("PASS: {sum(map(len,required.values()))} required methods in {len(required)} actual Lua usertypes")')
    check=out/'check.lua';check.write_text('\n'.join(script)+'\n')
    report=dict(status='unresolved receivers' if unresolved else 'ready for actual registration check',
        package=str(package),inputs=inputs,required=required,calls=rows,unresolved=unresolved,
        check=str(check),checkSha256=hashlib.sha256(check.read_bytes()).hexdigest(),
        limits='Receiver names are explicit package conventions. Verifies registration presence only, not argument ABI or engine behavior. Lua-local entity state is recorded separately.')
    (out/'inventory.json').write_text(json.dumps(report,indent=2)+'\n')
    if unresolved:raise ValueError('Unclassified receiver calls: '+repr(unresolved))
    return report


if __name__=='__main__':
    result=inventory();print(json.dumps({key:result[key] for key in ('status','required','unresolved')},indent=2))
