#include "attribute.h"


/**
 * @brief An attribute is a value that the ecosystem uses to determine if the creature is fit enough to survive.
 * Attributes are contributed by one or more traits by weighting the phenotypes of the traits.
 * @param name
 * @param traits
 * @param weights
 */
Attribute::Attribute(std::string name, std::vector<std::shared_ptr<Trait>> traits, std::vector<std::weak_ptr<TraitWeighting>> weights) : name_(name), traits_(traits), weights_(weights) {}

/**
 * @brief Calculates the value of the attribute for the given creatue.
 * @param creature
 * @return
 */
float Attribute::CalculateValue(const Creature &creature)
{
    float value = 0;
    int ii = 0;
    for (auto trait: traits_)
    {
        value += weights_[ii].lock()->CalculateValue(*trait, creature.get_genome());
        ii ++;
    }
    return value;
}

const std::string Attribute::get_name() const {return name_;}
