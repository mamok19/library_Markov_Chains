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
.
├── CMakeLists.txt
├── makefile
├── .gitignore
├── linked_list.c / linked_list.h
├── markov_chain.c / markov_chain.h
├── snakes_and_ladders.c
├── tweets_generator.c / tweets_generator
├── justdoit_tweets.txt


- **linked_list.\*** — helper list data structures  
- **markov_chain.\*** — core logic for building and querying the Markov chain  
- **tweets_generator.c** — example program to generate tweets from the chain  
- **justdoit_tweets.txt** — sample text input (used to build the model)  
- **snakes_and_ladders.c** — (maybe a project side file)  
- **CMakeLists.txt / makefile** — build scripts  

---

## Requirements

- A C compiler (e.g. `gcc`)  
- CMake (if using the CMake build)  
- Make (if using the provided makefile)  
- Standard C library  

---

### Using CMake

```bash
mkdir build
cd build
cmake ..
make


### Examples


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




