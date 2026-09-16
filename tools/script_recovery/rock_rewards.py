"""Dynamic CDefString reward consumption while the outer self resource stays live."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData

SOURCE='''-- Continuation phase; no Main and no resource ownership transfer.
function WithRockTrollRewardsPhase(quest, me, continuation)
    if not quest:GetStateBool("AddedItemsToRockTroll") then
        if quest:IsActiveThreadTerminating() then return end
        quest:AddRockTrollReward(me, 1)
        quest:AddRockTrollReward(me, 2)
        quest:SetStateBool("AddedItemsToRockTroll", true)
    end
    continuation()
end
'''


def recover(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('rock_rewards_witness.json').read_text())
    for region in w['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:raise ValueError('Rock reward native operand changed')
    for address,value in w['strings'].items():
        if data.string_at(int(address,16))!=value:raise ValueError('Rock reward field name changed')
    return SOURCE,w
