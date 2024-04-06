#include <iostream>
#include <limits>
#include <random>
#include <string>

// ANSI escape codes for text color
#define BLUE_TEXT "\033[33m"
#define RED_TEXT "\033[31m"
#define GREEN_TEXT "\033[32m"
#define RESET_COLOR "\033[0m"

using namespace std;

struct Participant {
    int id;
    int lotteryNumbers[4];
    string mobileNumber;  // Added mobile number field
    Participant* prev;
    Participant* next;
};

class HashTable {
private:
    static const int TABLE_SIZE = 100;  // Size of the hash table
    Participant* table[TABLE_SIZE];     // Array of pointers to participants

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table[i] = nullptr;
        }
    }

    void insert(const string& key, Participant* participant) {
        int index = hashFunction(key);
        table[index] = participant;
    }

    Participant* search(const string& key) {
        int index = hashFunction(key);
        return table[index];
    }

    void remove(const string& key) {
        int index = hashFunction(key);
        table[index] = nullptr;
    }

private:
    // Simple hash function for demonstration purposes
    int hashFunction(const string& key) {
        int hash = 0;
        for (char c : key) {
            hash += c;
        }
        return hash % TABLE_SIZE;
    }
};

class LotterySystem {
private:
    Participant* head;
    Participant* tail;
    int participantCount;
    HashTable mobileNumberTable;  // Hash table for mobile numbers

public:
    LotterySystem() {
        head = nullptr;
        tail = nullptr;
        participantCount = 0;
    }

    void addParticipant() {
        Participant* newParticipant = new Participant;
        newParticipant->prev = nullptr;
        newParticipant->next = nullptr;

        // Generate a unique participant ID
        newParticipant->id = participantCount + 1;

        // Prompt the participant to enter their mobile number
        cout << BLUE_TEXT << "Enter participant " << newParticipant->id << "'s mobile number: " << RESET_COLOR << endl;
        cin >> newParticipant->mobileNumber;

        // Check if the mobile number is already used
        if (mobileNumberTable.search(newParticipant->mobileNumber) != nullptr) {
            cout << BLUE_TEXT << "Mobile number already used! Please enter a different mobile number." << RESET_COLOR << endl;
            delete newParticipant;
            return;
        }

        // Add the participant to the mobile number hash table
        mobileNumberTable.insert(newParticipant->mobileNumber, newParticipant);

        // Prompt the participant to enter their lottery numbers with input validation
        cout << BLUE_TEXT << "Enter participant " << newParticipant->id << "'s lottery numbers (between 0 and 5): " << RESET_COLOR << endl;

        for (int i = 0; i < 4; i++) {
            int inputNumber;
            do {
                cout << "Enter number " << i + 1 << ": ";
                cin >> inputNumber;

                if (cin.fail() || inputNumber < 0 || inputNumber > 4) {
                    cin.clear();  // Clear the error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
                    cout << BLUE_TEXT << "Invalid input! Please enter a number between 0 and 5." << RESET_COLOR << endl;
                }
                else {
                    newParticipant->lotteryNumbers[i] = inputNumber;
                    break;
                }
            } while (true);  // Repeat until valid input is received
        }

        if (checkIfAllNumbersMatch(newParticipant->lotteryNumbers)) {
            // Remove the participant from the mobile number hash table if all numbers match
            mobileNumberTable.remove(newParticipant->mobileNumber);
            delete newParticipant;  // Delete the new participant if all numbers match
            return;
        }

        // Insert the new participant into the linked list
        if (head == nullptr) {
            head = newParticipant;
            tail = newParticipant;
        }
        else {
            tail->next = newParticipant;
            newParticipant->prev = tail;
            tail = newParticipant;
        }

        participantCount++;
    }

    void deleteLast() {
        if (head == nullptr) {
            cout << "Something went wrong! Restart the program" << endl;
        }
        else {
            // Remove the participant from the mobile number hash table before deletion
            mobileNumberTable.remove(tail->mobileNumber);

            Participant* temp = tail;
            tail = temp->prev;
            delete temp;
            participantCount--;
        }
    }

    void generateLotteryNumbers() {
        srand(time(NULL));

        int lotteryNumbers[4];

        for (int i = 0; i < 4; i++) {
            int number;
            do {
                number = rand() % 5;  // Generate a random number between 0 and 99
            } while (checkNumberExists(number, lotteryNumbers));  // Keep generating random numbers until a unique number is found

            lotteryNumbers[i] = number;
        }

        // Display the generated lottery numbers
        cout << endl;
        cout << BLUE_TEXT << "Winning Lottery Numbers: " << RESET_COLOR << endl;
        cout << "--------------------------------" << endl;
        for (int i = 0; i < 4; i++) {
            cout << " | " << lotteryNumbers[i] << " | ";
        }
        cout << endl;
        cout << "--------------------------------" << endl;
        cout << endl;
    }

