#include "markov_chain.h"
#include "linked_list.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define NUM_ARGS_ERROR "Usage: invalid number of ar guments"

int get_random_number(int max_number)
{
	/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
	return rand() % max_number;
}
// genaral functions
MarkovNode *create_markov_node(const void *data, MarkovChain *markov_chain) {
	//Allocate memory for Markov Node
	MarkovNode *markov_node = malloc(sizeof(MarkovNode));
	if (markov_node == NULL)
	{
		free(markov_node);
		return NULL;
	}
	markov_node->data = markov_chain->copy_func(data); // is size of data is the size of the pointer or the size of the string?
	if (markov_node->data == NULL) {
		free(markov_node->data);
		free(markov_node);
		return NULL;
	}
	// memcpy(markov_node->data, data, data_size);
	markov_node->frequency_capacity=0;
	markov_node->comp_func = markov_chain->comp_func;
	return markov_node;
}



//genarate tweets function related third part


MarkovNode* get_first_random_node(MarkovChain* markov_chain)
{
	/**
 * Get one random MarkovNode from the given markov_chain's database.
 * @param markov_chain
 * @return the random MarkovNode
 */
	// Node* tmp = markov_chain->database->first;
	while (true)
	{
		int rand_num = get_random_number(markov_chain->database->size);
		Node* tmp = markov_chain->database->first;
		for (int i = 0; i < rand_num; ++i)
		{
			tmp = tmp->next;
		}
		// int len_of_string = strlen(tmp->data->data);
		 if (markov_chain->is_last(tmp->data->data) == false)
		{
			return tmp->data;
		} //for some reason it dosent return the correct value but diffrent node answer: ididnt print the first node
	}
	return NULL; //something went wrong
}


MarkovNode* get_next_random_node(MarkovNode* cur_markov_node)
{
	/**
	 *the count is the counter untill rand and the
	 *count of nodes find the currect node acorrding to the frequancy
	 *count of frequancy and staing on the same node
 * Choose randomly the next MarkovNode, depend on it's occurrence frequency.
 * @param cur_markov_node current MarkovNode
 * @return the next random MarkovNode
 */
	int rand_num =0;
	if (cur_markov_node->frequency_capacity != 0)
	{
		rand_num = get_random_number(cur_markov_node->frequency_capacity);
	}
	int count_of_frequencies = 0;
	int count_of_nodes = 0;
	for (int count = 0; count <cur_markov_node->frequency_capacity; count++)
	{
		if (count == rand_num)
		{
			return cur_markov_node->frequency_list[count_of_nodes].markov_node;
		}
		if (cur_markov_node->frequency_list[count_of_frequencies].frequency > 0 && cur_markov_node->frequency_list[count_of_frequencies].frequency != count_of_frequencies)
		{
			count_of_frequencies++;
		}
		else
		{
			count_of_nodes++;
			count_of_frequencies = 0;
		}
	}
	return NULL;
}


void generate_random_sequence(MarkovChain* markov_chain, MarkovNode* first_node, int max_length)
{
	/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence must have at least 2 words in it.
 * @param first_node markov_node to start with
 * @param  max_length maximum length of chain to generate
 */
	markov_chain->print_func(first_node->data); // i did not use the printf method
	int count_of_words = 1;
	MarkovNode* next_node = first_node;
	while (count_of_words < max_length && markov_chain->is_last(next_node->data) == false)
	{
		next_node = get_next_random_node(next_node);
		markov_chain->print_func(next_node->data); //maybe there is a condition that i need to add
		count_of_words++;
	}
	printf("\n");
}

