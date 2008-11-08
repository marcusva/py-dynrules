/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */
 
#ifndef _RULESET_H_
#define _RULESET_H_

#include <vector>
#include "Rule.h"

class RuleSet
{
public:
    RuleSet ();
    RuleSet (double minweight, double maxweight);
    virtual ~RuleSet ();

    double getMinWeight () const;
    void setMinWeight (double minweight);
    double getMaxWeight () const;
    void setMaxWeight (double maxweight);
    double getWeight() const;

    std::vector<Rule> getRules();
    void addRule (Rule& rule);
    bool removeRule (Rule& rule);
    void clear ();

    virtual void updateWeights (double fraction, void *fitness);
    virtual double calculateAdjustment (void *fitness);
    virtual void distributeRemainder (double remainder);

protected:
    double _minweight;
    double _maxweight;
    double _weight;
    std::vector<Rule> _rules;
};

#endif /* _RULESET_H_ */
