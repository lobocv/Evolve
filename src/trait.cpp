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
 * @brief Create the phenospace that the genes alleles will reside in.
 */
void Trait::InitializePhenospace()
{
    int N_col = phenotypes_.size();
    int N_row = gene_codes_.size();

    for (int ii=0; ii < N_row; ii++)
    {
        for (int upper=0; upper < 2; upper++)
        {
            float sq_sum=0;
            std::vector<float> gene_vec;
            for (int jj=0; jj < N_col; jj++)
            {
                int v = std::rand() % 101 - 51;
                sq_sum += v*v;
                gene_vec.push_back(v);

            }
            float norm = 1.0 / std::sqrt(sq_sum);
            for (auto &v : gene_vec)
            {
                 v *= norm;
            }
            char gene = upper ? std::toupper(gene_codes_[ii]) : std::tolower(gene_codes_[ii]);
            genevectors_[gene] = gene_vec;
        }
    }

    phenovectors_ = std::vector<std::vector<int> >(N_col, std::vector<int>(N_col, 0));
    for (unsigned int ii=0; ii < N_col; ii++)
    {
        phenovectors_[ii][ii] = 1;
    }

}

/**
 * @brief Sum the all the genevectors from the genome to find the traitvector in phenospace.
 * @param genome
 * @return
 */
std::vector<float> Trait::CalculateTraitVector(const Genome &genome)
{
    std::vector<float> trait_vec(phenotypes_.size());
    int genes_found = 0;

    for (auto chromo_pair: genome)
    {
        for (int chromo_idx=0; chromo_idx < 2; chromo_idx++)
        {
            const GeneSequence *gene_sequence = &(chromo_idx ? chromo_pair.first.get_genes() : chromo_pair.second.get_genes());

            // Look for the trait's genes in the genome and sum the gene vector associated with the gene.
            for (const auto gene_code: gene_codes_)
            {
                GeneSequence::const_iterator it = gene_sequence->find(gene_code);
                if ( it != gene_sequence->end())
                {
                    std::vector<float> gene_vec;
                    char allele_code = (it->second.get_type() == Dominant) ? toupper(gene_code) : tolower(gene_code);
                    gene_vec = genevectors_[allele_code];
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
 * @brief Calculate statistics such as mean and standard deviation of the trait for the given creatures.
 * @param creatures
 * @return
 */
std::pair<std::vector<float>, std::vector<float>> Trait::CalculateStatistics(const std::vector<std::shared_ptr<Creature>> creatures)
{
    std::vector<float> sum(phenovectors_.size(), 0), mean(phenovectors_.size(), 0), mean_squared(phenovectors_.size(),0), stdev(phenovectors_.size(), 0);
    std::vector<float> *values = new std::vector<float>[creatures.size()];
    int ii=0;
    for (auto &c : creatures)
    {
        values[ii] = CalculateValue(c->get_genome());
        for (int jj=0; jj< gene_codes_.size(); jj++)
        {
            sum[jj] += values[ii][jj];
        }

        ii++;
    }
    for (int jj=0; jj < gene_codes_.size(); jj++)
    {
        mean[jj] = sum[jj] / creatures.size();
    }

    for (ii=0; ii < creatures.size(); ii++)
    {
        for (int jj=0; jj < gene_codes_.size(); jj++)
        {
            mean_squared[jj] += std::pow(values[ii][jj] - mean[jj], 2);
        }
    }
    delete[] values;
    for (int jj=0; jj < gene_codes_.size(); jj++)
    {
        stdev[jj] = std::sqrt(mean_squared[jj] / creatures.size());
    }
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
