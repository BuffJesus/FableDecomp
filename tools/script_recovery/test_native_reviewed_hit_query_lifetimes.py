from pathlib import Path
from copy import deepcopy
import hashlib,json,re,sys
import pytest
W=Path(__file__).resolve().parent
from tools.script_recovery import native_reviewed_hit_query_lifetimes as classifier
from tools.script_recovery.lua_staging_liveness import prove_unused_staging
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
from lupa.lua54 import LuaRuntime
F=W/'fixtures/hit_lifetimes/witch'
read=lambda name:json.loads((F/name).read_text(encoding='utf-8'))
digest=lambda data:hashlib.sha256(data).hexdigest()
@pytest.fixture
def inputs():
    class Image(FixtureImage):
        def string_at(self,address):return read('strings.json').get(hex(address))
    image=Image([(int(r['address'],16),bytes.fromhex(r['bytes'])) for r in read('image_parts.json')])
    return dict(source=(F/'phase.lua').read_text(encoding='utf-8'),todo=read('phase_todo.json'),calls=read('phase_calls.json'),
        fn=read('native_function.json'),row=read('candidate_row.json'),unit=read('candidate_unit.json'),owner='Witch',image=image,liveness=prove_unused_staging)
def run(inputs):return classifier.recover(**inputs)
def repin_lua(inputs):
    inputs['row']['luaPhaseSha256']=digest(inputs['source'].encode())
    for scope in inputs['row']['scopes']:scope['luaPrefixSha256']=digest(scope['oldLuaPrefix'].encode())
def change_prefix(inputs,index,old,new):
    scope=inputs['row']['scopes'][index];prefix=scope['oldLuaPrefix'];assert prefix.count(old)==1
    changed=prefix.replace(old,new,1);assert inputs['source'].count(prefix)==1
    inputs['source']=inputs['source'].replace(prefix,changed,1);scope['oldLuaPrefix']=changed;repin_lua(inputs)
def test_two_native_scopes_use_existing_helper_and_keep_numeric_flag_and_exit_locals(inputs):
    out,todo,calls,reviews=run(inputs)
    assert len(reviews)==2 and len(inputs['todo'])-len(todo)==2 and len(inputs['calls'])-len(calls)==4
    assert out.count('resources:IsHitByHeroExceptAbility(me, 14) and 1 or 0')==2
    assert not any('me:'+name+'(' in out for name in classifier.METHODS)
    assert [r['nativeRoles']['temporaryMask'] for r in reviews]==[14,112]
    assert [r['alias'] for r in reviews[0]['preservedBooleanLocals']]==['bVar7','bVar8']
    assert all(r['normalExit'] is False for r in reviews[0]['preservedBooleanLocals'])
    LuaRuntime().compile(out)
def test_absent_metadata_preserves_every_schema(inputs):
    inputs['row']=None
    assert run(inputs)==(inputs['source'],inputs['todo'],inputs['calls'],[])
@pytest.mark.parametrize('index',[0,1])
def test_one_scope_opt_in_preserves_other_prefix(inputs,index):
    other=inputs['row']['scopes'][1-index]['oldLuaPrefix'];inputs['row']['scopes']=[inputs['row']['scopes'][index]]
    out,todo,calls,reviews=run(inputs)
    assert out.count(other)==1 and len(reviews)==1 and len(inputs['calls'])-len(calls)==2
@pytest.mark.parametrize('fault',['native','source','phase','calls','owner','vtable','function','method','ability','literal','literalAddress',
    'missingProfile','badProfile','badProfileBytes','frame','scopeHash','prefixHash','duplicateScope','emptyScopes',
    'objectOffset','mask','sourceCalls','sourceCallDepth','sourceCallObject','sourceCallArgument','flagAlias','deadAlias',
    'missingTodo','duplicateTodo','wrongOccurrence','wrongCallIndex','inlineBreakCount','missingPreservedLocal'])
