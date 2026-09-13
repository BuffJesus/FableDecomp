local Q = require("MazeResearch.quest")
local A = require("MazeResearch.runtime_adapter")
function Init(quest, me) end
function Main(quest, me) A.run(Q, "history", quest, me) end
