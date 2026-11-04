from pathlib import Path
import os,subprocess,sys
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import readable_style

SOURCES=[
 'function Main()\n    local source, alias, result\n    source = read()\n    alias = source\n    result = alias\n    return result\nend\n',
 'function Main()\n    local r3, r2, r1, r4\n    r3 = read()\n    r2 = r3\n    r1 = r2\n    r4 = r1\n    mutate()\n    return r4\nend\n',
 'function Main()\n    local source, alias, result\n    source = read()\n    local function change()\n        source = other\n    end\n    alias = source\n    result = alias\n    change()\n    return result\nend\n',
]
CHILD='''import sys
from pathlib import Path
from tools.script_recovery import readable_style as style
p=Path(sys.argv[1]);exec(compile(p.read_text(),str(p),'exec'),style.__dict__)
source=sys.stdin.read()
if sys.argv[2]=='copies': result=''.join(style.propagate_copies(source.splitlines(True))[0])
else:
 from tools.script_recovery.build_readable_unit import readable_file
 result=readable_file(source)[0]
print(result,end='')
'''

def execute(source,fault):
    lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().fault=fault
    return lua.execute('''local count=0;local expected={label='expected'};other={label='other'}
function read()count=count+1;if fault then error('injected',0)end;return expected end
function mutate()count=count+1 end
'''+source+'''
local ok,v=pcall(Main);return ok,ok and v.label or tostring(v),count
''')

@pytest.mark.parametrize('source',SOURCES)
@pytest.mark.parametrize('stage',['copies','pipeline'])
def test_alias_traversal_is_repeatable_across_hash_seeds_and_preserves_identity(source,stage):
    root=Path(__file__).resolve().parents[2]
    implementation=Path(readable_style.propagate_copies.__code__.co_filename).resolve()
    outputs=[]
    for seed in range(8):
        result=subprocess.run([sys.executable,'-c',CHILD,str(implementation),stage],input=source,text=True,capture_output=True,check=True,cwd=root,env={**os.environ,'PYTHONHASHSEED':str(seed)})
        outputs.append(result.stdout)
        for fault in (False,True):assert execute(result.stdout,fault)==execute(source,fault)
    assert len(set(outputs))==1
