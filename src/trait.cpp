#include <ostream>
#include "trait.h"


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
std::pair<float, float> Trait::CalculateStatistics(const std::vector<std::shared_ptr<Creature>> creatures) {
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
    if (gene_codes_.length() < 2) {throw InvalidTraitParameterError();}
}

/**
 * @brief Calculate the value of the continuous trait based on the given genome.
 * @param genome
 * @return
 */
float ContinuousTrait::CalculateValue(const Genome &genome)
{
    auto dom_rec_ratio = GetAlleleRatio(gene_codes_, genome);
    float value = min_ + (max_ - min_) * dom_rec_ratio.first / (dom_rec_ratio.first + dom_rec_ratio.second);
//    std::cout << name_ << "=" << value << " (N_DOM=" << dom_rec_ratio.first << ", N_REC=" << dom_rec_ratio.second << ")" << std::endl;
    return value;
}
/**
 * \brief ContinuousTraits don't have phenotypes, per say, the dimensionality of their trait is infinite.
 */
int ContinuousTrait::ValueToPhenotypeIndex(float value)
{
  return 0;
}

/*
    DiscreteTrait
*/

DiscreteTrait::DiscreteTrait(std::string name, std::string genes, int n_phenotypes) : Trait(name, genes, n_phenotypes)
{
    if (n_phenotypes < 2)
    {
        throw InvalidTraitParameterError();
    } else if (n_phenotypes == 2 && genes.size() != 1) {
        // If the trait is binary, it can only have 1 gene determine it's value.
        throw InvalidTraitParameterError();
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

int DiscreteTrait::ValueToPhenotypeIndex(float value)
{
  // round to the nearest integer
  return (value+0.5);
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
