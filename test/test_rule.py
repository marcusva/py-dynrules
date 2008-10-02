import unittest
from dynrules import Rule

class RuleTest (unittest.TestCase):
    def test_weight (self):
        rule = Rule (1)
        self.assert_ (rule.id == 1)
        self.assert_ (rule.weight == 0)
        self.assert_ (rule.code == None)


if __name__ == "__main__":
    unittest.main ()
