import sys
import unittest
from dynrules import CRule


class CRuleTest(unittest.TestCase):
    def setUp(self):
        if sys.version_info[0:2] == (2, 6):
            self.assertIsNone = lambda x: self.assertEqual(x, None)

    def test_create(self):
        rule = CRule(1)
        self.assertEqual(rule.id, 1)
        self.assertEqual(rule.weight, 0)
        self.assertIsNone(rule.code)

    def test_weight(self):
        rule = CRule(1)
        self.assertEqual(rule.weight, 0)
        rule.weight = 100
        self.assertEqual(rule.weight, 100)

        def setw():
            rule.weight = -100
        self.assertRaises(ValueError, setw)

    def test_used(self):
        rule = CRule(1)
        rule.used = 1
        self.assertTrue(rule.used)
        rule.used = 0
        self.assertFalse(rule.used)
        rule.used = not True
        self.assertFalse(rule.used)
        rule.used = not False
        self.assertTrue(rule.used)

    def test_code(self):
        rule = CRule(1)
        self.assertIsNone(rule.code)
        rule.code = "Arbitrary string"
        self.assertEqual(rule.code, "Arbitrary string")
        if sys.version_info[0] < 3:
            rule.code = buffer("Arbitrary string")
            self.assertTrue(isinstance(rule.code, buffer))
        else:
            rule.code = bytes("Arbitrary string", "ascii")
            self.assertTrue(isinstance(rule.code, bytes))

if __name__ == "__main__":
    unittest.main()
