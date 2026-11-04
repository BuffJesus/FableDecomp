import copy
import hashlib
import json
import re
from pathlib import Path
from unittest.mock import patch

import pytest
from lupa.lua54 import LuaRuntime

ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())


@pytest.fixture(scope='module')
def captured(tmp_path_factory):
    from tools.script_recovery import native_reviewed_wide_text_arguments as adapter
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
        with pytest.raises(Done):converter.convert(unit,tmp_path_factory.mktemp('wide_text'))
    return result


@pytest.mark.parametrize('fault',[
    'native','source','lua','dependency','missing_dependency','literal','missing_literal',
    'vtable','slot','window','missing_window','call','missing_call','score','counter',
    'global','key','frame','vector_slot','receiver','missing_receiver','alias','local',
    'format','normal','cleanup','diagnostics','nonadjacent'])
def test_stale_or_ambiguous_evidence_rejected(captured,fault):
    from tools.script_recovery.native_reviewed_wide_text_arguments import restore
    c=captured;unit=copy.deepcopy(c['unit']);fn=copy.deepcopy(c['fn']);source=c['source'];todo=list(c['todo'])
    row=unit['reviewedWideTextArguments']['0x00e64fb0']
    if fault=='native':row['nativeSha256']='0'*64
    elif fault=='source':fn['decompile']+='\n'
    elif fault=='lua':source+='\n'
    elif fault=='dependency':row['dependencies'][0]['sha256']='0'*64
    elif fault=='missing_dependency':row['dependencies'].pop()
    elif fault=='literal':row['literals'][0]['bytes']='0000'
    elif fault=='missing_literal':row['literals'].pop()
    elif fault=='vtable':row['vtable']+=4
    elif fault=='slot':row['slot']+=4
    elif fault=='window':row['windows']['format']['bytes']='90'
    elif fault=='missing_window':row['windows'].pop('abortClose')
    elif fault=='call':row['calls'][next(iter(row['calls']))]+=4
    elif fault=='missing_call':row['calls'].pop(next(iter(row['calls'])))
    elif fault=='score':row['scorePush']+=1
    elif fault=='counter':unit['reviewedMasterIntStores']['0x00e64fb0']['valueRegister']='edi'
    elif fault=='global':row['globalAddress']+=4
    elif fault=='key':row['key']='unknown'
    elif fault=='frame':row['frameBytes']+=4
    elif fault=='vector_slot':row['vectorSlot']+=4
    elif fault=='receiver':row['vectorReceivers'][0]+=4
    elif fault=='missing_receiver':row['vectorReceivers'].pop()
    elif fault=='alias':row['alias']='resources'
    elif fault=='local':row['valueLocal']='unknown'
    elif fault=='format':row['formatLines'][0]='-- absent'
    elif fault=='normal':row['normalLines'][0]='-- absent'
    elif fault=='cleanup':row['cleanupLine']='-- absent'
    elif fault=='diagnostics':todo=[]
    elif fault=='nonadjacent':
        source=source.replace(row['formatLines'][0],row['formatLines'][0]+'\n    quest:Unexpected()')
        row['luaSha256']=hashlib.sha256(source.encode()).hexdigest()
    with pytest.raises(ValueError):restore(source,todo,fn,unit,c['rd'])


def test_unreviewed_function_is_unchanged(captured):
    from tools.script_recovery.native_reviewed_wide_text_arguments import restore
    c=captured;unit=copy.deepcopy(c['unit']);unit.pop('reviewedWideTextArguments')
    assert restore(c['source'],c['todo'],c['fn'],unit,c['rd'])==(c['source'],c['todo'])


