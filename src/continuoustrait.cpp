#include <ostream>
#include "trait.h"
#include <memory>
#include <algorithm>

/*
    ContinuousTrait
*/
ContinuousTrait::ContinuousTrait(std::string name, std::string genes, std::vector<std::string> n_phenotypes, float min, float max) : Trait(name, genes, n_phenotypes), min_(min), max_(max)
{
    if (gene_codes_.length() < 2) {throw InvalidTraitParameterError("ContinuousTrait must be represented by a polygene (more than 2 genes).");}
}

/**
 * @brief Calculate the value of the continuous trait based on the given genome.
 * @param genome
 * @return
 */
float ContinuousTrait::CalculateValue(const Genome &genome)
{
    auto dom_rec_ratio = CalculateNormalizedValue(genome);
    float value = min_ + (max_ - min_) * dom_rec_ratio;
//    std::cout << name_ << "=" << value << " (N_DOM=" << dom_rec_ratio.first << ", N_REC=" << dom_rec_ratio.second << ")" << std::endl;
    return value;
}

/**
 * @brief Calculate the value of the ContinuousTrait as a normalized number (0-1.0).
 * @param genome
 * @return
 */
float ContinuousTrait::CalculateNormalizedValue(const Genome &genome)
{
    auto dom_rec_ratio = GetAlleleRatio(gene_codes_, genome);
    return float(dom_rec_ratio.first) / (dom_rec_ratio.first + dom_rec_ratio.second);
}

/**
 * @brief Determine the phenotype that this value corresponds to by dividing up the
 * total range of the trait's value by the number of phenotypes.
 * @param value
 * @return
 */
std::string ContinuousTrait::ValueToPhenotype(float value)
{
    float range = (max_ - min_);
    value -= min_;
    for (int ii=0; ii < phenotypes_.size()-1; ii++)
    {
        float limit = (range * (ii+1)) / phenotypes_.size();
        if (value < limit)
        {
            return phenotypes_[ii];
        }
    }
    // The value was higher than the limit, return the largest bin-value phenotype
    return phenotypes_[phenotypes_.size()-1];
}

std::weak_ptr<TraitWeighting> ContinuousTrait::MakeWeighting(std::vector<float> weights)
{
    if (weights.size() != 1)
    {
        throw InvalidAttributeParameterError("ContinuousTraitWeighting must have a single weight value. Got " +
                                             std::to_string(weights.size()) + " weights instead.");
    }
    auto ptr = std::shared_ptr<TraitWeighting>(new ContinuousTraitWeighting(weights));
    weights_.push_back(ptr);
    return ptr;
}
