"""Verify native argument-key helpers and lifetime through optional reply."""
import hashlib
import json
from pathlib import Path

from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


def verify(data):
    w=json.loads(Path(__file__).with_name('native_affair_wife_argument_key_witness.json').read_text())
    for region in w['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            raise ValueError('AffairWife argument-key bytes changed: '+region['name'])
    for address,value in w['strings'].items():
        if data.string_at(int(address))!=value:raise ValueError('AffairWife argument-key literal changed')
    for offset,target in w['bindings'].items():
        if data.bytes_at(0x1260F0C+int(offset,16),4)!=target.to_bytes(4,'little'):
            raise ValueError('AffairWife text lookup binding changed')
    expected={'start':[0xDB3BBE],'use':[0xDB3BE2,0xDB3C0C,0xDB3C24,0xDB3C6F],'end':[0xDB3C82,0xDB3D8B]}
    if w['keyEvents']!=expected:raise ValueError('AffairWife argument-key event coverage changed')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(data.bytes_at(0xDB2B10,4890),0xDB2B10))
    events={site:(operation,('stack',36)) for operation,sites in expected.items() for site in sites}
    if not check_single_resource_lifetime(instructions,events):
        raise ValueError('AffairWife argument-key lifetime changed')
    return w
