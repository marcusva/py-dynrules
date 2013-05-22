/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#ifndef _MMAPRULEMANAGER_H_
#define _MMAPRULEMANAGER_H_

#include "RuleManager.h"

namespace dynrules
{
    /**
     * \brief A simple memory-mapped RuleManager implementation that does not
     * load its rules from an external data source.
     *
     * It is an extremely useful class for testing rules and basic algorithms, but
     * due to the in-memory management of all rules, it should not be used in a
     * productive environment, especially if large rule sets have to be managed.
     *
     * By default, the MMapRuleManager will reserve enough memory for the rules
     * to manage, when it is constructed. It will NOT fill the rules with useful
     * values though. It is up to caller to use loadRules() afterwards and fill the
     * returned Rule instances with the necessary data.
     */
    class MMapRuleManager : public RuleManager
    {
    public:

        /**
         * \brief Creates a new MMapRuleManager instance.
         *
         * \param maxrules The amount of Rule objects to create.
         */
        MMapRuleManager (unsigned int maxrules);

        /**
         * \brief Destroys the MMapRuleManager.
         *
         * Destroys the MMapRuleManager and frees the memory hold by
         * the attached Rule instances.
         */
        virtual ~MMapRuleManager();

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

        /**
         * \brief In-memory set of Rule objects to use.
         */
        std::vector<Rule*> _rules;
    };

} // namespace
#endif /* _MMAPRULEMANAGER_H_ */
