"""Prepare, never apply, the owned timer resource-class integration patch."""
import difflib
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT


def prepare():
    target=Path('D:/Code/ForgeFSE-retail-shadow/FableScriptExtender/LuaRetailResources.h')
    original=target.read_text();candidate=original
    edits=[('#include "LuaRetailFlags.h"','#include "LuaRetailFlags.h"\n#include "retail_owned_timer.h"'),
           ('public:\n','public:\n'+Path(__file__).with_name('retail_owned_timer_methods.inc').read_text()),
           ('    RegisterRetailFlags(lua);','    RegisterRetailFlags(lua);\n    RegisterRetailOwnedTimer(lua);'),
           ('    type["NewResource"] = &LuaRetailResources::NewResource;',
            '    type["NewResource"] = &LuaRetailResources::NewResource;\n    type["WithTimer"] = &LuaRetailResources::WithTimer;')]
    if 'WithTimer' in original or 'RetailOwnedTimer' in original:raise ValueError('Timer integration already present')
    for old,new in edits:
        if candidate.count(old)!=1:raise ValueError('Timer integration target shape changed')
        candidate=candidate.replace(old,new)
    out=ROOT/'work/owned_timer_proposal';out.mkdir(parents=True,exist_ok=True)
    header=Path(__file__).with_name('retail_owned_timer.h').read_text()
    (out/'LuaRetailResources.h').write_text(candidate)
    (out/'retail_owned_timer.h').write_text(header)
    patch=''.join(difflib.unified_diff(original.splitlines(True),candidate.splitlines(True),
        fromfile='a/FableScriptExtender/LuaRetailResources.h',tofile='b/FableScriptExtender/LuaRetailResources.h'))
    patch+=''.join(difflib.unified_diff([],header.splitlines(True),fromfile='/dev/null',tofile='b/FableScriptExtender/retail_owned_timer.h'))
    (out/'timer-integration.patch').write_text(patch)
    report={'status':'prepared-unapplied','target':str(target),
        'baseSha256':hashlib.sha256(original.encode()).hexdigest(),
        'candidateSha256':hashlib.sha256(candidate.encode()).hexdigest(),
        'runtimeCheck':'work/owned_timer_runtime_checks/result.json',
        'remaining':'Compose with the other staged resource extensions; full resource-class/DLL registration and gameplay checks remain.'}
    (out/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    if target.read_text()!=original:raise ValueError('Timer target changed during preparation')
    return report


if __name__=='__main__':print(json.dumps(prepare(),indent=2))
