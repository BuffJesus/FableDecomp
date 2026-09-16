"""Verified initial-objective caller and active-name output ABI."""
import hashlib
from tools.script_recovery.lift_native_lua import RData


def verify(data=None):
    data=data or RData()
    regions=[(0xdac198,0x81,'f2809a6bfdf0bcda0d5b75546a8f4d55f5e4a92a8297993a013f38b5d71971ff'),
             (0x891880,62,'bfe123a257c311e7914a061407d1ae85e7a9eb30267958f8652417bce80bf66f')]
    for address,size,digest in regions:
        if hashlib.sha256(data.bytes_at(address,size)).hexdigest()!=digest:
            raise ValueError('Oakvale objective caller/getter changed')
    for slot,target in ((0xa3c,0x891880),(0x4a0,0x896a30)):
        if int.from_bytes(data.bytes_at(0x1260f0c+slot,4),'little')!=target:
            raise ValueError('Oakvale objective API slot changed')
    if data.bytes_at(0x896b53,3)!=b'\xc2\x10\x00':
        raise ValueError('Oakvale objective callee stack convention changed')
    if data.bytes_at(0x122d70e,1)!=b'\0' or data.string_at(0x12d8244)!='TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01':
        raise ValueError('Oakvale objective literals changed')
    return dict(regions=[dict(address=a,size=n,sha256=h) for a,n,h in regions],
                status='native objective caller and getter ABI verified',
                cleanup=['activeNameOutput','objective','region1','region2'])


def lower(source, data=None):
    evidence=verify(data)
    before='''    -- TODO(native): p_Var6 = (_func_void *)&local_4;
    local ppVar3 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(ppVar3, "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01", "", "")
'''
    if source.count(before)!=1:raise ValueError('Oakvale objective draft correspondence changed')
    after='''    quest:WithRetailResources(function(resources)
        resources:SetInitialOakvaleObjective()
    end)
'''
    return source.replace(before,after),dict(evidence=evidence,method='SetInitialOakvaleObjective',
        remaining=['Complete Main and engine scheduling/persistence remain pending.',
                   'Callee failure before constructing the returned output and arbitrary native exceptions remain unproved.'])
