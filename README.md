# Evolve
Creature evolution simulator


# Terminology


Organism - Has one or many Traits with exactly one Trait per TraitFamily. When an organism reproduces, it's offspring
           are assigned different value Traits of the same TraitFamilys as a result of genetic recombination.

TraitFamily - Has two or more phenotypes defining a phenospace.

Trait - A vector in the phenospace of a particular TraitFamily. The phenotype of the organism is the projection of the current
        value of the trait onto it's TraitFamily's phenotypes.
        
Phenotype - A variant of a particular trait. Has an associated gene (allele). It is an eigen-vector in the 
           phenospace of a particular trait.

Phenospace - A vector space that spans possible phenotypes. Phenotypes are represented as eigen-vectors in the vector-space
            The phenospace need not be singular, however if it is not singular, the linearly dependent vectors 
            must be a negative scaling of one another. This signifies anti-correlation
            of the corresponding phenotypes.

Gene - Contains information about the strength of one or multiple Traits. When the gene has information about multiple traits
       it is said to be  pleiotropic. If a non-binary trait has influence from multiple genes, those genes are said to be
       polygenic. These two concepts are not mutually exclusive. An example of a polygene group is a group of genes that
       when observed in high numbers, cause the organism to be tall. An example of pleiotropic gene is a gene who's occurance
       affects both the height and weight of an organism simultaneously.
       