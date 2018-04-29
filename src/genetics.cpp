#include "common.h"
#include "genetics.h"
#include "creature.h"
#include <ctype.h>
#include <iostream>
#include <cstdlib>

/*
    Gene
*/
Gene::Gene(char name, GeneType type) : name_((char) toupper(name)), type_(type) {};

const char& Gene::get_name() const {
    return type_ ? (char)tolower(name_) : (char)toupper(name_);
}

const GeneType& Gene::get_type() const {return type_;};

bool Gene::operator< (const Gene &other) const {
    return name_ < (char) toupper(other.get_name());
}


/*
    Chromosome
*/
Chromosome::Chromosome(GeneSequence genes) : genes_(genes) {};

// Create a pair of chromosomes for the given gene codes.
// This creates two chromosomes that have random alleles from each gene in the list provided. 
ChromosomePair Chromosome::MakeRandomPair(std::string genecodes) {
    GeneSequence left_genome, right_genome;
    for (const auto char_code: genecodes) {
        GeneType left_allele = (FlipCoin()) ?  Dominant: Recessive;
        GeneType right_allele = (FlipCoin()) ? Dominant: Recessive;
        left_genome[char_code] = Gene(char_code, left_allele);
        right_genome[char_code] = Gene(char_code, right_allele);
    }
    Chromosome left_chromo(left_genome);
    Chromosome right_chromo(right_genome);

    return std::make_pair(left_chromo, right_chromo);
};

const int Chromosome::get_length() const {return genes_.size();};
const GeneSequence& Chromosome::get_genes() const {return genes_;};

/*
    Trait
*/
Trait::Trait(std::string name, TraitType type, std::string genes) : name_(name), type_(type), gene_codes_(genes){}
const std::string Trait::get_name() const { return name_;}
const std::string& Trait::get_genes() const {return gene_codes_;}
float Trait::CalculateValue(Genome &genome) {
    /*
     * Needs to go find the genes that govern this trait in the genome
     * and determine the strength of the trait. There are three types of
     * traits which compute their value differently:
     * Binary: Dominant / Recessive nature of alleles
     * Dicrete: TBD
     * Continuous: Frequency of either dominant or recessive
     *             genes in a polygene group
     *
    */

    return ComputePolygeneStrength(genome);
};

float Trait::ComputePolygeneStrength(Genome &genome) {
    int n_dominant = 0;
    int n_recessive = 0;
    const GeneSequence *gene_sequence;
    GeneSequence::const_iterator it;
    for (auto chromo_pair: genome)
    {
        for (int chromo_idx=0; chromo_idx < 2; chromo_idx++) {
            gene_sequence = &(chromo_idx ? chromo_pair.first.get_genes() : chromo_pair.second.get_genes());
            for (const auto gene_code: gene_codes_)
            {
                it = gene_sequence->find(gene_code);
                if ( it != gene_sequence->end()) {
                    if (it->second.get_type() == Dominant)
                    {
                        n_dominant++;
                    } else {
                        n_recessive++;
                    }
                }
            }
        }
        if (n_dominant + n_recessive != 2*gene_codes_.length()) {throw UnrepresentedTraitError();}
        std::cout << name_ << " (N_DOM=" << n_dominant << ", N_REC=" << n_recessive << ")" << std::endl;
    }

    return n_dominant / n_recessive; // TODO update this to be more representitive of bell-curve-like probability dist.
}
