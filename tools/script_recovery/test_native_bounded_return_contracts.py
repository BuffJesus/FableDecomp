import pytest
from tools.script_recovery import native_bounded_return_contracts as P

class Image:
 def __init__(self,raw):self.raw=raw
 def bytes_at(self,a,n):return self.raw[a-0x1000:a-0x1000+n] if 0x1000<=a and a+n<=0x1000+len(self.raw) else None

def test_explicit_large_budget_does_not_raise_the_default_cap():
 raw=b'\x90'*5000+b'\xc3';image=Image(raw);bounds=[(0x1000,0x1000+len(raw))]
 with pytest.raises(ValueError,match='instruction bound exceeded'):P.inspect(image,0x1000,bounds)
 result=P.inspect(image,0x1000,bounds,max_instructions=5001)
 assert len(result['instructions'])==5001 and result['purgeBytes']==0

@pytest.mark.parametrize('budget',[0,-1,True,False,1.5,'5000',65537,None])
def test_instruction_budget_is_explicit_integral_and_bounded(budget):
 with pytest.raises(ValueError,match='invalid instruction budget'):P.inspect(Image(b'\xc3'),0x1000,[(0x1000,0x1001)],max_instructions=budget)

@pytest.mark.parametrize('raw,purge,error',[
 (b'\xc3',0,None),
 (bytes.fromhex('85c07401c3c3'),0,None),
 (bytes.fromhex('e800000000c20400'),4,None),
 (bytes.fromhex('85c07403c20400c3'),None,'nonuniform return operands'),
 (bytes.fromhex('ebfe'),None,'reachable region without normal return path'),
 (bytes.fromhex('7402ebfec3'),None,'reachable region without normal return path'),
])
def test_normal_return_property_and_refusals_are_preserved(raw,purge,error):
 image=Image(raw);bounds=[(0x1000,0x1000+len(raw))]
 if error:
  with pytest.raises(ValueError,match=error):P.inspect(image,0x1000,bounds)
 else:assert P.inspect(image,0x1000,bounds)['purgeBytes']==purge

def test_default_budget_boundary_is_exact():
 raw=b'\x90'*4095+b'\xc3';result=P.inspect(Image(raw),0x1000,[(0x1000,0x1000+len(raw))])
 assert len(result['instructions'])==4096
 with pytest.raises(ValueError,match='instruction bound exceeded'):P.inspect(Image(raw),0x1000,[(0x1000,0x1000+len(raw))],max_instructions=4095)

