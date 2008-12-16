import sys, unittest
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

    def test_rulesadd (self):
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

    def test_rulesremove (self):
        ruleset = RuleSet (10, 20)
        
        self.assertRaises (TypeError, ruleset.add, None)
        self.assertRaises (TypeError, ruleset.add, 10)

        for i in range (10):
            ruleset.add (Rule (i))
        self.assert_ (len (ruleset.rules) == 10)
        self.assert_ (ruleset.weight == 100)
        rules = ruleset.rules

        ruleset.remove (rules[3])
        self.assert_ (len (ruleset.rules) == 9)
        self.assert_ (ruleset.weight == 90)

        self.assertRaises (ValueError, ruleset.remove, Rule (7))
        self.assert_ (len (ruleset.rules) == 9)
        self.assert_ (ruleset.weight == 90)

        self.assertRaises (ValueError, ruleset.remove, rules[3])
        ruleset.remove (rules[2])
        self.assert_ (len (ruleset.rules) == 8)
        self.assert_ (ruleset.weight == 80)

        ruleset.clear ()
        self.assert_ (len (ruleset.rules) == 0)
        self.assert_ (ruleset.weight == 0)

    def test_updateweights (self):
        ruleset = RuleSet (10, 20)

        rule = None
        for i in range (10):
            rule = Rule (i)
            rule.weight = 15
            ruleset.add (rule)

        ruleset.rules[3].used = True
        ruleset.rules[7].used = True

        ruleset.calculate_adjustment = lambda x: 3
        ruleset.distribute_remainder = lambda x: None

        ruleset.update_weights (None)
        for x, rule in enumerate (ruleset.rules):
            if x == 3 or x == 7:
                self.assert_ (rule.weight > 15)
            else:
                self.assert_ (rule.weight < 15)

if __name__ == "__main__":
    unittest.main ()
