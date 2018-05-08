#include "common.h"
#include "genetics.h"
#include "creature.h"
#include <ctype.h>
#include <iostream>
#include <cstdlib>
#include <cmath>

/*
    Gene
*/
/**
 * @brief The gene is the smallest building block of genetic material.
 * Genes are found in variant forms called Alleles. The combinations of particular alleles are used
 * to determine the value of certain traits.
 * @param name
 * @param type
 */
Gene::Gene(char name, GeneType type) : name_((char) toupper(name)), type_(type) {}

const char Gene::get_name() const
{
    return type_ ? (char)tolower(name_) : (char)toupper(name_);
}

const GeneType& Gene::get_type() const {return type_;}


/*
    Chromosome
*/
/**
 * @brief A chromosome is a sequence of alleles (genes).
 * They are used during the reproduction of two creatures by passing one chromosome from each parent
 * to the child.
 * @param genes
 */
Chromosome::Chromosome(GeneSequence genes) : genes_(genes) {}

// Create a pair of chromosomes for the given gene codes.
// This creates two chromosomes that have random alleles from each gene in the list provided. 
ChromosomePair Chromosome::MakeRandomPair(std::string genecodes)
{
    GeneSequence left_genome, right_genome;
    for (const auto char_code: genecodes)
    {
        GeneType left_allele = (FlipCoin()) ?  Dominant: Recessive;
        GeneType right_allele = (FlipCoin()) ? Dominant: Recessive;
        left_genome[char_code] = Gene(char_code, left_allele);
        right_genome[char_code] = Gene(char_code, right_allele);
    }
    Chromosome left_chromo(left_genome);
    Chromosome right_chromo(right_genome);

    return std::make_pair(left_chromo, right_chromo);
}

const int Chromosome::get_length() const {return genes_.size();}
const GeneSequence& Chromosome::get_genes() const {return genes_;}


/**
 * @brief Return the number of of each allele type.
 * @param gene_codes
 * @param genome
 * @return
 */
std::pair<int, int> GetAlleleRatio(const std::string &gene_codes, const Genome &genome)
{
    int n_dominant = 0;
    int n_recessive = 0;
    const GeneSequence *gene_sequence;
    GeneSequence::const_iterator it;
    for (auto chromo_pair: genome)
    {
        for (int chromo_idx=0; chromo_idx < 2; chromo_idx++)
        {
            gene_sequence = &(chromo_idx ? chromo_pair.first.get_genes() : chromo_pair.second.get_genes());
            for (const auto gene_code: gene_codes)
            {
                it = gene_sequence->find(gene_code);
                if ( it != gene_sequence->end())
                {
                    if (it->second.get_type() == Dominant)
                    {
                        n_dominant++;
                    } else {
                        n_recessive++;
                    }
                }
            }
        }
        if (n_dominant + n_recessive != 2*gene_codes.length()) {throw UnrepresentedTraitError("Not all genes were found in the genome.");}
    }

    return std::make_pair(n_dominant, n_recessive);
}


std::ostream &operator<<(std::ostream &stream, const Chromosome &obj)
{
    for (auto gene: obj.get_genes())
    {
        stream << gene.second;
    }
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const Gene &obj)
{
    stream << obj.get_name();
    return stream;
}
