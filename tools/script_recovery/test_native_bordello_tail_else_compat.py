import importlib.util
from pathlib import Path
import pytest
from tools.script_recovery.test_native_reviewed_bordello_lady_return import evidence,SOURCE,run_decline
from tools.script_recovery import native_reviewed_bordello_lady_return as m
@pytest.mark.parametrize('owned,hit,magic,heal',[(o,h,g,e) for o in (False,True) for h,g,e in [(False,False,False),(True,False,False),(False,True,False),(False,True,True)]])
def test_restored_native_else_reuses_identical_reviewed_behavior(evidence,owned,hit,magic,heal):
 fn,row,rdata=evidence;s=SOURCE.read_text();assert s.count(m.CLONED_HIT)==1
 changed=s.replace(m.CLONED_HIT,m.CLONED_HIT_WITH_ELSE)
 before=m.restore(s,fn,row,rdata);after=m.restore(changed,fn,row,rdata)
 assert before==after
 assert run_decline(before,owned=owned,hit=hit,magic=magic,heal=heal)==run_decline(after,owned=owned,hit=hit,magic=magic,heal=heal)
@pytest.mark.parametrize('replacement',['goto LAB_00e40184_c2','goto LAB_00e40183_c3','return','goto LAB_00e402f9_c2'])
def test_drifted_restored_else_rejected(evidence,replacement):
 fn,row,rdata=evidence;s=SOURCE.read_text();block=m.CLONED_HIT_WITH_ELSE.replace('                goto LAB_00e40183_c2', '                '+replacement)
 with pytest.raises(ValueError):m.restore(s.replace(m.CLONED_HIT,block),fn,row,rdata)
def test_duplicate_restored_hit_region_rejected(evidence):
 fn,row,rdata=evidence;s=SOURCE.read_text().replace(m.CLONED_HIT,m.CLONED_HIT_WITH_ELSE*2)
 with pytest.raises(ValueError):m.restore(s,fn,row,rdata)