def test_stale_or_ambiguous_evidence_rejects(inputs,fault):
    row=inputs['row'];scope=row['scopes'][1]
    if fault=='native':row['nativeExtentSha256']='0'*64
    elif fault=='source':row['sourceSha256']='0'*64
    elif fault=='phase':row['luaPhaseSha256']='0'*64
    elif fault=='calls':row['callsSha256']='0'*64
    elif fault=='owner':inputs['owner']='Other'
    elif fault=='vtable':row['entityVtable']='0x1'
    elif fault=='function':inputs['unit']['entities']['Witch']['functions']['Main']['address']='0x1'
    elif fault=='method':row['method']='Other'
    elif fault=='ability':row['ability']=13
    elif fault=='literal':row['literal']='WRONG'
    elif fault=='literalAddress':row['heroLiteral']='0x125d1c9'
    elif fault=='missingProfile':row['helperBodies'].pop()
    elif fault=='badProfile':row['helperBodies'][0]['sha256']='0'*64
    elif fault=='badProfileBytes':row['helperBodies'][0]['bytes']='90'
    elif fault=='frame':row['frameDepth']+=4
    elif fault=='scopeHash':scope['nativeSha256']='0'*64
    elif fault=='prefixHash':scope['luaPrefixSha256']='0'*64
    elif fault=='duplicateScope':row['scopes'].append(deepcopy(scope))
    elif fault=='emptyScopes':row['scopes']=[]
    elif fault=='objectOffset':scope['objectOffsets'][0]+=4
    elif fault=='mask':scope['temporaryMask']=14
    elif fault=='sourceCalls':scope['sourceCalls'][0]['site']='0x1'
    elif fault=='sourceCallDepth':scope['sourceCalls'][0]['metadata']['depth']+=4
    elif fault=='sourceCallObject':scope['sourceCalls'][0]['metadata']['ecxStack']+=4
    elif fault=='sourceCallArgument':scope['sourceCalls'][0]['arguments'][-1]='0'
    elif fault=='flagAlias':scope['flagAlias']='external'
    elif fault=='deadAlias':scope['deadResults'][0]='external'
    elif fault=='missingTodo':inputs['todo'].remove(scope['diagnostics'][0])
    elif fault=='duplicateTodo':inputs['todo'].append(scope['diagnostics'][0])
    elif fault=='wrongOccurrence':scope['callOccurrence']=0
    elif fault=='wrongCallIndex':scope['callListIndex']=row['scopes'][0]['callListIndex']
    elif fault=='inlineBreakCount':row['analysisInlineBreaks']=0
    elif fault=='missingPreservedLocal':row['scopes'][0]['preserveFalse'].pop()
    with pytest.raises(ValueError):run(inputs)
@pytest.mark.parametrize('scope',[0,1])
@pytest.mark.parametrize('fault',['wrongAbility','wrongFlag','wrongHeroBranch','wrongSpecialBranch','extraCall','extraWrite','liveScratch','conditionalLive','externalJump','globalFlag'])
def test_unsafe_lua_changes_reject_even_with_updated_hashes(inputs,scope,fault):
    row=inputs['row'];s=row['scopes'][scope]
    if fault=='wrongAbility':change_prefix(inputs,scope,'SpecialAbility(0xe)','SpecialAbility(13)')
    elif fault=='wrongFlag':change_prefix(inputs,scope,s['flagAlias']+' = 1',s['flagAlias']+' = 0')
    elif fault=='wrongHeroBranch':change_prefix(inputs,scope,'if not cVar5 then','if cVar5 then')
    elif fault=='wrongSpecialBranch':
        target='bVar4' if scope==0 else 'bVar8';change_prefix(inputs,scope,'if not '+target+' then goto','if '+target+' then goto')
    elif fault=='extraCall':change_prefix(inputs,scope,'cVar5 = me:MsgIsHitByHero()','cVar5 = me:MsgIsHitByHero()\nquest:Observe()')
    elif fault=='extraWrite':change_prefix(inputs,scope,'cVar5 = me:MsgIsHitByHero()','cVar5 = me:MsgIsHitByHero()\nunknown = 1')
    elif fault in ['liveScratch','conditionalLive']:
        alias=s['deadResults'][0];addition=f'quest:Observe({alias})\n'
        if fault=='conditionalLive':addition='if value then\n'+addition+'end\n'
        at=inputs['source'].index(s['oldLuaPrefix'])+len(s['oldLuaPrefix']);inputs['source']=inputs['source'][:at]+addition+inputs['source'][at:];repin_lua(inputs)
    elif fault=='externalJump':
        label=re.findall(r'::(\w+)::',s['oldLuaPrefix'])[0]
        at=inputs['source'].index(s['oldLuaPrefix']);inputs['source']=inputs['source'][:at]+'if value then goto '+label+' end\n'+inputs['source'][at:];repin_lua(inputs)
    elif fault=='globalFlag':
        inputs['source']=inputs['source'].replace(', '+s['flagAlias']+',',',',1);repin_lua(inputs)
    with pytest.raises(ValueError):run(inputs)
