#include "attribute.h"
#include <algorithm>


Attribute::Attribute(std::string name, std::vector<std::shared_ptr<Trait>> traits, std::vector<std::vector<float>> weights) : name_(name), traits_(traits) {
    float weight_sum = 0, max_weight=0;
    // Determine the maximum weight from all the phenotypes described by the traits.
    for (auto trait_weight: weights)
    {
        auto max_trait_weight = *std::max_element(trait_weight.begin(), trait_weight.end());
        max_weight = (max_trait_weight > max_weight) ? max_trait_weight : max_weight;
    }

    // Normalize all the weights.
    for (auto &trait_weight: weights)
    {
        for (int ii=0; ii < trait_weight.size(); ii++)
        {
            trait_weight[ii] /= max_weight;
        }
    }
    weights_ = weights;
    auto asd = 4;
}


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
