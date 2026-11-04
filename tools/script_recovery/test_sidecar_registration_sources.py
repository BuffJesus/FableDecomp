from pathlib import Path
import pytest
from tools.script_recovery.sidecar_registration_sources import included_registrations
from tools.script_recovery.smoke_run_unit import registered_methods

ROOT=Path(__file__).resolve().parents[2]
NAMES=('LuaManager.cpp','NoviUnitBindings.h','LuaRetailResources.h')

def profile(tmp_path,root,headers):
 for name in NAMES:(tmp_path/name).write_text(root if name=='NoviUnitBindings.h' else '',encoding='utf-8')
 for name,text in headers.items():
  path=tmp_path/name;path.parent.mkdir(exist_ok=True,parents=True);path.write_text(text,encoding='utf-8')
 return [tmp_path/name for name in NAMES]

def test_direct_local_called_helper(tmp_path):
 sources=profile(tmp_path,'#include "Extra.h"\nRegisterExtra(quest);',{'Extra.h':'inline void RegisterExtra(sol::usertype<Q>& quest){quest["Included"] = &Q::Included;}'})
 found=included_registrations(sources)
 assert set(found)=={'Included'} and found['Included'][0]['function']=='RegisterExtra'
 assert registered_methods(tmp_path)=={'Included'}

def test_transitive_headers_and_registration_calls_with_cycles(tmp_path):
 sources=profile(tmp_path,'#include "A.h"\nRegisterA(quest);',{'A.h':'#include "nested/B.h"\ninline void RegisterA(auto& quest){RegisterB(quest);quest["A"]=&Q::A;}','nested/B.h':'#include "../A.h"\ninline void RegisterB(auto& quest){RegisterA(quest);quest["B"]=&Q::B;}'})
 assert set(included_registrations(sources))=={'A','B'}

@pytest.mark.parametrize('call',['// RegisterExtra(quest);','const char* x="RegisterExtra(quest);";','auto x=R"lua(RegisterExtra(quest);)lua";','/*RegisterExtra(quest);*/','void RegisterExtra(sol::usertype<Q>& quest);',''])
def test_uncalled_or_text_only_helpers_do_not_register(tmp_path,call):
 sources=profile(tmp_path,'#include "Extra.h"\n'+call,{'Extra.h':'inline void RegisterExtra(auto& quest){quest["NotCalled"]=&Q::A;}'})
 assert included_registrations(sources)=={}

def test_unused_helper_in_called_header_is_not_inferred(tmp_path):
 sources=profile(tmp_path,'#include "Extra.h"\nRegisterExtra(quest);',{'Extra.h':'inline void RegisterExtra(auto& quest){quest["Yes"]=&Q::A;}\ninline void RegisterUnused(auto& quest){quest["No"]=&Q::B;}'})
 assert set(included_registrations(sources))=={'Yes'}

def test_comments_strings_raw_strings_and_reads_do_not_become_assignments(tmp_path):
 header='''inline void RegisterExtra(auto& quest){
 // quest["Comment"] = fake;
 /* quest["Comment2"] = fake; */
 auto example=R"demo(quest["Raw"] = fake; } {)demo";
 auto ordinary="quest[\\"Ordinary\\"] = fake; }";
 auto read=quest["Read"];
 if(quest["Compare"]==true){}
 quest["Yes"] = [](int value){return value;};
 }'''
 sources=profile(tmp_path,'#include "Extra.h"\nRegisterExtra(quest);',{'Extra.h':header})
 assert set(included_registrations(sources))=={'Yes'}

@pytest.mark.parametrize('include',['// #include "Extra.h"','/*\n#include "Extra.h"\n*/','auto text=R"demo(\n#include "Extra.h"\n)demo";'])
def test_text_only_includes_are_not_followed(tmp_path,include):
 sources=profile(tmp_path,include+'\nRegisterExtra(quest);',{'Extra.h':'inline void RegisterExtra(auto& quest){quest["No"]=&Q::A;}'})
 assert included_registrations(sources)=={}

def test_external_include_and_unreferenced_adjacent_file_are_not_followed(tmp_path):
 root=tmp_path/'profile';root.mkdir();(tmp_path/'Outside.h').write_text('inline void RegisterOutside(auto& quest){quest["External"]=&Q::A;}',encoding='utf-8')
 sources=profile(root,'#include "../Outside.h"\nRegisterOutside(quest);RegisterExtra(quest);',{'Extra.h':'inline void RegisterExtra(auto& quest){quest["Adjacent"]=&Q::A;}'})
 assert included_registrations(sources)=={}

def test_ambiguous_duplicate_helper_declines(tmp_path):
 sources=profile(tmp_path,'#include "A.h"\n#include "B.h"\nRegisterExtra(quest);',{name:'inline void RegisterExtra(auto& quest){quest["'+name[0]+'"]=&Q::A;}' for name in ('A.h','B.h')})
 assert included_registrations(sources)=={}

def test_missing_include_retains_pending_policy(tmp_path):
 sources=profile(tmp_path,'#include "Missing.h"\nRegisterExtra(quest);',{})
 assert included_registrations(sources)=={}

def test_existing_seed_policy_and_incomplete_profile_unchanged(tmp_path):
 profile(tmp_path,'quest["Existing"] = &Q::A;',{})
 assert registered_methods(tmp_path)=={'Existing'}
 (tmp_path/'LuaManager.cpp').unlink()
 with pytest.raises(ValueError,match='Incomplete sidecar source profile'):registered_methods(tmp_path)

def test_actual_current_sdk_included_arena_registration():
 sdk=ROOT/'work/codex_lua_movie_destroy_reentry_20261016/sidecar_candidate/FableScriptExtender'
 assert 'InitialiseArenaRounds' in registered_methods(sdk)
 found=included_registrations([sdk/name for name in NAMES])
 assert found['InitialiseArenaRounds']==[dict(path=str((sdk/'NoviArenaRounds.h').resolve()),function='RegisterNoviArenaRounds')]
