"""An entering branch can bypass an otherwise uninterrupted table definition."""
import hashlib
import pytest
from tools.script_recovery.test_native_reviewed_dispatch_staging import inputs
from tools.script_recovery import native_reviewed_dispatch_staging as classifier


@pytest.mark.parametrize('target',[0xe5779c,0xe577a3,0xe577b2])
def test_branch_into_table_lifetime_rejects(inputs,target):
    original=inputs['rdata'];fn=inputs['fn'];start=int(fn['address'],16)
    raw=bytearray(original.bytes_at(start,int(fn['bodyEndExclusive'],16)-start))
    branch=0xe57794
    raw[branch-start+2:branch-start+6]=(target-(branch+6)).to_bytes(4,'little',signed=True)
    class Mutant:
        def bytes_at(self,address,size):
            if start<=address and address+size<=start+len(raw):
                return bytes(raw[address-start:address-start+size])
            return original.bytes_at(address,size)
    inputs['rdata']=Mutant()
    inputs['row']['nativeExtentSha256']=hashlib.sha256(raw).hexdigest()
    inputs['parent']['nativeSha256']=hashlib.sha256(raw[:fn['size']]).hexdigest()
    with pytest.raises(ValueError,match='entry into table lifetime'):
        classifier.classify(**inputs)
