-- Reviewed root and entity lifecycle only. Entity Main is not yet reconstructed.
-- Non-registered reference; native-root-lifecycle-20260912.json pins the evidence.
local Q = {}

function Q.init(h) h:set_spoke(false) end

function Q.main(h)
    h:bind("MazeAtTavern", 0)
    h:finalize()
    h:objective(h:active(), "TEXT_QUEST_GUARDIAN_SISTER_INFO_FIRST_OBJECTIVE_01", "", "BowerstoneSlums")
    while not h:region_loaded("BowerstoneSlums") do
        h:frame()
        if h:terminating() then return end
    end
    if h:terminating() then return end
    h:time(10.0)
end

function Q.persist(h, context)
    h:set_spoke(h:persist_bool(context, "GuardianSpokeToHero", h:spoke(), false))
end

function Q.entity_init()
    -- Keep native offsets until the entity Main uses have been reviewed.
    return {offset_1c = 0, offset_20 = false, offset_24 = 0}
end

function Q.predicate_fail(h)
    if h:spoke() then h:deactivate(h:active(), 0) end
end

return Q
