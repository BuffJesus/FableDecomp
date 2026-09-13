-- Disabled reviewed port; requires source runtime RetailFlags/retained things.
local Q = require("MazeResearch.quest")
local A = require("MazeResearch.runtime_adapter")
function Init(quest) Q.init(A.new(quest)) end
function Main(quest) Q.main(A.new(quest)) end
function OnPersist(quest, context) Q.persist(A.new(quest), context) end
function UnLimboSword(quest) A.run(Q, "unlimbo", quest) end
