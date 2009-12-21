/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#include "MMapRuleManagement.h"

namespace dynrules
{
MMapRuleManagement::MMapRuleManagement (unsigned int maxrules) :
    RuleManagement (maxrules),
    _rules(0)
{
    unsigned int i;
    for (i = 0; i < maxrules; i++)
        this->_rules.push_back (new Rule (i));
}

MMapRuleManagement::~MMapRuleManagement ()
{
    std::vector<Rule*>::iterator iter;
    for (iter = this->_rules.begin (); iter != this->_rules.end (); iter++)
    {
        delete *iter;
    }
    this->_rules.clear ();
}

std::vector<Rule*> MMapRuleManagement::loadRules ()
{
    return this->_rules;
}

std::vector<Rule*> MMapRuleManagement::loadRules (unsigned int maxrules)
{
    return this->_rules;
}

bool MMapRuleManagement::saveRules (std::vector<Rule*> rules)
{
    return true;
}

} // namespace
