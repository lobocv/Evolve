#include <ostream>
#include "trait.h"
#include <memory>
#include <algorithm>

DiscreteTrait::DiscreteTrait(std::string name, std::string genes, std::vector<std::string> phenotypes) : Trait(name, genes, phenotypes)
{
    InitializePhenospace();
}

/**
 * @brief Create the phenospace that the genes alleles will reside in.
 */
void DiscreteTrait::InitializePhenospace()
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
 * @brief Calculate the value of the discrete trait based on the given genome.
 * @param genome
 * @return
 */
float DiscreteTrait::CalculateValue(const Genome &genome)
{
    const GeneSequence *gene_sequence;
    GeneSequence::const_iterator it;
    std::vector<float> gene_vec_sum(phenotypes_.size());
    int genes_found = 0;

    // Sum the all the gene's vectors in phenospace to find the average vector
    // Dominant alleles are added to the sum while recessive alleles are subtracted.
    for (auto chromo_pair: genome)
    {
        for (int chromo_idx=0; chromo_idx < 2; chromo_idx++)
        {
            gene_sequence = &(chromo_idx ? chromo_pair.first.get_genes() : chromo_pair.second.get_genes());
            for (const auto gene_code: gene_codes_)
            {
                it = gene_sequence->find(gene_code);
                if ( it != gene_sequence->end())
                {
                    if (it->second.get_type() == Dominant) {
                        std::vector<float> gene_vec = genevectors_[std::toupper(gene_code)];
                        std::transform(gene_vec_sum.begin(), gene_vec_sum.end(), gene_vec.begin(),
                                       gene_vec_sum.begin(), std::plus<float>());
                    } else {
                        std::vector<float> gene_vec = genevectors_[std::tolower(gene_code)];
                        std::transform(gene_vec_sum.begin(), gene_vec_sum.end(), gene_vec.begin(),
                                       gene_vec_sum.begin(), std::minus<float>());
                    }

                    genes_found++;
                }
            }
        }
    }
    if (genes_found == 2*gene_codes_.size())
    {
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
        } projector(gene_vec_sum);
        // Find the largest projection. This returns an iterator
        auto largest_projection = std::max_element(phenovectors_.begin(), phenovectors_.end(), projector);
        // Convert the iterator to an index
        int phenotype_index = std::distance(phenovectors_.begin(), largest_projection);
        return phenotype_index;

    } else {
        throw UnrepresentedTraitError("Not all genes that describe this trait are present.");
    }


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
