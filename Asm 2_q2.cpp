#include <iostream>
#include <fstream>
using namespace std;
#define SIZE 10000 //size of text read for each term
#define LEN 31 // max length of sctring characters
#define MAX 2500 // max of index inWords

bool checkLetter(char charac) {
	return charac >= 'a' && charac <= 'z';
}

void addSameType(char& toAdd, char forAdd, int& g) {
	toAdd = forAdd;
	g += 1;
}

void addDiffType(char& past, char& toAdd, char forAdd, int& c, int& g) {
	past = '\0';
	c += 1;
	toAdd = forAdd;
	g = 1;
}

void getInput(char inWords[MAX][LEN + 1], int& c) {
	char inText[MAX];
	cout << "Please input paragraph in the console: " << endl;
	cin.getline(inText, MAX);
	int i = 0, g = 0;
	bool letter = checkLetter(inText[i]);

	while (i != strlen(inText) && i < MAX) {
		if (letter) {
			if (checkLetter(inText[i]))
				addSameType(inWords[c][g], inText[i], g);
			else {
				addDiffType(inWords[c][g], inWords[c + 1][0], inText[i], c, g);
				letter = false;
			}
		}
		else {
			if (!(checkLetter(inText[i])))
				addSameType(inWords[c][g], inText[i], g);
			else {
				addDiffType(inWords[c][g], inWords[c + 1][0], inText[i], c, g);
				letter = true;
			}
		}
		i += 1;
	}
	inWords[c][g] = '\0';
	c += 1;
	cout << endl;
}

bool checkCorrect(char word[LEN + 1], char inWord[LEN + 1], int len, bool& misspell) {
	bool correct = false;
	int wrongLetter = 0;
	for (int i = 0; word[i] != '\0' && i < len; i++)
		if (inWord[i] != word[i])
			wrongLetter += 1;
	if (wrongLetter == 0)
		correct = true;
	else if (wrongLetter == 1)
		misspell = true;
	return correct;
}

bool checkOneLessL(char word[LEN + 1], char inWord[LEN + 1], int len) {
	bool misspell;
	if (inWord[0] != word[0]) {
		misspell = true;
		for (int i = 1; i <= len; i++)
			if (inWord[i] != word[i - 1]) {
				misspell = false;
				break;
			}
	}
	else {
		misspell = false;
		for (int i = 1, g = 1; i <= len; i++, g++)
			if (inWord[i] != word[g])
				if (misspell == true) {
					misspell = false;
					break;
				}
				else {
					misspell = true;
					g -= 1;
				}
	}
	return misspell;
}

bool checkOneMoreL(char word[LEN + 1], char inWord[LEN + 1], int len) {
	bool misspell = false;
	if (inWord[0] != word[0]) {
		misspell = true;
		for (int i = 0; i < len; i++)
			if (inWord[i] != word[i + 1]) {
				misspell = false;
				break;
			}
	}
	else {
		for (int i = 1, g = 1; i < len; i++, g++)
			if (inWord[i] != word[g])
				if (misspell == true) {
					misspell = false;
					break;
				}
				else {
					misspell = true;
					i -= 1;
				}
			else if (i == len - 1 && misspell == false)
				misspell = true;
	}
	return misspell;
}

void addSuggestion(char suggestion[], char word[LEN + 1], int len, int& g) {
	for (int z = 0; z < len; z++)
		suggestion[z] = word[z];
	suggestion[len] = '\0';
	g += 1;
}

