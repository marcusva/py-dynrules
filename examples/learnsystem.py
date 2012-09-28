"""
A basic LearnSystem implementation demonstrating the capabilities of
dynrules.
"""
import sys
from dynrules import RuleSet, Rule, LearnSystem


class Learner(LearnSystem):
    def create_header(self):
        return "# ----\n# HEADER\n# ----\n"

    def create_footer(self):
        return "# ----\n# FOOTER\n# ----\n"

def create_rules():
    ruleset = RuleSet(0, 10)
    for i in range(20):
        rule = Rule(i)
        rule.weight = 1
        rule.code = "# CODE FOR RULE %d\n" % i
        ruleset.add(rule)
    return ruleset

def run():
    ruleset = create_rules()
    learnsystem = Learner(ruleset)
    learnsystem.create_script("testscript.py", 10)

if __name__ == "__main__":
    sys.exit(run())
