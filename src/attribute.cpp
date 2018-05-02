#include "attribute.h"



Attribute::Attribute(std::string name, std::vector<std::shared_ptr<Trait>> traits, std::vector<int> weights) : name_(name), traits_(traits) {
    float weight_sum = 0;
    for (auto w: weights)
    {
        weight_sum += w;
    }
    for (auto w: weights)
    {
        weights_.push_back(w / weight_sum);
    }
    weight_sum = weight_sum;
}


float Attribute::CalculateValue(const Creature &creature)
{
    float value = 0;
    int ii = 0;
    for (auto trait: traits_)
    {
        value += weights_[ii] * trait->CalculateValue(creature.get_genome());
        ii ++;
    }
    return value;
}
