/*
Data Structures
Homework 1
9/25/2019
Michael Reagan
Spencer Jones
Nathan Draper
Section 01
Group 10
*/

/*
This program will read a text file and output the author name, line count, and word count to an output file called CardCatalog.txt.
This program assumes that the input file will use the following format at the beginning of the file:

Book Title
Author Name
Contents:
text

The program will also calculate the percentage frequency of each letter of the alphabet found in the text and display it to the user if the
user chooses to see it.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <numeric>

using namespace std;

const int LETTERS = 26;
const string OUTPUT_FILENAME = "CardCatalog.txt";

struct Book {
	string title;
	string author;
	unsigned wordCount = 0;
	unsigned lineCount = 0;
	double letterFrequency[LETTERS] = {0};
 };

/*
This function parses a string containing first name and last name separated by a space and stores
the names in an array passed into the function.
It will not work with middle names or initials, and it will not work if there are extra spaces in between the first and last name.
*/
void parseName(string name, string splitName[]) {
	const int index = name.find(" ");
	splitName[0] = name.substr(0, index);
	splitName[1] = name.substr(index + 1, name.length());
}

// This function prompts the user for a string, validates that the input stream has no errors, and then returns the string
string getString() {
	string str;

	cin >> str;

	// Validation
	while (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Enter a valid string: ";
		cin >> str;
	}

	return str;
}

/* 
This function accepts an fstream file object and prompts the user for a filname with the getString() function.
The function will ask again if the file does not open correctly (E.g. when the file is to be opened in read mode but does not exist).
The function accepts a boolean *read* which should be true if the file is to be opened in read-only mode, false if write only. This way,
the function could be used for either input or output files.
Files can not be opened in read and write mode simultaneously with this function.
*/
void openFile(fstream& file, bool read) {
	string filename;

	do{
		cout << "Enter a filename: ";
		filename = getString();
		if (read) {
			file.open(filename, ios::in);
		}
		else {
			file.open(filename, ios::out, ios::app);
		}

		if (!file.is_open()) {
			cout << "Error: file does not exist." << endl;
		}
	} while (!file.is_open());
}

int main() {
	string run = "y"; // "y" if user chooses to enter another book
	string choice; // "y" if user chooses to view letter frequency
	char input; // stores input from file one character at a time
	Book book; // struct to hold book data
	fstream data; // input file
	ofstream output; // output file
	string splitName[2]; // output file also includes author first name and last name on individual lines, stored in this array

	while (run == "y"){

		// Prompt user for file
		openFile (data, true);
		
		// set author and title
		getline(data, book.title);
		getline(data, book.author);
		data.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore "Contents:" line

		// loop and get character, line and word counts
		while (!data.eof()) {
			data.get(input);

			// increment word count when space character is encountered, ignore extra spaces
			if (input == ' '){
				book.wordCount += 1;
				while (input == ' ') { // ignore any extra spaces between words
					data.get(input);
					if (data.eof()) {
						book.wordCount -= 1; // wordCount is incremented at the end of the file in case the file ends with no space, this is to compensate if it does
						break;
					}
					if (input == '\n') {
						book.wordCount -= 1; // wordCount is incremented when lineCount is incremented in case the line ends with no space, this is to compensate if it does
					}
				}
			}

			// increment lineCount when newline character is encountered
			if (input == '\n') {
				book.lineCount += 1;
				book.wordCount += 1; // assume no space at end of line, so increment wordCount by one as well
				data.get(input);
			}
			
			// increment letter frequency
			if (isalpha(input)) {
				input = tolower(input);
				int index = (int)input - (int)'a'; // index by ascii value zeroed at 'a'
				book.letterFrequency[index] += 1;
			}
		}

		data.close();

		book.wordCount += 1; // The final word doesn't necessarily have a space after it, so it needs to be counted (if it does, the wordCount is decremented in the loop to compensate)
		book.lineCount += 1; // Count the final line

		output.open(OUTPUT_FILENAME, ios::app);
		
		// Sum the entire book.letterFrequency array to find the total letter count to calculate percentages
		// this only calculates letter frequency as a percentage of all alpha characters, not including symbols, numbers etc. in totalChar
		double totalChar = accumulate(begin(book.letterFrequency), end(book.letterFrequency), 0, plus<unsigned>());
		
		// get first and last name separated to print to output file
		parseName(book.author, splitName);

		// Write everything to output file
		output << "Title: " << book.title << "\nAuthor full name: " << book.author << "\nAuthor first name: " << splitName[0] << "\nAuthor last name: " << splitName[1] 
			<< "\nLine Count: " << book.lineCount << "\nWord Count: " << book.wordCount << endl;

		output << endl; // blank line between entries
		output.close();

		cout << "Would you like to view the letter frequency? y/n ";
		choice = getString();

		if (choice == "y") {
			cout << book.title << " Letter Frequency:\n";
			for (int i = 0; i < LETTERS; i++) {
				cout << (char)(i + (int)'a') << ": " << (book.letterFrequency[i] * 100) / totalChar << "%" << endl; // for example, looks like "d: 5.25%"
			}
		}

		// clear book for next loop
		book = Book();

		cout << "Would you like to enter another book? y/n ";
		run = getString();
	}
	
	return 0;
}