def test_change_scope_and_unresolved_online_score(captured):
    c=captured;row=c['unit']['reviewedWideTextArguments']['0x00e64fb0']
    assert len(c['todo'])-len(c['remaining'])==7
    assert 'UpdateOnlineScore_ChickenKick: not in FSE manifest' in c['remaining']
    assert any('ctr_CVar19 ==' in t for t in c['remaining'])
    assert 'GetFormattedString: not in FSE manifest' not in c['remaining']
    before=[s.strip() for s in c['source'].splitlines() if s.strip() not in row['formatLines']+[row['cleanupLine']]]
    after=[s.strip() for s in c['draft'].splitlines() if row['alias'] not in s]
    assert before==after
    assert c['draft'].count(':Close()')==2
    assert c['draft'].count(':ClearGlobalText(')==1


@pytest.mark.parametrize('style',['draft','readable'])
def test_emitted_signed_score_lifetime_and_yield(captured,style):
    lines=[s.strip() for s in captured[style].splitlines()]
    create=next(s for s in lines if '= resources:NewWideTextArguments()' in s)
    name=create.split(' = ')[0].removeprefix('local ')
    append=next(s for s in lines if ':AppendInteger(' in s)
    value=re.search(r'AppendInteger\(+([A-Za-z_]\w*) \+',append)[1]
    format_line=next(s for s in lines if ':FormatGlobalText(' in s)
    clear=next(s for s in lines if ':ClearGlobalText(' in s)
    closes=[s for s in lines if s==name+':Close()']
    assert len(closes)==2
    at=lines.index(clear)
    assert '.helper_E68B20(' in lines[at-1]
    assert lines[at+1]=='quest:PauseAllNonScriptedEntities(false)'
    assert ':DestroyMovie(' in lines[at+2] and lines[at+3]==closes[0]
    normal='\n'.join(lines[at-1:at+4])
    helper_value=re.search(r'\(quest, me, (\w+)\)',lines[at-1])[1]
    movie_value=re.search(r'DestroyMovie\((\w+)\)',lines[at+2])[1]
    lua=LuaRuntime()
    run=lua.eval('''function(score,abort)
      local '''+value+'''=score
      local '''+name+'''
      local '''+helper_value+'''="macro"
      local '''+movie_value+'''=7
      local events={};local function emit(s) events[#events+1]=s end
      local global="old";local closed=false;local integer
      local resources={}
      function resources:NewWideTextArguments()
        emit("new")
        return {
          AppendInteger=function(self,n) assert(math.type(n)=="integer");integer=n;emit("append") end,
          FormatGlobalText=function(self,address,key)
            assert(address==0x143e908 and key=="TEXT_QST_B17_SCORE")
            global="score:"..integer;emit("format")
          end,
          ClearGlobalText=function(self,address) assert(address==0x143e908);global="";emit("clear") end,
          Close=function() assert(not closed);closed=true;emit("close") end
        }
      end
      function resources:DestroyMovie(id) assert(id==7);emit("movie") end
      local quest={PauseAllNonScriptedEntities=function(self,pause) assert(pause==false);emit("unpause") end}
      local me={}
      local helpers={helper_E68B20=function(q,m,key)
        assert(q==quest and m==me and key=="macro");emit("helper");coroutine.yield(global);emit("returned")
      end}
      local function require(module) assert(module=="V_ChickenKicking.native_quest_helpers");return helpers end
      local co=coroutine.create(function()
        '''+'\n'.join([create,append,format_line])+'''
        if abort then '''+closes[1]+''' else
          '''+normal+'''
        end
      end)
      local ok,shown=coroutine.resume(co);assert(ok,shown)
      if not abort then assert(not closed and shown==global);assert(coroutine.resume(co)) end
      assert(coroutine.status(co)=="dead" and closed)
      return integer,global,table.concat(events,",")
    end''')
    for score in [0,1,500,-1,-2147483648,2147483647,2147483648,4294967295,16777217]:
        expected=(score+2147483648)%4294967296-2147483648
        for abort in [False,True]:
            assert run(score,abort)==(expected,'score:'+str(expected) if abort else '',
                'new,append,format,close' if abort else 'new,append,format,helper,returned,clear,unpause,movie,close')
