import unittest

from tools.script_recovery.quest_unit_evidence import thread_names


class ThreadNamesTests(unittest.TestCase):
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
