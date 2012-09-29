import unittest
from dynrules import Rule, RuleSet, LearnSystem


class LearnSystemTest(unittest.TestCase):
    def test_create(self):
        lsystem = LearnSystem(RuleSet(10, 20))
        self.assertTrue(isinstance(lsystem.ruleset, RuleSet))
        self.assertEqual(lsystem.maxtries, 100)
        self.assertEqual(lsystem.maxscriptsize, 1024)

    def test_maxtries(self):
        lsystem = LearnSystem(RuleSet(10, 20))
        self.assertEqual(lsystem.maxtries, 100)

        def setm(x):
            lsystem.maxtries = x

        self.assertRaises(ValueError, setm, 0)
        self.assertRaises(ValueError, setm, -100)
        lsystem.maxtries = 10
        self.assertEqual(lsystem.maxtries, 10)

    def test_maxscriptsize(self):
        lsystem = LearnSystem(RuleSet(10, 20))
        self.assertEqual(lsystem.maxscriptsize, 1024)

        def setm(x):
            lsystem.maxscriptsize = x

        self.assertRaises(ValueError, setm, 0)
        self.assertRaises(ValueError, setm, -100)
        lsystem.maxscriptsize = 10
        self.assertEqual(lsystem.maxscriptsize, 10)


if __name__ == "__main__":
    unittest.main()
