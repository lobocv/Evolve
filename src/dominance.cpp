#include "dominance.h"
#include "gene.h"
#include <ctype.h>
#include <utility>



AlleleRelationship::AlleleRelationship(Phenotype* phenotype, char gene_code): phenotype(*phenotype), gene_code(gene_code) {
    // Create two alleles for the gene relationship, one denoted as an upper case letter
    // and one as lower case, as is convention.
    char dominant_code = (char)toupper(gene_code);
    char recessive_code = (char)tolower(gene_code);

    alleles = std::make_pair( Gene(dominant_code), Gene(recessive_code) );
};


CompleteDominance::CompleteDominance(Phenotype* phenotype, char gene_code) : AlleleRelationship(phenotype, gene_code) {

    dominant_allele = &alleles.first;
    recessive_allele = &alleles.second;
};