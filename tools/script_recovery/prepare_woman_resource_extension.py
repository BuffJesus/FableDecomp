"""Stage the owned position method over the checked combined resource proposal."""
import difflib
import hashlib
import json
from pathlib import Path

from tools.script_recovery.lift_native_lua import ROOT


def prepare(base=ROOT/'work/man_resource_integration', output=ROOT/'work/woman_resource_integration',
            runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    base,output=Path(base),Path(output)
    report=json.loads((base/'proposal.json').read_text())
    candidate=(base/'LuaRetailResources.h').read_bytes()
    if hashlib.sha256(candidate).hexdigest()!=report['candidateSha256']:
        raise ValueError('Combined resource proposal changed')
    header=Path(runtime)/'FableScriptExtender/LuaRetailResources.h'
    original=header.read_bytes()
    if hashlib.sha256(original).hexdigest()!=report['sourceSha256']:
        raise ValueError('Runtime resource bridge changed')
    source=candidate.decode();newline='\r\n' if '\r\n' in source else '\n'
    anchor='    unsigned NewThingFromResource(unsigned id) {'
    binding='    type["NewThingFromResource"] = &LuaRetailResources::NewThingFromResource;'
    if source.count(anchor)!=1 or source.count(binding)!=1 or ' ThingPosition(' in source:
        raise ValueError('Owned position insertion point changed')
    fragment=Path(__file__).with_name('retail_owned_thing_position.inc').read_text()
    source=source.replace(anchor,fragment.replace('\n',newline)+newline+anchor)
    source=source.replace(binding,'    type["ThingPosition"] = &LuaRetailResources::ThingPosition;'+newline+binding)
    output.mkdir(parents=True,exist_ok=True)
    (output/'LuaRetailResources.h').write_bytes(source.encode())
    patch=''.join(difflib.unified_diff(original.decode().splitlines(True),source.splitlines(True),
        fromfile='a/FableScriptExtender/LuaRetailResources.h',tofile='b/FableScriptExtender/LuaRetailResources.h'))
    (output/'resource-integration.patch').write_bytes(patch.encode())
    report.update(status='owned-position-proposal-pending-compiled-validation',
        source=str(header),
        parentProposal=str(base),parentCandidateSha256=report['candidateSha256'],
        candidateSha256=hashlib.sha256(source.encode()).hexdigest(),
        methods=report['methods']+['ThingPosition'],ownedPositionFragmentSha256=hashlib.sha256(fragment.encode()).hexdigest())
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(json.dumps(prepare(),indent=2))
