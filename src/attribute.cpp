#include "attribute.h"
#include <numeric>

/**
 * @brief An attribute is a value that the ecosystem uses to determine if the creature is fit enough to survive.
 * Attributes are contributed by one or more traits by weighting the phenotypes of the traits.
 * @param name
 * @param traits
 * @param weights
 */
Attribute::Attribute(std::string name, std::vector<std::shared_ptr<Trait>> traits, std::vector<Phenovector> weights) : name_(name), traits_(traits), weights_(weights) {}

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
        // For each trait, find the weight that corresponds to the phenotype
        // dimension in phenospace and multiply them. Sum contributions
        // from all traits in the attribute.
        auto trait_vec = trait->CumulativePhenovector(creature.GetGenome());
        auto &weight_vec = weights_[ii];
        int phenotype_index = trait->PhenovectorMaxDimension(trait_vec);
        value += weight_vec[phenotype_index] * trait_vec[phenotype_index];
        ii ++;
    }
    return value;
}

const std::string Attribute::GetName() const {return name_;}
