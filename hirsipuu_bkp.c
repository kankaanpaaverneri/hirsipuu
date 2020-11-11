#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#define TEXTFILE "sanasto.txt"
#define SIZE 36
#define MAX_ATTEMPTS 10
#define WORD_COUNT 653

#define SUCCESS 0
#define FAIL 1

#define SMALL_LETTERS_BEGIN 64
#define SMALL_LETTERS_END 91
#define UPPER_LETTERS_BEGIN 96
#define UPPER_LETTERS_END 123

void get_word_from_a_file(char*);
void remove_spaces(char*);
int hirsipuu(const char*);
void fill(char*);
int check_winning_condition(const char*);

//const unsigned int word_count = 653;

int main(void) {
	srand((int)time(NULL));
	char word[SIZE];
	get_word_from_a_file(word);
	//printf("%s", word);
	if(hirsipuu(word) == SUCCESS) {
		printf("VOITIT PELIN\n");
	} else {
		printf("SINUT HIRTETTIIN\n");
		printf("VASTAUS: %s\n", word);
	}

	return 0;
}

void get_word_from_a_file(char word[SIZE]) {
	FILE *f = fopen(TEXTFILE, "r");
	if(f == NULL) {
		printf("ERROR READING FILE\n");
		exit(1);
	}
	size_t words_read = 0;
	const size_t random_word = rand()%WORD_COUNT+1;
	while(fgets(word, SIZE-1, f)) {
		words_read++;
		if(words_read == random_word)
			break;
	}
	remove_spaces(word);
	fclose(f);
}

void remove_spaces(char *word) {
	int i = 0;
	for(i = 0; word[i] != '\0'; i++) {
		word[i] = word[i+1];
	}
	word[i] = '\0';
}

int hirsipuu(const char *word) {
	char reveal[SIZE];
	strcpy(reveal, word);
	fill(reveal);
	int attempts = 0;
	while(check_winning_condition(reveal) == FAIL) {
		char buffer[SIZE];
		printf("Syötä kirjain: ");
		fgets(buffer, SIZE-1, stdin);
		char quess;
		if(sscanf(buffer, "%c", &quess) != 1) {
			printf("VIHREELLINEN SYÖTE\n");
			continue;
		}
		int letters_read = 0;
		for(size_t i = 0; word[i] != '\0'; i++) {
			if(quess != word[i])
				letters_read++;
			else {
				reveal[i] = word[i];
			}
		}
		if(letters_read == strlen(word)) {
			printf("AIJAI :(\n");
			attempts++;
		} else {
			printf("HYVÄ :)\n");
		}
		if(attempts == MAX_ATTEMPTS) {
			return FAIL;
		}
		printf("%s\n", reveal);
	}
	return SUCCESS;
}

void fill(char *reveal) {
	for(int i = 0; i < strlen(reveal)-1; i++) {
		reveal[i] = '*';
	}
}

int check_winning_condition(const char *reveal) {
	for(int i = 0; i < strlen(reveal)-1; i++) {
		if(reveal[i] == '*')
			return FAIL;
	}
	return SUCCESS;
}