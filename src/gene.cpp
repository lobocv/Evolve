#include "gene.h"
#include "common.h"
#include <ctype.h>
#include <iostream>
#include <cstdlib>

Gene::Gene(char name) : name{name} {};

char Gene::get_name() const {
    return name;
};


Chromosome::Chromosome(geneset genecodes) {
    for (auto genecode: genecodes) {
        genes.push_back(Gene(genecode));
    }
};

// Create a pair of chromosomes for the given gene codes.
// This creates two chromosomes that have random alleles from each gene in the list provided. 
std::pair<Chromosome, Chromosome> Chromosome::MakePair(geneset genecodes) {
    geneset left_chromo, right_chromo;
    for (auto genecode: genecodes) {
        left_chromo.insert((std::rand() % 2) ? (char) toupper(genecode): (char) tolower(genecode) );
        right_chromo.insert((std::rand() % 2) ? (char) toupper(genecode): (char) tolower(genecode) );
    }
    Chromosome uppcase_case_chromosome(left_chromo);
    Chromosome lower_case_chromosome(right_chromo);

    return std::make_pair(uppcase_case_chromosome, lower_case_chromosome);
};

std::ostream &operator<< (std::ostream &stream, const Chromosome &c) {
    auto it = c.genes.begin();
    for (it; it != c.genes.end(); it++) {
        stream << (*it).get_name();    
    }
    return stream;
};