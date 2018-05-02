#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "trait.h"
#include "creature.h"

class Attribute
{
    std::string name_;
    std::vector<std::shared_ptr<Trait>> traits_;
    std::vector<float> weights_;
public:
    Attribute(std::string name, std::vector<std::shared_ptr<Trait>> traits, std::vector<int>);
    float CalculateValue(const Creature &creature);
};

#endif // ATTRIBUTE_H
