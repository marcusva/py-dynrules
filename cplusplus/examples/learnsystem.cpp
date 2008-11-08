#include <iostream>
#include <sstream>
#include <ctime>
#include "dynrules.h"

int main (void)
{
    Rule *rules = new Rule[5];
    std::stringstream code;
    LearnSystem lsystem = LearnSystem (0, 20);
    RuleSet *ruleset = lsystem.getRuleSet ();

    srand (time (0));
    for (int i = 0; i < 5; i++)
    {
        code.str("");
        code.clear ();
        code << "###\nCode for Rule " << i << "\n###\n";
        rules[i].setId (i);
        rules[i].setCode (code.str ());
        rules[i].setWeight (static_cast<double>(rand()) / RAND_MAX);
        ruleset->addRule (rules[i]);
    }

    lsystem.createScript (std::cout, 7);
    return 0;
}
