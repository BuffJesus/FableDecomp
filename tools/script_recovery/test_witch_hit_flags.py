"""Projected stack bytes remain mutable; Witch hit gates agree with retail branches."""
import itertools
import json
from pathlib import Path
import subprocess
import sys

import pytest

from tools.script_recovery.native_evidence_lowering import normalise_typed_decompile, rename_scalar_stack_locals
from tools.script_recovery.test_entity_persistence import generated, witch_vm

ROOT = Path(__file__).resolve().parents[2]


def test_projected_byte_is_not_an_object_slot_alias():
    source = "xStack_10_b3 = 0;\nif (hit) {\nxStack_10_b3 = 1;\n}\nif (xStack_10_b3 != 0) {\nCall();\n}"
    result = rename_scalar_stack_locals(source)
    assert result == source.replace('xStack_10_b3', 'native_arg_byte_xStack_10_b3')
    assert rename_scalar_stack_locals(source + '\nTakeAddress(&xStack_10_b3);') == source + '\nTakeAddress(&xStack_10_b3);'


@pytest.mark.parametrize('low', ['0x10', '0', '0xffffff'])
def test_combined_clear_preserves_low_word_and_clears_projected_high_byte(low):
    source = f'  uStack_a8 = uStack_a8 & 0xffffff | {low};'
    result = normalise_typed_decompile(source)
    assert source in result and 'uStack_a8_b3 = 0;' in result


def test_nonzero_high_bits_are_not_reported_as_clear():
    source = '  uStack_a8 = uStack_a8 & 0xffffff | 0x1000000;'
    assert 'uStack_a8_b3 = 0;' not in normalise_typed_decompile(source)


@pytest.mark.parametrize('extra', ['Use(xStack_10_b3);', 'other = xStack_10_b3;',
                                  'xStack_10_b3 = GetFlag();', 'if (Check(xStack_10_b3)) {'])
def test_escaping_or_mixed_projection_stays_unresolved(extra):
    source = 'xStack_10_b3 = 0;\nxStack_10_b3 = 1;\nif (xStack_10_b3 != 0) {\n}\n' + extra
    assert rename_scalar_stack_locals(source) == source


@pytest.mark.parametrize('source', ['if (uStack_10_b2 == 0) {',
                                    'xStack_10_b3 = 1;\nif (xStack_10_b3 != 0) {'])
def test_missing_stores_do_not_create_a_free_global_or_assume_initialization(source):
    assert rename_scalar_stack_locals(source) == source


def test_original_word_lifetime_is_checked_before_dead_store_removal():
    lowered = 'uStack_10_b3 = 0;\nuStack_10_b3 = 1;\nif (uStack_10_b3 != 0) {\n}'
    original = 'uStack_10 = 4;\nuStack_10 = CONCAT13(1,(undefined3)uStack_10);'
    assert rename_scalar_stack_locals(lowered, source=original) == lowered
    assert rename_scalar_stack_locals(lowered, source='Construct(&uStack_10);') == lowered


