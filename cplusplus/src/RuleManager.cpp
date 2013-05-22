/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#include "RuleManager.h"

namespace dynrules
{

RuleManager::RuleManager (unsigned int maxrules) :
    _maxrules (maxrules)
{
}

RuleManager::~RuleManager ()
{
}

unsigned int RuleManager::getMaxRules () const
{
    return this->_maxrules;
}

bool RuleManager::saveRulesHintFile (std::string filename,
    LearnSystem& lsystem) const
{
    std::ofstream fd;

    fd.open (filename.c_str (), std::ios_base::out);
    if (!fd)
        return false; // TODO: error escalation?

    lsystem.createScript (fd, this->_maxrules);
    fd.close ();
    return true;
}

} // namespace
