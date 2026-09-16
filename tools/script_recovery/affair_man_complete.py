"""Isolated complete readable husband candidate; native integration remains pending."""
import hashlib,json,re
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.generate_affair_man_resource_candidate import generate as resource_candidate
from tools.script_recovery.readable_lua import readable_source,rename_labels
from tools.script_recovery.structure_affair_man_lua import structure_cleanup
from tools.script_recovery.readable_affair_man import readable_man_source
from tools.script_recovery.affair_man_complete_dispatcher_native import prove as prove_dispatcher
LABELS=dict(zip(('LAB_00db0b30','LAB_00db1593','LAB_00db15e6','LAB_00db1c6c','LAB_00db1c71','LAB_00db1d7c','LAB_00db1d8e'),
    ('checkInteractions','checkAffairWomanAfterConversation','finishConversationMovie','finishInteraction','waitForNextInteractionFrame','releaseAffairActors','releaseManControl')))

def baseline(**kwargs):
    source,report=resource_candidate(**kwargs);source,_=readable_source(source,inline_literals=True)
    source=rename_labels(source,LABELS);source,_=structure_cleanup(source);source,_=readable_man_source(source);return source,report

def prove(data=None):
    d=data or RData();w=json.loads(Path(__file__).with_name('affair_man_complete_witness.json').read_text())
    for row in w['functions']:
        if hashlib.sha256(d.bytes_at(row['address'],row['size'])).hexdigest()!=row['sha256']:raise ValueError('AffairMan native bytes changed')
    for address,text in w['literals'].items():
        if d.string_at(int(address,16))!=text:raise ValueError('AffairMan literal changed')
    for slot,pc in w['slots'].items():
        if int.from_bytes(d.bytes_at(0x1260f0c+int(slot,16),4),'little')!=pc:raise ValueError('AffairMan Init API changed')
    return w

def generate(**kwargs):
    w=prove();source,report=baseline(**kwargs)
    if hashlib.sha256(source.encode()).hexdigest()!=w['sourceSha256']:raise ValueError('AffairMan complete source correspondence changed')
    start=source.index('    quest:EntitySetAsDamageable(me, false)');end=source.index('\nend',start)
    source=source[:start]+'''    quest:WithRetailResources(function(resources)
        resources:InitializeAffairManActor(me)
    end)'''+source[end:]
    for key,b2 in [('ST_OPINION_FEAR_IDLE_COWERING',False),('GIVE_KISS',True),('GIVE_HUG',True)]:
        old=f'resources:PlayAnimation(man_resource, "{key}", false, {str(b2).lower()}, false, true, resources:ReadAnimationArgument5(), false, false)'
        if source.count(old)!=1:raise ValueError('AffairMan animation source changed')
        source=source.replace(old,f'resources:PlayAffairManAnimation(man_resource, "{key}", {str(b2).lower()})')
    comments=re.findall(r'-- TODO\(native\):[^\n]*',source)
    if comments!=w['historicalComments']:raise ValueError('AffairMan TODO classification changed')
    source=re.sub(r'(?m)^ *-- TODO\(native\):[^\n]*\n','',source)
    report['requiresExtension']=[name for name in report.get('requiresExtension',[]) if name not in ('PlayAnimation','ReadAnimationArgument5')]+['InitializeAffairManActor','PlayAffairManAnimation']
    source=re.sub(r'(?m)^-- MoveToPosition, [^\n]+\n','-- '+', '.join(report['requiresExtension'])+'.\n',source,count=1)
    report['completePass']={'evidence':w,'outerDispatcher':prove_dispatcher(),'pending':['Merged resource owner and raw generic-wrapper/null behavior audit.','Outer dispatcher compared with explicit phase boundaries; inner engine behavior and state/scheduler/save-restore validation remain pending.','Raw animation byte forwarded unchanged; dynamic expert interpretation is not inferred.']}
    report['openGaps']=list(report['completePass']['pending'])
    report['sha256']=hashlib.sha256(source.encode()).hexdigest()
    out=ROOT/'work/affair_man_complete';out.mkdir(parents=True,exist_ok=True)
    (out/'CANDIDATE.lua').write_text(source);(out/'REPORT.json').write_text(json.dumps(report,indent=2)+'\n');(out/'quests.lua').write_text('Quests = {} -- Disabled recovery.\n');return source,report

if __name__=='__main__':generate()
