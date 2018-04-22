#include "common.h"
#include "genetics.h"
#include "creature.h"
#include <ctype.h>
#include <iostream>
#include <cstdlib>

/*
    Gene
*/
Gene::Gene(char name) : name_{name} {};

const char &Gene::get_name() const {name_;}


/*
    Chromosome
*/
Chromosome::Chromosome(std::string genecodes) {
    for (auto genecode: genecodes) {
        genes_.push_back(Gene(genecode));
    }
};

// Create a pair of chromosomes for the given gene codes.
// This creates two chromosomes that have random alleles from each gene in the list provided. 
ChromosomePair Chromosome::MakeRandomPair(std::string genecodes) {
    std::string left_genome, right_genome;
    for (const auto char_code: genecodes) {
        int left_allele = (FlipCoin()) ?  toupper(char_code): tolower(char_code);
        int right_allele = (FlipCoin()) ? toupper(char_code): tolower(char_code);
        left_genome.push_back((char) left_allele);
        right_genome.push_back((char) right_allele);
    }
    Chromosome left_chromo(left_genome);
    Chromosome right_chromo(right_genome);

    return std::make_pair(left_chromo, right_chromo);
};

const int Chromosome::get_length() const {return genes_.size();};