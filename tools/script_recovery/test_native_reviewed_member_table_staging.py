from pathlib import Path
from copy import deepcopy
from tools.script_recovery.native_evidence_lowering import LoweringSpec
import hashlib,json,re,sys
import pytest
W=Path(__file__).resolve().parent;R=W.parents[1];sys.path.insert(0,str(R))
from tools.script_recovery import native_reviewed_member_table_staging as classifier
from tools.script_recovery import convert_quest_unit as converter
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
from tools.script_recovery.lift_native_lua import RData
from lupa.lua54 import LuaRuntime
F=W/'fixtures/member_table/witch'
read=lambda name:json.loads((F/name).read_text())
digest=lambda raw:hashlib.sha256(raw).hexdigest()
@pytest.fixture
def inputs(monkeypatch):
    def reject_retail_read(*args,**kwargs):raise AssertionError("Portable classifier case attempted a retail image open")
    monkeypatch.setattr(RData,"__init__",reject_retail_read)
    class Image(FixtureImage):
        def string_at(self,address):return read('strings.json').get(hex(address))
    image=Image([(int(r['address'],16),bytes.fromhex(r['bytes'])) for r in read('image_parts.json')])
    abi=read('child_abi_facts.json')
    monkeypatch.setattr(converter,'callee_stack_words',lambda target:abi[hex(target)])
    return dict(source=(F/'phase.lua').read_text(),todo=read('phase_todo.json'),fn=read('native_function.json'),row=read('candidate_row.json'),
        speech_input=read('speech_input.json')['source'],parent=read('member_review.json'),spec=LoweringSpec(read('candidate_unit.json'),'Witch',entity=True,thing_slots={}),image=image,unit=read('candidate_unit.json'),owner='Witch')
def run(inputs):return classifier.classify(**inputs)
def repin(inputs):
    row=inputs['row'];row['luaPhaseSha256']=digest(inputs['source'].encode());row['speechInputSha256']=digest(inputs['speech_input'].encode())
    row['exportedSourceSha256']=digest(inputs['fn']['decompile'].encode());inputs['parent']['sourceSha256']=row['exportedSourceSha256']
    row['parentReviewSha256']=digest(json.dumps(inputs['parent'],sort_keys=True,separators=(',',':')).encode())
def test_native_checked_alias_free_comment_only(inputs):
    out,todo,reviews=run(inputs)
    assert len(reviews)==1 and len(inputs['todo'])-len(todo)==1
    assert classifier.significant(out)==classifier.significant(inputs['source'])
    assert out.count('-- reviewed native member speech table: xStack_60')==1
    assert reviews[0]['resource']['offset']==88 and reviews[0]['executableAliasTokens']==0
    assert reviews[0]['commentOnly'] is True
    LuaRuntime().compile(out)
def test_missing_metadata_preserves_schemas(inputs):
    inputs['row']=None;assert run(inputs)==(inputs['source'],inputs['todo'],[])
@pytest.mark.parametrize('fault',['owner','vtable','function','native','exported','phase','normalized','parent','missingParent','emptyReads','duplicateReads','site','dispatch','alias','fragment','assignment','exportedAlias','diagnostic','missingTodo','duplicateTodo','resource','parentOffset'])
def test_stale_or_ambiguous_review_rejects(inputs,fault):
    row=inputs['row'];read=row['reads'][0]
    if fault=='owner':inputs['owner']='Other'
    elif fault=='vtable':row['entityVtable']='0x1'
    elif fault=='function':inputs['unit']['entities']['Witch']['functions']['Main']['address']='0x1'
    elif fault=='native':row['nativeExtentSha256']='0'*64
    elif fault=='exported':row['exportedSourceSha256']='0'*64
    elif fault=='phase':row['luaPhaseSha256']='0'*64
    elif fault=='normalized':row['speechInputSha256']='0'*64
    elif fault=='parent':row['parentReviewSha256']='0'*64
    elif fault=='missingParent':inputs['parent']=None
    elif fault=='emptyReads':row['reads']=[]
    elif fault=='duplicateReads':row['reads'].append(deepcopy(read))
    elif fault=='site':read['nativeRead']='0xecf0ff'
    elif fault=='dispatch':read['dispatch']='0xecf12b'
    elif fault=='alias':read['alias']='other'
    elif fault=='fragment':read['nativeSourceFragment']='missing;'
    elif fault=='assignment':read['normalizedAssignment']='missing;'
    elif fault=='exportedAlias':read['exportedAlias']='other'
    elif fault=='diagnostic':read['diagnostic']='unlifted: missing;'
    elif fault=='missingTodo':inputs['todo'].remove(read['diagnostic'])
    elif fault=='duplicateTodo':inputs['todo'].append(read['diagnostic'])
    elif fault=='resource':inputs['spec'].parent_resources[88]='wrong'
    elif fault=='parentOffset':inputs['spec'].parent_off=24
    with pytest.raises(ValueError):run(inputs)
