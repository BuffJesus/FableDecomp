import unittest
from tools.script_recovery.native_oakvale_quiescence import execute


class QuiescenceTests(unittest.TestCase):
    def test_native_termination_traversal_and_active_process_restoration(self):
        for threads in range(4):
            for entities in range(4):
                for resumes in range(4):
                    for entity_flag,thing_valid in ((False,False),(True,False),(True,True)):
                        with self.subTest(threads=threads,entities=entities,resumes=resumes,entity_flag=entity_flag,thing_valid=thing_valid):
                            execute(threads,entities,resumes,entity_flag,thing_valid)


if __name__=='__main__':unittest.main()
