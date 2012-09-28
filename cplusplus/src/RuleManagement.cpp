/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#include "RuleManagement.h"

namespace dynrules
{

RuleManagement::RuleManagement (unsigned int maxrules) :
    _maxrules (maxrules)
{
}

RuleManagement::~RuleManagement ()
{
}

unsigned int RuleManagement::getMaxRules () const
{
    return this->_maxrules;
}

bool RuleManagement::saveRulesHintFile (std::string filename,
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
