/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#include "RuleManagement.h"

RuleManagement::RuleManagement (unsigned int maxrules) :
    _maxrules (maxrules)
{
}

bool RuleManagement::saveRulesHintFile (std::string prefix,
    std::string suffix, LearnSystem *lsystem)
{
    std::stringstream filename;
    std::ofstream fd;

    filename << prefix << RULEFILE_MIDDLE << suffix << RULEFILE_SUFFIX;
    fd.open (filename.str ().c_str (), std::ios_base::out);
    if (!fd)
        return false; // TODO: error escalation?

    lsystem->createScript (fd, this->_maxrules);
    fd.close ();
    return true;
}
