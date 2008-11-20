/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#include <ctime>
#include "LearnSystem.h"

LearnSystem::LearnSystem () :
    _maxtries (100),
    _maxscriptsize(1024),
    _ruleset (new RuleSet(0,0))
{
}

LearnSystem::LearnSystem (double minweight, double maxweight) :
    _maxtries (100),
    _maxscriptsize(1024),
    _ruleset(new RuleSet (minweight, maxweight))
{
}

LearnSystem::LearnSystem (RuleSet* ruleset) :
    _maxtries(100),
    _maxscriptsize(1024),
    _ruleset(ruleset)
{
}

LearnSystem::LearnSystem (const LearnSystem& lsystem) :
    _maxtries(lsystem.getMaxTries ()),
    _maxscriptsize(lsystem.getMaxScriptSize ()),
    _ruleset(new RuleSet (*(lsystem.getRuleSet())))
{
}

LearnSystem::~LearnSystem ()
{
    delete this->_ruleset;
}

RuleSet* LearnSystem::getRuleSet () const
{
    return this->_ruleset;
}

void LearnSystem::setRuleSet (RuleSet* ruleset)
{
    this->_ruleset = ruleset;
}

unsigned int LearnSystem::getMaxTries () const
{
    return this->_maxtries;
}

void LearnSystem::setMaxTries (unsigned int maxtries)
{
    this->_maxtries = maxtries;
}

unsigned int LearnSystem::getMaxScriptSize () const
{
    return this->_maxscriptsize;
}

void LearnSystem::setMaxScriptSize (unsigned int maxscriptsize)
{
    this->_maxscriptsize = maxscriptsize;
}

std::string LearnSystem::createHeader () const
{
    std::string retval = "";
    return retval;
}

std::string LearnSystem::createFooter () const
{
    std::string retval = "";
    return retval;
}

std::string LearnSystem::createRules (unsigned int maxrules) const
{
    std::string buf, retval;
    std::vector<Rule*> rules;
    Rule *rule;
    unsigned int tries, i;
    int added = 0, j, selected;
    size_t len, count, written = 0;
    double wsum, fraction, weights = this->_ruleset->getWeight ();

    if (weights == 0 || maxrules == 0)
        return "";

    rules = this->_ruleset->getRules ();
    count = rules.size ();

    /* Initialise the random number generator */
    srand (time (0));

    /* TODO: we do not guarantee that j < count. Usually this should be
     * dealt by the fraction sum function, though a safety net using
     * j % count or so might be better.
     */
    for (i = 0; i < maxrules; i++)
    {
        if (written >= static_cast<size_t>(this->_maxscriptsize))
            break;

        tries = added = 0;
        while (tries < this->_maxtries && !added)
        {
            j = 0;
            selected = -1;
            wsum = 0.;
            fraction = ((static_cast<double>(rand())) / RAND_MAX) * weights;
            while (selected == -1)
            {
                rule = rules.at (j);
                wsum += rule->getWeight ();
                if (wsum > fraction)
                {
                    selected = j;
                    break;
                }
                j++;
            }

            rule = rules.at (selected);

            /* Write the rule code */
            buf = rule->getCode ();
            len = buf.size ();
            /* Buffer acquired, write the raw data. */
            if (written + len > static_cast<size_t>(this->_maxscriptsize))
                goto finish;
            retval.append (buf);
            written += len;
            added = 1;

            tries++;
            break;
        }
    }

finish:
    return retval;
}

void LearnSystem::createScript (std::ostream& stream, unsigned int maxrules)
{
    stream << this->createHeader () << std::endl;
    stream << this->createRules (maxrules) << std::endl;
    stream << this->createFooter () << std::endl;
    return;
}
