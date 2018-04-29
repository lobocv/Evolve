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

const char &Gene::get_name() const {
    return type_ ? (char)toupper(name_) : (char)tolower(name_);
}


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
Trait::Trait(std::string name, std::string genes) : name_(name), gene_codes_(genes){}
const std::string Trait::get_name() const { return name_;}
const std::string& Trait::get_genes() const {return gene_codes_;}
float Trait::CalculateValue(Genome &genome) {
    std::cout << "CALCULATE VALUE" << std::endl;

    // Go through each chromosome pair in the genome
    // Attempt to find the trait genes in each chromosome
    // If exists, use it to calculate trait value
    // if any of the trait's genes don't exist in the genome
    // then raise error.

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



};