@pytest.mark.parametrize('statement',['quest:Observe(xStack_60)','if alive then\nquest:Observe(xStack_60)\nend','xStack_60 = nil','local xStack_60','local callback = function() return xStack_60 end'])
def test_executable_alias_presence_rejects_after_rehash(inputs,statement):
    inputs['source']=inputs['source'].replace(inputs['row']['reads'][0]['diagnostic'][10:]+'\n',inputs['row']['reads'][0]['diagnostic'][10:]+'\n'+statement+'\n',1)
    repin(inputs)
    with pytest.raises(ValueError,match='executable presence'):run(inputs)
def test_alias_text_in_strings_and_comments_is_harmless(inputs):
    inputs['source']=inputs['source'].replace('function Main(quest, me)\n','function Main(quest, me)\n    -- xStack_60\n    quest:Trace("xStack_60")\n',1)
    repin(inputs);assert len(run(inputs)[2])==1
@pytest.mark.parametrize('fault',['assignmentField','assignmentParent','exportedField','exportedParent','tableAlias','duplicateExport','markerField','wrongNativeCallReview'])
def test_changed_source_correspondence_rejects_after_rehash(inputs,fault):
    read=inputs['row']['reads'][0]
    if fault in ['assignmentField','assignmentParent']:
        old=read['normalizedAssignment'];new=old.replace('0x58','0x48') if fault=='assignmentField' else old.replace('0x14','0x18')
        inputs['speech_input']=inputs['speech_input'].replace(old,new);read['normalizedAssignment']=new
    elif fault in ['exportedField','exportedParent']:
        old=read['nativeSourceFragment'];new=old.replace('0x58','0x48') if fault=='exportedField' else old.replace('0x14','0x18')
        inputs['fn']['decompile']=inputs['fn']['decompile'].replace(old,new);read['nativeSourceFragment']=new
    elif fault=='tableAlias':
        call=next(c for c in inputs['parent']['calls'] if int(c['site'],16)==0xecf12a)
        old=call['old'];new=old.replace('(int)xStack_60','(int)other')
        inputs['speech_input']=inputs['speech_input'].replace(old,new);call['old']=new
    elif fault=='duplicateExport':inputs['fn']['decompile']+='\n'+read['nativeSourceFragment']
    elif fault=='markerField':
        old=read['diagnostic'];new=old.replace('0x58','0x48');inputs['todo']=[new if x==old else x for x in inputs['todo']]
        inputs['source']=inputs['source'].replace(old[10:],new[10:]);read['diagnostic']=new
    elif fault=='wrongNativeCallReview':next(c for c in inputs['parent']['calls'] if int(c['site'],16)==0xecf12a)['operands'][-1][1]=1
    repin(inputs)
    with pytest.raises(ValueError):run(inputs)
@pytest.mark.parametrize('address,replacement',[(0xecf0fe,'5c'),(0xecf10e,'6c'),(0xecf126,'74'),(0xecf129,'5c'),(0xecf12c,'38'),(0xecf104,'00'),(0xecf118,'d8'),(0xecf111,'90'),(0xecf107,'90')])
def test_native_operand_semantics_reject_after_rehash(inputs,address,replacement):
    original=inputs['image'];start=int(inputs['fn']['address'],16);raw=bytearray(original.bytes_at(start,4086));raw[address-start]=int(replacement,16)
    class Mutant:
        def bytes_at(self,a,n):
            if start<=a and a+n<=start+len(raw):return bytes(raw[a-start:a-start+n])
            return original.bytes_at(a,n)
        def string_at(self,a):return original.string_at(a)
    inputs['image']=Mutant();inputs['row']['nativeExtentSha256']=digest(raw);inputs['parent']['nativeSha256']=digest(raw)
    from capstone import Cs,CS_ARCH_X86,CS_MODE_32
    by={i.address:i for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(raw),start)}
    for call in inputs['parent']['calls']:
        a,b=int(call['start'],16),int(call['site'],16)
        if b in by:call['windowSha256']=digest(raw[a-start:b+by[b].size-start])
    for site in inputs['parent']['deadTableLoads']['sites']:
        a,b=int(site['start'],16),int(site['call'],16)
        if b in by:site['windowSha256']=digest(raw[a-start:b+by[b].size-start])
    repin(inputs)
    with pytest.raises(ValueError):run(inputs)
@pytest.mark.parametrize('target,value',[(0x891ca0,1),(0x7e7390,5)])
def test_child_purge_facts_must_match(inputs,monkeypatch,target,value):
    abi=read('child_abi_facts.json');abi[hex(target)]=value
    monkeypatch.setattr(converter,'callee_stack_words',lambda t:abi[hex(t)])
    with pytest.raises(ValueError):run(inputs)
