#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "trait.h"
#include "creature.h"

class Attribute
{
    std::string name_;
    std::vector<std::shared_ptr<Trait>> traits_;
    std::vector<std::vector<float>> weights_;
public:
    Attribute(std::string name, std::vector<std::shared_ptr<Trait>> traits, std::vector<std::vector<float>> weights);
    float CalculateValue(const Creature &creature);
    const std::string get_name() const;
};

#endif // ATTRIBUTE_H