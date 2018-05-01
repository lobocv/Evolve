#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "trait.h"

class Attribute
{
    std::string name_;
    std::vector<std::shared_ptr<Trait>> traits_;
public:
    Attribute(std::string name, std::vector<std::shared_ptr<Trait>> traits);
};

#endif // ATTRIBUTE_H
