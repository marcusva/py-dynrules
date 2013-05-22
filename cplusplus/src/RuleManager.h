/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#ifndef _RULEMANAGER_H_
#define _RULEMANAGER_H_

#include <vector>
#include <sstream>
#include <fstream>

#include "Rule.h"
#include "LearnSystem.h"

namespace dynrules
{
    /**
     * \brief The RuleManager class takes care of loading and saving rules from
     *  arbitrary data sources.
     *
     * The base is an abstract class, which's loadRules() method must be
     * implemented according to the specific needs of the application.
     *
     */
    class RuleManager
    {
    public:

        /**
         * \brief Creates a new RuleManager instance.
         *
         * \param maxrules The default amount of rules to manage.
         */
        RuleManager (unsigned int maxrules);

        /**
         * \brief Destroys the RuleManager.
         *
         * Destroys the RuleManager and frees the memory hold by it.
         */
        virtual ~RuleManager ();

        /**
         * \brief Loads all existing rules from the underlying data source.
         *
         * \return A std::vector containing the loaded rules.
         */
        virtual std::vector<Rule*> loadRules () = 0;

        /**
         * \brief Loads a specific amount of rules from the underlying data source.
         *
         * \param maxrules The amount of rules to load.
         * \return A std::vector containing the loaded rules.
         */
        virtual std::vector<Rule*> loadRules (unsigned int maxrules) = 0;

        /**
         * \brief Saves the passed rules to the underlying data source.
         *
         * \param rules A std::vector containing the rules to save.
         * \return true, if saving the rules was successful, false otherwise.
         */
        virtual bool saveRules (std::vector<Rule*> rules) = 0;

        /**
         * \brief Saves a LearnSystem/RuleSet combination to a physical
         * file.
         *
         * \param filename The file name.
         * \param lsystem The LearnSystem to save the rules for.
         * \return true on success, false otherwise.
         */
        bool saveRulesHintFile (std::string filename, LearnSystem& lsystem) const;

        /**
         * \brief Gets the maximum number of rules, an instance will deal with.
         *
         * \return The maximum number of rules to deal with.
         */
        unsigned int getMaxRules () const;

    protected:

        /**
         * \brief The maximum amount of Rule objects the RuleManager
         * will manage.
         */
        unsigned int _maxrules;

    };

} // namespace

#endif /* _RULEMANAGER_H_ */
