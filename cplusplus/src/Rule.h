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

class Rule
{
public:
    Rule ();
    Rule (int id);
    Rule (int id, std::string code);
    Rule (int id, double weight);
    Rule (int id, std::string code, double weight);
    virtual ~Rule ();

    double getWeight () const;
    void setWeight (double weight);
    
    bool getUsed () const;
    void setUsed (bool used);

    int getId () const;
    void setId (int id);

    std::string getCode () const;
    void setCode (const std::string& code);

    bool operator ==(const Rule& rule);
 
protected:
    int _id;
    double _weight;
    bool _used;
    std::string _code;
};

bool operator ==(const Rule& a, const Rule& b);

#endif /* _RULE_H_ */
