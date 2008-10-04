import unittest
from dynrules import Rule, RuleSet, LearnSystem

class LearnSystemTest (unittest.TestCase):
    def test_create (self):
        lsystem = LearnSystem (RuleSet (10, 20))
        self.assert_ (isinstance (lsystem.ruleset, RuleSet))
        self.assert_ (lsystem.maxtries == 100)
        self.assert_ (lsystem.maxscriptsize == 1024)

    def test_maxtries (self):
        lsystem = LearnSystem (RuleSet (10, 20))
        self.assert_ (lsystem.maxtries == 100)

        def setm (x):
            lsystem.maxtries = x

        self.assertRaises (ValueError, setm, 0)
        self.assertRaises (ValueError, setm, -100)
        lsystem.maxtries = 10
        self.assert_ (lsystem.maxtries == 10)

    def test_maxscriptsize (self):
        lsystem = LearnSystem (RuleSet (10, 20))
        self.assert_ (lsystem.maxscriptsize == 1024)

        def setm (x):
            lsystem.maxscriptsize = x

        self.assertRaises (ValueError, setm, 0)
        self.assertRaises (ValueError, setm, -100)
        lsystem.maxscriptsize = 10
        self.assert_ (lsystem.maxscriptsize == 10)

if __name__ == "__main__":
    unittest.main ()
