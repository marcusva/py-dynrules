import unittest
from dynrules import CRule, CRuleSet, CLearnSystem

class CLearnSystemTest (unittest.TestCase):
    def test_create (self):
        lsystem = CLearnSystem (CRuleSet (10, 20))
        self.assertTrue (isinstance (lsystem.ruleset, CRuleSet))
        self.assertTrue (lsystem.maxtries == 100)
        self.assertTrue (lsystem.maxscriptsize == 1024)

    def test_maxtries (self):
        lsystem = CLearnSystem (CRuleSet (10, 20))
        self.assertTrue (lsystem.maxtries == 100)

        def setm (x):
            lsystem.maxtries = x

        self.assertRaises (ValueError, setm, 0)
        self.assertRaises (ValueError, setm, -100)
        lsystem.maxtries = 10
        self.assertTrue (lsystem.maxtries == 10)

    def test_maxscriptsize (self):
        lsystem = CLearnSystem (CRuleSet (10, 20))
        self.assertTrue (lsystem.maxscriptsize == 1024)

        def setm (x):
            lsystem.maxscriptsize = x

        self.assertRaises (ValueError, setm, 0)
        self.assertRaises (ValueError, setm, -100)
        lsystem.maxscriptsize = 10
        self.assertTrue (lsystem.maxscriptsize == 10)

if __name__ == "__main__":
    unittest.main ()
