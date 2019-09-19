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
	unsigned letterFrequency[LETTERS] = {0};
 };

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

void openFile(fstream& file, bool read) {
	string filename;

	do{
		cout << "Enter a filename: ";
		filename = getString();
		if (read) {
			file.open(filename, ios::in);
		}
		else {
			file.open(filename, ios::out);
		}

		if (!file.is_open()) {
			cout << "Error: file does not exist." << endl;
		}
	} while (!file.is_open());
}

int main() {
	string run = "y";
	char input;
	Book book;
	fstream data;
	ofstream output;

	while (run == "y"){

		// Prompt user for file
		openFile (data, true);
		
		// set author and title
		getline(data, book.title);
		getline(data, book.author);

		// loop and get character, line and word counts.
		while (!data.eof()) {
			data.get(input);

			// increment line count when newline character is encountered
			if (input == '\n') {
				book.lineCount += 1;
				data.get(input);
			}

			// increment word count when space character is encountered, ignore extra spaces
			if (input == ' '){
				book.wordCount += 1;
				while (input == ' ') {
					data.get(input);
				}
			}
			
			// increment letter frequency
			if (isalpha(input)) {
				input = tolower(input);
				int index = (int)input - (int)'a';
				book.letterFrequency[index] += 1;
			}


		}
		data.close();
		
		output.open(OUTPUT_FILENAME, ios::app);
		
		double totalChar = accumulate(begin(book.letterFrequency), end(book.letterFrequency), 0, plus<unsigned>());

		cout << "Title: " << book.author << "\nAuthor: " << book.title << "\nLine Count: " << book.lineCount << "\nWordCount: " << book.wordCount << endl;
		cout << "Letter Frequency:\n";
		for (int i = 0; i < LETTERS; i++) {
			cout << (char)(i + (int)'a') << ": " << (double)(book.letterFrequency[i] / totalChar) << endl;
		}
		
		book = Book();

		cout << "Would you like to enter another book? ";
		run = getString();
	}
	
	return 0;
}