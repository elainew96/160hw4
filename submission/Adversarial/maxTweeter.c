//
// maxTweeter.c
// ECS 160 HW 4
//
// Created by Daniel Ruiz and Nick Doherty
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MOST_FREQUENT_COUNT 10
#define MAX_LINE_WIDTH 1024
#define MAX_FILE_LENGTH 20000
#define MAX_TWEETERS MAX_FILE_LENGTH

#define NAME_KEY "\"name\""

static void _print_invalid_and_exit() {
    printf("Invalid Input Format\n");
    exit(0);
}

// Returns a new string stripped of beginning and ending "'s
// You are responsible for freeing the returned string
static char * _string_by_stripping_quotes(char *string) {
    int length = strlen(string);
    if (string != NULL && length > 0) {
        if (string[0] == '\"') {
            string++;
            length -= 1;
        }
        if (string[strlen(string) - 1] == '\"' && length > 1) {
            length -= 1;
        }

        char *strippedString = (char *)malloc(sizeof(char) * (length + 1));
        memcpy(strippedString, string, length);
        strippedString[length] = 0;

        return strippedString;
    } else if (length == 0) {
        char *string = (char *)malloc(sizeof(char) * 1);
        string[0] = 0;

        return string;
    }

    return NULL;
}

// Returns the value at the given index in the given csv line, if it exists
// You are responsible for freeing this pointer
static char * _get_value_at_index(char *line, int index) {
    // Copy the input string since strtok will destroy the string
    char *copy = strdup(line);
    char *copyStart = copy; // Save the beginning of the malloc'd memory for freeing later
    
    char *token = strtok(copy, ",");
    while (token != NULL && index > 0) {
        token = strtok(NULL, ",");
        index--;
    }

    char *value = NULL;
    if (index == 0) {
        int length = strlen(token);
        value = (char *)malloc(sizeof(char) * (length + 1));
        memcpy(value, token, length + 1);
        value[length] = 0;
    }

    free(copyStart);
	return value;
}

// Returns the index in the csv line containing value, if it exists
static int _get_index_of_value(char *line, char *value) {
    // Copy the input string since strtok will destroy the string
    char *copy = strdup(line);
    char *copyStart = copy; // Save the beginning of the malloc'd memory for freeing later

    int index = 0;
    char *token = strtok(copy, ",");
    while (token != NULL) {
        if (strcmp(token, value) == 0) {
            free(copy);
            return index; 
        }
        token = strtok(NULL, ",");
        index += 1;
    }

    free(copyStart);
    return -1;
}

// Returns the column index in the csv file containing the 'name' field
static int _get_tweeter_name_index(FILE *stream) {
    char line[MAX_LINE_WIDTH];
    if (fgets(line, MAX_LINE_WIDTH, stream) != NULL) {
        int index = _get_index_of_value(line, NAME_KEY);
        if (index >= 0) {
            return index;
        } else {
            _print_invalid_and_exit();
        }
    }

    _print_invalid_and_exit();
    return -1;
}

// Returns the index of a given value in a list of c strings
// If value is not found, it is inserted and its index is returned
static void _update_tweeters_and_frequencies_inserting_if_needed(char *tweeter, char **tweeters, int *frequencies) {
    // If we already have this tweeter in our data structures, just increment the count
    // Otherwise, insert them and initialize their count to 1
    int found = 0, index = 0;
    for (; index < MAX_TWEETERS && tweeters[index] != NULL; index++) {
        if (strcmp(tweeter, tweeters[index]) == 0) {
            found = 1;
            break;
        }
    }
    if (found) {
        frequencies[index] += 1;
    } else {
        tweeters[index] = tweeter;
        frequencies[index] = 1;
    }
}

// Determines and displays the most frequent tweets in a given file stream
static void _displayMostFrequentTweets(FILE* stream) {
    int nameIndex = _get_tweeter_name_index(stream);

    char *tweeters[MAX_TWEETERS];
    int frequencies[MAX_TWEETERS];

    int mostFrequent[MOST_FREQUENT_COUNT];
    
    char line[MAX_LINE_WIDTH];
    while (fgets(line, MAX_LINE_WIDTH, stream)) {	
		char *name = _get_value_at_index(line, nameIndex);
        if (name) {
            _update_tweeters_and_frequencies_inserting_if_needed(name, tweeters, frequencies);
        } else {
            _print_invalid_and_exit();
        }
	}

	// Implements a simple naive sort
    for (int i = 0; i < MAX_TWEETERS && tweeters[i] != NULL; i++) {
        for (int j = i + 1; j < MAX_TWEETERS && tweeters[j] != NULL; j++) {
            if (frequencies[i] < frequencies[j]) {
            	// Swap frequencies
                int tempFreq = frequencies[i];
                frequencies[i] = frequencies[j];
                frequencies[j] = tempFreq;
                
                // Swap tweeters
                char* tempTweet = tweeters[i];
                tweeters[i] = tweeters[j];
                tweeters[j] = tempTweet;
            }
        }
    }

	// Print the most frequent tweeters
	for (int i = 0; i < MOST_FREQUENT_COUNT && tweeters[i] != NULL; i++) {
        char *name = _string_by_stripping_quotes(tweeters[i]);
		
        printf("%s:%d\n", name, frequencies[i]);
        
        free(name);
	}
}

int main(int argc, char *argv[] ) {
 	FILE *stream = fopen(argv[1], "r");
    if (stream != NULL) {
 	  _displayMostFrequentTweets(stream);
    } else {
        _print_invalid_and_exit();
    }
    fclose(stream);

    return 0;
}
