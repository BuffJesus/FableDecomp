from copy import deepcopy
from types import SimpleNamespace
from unittest.mock import patch
import hashlib
import pytest
from tools.script_recovery import native_reviewed_saved_table_staging as C
from tools.script_recovery import convert_quest_unit
from tools.script_recovery.lua_staging_liveness import prove_unused_staging
from tools.script_recovery.native_cleanup_regions import hoist_cleanup_regions
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
sha=lambda b:hashlib.sha256(b).hexdigest()

def fixture():
 # The cache/ABI engines are isolated here; their own suites and the actual
 # original-image proof exercise those contracts independently.
 start=0x1000;wrapper=0x2000;accessor=0x2100;pred_tail=0x2200;table=0x3000;api=0x4000
 raw=bytes.fromhex('8bf1e8f90f000084c07501c38b4c24388b116a00ff92ec050000c3')
 pointer=0x100c;tail=0x1012;dispatch=0x1014
 predicate=dict(wrapper=dict(address=hex(wrapper),bytes='8b01ff500c8bc8e904020000'),accessor=dict(address=hex(accessor),bytes='8b4114c3'),tail=dict(address=hex(pred_tail),bytes='8b412c85c0750332c0c38a4005c3'),vtable=hex(table),accessorOffset=20,stateOffset=44,flagOffset=5)
 # wrapper JMP displacement from 0x2007 + 5 to 0x2200.
 predicate['wrapper']['bytes']='8b01ff500c8bc8e9f4010000'
 parts=[(start,raw),(table+12,accessor.to_bytes(4,'little')),(C.VTABLE_BASE+0x5ec,api.to_bytes(4,'little')),(api,b'\x90'*16)]
 parts += [(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in [predicate[k] for k in ['wrapper','accessor','tail']]]
 image=FixtureImage(parts)
 fragment='flag = Poll(this); if (flag) { value = *saved; receiver = saved; goto LAB_00001012; }'
 region='    alive = not quest:IsActiveThreadTerminating()\n    stop = not alive\n    if stop then\n        -- TODO(native): value = *cache\n        value = nil --[[unresolved native value]]\n        -- TODO(native): goto LAB_00001012\n    end\n'
 source='function Main()\n    local value\n'+region+'    -- LAB_00001012: (native jump target)\n    quest:PauseAllNonScriptedEntities(false)\n    return\nend\n'
 row=dict(imageSha256=sha(image.data),sourceSha256=sha(fragment.encode()),luaPhaseSha256=sha(source.encode()),scopeLimit='ABI in-bounds unverified',cacheEvidenceSha256='1'*64,prefixEvidenceSha256='2'*64,cacheProof=dict(interfaceOffset=4,nativeSha256=sha(raw),pointerRead=hex(pointer),tableRead=hex(pointer+4),tableRegister='edx',cacheStore='0x1000'),sharedTail=hex(tail),dispatch=hex(dispatch),readBranchBytes=6,readShape='guarded_fallthrough',api=dict(method='PauseAllNonScriptedEntities',slot='0x5ec',target=hex(api),prefix='90'*16),predicate=predicate,nativePredicateBranches=[dict(call='0x1002',branch='0x1009',bytes=raw[2:11].hex())],copies=[],alias='value',cacheAlias='cache',sourceCacheAlias='saved',nativeSourceFragment=fragment,sourceConditionAlias='flag',sourceReceiverAlias='receiver',sourcePredicate='Poll',luaConditionAlias='stop',luaAliveAlias='alive',diagnosticMultiplicity=1,originalLuaRegion=region)
 return dict(source=source,todo=['unlifted: value = *cache'],fn=dict(address=hex(start),bodyEndExclusive=hex(start+len(raw)),decompile=fragment),row=row,spec=SimpleNamespace(entity=True),image=image,copy_trace=[])

def run(b):
 with patch.object(C,'resolve',return_value=({},[])),patch.object(C,'prove',return_value=dict(isolatedForShapeTest=True)),patch.object(C,'analyze',return_value=dict(states={0x1002:dict(ecx=('entry_this',))})),patch.object(convert_quest_unit,'callee_stack_words',return_value=1):
  return C.classify(**b,liveness=prove_unused_staging,hoister=hoist_cleanup_regions)

def test_guarded_fallthrough_annotation_retains_executable_source_and_posthoist_liveness():
 b=fixture();out,todo,reviews=run(b)
 assert todo==[] and len(reviews)==1 and 'value = nil' in out
 assert reviews[0]['executableLuaUnchanged'] and reviews[0]['aliases'][0]['postHoistLiveness']['liveness']['copiedAliases']==[]

@pytest.mark.parametrize('key,value',[
 ('readShape','other'),('readBranchBytes',7),('sharedTail','0x1013'),('dispatch','0x1015'),
 ('sourceConditionAlias','wrong'),('sourceReceiverAlias','wrong'),('sourcePredicate','Other'),
 ('sourceCacheAlias','wrong'),('luaConditionAlias','alive'),('luaAliveAlias','other'),
 ('copies',[{}]),('diagnosticMultiplicity',2),('nativePredicateBranches',[]),
])
def test_changed_guard_contract_refuses(key,value):
 b=fixture();b['row'][key]=value
 with pytest.raises(ValueError):run(b)

@pytest.mark.parametrize('old,new',[
 ('stop = not alive','stop = alive'),('if stop then','if alive then'),
 ('value = nil','value = nil; observe(value)'),('    return\n','    observe(value)\n    return\n'),
 ('-- TODO(native): goto LAB_00001012','-- TODO(native): goto LAB_00001013'),
])
def test_changed_lua_guard_or_observable_alias_refuses_even_with_updated_phase_seal(old,new):
 b=fixture();b['source']=b['source'].replace(old,new);b['row']['luaPhaseSha256']=sha(b['source'].encode())
 b['row']['originalLuaRegion']=b['row']['originalLuaRegion'].replace(old,new)
 with pytest.raises(ValueError):run(b)

def test_unexpected_copy_trace_refuses():
 b=fixture();b['copy_trace']=[dict(target='LAB_0000100b')]
 with pytest.raises(ValueError):run(b)

@pytest.mark.parametrize('kind',['call_into_table_read','jump_into_dispatch','unverified_indirect_jump'])
def test_control_transfers_cannot_bypass_reload_or_argument_with_updated_native_seals(kind):
 b=fixture();image=b['image'];raw=bytearray(image.bytes_at(0x1000,26))
 if kind=='call_into_table_read':raw[3:7]=(0x1010-0x1007).to_bytes(4,'little',signed=True)
 elif kind=='jump_into_dispatch':raw[10]=0x1014-0x100b
 else:raw[:2]=bytes.fromhex('ffe0')
 # Rebuild the synthetic image,so exact-byte seals cannot mask semantic guard failures.
 original=fixture()['image']
 parts=[(0x1000,bytes(raw)),(0x300c,original.bytes_at(0x300c,4)),(C.VTABLE_BASE+0x5ec,original.bytes_at(C.VTABLE_BASE+0x5ec,4)),(0x4000,original.bytes_at(0x4000,16))]
 parts += [(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in [b['row']['predicate'][k] for k in ['wrapper','accessor','tail']]]
 b['image']=FixtureImage(parts);b['row']['imageSha256']=sha(b['image'].data);b['row']['cacheProof']['nativeSha256']=sha(raw)
 with pytest.raises(ValueError):run(b)

@pytest.mark.parametrize('old,new',[
 ('if (flag)','if (!flag)'),('value = *saved','value = **saved'),
 ('receiver = saved','receiver = other'),('goto LAB_00001012','goto LAB_00001013'),
])
def test_exported_guard_or_pointer_copy_changes_refuse_even_with_resealed_source(old,new):
 b=fixture();fragment=b['fn']['decompile'].replace(old,new);b['fn']['decompile']=fragment;b['row']['nativeSourceFragment']=fragment;b['row']['sourceSha256']=sha(fragment.encode())
 with pytest.raises(ValueError):run(b)
