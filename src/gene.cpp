#include "gene.h"
#include "common.h"
#include <ctype.h>
#include <iostream>
#include <cstdlib>

Gene::Gene(char name) : name{name} {};

char Gene::get_name() const {
    return name;
};


Chromosome::Chromosome(std::string genecodes) {
    for (auto genecode: genecodes) {
        genes.push_back(Gene(genecode));
    }
};

// Create a pair of chromosomes for the given gene codes.
// This creates two chromosomes that have random alleles from each gene in the list provided. 
std::pair<Chromosome, Chromosome> Chromosome::MakePair(std::string genecodes) {
    std::string left_genome, right_genome;
    for (const auto char_code: genecodes) {
        int left_allele = (std::rand() % 2) ?  toupper(char_code): tolower(char_code);
        int right_allele = (std::rand() % 2) ? toupper(char_code): tolower(char_code);
        left_genome.push_back((char) left_allele);
        right_genome.push_back((char) right_allele);
    }
    Chromosome left_chromo(left_genome);
    Chromosome right_chromo(right_genome);

    return std::make_pair(left_chromo, right_chromo);
};

std::ostream &operator<< (std::ostream &stream, const Chromosome &c) {
    auto it = c.genes.begin();
    for (it; it != c.genes.end(); it++) {
        stream << (*it).get_name();    
    }
    return stream;
};