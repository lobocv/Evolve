#include <ostream>
#include "trait.h"
#include <memory>
#include <algorithm>
/*
    Trait
*/

/**
 * @brief Base-class of traits of creatures / species in the ecosytsem.
 * @param name
 * @param genes
 */
Trait::Trait(std::string name, std::string genes, std::vector<std::string> phenotypes) : name_(name), gene_codes_(genes), phenotypes_(phenotypes){}
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
    delete[] values;
    stdev = std::sqrt(mean_squared / creatures.size());
    return std::make_pair(mean, stdev);

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
