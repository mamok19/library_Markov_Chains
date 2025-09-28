# Library: Markov Chains for Probabilistic Text Generation

A small C library implementing **Markov chains** for probabilistic text generation.  You can use it to build models from input text (e.g. tweets) and then generate new text based on that model.

---

## Table of Contents

- [Features](#features)  
- [Project Structure](#project-structure)  
- [Requirements](#requirements)  
- [Building / Installation](#building--installation)  
- [Usage](#usage)  
- [API Reference](#api-reference)  
- [Examples](#examples)  
- [Testing](#testing)  
- [Future Work](#future-work)  
- [License](#license)

---

## Features

- Build a Markov chain model from text input  
- Generate new text probabilistically based on the model  
- Support for configurable “order” of Markov chains (e.g. how many past words or characters to condition on)  
- Clean C implementation, easy to integrate into other C projects  

---

## Project Structure

Here’s a general outline of how you might use the library in your own C code:


#include "markov_chain.h"

int main(void) {
    MarkovChain *mc = markov_chain_create(order);     // e.g. order = 2
    markov_chain_train(mc, "justdoit_tweets.txt");    // train on a file or text
    char *generated = markov_chain_generate(mc, length);
    printf("Generated: %s\n", generated);
    markov_chain_free(mc);
    free(generated);
    return 0;
}




