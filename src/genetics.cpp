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
Gene::Gene(char name, GeneType type) : name_((char) toupper(name)), type_(type) {}

const char Gene::get_name() const
{
    return type_ ? (char)tolower(name_) : (char)toupper(name_);
}

const GeneType& Gene::get_type() const {return type_;}


/*
    Chromosome
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

/*
    Trait
*/
Trait::Trait(std::string name, std::string genes) : name_(name), gene_codes_(genes){}
const std::string Trait::get_name() const { return name_;}
const std::string& Trait::get_genes() const {return gene_codes_;}

ContinuousTrait::ContinuousTrait(std::string name, std::string genes, float min, float max) : Trait(name, genes), min_(min), max_(max)
{
    if (gene_codes_.length() < 2) {throw InvalidTraitParameterError();}
}

float ContinuousTrait::CalculateValue(const Genome &genome)
{
    auto dom_rec_ratio = GetAlleleRatio(gene_codes_, genome);
    float value = min_ + (max_ - min_) * dom_rec_ratio.first / (dom_rec_ratio.first + dom_rec_ratio.second);
    std::cout << name_ << "=" << value << " (N_DOM=" << dom_rec_ratio.first << ", N_REC=" << dom_rec_ratio.second << ")" << std::endl;
    return value;
}

std::pair<float, float> ContinuousTrait::CalculateStatistics(const std::vector<std::shared_ptr<Creature>> creatures) {
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

DiscreteTrait::DiscreteTrait(std::string name, std::string genes) : Trait(name, genes)
{
    if (gene_codes_.length() != 1) {throw InvalidTraitParameterError();}
}

float DiscreteTrait::CalculateValue(const Genome &genome)
{
    std::string gene_code = gene_codes_.substr(0, 1);
    auto dom_rec_ratio = GetAlleleRatio(gene_code, genome);
    float outcome = dom_rec_ratio.first > 0 ? 1.0 : 0.0;
    std::cout << name_ << "=" << outcome << " (N_DOM=" << dom_rec_ratio.first << ", N_REC=" << dom_rec_ratio.second << ")" << std::endl;
    return outcome;
}


/*
 * Return the number of dominant and
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
        if (n_dominant + n_recessive != 2*gene_codes.length()) {throw UnrepresentedTraitError();}
    }

    return std::make_pair(n_dominant, n_recessive); // TODO update this to be more representitive of bell-curve-like probability dist.
}

