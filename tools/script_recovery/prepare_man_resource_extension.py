"""Prepare, but never apply, the explicit-resource action extension."""
import argparse
import difflib
import hashlib
import json
from pathlib import Path

METHODS=('MoveToPosition','PlayAnimation','ClearCommands','ClearAllActions','ThingIsDistanceFromPositionOver',
         'ReadAnimationArgument5','IsHitByHeroExceptAbility', 'ThingsAreWithinDistance',
         'FaceThing','AddConversationPerson','AddConversationLine')
ANIMATION_READS={0xDB1697:'8a1548573701',0xDB1BEB:'8a0d48573701',0xDB1C41:'8a0d48573701'}
HIT_REGION=(0xDB0B30,0xEC,'7a15201228e55f9cafa7ec92caf2bb70254f0c5c8bcc1bae10fffe6c14ff28ca')
WRAPPERS={
    'MoveToPosition':(0x7e72f0,'8b490885c974058b01ff6010c21400'),
    'PlayAnimation':(0x7e73d0,'8b490885c974058b01ff6048c22000'),
    'ClearCommands':(0x7e7360,'8b490885c974058b01ff6028c3'),
    'ClearAllActions':(0x7e7400,'8b490885c974058b01ff6054c3'),
    # Prologue of the __fastcall Thing distance helper: IsAlive slot 0x12c, then GetPos slot 0x18.
    'ThingIsDistanceFromPositionOver':(0xcbe45c,'56578bf98b078bf2ff902c010000')}


def prepare(runtime,output,rdata=None):
    if rdata is None:
        from .lift_native_lua import RData
        rdata=RData()
    for name,(address,hexbytes) in WRAPPERS.items():
        expected=bytes.fromhex(hexbytes)
        if rdata.bytes_at(address,len(expected))!=expected:
            raise ValueError(f'Native {name} wrapper changed')
    for address, hexbytes in ANIMATION_READS.items():
        expected=bytes.fromhex(hexbytes)
        if rdata.bytes_at(address,len(expected))!=expected:
            raise ValueError('Native animation argument byte read changed')
    address,size,expected=HIT_REGION
    raw=rdata.bytes_at(address,size)
    if raw is None or hashlib.sha256(raw).hexdigest()!=expected:
        raise ValueError('Native hero hit query/string lifetime changed')
    header=Path(runtime)/'FableScriptExtender/LuaRetailResources.h'
    raw=header.read_bytes()
    source=raw.decode('utf-8')
    newline='\r\n' if '\r\n' in source else '\n'
    if any(f' {name}(' in source or f'type["{name}"]' in source for name in METHODS):
        raise ValueError('Runtime already contains an action extension; review that implementation first')
    method_anchor='    unsigned NewThingFromResource(unsigned id) {'
    bind_anchor='    type["NewThingFromResource"] = &LuaRetailResources::NewThingFromResource;'
    if source.count(method_anchor)!=1 or source.count(bind_anchor)!=1:
        raise ValueError('Resource bridge anchors changed')
    fragment=Path(__file__).with_name('retail_resource_actions.inc').read_text()
    fragment+=Path(__file__).with_name('retail_thing_actions.inc').read_text()
    candidate=source.replace(method_anchor,fragment.replace('\n',newline)+newline+method_anchor)
    position = Path(__file__).with_name('retail_thing_position.inc').read_text()
    witness = json.loads(Path(__file__).with_name('native_affair_wife_position_witness.json').read_text())
    for region in witness['regions']:
        raw_position = rdata.bytes_at(region['address'], region['size'])
        if raw_position is None or hashlib.sha256(raw_position).hexdigest() != region['sha256']:
            raise ValueError('Native Thing position/fallback changed')
    class_anchor = 'class LuaRetailResources'
    registration_anchor = '    RegisterRetailFlags(lua);'
    if (source.count(class_anchor) != 1 or source.count(registration_anchor) != 1
            or 'RetailThingPosition' in source):
        raise ValueError('Retail position bridge anchors changed')
    candidate = candidate.replace(class_anchor, position.replace('\n', newline) + newline + class_anchor)
    candidate = candidate.replace(registration_anchor, registration_anchor + newline +
        '    lua.set_function("RetailThingPosition", &ReadRetailThingPosition);')
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
            'ClearAllActions':'0x007E7400 slot0x54 RET0',
            'ThingIsDistanceFromPositionOver':'0x00CBE45C __fastcall(CScriptThing*,const C3DVector*,float) RET4'},
        'globalBindings': ['RetailThingPosition'],
        'positionFragmentSha256': hashlib.sha256(position.encode()).hexdigest(),
        'nativeBytes':{name:{'address':address,'bytes':value} for name,(address,value) in WRAPPERS.items()},
        'animationArgument5':{'address':'0x01375748','type':'volatile unsigned byte, converted to bool',
            'readSites':{hex(k):v for k,v in ANIMATION_READS.items()},
            'meaning':'fifth PlayAnimation boolean; gameplay meaning and writers are not established',
            'policy':'read current relocated byte on every call; never cache the initial image value'},
        'heroHitQuery':{'region':hex(HIT_REGION[0]),'size':HIT_REGION[1],'sha256':HIT_REGION[2],
            'slots':['0x54','0xA8','0xA4'],'excludedAbility':14,
            'ownedTemporaries':'CCharString hero names, conditionally constructed and destroyed in reverse order'},
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
