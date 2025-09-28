
#include "markov_chain.h"
#include "linked_list.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//Don't change the macros!
#define FILE_PATH_ERROR "Error: incorrect file path"
#define NUM_ARGS_ERROR "Usage: invalid number of ar guments"
#define DELIMITERS " \n\t\r"

// functions for the markov chain initialization

void print_func_init(const void * data){
  printf("%s ", (char *)data);
}
int comp_func_init(const void * first, const void * second){
  return strcmp((char *)first, (char *)second);
}

void * copy_func_init(const void * src){
	char* src1 =((char *)src);
	char* dest = malloc(strlen(src1) + 1);
	strcpy(dest, src1);
	return dest;
}

bool is_last_init(const void * data){
  return ((char *)data)[strlen((char *)data) - 1] == '.';
}

int check_if_File_Name_Valid (const char *file_name)
{
	// I stopped here need to find out how to check files name
	FILE *file = fopen(file_name, "r");
	if (file)
	{
		fclose(file);
		return EXIT_SUCCESS;
	}
	else
	{
		fprintf(stdout, FILE_PATH_ERROR);
		return EXIT_FAILURE;
	}
}

int fill_database(FILE *fp, int words_to_read, MarkovChain* markov_chain)
{
	/**
	 *reads the words from the file and create database in markovchain
	 *@param File* fp -need to check the file if open
	 *@param int words_to_read could be 0 if neccery
	 *@param MarkovChain markov_chain database that need to be filed
	 */
	// if (check_if_File_Name_Valid(fp)) //exit failure = 1
	// {
	// 	return EXIT_FAILURE;
	// }
	// FILE *reading_file = fopen(fp, "r");
	// markov_chain->database = malloc(sizeof(LinkedList));
	// markov_chain->database->first = NULL;
	// markov_chain->database->last = NULL;
	// markov_chain->database->size = 0;
	char line[1024]= {0};
	char *token = NULL;
	while (fgets(line, sizeof(line), fp) && words_to_read >0)
	{
		token = strtok(line, DELIMITERS);
		// size_t token_length = strlen(token) + 1; // +1 for the null
		while (token != NULL)
		{
			words_to_read--;
			MarkovNode* first = add_to_database(markov_chain, (void*)token)->data;// i think the add function is not putting data correct
			if (first == NULL)
			{
				fprintf(stdout, "Error in add_to_database()\n");
				free_database(&markov_chain);
				return EXIT_FAILURE;
			}
			token = strtok(NULL, DELIMITERS);
			if (token != NULL)
			{
				// size_t next_token_length = strlen(token) + 1;
				MarkovNode* second = add_to_database(markov_chain, (void*)token)->data;
				add_node_to_frequency_list(first, second);
			}
		}
	}
	fclose(fp);
	return EXIT_SUCCESS;
}

MarkovChain* initialize_markov_chain()
{
	MarkovChain* markov_chain = malloc(sizeof(MarkovChain));
	if (!markov_chain)
	{
		printf(ALLOCATION_ERROR_MESSAGE);
		return NULL;
	}
	markov_chain->database = malloc(sizeof(LinkedList));
	if (!markov_chain->database) {
		printf(ALLOCATION_ERROR_MESSAGE);
		free(markov_chain);
		return NULL;
	}
    // initialize the marjov chain functions
    markov_chain->print_func = print_func_init;
    markov_chain->comp_func = comp_func_init;
    markov_chain->copy_func = copy_func_init;
    markov_chain->free_data = free;
    markov_chain->is_last = is_last_init;

    //initialize the linked list
	markov_chain->database->first = NULL;
	markov_chain->database->last = NULL;
	markov_chain->database->size = 0;
	return markov_chain;
}


int main(int argc, char* argv[])
{
	if (argc < 4 || argc > 5 )
	{
		printf("%s\n", NUM_ARGS_ERROR);
		return EXIT_FAILURE;
	}
	char *end;
	int const num_of_tweets= (int)strtol(argv[2], &end, 10);
	if (*end != '\0')
	{
		printf("Conversion error %s\n", end);
		return EXIT_FAILURE;
	}
	if (num_of_tweets> 20)
	{
		printf("To many requasted Tweets the max is 20 yours is %d \n", num_of_tweets);
		return EXIT_FAILURE;
	}
	//creating the markov chain that we will use
	MarkovChain *markov_chain = initialize_markov_chain(); //need to undarstand what to initialize here
	//starting to create database and tweets
	int words_to_read_from_file = INT_MAX;
	if (argc == 5)
	{
		char *end2;
		words_to_read_from_file = (int)strtol(argv[4], &end2, 10);
		if (*end2 != '\0')
		{
			printf("Conversion error %s\n", end2);
			free(markov_chain);
			return EXIT_FAILURE;
		}
	}
	if(check_if_File_Name_Valid(argv[3]) == EXIT_SUCCESS)
	{
		FILE *file = fopen(argv[3], "r");
		if (fill_database(file, words_to_read_from_file, markov_chain))
		{
			printf("EROR: something went wrong with the database fill\n");
			fclose(file);
			free_database(&markov_chain);
			return EXIT_FAILURE;
		}
		// fclose(file);
	}
	else
	{
		printf("%s\n", argv[3]);
		printf("File doesn't exist\n");
		return EXIT_FAILURE;
	}
	for (int i = 1; i <= num_of_tweets; i++)
	{
		printf("Tweet %d: ", i);
		generate_random_sequence(markov_chain ,get_first_random_node(markov_chain), markov_chain->database->size); //i need to undarstand what to put on max
	}
	free_database(&markov_chain);
	return EXIT_SUCCESS;
}




