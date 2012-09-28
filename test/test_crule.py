import sys, unittest
from dynrules import CRule

class CRuleTest (unittest.TestCase):
    def test_create (self):
        rule = CRule (1)
        self.assertTrue (rule.id == 1)
        self.assertTrue (rule.weight == 0)
        self.assertTrue (rule.code == None)

    def test_weight (self):
        rule = CRule (1)
        self.assertTrue (rule.weight == 0)
        rule.weight = 100
        self.assertTrue (rule.weight == 100.)

        def setw ():
            rule.weight = -100
        self.assertRaises (ValueError, setw)

    def test_used (self):
        rule = CRule (1)
        rule.used = 1
        self.assertTrue (rule.used)
        rule.used = 0
        self.assertFalse (rule.used)
        rule.used = not True
        self.assertFalse (rule.used)
        rule.used = not False
        self.assertTrue (rule.used)

    def test_code (self):
        rule = CRule (1)
        self.assertTrue (rule.code == None)
        rule.code = "Arbitrary string"
        self.assertTrue (rule.code == "Arbitrary string")
        if sys.version_info[0] < 3:
            rule.code = buffer("Arbitrary string")
            self.assertTrue (isinstance (rule.code, buffer))
        else:
            rule.code = bytes("Arbitrary string", "ascii")
            self.assertTrue (isinstance (rule.code, bytes))

if __name__ == "__main__":
    unittest.main ()
