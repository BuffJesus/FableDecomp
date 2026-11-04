from pathlib import Path
import importlib.util
import pytest
from tools.script_recovery import lua_staging_liveness as candidate

def prove(body):
    source='function Main(quest, me)\nlocal x, y, cond\n'+body+'\nend\n'
    return candidate.prove_unused_staging(source,source.index('x = nil'),'x')

@pytest.mark.parametrize('body',[
    'x = nil\nrepeat\nx = 1\nbreak\nuntil cond\nquest:Observe(x)',
    'x = nil\nwhile cond do\nbreak\nquest:Observe(x)\nend\nx = 1',
    'x = nil\nrepeat\nif cond then\nx = 1\nbreak\nend\nx = 2\nuntil cond\nquest:Observe(x)',
    'x = nil\nrepeat\nwhile cond do\nbreak\nend\nx = 1\nbreak\nuntil cond\nquest:Observe(x)',
    'x = nil\nrepeat\nbreak\nuntil x\nx = 1',
])
def test_dead_paths_prove(body):
    assert prove(body)['alias']=='x'

@pytest.mark.parametrize('body',[
    'x = nil\nbreak',
    'x = nil\nrepeat\nbreak\nx = 1\nuntil cond\nquest:Observe(x)',
    'x = nil\nwhile cond do\nx = 1\nbreak\nend\nquest:Observe(x)',
    'x = nil\nrepeat\nif cond then\nbreak\nend\nx = 1\nuntil cond\nquest:Observe(x)',
    'x = nil\nrepeat\nwhile cond do\nbreak\nend\nquest:Observe(x)\nbreak\nuntil cond',
    'x = nil\nrepeat\nrepeat\nbreak\nuntil cond\nquest:Observe(x)\nbreak\nuntil cond',
    'x = nil\nwhile cond do\nrepeat\nbreak\nuntil cond\nquest:Observe(x)\nbreak\nend',
    'x = nil\nrepeat\nquest:Observe(x)\nbreak\nuntil cond',
])
def test_live_paths_reject(body):
    with pytest.raises(ValueError):prove(body)
