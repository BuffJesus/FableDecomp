from pathlib import Path
import hashlib,json,re,sys
import pytest
from lupa.lua54 import LuaRuntime
R=Path(__file__).resolve().parents[2]
from tools.script_recovery import native_reviewed_parent_member_health_lifetimes as MODULE
from tools.script_recovery import native_parent_member_frame_contracts as FRAME
from tools.script_recovery import native_register_snapshot_provenance as PROVENANCE
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
from tools.script_recovery.lift_native_lua import RData
F=Path(__file__).resolve().parent/'fixtures/witch_health';read=lambda p:json.loads(p.read_text());serial=MODULE.serial;sha=lambda b:hashlib.sha256(b).hexdigest()
@pytest.fixture(autouse=True)
def no_retail(monkeypatch):monkeypatch.setattr(RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))
def inputs():
 p=read(F/'phase.json');row=read(F/'candidate_row.json');return dict(source=(F/'phase.lua').read_text(),todo=p['todo'],calls=p['calls'],fn=p['fn'],row=row,unit=read(F/'candidate_unit.json'),owner=row['owner'],image=FixtureImage([(int(i['address'],16),bytes.fromhex(i['bytes'])) for i in read(F/'image_parts.json')]),manifest={'GetHealth':row['hostContract']},before=p['before'],after=p['after'])
def run(d):return MODULE.recover(**d)
def frame(d):
 r=d['row'];fn=d['fn'];a,b=int(fn['address'],16),int(fn['bodyEndExclusive'],16);return FRAME.prove(d['image'].bytes_at(a,b-a),fn,d['image'],r['frameProfile'],entity_vtable=0x12f1a78,scope_start=0xece7ff,scope_end=b,frame_depth=184,this_definition=0xece7f9,actor_definition=0xece7fc,actor_profile='entry-edi',owner_register='esi')
def repin(d):
 for key,value in [('phaseSha256',d['source'].encode()),('callsSha256',serial(d['calls'])),('todoSha256',serial(d['todo'])),('beforeSha256',serial(d['before'])),('afterSha256',serial(d['after']))]:d['row'][key]=sha(value)
def test_opt_out_is_identity_without_evidence():assert MODULE.recover('s',['t'],['c'],{},None,{},None,None,None,None,None)==('s',['t'],['c'],[])
def test_six_scopes_one_inline_release_and_retained_query_order():
 d=inputs();source,todo,calls,reviews=run(d);assert len(reviews)==6 and sum(len(r['deadValues']) for r in reviews)==sum(len(r['composedDeadValues']) for r in reviews)==18 and calls.count('RESOURCE_ResourceThingHealthAbove')==6 and len(calls)==len(d['calls'])-6 and todo==d['todo'];assert source.count('resources:ResourceThingHealthAbove(')==6 and 'quest:GetHealth(' not in source
 staged=[r for r in reviews if 'retainedStaging' in r];assert len(staged)==1 and staged[0]['site']=='0xecf089';plan=staged[0]['retainedStaging'];assert plan['booleanDecisionAlias']=='fret_02' and plan['queryBeforeLocalClears'] is True
 query=source.index('fret_02 = resources:ResourceThingHealthAbove(');clear=source.index(plan['text'],query);branch=source.index('if fret_02 then',clear);assert query<clear<branch;assert staged[0]['native']['cleanupKind']=='inline-Thing-release' and len(staged[0]['native']['nodes'])==27;LuaRuntime().compile(source)
def test_full_frame_saved_table_external_import_and_open_control_deleter():
 d=inputs();_,report=frame(d);assert report['reachableInstructions']==1227 and report['callCount']==233 and report['knownStaticProfileCalls']==232 and len(report['correctedExportDepths'])==52 and set(report['requiredIndirectABI'])=={'0xecf0c8'}
 saved=[r['savedParentTable'] for r in report['callReviews'] if 'savedParentTable' in r];assert len(saved)==1 and saved[0]['callerEntryCoordinate']==-96 and saved[0]['receiverValueEquivalenceProved'] is False
 external=report['externalNormalABIConditions']['0xbfe9bc'];assert external['implementationVerified'] is False and external['closedAllPathReturnProof'] is False and external['normalABIRequired']['stackPurge']==0
@pytest.mark.parametrize('key',['nativeSha256','fnSha256','phaseSha256','callsSha256','todoSha256','beforeSha256','afterSha256'])
def test_stale_top_level_certificate_rejects(key):
 d=inputs();d['row'][key]='0'*64
 with pytest.raises(ValueError):run(d)
