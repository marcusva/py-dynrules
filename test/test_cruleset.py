import unittest
from dynrules import CRule, CRuleSet

class CRuleSetTest (unittest.TestCase):
    def test_create (self):
        ruleset = CRuleSet (0, 20)
        self.assert_ (ruleset.minweight == 0)
        self.assert_ (ruleset.maxweight == 20)
        self.assert_ (ruleset.weight == 0)
        self.assert_ (ruleset.rules == [])
        
        v = None
        self.assertRaises (TypeError, CRuleSet, 0, v)
        self.assertRaises (TypeError, CRuleSet, None, 10)
        self.assertRaises (TypeError, CRuleSet, "hello", 10)
        self.assertRaises (TypeError, CRuleSet, 10, "hello")
        self.assertRaises (ValueError, CRuleSet, 10, 9)
        self.assertRaises (ValueError, CRuleSet, -33, -34)

    def test_minweight (self):
        ruleset = CRuleSet (0, 20)
        self.assert_ (ruleset.minweight == 0)
        ruleset.minweight = 20
        self.assert_ (ruleset.minweight == 20)

        def setm (x):
            ruleset.minweight = x
        self.assertRaises (ValueError, setm, -1)
        self.assertRaises (ValueError, setm, 50)
        self.assertRaises (TypeError, setm, "hello")
        self.assertRaises (TypeError, setm, None)

    def test_maxweight (self):
        ruleset = CRuleSet (10, 20)
        self.assert_ (ruleset.maxweight == 20)
        ruleset.maxweight = 10
        self.assert_ (ruleset.maxweight == 10)

        def setm (x):
            ruleset.maxweight = x
        self.assertRaises (ValueError, setm, -1)
        self.assertRaises (ValueError, setm, 8)
        self.assertRaises (TypeError, setm, "hello")
        self.assertRaises (TypeError, setm, None)

    def test_rulesadd (self):
        ruleset = CRuleSet (10, 20)
        
        self.assertRaises (TypeError, ruleset.add, None)
        self.assertRaises (TypeError, ruleset.add, 10)

        ruleset.add (CRule (1))
        ruleset.add (CRule (1))
        ruleset.add (CRule (1))
        self.assert_ (len (ruleset.rules) == 1)
        self.assert_ (ruleset.weight == 10)

        ruleset.add (CRule (2))
        ruleset.add (CRule (3))
        self.assert_ (len (ruleset.rules) == 3)
        self.assert_ (ruleset.weight == 30)

        ruleset.clear ()
        self.assert_ (len (ruleset.rules) == 0)
        self.assert_ (ruleset.weight == 0)

    def test_rulesremove (self):
        ruleset = CRuleSet (10, 20)
        
        self.assertRaises (TypeError, ruleset.add, None)
        self.assertRaises (TypeError, ruleset.add, 10)

        for i in range (10):
            ruleset.add (CRule (i))
        self.assert_ (len (ruleset.rules) == 10)
        self.assert_ (ruleset.weight == 100)
        rules = ruleset.rules

        ruleset.remove (rules[3])
        self.assert_ (len (ruleset.rules) == 9)
        self.assert_ (ruleset.weight == 90)

        self.assertRaises (ValueError, ruleset.remove, CRule (7))
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
        ruleset = CRuleSet (10, 20)

        rule = None
        for i in range (10):
            rule = CRule (i)
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
