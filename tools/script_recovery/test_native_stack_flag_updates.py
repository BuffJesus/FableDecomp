import copy
import hashlib
import itertools
import json
import random
import re
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime, LuaError

ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())

@pytest.fixture(scope='module')
def captured(tmp_path_factory):
    from tools.script_recovery import native_reviewed_stack_flag_updates as adapter
    from tools.script_recovery.convert_quest_unit import UnitConverter
    from tools.script_recovery.build_readable_unit import readable_file
    converter=UnitConverter(ROOT/'refs/script_recovery/chicken_kicking/translation_unit_typed.json')
    unit=json.loads((ROOT/'refs/script_recovery/chicken_kicking/units/V_ChickenKicking.json').read_text())
    result={};original=adapter.restore
    class Done(Exception):pass
    def capture(source,todo,fn,unit,rd):
        generated,remaining=original(source,todo,fn,unit,rd)
        if fn['address'].lower()=='0x00e64fb0':
            result.update(source=source,todo=todo,fn=fn,unit=unit,rd=rd,draft=generated,
                          readable=readable_file(generated)[0],remaining=remaining)
            raise Done()
        return generated,remaining
    with patch.object(converter,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):
        with pytest.raises(Done):converter.convert(unit,tmp_path_factory.mktemp('stack_flags'))
    return result

@pytest.mark.parametrize('fault',['native','source','lua','frame','slot','seed','initial','register','receiver',
 'argument','message','mask','window','cleanup','missing_destructor','destructor','snapshot','updated','word',
 'aliases','initializer','missing_alias_use','diagnostics'])
def test_ambiguous_or_stale_flag_rejected(captured,fault):
    from tools.script_recovery.native_reviewed_stack_flag_updates import restore
    c=captured;unit=copy.deepcopy(c['unit']);fn=copy.deepcopy(c['fn']);source=c['source'];todo=list(c['todo'])
    row=unit['reviewedStackFlagUpdates']['0x00e64fb0']
    if fault=='native':row['nativeSha256']='0'*64
    elif fault=='source':fn['decompile']+='\n'
    elif fault=='lua':source+='\n'
    elif fault=='frame':row['frameBytes']+=4
    elif fault=='slot':row['entrySlot']+=4
    elif fault=='seed':row['zeroRegister']=row['initialStore']
    elif fault=='initial':row['initialStore']=row['zeroRegister']
    elif fault=='register':row['valueRegister']='edi'
    elif fault=='receiver':row['receiverRegister']='esi'
    elif fault=='argument':row['nameStringOffset']+=4
    elif fault=='message':row['messageSlot']+=4
    elif fault=='mask':row['mask']=2
    elif fault=='window':row['window']['bytes']='90'
    elif fault=='cleanup':row['cleanupSha256']='0'*64
    elif fault=='missing_destructor':row['destructors'].pop()
    elif fault=='destructor':row['destructors'][0]=row['initialStore']
    elif fault=='snapshot':row['snapshotLocal']='other'
    elif fault=='updated':row['updatedLocal']='other'
    elif fault=='word':row['wordLocal']='other'
    elif fault=='aliases':row['updatedLocal']=row['snapshotLocal']
    elif fault=='initializer':
        source=source.replace('C_stk_370 = 0\n','C_stk_370 = 1\n');row['luaSha256']=hashlib.sha256(source.encode()).hexdigest()
    elif fault=='missing_alias_use':row['aliasUses'].remove('CVar17 = ctr_CVar19 | 3')
    elif fault=='diagnostics':todo=[]
    with pytest.raises(ValueError):restore(source,todo,fn,unit,c['rd'])

def test_unreviewed_function_is_unchanged(captured):
    from tools.script_recovery.native_reviewed_stack_flag_updates import restore
    c=captured;unit=copy.deepcopy(c['unit']);unit.pop('reviewedStackFlagUpdates')
    assert restore(c['source'],c['todo'],c['fn'],unit,c['rd'])==(c['source'],c['todo'])

def test_only_missing_scalar_snapshots_restored(captured):
    c=captured
    before=c['source'];after=c['draft']
    lines=before.splitlines();at=next(i for i,s in enumerate(lines) if '-- TODO(native): xStack_374 = xStack_374 | 1;' in s)
    indent=lines[at][:-len(lines[at].lstrip())]
    lines[at:at+1]=[indent+s for s in ['ctr_CVar19 = C_stk_370 % 4294967296','CVar17 = ctr_CVar19 | 1','C_stk_370 = CVar17']]
    assert '\n'.join(lines)==after.rstrip('\n')
    assert len(c['todo'])-len(c['remaining'])==1
    assert all(t.startswith('label ') for t in c['remaining'])

def fragment(text,old=False):
    lines=[s.strip() for s in text.splitlines()]
    if old:
        start=next(i for i,s in enumerate(lines) if s=='-- TODO(native): xStack_374 = xStack_374 | 1;')
        word='C_stk_370'
    else:
        start=next(i for i,s in enumerate(lines) if re.fullmatch(r'(?:local )?\w+ = \w+ % 4294967296',s))
        word=re.search(r'= (\w+) %',lines[start])[1]
    end=next(i for i in range(start+1,len(lines)) if re.fullmatch(r'if \w+ ~= 0 then',lines[i]))
    reaction=re.search(r'if (\w+) ~=',lines[end])[1]
    block='\n'.join(lines[start:end])
    names=sorted({m[1] for s in lines[start:end] if (m:=re.match(r'(?:local )?(\w+) = ',s))}|{word,'ctr_CVar19','CVar17'})
    return block,word,reaction,names

def executable(text,old=False):
    block,word,reaction,names=fragment(text,old)
    lua=LuaRuntime()
    run=lua.eval('''function(initial,hit,special,heal)
      local '''+','.join(names)+'''
      '''+word+'''=initial
      local HERO_ABILITY_HEAL_LIFE_SPELL=14
      local trace,flags={},{}
      local function observe(name)
       trace[#trace+1]=name;flags[#flags+1]=tostring('''+word+''')
      end
      local me={
       MsgIsHitByHero=function() observe("hit");return hit end,
       MsgIsHitByAnySpecialAbilityFromHero=function() observe("special");return special end,
       MsgIsHitByHeroSpecialAbility=function(self,ability) assert(ability==14);observe("heal");return heal end
      }
      '''+block+'''
      return '''+word+''','''+reaction+''',table.concat(trace,","),table.concat(flags,",")
    end''')
    return run

@pytest.mark.parametrize('hit',[False,True])
def test_original_missing_alias_fails_first_bitwise_use(captured,hit):
    run=executable(captured['source'],old=True)
    with pytest.raises(LuaError,match='nil'):run(0,hit,False,False)

@pytest.mark.parametrize('style',['draft','readable'])
def test_emitted_flag_word_and_message_order(captured,style):
    run=executable(captured[style])
    rng=random.Random(51)
    masks=[0,8,0x2000,0x80000000,0xfffffff8]+[rng.getrandbits(32)&~7 for _ in range(27)]
    for mask in masks:
        for initial in [mask,(mask+2147483648)%4294967296-2147483648]:
            for hit,special,heal in itertools.product([False,True],repeat=3):
                trace=['hit']+([] if hit else ['special']+(['heal'] if special else []))
                flags=[mask|1]+([] if hit else [mask|3]+([mask|7] if special else []))
                reaction=int(hit or (special and not heal))
                assert run(initial,hit,special,heal)==(mask,reaction,','.join(trace),','.join(map(str,flags)))

