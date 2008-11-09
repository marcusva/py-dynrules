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

/**
 * A container class for managing Rule objects and their weights.
 */
class RuleSet
{
public:
    /**
     * \brief Creates a new RuleSet instance.
     */
    RuleSet ();

    /**
     * \brief Creates a new RuleSet instance.
     *
     * \param minweight The minimum weight for the individual rules.
     * \param maxweight The maximum weight for the individual rules.
     * \exception char* Thrown, if minweight is greater than the set
     * maxweight.
     */
    RuleSet (double minweight, double maxweight);

    /**
     * \brief Destroys the RuleSet.
     */
    virtual ~RuleSet ();

    /**
     * \brief Gets the minimum weight for the individual rules.
     *
     * \return The minimum weight.
     */
    double getMinWeight () const;
    /**
     * \brief Sets the minimum weight for the individual rules.
     *
     * \param minweight The minimum weight to use.
     * \exception char* Thrown, if minweight is greater than the set
     * maxweight.
     */
    void setMinWeight (double minweight);

    /**
     * \brief Gets the maximum weight for the individual rules.
     *
     * \return The maximum weight.
     */
    double getMaxWeight () const;

    /**
     * \brief Sets the maximum weight for the individual rules.
     *
     * \param maxweight The maximum weight to use.
     * \exception char* Thrown, if maxweight is smaller than the set
     * minweight.
     */
    void setMaxWeight (double maxweight);

    /**
     * \brief Gets the current weight of all Rule objects.
     *
     * \return The weight of all Rule objects hold by the RuleSet.
     */
    double getWeight() const;

    /**
     * \brief Gets the Rule objects hold by the RuleSet.
     *
     * \return A std::vector containing the Rule objects.
     */
    std::vector<Rule> getRules();

    /**
     * \brief Adds a Rule to the RuleSet.
     *
     * \param rule The Rule to add.
     */
    void addRule (Rule& rule);

    /**
     * \brief Removes a Rule from the RuleSet.
     *
     * Removes a Rule from the RuleSet. This will use the standard
     * equality check for the Rule (x == y), and does not check whether
     * the instances are identical.
     *
     * \param rule The Rule to remove.
     */
    bool removeRule (Rule& rule);

    /**
     * \brief Removes all Rule objects from the RuleSet.
     */
    void clear ();

    /**
     * \brief Updates the weights of all contained Rules objects.
     *
     * Updates the weights of all contained Rule objects based on Pieter
     * Spronck's dynamic scripting algorithm.
     *
     * \param fraction The fraction value to distribute to the contained
     * Rule objects.
     * \param fitness The measure of the overall fitness of the
     * performance or whatever is suitable in the concrete
     * RuleSet::calculateAdjustment() implementation
     */
    void updateWeights (double fraction, void *fitness);

    /**
     * \brief Calculates the reward or penalty for the active rules.
     *
     * Calculates the reward or penalty, each of the activated rules
     * recives. fitness hereby can be used as measure of the performance
     * or whatever is suitable in the implementation.
     *
     * \param fitness The measure of the overall fitness.
     */
    virtual double calculateAdjustment (void *fitness);

    /**
     * \brief Distributes the remainder of the weight differences.

     * Distributes the remainder of the weight differences between the
     * last weights and current weights.
     *
     * \param remainder The remainder to distribute.
     */
    virtual void distributeRemainder (double remainder);

protected:
    double _minweight;
    double _maxweight;
    double _weight;
    std::vector<Rule> _rules;
};

#endif /* _RULESET_H_ */
