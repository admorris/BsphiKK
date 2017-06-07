# Toy studies

These studies are designed to test the self-consistency of the PDF: can it find the values used to generate a dataset from itself?
Discrepancies indicate bugs which must be ironed out if the results are ever to be trusted.

## Modularity of the signal PDF

I like to think of the PDF in 'modules' which can be switched on or off in the XML.
Each of these can be tested for bug and biases using toy studies.
These are

- matrix element
- time integral
- 4D acceptance
- m(KK) resolution

### Matrix element

Variations:
- non-resonant
- flatte shape
- breit-wigner shape
- spin: 0, 1, 2
- barrier factor radii

### Time integral

Variations:
- no mixing -> calculate at t=0
- lower bound of integral
- ΓH and ΓL

### 4D acceptance

- no acceptance function
- threshold switch-on function
- moments vs histogram

### m(KK) resolution

Variations
- no smearing
- number of samples
- mass-independent width
- value of width

## The background PDF

The background PDF is reasonably simple compared to the signal PDF: it's just some sort of 4D shape fixed to the sideband.

Variations:
- moments vs histogram for the angular part
- histogram vs crystal ball for mass part

