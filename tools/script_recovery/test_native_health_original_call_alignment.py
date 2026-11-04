from pathlib import Path
import copy,json,sys
import pytest
W=Path(__file__).resolve().parent
from tools.script_recovery import native_health_copy_correspondence as M
F=W/'fixtures/mans_resource_health'
read=lambda name:json.loads((F/name).read_text())
def inputs():return read('phase.json')['fn'],read('before.json'),read('after.json'),(F/'phase.lua').read_text()
def test_complete_original_inventory_survives_unrelated_normalization():
 fn,before,after,source=inputs()
 assert len(fn['callOrder'])==274
 assert M._text_order_sites(fn['decompile'],fn) is not None
 assert M._text_order_sites(M.bind_st0_results(M.normalise_typed_decompile(fn['decompile'])),fn) is None
 bound=M.bind_st0_results(M.join_wrapped_statements(fn['decompile']))
 assert len(M._text_order_sites(bound,fn))==222
 result=M.prove(fn,before,after,source)
 assert result['nativeSites']==result['emittedUses']==13
@pytest.mark.parametrize('fault',['missingCall','extraCall','swapCall','dropRand'])
def test_full_original_inventory_still_rejects(fault):
 fn,before,after,source=inputs()
 if fault=='missingCall':fn['callOrder'].pop(27)
 elif fault=='extraCall':fn['callOrder'].insert(27,fn['callOrder'][27])
 elif fault=='swapCall':fn['callOrder'][27],fn['callOrder'][28]=fn['callOrder'][28],fn['callOrder'][27]
 else:
  assert 'rand(' in fn['decompile']
  fn['decompile']=fn['decompile'].replace('rand(','0+(')
 with pytest.raises(ValueError):M.prove(fn,before,after,source)
def test_noncommuting_normalization_rejects(monkeypatch):
 fn,before,after,source=inputs();normal=M.normalise_typed_decompile
 bound=M.bind_st0_results(M.join_wrapped_statements(fn['decompile']))
 assert bound!=fn['decompile']
 monkeypatch.setattr(M,'normalise_typed_decompile',lambda text:normal(text)+('\n-- changed' if text==bound else ''))
 with pytest.raises(ValueError,match='binding/normalization correspondence'):M.prove(fn,before,after,source)
