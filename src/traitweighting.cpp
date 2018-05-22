#include "trait.h"
#include "algorithm"

TraitWeighting::TraitWeighting(std::vector<float> weights) : weights_(weights) {}

//ContinuousTraitWeighting::ContinuousTraitWeighting(std::vector<float> weights) : TraitWeighting(weights) {
//    if (weights.size() > 1) {
//        throw InvalidAttributeParameterError("ContinuousTraitWeight can have only a single weighting. Got " + std::to_string(weights.size()) + " weights");
//    }
//}

//float ContinuousTraitWeighting::CalculateValue(Trait &trait, const Genome &genome)
//{
//    ContinuousTrait* cont_trait = static_cast<ContinuousTrait*>(&trait);
//    auto value = cont_trait->CalculateNormalizedValue(genome);
//    return weights_[0] * value;
//}

DiscreteTraitWeighting::DiscreteTraitWeighting(std::vector<float> weights): TraitWeighting(weights) {}

float DiscreteTraitWeighting::CalculateValue(Trait &trait, const Genome &genome)
{
    DiscreteTrait* cont_trait = static_cast<DiscreteTrait*>(&trait);
    auto trait_vec = cont_trait->CalculateTraitVector(genome);
    int phenotype_index = cont_trait->ValueToPhenotypeDimension(trait_vec);
    return weights_[(int) phenotype_index];
}