    bool checkNumberExists(int number, int lotteryNumbers[]) {
        for (int i = 0; i < 4; i++) {
            if (lotteryNumbers[i] == number) {
                return true;
            }
        }

        return false;
    }

    bool checkIfAllNumbersMatch(int lotteryNumbers[]) {
        Participant* currentParticipant = head;

        while (currentParticipant != nullptr) {
            bool allNumbersMatch = true;

            for (int i = 0; i < 4; i++) {
                if (lotteryNumbers[i] != currentParticipant->lotteryNumbers[i]) {
                    allNumbersMatch = false;
                    break;  // If any number doesn't match, break the loop
                }
            }

            if (allNumbersMatch) {
                cout << BLUE_TEXT << "LOTTERY YOU REQUESTED ALREADY SOLD OUT!!! TRY NEW ONE" << RESET_COLOR << endl;
                return true;  // Return true if all numbers match
            }

            currentParticipant = currentParticipant->next;
        }

        return false;  // Return false if no match is found
    }

    void printParticipantsAndLotteryNumbers() {
        srand(time(NULL));

        int lotteryNumbers[4];

        for (int i = 0; i < 4; i++) {
            int number;
            do {
                number = rand() % 5;  // Generate a random number between 0 and 99
            } while (checkNumberExists(number, lotteryNumbers));  // Keep generating random numbers until a unique number is found

            lotteryNumbers[i] = number;
        }

        // Display the generated lottery numbers
        cout << endl;
        cout << BLUE_TEXT << "Winning Lottery Numbers: " << RESET_COLOR << endl;
        cout << "--------------------------------" << endl;
        for (int i = 0; i < 4; i++) {
            cout << " | " << lotteryNumbers[i] << " | ";
        }
        cout << endl;
        cout << "--------------------------------" << endl;
        cout << endl;

        Participant* currentParticipant = head;

        while (currentParticipant != nullptr) {
            cout << "Participant ID: " << currentParticipant->id << endl;
            cout << "Mobile Number: " << currentParticipant->mobileNumber << endl;  // Display mobile number
            cout << "Lottery Numbers: ";
            int MatchingNumbers = 0;
            for (int i = 0; i < 4; i++) {
                if (lotteryNumbers[i] == currentParticipant->lotteryNumbers[i]) {
                    MatchingNumbers++;
                }
                cout << currentParticipant->lotteryNumbers[i] << " ";
            }
            cout << endl;
            cout << MatchingNumbers << " Numbers Were Matched " << endl;
            cout << endl;
            if (MatchingNumbers == 4) {
                cout << GREEN_TEXT << "Congratulations! You won Rs 50000" << RESET_COLOR << endl;
            }
            else if (MatchingNumbers == 3) {
                cout << GREEN_TEXT << "Congratulations! You won Rs 5000" << RESET_COLOR << endl;
            }
            else if (MatchingNumbers == 2) {
                cout << GREEN_TEXT << "Congratulations! You won Rs 200" << RESET_COLOR << endl;
            }
            else if (MatchingNumbers == 1) {
                cout << GREEN_TEXT << "Congratulations! You won Rs 40" << RESET_COLOR << endl;
            }
            else {
                cout << RED_TEXT << "Hard Luck! Try next time" << RESET_COLOR << endl;
            }
            cout << endl;
            cout << "------------------------------------------" << endl;

            currentParticipant = currentParticipant->next;
        }
    }
};

int main() {
    cout << endl;
    cout << BLUE_TEXT << "********" << RESET_COLOR << endl;
    cout << BLUE_TEXT << "HII....WELCOME TO THE LOTTERY MANAGEMENT SYSTEM....." << RESET_COLOR << endl;
    cout << endl << endl;
    cout << BLUE_TEXT << "INSTRUCTIONS:-" << RESET_COLOR << endl;
    cout << "  #" << "Only one number is matched      -------- Rs 40" << endl;
    cout << "  #" << "Only two numbers are matched    -------- Rs 200" << endl;
    cout << "  #" << "Only three numbers are matched  -------- Rs 5000" << endl;
    cout << "  #" << "All numbers are matched         -------- Rs 50000" << endl << endl;
    cout << "  #" << "You can choose numbers between 0-5" << endl;

    cout << endl << endl;

    LotterySystem system;

    // Add participants and their lottery numbers
    bool continueAdding = true;
    while (continueAdding) {
        system.addParticipant();

        cout << BLUE_TEXT << "Do you want to add another participant? (Y/N): " << RESET_COLOR << endl;
        char input;
        cin >> input;

        continueAdding = (input == 'Y' || input == 'y');
    }

    // Generate lottery numbers
    // system.generateLotteryNumbers();

    // Display participants and their lottery numbers
    system.printParticipantsAndLotteryNumbers();

    return 0;
}