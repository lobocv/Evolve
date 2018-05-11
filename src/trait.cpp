#include <ostream>
#include "trait.h"
#include <memory>

/*
    Trait
*/

/**
 * @brief Base-class of traits of creatures / species in the ecosytsem.
 * @param name
 * @param genes
 */
Trait::Trait(std::string name, std::string genes, int n_phenotypes) : name_(name), gene_codes_(genes), n_phenotypes_(n_phenotypes){}
const std::string Trait::get_name() const { return name_;}
const std::string& Trait::get_genes() const {return gene_codes_;}

/**
 * @brief Calculate statistics such as mean and standard deviation of the trait for the given creatures.
 * @param creatures
 * @return
 */
std::pair<float, float> Trait::CalculateStatistics(const std::vector<std::shared_ptr<Creature>> creatures)
{
    double sum = 0, mean=0, mean_squared=0, stdev=0;
    float* values = new float[creatures.size()];
    int ii=0;
    for (auto &c : creatures)
    {
        values[ii] = CalculateValue(c->get_genome());
        sum += values[ii];
        ii++;
    }
    mean = sum / creatures.size();
    for (ii=0; ii < creatures.size(); ii++)
    {
        mean_squared += std::pow(values[ii] - mean, 2);
    }
    stdev = std::sqrt(mean_squared / creatures.size());
    return std::make_pair(mean, stdev);

}


/*
    ContinuousTrait
*/
ContinuousTrait::ContinuousTrait(std::string name, std::string genes, int n_phenotypes, float min, float max) : Trait(name, genes, n_phenotypes), min_(min), max_(max)
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


/*
    DiscreteTrait
*/

DiscreteTrait::DiscreteTrait(std::string name, std::string genes, int n_phenotypes) : Trait(name, genes, n_phenotypes)
{
    if (n_phenotypes < 2)
    {
        throw InvalidTraitParameterError("Traits must have a minimum of 2 phenotypes. Got " + n_phenotypes);
    } else if (n_phenotypes == 2 && genes.size() != 1) {
        // If the trait is binary, it can only have 1 gene determine it's value.
        throw InvalidTraitParameterError("Discrete binary traits must be dependent on only one gene.");
    }
}

/**
 * @brief Calculate the value of the discrete trait based on the given genome.
 * @param genome
 * @return
 */
float DiscreteTrait::CalculateValue(const Genome &genome)
{
    std::string gene_code = gene_codes_.substr(0, 1);
    auto dom_rec_ratio = GetAlleleRatio(gene_code, genome);
    float outcome = dom_rec_ratio.first > 0 ? 1.0 : 0.0;
//    std::cout << name_ << "=" << outcome << " (N_DOM=" << dom_rec_ratio.first << ", N_REC=" << dom_rec_ratio.second << ")" << std::endl;
    return outcome;
}

std::weak_ptr<TraitWeighting> DiscreteTrait::MakeWeighting(std::vector<float> weights)
{
    if (weights.size() != n_phenotypes_) {
        throw InvalidAttributeParameterError("DiscreteTraitWeighting '" + name_ + "' must have a " +
                                             std::to_string(n_phenotypes_) + " weight values. " +
                                             "Got " + std::to_string(weights.size()) + " weight(s) instead.");
    }
    auto ptr = std::shared_ptr<TraitWeighting>(new DiscreteTraitWeighting(weights));
    weights_.push_back(ptr);
    return ptr;
}


std::ostream &operator<<(std::ostream &stream, const Trait &obj)
{
    stream << obj.get_name() << " (";
    for (auto gene: obj.get_genes())
    {
        stream << gene;
    }
    stream << ")";
    return stream;
}

TraitWeighting::TraitWeighting(std::vector<float> weights) : weights_(weights) {}

ContinuousTraitWeighting::ContinuousTraitWeighting(std::vector<float> weights) : TraitWeighting(weights) {
    if (weights.size() > 1) {
        throw InvalidAttributeParameterError("ContinuousTraitWeight can have only a single weighting. Got " + std::to_string(weights.size()) + " weights");
    }
}

float ContinuousTraitWeighting::CalculateValue(Trait &trait, const Genome &genome)
{
    ContinuousTrait* cont_trait = static_cast<ContinuousTrait*>(&trait);
    auto value = cont_trait->CalculateNormalizedValue(genome);
    return weights_[0] * value;
}

DiscreteTraitWeighting::DiscreteTraitWeighting(std::vector<float> weights): TraitWeighting(weights) {}

float DiscreteTraitWeighting::CalculateValue(Trait &trait, const Genome &genome)
{
    DiscreteTrait* cont_trait = static_cast<DiscreteTrait*>(&trait);
    auto phenotype_idx = cont_trait->CalculateValue(genome);
    return weights_[(int) phenotype_idx];
}
