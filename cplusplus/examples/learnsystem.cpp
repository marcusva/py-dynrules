#include <iostream>
#include <sstream>
#include <ctime>
#include "dynrules.h"
#include "WarriorRuleSet.h"

using namespace dynrules;

WarriorRuleSet*
_create_warrior_rules ()
{
    WarriorRuleSet *ruleset = new WarriorRuleSet (0, 20);
    Rule *rule;

    rule = new Rule (1);
    rule->setWeight (10);
    rule->setCode ("if warrior.strength >= enemy.strength: warrior.fight (enemy)\n");
    ruleset->addRule (rule);
    
    rule = new Rule (2);
    rule->setWeight (10);
    rule->setCode ("if warrior.strength < enemy.strength: warrior.do_walk (~direction)\n");
    ruleset->addRule (rule);
    
    rule = new Rule (3);
    rule->setWeight (5);
    rule->setCode ("if warrior.strength >= enemy.strength: warrior.do_walk (~direction)\n");
    ruleset->addRule (rule);
    
    rule = new Rule (4);
    rule->setWeight (5);
    rule->setCode ("if warrior.strength < enemy.strength: warrior.fight (enemy)\n");
    ruleset->addRule (rule);
    
    return ruleset;
}

int main (int argc, char* argv[])
{
    try
    {
        // Create some randomized scripts.
        time_t last, now;
        last = now = time (0);
        WarriorRuleSet *ruleset = _create_warrior_rules ();
        LearnSystem warriorlearnsystem = LearnSystem (ruleset);
        warriorlearnsystem.createScript (std::cout, 8);
        while (last == now)
            now = time (0);
        last = now;
        warriorlearnsystem.createScript (std::cout, 8);
        while (last == now)
            now = time (0);
        last = now;
        warriorlearnsystem.createScript (std::cout, 8);
        while (last == now)
            now = time (0);
        last = now;
        warriorlearnsystem.createScript (std::cout, 8);


    }
    catch (std::exception& e)
    {
        std::cout << "an error occured:" << e.what () << std::endl;
        return 1;
    }
    return 0;
}
