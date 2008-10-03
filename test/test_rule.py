import unittest
from dynrules import Rule

class RuleTest (unittest.TestCase):
    def test_create (self):
        rule = Rule (1)
        self.assert_ (rule.id == 1)
        self.assert_ (rule.weight == 0)
        self.assert_ (rule.code == None)

    def test_weight (self):
        rule = Rule (1)
        self.assert_ (rule.weight == 0)
        rule.weight = 100
        self.assert_ (rule.weight == 100.)

        def setw ():
            rule.weight = -100
        self.assertRaises (ValueError, setw)

    def test_used (self):
        rule = Rule (1)
        rule.used = 1
        self.assert_ (rule.used == True)
        rule.used = 0
        self.assert_ (rule.used == False)
        rule.used = not True
        self.assert_ (rule.used == False)
        rule.used = not False
        self.assert_ (rule.used == True)

    def test_code (self):
        rule = Rule (1)
        self.assert_ (rule.code == None)
        rule.code = "Arbitrary string"
        self.assert_ (rule.code == "Arbitrary string")
        rule.code = buffer("Arbitrary string")
        self.assert_ (isinstance (rule.code, buffer))

if __name__ == "__main__":
    unittest.main ()