@pytest.mark.parametrize('fault',['nilInitializer','numericInitializer','numericWrite','parallelWrite','shadowLocal','changedCleanup'])
def test_first_scope_boolean_exit_invariant_is_required(inputs,fault):
    if fault=='nilInitializer':inputs['source']=inputs['source'].replace('    bVar7 = false\n','    bVar7 = nil\n',1)
    elif fault=='numericInitializer':inputs['source']=inputs['source'].replace('    bVar8 = false\n','    bVar8 = 0\n',1)
    elif fault=='numericWrite':inputs['source']=inputs['source'].replace('bVar8 = not alive','bVar8 = 0',1)
    elif fault=='parallelWrite':inputs['source']=inputs['source'].replace('    bVar7 = false\n','    bVar7, bVar8 = false, false\n',1)
    elif fault=='shadowLocal':inputs['source']=inputs['source'].replace('    bVar7 = false\n','    local bVar7 = false\n',1)
    elif fault=='changedCleanup':change_prefix(inputs,0,'bVar8 = false','bVar8 = true')
    repin_lua(inputs)
    with pytest.raises(ValueError):run(inputs)
@pytest.mark.parametrize('index',[0,1])
@pytest.mark.parametrize('fault',['badFalse','badTrue','badAbility','badMaskSpill','badQuerySlot','missingTest'])
def test_native_semantics_reject_after_rehash(inputs,index,fault):
    body=inputs['fn'];start=int(body['address'],16);original=inputs['image'];raw=bytearray(original.bytes_at(start,int(body['bodyEndExclusive'],16)-start))
    controls=[dict(badFalse=(0xecef5e,'01'),badTrue=(0xecef65,'00'),badAbility=(0xecef49,'0d'),badMaskSpill=(0xeceefc,'17'),badQuerySlot=(0xeceeff,'50'),missingTest=(0xecef00,'9090')),
        dict(badFalse=(0xecf573,'01'),badTrue=(0xecf57a,'00'),badAbility=(0xecf55e,'0d'),badMaskSpill=(0xecf511,'17'),badQuerySlot=(0xecf514,'50'),missingTest=(0xecf515,'9090'))]
    address,text=controls[index][fault];data=bytes.fromhex(text);raw[address-start:address-start+len(data)]=data
    class Mutant:
        def bytes_at(self,address,size):
            if start<=address and address+size<=start+len(raw):return bytes(raw[address-start:address-start+size])
            return original.bytes_at(address,size)
        def string_at(self,address):return original.string_at(address)
    inputs['image']=Mutant();inputs['row']['nativeExtentSha256']=digest(raw)
    for scope in inputs['row']['scopes']:
        a,b=int(scope['start'],16),int(scope['endExclusive'],16);scope['nativeSha256']=digest(raw[a-start:b-start])
    with pytest.raises(ValueError):run(inputs)
def test_equivalent_no_index_sib_encoding_is_accepted(inputs):
    original=inputs['image'];start=int(inputs['fn']['address'],16);raw=bytearray(original.bytes_at(start,int(inputs['fn']['bodyEndExclusive'],16)-start))
    raw[0xeceede-start]=0x64
    class Equivalent:
        def bytes_at(self,address,size):
            if start<=address and address+size<=start+len(raw):return bytes(raw[address-start:address-start+size])
            return original.bytes_at(address,size)
        def string_at(self,address):return original.string_at(address)
    inputs['image']=Equivalent();inputs['row']['nativeExtentSha256']=digest(raw)
    scope=inputs['row']['scopes'][0];a,b=int(scope['start'],16),int(scope['endExclusive'],16);scope['nativeSha256']=digest(raw[a-start:b-start])
    assert len(run(inputs)[3])==2
