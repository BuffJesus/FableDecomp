from pathlib import Path
from copy import copy,deepcopy
import hashlib,sys
import pytest
from tools.script_recovery.test_native_reviewed_interface_table_staging import baseline,run

def patch_image(original,address,replacement):
    image=copy(original);rva=address-image.base
    offsets=[raw+rva-sva for sva,size,raw in image.raw_secs if sva<=rva and rva+len(replacement)<=sva+size]
    assert len(offsets)==1
    offset=offsets[0];image.data=image.data[:offset]+replacement+image.data[offset+len(replacement):]
    return image

@pytest.mark.parametrize('address,replacement,reason',[
 (0xe551c8,bytes.fromhex('08'),'wrong field'),
 (0xe551d9,bytes.fromhex('b30090'),'partial saved-register clobber'),
 (0xe55316,bytes.fromhex('8b02'),'wrong table source'),
 (0xe55318,bytes.fromhex('8bca'),'wrong receiver source'),
 (0xe5531b,bytes.fromhex('21090000'),'wrong shared branch destination'),
 (0xe55c40,bytes.fromhex('01'),'wrong boolean'),
 (0xe55c43,bytes.fromhex('e8050000'),'wrong slot'),
 (0xe55232,(0xe55318-(0xe55230+6)).to_bytes(4,'little',signed=True),'branch bypasses table load'),
])
def test_native_bytes_are_checked_beyond_resealed_identity(baseline,address,replacement,reason):
    image=patch_image(baseline['image'],address,replacement);row=deepcopy(baseline['row'])
    row['imageSha256']=hashlib.sha256(image.data).hexdigest()
    row['nativeSha256']=hashlib.sha256(image.bytes_at(0xe54e90,3534)).hexdigest()
    with pytest.raises(ValueError):run(baseline,row=row,image=image)
