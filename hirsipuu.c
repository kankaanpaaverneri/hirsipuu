#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <wchar.h>

#define TEXTFILE "sanasto.txt"
#define SIZE 36
#define MAX_ATTEMPTS 10
#define WORD_COUNT 653
#define SUCCESS 0
#define FAIL 1

void get_word_from_a_file(wchar_t*);
void remove_spaces(wchar_t*);
int hirsipuu(const wchar_t*);
void fill(wchar_t*);
int check_winning_condition(const wchar_t*);
int check_if_used_letter(const wchar_t*, wchar_t);


int main(void) {
	setlocale(LC_ALL, "");
	srand((int)time(NULL));
	wchar_t word[SIZE];
	get_word_from_a_file(word);
	//wprintf(L"%ls", word);
	if(hirsipuu(word) == SUCCESS) {
		wprintf(L"VOITIT PELIN\n");
	} else {
		wprintf(L"SINUT HIRTETTIIN\n");
		wprintf(L"VASTAUS: %ls\n", word);
	}
	return 0;
}

//Reads the file which contains the words
//It always takes a random word from the file
void get_word_from_a_file(wchar_t *word) {
	FILE *f = fopen(TEXTFILE, "r");
	if(f == NULL) {
		wprintf(L"ERROR READING FILE\n");
		exit(1);
	}
	size_t words_read = 0;
	const size_t random_word = rand()%WORD_COUNT+1;
	while(fgetws(word, SIZE-1, f)) {
		words_read++;
		if(words_read == random_word)
			break;
	}
	remove_spaces(word);
	fclose(f);
}

/*
File which contains the words has a space at the begining of every line
This removes that unnecessary space at the begining.
*/
void remove_spaces(wchar_t *word) {
	size_t i = 0;
	for(i = 0; word[i] != '\0'; i++) {
		word[i] = word[i+1];
	}
	word[i] = '\0';
}

int hirsipuu(const wchar_t *word) {
	wchar_t reveal[SIZE];
	wcscpy(reveal, word);
	fill(reveal);

	int attempts = 0;

	//Loop checks if you solved the word or failed
	while(check_winning_condition(reveal) == FAIL) {
		//Get input
		wchar_t buffer[SIZE];
		wprintf(L"Syötä kirjain: ");
		fgetws(buffer, SIZE-1, stdin);
		wchar_t quess;
		if(swscanf(buffer, L"%lc", &quess) != 1) {
			wprintf(L"VIHREELLINEN SYÖTE\n");
			continue;
		}
		if(check_if_used_letter(reveal, quess) == SUCCESS) {
			wprintf(L"KIRJAIN ON JO KÄYTETTY\n");
			continue;
		}

		//Checks if input matches the reveal strings
		unsigned int letters_read = 0;
		for(size_t i = 0; word[i] != '\0'; i++) {
			if(quess != word[i])
				letters_read++;
			else {
				reveal[i] = word[i];
			}
		}
		if(letters_read == wcslen(word)) {
			wprintf(L"AIJAI :(\n");
			attempts++;
			wprintf(L"Yrityksiä jäljellä: %lu\n", MAX_ATTEMPTS-attempts);
		} else {
			wprintf(L"HYVÄ :)\n");
		}
		if(attempts == MAX_ATTEMPTS) {
			return FAIL;
		}
		wprintf(L"%ls\n", reveal);
	}
	return SUCCESS;
}

//Fills the reveal string with * characters
void fill(wchar_t *reveal) {
	for(size_t i = 0; i < wcslen(reveal)-1; i++) {
		reveal[i] = L'*';
	}
}

int check_winning_condition(const wchar_t *reveal) {
	for(size_t i = 0; i < wcslen(reveal)-1; i++) {
		if(reveal[i] == L'*')
			return FAIL;
	}
	return SUCCESS;
}

int check_if_used_letter(const wchar_t *reveal, wchar_t quess) {
	for(size_t i = 0; i < wcslen(reveal)-1; i++) {
		if(quess == reveal[i])
			return SUCCESS;
	}
	return FAIL;
}