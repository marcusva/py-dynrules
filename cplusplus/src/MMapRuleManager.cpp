/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#include "MMapRuleManager.h"

namespace dynrules
{
MMapRuleManager::MMapRuleManager (unsigned int maxrules) :
    RuleManager (maxrules),
    _rules(0)
{
    unsigned int i;
    for (i = 0; i < maxrules; i++)
        this->_rules.push_back (new Rule (i));
}

MMapRuleManager::~MMapRuleManager ()
{
    std::vector<Rule*>::iterator iter;
    for (iter = this->_rules.begin (); iter != this->_rules.end (); iter++)
    {
        delete *iter;
    }
    this->_rules.clear ();
}

std::vector<Rule*> MMapRuleManager::loadRules ()
{
    return this->_rules;
}

std::vector<Rule*> MMapRuleManager::loadRules (unsigned int maxrules)
{
    return this->_rules;
}

bool MMapRuleManager::saveRules (std::vector<Rule*> rules)
{
    return true;
}

} // namespace
