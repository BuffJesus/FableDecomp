local Q = require("GuardianTrophyDealerInfo.quest")
local A = require("GuardianTrophyDealerInfo.runtime_adapter")
function Init(quest) Q.init(A.new(quest)) end
function Main(quest) Q.main(A.new(quest)) end
function OnPersist(quest, context) Q.persist(A.new(quest), context) end
function WaitForPieceOver(quest) A.run(Q, "waiter", quest) end
