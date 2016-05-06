## BasicSPN tools for CM30173/CM50210

This programs here demonstrate the concept of differential cryptanalysis on a basic Substitution-Permutation network.

* `make diff-class` gives you an implementation of the differential cryptanalysis attack on the SPN defined in the book.
* `make diff-book` same as above but this is the SPN defined in the book  -- Cryptography: theory and practice.

#### Note:

The SPN parameters are defined in diff_crypto.c; Currently, rounds keys have to be set manually.
