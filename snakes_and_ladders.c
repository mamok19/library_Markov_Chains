#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1

#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define SNAKE_PRINTING_FORMAT "[%d] -snake to-> "
#define LADDER_PRINTING_FORMAT "[%d] -ladder to-> "
#define REGULAR_CELL_PRINTING_FORMAT "[%d] -> "
#define LAST_CELL_PRINTING "[%d]"
#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20

#define NUM_ARGS_ERROR "Usage: invalid number of arguments"

//************************// functions for filling database //************************//
/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {
    {13, 4},
    {85, 17},
    {95, 67},
    {97, 58},
    {66, 89},
    {87, 31},
    {57, 83},
    {91, 25},
    {28, 50},
    {35, 11},
    {8, 30},
    {41, 62},
    {81, 43},
    {69, 32},
    {20, 39},
    {33, 70},
    {79, 99},
    {23, 76},
    {15, 47},
    {61, 14}
};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to; // cell which ladder leads to, if there is one
    int snake_to; // cell which snake leads to, if there is one
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/**
 * allocates memory for cells on the board and initalizes them
 * @param cells Array of pointer to Cell, represents game board
 * @return EXIT_SUCCESS if successful, else EXIT_FAILURE
 */
int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(cells[j]);
            }
            printf(ALLOCATION_ERROR_MESSAGE);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell){i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        } else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

int add_cells_to_database(MarkovChain *markov_chain, Cell *cells[BOARD_SIZE])
{
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        Node *tmp = add_to_database(markov_chain, cells[i]);
        if (tmp == NULL)
        {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int set_nodes_frequencies(MarkovChain *markov_chain, Cell *cells[BOARD_SIZE])
{
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node = get_node_from_database(markov_chain, cells[i])->data;
        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain,
                                             cells[index_to])->data;
            int res = add_node_to_frequency_list(from_node, to_node);
            if (res == EXIT_FAILURE)
            {
                return EXIT_FAILURE;
            }
        }
        else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell *) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node = get_node_from_database(markov_chain,
                                                 cells[index_to])->data;
                int res = add_node_to_frequency_list(from_node, to_node);
                if (res == EXIT_FAILURE)
                {
                    return EXIT_FAILURE;
                }
            }
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int fill_database_snakes(MarkovChain *markov_chain)
{
    Cell *cells[BOARD_SIZE];
    if (create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    if (add_cells_to_database(markov_chain, cells) == EXIT_FAILURE)
    {
        for (size_t i = 0; i < BOARD_SIZE; i++)
        {
            free(cells[i]);
        }
        return EXIT_FAILURE;
    }

    if(set_nodes_frequencies(markov_chain, cells) == EXIT_FAILURE)
    {
        for (size_t i = 0; i < BOARD_SIZE; i++)
        {
            free(cells[i]);
        }
        return EXIT_FAILURE;
    }

    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}
//************************// functions for initialize markovnode //************************//
bool is_last_initialize(const void * data){
    return ((Cell *)data)->number == 100;
}

void print_func_initialize(const void * data){
    Cell * cell = (Cell *)data;
    if (is_last_initialize(data)) {
        //print without arrow after
        printf(LAST_CELL_PRINTING, cell->number);
    }
    else if (cell->snake_to != EMPTY) {
        printf(SNAKE_PRINTING_FORMAT, cell->number);
    } else if (cell->ladder_to != EMPTY) {
        printf(LADDER_PRINTING_FORMAT,cell->number);
    } else {
        printf(REGULAR_CELL_PRINTING_FORMAT, cell->number);
    }
}
int comp_func_initialize(const void * first, const void * second){
    Cell *cell1 = (Cell *)first;
    Cell *cell2 = (Cell *)second;
    if (cell1->number == cell2->number) {
        return 0;
    }
    if (cell1->number > cell2->number){
        return 1;
    }
    return -1;
}

void * copy_func_initialize(const void * src){
    Cell cell = *(Cell *)src;
    Cell *new_cell = malloc(sizeof(Cell));
    if (new_cell == NULL) {
        return NULL;
    }
    new_cell->number = cell.number;
    new_cell->ladder_to = cell.ladder_to;
    new_cell->snake_to = cell.snake_to;
    return new_cell;
}


MarkovChain* initialize_markov_snakes()
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
    markov_chain->print_func = print_func_initialize;
    markov_chain->comp_func = comp_func_initialize;
    markov_chain->copy_func = copy_func_initialize;
    markov_chain->free_data = free;
    markov_chain->is_last = is_last_initialize;

    //initialize the linked list
    markov_chain->database->first = NULL;
    markov_chain->database->last = NULL;
    markov_chain->database->size = 0;
    return markov_chain;
}






/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("%s\n", NUM_ARGS_ERROR);
        return EXIT_FAILURE;
    }

    int seed = atoi(argv[1]);
    srand(seed);

    MarkovChain *markov_chain = initialize_markov_snakes();
    if (markov_chain == NULL)
    {
        return EXIT_FAILURE;
    }

    if (fill_database_snakes(markov_chain) == EXIT_FAILURE) // fill_database is not implemented
    {
        free_database(&markov_chain);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < atoi(argv[2]); i++)
    {
        printf("Random Walk %d:", i + 1);
        MarkovNode *first_node = markov_chain->database->first->data;
        generate_random_sequence(markov_chain, first_node, MAX_GENERATION_LENGTH);
    }

    free_database(&markov_chain);
    return EXIT_SUCCESS;
}
