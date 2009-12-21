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

namespace dynrules
{
    /**
     * \brief The LearnSystem class generates scripts from RuleSet objects.
     *
     * The LearnSystem class takes care of creating new scripts based on a
     * predefined RuleSet. It does not evaluate the scripts nor modifies
     * the rules written to them.
     *
     * The procedure of creating scripts is done using three phases:
     *
     *  - header creation
     *  - rule code creation
     *  - footer creation
     *
     *  The header and footer are freely choosable. You can simple override
     *  or reassign the create_header() and create_footer() methods to let
     *  them return your required code.
     */
    class LearnSystem
    {
    public:
        /**
         * \brief Creates a new LearnSystem instance.
         *
         * \exception bad_alloc Thrown, if the embedded RuleSet could not be
         * allocated.
         */
        LearnSystem ();

        /**
         * \brief Creates a new LearnSystem instance.
         *
         * \param minweight The minimum weight limit for the embedded RuleSet.
         * \param maxweight The maximum weight limit for the embedded RuleSet.
         * \exception bad_alloc Thrown, if the embedded RuleSet could not be
         * allocated.
         * \exception char* Thrown, if the embedded RuleSet could not be
         * allocated.
         */
        LearnSystem (double minweight, double maxweight);

        /**
         * \brief Creates a new LearnSystem instance using an existing RuleSet.
         *
         * \param ruleset The RuleSet to use.
         */
        LearnSystem (RuleSet* ruleset);

        /**
         * \brief Creates a new LearnSystem instance from a LearnSystem.
         *
         * Creates a new LearnSystem instance from a LearnSystem. The embedded
         * RuleSet will be copied, not shared.
         *
         * \param lsystem The LearnSystem to create the instance from.
         * \exception bad_alloc Thrown, if the embedded RuleSet could not be
         * allocated.
         */
        LearnSystem (const LearnSystem& lsystem);

        /**
         * \brief Destroys the LearnSystem.
         *
         * Destroys the LearnSystem and frees the memory hold by the
         * embedded RuleSet.
         */
        virtual ~LearnSystem ();

        /**
         * \brief Gets the RuleSet used by the LearnSystem.
         *
         * \return The RuleSet used by the LearnSystem.
         */
        RuleSet* getRuleSet () const;

        /**
         * \brief Sets the RuleSet to use by the LearnSystem.
         *
         * Sets the RuleSet to use by the LearnSystem _without_ freeing
         * the currently assigned one. This requires you to deallocate any
         * previously used RuleSet manually:
         *
         * \code
         *   RuleSet *tmp = lsystem.getRuleSet();
         *   lsystem.setRuleSet(new_ruleset_to_use);
         *   delete tmp;
         * \endcode
         *
         * \param ruleset The RuleSet to use.
         */
        void setRuleSet (RuleSet* ruleset);

        /**
         * \brief Gets the amount of tries to select and add rules to the
         * script to generate.
         *
         * \return The amount of tries.
         */
        unsigned int getMaxTries () const;

        /**
         * \brief Sets the amount of tries to select and add rules to the
         * script to generate.
         *
         * \param maxtries The amount of tries to set.
         */
        void setMaxTries (unsigned int maxtries);

        /**
         * \brief Gets the maximum script size to generate.
         *
         * \return The maximum script size to generate.
         */
        unsigned int getMaxScriptSize () const;

        /**
         * \brief Sets the maximum script size to generate.
         *
         * \param maxscriptsize The maximum script size.
         */
        void setMaxScriptSize (unsigned int maxscriptsize);

        /**
         * \brief Creates and returns the header information for the script to
         * generate.
         *
         * Creates the header information to precede the rules within the
         * script. The default implementation will return an empty string.
         *
         * \return The string containing the header information.
         */
        virtual std::string createHeader () const;

        /**
         * \brief Creates and returns the footer information for the script to
         * generate.
         *
         * Creates the footer information to follow the rules within the
         * script. The default implementation will return an empty string.
         *
         * \return The string containing the footer information.
         */
        virtual std::string createFooter () const;

        /**
         * \brief Creates and returns the code of maxrules rules
         *
         * Creates and returns the code of maxrules rules. The function uses
         * the dynamic scripting algorithm as described by Pieter Spronck et
         * al.
         *
         * \param maxrules The maximum amount of rule code to create.
         * \return The string containing the rule code.
         */
        virtual std::string createRules (unsigned int maxrules) const;

        /**
         * \brief Creates the complete script contents.
         *
         * Creates the complete script contents, including the header,
         * footer and rules information and passes them to the given stream.
         *
         * Basically, this method does:
         *
         * \code
         *   stream << createHeader ();
         *   stream << createRules (maxrules);
         *   stream << createFooter ();
         * \endcode
         *
         * \param stream The stream to pass the script code to.
         * \param maxrules The maximum amount of rule code to create.
         */
        void createScript (std::ostream &stream, unsigned int maxrules);

    protected:

        /**
         * \brief The maximum number of tries to create script content from rules.
         */
        unsigned int _maxtries;

        /**
         * \brief The maximum script size in bytes.
         */
        unsigned int _maxscriptsize;

        /**
         * \brief The RuleSet to take the rules from for generating the scripts.
         */
        RuleSet* _ruleset;
    };

} // namespace

#endif /* _LEARNSYSTEM_H_ */
