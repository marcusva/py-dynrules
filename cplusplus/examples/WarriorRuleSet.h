#ifndef _WARRRIORRULESET_H_
#define _WARRRIORRULESET_H_

#include "dynrules.h"

using namespace dynrules;

class WarriorRuleSet : public RuleSet
{
public:
    WarriorRuleSet (double minweight, double maxweight) :
        RuleSet (minweight, maxweight)
    {
    };

    double calculateAdjustment (void *fitness)
    {
        if (fitness == 0)
            return 0.f;
        
        double value = *(static_cast<double*> (fitness));
        
        if (value > 3.f)
        {
            // Excellent work - the warrior killed anything.
            return value - 3;
        }
        
        if (value < 0.f)
        {
            // The warrior died.
            return value;
        }

        return 3.f - value;
    };

    void distributeRemainder (double remainder)
    {
        int count = this->_rules.size ();
        if (count == 0)
            return;

        double fraction = remainder / float (count);
        std::vector<Rule*>::iterator iter;
        for (iter = this->_rules.begin (); iter != this->_rules.end (); iter++)
        {
            (*iter)->setWeight ((*iter)->getWeight () + fraction);
        }
    };

protected:
};

#endif /* _WARRRIORRULESET_H_ */
