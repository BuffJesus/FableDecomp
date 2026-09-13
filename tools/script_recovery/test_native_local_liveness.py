import unittest

from tools.script_recovery.native_local_liveness import dead_after_marker


class LocalLivenessTests(unittest.TestCase):
    def test_else_overwrite_does_not_hide_a_read_on_the_true_path(self):
        self.assertFalse(self.check('MARK();\nif (choose) {\n}\nelse {\npcVar1 = fresh;\n}\nuse(pcVar1);'))

    def check(self, source):
        return dead_after_marker(source.splitlines(), 'MARK();', ('pcVar1', 'pcVar2'))

    def test_previous_value_reads_and_later_overwrites_are_safe(self):
        self.assertTrue(self.check('use(pcVar1);\nMARK();\npcVar1 = fresh;\npcVar2 = other;\nuse(pcVar1,pcVar2);'))
        self.assertTrue(self.check('MARK();\ngoto LAB_ab;\nuse(pcVar1);\nLAB_ab:\npcVar1 = fresh;\nreturn;'))

    def test_conditional_overwrite_cannot_hide_a_live_alternate_path(self):
        self.assertFalse(self.check('MARK();\nif (choose) {\npcVar1 = fresh;\n}\nuse(pcVar1);'))
        self.assertFalse(self.check('MARK();\nif(choose) goto LAB_ab;\nuse(pcVar1);\nLAB_ab:\nreturn;'))
        self.assertTrue(self.check('MARK();\nif (choose) goto LAB_ab;\npcVar1 = fresh;\nuse(pcVar1);\nreturn;\nLAB_ab:\npcVar1 = other;\nuse(pcVar1);'))

    def test_back_edge_finds_a_read_before_the_marker(self):
        self.assertFalse(self.check('LAB_ab:\nuse(pcVar1);\nMARK();\ngoto LAB_ab;'))
        self.assertTrue(self.check('LAB_ab:\npcVar1 = fresh;\nuse(pcVar1);\nMARK();\ngoto LAB_ab;'))

    def test_loop_exit_and_break_paths_remain_observable(self):
        self.assertFalse(self.check('MARK();\nwhile (running) {\npcVar1 = fresh;\n}\nuse(pcVar1);'))
        self.assertFalse(self.check('MARK();\ndo {\nif(stop) break;\npcVar1 = fresh;\n} while (running);\nuse(pcVar1);'))
        self.assertTrue(self.check('MARK();\ndo {\npcVar1 = fresh;\n} while (running);\nuse(pcVar1);'))

    def test_unknown_control_flow_or_escaped_addresses_reject_proof(self):
        for source in ('escape(&pcVar1);\nMARK();', 'escape(&(pcVar1));\nMARK();',
                       'MARK();\ngoto LAB_missing;', 'MARK();\ncontinue;',
                       'MARK();\nswitch (x) {\n}', 'MARK();\n}',
                       'MARK();\npcVar1 = pcVar1 + 1;'):
            self.assertFalse(self.check(source), source)

    def test_returning_function_is_not_a_return_statement(self):
        self.assertFalse(self.check('MARK();\nreturning_function();\nuse(pcVar1);'))
        self.assertTrue(self.check('MARK();\nreturn;\nuse(pcVar1);'))


if __name__ == '__main__':
    unittest.main()
