#include <ostream>
#include "trait.h"
#include <memory>
#include <algorithm>

DiscreteTrait::DiscreteTrait(std::string name, std::string genes, std::vector<std::string> phenotypes) : Trait(name, genes, phenotypes)
{
    InitializePhenospace();
}

/**
 * @brief Calculate the value of the discrete trait based on the given genome.
 * @param genome
 * @return
 */
float DiscreteTrait::CalculateValue(const Genome &genome)
{
    auto trait_vec = CalculateTraitVector(genome);
    // Find the largest projection onto the eigenvectors of the phenospace to determine
    // the phenotype of the trait.

    /**
     * @brief The PhenospaceProjector struct is a comparator functor that returns True
     * if the vector it is projecting has a larger projection on the second argument vector,
     * when compared to the projection onto the first argument vector.
     */
    struct PhenospaceProjector
    {
        std::vector<float> vect_;
        PhenospaceProjector(std::vector<float> vect)  : vect_(vect){}
        bool operator() (std::vector<int> a, std::vector<int> b)
        {
            auto ainner = std::inner_product(a.begin(), a.end(), vect_.begin(), 0.0);
            auto binner = std::inner_product(b.begin(), b.end(), vect_.begin(), 0.0);
            return std::abs(ainner) < std::abs(binner);
        }
    } projector(trait_vec);
    // Find the largest projection. This returns an iterator
    auto largest_projection = std::max_element(phenovectors_.begin(), phenovectors_.end(), projector);
    // Convert the iterator to an index
    int phenotype_index = std::distance(phenovectors_.begin(), largest_projection);
    return phenotype_index;

}

std::string DiscreteTrait::ValueToPhenotype(float value){return phenotypes_[int(value)];}


std::weak_ptr<TraitWeighting> DiscreteTrait::MakeWeighting(std::vector<float> weights)
{
    if (weights.size() != phenotypes_.size()) {
        throw InvalidAttributeParameterError("DiscreteTraitWeighting '" + name_ + "' must have a " +
                                             std::to_string(phenotypes_.size()) + " weight values. " +
                                             "Got " + std::to_string(weights.size()) + " weight(s) instead.");
    }
    auto ptr = std::shared_ptr<TraitWeighting>(new DiscreteTraitWeighting(weights));
    weights_.push_back(ptr);
    return ptr;
}
