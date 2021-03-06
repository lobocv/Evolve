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
const std::string Trait::GetName() const { return name_;}
const std::string& Trait::GetGenes() const {return gene_codes_;}

/**
 * @brief Sum the all the genevectors from the genome to find the traitvector in phenospace.
 * @param genome
 * @return
 */
Phenovector Trait::CumulativePhenovector(const Genome &genome)
{
    Phenovector trait_vec(phenotype_vectors_.size());
    int genes_found = 0;

    for (auto chromo_pair: genome)
    {
        for (int chromo_idx=0; chromo_idx < 2; chromo_idx++)
        {
            const GeneSequence *gene_sequence = &(chromo_idx ? chromo_pair.first.GetGenes() : chromo_pair.second.GetGenes());

            // Look for the trait's genes in the genome and sum the gene vector associated with the gene.
            for (const auto gene_code: gene_codes_)
            {
                GeneSequence::const_iterator it = gene_sequence->find(gene_code);
                if ( it != gene_sequence->end())
                {
                    Phenovector gene_vec;
                    char allele_code = (it->second.GetType() == Dominant) ? toupper(gene_code) : tolower(gene_code);
                    gene_vec = gene_phenovectors_[allele_code];
                    std::transform(gene_vec.begin(), gene_vec.end(), trait_vec.begin(),
                                   trait_vec.begin(), std::plus<float>());
                    genes_found++;
                }
            }
        }
    }
    if (genes_found != 2*gene_codes_.size())
    {
        throw UnrepresentedTraitError("Not all genes that describe this trait are present.");
    }

    return trait_vec;
}

/**
 * @brief Return the dimension of the trait vector that has the largest projection
 * onto the eigen-vectors (phenotypes) of the phenospace described by this trait.
 * @param trait_vec
 * @return
 */
int Trait::PhenovectorMaxDimension(Phenovector trait_vec)
{
    // Since the eigen-vectors of the phenospace are designed to be the identity matrix,
    // The largest projections ends up becoming the dimension with the largest value.
    // Therefore find the largest element and the index it corresponds to (the dimension)
    auto max_val_func = [](float a, float b) {return (fabs(a) < fabs(b));};
    auto max_iter = std::max_element(trait_vec.begin(), trait_vec.end(), max_val_func);
    int phenotype_index = std::distance(trait_vec.begin(), max_iter);
    return phenotype_index;
}

/**
 * @brief Return the phenotype that this trait vector most closely resembles.
 * @param trait_vec
 * @return
 */
std::string Trait::ValueToPhenotype(Phenovector trait_vec)
{
    int phenotype_index = PhenovectorMaxDimension(trait_vec);
    return phenotypes_[phenotype_index];
}

std::string Trait::ValueToPhenotype(std::shared_ptr<Creature> c)
{
    auto trait_vec = CumulativePhenovector(c->GetGenome());
    return ValueToPhenotype(trait_vec);
}


float Trait::ApplyWeighting(PhenotypeWeights weights, Phenovector trait_vec)
{
    auto phenotype = ValueToPhenotype(trait_vec);
    auto norm_trait_vec = NormalizeTraitVector(trait_vec);
    int phenotype_index = PhenovectorMaxDimension(trait_vec);
    float max_projection = std::fabs(norm_trait_vec[phenotype_index]);
    return weights[phenotype] * max_projection;
}


/**
 * @brief Calculate statistics such as mean and standard deviation of the trait for the given creatures.
 * @param creatures
 * @return
 */
std::pair<std::vector<float>, std::vector<float>> Trait::CalculateStatistics(const std::vector<std::shared_ptr<Creature>> creatures)
{
    std::vector<float> sum(phenotype_vectors_.size(), 0), mean(phenotype_vectors_.size(), 0), mean_squared(phenotype_vectors_.size(),0), stdev(phenotype_vectors_.size(), 0);
    Phenovector *values = new std::vector<float>[creatures.size()];
    int ii=0;
    for (auto &c : creatures)
    {
        values[ii] = CumulativePhenovector(c->GetGenome());
        for (int jj=0; jj< phenotype_vectors_.size(); jj++)
        {
            sum[jj] += values[ii][jj];
        }

        ii++;
    }
    for (int jj=0; jj < phenotype_vectors_.size(); jj++)
    {
        mean[jj] = sum[jj] / creatures.size();
    }

    for (ii=0; ii < creatures.size(); ii++)
    {
        for (int jj=0; jj < phenotype_vectors_.size(); jj++)
        {
            mean_squared[jj] += std::pow(values[ii][jj] - mean[jj], 2);
        }
    }
    delete[] values;
    for (int jj=0; jj < phenotype_vectors_.size(); jj++)
    {
        stdev[jj] = std::sqrt(mean_squared[jj] / creatures.size());
    }
    return std::make_pair(mean, stdev);

}

/**
 * @brief Calculate the frequency of all the phenotypes within the given creature population
 * @param creatures
 * @return
 */
std::map<std::string, int> Trait::CalculatePhenotypeStatistics(const std::vector<std::shared_ptr<Creature>> creatures)
{
    std::map<std::string, int> counter;
    for (auto phenotype: phenotypes_)
    {
        counter[phenotype] = 0;
    }
    for (auto c : creatures)
    {
        std::string phenotype = ValueToPhenotype(c);
        counter[phenotype]++;

    }
    return counter;
}


std::ostream &operator<<(std::ostream &stream, const Trait &obj)
{
    stream << obj.GetName() << " (";
    for (auto gene: obj.GetGenes())
    {
        stream << gene;
    }
    stream << ")";
    return stream;
}
