/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#ifndef _RULE_H_
#define _RULE_H_

#include <string>

/**
 * \brief A simple rule container.
 *
 * Rule is a simple class type that carries a weight indicator and
 * arbitrary code data for usage in the dynamic script generation
 * process.
 */
class Rule
{
public:
    /**
     * \brief Creates a new Rule instance.
     */
    Rule ();

    /**
     * \brief Creates a new Rule instance with a unique id.
     *
     * \param id The unique id to use.
     */
    Rule (int id);

    /**
     * \brief Creates a new Rule instance with a unique id and code.
     *
     * \param id The unique id to use.
     * \param code The code hold by the Rule.
     */
    Rule (int id, std::string code);

    /**
     * \brief Creates a new Rule instance with a unique id and a weight.
     *
     * \param id The unique id to use.
     * \param weight The weight of the Rule.
     */
    Rule (int id, double weight);

    /**
     * \brief Creates a new Rule instance.
     *
     * \param id The unique id to use.
     * \param code The code hold by the Rule.
     * \param weight The weight of the Rule.
     */
    Rule (int id, std::string code, double weight);

    /**
     * \brief Destroys the Rule.
     */
    virtual ~Rule ();

    /**
     * \brief Gets the weight of the Rule.
     *
     * \return The weight of the Rule.
     */
    double getWeight () const;

    /**
     * \brief Sets the weight of the Rule.
     *
     * \param weight The weight to set.
     */
    void setWeight (double weight);

    /**
     * \brief Gets whether the Rule was used or not.
     *
     * \return The usage state of the Rule.
     */
    bool getUsed () const;

    /**
     * \brief Sets whether the Rule was used or not.
     *
     * \param used The usage state to set.
     */
    void setUsed (bool used);

    /**
     * \brief Gets the (unique) id of the Rule.
     *
     * \return The id of the Rule.
     */
    int getId () const;

    /**
     * \brief Sets the (unique) id of the Rule.
     *
     * \param id The id to set.
     */
    void setId (int id);

    /**
     * \brief Gets the code hold by the Rule.
     *
     * \return The code hold by the Rule.
     */
    std::string getCode () const;

    /**
     * \brief Sets the code to hold by the Rule.
     *
     * \param code The code to hold.
     */
    void setCode (const std::string& code);

    /**
     * \brief Compares the rule with another Rule.
     *
     * Comapares this Rule with another Rule instance. Note that this
     * will only check, whether the id attributes of both rules are the
     * same!
     *
     * \return true, if the id of this Rule is equal to the id of the
     * passed Rule, false otherwise.
     */
    bool operator ==(const Rule& rule);

protected:
    int _id;
    double _weight;
    bool _used;
    std::string _code;
};

/**
 * \brief Compares two Rule instances.
 *
 * Comapares a Rule with another Rule instance. Note that this will only
 * check, whether the id attributes of both rules are the same!
 *
 * \return true, if the id of both Rules are equal, false otherwise.
 */
bool operator ==(const Rule& a, const Rule& b);

#endif /* _RULE_H_ */
