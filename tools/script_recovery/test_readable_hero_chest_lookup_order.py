from pathlib import Path
import pytest
import importlib.util
from tools.script_recovery.build_readable_unit import readable_file
W=Path(__file__).resolve().parent
if (W/'candidate_bootstrap.py').exists():
    spec=importlib.util.spec_from_file_location('candidate_chest',W/'test_readable_chest_exits.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);run=m.run
else:
    from tools.script_recovery.test_readable_chest_exits import run
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())
@pytest.mark.parametrize('scenario',['normal','failed_open','cancel','hit','special','both_hits','shutdown'])
def test_retail_chest_lookup_counts_and_exit_effects(scenario):
    draft=(ROOT/'refs/script_recovery/lifted/Expressions/draft/FSE/Global_OpenChest/Global_OpenChest.lua').read_text()
    readable=readable_file(draft,rel='FSE/Global_OpenChest/Global_OpenChest.lua')[0]
    before=run(draft,scenario);after=run(readable,scenario)
    assert after.count('quest.GetHero')==before.count('quest.GetHero')
    # Termination-query coalescing is established separately. Preserve all
    # other engine effects, including every hero lookup and both frame exits.
    assert [e for e in after if e!='quest.IsActiveThreadTerminating']==[e for e in before if e!='quest.IsActiveThreadTerminating']
    assert 'resources.DestroyMovie' in after and 'resources.ReleaseResource' in after
