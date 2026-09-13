-- Native entity Init/OnPersist/OnPredicateFail/OnInterrupted are empty.
local reference = require("MeetSister.quest")
local adapter = require("MeetSister.runtime_adapter")
function Init(quest, me) end
function Main(quest, me)
    adapter.run(reference, "messenger", quest, me)
end
