import unittest
from dynrules import Rule, RuleSet

class RuleSetTest (unittest.TestCase):
    def test_create (self):
        ruleset = RuleSet (0, 20)
        self.assert_ (ruleset.minweight == 0)
        self.assert_ (ruleset.maxweight == 20)
        self.assert_ (ruleset.weight == 0)
        self.assert_ (ruleset.rules == [])

    def test_minweight (self):
        ruleset = RuleSet (0, 20)
        self.assert_ (ruleset.minweight == 0)
        ruleset.minweight = 20
        self.assert_ (ruleset.minweight == 20)

        def setm (x):
            ruleset.minweight = x
        self.assertRaises (ValueError, setm, -1)
        self.assertRaises (ValueError, setm, 50)

    def test_maxweight (self):
        ruleset = RuleSet (10, 20)
        self.assert_ (ruleset.maxweight == 20)
        ruleset.maxweight = 10
        self.assert_ (ruleset.maxweight == 10)

        def setm (x):
            ruleset.maxweight = x
        self.assertRaises (ValueError, setm, -1)
        self.assertRaises (ValueError, setm, 8)

    def test_rules (self):
        ruleset = RuleSet (10, 20)
        
        self.assertRaises (TypeError, ruleset.add, None)
        self.assertRaises (TypeError, ruleset.add, 10)

        ruleset.add (Rule (1))
        ruleset.add (Rule (1))
        ruleset.add (Rule (1))
        self.assert_ (len (ruleset.rules) == 1)
        self.assert_ (ruleset.weight == 10)

        ruleset.add (Rule (2))
        ruleset.add (Rule (3))
        self.assert_ (len (ruleset.rules) == 3)
        self.assert_ (ruleset.weight == 30)

        ruleset.clear ()
        self.assert_ (len (ruleset.rules) == 0)
        self.assert_ (ruleset.weight == 0)

if __name__ == "__main__":
    unittest.main ()