// data base and get structets first part
Node* get_node_from_database(MarkovChain* markov_chain, void* data_ptr)
{
	/*
		* Check if data_ptr is in database. If so, return the Node wrapping it in
		 * the markov_chain, otherwise return NULL.
		 * @param markov_chain the chain to look in its database
		 * @param data_ptr the data to look for
		 * @return Pointer to the Node wrapping given data, NULL if state not in
		 * database.
		 */
	Node* tmp = markov_chain->database->first;
	for (int i = 0; i < markov_chain->database->size; i++)
	{
		if (markov_chain->comp_func(data_ptr, tmp->data->data) == EXIT_SUCCESS) //dosent get from the add a char insted gets a pointer
		{
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

Node* add_to_database(MarkovChain* markov_chain, void* data_ptr)
{
	/**
	* If data_ptr in markov_chain, return it's node. Otherwise, create new
	 * node, add to end of markov_chain's database and return it.
	 * @param markov_chain the chain to look in its database
	 * @param data_ptr the data to look for
	 * @return Node wrapping given data_ptr in given chain's database,
	 * returns NULL in case of memory allocation failure.
	 */
	Node* tmp = get_node_from_database(markov_chain, data_ptr);
	if (tmp != NULL)
	{
		return tmp;
	}

	MarkovNode * markov_node = create_markov_node(data_ptr ,markov_chain); //need to understand what to put in the data size
	if (add(markov_chain->database, markov_node)== EXIT_SUCCESS)
	{
		return markov_chain->database->last;
	} //adds an allocation of memory of new node
	free(markov_node->data);
	markov_node->data = NULL;
	free(markov_node);
	markov_node = NULL;
	return NULL;
	}

MarkovNodeFrequency *create_markov_node_frequency()
{
	MarkovNodeFrequency *frequency_list = malloc(sizeof(MarkovNodeFrequency)); //want me to add this into the scope of the function
	if (frequency_list == NULL)
	{
		return NULL;
	}
	return frequency_list;
}

int add_node_to_frequency_list(MarkovNode* first_node, MarkovNode* second_node)
{
	/**
	 * Add the second markov_node to the frequency list of the first markov_node.
	 * If already in list, update it's occurrence frequency value.
	 * @param first_node
	 * @param second_node
	 * @return success/failure: 0 if the process was successful, 1 if in
	 * case of allocation error.
	 */
	int is_first_node = 1;
	if (first_node->frequency_list == NULL)
	{
		is_first_node = 0;
		first_node->frequency_list = create_markov_node_frequency();
	}
	else
	{
		for (int i = 0; i < first_node->frequency_capacity; i++)
		{
			if (first_node->comp_func(first_node->frequency_list[i].markov_node->data, second_node->data) == EXIT_SUCCESS)
			{
				first_node->frequency_list[i].frequency++;
				return EXIT_SUCCESS;
			}
			// i need to find a way to go over and check for the names
			i++;
		}
	}
	if (is_first_node)
	{
		MarkovNodeFrequency* new_array = realloc(first_node->frequency_list,(sizeof(MarkovNodeFrequency) * (first_node->frequency_capacity + 1)));
		if (new_array == NULL)
		{
			printf(ALLOCATION_ERROR_MESSAGE);
			return EXIT_FAILURE; // Allocation failure
		}
		first_node->frequency_list = new_array; // super important to add the new array to the frequency list
	}
	first_node->frequency_list[first_node->frequency_capacity].markov_node = second_node;
	first_node->frequency_list[first_node->frequency_capacity].frequency = 1;
	first_node->frequency_capacity++;
	return EXIT_SUCCESS;
}

void free_database(MarkovChain ** ptr_chain) {
	MarkovChain *markov_chain = *ptr_chain;
	if (markov_chain->database != NULL) {
		Node *current = markov_chain->database->first;
		while (current != NULL) {
			Node *next = current->next;
			MarkovNode *markov_node = current->data;
			if (markov_node != NULL) {
				if (markov_node->frequency_list != NULL) {
					markov_chain->free_data(markov_node->frequency_list); // free the frequancy list
				}
				markov_chain->free_data(markov_node->data); // free the data
				markov_node->data = NULL;
				markov_chain->free_data(markov_node); // free the markovnode
				markov_node = NULL;

			}
			markov_chain->free_data(current); //free the node
			current = next;
		}
		markov_chain->free_data(markov_chain->database); // free the database
		markov_chain->database = NULL;
	}
	markov_chain->free_data(markov_chain); // free the markov chain
	markov_chain = NULL;
	*ptr_chain = NULL;
}




