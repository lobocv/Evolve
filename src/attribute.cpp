#include "attribute.h"
#include <algorithm>


/**
 * @brief An attribute is a value that the ecosystem uses to determine if the creature is fit enough to survive.
 * Attributes are contributed by one or more traits by weighting the phenotypes of the traits.
 * @param name
 * @param traits
 * @param weights
 */
Attribute::Attribute(std::string name, std::vector<std::shared_ptr<Trait>> traits, std::vector<std::vector<float>> weights) : name_(name), traits_(traits)
{
    float weight_sum = 0, max_weight=0;
    // Determine the maximum weight from all the phenotypes described by the traits.
    for (auto trait_weight: weights)
    {
        auto max_trait_weight = *std::max_element(trait_weight.begin(), trait_weight.end());
        max_weight = (max_trait_weight > max_weight) ? max_trait_weight : max_weight;
    }

    // Normalize all the weights.
    auto normalizer = [max_weight](float w) {return w / max_weight;}; // lambda function that normalizes the weights.
    int ii = 0;
    for (auto &trait_weight: weights)
    {
        if (traits[ii]->n_phenotypes_ != trait_weight.size()) {throw InvalidAttributeParameterError("Number of weights does not match the number of phenotypes.");}
        std::transform(trait_weight.begin(), trait_weight.end(), trait_weight.begin(), normalizer);
        ii++;
    }
    weights_ = weights;
}

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
        int trait_value = trait->CalculateValue(creature.get_genome());
        int pheno_idx = trait->ValueToPhenotypeIndex(trait_value);
        value += weights_[ii][pheno_idx] * trait_value;
        ii ++;
    }
    return value;
}

const std::string Attribute::get_name() const {return name_;}
