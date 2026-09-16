"""Native scoped talk/hit predicates and two-poll presented output classifier."""
import hashlib,json
from pathlib import Path
from tools.script_recovery.teddy_girl_health import prove as main
from tools.script_recovery.lift_native_lua import RData
SOURCE='''function TeddyGirlPresentedKind(resources, output)
    if resources:PollPresentedItem(output) and resources:PresentedItemMatches(output, "OBJECT_TEDDY_BEAR_UNGIVEABLE") then
        return "teddy"
    end
    if resources:PollPresentedItem(output) and not resources:PresentedItemMatches(output, "OBJECT_TEDDY_BEAR_UNGIVEABLE") then
        return "other"
    end
    return "none"
end
'''

def prove(data=None):
    data=data or RData();w=main(data);helpers=json.loads(Path(__file__).with_name('teddy_girl_predicates_witness.json').read_text())
    for row in helpers['pages']:
        raw=data.bytes_at(row['address'],row['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=row['sha256']:raise ValueError('TeddyGirl CString comparison helper changed')
    if data.string_at(0x125d1c8)!='SCRIPT_NAME_HERO' or data.string_at(0x12d8958)!='OBJECT_TEDDY_BEAR_UNGIVEABLE' or data.bytes_at(0x122d70e,1)!=b'\0':raise ValueError('TeddyGirl predicate literals changed')
    return {'nativeMainSha256':w['mainSha256'],'comparisonHelpers':helpers,
        'talkRange':[0xdaf139,0xdaf1df],'hitRange':[0xdb018e,0xdb0267],
        'presentedRange':[0xdaf6e8,0xdaf87a],'presentedOutputStack':16,'excludedAbility':14,
        'limits':['Caller retains one presented CString per full loop iteration; polling never reconstructs it.']}
