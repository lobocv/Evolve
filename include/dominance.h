#ifndef __Dominance
#define __Dominance

#include <utility>
#include "phenotype.h"
#include "gene.h"

class Phenotype; // Forward declaration

/* 
From wikipedia: https://en.wikipedia.org/wiki/Dominance_(genetics)

While it is often convenient to talk about a recessive allele or a dominant trait,
dominance is not inherent to either an allele or its phenotype. Dominance is a
relationship between two alleles of a gene and their associated phenotypes.
A "dominant" allele is dominant to a particular allele of the same gene that can 
be inferred from the context, but it may be recessive to a third allele, and 
codominant to a fourth. Similarly, a "recessive" trait is a trait associated 
with a particular recessive allele implied by the context, but that same trait
may occur in a different context where it is due to some other gene and a 
dominant allele.
*/



struct AlleleRelationship {    
    Phenotype& phenotype;
    char gene_code;
    std::pair<Gene, Gene> alleles;
    AlleleRelationship(Phenotype&, char);
};

/*
Definitions of types of dominance, from Wikipedia:

In complete dominance, the effect of one allele in a heterozygous genotype completely masks 
the effect of the other. The allele that masks the other is said to be dominant to the latter,
 and the allele that is masked is said to be recessive to the former.
*/
class CompleteDominance: public AlleleRelationship {
    Gene &dominant_allele;
    Gene &recessive_allele;

public:
    CompleteDominance(Phenotype&, char);
};

/*
Incomplete dominance occurs when the phenotype of the heterozygous genotype is distinct from and 
often intermediate to the phenotypes of the homozygous genotypes. For example, red and white parent
flowers make offspring that are pink.
*/
class IncompleteDominance: public AlleleRelationship {
};

/*
Co-dominance occurs when the contributions of both alleles are visible in the phenotype. 
For example, red and white parent flowers make offspring with both red and white petals.
*/
class CodominantDominance: public AlleleRelationship {
};


#endif