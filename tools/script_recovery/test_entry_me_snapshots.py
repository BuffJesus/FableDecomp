import sys
from pathlib import Path
import pytest
W=Path(__file__).resolve().parent;R=W.parents[1];sys.path.insert(0,str(R));sys.path.insert(0,str(W))
from native_entry_me_snapshots import restore
from tools.script_recovery.native_evidence_lowering import ME_RECEIVER,strip_receiver_arguments
BASE='''void f(void *this)
{
  CScriptThing *p;
  CScriptThing *saved;
  p = (CScriptThing *)(this + 8);
  saved = p;
  GSI->NewScriptFrame();
  p = GSI->GetHero();
  CScriptThing::SetFriendsWithEverythingFlag((CScriptThing *)(this + 8), saved,true);
}
'''
def test_immutable_snapshot_survives_register_reuse():
 out=strip_receiver_arguments(restore(BASE,ME_RECEIVER))
 assert 'saved = p;' in out
 assert 'SetFriendsWithEverythingFlag((CScriptThing *)(this + 8), true);' in out
 assert 'p = GSI->GetHero();' in out

@pytest.mark.parametrize('before,after',[
 ('  saved = p;','  if (test) {\n  saved = p;\n  }'),
 ('  saved = p;','  GSI->Call();\n  saved = p;'),
 ('  saved = p;','LAB_1:\n  saved = p;'),
 ('  saved = p;','  p = GSI->GetHero();\n  saved = p;'),
 ('  saved = p;','  p = other;\n  saved = p;'),
 ('  CScriptThing *saved;','  CScriptThing saved;'),
 ('  saved = p;','  saved = p;\n  saved = other;'),
 ('  GSI->NewScriptFrame();','  GSI->Write(&saved);'),
 ('  GSI->NewScriptFrame();','  saved += 4;'),
 ('  GSI->NewScriptFrame();','  saved++;'),
 ('  GSI->NewScriptFrame();','  ++saved;'),
 ('  saved = p;','  saved = (CScriptThing *)other;'),
])
def test_ambiguous_snapshot_unchanged(before,after):
 source=BASE.replace(before,after);assert restore(source,ME_RECEIVER)==source

def test_line_endings_preserved():
 source=BASE.replace('\n','\r\n');out=restore(source,ME_RECEIVER)
 assert out.count('\r\n')==source.count('\r\n')
 assert restore(out,ME_RECEIVER)==out
