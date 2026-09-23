"""A stack resource's member zero-init must not clobber the handle (2026-09-22, found in-game).

Q_WaspBoss::WaspIntro 0x00E12F20 builds two resources on the stack. Ghidra spells each inlined
constructor as a vtable store plus one zero per member:

    CBaseIntelligentPointer::CBaseIntelligentPointer(appuStack_20);
    appuStack_20[0] = &PTR__scalar_deleting_destructor__0127094c;   -- folds to RESOURCE_NewResource()
    CStack_18 = (CCharString_bv)0x0;                                -- the object's OWN members
    piStack_14 = (int *)0x0;

`canonicalise_stack_objects` folds all three slot names onto the object, which used to leave
`xStack_20 = RESOURCE_NewResource(); xStack_20 = (CCharString)0x0;` -- and the zero won, so the handle
was dead on arrival: the lifter dropped the construction and folded every later read to the constant.
The Lua came out as a bare `resources:NewResource()` statement plus `resources:TryAcquire(0, ...)`,
`SetActor(..., 0)` and `ReleaseResource(0)`, and the sidecar threw
`LUA RUNTIME ERROR in thread 'DoMission': Invalid or released retail resource` at the wasp intro
(autopilot run 36), so the quest never played its cutscene.
"""
import re
import unittest
from pathlib import Path

import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))

from tools.script_recovery.native_evidence_lowering import drop_member_zero_inits  # noqa: E402

WASP = ROOT / 'refs/script_recovery/lifted/WaspBoss/readable/FSE/WaspBoss/WaspBoss.lua'


class MemberZeroInitTests(unittest.TestCase):
    def test_zero_after_construction_is_dropped(self):
        text = ("  xStack_20 = RESOURCE_NewResource();\n"
                "  xStack_20 = (CCharString)0x0;\n"
                "  ePriority = 4;\n")
        out = drop_member_zero_inits(text)
        self.assertNotIn('= (CCharString)0x0', out)
        self.assertIn('xStack_20 = RESOURCE_NewResource();', out)
        self.assertIn('ePriority = 4;', out, 'only the object\'s own zeroes may go')

    def test_a_later_zero_is_left_alone(self):
        """Only the zeroes contiguous with the construction are members; a real assignment stays."""
        text = ("  xStack_20 = RESOURCE_NewResource();\n"
                "  ePriority = 4;\n"
                "  xStack_20 = 0;\n")
        self.assertEqual(drop_member_zero_inits(text), text)

    def test_every_object_kind_is_covered(self):
        for ctor in ('RESOURCE_NewResource()', 'RESOURCE_StartMovie("")', 'ACTORMAP_New()',
                     'STRINGMAP_New()', 'QUESTTHING_Empty()'):
            with self.subTest(ctor=ctor):
                out = drop_member_zero_inits(f"  x = {ctor};\n  x = (int *)0x0;\n")
                self.assertNotIn('0x0', out)


class WaspIntroResourcesTests(unittest.TestCase):
    """The shipped lift is the evidence that the fix reaches the package."""

    def setUp(self):
        if not WASP.is_file():
            self.skipTest('WaspBoss readable package not generated')
        self.src = WASP.read_text(encoding='utf-8')

    def test_no_resource_call_takes_the_literal_zero(self):
        for call in ('TryAcquire', 'ReleaseResource', 'DestroyMovie', 'DestroyActorMap'):
            for m in re.finditer(r'resources:' + call + r'\((\s*0\s*[,)])', self.src):
                self.fail(f'{call} takes the literal 0 -- the resource handle was clobbered: {m.group(0)}')

    def test_the_intro_acquires_two_named_resources(self):
        body = re.search(r'function WaspIntro\(quest\).*?\nend\n', self.src, re.S)
        self.assertIsNotNone(body, 'WaspIntro not found in the readable package')
        acquires = re.findall(r'resources:TryAcquire\((\w+),', body.group(0))
        self.assertEqual(len(set(acquires)), 2,
                         f'retail acquires the hero and WaspVictim on two resources; got {acquires}')


if __name__ == '__main__':
    unittest.main()
