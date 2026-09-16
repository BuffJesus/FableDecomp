import tempfile
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime

from tools.script_recovery.generate_book_trader_resource_candidate import DRAFT, generate, verified_hit_scope, verified_facing
from tools.script_recovery.lift_native_lua import RData


MOCK = r'''
events = {}
local function record(name, ...) events[#events+1] = {name, ...} end
local live, next_id, paused = {}, 0, false
local function create(kind)
    next_id = next_id + 1; live[next_id] = kind
    record('new.' .. kind, next_id); return next_id
end
local function require_kind(id, kind) assert(live[id] == kind, 'wrong/dead ' .. kind) end
local function destroy(id, kind)
    require_kind(id, kind); record('destroy.' .. kind, id); live[id] = nil
end
local resources = {}
function resources:IsHitByHeroExceptAbility(actor, ability)
    assert(actor == me and ability == 14)
    if options.trace_hit then record('hit.scope', ability) end
    return options.hit or (options.special and not options.excluded) or false
end
function resources:NewResource() return create('resource') end
function resources:PrepareResource(id) require_kind(id, 'resource'); record('prepare', id) end
function resources:TryAcquire(id, actor, priority)
    require_kind(id, 'resource'); assert(actor == me); record('acquire', id, priority)
    return not options.fail_acquire
end
function resources:ReleaseResource(id)
    assert(not paused)
    for _,kind in pairs(live) do assert(kind == 'resource', 'resource released before child') end
    destroy(id, 'resource')
end
function resources:NewThingFromResource(id) require_kind(id, 'resource'); return create('thing') end
function resources:DestroyThing(id) destroy(id, 'thing') end
function resources:ThingHealth(id)
    require_kind(id, 'thing'); record('health', id)
    if options.error_at == 'health' then error('injected health failure') end
    return options.health or 100
end
local moves = 0
function resources:ThingIsDistanceFromPositionOver(id, position, threshold)
    require_kind(id, 'thing'); assert(threshold == 2.0); record('distance', id)
    return moves == 0
end
function resources:MoveToPosition(id, position, radius, move_type, a, b)
    require_kind(id, 'resource'); assert(radius == 0 and move_type == 0 and not a and b)
    moves = moves + 1; record('move', id, position.x, position.y, position.z)
end
function resources:StartMovie(class) assert(class == ''); return create('movie') end
function resources:DestroyMovie(id) destroy(id, 'movie') end
function resources:Pause(value) paused = value; record('pause', value) end
function resources:Speak(id, actor, text, selection, listen, sound2d, overfade)
    require_kind(id, 'resource'); assert(actor == hero)
    assert(selection == 0 and not listen and sound2d and not overfade)
    record('speak', id, text)
    if options.error_at == 'speak' then error('injected speech failure') end
end
function resources:IsPerformingScriptTask(id)
    require_kind(id, 'resource'); record('task', id); return options.busy or false
end
function resources:ReadAnimationArgument5() record('animation.byte', options.animation_byte); return options.animation_byte end
function resources:PlayAnimation(id, name, a,b,c,d,byte,e)
    require_kind(id, 'resource'); assert(not a and not b and not c and d and not e)
    record('animation', id, byte)
end
local frames, checks = 0, 0
hero = {name='hero'}
me = {}
function me:GetHomePos() return {x=-17.5,y=4,z=8} end
function me:IsDistanceFromPositionOver(position, distance) assert(distance == 0.1); return options.move or false end
function me:MsgIsHitByHero() error('unscoped hit query') end
function me:MsgIsHitByAnySpecialAbilityFromHero() error('unscoped hit query') end
function me:MsgIsHitByHeroSpecialAbility() error('unscoped hit query') end
function me:IsTalkedToByHero() return options.talk or false end
quest = {}
function quest:WithRetailResources(body)
    local ok, err = pcall(body, resources)
    if paused then resources:Pause(false) end
    for id=next_id,1,-1 do
        if live[id] == 'thing' then resources:DestroyThing(id)
        elseif live[id] == 'movie' then resources:DestroyMovie(id)
        elseif live[id] == 'resource' then resources:ReleaseResource(id) end
    end
    assert(next(live) == nil)
    if not ok then error(err) end
end
function quest:NewScriptFrame() frames = frames+1; return true end
function quest:IsActiveThreadTerminating()
    checks = checks+1
    return options.stop_check == checks or frames >= (options.stop_frame or 2)
end
function quest:GetHero() return hero end
function quest:GetStateInt() return 17 end
local quest_flags = {GivenSweets=options.given_sweets or false}
function quest:GetStateBool(name) return quest_flags[name] or false end
function quest:SetStateBool(name, value) quest_flags[name]=value; record('state', name, value) end
function quest:GiveHeroYesNoQuestion(prompt, yes, no, help, flag)
    assert(prompt=='TEXT_QST_048_TRADER_BUY_SWEETS' and yes=='TEXT_OBJECT_HERO_ANSWER_YES')
    assert(no=='TEXT_OBJECT_HERO_ANSWER_NO' and help=='' and flag)
    record('question', prompt)
end
local answers = 0
function quest:MsgIsQuestionAnsweredYesOrNo()
    answers=answers+1
    return answers <= (options.wait_answers or 0) and -1 or (options.answer or 1)
end
function quest:GetHeroGold() return options.gold or 0 end
function quest:GiveHeroObject(name, variation) record('object',name,variation) end
function quest:GiveHeroGold(amount) record('gold',amount) end
function quest:GetActiveQuestName() return 'active-novi' end
function quest:SetQuestCardObjective(name, objective, a, b)
    assert(name=='active-novi' and a=='' and b=='');record('objective',objective)
end
function quest:ClearThingHasInformation(actor) assert(actor==me);record('clear.info') end
function quest:GetTimer() return options.animation_byte and 0 or 7 end
function quest:RetailRandModulo() return 0 end
function quest:IsDistanceBetweenThingsUnder() return true end
function quest:GetThingWithScriptName(name) return {name=name} end
function quest:AddNewConversation() return 19 end
function quest:StartConversationWithHero(actor, soundIn2D, playDuringCutscene)
    assert(actor == me and soundIn2D == false and playDuringCutscene == false)
    if options.trace_conversation then record('conversation.setup') end
    if options.error_at == 'conversation.setup' then error('injected conversation setup failure') end
    return 19
end
function quest:AddConversationLineToHero(conversation, text, actor, subtitle)
    assert(conversation == 19 and text == 'TEXT_QST_048_TRADER_ROLL_UP' and actor == me and subtitle == false)
    if options.trace_conversation then record('conversation.line', conversation, text) end
    if options.error_at == 'conversation' then error('injected conversation failure') end
end
for _,name in ipairs({'EntitySetAsDamageable','EntitySetAsKillable','EntitySetAsToAddToComboMultiplierWhenHit',
    'SetThingHasInformation','SetIsPushableByHero','EntitySetAsUseMovementInActions',
    'EntitySetDeedReactionsEnabled','EntitySetFacingAngleTowardsThing','EntitySetThingAsAllyOfThing',
    'SetTimer','AddPersonToConversation','AddLineToConversation'}) do quest[name] = function() end end
package.preload['NewOakValeIntro.native_quest_helpers'] = function()
    return {AddBadDeed=function() end}
end
function quest:FaceThingByScriptName(actor, name, snap)
    assert(actor == me and name == 'NOVI_Theresa' and type(snap) == 'boolean')
    if options.trace_facing then record('face', snap) end
    if options.error_at == 'face' then error('injected facing failure') end
end
'''


class BookTraderCandidateTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source, cls.report = generate()

    def run_case(self, **options):
        lua = LuaRuntime()
        lua.globals().options = lua.table_from(options)
        lua.execute(MOCK)
        lua.execute(self.source)
        lua.execute('Init(quest,me)')
        error = None
        try:
            lua.execute('Main(quest,me)')
        except Exception as exc:
            error = str(exc)
        events = [tuple(row[i] for i in range(1, len(row)+1)) for row in lua.globals().events.values()]
        return events, error

    def test_scoped_conversation_line_preserves_arguments_and_error_cleanup(self):
        for failure in (False, True):
            events, error = self.run_case(animation_byte=47, trace_conversation=True,
                error_at='conversation' if failure else None)
            self.assertIn(('conversation.line', 19, 'TEXT_QST_048_TRADER_ROLL_UP'), events)
            self.assertLess(events.index(('conversation.setup',)), events.index(('conversation.line', 19, 'TEXT_QST_048_TRADER_ROLL_UP')))
            self.assertEqual(events[-1], ('destroy.resource', 1))
            if failure:
                self.assertIn('injected conversation failure', error)
            else:
                self.assertIsNone(error)
        self.assertNotIn('native_arg_book_listener = quest:GetHero()', self.source)
        self.assertNotIn('quest:AddNewConversation(', self.source)
        self.assertNotIn('quest:AddPersonToConversation(', self.source)
        events, error = self.run_case(animation_byte=47, trace_conversation=True, error_at='conversation.setup')
        self.assertIn('injected conversation setup failure', error)
        self.assertEqual(events[-1], ('destroy.resource', 1))

    def test_hit_movie_health_speech_and_release_order(self):
        events, error = self.run_case(hit=True)
        self.assertIsNone(error)
        self.assertEqual(events, [('new.resource', 1), ('prepare', 1), ('acquire', 1, 3),
            ('prepare', 1), ('acquire', 1, 4), ('new.movie', 2), ('pause', True),
            ('new.thing', 3), ('health', 3), ('destroy.thing', 3),
            ('speak', 1, 'TEXT_QST_048_TRADER_ON_HIT'), ('task', 1),
            ('pause', False), ('destroy.movie', 2), ('destroy.resource', 1)])

    def test_scoped_hit_result_drives_native_branch(self):
        # Helper RAII/short-circuit mechanics have compiled runtime coverage;
        # here prove the generated caller uses its result for every combination.
        for direct in (False, True):
            for special in (False, True):
                for excluded in (False, True):
                    with self.subTest(direct=direct, special=special, excluded=excluded):
                        events, error = self.run_case(hit=direct, special=special,
                            excluded=excluded, trace_hit=True)
                        self.assertIsNone(error)
                        self.assertIn(('hit.scope', 14), events)
                        spoken = any(e[0] == 'speak' and e[2] == 'TEXT_QST_048_TRADER_ON_HIT' for e in events)
                        self.assertEqual(spoken, direct or (special and not excluded))
        self.assertNotIn('uStack_120 =', self.source)

    def test_hit_scope_rejects_changed_cleanup_or_source_name(self):
        real = RData()
        class Changed:
            def __init__(self, name=False): self.name = name
            def bytes_at(self, address, size):
                raw = real.bytes_at(address, size)
                if not self.name and address == 0xDB4234:
                    raw = raw[:-1] + bytes([raw[-1] ^ 1])
                return raw
            def string_at(self, address):
                return 'OTHER_HERO' if self.name else real.string_at(address)
        for data in (Changed(), Changed(name=True)):
            with self.assertRaisesRegex(ValueError, 'hit scope evidence changed'):
                verified_hit_scope(data)

    def test_facing_preserves_staged_snap_argument_across_lookup(self):
        events, error = self.run_case(hit=True, trace_facing=True)
        self.assertIsNone(error)
        self.assertEqual([e for e in events if e[0] == 'face'], [('face', True)])
        events, error = self.run_case(move=True, trace_facing=True, stop_frame=3)
        self.assertIsNone(error)
        self.assertIn(('face', False), events)
        self.assertEqual(self.source.count('FaceThingByScriptName(me, "NOVI_Theresa", false)'), 2)
        self.assertEqual(self.source.count('FaceThingByScriptName(me, "NOVI_Theresa", true)'), 1)
        self.assertNotIn('quest:GetThingWithScriptName("NOVI_Theresa")', self.source)
        real = RData()
        class Changed:
            def bytes_at(self, address, size):
                raw = real.bytes_at(address, size)
                # Change getter ret8 to ret12: the staged third argument must
                # no longer be treated as surviving for the facing call.
                if address == 0x8A7D60:
                    raw = raw[:0x95] + bytes([12]) + raw[0x96:]
                return raw
            def string_at(self, address): return real.string_at(address)
        with self.assertRaisesRegex(ValueError, 'facing operands or ABI changed'):
            verified_facing(Changed())

    def test_facing_adapter_error_unwinds_enclosing_resource_and_movie(self):
        for scenario in ({'hit': True}, {'move': True, 'stop_frame': 3}):
            events, error = self.run_case(error_at='face', **scenario)
            self.assertIn('injected facing failure', error)
            self.assertEqual(sum(e[0] == 'destroy.resource' for e in events), 1)
            self.assertEqual(sum(e[0] == 'destroy.movie' for e in events), int(scenario.get('hit', False)))

    def test_purchase_threshold_and_side_effect_order(self):
        for gold in (0, 2, 3, 99):
            with self.subTest(gold=gold):
                events, error=self.run_case(talk=True, gold=gold)
                self.assertIsNone(error)
                effects=[e for e in events if e[0] in ('object','gold','objective','state','clear.info')]
                self.assertEqual(effects, [
                    ('object','OBJECT_CHOCOLATE_BOX_UNGIVEABLE',-1), ('gold',-3),
                    ('objective','TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_04'),
                    ('state','GivenSweets',True), ('clear.info',)] if gold>=3 else [])
                speech=[e[2] for e in events if e[0]=='speak']
                self.assertEqual(speech, ['TEXT_QST_048_TRADER_INTRO',
                    'TEXT_QST_048_TRADER_GIVES_SWEETS' if gold>=3 else 'TEXT_QST_048_TRADER_NOT_ENOUGH_CASH'])

    def test_decline_pending_cancellation_and_repeat_purchase(self):
        events,error=self.run_case(talk=True,gold=3,answer=0)
        self.assertIsNone(error)
        self.assertIn(('speak',1,'TEXT_QST_048_TRADER_BUY_LATER'),events)
        self.assertFalse(any(e[0]=='object' for e in events))
        events,error=self.run_case(talk=True,gold=3,wait_answers=10)
        self.assertIsNone(error)
        self.assertFalse(any(e[0]=='object' for e in events))
        self.assertEqual(events[-3:], [('pause',False),('destroy.movie',2),('destroy.resource',1)])
        events,error=self.run_case(talk=True,gold=3,stop_frame=3)
        self.assertIsNone(error)
        self.assertEqual(sum(e[0]=='object' for e in events),1)
        self.assertEqual(sum(e[0]=='question' for e in events),1)
        self.assertIn(('speak',1,'TEXT_QST_048_TRADER_INTRO_10'),events)

    def test_all_reached_termination_checks_release_once_after_children(self):
        for check in range(1, 17):
            with self.subTest(check=check):
                events, error = self.run_case(hit=True, stop_check=check, stop_frame=100)
                self.assertIsNone(error)
                creates = [e for e in events if e[0] == 'new.resource']
                releases = [e for e in events if e[0] == 'destroy.resource']
                self.assertEqual(len(creates), 0 if check == 1 else 1)
                self.assertEqual(len(creates), len(releases))
        events, error = self.run_case(fail_acquire=True)
        self.assertIsNone(error)
        self.assertEqual(events, [('new.resource', 1), ('prepare', 1), ('acquire', 1, 3), ('destroy.resource', 1)])
        # DB41C7 is the fourth destruction route: cancellation after a home
        # movement task finishes, before the next distance iteration.
        events, error = self.run_case(move=True, stop_check=6, stop_frame=100)
        self.assertIsNone(error)
        self.assertIn(('move', 1, -17.5, 4, 8), events)
        self.assertEqual(sum(e[0] == 'destroy.resource' for e in events), 1)

    def test_movement_uses_scoped_thing_and_animation_reads_live_byte(self):
        events, error = self.run_case(move=True, stop_frame=3)
        self.assertIsNone(error)
        self.assertIn(('move', 1, -17.5, 4, 8), events)
        self.assertEqual(sum(e[0] == 'new.thing' for e in events), 2)
        for value in (0, 47, 255):
            events, error = self.run_case(animation_byte=value)
            self.assertIsNone(error)
            self.assertIn(('animation.byte', value), events)
            self.assertIn(('animation', 1, value), events)

    def test_dead_nan_and_task_cancellation_keep_movie_cleanup(self):
        for health in (0, -1, float('nan')):
            events, error = self.run_case(hit=True, health=health)
            self.assertIsNone(error)
            self.assertFalse(any(e[0] == 'speak' for e in events))
            self.assertEqual(events[-3:], [('pause', False), ('destroy.movie', 2), ('destroy.resource', 1)])
        events, error = self.run_case(hit=True, busy=True)
        self.assertIsNone(error)
        self.assertEqual(events[-3:], [('pause', False), ('destroy.movie', 2), ('destroy.resource', 1)])

    def test_errors_close_temporary_movie_and_resource_without_double_release(self):
        for location in ('health', 'speak'):
            events, error = self.run_case(hit=True, error_at=location)
            self.assertIn('injected', error)
            self.assertEqual(sum(e[0] == 'destroy.thing' for e in events), 1)
            self.assertEqual(sum(e[0] == 'destroy.movie' for e in events), 1)
            self.assertEqual(sum(e[0] == 'destroy.resource' for e in events), 1)

    def test_changed_draft_rejects_before_writing(self):
        with tempfile.TemporaryDirectory() as directory:
            draft = Path(directory) / 'draft.lua'
            draft.write_text(DRAFT.read_text(encoding='utf-8') + '\n', encoding='utf-8')
            output = Path(directory) / 'candidate'
            with self.assertRaisesRegex(ValueError, 'draft changed'):
                generate(output, draft_path=draft)
            self.assertFalse(output.exists())


if __name__ == '__main__':
    unittest.main()
