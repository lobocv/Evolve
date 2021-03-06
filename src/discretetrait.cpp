#include <ostream>
#include "trait.h"
#include <memory>
#include <algorithm>

DiscreteTrait::DiscreteTrait(std::string name, std::string genes, std::vector<std::string> phenotypes) : Trait(name, genes, phenotypes)
{
}

/**
 * @brief Create the phenospace that the genes alleles will reside in.
 * Each eigenvector in the phenospace corresponds to a specific phenotype
 */
void DiscreteTrait::InitializePhenospace()
{
    int N_col = phenotypes_.size();
    phenotype_vectors_ = std::vector<Phenovector>(N_col, Phenovector(N_col, 0));
    for (unsigned int ii=0; ii < N_col; ii++)
    {
        phenotype_vectors_[ii][ii] = 1;
    }
    InitializeGenevectors();

}

/**
 * @brief Generate vectors in the phenospace for each gene in the trait.
 */
void DiscreteTrait::InitializeGenevectors()
{
    int N_col = phenotypes_.size();
    int N_row = gene_codes_.size();

    for (int ii=0; ii < N_row; ii++)
    {
        for (int upper=0; upper < 2; upper++)
        {
            float sq_sum=0;
            Phenovector gene_vec;
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
            char gene = upper ? toupper(gene_codes_[ii]) : tolower(gene_codes_[ii]);
            gene_phenovectors_[gene] = gene_vec;
        }
    }
}
