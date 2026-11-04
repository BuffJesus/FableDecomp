from pathlib import Path
import copy,importlib.util,json,sys
import pytest
F=Path(__file__).resolve().parent/'fixtures/witch_copy_health'
from tools.script_recovery import native_health_copy_correspondence as MODULE
read=lambda p:json.loads(p.read_text())

def inputs():
 p=read(F/'phase.json');return dict(fn=p['fn'],before=p['before'],after=p['after'],phase=(F/'phase.lua').read_text(),getter_slot=0x30,inline_member_name='seh_Witch',inline_cleanup_profiles=read(F/'correspondence.json')['inlineCleanupProfiles'],allow_zero_staging=True)
def test_six_actual_native_AST_Lua_uses_and_retained_zero_staging():
 result=MODULE.prove(**inputs());assert result['nativeSites']==result['emittedUses']==6;rows=result['scopes'];assert sum(bool(s['retainedStaging']) for s in rows)==1 and 'x_stk_34 = nil' in rows[3]['retainedStaging'] and int(rows[3]['native']['call']['site'],16)==0xecf089
@pytest.mark.parametrize('fault',['missing-profile','unused-profile','wrong-kind','callback-site','delete-site','base-site','callback-slot','delete-target','base-target','no-staging-option','staging-value','staging-alias','staging-call','result','actor','condition','before','after'])
def test_forged_inline_source_correspondence_rejects(fault):
 d=inputs();p=d['inline_cleanup_profiles']['0xecf089']
 if fault=='missing-profile':d['inline_cleanup_profiles']={}
 elif fault=='unused-profile':d['inline_cleanup_profiles']['0xecf08a']=copy.deepcopy(p)
 elif fault=='wrong-kind':p['kind']='direct'
 elif fault=='callback-site':p['callback']='0xecf0cc'
 elif fault=='delete-site':p['delete']='0xecf0d7'
 elif fault=='base-site':p['base']='0xecf0f4'
 elif fault in ['callback-slot','delete-target','base-target']:
  site={'callback-slot':0xecf0c8,'delete-target':0xecf0d3,'base-target':0xecf0f0}[fault];call=next(c for c in d['fn']['calls']+d['fn']['indirectCalls'] if int(c['site'],16)==site)
  if fault=='callback-slot':call['slot']='0x8'
  else:call['target']='0x00bfe9c0'
 elif fault=='no-staging-option':d['allow_zero_staging']=False
 elif fault=='staging-value':d['phase']=d['phase'].replace('x_stk_34 = 0','x_stk_34 = 1',1)
 elif fault=='staging-alias':d['phase']=d['phase'].replace('x_stk_34 = nil','fret_02 = nil',1).replace('x_stk_34 = 0','fret_02 = 0',1)
 elif fault=='staging-call':d['phase']=d['phase'].replace('x_stk_34 = 0','quest:Observe(x_stk_34)',1)
 elif fault=='result':d['phase']=d['phase'].replace('fret_02 = quest:GetHealth','unknown = quest:GetHealth',1)
 elif fault=='actor':d['phase']=d['phase'].replace('fret_02 = quest:GetHealth(pCVar11)','fret_02 = quest:GetHealth(wrong)',1)
 elif fault=='condition':d['phase']=d['phase'].replace('if fVar3 < fret_02 then','if fVar3 > fret_02 then',1)
 elif fault=='before':d['before']=[s for s in d['before'] if 'fret_02 = GSI->GetHealth' not in s]
 else:d['after'].append('fret_02 = GSI->GetHealth(pCVar11);')
 with pytest.raises(ValueError):MODULE.prove(**d)
