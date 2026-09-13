"""Prepare, but never apply, the explicit-resource action extension."""
import argparse
import difflib
import hashlib
import json
from pathlib import Path

METHODS=('MoveToPosition','PlayAnimation','ClearCommands','ClearAllActions')
WRAPPERS={
    'MoveToPosition':(0x7e72f0,'8b490885c974058b01ff6010c21400'),
    'PlayAnimation':(0x7e73d0,'8b490885c974058b01ff6048c22000'),
    'ClearCommands':(0x7e7360,'8b490885c974058b01ff6028c3'),
    'ClearAllActions':(0x7e7400,'8b490885c974058b01ff6054c3')}


def prepare(runtime,output,rdata=None):
    if rdata is None:
        from .lift_native_lua import RData
        rdata=RData()
    for name,(address,hexbytes) in WRAPPERS.items():
        expected=bytes.fromhex(hexbytes)
        if rdata.bytes_at(address,len(expected))!=expected:
            raise ValueError(f'Native {name} wrapper changed')
    header=Path(runtime)/'FableScriptExtender/LuaRetailResources.h'
    raw=header.read_bytes()
    source=raw.decode('utf-8')
    newline='\r\n' if '\r\n' in source else '\n'
    if any(f'void {name}(' in source or f'type["{name}"]' in source for name in METHODS):
        raise ValueError('Runtime already contains an action extension; review that implementation first')
    method_anchor='    unsigned NewThingFromResource(unsigned id) {'
    bind_anchor='    type["NewThingFromResource"] = &LuaRetailResources::NewThingFromResource;'
    if source.count(method_anchor)!=1 or source.count(bind_anchor)!=1:
        raise ValueError('Resource bridge anchors changed')
    fragment=Path(__file__).with_name('retail_resource_actions.inc').read_text()
    candidate=source.replace(method_anchor,fragment.replace('\n',newline)+newline+method_anchor)
    bindings=newline.join(f'    type["{name}"] = &LuaRetailResources::{name};' for name in METHODS)
    candidate=candidate.replace(bind_anchor,bindings+newline+bind_anchor)
    patch=''.join(difflib.unified_diff(source.splitlines(True),candidate.splitlines(True),
        fromfile='a/FableScriptExtender/LuaRetailResources.h',tofile='b/FableScriptExtender/LuaRetailResources.h'))
    output=Path(output)
    output.mkdir(parents=True,exist_ok=True)
    (output/'LuaRetailResources.h').write_bytes(candidate.encode('utf8'))
    (output/'resource-actions.patch').write_bytes(patch.encode('utf8'))
    metadata={'status':'proposal-only-not-applied','source':str(header),
        'sourceSha256':hashlib.sha256(raw).hexdigest(),'fragmentSha256':hashlib.sha256(fragment.encode()).hexdigest(),
        'methods':list(METHODS),'nativeWrappers':{'MoveToPosition':'0x007E72F0 slot0x10 RET0x14',
            'PlayAnimation':'0x007E73D0 slot0x48 RET0x20','ClearCommands':'0x007E7360 slot0x28 RET0',
            'ClearAllActions':'0x007E7400 slot0x54 RET0'},
        'nativeBytes':{name:{'address':address,'bytes':value} for name,(address,value) in WRAPPERS.items()},
        'requirements':['Build the complete runtime for x86 and verify ABI/bindings before deployment',
            'Lower native resource creation, acquisition, uses and destruction together',
            'Do not mix explicit resource actions with cached entity control',
            'Keep generated New Oakvale registration disabled'],
        'limitations':['Forwarding harness uses stub types, not engine ABI','No DLL built or deployed']}
    (output/'proposal.json').write_text(json.dumps(metadata,indent=2)+'\n',encoding='utf8')
    return metadata


if __name__=='__main__':
    parser=argparse.ArgumentParser()
    parser.add_argument('--runtime',type=Path,default=Path('D:/Code/ForgeFSE-retail-shadow'))
    parser.add_argument('--output',type=Path,default=Path('work/man_resource_extension'))
    args=parser.parse_args()
    print(json.dumps(prepare(args.runtime,args.output),indent=2))
