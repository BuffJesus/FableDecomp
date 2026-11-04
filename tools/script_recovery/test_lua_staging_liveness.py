import re
from pathlib import Path
import pytest
from tools.script_recovery.lua_staging_liveness import prove_unused_staging
R=Path(__file__).resolve().parents[2]
FIXTURE=R/'tools/script_recovery/fixtures/staging_liveness/woman_main.lua'
def prove(body):
 source='function Main(quest, me)\nlocal x, y, z, cond\n'+body+'\nend\n'
 return prove_unused_staging(source,source.index('x = nil'),'x')
@pytest.mark.parametrize('body',[
 'x = nil\nx = 1\nquest:Observe(x)',
 'x = nil\ny = x\nx = 1\ny = 2\nquest:Observe(y)',
 'x = nil\nif cond then\nx = 1\nelse\nx = 2\nend\nquest:Observe(x)',
 'x = nil\nif cond then\nx = 1\nelseif cond then\nx = 2\nelse\nx = 3\nend\nquest:Observe(x)',
 'x = nil\nwhile cond do\ny = x\ny = 0\nend\nx = 2\nquest:Observe(x)',
 'x = nil\nrepeat\ny = x\nx = 2\ny = 3\nuntil cond\nquest:Observe(x)',
 'x = nil\nrepeat\nx = 1\nuntil x',
 'x = nil\ngoto DONE\nquest:Observe(x)\n::DONE::\nx = 2',
 'x = nil\nif cond then return end\nx = 2\nquest:Observe(x)',
 'x = nil\n::AGAIN::\ny = x\nx = 2\ny = 0\nif cond then goto AGAIN end\nquest:Observe(x)',
 'x = nil\nquest:Observe("x; goto LOST")\n-- quest:Observe(x)\nx = 2',
 'x = nil\ny = x\nz = y\nx = 1\ny = 1\nz = 1\nquest:Observe(z)',
 'x = nil\nif cond then\ny = x\nreturn\nelse\nx = 1\nend\nquest:Observe(x)',
])
def test_unused_and_locally_copied_values_prove(body):
 assert prove(body)['alias']=='x'
@pytest.mark.parametrize('body',[
 'x = nil\nquest:Observe(x)',
 'x = nil\nx = quest:Observe(x)',
 'x = nil\ny = x\nquest:Observe(y)',
 'x = nil\ny = x\nz = y\nquest:Observe(z)',
 'x = nil\nif cond then\nx = 1\nend\nquest:Observe(x)',
 'x = nil\nif cond then\nx = 1\nelseif cond then\nx = 2\nend\nquest:Observe(x)',
 'x = nil\nwhile cond do\nx = 1\nend\nquest:Observe(x)',
 'x = nil\nrepeat\nquest:Observe(x)\nx = 1\nuntil cond',
 'x = nil\nif cond then goto READ end\nx = 1\n::READ::\nquest:Observe(x)',
 'x = nil\nif x then return end',
 'x = nil\nreturn x',
 'x = nil\nGLOBAL = x',
 'x = nil\nquest.saved = x',
 'x = nil\ny = x or false',
 'x = nil\nlocal inner = x',
 'x = nil\nlocal x = 1',
 'x = nil\nlocal f = function() return x end',
 'x = nil\ngoto LOST',
 'x = nil\n::HERE::\n::HERE::',
 'x = nil\nfor n=1,2 do\nx = 1\nend',
 'x = nil\nunrelated = 0; goto READ\nx = 1\n::READ::\nquest:Observe(x)',
 'x = nil\nrepeat\nif cond then\nx = 1\nend\nuntil x',
])
def test_live_escaping_or_unsupported_values_reject(body):
 with pytest.raises(ValueError):prove(body)
@pytest.mark.parametrize('index',range(9))
def test_actual_nine_aliases_unobserved_even_with_local_copy_chains(index):
 source=FIXTURE.read_text();matches=list(re.finditer(r'(?m)^\s*([A-Za-z_]\w*) = nil --\[\[unresolved native value\]\]',source));assert len(matches)==9;m=matches[index]
 result=prove_unused_staging(source,m.start(1),m[1]);assert result['visitedNodes']>0
 end=source.index('\n',m.end());mutant=source[:end]+'\nquest:EvidenceObserve('+m[1]+')'+source[end:]
 with pytest.raises(ValueError):prove_unused_staging(mutant,m.start(1),m[1])

@pytest.mark.parametrize('fault',['duplicateParameter','invalidOffset','wrongAlias','nonNilMarker','multipleFunction','extraControl'])
def test_invalid_function_or_marker_rejects(fault):
 source='function Main(quest, me)\nlocal x, y\nx = nil\nx = 1\nend\n';offset=source.index('x = nil');alias='x'
 if fault=='duplicateParameter':source=source.replace('quest, me','quest, quest');offset=source.index('x = nil')
 elif fault=='invalidOffset':offset=len(source)+1
 elif fault=='wrongAlias':alias='globalValue'
 elif fault=='nonNilMarker':source=source.replace('x = nil','x = 0')
 elif fault=='multipleFunction':source+='function Other()\nend\n'
 elif fault=='extraControl':source=source.replace('x = 1','do\nx = 1\nend')
 with pytest.raises(ValueError):prove_unused_staging(source,offset,alias)
