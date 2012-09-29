import sys
import unittest
from dynrules import Rule, RuleSet


class RuleSetTest(unittest.TestCase):
    def test_create(self):
        ruleset = RuleSet(0, 20)
        self.assertTrue(ruleset.minweight == 0)
        self.assertTrue(ruleset.maxweight == 20)
        self.assertTrue(ruleset.weight == 0)
        self.assertTrue(ruleset.rules == [])

        v = None
        self.assertRaises(TypeError, RuleSet, 0, v)
        self.assertRaises(TypeError, RuleSet, None, 10)
        self.assertRaises(ValueError, RuleSet, "hello", 10)
        self.assertRaises(ValueError, RuleSet, 10, "hello")
        self.assertRaises(ValueError, RuleSet, 10, 9)
        self.assertRaises(ValueError, RuleSet, -33, -34)

    def test_minweight(self):
        ruleset = RuleSet(0, 20)
        self.assertEqual(ruleset.minweight, 0)
        ruleset.minweight = 20
        self.assertEqual(ruleset.minweight, 20)

        def setm(x):
            ruleset.minweight = x
        self.assertRaises(ValueError, setm, -1)
        self.assertRaises(ValueError, setm, 50)
        self.assertRaises(ValueError, setm, "hello")
        self.assertRaises(TypeError, setm, None)

    def test_maxweight(self):
        ruleset = RuleSet(10, 20)
        self.assertEqual(ruleset.maxweight, 20)
        ruleset.maxweight = 10
        self.assertEqual(ruleset.maxweight, 10)

        def setm(x):
            ruleset.maxweight = x
        self.assertRaises(ValueError, setm, -1)
        self.assertRaises(ValueError, setm, 8)
        self.assertRaises(ValueError, setm, "hello")
        self.assertRaises(TypeError, setm, None)

    def test_rulesadd(self):
        ruleset = RuleSet(10, 20)

        self.assertRaises(TypeError, ruleset.add, None)
        self.assertRaises(TypeError, ruleset.add, 10)

        ruleset.add(Rule(1))
        ruleset.add(Rule(1))
        ruleset.add(Rule(1))
        self.assertEqual(len(ruleset.rules), 1)
        self.assertEqual(ruleset.weight, 10)

        ruleset.add(Rule(2))
        ruleset.add(Rule(3))
        self.assertEqual(len(ruleset.rules), 3)
        self.assertEqual(ruleset.weight, 30)

        ruleset.clear()
        self.assertEqual(len(ruleset.rules), 0)
        self.assertEqual(ruleset.weight, 0)

        self.assertRaises(TypeError, ruleset.add, None)
        self.assertRaises(TypeError, ruleset.add, "test")

    def test_rulesremove(self):
        ruleset = RuleSet(10, 20)

        self.assertRaises(TypeError, ruleset.add, None)
        self.assertRaises(TypeError, ruleset.add, 10)

        for i in range(10):
            ruleset.add(Rule(i))
        self.assertEqual(len(ruleset.rules), 10)
        self.assertEqual(ruleset.weight, 100)
        rules = ruleset.rules

        ruleset.remove(rules[3])
        self.assertEqual(len(ruleset.rules), 9)
        self.assertEqual(ruleset.weight, 90)

        self.assertRaises(ValueError, ruleset.remove, Rule(7))
        self.assertEqual(len(ruleset.rules), 9)
        self.assertTrue(ruleset.weight, 90)

        self.assertRaises(ValueError, ruleset.remove, rules[3])
        ruleset.remove(rules[2])
        self.assertEqual(len(ruleset.rules), 8)
        self.assertEqual(ruleset.weight, 80)

        ruleset.clear()
        self.assertEqual(len(ruleset.rules), 0)
        self.assertEqual(ruleset.weight, 0)

    def test_updateweights(self):
        ruleset = RuleSet(10, 20)

        rule = None
        for i in range(10):
            rule = Rule(i)
            rule.weight = 15
            ruleset.add(rule)

        ruleset.rules[3].used = True
        ruleset.rules[7].used = True

        ruleset.calculate_adjustment = lambda x: 3
        ruleset.distribute_remainder = lambda x: None

        ruleset.update_weights(None)
        for x, rule in enumerate(ruleset.rules):
            if x == 3 or x == 7:
                self.assertTrue(rule.weight > 15)
            else:
                self.assertTrue(rule.weight < 15)


if __name__ == "__main__":
    unittest.main()
