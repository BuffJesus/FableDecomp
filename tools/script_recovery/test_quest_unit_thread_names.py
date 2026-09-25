import unittest

from tools.script_recovery.quest_unit_evidence import build_unit, thread_names


class ThreadNamesTests(unittest.TestCase):
    def test_sibling_quests_keep_their_own_named_worker(self):
        inventory = {
            'quests': [{'script': s, 'entities': [], 'allocator': '0x1', 'vtable': '0x2'}
                       for s in ('Q_A', 'Q_B')],
            'threads': [
                {'body': '0x110', 'name': 'WatchForTermination', 'registrationFunction': '0x100'},
                {'body': '0x210', 'name': 'WatchForTermination', 'registrationFunction': '0x200'}],
        }
        tu = {a: {'calls': []} for a in ('0x100', '0x110', '0x200', '0x210')}
        for script, root, body in [('Q_A', '0x100', '0x110'), ('Q_B', '0x200', '0x210')]:
            result = build_unit(script, inventory, {'lifecycle': [{'role': 'Main', 'address': root}]},
                                tu, ('0x100', '0x300'), {}, None, {})
            self.assertEqual(result['quest']['functions']['WatchForTermination']['address'], body)
            self.assertEqual(set(result['quest']['functions']), {'Main', 'WatchForTermination'})
        inventory['threads'][1]['registrationFunction'] = '0x110'
        with self.assertRaisesRegex(ValueError, 'ambiguous native thread name'):
            build_unit('Q_A', inventory, {'lifecycle': [{'role': 'Main', 'address': '0x100'}]},
                       tu, ('0x100', '0x300'), {}, None, {})

    def test_unnamed_workers_cannot_overwrite_each_other(self):
        names = thread_names([
            {'body': '0x00E04F10', 'name': None},
            {'body': '0x00E0A820', 'name': None},
        ])
        self.assertEqual(len(set(names.values())), 2)
        self.assertEqual(names['0x00e04f10'], 'NativeThread_00e04f10')
        self.assertEqual(names['0x00e0a820'], 'NativeThread_00e0a820')

    def test_repeated_registration_of_same_worker_is_allowed(self):
        row = {'body': '0x00E04F10', 'name': 'WatchForPickpocketing'}
        self.assertEqual(thread_names([row, row]),
                         {'0x00e04f10': 'WatchForPickpocketing'})

    def test_different_bodies_with_same_name_are_rejected(self):
        with self.assertRaisesRegex(ValueError, 'ambiguous native thread name'):
            thread_names([{'body': '0x10', 'name': 'Worker'},
                          {'body': '0x20', 'name': 'Worker'}])

    def test_conflicting_names_for_one_body_are_rejected(self):
        with self.assertRaisesRegex(ValueError, 'conflicting native thread names'):
            thread_names([{'body': '0x10', 'name': 'Worker'},
                          {'body': '0x10', 'name': 'Other'}])


if __name__ == '__main__':
    unittest.main()
