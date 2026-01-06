# Rotate-Paper

This repository contains the sources for the paper
[Valentin Blomer, Kai-Uwe Bux: The cost of cyclic permutations and remainder sums in the Euclidean algorithm](http://arxiv.org/abs/2601.00979)


## Running the measurements

You will need the nix package manager. 

Clone the repository and change-directory into it.

Now, do:

   ```bash
   nix develop
   make
   ```
   
This will make all measurements on your local machine and create the paper with figures showing those measurements. Beware: this may take days.
