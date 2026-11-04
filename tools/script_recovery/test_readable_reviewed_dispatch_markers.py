import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_dispatch_scaffolding import prune_unused_dispatch_seeds

def pair(offset='4'):
 return ('    -- reviewed native interface-pointer staging: CVar12 = *(this + '+offset+')\n'
         '    scratch = nil --[[reviewed native interface-pointer staging]]\n')

@pytest.mark.parametrize('entity,offset',[(True,'4'),(True,'0x4'),(False,'64'),(False,'0x40')])
def test_matching_reviewed_pointer_retains_old_private_seed_projection(entity,offset):
 source='function Main()\n    local scratch\n'+pair(offset)+'    return 17\nend\n'
 legacy=source.replace('reviewed native interface-pointer staging:','TODO(native):').replace(
    '--[[reviewed native interface-pointer staging]]','--[[unresolved native value]]')
 out,proof=prune_unused_dispatch_seeds(source,entity=entity)
 assert out==prune_unused_dispatch_seeds(legacy,entity=entity)[0]
 assert len(proof)==1 and proof[0]['nativeStatement'].startswith('-- reviewed native interface-pointer staging:')
 for text in (source,out):assert LuaRuntime().execute(text+'return Main()')==17

@pytest.mark.parametrize('extra',[
 '    return scratch\n','    Consume(scratch)\n','    scratch = 9\n',
 '    local function callback() return scratch end\n','    -- consume(scratch)\n',
 '    local payload="scratch"\n','    local scratch\n','    local scratch=9\n',
])
def test_reviewed_marker_does_not_override_binding_or_use_guards(extra):
 source='function Main()\n    local scratch\n'+pair()+extra+'end\n'
 assert prune_unused_dispatch_seeds(source,entity=True)==(source,[])

@pytest.mark.parametrize('change',[
 lambda text:text.replace('--[[reviewed native interface-pointer staging]]','--[[unresolved native value]]'),
 lambda text:text.replace('-- reviewed native interface-pointer staging:','-- TODO(native):'),
 lambda text:text.replace('-- reviewed native interface-pointer staging:','-- reviewed native register-dispatch staging:'),
])
def test_mismatched_review_status_or_other_family_keeps_the_seed(change):
 source='function Main()\n    local scratch\n'+change(pair())+'end\n'
 assert prune_unused_dispatch_seeds(source,entity=True)==(source,[])

@pytest.mark.parametrize('wrapper',[lambda text:'local payload=[=[\n'+text+']=]\n',lambda text:'--[=[\n'+text+']=]\n'])
def test_reviewed_lookalike_inside_opaque_text_is_not_evidence(wrapper):
 source='function Main()\n    local scratch\n'+wrapper(pair())+'end\n'
 assert prune_unused_dispatch_seeds(source,entity=True)==(source,[])

@pytest.mark.parametrize('header,declaration',[('function Main(scratch)\n',''),('function Main()\n',''),
 ('function Main()\n','    local scratch, scratch\n')])
def test_reviewed_seed_does_not_claim_global_parameter_or_duplicate_storage(header,declaration):
 source=header+declaration+pair()+'end\n'
 assert prune_unused_dispatch_seeds(source,entity=True)==(source,[])

@pytest.mark.parametrize('entity,offset',[(False,'4'),(True,'0x40'),(None,'4')])
def test_reviewed_seed_requires_the_known_owner_offset(entity,offset):
 source='function Main()\n    local scratch\n'+pair(offset)+'end\n'
 assert prune_unused_dispatch_seeds(source,entity=entity)==(source,[])
