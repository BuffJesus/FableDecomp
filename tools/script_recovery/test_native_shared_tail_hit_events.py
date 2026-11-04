import json,itertools
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
ROOT=Path(__file__).resolve().parents[2]
CASES={'bordello': ('V_Bordello/Entities/BordelloGuard.lua', '00e40afa_c1', 'c_stk_bd', 'FLOW_past_lab_00e40afa_c1'), 'guild_training': ('GuildTraining/Entities/PreMeleeMaze.lua', '00d44243_c1', 'bVar4', 'FLOW_past_lab_00d44243_c1'), 'sick_child': ('V_SickChild/Entities/SickChild.lua', '00ec6567_c2', 'c_stk_81', 'FLOW_past_lab_00ec6567_c2'), 'white_balverine': ('WhiteBalverineKnotholeGlade/Entities/KG_Chief.lua', '00e17f13_c1', 'c_stk_85', 'FLOW_past_lab_00e17f21_c1')}
UNITS={'bordello':'V_Bordello','guild_training':'Q_GuildTraining','sick_child':'V_SickChild','white_balverine':'Q_WhiteBalverineKnotholeGlade'}
@pytest.fixture(scope='module',params=list(CASES))
def copied_hit(request,tmp_path_factory):
 unit=request.param
 evidence=ROOT/'refs/script_recovery'/unit
 c=UnitConverter(evidence/'translation_unit_typed.json');out=tmp_path_factory.mktemp('copied-hit-'+unit)
 ir=json.loads((evidence/'units'/(UNITS[unit]+'.json')).read_text())
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(ir,out)
 filename,target,result,last_label=CASES[unit]
 source=(out/'FSE'/filename).read_text()
 label='::LAB_'+target+'::';end=source.index('::'+last_label+'::')+len('::'+last_label+'::')
 assert source.count(label)==1
 start=source.rfind(' = me:MsgIsHitByHero()',0,source.index(label));start=source.rfind('\n',0,start)+1
 fragment=source[start:end]
 prefix='local bVar3,bVar4,bVar5,bVar6,c_stk_bd,c_stk_81,c_stk_85=false,false,false,false,previous,previous,previous\n'
 return unit,'return function(me,quest,previous)\n'+prefix+fragment+'\nreturn '+result+' end'
@pytest.mark.parametrize('hit,magic,heal,previous,state',list(itertools.product((False,True),(False,True),(False,True),(0,1),(0,7))))
def test_actual_copied_damage_event_preserves_heal_exclusion_and_resets_flags(copied_hit,hit,magic,heal,previous,state):
 unit,source=copied_hit;lua=LuaRuntime();run=lua.execute(source);calls=[]
 actor=lua.table_from({'MsgIsHitByHero':lambda _:(calls.append('hit') or hit),
  'MsgIsHitByAnySpecialAbilityFromHero':lambda _:(calls.append('magic') or magic),
  'MsgIsHitByHeroSpecialAbility':lambda _,ability:(calls.append('heal:'+str(ability)) or heal)})
 q=lua.table_from({'GetStateInt':lambda _,name:state})
 value=run(actor,q,previous);flag=value if isinstance(value,bool) else value!=0
 expected=(hit or (magic and not heal)) and (unit!='white_balverine' or state!=7)
 expected_calls=['hit']+([] if hit else ['magic']+(['heal:14'] if magic else []))
 assert flag==expected and calls==expected_calls
