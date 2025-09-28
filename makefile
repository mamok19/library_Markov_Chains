tweets_generator: tweets_generator.c linked_list.c linked_list.h markov_chain.c markov_chain.h
	gcc -std=c99 linked_list.c markov_chain.c tweets_generator.c -o tweets_generator

snakes_and_ladders: snakes_and_ladders.c linked_list.c linked_list.h markov_chain.c markov_chain.h
	gcc -std=c99 linked_list.c markov_chain.c snakes_and_ladders.c -o snakes_and_ladders