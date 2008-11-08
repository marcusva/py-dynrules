/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */
#ifndef _LEARNSYSTEM_H_
#define _LEARNSYSTEM_H_

#include <iostream>
#include <string>
#include "RuleSet.h"

class LearnSystem
{
public:
    LearnSystem ();
    LearnSystem (int minweight, int maxweight);
    LearnSystem (RuleSet* ruleset);
    LearnSystem (const LearnSystem& lsystem);
    virtual ~LearnSystem ();

    RuleSet* getRuleSet () const;
    void setRuleSet (RuleSet* ruleset);

    unsigned int getMaxTries () const;
    void setMaxTries (unsigned int maxtries);

    unsigned int getMaxScriptSize () const;
    void setMaxScriptSize (unsigned int maxscriptsize);

    virtual std::string createHeader ();
    virtual std::string createFooter ();
    virtual std::string createRules (int maxrules);
    void createScript (std::ostream &stream, int maxrules);

protected:
    unsigned int _maxtries;
    unsigned int _maxscriptsize;
    RuleSet* _ruleset;
};

#endif /* _LEARNSYSTEM_H_ */