@pytest.mark.parametrize('fault',['missing','purge','ABI','closed','implementation','thunk','IAT','descriptor','lookup','module-address','symbol-address','module','symbol'])
def test_external_import_or_scope_forgery_rejects(fault):
 d=inputs();p=d['row']['frameProfile'];e=p['externalNormalABIs']['0xbfe9bc']
 if fault=='missing':p['externalNormalABIs']={}
 elif fault=='purge':e['purgeBytes']=4
 elif fault=='ABI':e['normalABIRequired']['callingConvention']='stdcall'
 elif fault=='closed':e['closedAllPathReturnProof']=True
 elif fault=='implementation':e['implementationVerified']=True
 elif fault=='thunk':e['thunkBytes']='90'*6
 elif fault=='IAT':e['iatSlot']+=4
 elif fault=='descriptor':e['descriptorBytes']='00'*20
 elif fault=='lookup':e['lookupBytes']='00'*4
 elif fault=='module-address':e['moduleNameAddress']=hex(int(e['moduleNameAddress'],16)+1)
 elif fault=='symbol-address':e['symbolNameAddress']=hex(int(e['symbolNameAddress'],16)+1)
 elif fault=='module':e['module']='wrong.dll'
 else:e['symbol']='free'
 with pytest.raises((ValueError,AssertionError)):run(d)
@pytest.mark.parametrize('fault',['missing','extra','native-pin','store','read','table-load','dispatch','register','parent','member','vtable','slot','helper-bytes','helper-address','caller-frame','heap-claim','receiver-equality','call-purge'])
def test_saved_parent_table_forgery_rejects(fault):
 d=inputs();p=d['row']['frameProfile'];s=p['savedParentTables']['0xecf12a']
 if fault=='missing':p['savedParentTables']={}
 elif fault=='extra':p['savedParentTables']['0xecf12b']=dict(s)
 elif fault=='native-pin':s['nativeSha256']='0'*64
 elif fault in ['store','read','table-load','dispatch']:
  key={'store':'cacheStore','read':'cacheRead','table-load':'tableLoad','dispatch':'dispatch'}[fault];s[key]=hex(int(s[key],16)+1)
 elif fault=='register':s['tableRegister']='edx'
 elif fault=='parent':s['parentOffset']=24
 elif fault=='member':s['memberOffset']=72
 elif fault=='vtable':s['vtable']='0x1270950'
 elif fault=='slot':s['slot']='0x38'
 elif fault=='helper-bytes':s['readOnlyBuffer']['bytes']='90'*15
 elif fault=='helper-address':s['readOnlyBuffer']['address']='0x99e4c4'
 elif fault=='caller-frame':s['abiContract']['callerFramePreserved']=False
 elif fault=='heap-claim':s['abiContract']['heapVerified']=True
 elif fault=='receiver-equality':next(r for r in p['callContracts'] if r['family']=='parent_saved_table_profile')['receiverValueEquivalenceProved']=True
 else:next(r for r in p['callContracts'] if r['site']=='0xecf119')['purgeBytes']=4
 with pytest.raises((ValueError,AssertionError)):run(d)
@pytest.mark.parametrize('fault',['missing-inline','callback','delete','base','kind','no-staging-option','break-count','extra-indirect','callback-purge','host-return','registered-parent','member-name'])
def test_inline_or_registration_profile_forgery_rejects(fault):
 d=inputs();r=d['row'];inline=r['inlineCleanupProfiles']['0xecf089'];p=r['frameProfile']
 if fault=='missing-inline':r['inlineCleanupProfiles']={}
 elif fault in ['callback','delete','base']:inline[fault]=hex(int(inline[fault],16)+1)
 elif fault=='kind':inline['kind']='direct'
 elif fault=='no-staging-option':r['allowZeroStaging']=False
 elif fault=='break-count':r['analysisInlineBreaks']=0
 elif fault=='extra-indirect':p['requiredIndirectABI']['0xecf12a']=dict(p['requiredIndirectABI']['0xecf0c8'])
 elif fault=='callback-purge':p['requiredIndirectABI']['0xecf0c8']['requiredNormalPurge']=4
 elif fault=='host-return':r['hostContract']['returnType']='int'
 elif fault=='registered-parent':d['unit']['vtable']='0x12f17e0'
 else:r['memberName']='seh_Mother'
 with pytest.raises((ValueError,AssertionError)):run(d)
@pytest.mark.parametrize('alias',['pCVar11','fret_02','fVar3'])
def test_repinned_observable_inline_scope_read_reaches_liveness_rejection(alias):
 d=inputs();match=re.search(r'(?m)^(?P<i> *)if fVar3 < fret_02 then\n',d['source']);assert match is not None;at=match.end();d['source']=d['source'][:at]+match['i']+'    quest:Observe('+alias+')\n'+d['source'][at:];repin(d)
 with pytest.raises(ValueError,match='live staging value'):run(d)
def test_disabled_nested_provenance_preserves_accepted_default_states():
 d=inputs();a,b=int(d['fn']['address'],16),int(d['fn']['bodyEndExclusive'],16);raw=d['image'].bytes_at(a,b-a);golden=read(F/'default_provenance.json');assert sha(raw)==golden['nativeSha256'];result=PROVENANCE.analyze(raw,a,member_offsets=(8,));canonical=json.dumps({hex(k):v for k,v in result['states'].items()},sort_keys=True,separators=(',',':')).encode();assert sha(canonical)==golden['statesSha256'] and result['visits']==golden['visits']
