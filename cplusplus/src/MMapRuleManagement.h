/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#ifndef _MMAPRULEMANAGEMENT_H_
#define _MMAPRULEMANAGEMENT_H_

#include "RuleManagement.h"

/**
 * \brief A simple memory-mapped RuleManagement implementation that does not
 * load its rules from an external data source.
 *
 * It is an extremely useful class for testing rules and basic algorithms, but
 * due to the in-memory management of all rules, it should not be used in a
 * productive environment, especially if large rule sets have to be managed.
 *
 * By default, the MMapRuleManagement will reserve enough memory for the rules
 * to manage, when it is constructed. It will NOT fill the rules with useful
 * values though. It is up to caller to use loadRules() afterwards and fill the
 * returned Rule instances with the necessary data.
 */
class MMapRuleManagement : public RuleManagement
{
public:

    /**
     * \brief Creates a new MMapRuleManagement instance.
     *
     * \param maxrules The amount of Rule objects to create.
     */
    MMapRuleManagement (unsigned int maxrules);

    /**
     * \brief Destroys the MMapRuleManagement.
     *
     * Destroys the MMapRuleManagement and frees the memory hold by
     * the attached Rule instances.
     */
    virtual ~MMapRuleManagement();

    /**
     * \brief Loads all existing rules.
     *
     * \return A vector containing the Rule objects hold by this instance.
     * The caller should not free the returned results.
     */
    std::vector<Rule*> loadRules ();

    /**
     * \brief Loads a specific amount of rules.
     *
     * This behaves exactly like loadRules().
     *
     * \param maxrules The amount of rules to load.
     * \return A std::vector containing the loaded rules.
     */
    std::vector<Rule*> loadRules (unsigned int maxrules);

    /**
     * \brief Saves the passed rules to the underlying data source.
     *
     * This does nothing.
     *
     * \param rules A std::vector containing the rules to save.
     * \return true, if saving the rules was successful, false otherwise.
     */
    bool saveRules (std::vector<Rule*> rules);

protected:
    std::vector<Rule*> _rules;
};

#endif /* _MMAPRULEMANAGEMENT_H_ */