def native_matrix():
    """Execute retail predicate/cleanup instructions; stop at either real successor."""
    from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
    from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_EBP, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_ESP, UC_X86_REG_EIP
    from tools.script_recovery.lift_native_lua import RData
    image = RData()
    output = []
    for phase, hit, special, heal in itertools.product([1, 4], [False, True], [False, True], [False, True]):
        uc = Uc(UC_ARCH_X86, UC_MODE_32)
        uc.mem_map(0xece000, 0x2000)
        uc.mem_write(0xece7f0, image.bytes_at(0xece7f0, 4083))
        uc.mem_map(0x200000, 0x40000)
        uc.mem_map(0x300000, 0x4000)
        # Constructor/destructor targets are intercepted before executing.
        uc.mem_map(0x99e000, 0x2000)
        def put(at, value): uc.mem_write(at, int(value).to_bytes(4, 'little'))
        def get(at): return int.from_bytes(uc.mem_read(at, 4), 'little')
        put(0x210008, 0x200000)
        targets = {0x220054: ('hit', hit, 4), 0x2200a8: ('special', special, 4), 0x2200a4: ('heal', heal, 8)}
        for target in targets: put(0x200000 + (target & 0xfff), target)
        uc.reg_write(UC_X86_REG_ESI, 0x210000)
        uc.reg_write(UC_X86_REG_EDI, 0x210008)
        uc.reg_write(UC_X86_REG_ESP, 0x302000)
        uc.reg_write(UC_X86_REG_EBX, 0)
        uc.reg_write(UC_X86_REG_EBP, 0)
        entry, yes, no = (0xeceee5, 0xecefb1, 0xecf1a6) if phase == 1 else (0xecf4fe, 0xecf5c6, 0xecf74d)
        trace, result = [], []
        def hook(machine, pc, size, _):
            if pc in (yes, no):
                result.append(pc == yes); machine.emu_stop(); return
            if pc in targets:
                name, value, purge = targets[pc]
                trace.append(name)
                if name == 'heal': assert get(machine.reg_read(UC_X86_REG_ESP) + 4) == 14
            elif pc == 0x99ebf0:
                value, purge = 0, 8
            elif pc == 0x99eae0:
                value, purge = 0, 0
            else:
                return
            sp = machine.reg_read(UC_X86_REG_ESP)
            machine.reg_write(UC_X86_REG_EAX, int(value))
            machine.reg_write(UC_X86_REG_ESP, sp + 4 + purge)
            machine.reg_write(UC_X86_REG_EIP, get(sp))
        uc.hook_add(UC_HOOK_CODE, hook)
        uc.emu_start(entry, 0xecf800, count=1000)
        assert len(result) == 1 and uc.reg_read(UC_X86_REG_ESP) == 0x302000
        assert result[0] == (hit or (special and not heal))
        assert uc.mem_read(0x302013, 1)[0] == int(result[0])
        output.append(dict(phase=phase, hit=hit, special=special, heal=heal, result=result[0], trace=trace))
    return output


@pytest.fixture(scope='module')
def retail():
    process = subprocess.run([sys.executable, '-m', 'tools.script_recovery.test_witch_hit_flags'],
                             cwd=ROOT, capture_output=True, text=True, check=True)
    return json.loads(process.stdout)


@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('case', range(16))
def test_generated_witch_hit_gate_matches_retail(generated, retail, stage, case):
    expected = retail[case]
    lua = witch_vm((generated/stage/'FSE/V_SickChild/Entities/Witch.lua').read_text())
    lua.globals().scenario = lua.table_from(expected)
    lua.execute('''
        snapshot={IntroDone=true,ThingsForPotionGot=scenario.phase}; loading=true
        OnPersist(quest,me,context); loading=false
        local member, boundary, resources = {}, {}, {}
        local frames, movies, trace = 0, 0, {}
        function resources:MemberResource(key) assert(key=='seh_Witch'); return member end
        function resources:PrepareResource(v) assert(v==member) end
        function resources:TryAcquire(v,m,p) assert(v==member and m==me and p==4); return true end
        function resources:StartMovie() movies=movies+1; error(boundary) end
        function quest:RetailResources() return resources end
        function quest:GetHero() return hero end
        function quest:NewScriptFrame(v)
            assert(v==me); frames=frames+1; assert(frames<5)
            if frames>=3 then stop=true end
            return not stop
        end
        function quest:IsActiveThreadTerminating() return stop end
        function me:IsTalkedToByHero() return false end
        function me:MsgIsHitByHero() trace[#trace+1]='hit'; return scenario.hit end
        function me:MsgIsHitByAnySpecialAbilityFromHero() trace[#trace+1]='special'; return scenario.special end
        function me:MsgIsHitByHeroSpecialAbility(ability)
            assert(ability==14); trace[#trace+1]='heal'; return scenario.heal
        end
        local ok,err=pcall(Main,quest,me)
        if scenario.result then assert(not ok and err==boundary, tostring(err)); assert(movies==1)
        else assert(ok,err); assert(movies==0 and stop) end
        observedTrace=table.concat(trace,',')
    ''')
    # Some phases poll twice before the bounded frame exit; each evaluation
    # must preserve retail short-circuit order and skip healing-only reactions.
    trace = ','.join(expected['trace'])
    assert lua.globals().observedTrace in (trace, trace + ',' + trace)


if __name__ == '__main__':
    print(json.dumps(native_matrix()))
