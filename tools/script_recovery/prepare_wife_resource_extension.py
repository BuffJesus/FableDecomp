"""Stage argument-key methods over the owned-position resource proposal."""
import difflib
import hashlib
import json
from pathlib import Path

from tools.script_recovery.lift_native_lua import ROOT


def prepare(base=ROOT/'work/woman_resource_integration',output=ROOT/'work/wife_resource_integration'):
    base,output=Path(base),Path(output)
    report=json.loads((base/'proposal.json').read_text())
    candidate=(base/'LuaRetailResources.h').read_bytes()
    if hashlib.sha256(candidate).hexdigest()!=report['candidateSha256']:
        raise ValueError('Parent resource proposal changed')
    header=Path(report['source']);original=header.read_bytes()
    if hashlib.sha256(original).hexdigest()!=report['sourceSha256']:
        raise ValueError('Runtime resource source changed')
    source=candidate.decode();newline='\r\n' if '\r\n' in source else '\n'
    anchor='    unsigned NewThingFromResource(unsigned id) {'
    binding='    type["NewThingFromResource"] = &LuaRetailResources::NewThingFromResource;'
    registration='inline void RegisterRetailResources(sol::state& lua) {'
    for marker in ('#pragma once',anchor,binding,registration):
        if source.count(marker)!=1:raise ValueError('Wife resource insertion changed')
    fragment=Path(__file__).with_name('retail_wife_argument_key_methods.inc').read_text()
    source=source.replace('#pragma once','#pragma once'+newline+'#include "retail_wife_argument_key_lua.h"')
    source=source.replace(anchor,fragment.replace('\n',newline)+newline+anchor)
    source=source.replace(registration,registration+newline+'    RegisterRetailWifeArgumentKey(lua);')
    methods=['WithArgumentKey','AddArgumentKeyLine']
    source=source.replace(binding,''.join('    type["'+m+'"] = &LuaRetailResources::'+m+';'+newline for m in methods)+binding)
    output.mkdir(parents=True,exist_ok=True)
    (output/'LuaRetailResources.h').write_bytes(source.encode())
    patch=''.join(difflib.unified_diff(original.decode().splitlines(True),source.splitlines(True),
        fromfile='a/FableScriptExtender/LuaRetailResources.h',tofile='b/FableScriptExtender/LuaRetailResources.h'))
    helpers={}
    for name in ('retail_wife_argument_key.h','retail_wife_argument_key_lua.h'):
        content=Path(__file__).with_name(name).read_text()
        (output/name).write_text(content)
        helpers[name]=hashlib.sha256((output/name).read_bytes()).hexdigest()
        patch+=''.join(difflib.unified_diff([],content.splitlines(True),fromfile='/dev/null',tofile='b/FableScriptExtender/'+name))
    (output/'resource-integration.patch').write_bytes(patch.encode())
    report.update(status='proposal-only-not-applied',parentProposal=str(base),
        parentCandidateSha256=report['candidateSha256'],candidateSha256=hashlib.sha256(source.encode()).hexdigest(),
        methods=report['methods']+methods,helperHeaders=helpers,
        validation='See work/wife_argument_key_checks/result.json for current compiled resource-method validation; complete DLL/gameplay pending')
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(json.dumps(prepare(),indent=2))
