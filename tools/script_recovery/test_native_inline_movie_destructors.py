import copy
import json
import re
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime,LuaError
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())

@pytest.fixture(scope='module')
def captured(tmp_path_factory):
    from tools.script_recovery import native_reviewed_inline_movie_destructors as adapter
    from tools.script_recovery.convert_quest_unit import UnitConverter
    from tools.script_recovery.build_readable_unit import readable_file
    converter=UnitConverter(ROOT/'refs/script_recovery/chicken_kicking/translation_unit_typed.json')
    unit=json.loads((ROOT/'refs/script_recovery/chicken_kicking/units/V_ChickenKicking.json').read_text());result={};original=adapter.restore
    class Done(Exception):pass
    def capture(source,todo,fn,unit,rd):
        generated,remaining=original(source,todo,fn,unit,rd)
        if fn['address'].lower()=='0x00e64fb0':
            result.update(source=source,todo=todo,fn=fn,unit=unit,rd=rd,draft=generated,readable=readable_file(generated)[0],remaining=remaining)
            raise Done()
        return generated,remaining
    with patch.object(converter,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):
        with pytest.raises(Done):converter.convert(unit,tmp_path_factory.mktemp('inline_movies'))
    return result

@pytest.mark.parametrize('fault',['native','source','lua','frame','dependency_inventory','dependency_body','dependency_start','object_inventory','local','entry_slot','ctor_receiver','ctor_call','start_receiver','start_call','construction','creation_count','path_inventory','path_window','member_receiver','member_call','base_receiver','vtable','base_call','jump','alias','source_count'])
def test_stale_or_ambiguous_cleanup_rejected(captured,fault):
    from tools.script_recovery.native_reviewed_inline_movie_destructors import restore
    c=captured;unit=copy.deepcopy(c['unit']);fn=copy.deepcopy(c['fn']);source=c['source'];row=unit['reviewedInlineMovieDestructors']['0x00e64fb0'];obj=row['objects'][0];path=obj['paths'][0]
    if fault=='native':row['nativeSha256']='0'*64
    elif fault=='source':fn['decompile']+='\n'
    elif fault=='lua':source+='\n'
    elif fault=='frame':row['frameBytes']+=4
    elif fault=='dependency_inventory':row['dependencies'].pop('member')
    elif fault=='dependency_body':row['dependencies']['member']['sha256']='0'*64
    elif fault=='dependency_start':row['dependencies']['movie']['start']+=1
    elif fault=='object_inventory':row['objects'].pop()
    elif fault=='local':obj['local']='other'
    elif fault=='entry_slot':obj['entrySlot']-=4
    elif fault in ['ctor_receiver','ctor_call','start_receiver','start_call']:
        key=dict(ctor_receiver='constructorLea',ctor_call='constructorCall',start_receiver='startReceiver',start_call='startCall')[fault];obj[key]=path['baseCall']
    elif fault=='construction':obj['constructionWindow']['sha256']='0'*64
    elif fault=='creation_count':obj['creationCount']=1
    elif fault=='path_inventory':obj['paths'].pop()
    elif fault=='path_window':path['window']['sha256']='0'*64
    elif fault in ['member_receiver','member_call','base_receiver','vtable','base_call','jump']:
        key=dict(member_receiver='memberLea',member_call='memberCall',base_receiver='baseLea',vtable='vtableStore',base_call='baseCall',jump='jump')[fault];path[key]=obj['constructorCall']
    elif fault=='alias':row['alias']='missing'
    elif fault=='source_count':obj['sourceCount']=3
    with pytest.raises(ValueError):restore(source,c['todo'],fn,unit,c['rd'])

def test_unreviewed_body_unchanged(captured):
    from tools.script_recovery.native_reviewed_inline_movie_destructors import restore
    c=captured;unit=copy.deepcopy(c['unit']);unit.pop('reviewedInlineMovieDestructors')
    assert restore(c['source'],c['todo'],c['fn'],unit,c['rd'])==(c['source'],c['todo'])

def test_four_source_cleanup_sites_only(captured):
    c=captured;before=c['source'].splitlines();after=c['draft'].splitlines();assert len(before)==len(after)
    changes=[(a.strip(),b.strip()) for a,b in zip(before,after) if a!=b]
    assert changes==[(f'pCVar16 = {name}',f'resources:DestroyMovie({name})') for name in ['xStack_32c','xStack_32c','xStack_34c','xStack_34c']]
    assert c['remaining']==c['todo']


def meeting_cleanups(text,old=False):
    # Limit to the two meeting lifetimes. Other movies reuse these stack slots.
    a=text.index('if not __native_entity_state:GetStateBool("GhostChat") then',text.index('"OBJECT_QUEST_CARD_CHICKEN_KICKING"'))
    b=text.index('cVar7 = quest:GetStateBool("KnowGhostHasGone")',a) if old or 'cVar7 = quest:GetStateBool("KnowGhostHasGone")' in text[a:] else len(text)
    block=text[a:b]
    if old:return [(m[1],m[0]) for m in re.finditer(r'pCVar16 = (xStack_\w+)',block)]
    # Existing explicit abort destructors are included: all must be safe and retire ownership.
    return [(m[1],m[0]) for m in re.finditer(r'resources:DestroyMovie\((\w+)\)',block)]


def execute_cleanup(name,line,handle):
    lua=LuaRuntime(unpack_returned_tuples=True)
    return lua.eval('''function(handle)
      local live=true;local trace={};local '''+name+'''=handle;local pCVar16
      local resources={DestroyMovie=function(self,id) assert(live and id==handle);live=false;trace[#trace+1]='destroy' end,
        StartMovie=function() assert(not live,'movie still active');live=true;trace[#trace+1]='start' end}
      trace[#trace+1]='unpause'
      '''+line+'''
      resources:StartMovie('')
      return table.concat(trace,',')
    end''')(handle)


def test_original_normal_paths_block_next_movie(captured):
    sites=meeting_cleanups(captured['source'],old=True);assert len(sites)==4
    for name,line in sites:
        with pytest.raises(LuaError,match='movie still active'):execute_cleanup(name,line,1)

@pytest.mark.parametrize('style',['draft','readable'])
def test_emitted_cleanup_retires_movie_before_next_start(captured,style):
    sites=meeting_cleanups(captured[style]);assert len(sites)>=4
    for name,line in sites:
        for handle in [1,27,4294967295]:assert execute_cleanup(name,line,handle)=='unpause,destroy,start'
