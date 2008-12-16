import sys, unittest
from dynrules import CRule

class CRuleTest (unittest.TestCase):
    def test_create (self):
        rule = CRule (1)
        self.assert_ (rule.id == 1)
        self.assert_ (rule.weight == 0)
        self.assert_ (rule.code == None)

    def test_weight (self):
        rule = CRule (1)
        self.assert_ (rule.weight == 0)
        rule.weight = 100
        self.assert_ (rule.weight == 100.)

        def setw ():
            rule.weight = -100
        self.assertRaises (ValueError, setw)

    def test_used (self):
        rule = CRule (1)
        rule.used = 1
        self.assert_ (rule.used == True)
        rule.used = 0
        self.assert_ (rule.used == False)
        rule.used = not True
        self.assert_ (rule.used == False)
        rule.used = not False
        self.assert_ (rule.used == True)

    def test_code (self):
        rule = CRule (1)
        self.assert_ (rule.code == None)
        rule.code = "Arbitrary string"
        self.assert_ (rule.code == "Arbitrary string")
        if sys.version_info[0] < 3:
            rule.code = buffer("Arbitrary string")
            self.assert_ (isinstance (rule.code, buffer))
        else:
            rule.code = bytes("Arbitrary string", "ascii")
            self.assert_ (isinstance (rule.code, bytes))

if __name__ == "__main__":
    unittest.main ()
