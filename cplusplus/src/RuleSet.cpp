/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */
#include "RuleSet.h"

RuleSet::RuleSet () :
    _minweight(0),
    _maxweight(0),
    _weight(0),
    _rules(0)
{
}

RuleSet::RuleSet (double minweight, double maxweight) :
    _minweight(minweight),
    _maxweight(maxweight),
    _weight(0),
    _rules(0)
{
}

RuleSet::~RuleSet ()
{
}


double RuleSet::getMinWeight () const
{
    return this->_minweight;
}

void RuleSet::setMinWeight (double minweight)
{
    //FAssertMsg (minweight >= 0, "minweight must not be negative");
    //FAssertMsg (minweight <= this->_maxweight,
    //"minweight must not be greater than maxweight");
    this->_minweight = minweight;
}

double RuleSet::getMaxWeight () const
{
    return this->_maxweight;
}

void RuleSet::setMaxWeight (double maxweight)
{
    //FAssertMsg (maxweight >= 0, "maxweight must not be negative");
    //FAssertMsg (maxweight >= this->_minweight,
    //"maxweight must not be smaller than minweight");
    this->_maxweight = maxweight;
}

double RuleSet::getWeight () const
{
    return this->_weight;
}

std::vector<Rule> RuleSet::getRules()
{
    return this->_rules;
}

void RuleSet::addRule (Rule& rule)
{
    this->_rules.push_back (rule);
    if (rule.getWeight() > this->_maxweight)
        rule.setWeight (this->_maxweight);
    else if (rule.getWeight () < this->_minweight)
        rule.setWeight (this->_minweight);
    this->_weight += rule.getWeight ();
}

bool RuleSet::removeRule (Rule& rule)
{
    bool found = false;
    std::vector<Rule>::iterator iter;
    for (iter = this->_rules.begin (); iter != this->_rules.end (); iter++)
    {
        if ((*iter) == rule)
        {
            found = true;
            break;
        }
    }
    if (found)
    {
        this->_rules.erase (iter);
        this->_weight -= (*iter).getWeight ();
    }
    return found;
}

void RuleSet::clear ()
{
    this->_rules.clear();
    this->_weight = 0.f;
}

void RuleSet::updateWeights (double fraction, void *fitness)
{
    /*
     * Adapted from Pieter Spronck's algorithm as explained in
     * Spronck et al: 2005, 'Adaptive Game AI with Dynamic Scripting'
     */
    Rule rule;
    std::vector<Rule>::iterator it;
    size_t count, usedcount = 0, nonactive;
    double totweight = 0, adjustment, compensation, _remainder;

    count = this->_rules.size ();
    if (count == 0)
        return;

    for (it = this->_rules.begin (); it != this->_rules.end (); it++)
    {
        rule = *it;
        if (rule.getUsed ())
            usedcount++;
    }
    if (usedcount == 0 || usedcount == count)
        return;
    
    nonactive = count - usedcount;
    adjustment = this->calculateAdjustment (fitness);
    compensation = (static_cast<double>(-(static_cast<int>(usedcount)) *
            adjustment)) / nonactive;
    _remainder = 0;

    for (it = this->_rules.begin (); it != this->_rules.end (); it++)
    {
        rule = *it;

        rule.setWeight((rule.getUsed ()) ? adjustment : compensation);
        if (rule.getWeight () < this->_minweight)
        {
            _remainder += rule.getWeight () - this->_minweight;
            rule.setWeight (this->_minweight);
        }
        else if (rule.getWeight () > this->_maxweight)
        {
            _remainder += rule.getWeight () - this->_maxweight;
            rule.setWeight (this->_maxweight);
        }
        totweight += rule.getWeight ();
    }
    
    this->_weight = totweight;
    this->distributeRemainder (_remainder);

    totweight = 0;
    for (it = this->_rules.begin (); it != this->_rules.end (); it++)
    {
        rule = *it;
        totweight += rule.getWeight ();
     }
     this->_weight = totweight;
}

double RuleSet::calculateAdjustment (void *fitness)
{
    return 0.f;
}

void RuleSet::distributeRemainder (double remainder)
{
}