void findMisspell(char words[SIZE][LEN + 1], int num, char inWord[LEN + 1], bool& correct, char suggestions[10][LEN + 1], int& g) {
	int len = strlen(inWord);
	for (int j = 0; j < num; j++) {  // loop for each words in words[]
		bool misspell = false;
		if (strlen(words[j]) == len) {  // when words len == inWords len
			correct = checkCorrect(words[j], inWord, len, misspell);
			if (correct)
				break;
		}
		else if (strlen(words[j]) == len - 1)  // when words len - 1 == inWords len
			misspell = checkOneLessL(words[j], inWord, len);
		else if (strlen(words[j]) == len + 1) {  // when words len + 1 == inWords len
			misspell = checkOneMoreL(words[j], inWord, len);
		}
		if (misspell && g != 10)
			addSuggestion(suggestions[g], words[j], strlen(words[j]), g); // add misspelled word in suggestion
	}

}

void printSuggestions(char word[LEN + 1], char suggestions[10][LEN + 1], int g) {
	cout << "The spelling of " << word << " is incorrect! Please select one of the following suggestions: " << endl;
	if (g != 0) {
		int i;
		for (i = 0; i < g; i++)
			cout << i + 1 << ". " << suggestions[i] << endl;
		cout << i + 1 << ". give up" << endl;
	}
	else
		cout << "No suggestion." << endl << "1. give up" << endl;
}

int getSelection(int g) {
	int choice = -1;
	while (!(choice > 0 && choice <= g + 1)) {
		cout << "Your selection is: ";
		cin >> choice;
		if (!(choice > 0 && choice <= g + 1))
			cout << "Your input is invalid! Please input again!" << endl;
	}
	cout << endl;
	return choice - 1;
}

void output(char inWords[MAX][LEN + 1], int c) {
	ofstream fout;
	fout.open("output.txt");
	if (!fout.is_open()) {  // check if file is open
		cout << "Unable to open file hello.txt." << endl;
		exit(-20);
	}
	else {
		for (int z = 0; z < c; z++)
			fout << inWords[z];
		cout << "The corrected paragraph has been written into output.txt." << endl;
	}
	fout.close();
}

int main() {
	char s[100], words[SIZE][LEN + 1] = { 0 }, inWords[MAX][LEN + 1] = { 0 };
	int num = 0, count = 0, numWords = 0, wordPos = 0; // numWords = num of words in words[], count = num of index in inWords
	for (int k = 0; k < SIZE; k++) words[k][0] = '\0'; // initialize the words array
	getInput(inWords, count);
	ifstream fin;
	fin.open("wordsEn.txt");
	if (!fin.is_open()) {
		cout << "Unable to open file wordsEn.txt." << endl;
		exit(-10); // error
	}
	for (int i = 0; i < count; i++) {  // loop for every index in inWords, i = index of current inWords[]
		if (checkLetter(inWords[i][0])) { // make sure it's a word, not symbols
			bool correct = false;
			char suggestions[10][LEN + 1] = { 0 };
			int choice, g = 0;  // g = num of suggestions
			while (!fin.eof() && numWords < SIZE) {
				fin.getline(s, 100);  // read a line
				if (fin.fail() && s[0] != '\0') { // some errors encountered
					cout << "last line: " << s << endl;
					exit(-11);
				}
				else { // process when there is no error
					if (strlen(s) <= LEN + 1) { // process when num of letter <= LEN
						int j = 0;
						for (j = 0; j < strlen(s); j++) words[numWords][j] = s[j]; // copy s to words[i]
						words[numWords][j] = '\0';
						numWords++;
					}
					if (numWords == SIZE) {
						findMisspell(words, numWords, inWords[i], correct, suggestions, g);
						memset(words, '\0', sizeof(words));
						numWords = 0;
						if (correct)
							break;
					}
				}
			}
			if (numWords != 0)
				findMisspell(words, numWords, inWords[i], correct, suggestions, g);
			if (!(correct)) {
				printSuggestions(inWords[i], suggestions, g);
				choice = getSelection(g);
				if (choice != g) {
					for (int z = 0; z <= strlen(suggestions[choice]); z++)
						inWords[i][z] = suggestions[choice][z];
				}
			}
			fin.clear();
			fin.seekg(0);
		}
	}

	output(inWords, count);

	fin.close();

	return 0;
}