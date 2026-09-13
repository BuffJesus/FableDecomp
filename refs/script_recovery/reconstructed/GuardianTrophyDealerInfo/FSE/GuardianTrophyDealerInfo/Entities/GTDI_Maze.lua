local Q = require("GuardianTrophyDealerInfo.quest")
local A = require("GuardianTrophyDealerInfo.runtime_adapter")
-- Entity VM owns the angle captured by native Init at entity+1C.
local initial_angle
function Init(quest, me) initial_angle = Q.entity_init(A.new(quest, nil, me)) end
function Main(quest, me)
    assert(initial_angle ~= nil, "GTDI_Maze requires Init before Main")
    A.run(Q, "entity_main", quest, me, initial_angle)
end
