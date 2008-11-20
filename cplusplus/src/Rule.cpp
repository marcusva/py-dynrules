/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#include <iostream>
#include "Rule.h"

Rule::Rule () :
    _id(0),
    _weight(0.f),
    _used(false),
    _code("")
{
}

Rule::Rule (int id) :
    _id(id),
    _weight(0.f),
    _used(false),
    _code("")
{
}

Rule::Rule (int id, std::string code) :
    _id(id),
    _weight(0.f),
    _used(false),
    _code(code)
{
}

Rule::Rule (int id, double weight) :
    _id(id),
    _weight(weight),
    _used(false),
    _code("")
{
}

Rule::Rule (int id, std::string code, double weight) :
    _id(id),
    _weight(weight),
    _used(false),
    _code(code)
{
}

Rule::~Rule ()
{
}

double Rule::getWeight () const
{
    return this->_weight;
}

void Rule::setWeight (double weight)
{
    this->_weight = weight;
}
    
bool Rule::getUsed () const
{
    return this->_used;
}

void Rule::setUsed (bool used)
{
    this->_used = used;
}

int Rule::getId () const
{
    return _id;
}

void Rule::setId (int id)
{
    this->_id = id;
}

std::string Rule::getCode () const
{
    return this->_code;
}

void Rule::setCode (const std::string& code)
{
    this->_code = code;
}

bool Rule::operator ==(const Rule& rule)
{
    return _id == rule._id;
}

bool operator ==(const Rule& a, const Rule& b)
{
    return a.getId() == b.getId();
}
