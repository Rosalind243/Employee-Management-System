#include <iostream>
#include "Employee.h"
#include <fstream>
#include <sstream>
#include <string>
#include "Date.h"
#include <regex>
#include <limits>
#include <algorithm> 
#include <vector>
// Global variables
#define MAX_NO_OF_RECORDS 51

Employee EmployeeRecordsDB [MAX_NO_OF_RECORDS];

/* Keeps track of the actual number of records
   empDBSize's value MUST BE UPDATED WHENEVER an employee record is : 
   - inserted into EmployeeRecordsDB  OR 
   - deleted from EmployeeRecordsDB
*/

int empDBSize = 0; 

// Function prototypes
void printWelcomeMsg();
int printMainMenuOptions();
int queryCurrentNoOfEmpRecs();
void readFromCSVFile();
void displayAllRecords();
void searchByIC();
void searchByName();
void searchByEmail();
void searchByPhoneNumber();
void insertNewEmployeeRecord();
void Update();
void vaildation();
void Delete();
void writeDataToFile();

using namespace std;


class invalidDates {
	public:
		string ic;
		string name;
		Date birthDate;
		Date hiredDate;
};

class duplicateIC {
	public:
		string ic;
		string name;
};

bool isIndexColumn (const string& value)
{
 	for (char c : value) {
 		 if (!isdigit(c) )return false;
 	}
 	return true;
	
} 

// Function to validate a date and provide detailed error messages
bool isValidDate(int day, int month, int year, string& errorMessage) {


    // Check days in month
    if (month == 2) {
        // Leap year check for February
        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeapYear) {
            if (day < 1 || day > 29) {
                errorMessage = "Day value > 29 for month = 2 in leap year! (Expected range is 1-29)";
                return false;
            }
        } else {
            if (day < 1 || day > 28) {
                errorMessage = "Day value > 28 for month = 2 in non-leap year! (Expected range is 1-28)";
                return false;
            }
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day < 1 || day > 30) {
            errorMessage = "Day value > 30 for month = " + to_string(month) + "! (Expected range is 1-30)";
            return false;
        }
    }

    return true; // Date is valid
}

// Function to input and validate a date
Date inputDate(const string& prompt) {
    int day, month, year;
    char delimiter;
    string errorMessage;
	regex datePattern (R"(^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-(\d{4})$)");//matches date pattern dd-mm-yyyy
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input); // Use getline to allow spaces in input

        // Check for unwanted characters (excluding spaces)
        bool hasUnwantedChars = false;
        for (char c : input) {
            if (!isdigit(c) && c != '-') { // Allow digits and '-'
                hasUnwantedChars = true;
                break;
            }
        }

        if (hasUnwantedChars) {
            cout << "Error: Input contains unwanted characters. Please try again." << endl;
            continue; // Prompt the user to re-enter the input
        }
        
        if(!regex_match(input, datePattern)){
        	cout << "Error: Invalid date format. Please try again." << endl;
        	continue;
        }

        // Replace '/' with '-' for compatibility
        // replace(input.begin(), input.end(), '/', '-');

        stringstream ss(input);
        if (ss >> day >> delimiter >> month >> delimiter >> year) {
            if (delimiter == '-' && isValidDate(day, month, year, errorMessage)) {
                return Date(day, month, year);
            } else {
                cout << errorMessage << endl;
            }
        } else {
            cout << "Invalid date format. Please enter the date in the format dd-mm-yyyy." << endl;
        }
    }
}

int main() 
{
    printWelcomeMsg();

    int userEnteredNumber;

    do 
    {
        userEnteredNumber = printMainMenuOptions();
        cout << endl;

        switch (userEnteredNumber) {
            case 1:
             
                cout << "-------------------------------------------------------------------------------------" << endl;
                cout << "Get current no. of records in Employee Database" <<endl;
                cout << "-------------------------------------------------------------------------------------" << endl;
                cout << "Current no. of Employee Records in Database : " << queryCurrentNoOfEmpRecs() << endl;
               
                break;

            case 2:
                cout << "-------------------------------------------------------------------------------------" << endl;
                cout << "Reading data from file to populate Employee DB ..." <<endl;
                cout << "-------------------------------------------------------------------------------------" << endl;
                readFromCSVFile(); // Call the function to read from CSV file
                
                break;

            case 3:
                displayAllRecords(); // Call the function to display all records
                break;

            case 4:
                searchByIC();
                break;

            case 5:
                searchByName(); // Call the function to search by Name
                break;

            case 6:
                searchByEmail(); // Call the function to search by Email
                break;

            case 7:
                searchByPhoneNumber(); // Call the function to search by Phone Number
                break;

            case 8:
                insertNewEmployeeRecord();
                break;

            case 9:
                Update();
                break;

            case 10:
                Delete();
                break;

            case 11:
                writeDataToFile();
                break;

            case 12:
                break;

            default:
                cout << endl << "Invalid Choice => " << userEnteredNumber << ", please try again!" << endl;        }

      	// Clear the input buffer to remove any leftover input
        //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

       // cout << endl;


    } while (userEnteredNumber != 12);

    cout << "\nThank you for using this program, have a nice day!" << endl;

    return 0;
}

// Function to print the welcome message
void printWelcomeMsg() {
    cout << "\n------------------------------------------------------" << endl;
    cout << "- Welcome to my Assn Program! -" << endl;
    cout << "------------------------------------------------------" << endl;
    cout << "- Student Number : XXXXXXX " << endl;
    cout << "- Student Name   : XXX " << endl;
    cout << "------------------------------------------------------" << endl;


}

// Function to print the main menu and get user input
int printMainMenuOptions() {
    string input;
    regex validChoicePattern(R"(^(1[0-2]|[1-9])$)"); // Corrected regex to match numbers 1-12

    int userEnteredNumber;

    while (true) {
        cout << "\n-----------------------------------------------------" << endl;
        cout << "\n 1) Get current no. of records in Employee Database" << endl;
        cout << " 2) Read Data From File and Populate Employee DB" << endl;
        cout << " 3) Display All Records" << endl;
        cout << " 4) Search for records by IC" << endl;
        cout << " 5) Search for records by Name" << endl;
        cout << " 6) Search for records by Email" << endl;
        cout << " 7) Search for records by Phone Number" << endl;
        cout << " 8) INSERT New Employee Record" << endl;
        cout << " 9) Search (by ic) & UPDATE Existing Employee Record" << endl;
        cout << "10) Search (by ic) & DELETE Existing Employee Record" << endl;
        cout << "11) Write Data From Employee DB To File" << endl;
        cout << "12) Quit" << endl;
        cout << "\n-----------------------------------------------------" << endl;
        cout << endl;

        cout << "Please enter your choice (1 - 12): ";
        getline(cin, input); // Read the entire input line

        // Check if input is empty
        if (input.empty()) {
            cout << "Invalid input. Please enter a number between 1 and 12." << endl;
            continue;
        }

        // Validate input using regex
        if (regex_match(input, validChoicePattern)) {
            userEnteredNumber = stoi(input); // Convert valid input (string) to integer
            break; // Exit the loop if input is valid
        } else {
            cout << "Invalid input. Please enter a number between 1 and 12." << endl;
        }
    }

    return userEnteredNumber;
}

int queryCurrentNoOfEmpRecs () 
{ 
  return (empDBSize);

}   // end queryCurrentNoOfEmpRecs () ...


// Function to read data from a CSV file
void readFromCSVFile() {
	string idx, name, email, ic, phoneNum, hireDateStr, birthDateStr;
    string fileName;
    string line;
    bool skipFirstColumn;
    ifstream file;
    vector<invalidDates> invalidDates;
    vector<duplicateIC> duplicateIC;
    do{
    	do{
     		cout << "Enter the input (CSV) file's name : ";
    		getline(cin,fileName);
    		
    		if (fileName.empty()){
    			cout<<"Enter a filename"<<endl;
    		}
    	}while(fileName.empty());
    
 
    file.open(fileName); // Open the file
    

    if (!file.is_open()) {
        cerr << "Error: Could not open the file!" << endl; 
    }
    
    }while(!file.is_open());

    // Skip the header line
    getline(file, line);
	

    while (getline(file, line) && (empDBSize < MAX_NO_OF_RECORDS)) {
        stringstream ss(line);
        // Read csv values separate by commas
    	getline(ss, idx, ',');
    	skipFirstColumn = isIndexColumn(idx);
		if (skipFirstColumn){
       		getline(ss, name, ','); //If first column is Idx, skip
      	}
      	else {
       		name = idx; //Else take first column
      	}
        // Parse the line

        getline(ss, email, ',') &&
        getline(ss, ic, ',') &&
        getline(ss, phoneNum, ',') &&
        getline(ss, hireDateStr, ',') &&
        getline(ss, birthDateStr);

        // Parse hireDateStr and birthDateStr into Date objects
        int hireDay, hireMonth, hireYear;
        int birthDay, birthMonth, birthYear;
        char delimiter;

        // Parse hireDateStr (assuming format: DD/MM/YYYY)
        stringstream hireDateSS(hireDateStr);
        hireDateSS >> hireDay >> delimiter >> hireMonth >> delimiter >> hireYear;

        // Parse birthDateStr (assuming format: DD/MM/YYYY)
        stringstream birthDateSS(birthDateStr);
        birthDateSS >> birthDay >> delimiter >> birthMonth >> delimiter >> birthYear;
		
        // Create Date objects
        Date hireDate(hireDay, hireMonth, hireYear);
        Date birthDate(birthDay, birthMonth, birthYear);
		
		 // **Check if hire date is at least 18 years after birth date**
		bool isHireDateValid = true;

		// Calculate age at hire date
		int ageAtHire = hireYear - birthYear;
		bool dupe = false;
		if (empDBSize != 0){
			for (int i = 0; i < empDBSize; i++){
				if (ic == EmployeeRecordsDB[i].getIC()){ //If have same IC, skip person to include
					duplicateIC.push_back({ic,name});
					dupe = true;
				}
				else if(i == (empDBSize-1)){
					break;
				}
			}
		}
		if (!dupe){
			// Adjust for month/day if hire date hasn't occurred yet in the birth year
			if (hireMonth < birthMonth || (hireMonth == birthMonth && hireDay < birthDay)) {
				ageAtHire--; // Subtract 1 year if birthday hasn't passed yet
			}

			if (ageAtHire < 18) {
				isHireDateValid = false;
			}
			
			// Create an Employee object and add it to the database
			if (isHireDateValid) {
				EmployeeRecordsDB[empDBSize] = Employee(name, email, ic, phoneNum, hireDate, birthDate);
				empDBSize++; // Update the number of records in the databases
	  		}
	  		else {
	  			invalidDates.push_back({ic, name, Date(birthDay, birthMonth, birthYear), Date(hireDay, hireMonth, hireYear)});
	  		}
  		}
	}
    file.close();

    if (empDBSize == MAX_NO_OF_RECORDS) {
        cout << "Maximum number of records reacched. Ignoring additional data from file " <<fileName<< " !!! " << endl;
    }

    cout << "Done! Total no. of records read in and stored in DB : " << empDBSize << endl;
	
	// Lists all invalid Employees not included into DB
	if (!invalidDates.empty()){
		cout << "Please take note, the following employees were not added due to incorrect hire dates of not being 18 years old :" << endl;
		cout << left << setw(EMP_IC_LENGTH) << "IC" << setw(EMP_NAME_LENGTH) << "Name" << setw(EMP_DATE_LENGTH) << "Birth Date" << setw(EMP_DATE_LENGTH) << "Hired Date" << endl;
		for (const auto& invalidDates : invalidDates) {
			cout << left << setw(EMP_IC_LENGTH) << invalidDates.ic << setw(EMP_NAME_LENGTH) << invalidDates.name << setw(EMP_DATE_LENGTH) << invalidDates.birthDate.toString() << setw(EMP_DATE_LENGTH) << invalidDates.hiredDate.toString() << endl;
		}
		invalidDates.clear();
		invalidDates.shrink_to_fit(); 
		cout << endl;
	}
	if (!duplicateIC.empty()){
		cout << "Please take note, the following employees were not added due to having the same IC as an employee :" << endl;
		cout << left << setw(EMP_IC_LENGTH) << "IC" << setw(EMP_NAME_LENGTH) << "Name" << endl;
		for (const auto& duplicateIC : duplicateIC) {
			cout << left << setw(EMP_IC_LENGTH) << duplicateIC.ic << setw(EMP_NAME_LENGTH) << duplicateIC.name << endl;
		}
		duplicateIC.clear();
		duplicateIC.shrink_to_fit();
		cout << endl;
	}
}

void displayAllRecords() {
    if (empDBSize == 0) {
        cout << "No records found in the database." << endl;
        return;
    }

    string showIndex;
    bool validInput = false;

    // Loop until valid input is received
    while (!validInput) {
        cout << "Show array index? {y/n} : ";
        getline(cin, showIndex);
      

        // Check if the input is valid (either 'y', 'Y', 'n', or 'N')
        if (showIndex == "y" || showIndex == "Y" || showIndex == "n" || showIndex == "N") {
            validInput = true; // Exit the loop
        } 
        else{
        	cout << "Invalid input. Please enter 'y' or 'n'." << endl;
        }
    }

    // Display the table header
    cout << right << setw(80) << "All Employee Records (total = " << empDBSize << ") ..." << endl<<endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    if (showIndex == "y" || showIndex == "Y") {
        // Display header with ID column
        cout << left << setw(EMP_IDX_LENGTH) << "ID" 
             << setw(EMP_IC_LENGTH) << "IC" 
             << setw(EMP_NAME_LENGTH) << "Name" 
             << setw(EMP_TEL_LENGTH) << "Phone" 
             << setw(EMP_DATE_LENGTH) << "Birth Date" 
             << setw(EMP_DATE_LENGTH) << "Hired Date" 
             << setw(EMP_EMAIL_LENGTH) << "Email" << endl;
    } else {
        // Display header without ID column
        cout << left << setw(EMP_IC_LENGTH) << "IC" 
             << setw(EMP_NAME_LENGTH) << "Name" 
             << setw(EMP_TEL_LENGTH) << "Phone" 
             << setw(EMP_DATE_LENGTH) << "Birth Date" 
             << setw(EMP_DATE_LENGTH) << "Hired Date" 
             << setw(EMP_EMAIL_LENGTH) << "Email" << endl;
    }

    cout << "-----------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    // Display each record
    for (int i = 0; i < empDBSize; i++) {
        Employee emp = EmployeeRecordsDB[i];

        if (showIndex == "y" || showIndex == "Y") {
            // Display record with ID column
            cout << left << setw(EMP_IDX_LENGTH) << i;
        }

        // Display employee details
        cout << left << setw(EMP_IC_LENGTH) << emp.getIC() 
             << setw(EMP_NAME_LENGTH) << emp.getName() 
             << setw(EMP_TEL_LENGTH) << emp.getPhoneNum() 
             << setw(EMP_DATE_LENGTH) << emp.getBirthDate().toString() 
             << setw(EMP_DATE_LENGTH) << emp.getHiredDate().toString() 
             << setw(EMP_EMAIL_LENGTH) << emp.getEmail() << endl;
    }

    cout << "-----------------------------------------------------------------------------------------------------------------------------------------------" << endl;
}
void searchByIC() {
    string icToSearch ="00"; // Initialize the IC value
    string input;
    regex validChoicePattern(R"(^[1-4]$)"); // Regex to match numbers 1-4
    regex specialChar(R"([^a-zA-Z0-9])"); //matches all special characters
    int subChoice;

    do {
        // Display the sub-menu with the current IC value
 		cout << "-------------------------------------------------------------------------------" << endl;
		cout << "1) Enter an IC " << right << setw(23) << "(current value = '" << icToSearch << "')" << endl;
        cout << "2) Search for employee IC"<< right << setw(13) <<  "= '" << icToSearch << "' (i.e. EXACT MATCH)" << endl;
        cout << "3) Search for employee IC containing '" << icToSearch << "' (i.e. PARTIAL MATCH)" << endl;
        cout << "4) Back to main menu" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        cout << "Please enter your choice (1 - 4): ";
        getline(cin, input); // Read the entire input line

          // Validate sub-menu choice using regex
        if (!regex_match(input, validChoicePattern)) {
            cout << "Invalid input. Please enter a number between 1 and 4." << endl;
            continue; // Restart the loop
        }
        subChoice = stoi(input); // Convert valid input to integer
		
        switch (subChoice) {
            case 1: {
				cout << "Please type in IC value to search for (< 10 chars) : ";
					while (true) {
						getline(cin, icToSearch);
						cout << endl;
						if (icToSearch.empty()) {
							cout << "Input cannot be empty. Please enter data again (< 10 chars) : " ;
						}
						else if (icToSearch.length() >= 10) {
							cout << "Input too long. Please enter data again (< 10 chars) : ";
						}
						else if (regex_search(icToSearch, specialChar)){
				   			cout << "Input should not have any unwanted character, please enter data again (< 10 chars) : ";
				   		}
						else {
							break;  // Exit loop only when valid input is received
						}
					}  // Infinite loop until break
				break;
            }
            case 2: {
                // Exact match search
                bool found = false;
                int count = 0;
                regex exactMatchPattern("^" + icToSearch + "$", regex_constants::icase); // Case-insensitive exact match
                for (int i = 0; i < empDBSize; i++) {
                    
                    	if (regex_match(EmployeeRecordsDB[i].getIC(), exactMatchPattern)) {
		                    if (!found) {
		                        cout << "\nRecords with IC EXACTLY matching '" << icToSearch << "':" << endl;
		                        cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
		                        cout << left << setw(EMP_IC_LENGTH) << "IC" 
		                             << setw(EMP_NAME_LENGTH) << "Name" 
		                             << setw(EMP_TEL_LENGTH) << "Phone" 
		                             << setw(EMP_DATE_LENGTH) << "Birth Date" 
		                             << setw(EMP_DATE_LENGTH) << "Hired Date" 
		                             << setw(EMP_EMAIL_LENGTH) << "Email" << endl;
		                        cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
		                        found = true;
		                    }
		                    Employee emp = EmployeeRecordsDB[i];
		                    cout << left << setw(EMP_IC_LENGTH) << emp.getIC() 
		                         << setw(EMP_NAME_LENGTH) << emp.getName() 
		                         << setw(EMP_TEL_LENGTH) << emp.getPhoneNum() 
		                         << setw(EMP_DATE_LENGTH) << emp.getBirthDate().toString() 
		                         << setw(EMP_DATE_LENGTH) << emp.getHiredDate().toString() 
		                         << setw(EMP_EMAIL_LENGTH) << emp.getEmail() << endl;
		                    count++;
		                }
                }	
                if (!found) {
                    cout << "\nNo matching records found !!!" << endl;
                } else {
                    cout << "\n" << count << " records found, with IC EXACTLY matching " << icToSearch << " !!" << endl;
                }
                break;
            }

            case 3: {
                // Partial match search
                bool found = false;
                int count = 0;
                regex partialMatchPattern(icToSearch, regex_constants::icase); // Case-insensitive partial match
                for (int i = 0; i < empDBSize; i++) {
                    if (regex_search(EmployeeRecordsDB[i].getIC(), partialMatchPattern)) {
                        if (!found) {
                            cout << "\nRecords with IC PARTIALLY matching '" << icToSearch << "':" << endl;
             				cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                            cout << left << setw(EMP_IC_LENGTH) << "IC" 
                                 << setw(EMP_NAME_LENGTH) << "Name" 
                                 << setw(EMP_TEL_LENGTH) << "Phone" 
                                 << setw(EMP_DATE_LENGTH) << "Birth Date" 
                                 << setw(EMP_DATE_LENGTH) << "Hired Date" 
                                 << setw(EMP_EMAIL_LENGTH) << "Email" << endl;
                            cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                            found = true;
                        }
                        Employee emp = EmployeeRecordsDB[i];
                        cout << left << setw(EMP_IC_LENGTH) << emp.getIC() 
                             << setw(EMP_NAME_LENGTH) << emp.getName() 
                             << setw(EMP_TEL_LENGTH) << emp.getPhoneNum() 
                             << setw(EMP_DATE_LENGTH) << emp.getBirthDate().toString() 
                             << setw(EMP_DATE_LENGTH) << emp.getHiredDate().toString() 
                             << setw(EMP_EMAIL_LENGTH) << emp.getEmail() << endl;
                        count++;
                    }
                }
                if (!found) {
                    cout << "\nNo matching records found !!!" << endl;
                } else {
                    cout << "\n" << count << " records found, with IC PARTIALLY matching " << icToSearch << " !!" << endl;
                }
                break;
            }

            case 4:
                // Return to main menu
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
                //continue; // Restart the loop
               
        }
    } while (subChoice != 4); // Loop until the user chooses to go back to the main menu
}

// Function to search for records by Name
void searchByName() {
    string nameToSearch = "SAMPLE NAME"; // Initialize the name value
    string input;
    regex validChoicePattern(R"(^[1-4]$)"); // Regex to match numbers 1-4
    regex specialChar(R"([^a-zA-Z])"); //matches all special characters
    
    int subChoice;

    do {
        // Display the sub-menu with the current name value
        cout << "-------------------------------------------------------------------------------" << endl;
        cout << "1) Enter a Name" << right << setw(25) << "(current value = '" << nameToSearch << "')" << endl;
        cout << "2) Search for employee name"<< right << setw(13) <<  "= '" << nameToSearch << "' (i.e. EXACT MATCH)" << endl;
        cout << "3) Search for employee name containing '" << nameToSearch << "' (i.e. PARTIAL MATCH)" << endl;
        cout << "4) Back to main menu" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        cout << "Please enter your choice (1 - 4): ";
        getline(cin, input); // Read the entire input line
        
        

        // Validate sub-menu choice using regex
        if (!regex_match(input, validChoicePattern)) {
            cout << "Invalid input. Please enter a number between 1 and 4." << endl;
            continue; // Restart the loop
        }

        subChoice = stoi(input); // Convert valid input to integer

        switch (subChoice) {
            case 1: {
            	cout << "Please type in name value to search for (< 35 chars) :  ";
				while(true) {
					getline(cin, nameToSearch);
					cout << endl;
					if (nameToSearch.empty()) {
						cout << "Input should not be empty. Please enter valid data (< 35 chars) : ";
					}
					else if(nameToSearch.length() >= 35){
					    cout << "Input too long. Please enter data again (< 35 chars) : ";
					}
					else if (regex_search(nameToSearch, specialChar)){
               			cout << "Input should not have any unwanted character. Please enter data again (< 35 chars) : ";
               		}
					else {
						break;  // Exit loop only when valid input is received
					}
				}
				break;
            }
            case 2: {
                // Exact match search
                bool found = false;
                int count = 0;
                regex exactMatchPattern("^" + nameToSearch + "$", regex_constants::icase); // Case-insensitive exact match

                for (int i = 0; i < empDBSize; i++) {
                    if (regex_match(EmployeeRecordsDB[i].getName(), exactMatchPattern)) {
                        if (!found) {
                            cout << "\nRecords with Name EXACTLY matching '" << nameToSearch << "':" << endl;
                            cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                            cout << left << setw(EMP_IC_LENGTH) << "IC" 
                                 << setw(EMP_NAME_LENGTH) << "Name" 
                                 << setw(EMP_TEL_LENGTH) << "Phone" 
                                 << setw(EMP_DATE_LENGTH) << "Birth Date" 
                                 << setw(EMP_DATE_LENGTH) << "Hired Date" 
                                 << setw(EMP_EMAIL_LENGTH) << "Email" << endl;
                            cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                            found = true;
                        }
                        Employee emp = EmployeeRecordsDB[i];
                        cout << left << setw(EMP_IC_LENGTH) << emp.getIC() 
                             << setw(EMP_NAME_LENGTH) << emp.getName() 
                             << setw(EMP_TEL_LENGTH) << emp.getPhoneNum() 
                             << setw(EMP_DATE_LENGTH) << emp.getBirthDate().toString() 
                             << setw(EMP_DATE_LENGTH) << emp.getHiredDate().toString() 
                             << setw(EMP_EMAIL_LENGTH) << emp.getEmail() << endl;
                    }
                }
                if (!found) {
                    cout << "\nNo matching records found !!!" << endl;
                }
                break;
            }

            case 3: {
                // Partial match search
		        bool found = false;
                int count = 0;
                regex partialMatchPattern(nameToSearch, regex_constants::icase); // Case-insensitive partial match

                for (int i = 0; i < empDBSize; i++) {
                    if (regex_search(EmployeeRecordsDB[i].getName(), partialMatchPattern)) {
                        if (!found) {
                            cout << "\nRecords with Name PARTIALLY matching '" << nameToSearch << "':" << endl;
                            cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                            cout << left << setw(EMP_IC_LENGTH) << "IC" 
                                 << setw(EMP_NAME_LENGTH) << "Name" 
                                 << setw(EMP_TEL_LENGTH) << "Phone" 
                                 << setw(EMP_DATE_LENGTH) << "Birth Date" 
                                 << setw(EMP_DATE_LENGTH) << "Hired Date" 
                                 << setw(EMP_EMAIL_LENGTH) << "Email" << endl;
                            cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                            found = true;
                        }
                        Employee emp = EmployeeRecordsDB[i];
                        cout << left << setw(EMP_IC_LENGTH) << emp.getIC() 
                             << setw(EMP_NAME_LENGTH) << emp.getName() 
                             << setw(EMP_TEL_LENGTH) << emp.getPhoneNum() 
                             << setw(EMP_DATE_LENGTH) << emp.getBirthDate().toString() 
                             << setw(EMP_DATE_LENGTH) << emp.getHiredDate().toString() 
                             << setw(EMP_EMAIL_LENGTH) << emp.getEmail() << endl;
                        count++;
                    }
                }
                if (!found) {
                    cout << "\nNo matching records found !!!" << endl;
                } else {
                    cout << "\n" << count << " records found, with Name PARTIALLY matching " << nameToSearch << " !!" << endl;
                }
                break;
            }

            case 4:
                // Return to main menu
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
               
        }
    } while (subChoice != 4); // Loop until the user chooses to go back to the main menu
}


// Function to search for records by Email
void searchByEmail() {
    string emailToSearch = "SAMPLE@GMAIL.COM"; // Initialize the email value
    string input;
    regex validChoicePattern(R"(^[1-4]$)"); // Regex to match numbers 1-4
    int subChoice;
    regex spacePattern(R"(\s+)");// Regex to match space

    do {
    	 // Display the sub-menu with the current email value
    	cout << "-------------------------------------------------------------------------------" << endl;
        cout << "1) Enter an Email " << right << setw(23) << "(current value = '" << emailToSearch  << "')" << endl;
        cout << "2) Search for employee email"<< right << setw(13) <<  "= '" << emailToSearch << "' (i.e. EXACT MATCH)" << endl;
        cout << "3) Search for employee email containing '" << emailToSearch << "' (i.e. PARTIAL MATCH)" << endl;
        cout << "4) Back to main menu" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        cout << "Please enter your choice (1 - 4): ";
        getline(cin, input); // Read the entire input line

        // Validate sub-menu choice using regex
        if (!regex_match(input, validChoicePattern)) {
            cout << "Invalid input. Please enter a number between 1 and 4." << endl;
            continue; // Restart the loop
        }
        subChoice = stoi(input); // Convert valid input to integer
        switch (subChoice) {
            case 1: {
            	cout << "Please type in Email value to search for (< 35 chars) : ";
            	while(true) {	
					getline(cin, emailToSearch);
					cout << endl;
					if (emailToSearch.empty()) {
						cout << "Input cannot be empty. Please enter data again (< 35 chars) : ";
					}
					else if(emailToSearch.length() >= 35){
						cout << "Input too long. Please enter data again (< 35 chars) : ";
					}
					else if(regex_match(emailToSearch, spacePattern)){
						cout << "Input cannot have a spacing. Please enter data again (< 35 chars) : ";
					}
					else {
						break;  // Exit loop only when valid input is received
					}
				}
           		break;      
            }
            case 2: {
                // Exact match search
                bool found = false;
                regex exactMatchPattern("^" + emailToSearch + "$", regex_constants::icase); // Case-insensitive exact match

                for (int i = 0; i < empDBSize; i++) {
                    if (regex_match(EmployeeRecordsDB[i].getEmail(), exactMatchPattern)) {
                        if (!found) {
                            cout << "\nRecords with Email EXACTLY matching '" << emailToSearch << "':" << endl;
                            cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                            cout << left << setw(EMP_IC_LENGTH) << "IC" 
                                 << setw(EMP_NAME_LENGTH) << "Name" 
                                 << setw(EMP_TEL_LENGTH) << "Phone" 
                                 << setw(EMP_DATE_LENGTH) << "Birth Date" 
                                 << setw(EMP_DATE_LENGTH) << "Hired Date" 
                                 << setw(EMP_EMAIL_LENGTH) << "Email" << endl;
                            cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                            found = true;
                        }
                        Employee emp = EmployeeRecordsDB[i];
                        cout << left << setw(EMP_IC_LENGTH) << emp.getIC() 
                             << setw(EMP_NAME_LENGTH) << emp.getName() 
                             << setw(EMP_TEL_LENGTH) << emp.getPhoneNum() 
                             << setw(EMP_DATE_LENGTH) << emp.getBirthDate().toString() 
                             << setw(EMP_DATE_LENGTH) << emp.getHiredDate().toString() 
                             << setw(EMP_EMAIL_LENGTH) << emp.getEmail() << endl;
                    }
                }
                if (!found) {
                    cout << "\nNo matching records found !!!" << endl;
                }
                break;
            }

            case 3: {
                // Partial match search
                bool found = false;
                int count = 0;
                regex partialMatchPattern(emailToSearch, regex_constants::icase); // Case-insensitive partial match

                for (int i = 0; i < empDBSize; i++) {
                    if (regex_search(EmployeeRecordsDB[i].getEmail(), partialMatchPattern)) {
                        if (!found) {
                            cout << "\nRecords with Email PARTIALLY matching '" << emailToSearch << "':" << endl;
                            cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                            cout << left << setw(EMP_IC_LENGTH) << "IC" 
                                 << setw(EMP_NAME_LENGTH) << "Name" 
                                 << setw(EMP_TEL_LENGTH) << "Phone" 
                                 << setw(EMP_DATE_LENGTH) << "Birth Date" 
                                 << setw(EMP_DATE_LENGTH) << "Hired Date" 
                                 << setw(EMP_EMAIL_LENGTH) << "Email" << endl;
                            cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                            found = true;
                        }
                        Employee emp = EmployeeRecordsDB[i];
                        cout << left << setw(EMP_IC_LENGTH) << emp.getIC() 
                             << setw(EMP_NAME_LENGTH) << emp.getName() 
                             << setw(EMP_TEL_LENGTH) << emp.getPhoneNum() 
                             << setw(EMP_DATE_LENGTH) << emp.getBirthDate().toString() 
                             << setw(EMP_DATE_LENGTH) << emp.getHiredDate().toString() 
                             << setw(EMP_EMAIL_LENGTH) << emp.getEmail() << endl;
                        count++;
                    }
                }
                if (!found) {
                    cout << "\nNo matching records found !!!" << endl;
                } else {
                    cout << "\n" << count << " records found, with Email PARTIALLY matching " << emailToSearch << " !!" << endl;
                }
                break;
            }

            case 4:
                // Return to main menu
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
                
        }
    } while (subChoice != 4); // Loop until the user chooses to go back to the main menu
}

// Function to escape special regex characters in a string
string escapeRegex(const string& input) {
    static const regex specialChars{R"([-[\]{}()*+?.,\^$|#\s])"};
    return regex_replace(input, specialChars, R"(\$&)");
}
// Function to search for records by Phone Number
void searchByPhoneNumber() {

    string phoneToSearch = "(xx)-xxxxxxxx"; // Initialize the phone number value
    string input;
    regex validChoicePattern(R"(^[1-4]$)"); // Regex to match numbers 1-4
    regex phoneNumberPattern(R"(^[0-9\-\(\)]+$)");
    int subChoice;

    do {
        // Display the sub-menu with the current phone number value
        cout << "-------------------------------------------------------------------------------" << endl;
        cout << "1) Enter a phone number " << right << setw(24) << "(current value = '" << phoneToSearch << "')" << endl;
        cout << "2) Search for employee phone number" << right << setw(13) << "= '" << phoneToSearch << "' (i.e. EXACT MATCH)" << endl;
        cout << "3) Search for employee phone number containing '" << phoneToSearch << "' (i.e. PARTIAL MATCH)" << endl;
        cout << "4) Back to main menu" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        cout << "Please enter your choice (1 - 4): ";
        getline(cin, input);

        // Validate sub-menu choice using regex
        if (!regex_match(input, validChoicePattern)) {
            cout << "Invalid input. Please enter a number between 1 and 4." << endl;
            continue;
        }

        subChoice = stoi(input);

        switch (subChoice) {
            case 1: {
                bool isValidPhoneNumber = false;
                cout << "Please type in phone number value to search for (< 16 chars) : ";
                while (!isValidPhoneNumber) {
                    getline(cin, phoneToSearch);
                    cout << endl;
                    if (phoneToSearch.empty()) {
						cout << "Input cannot be empty. Please enter data again (< 16 chars) : ";
					}
                    else if (phoneToSearch.length() >= 16) {
					    cout << "Input too long. Please enter data again (< 16 chars) : ";
					}
                	else if (!regex_match(phoneToSearch, phoneNumberPattern)) {
                        cout << "Input should not have any unwanted character. Please enter data again (< 16 chars) : ";
                    }
                    else{
                    	isValidPhoneNumber = true;
                    }
                }
                break;
            }

            case 2: {
                // Exact match search
                bool found = false;
                int count = 0;
                string escapedPattern = "^" + escapeRegex(phoneToSearch) + "$";
                regex exactMatchPattern(escapedPattern, regex_constants::icase);

                for (int i = 0; i < empDBSize; i++) {
                    if (regex_match(EmployeeRecordsDB[i].getPhoneNum(), exactMatchPattern)) {
                        if (!found) {
                            cout << "\nRecords with Phone Number EXACTLY matching '" << phoneToSearch << "':" << endl;
                            cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                            cout << left << setw(EMP_IC_LENGTH) << "IC" 
                                 << setw(EMP_NAME_LENGTH) << "Name" 
                                 << setw(EMP_TEL_LENGTH) << "Phone" 
                                 << setw(EMP_DATE_LENGTH) << "Birth Date" 
                                 << setw(EMP_DATE_LENGTH) << "Hired Date" 
                                 << setw(EMP_EMAIL_LENGTH) << "Email" << endl;
                            cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                            found = true;
                        }
                        Employee emp = EmployeeRecordsDB[i];
                        cout << left << setw(EMP_IC_LENGTH) << emp.getIC() 
                             << setw(EMP_NAME_LENGTH) << emp.getName() 
                             << setw(EMP_TEL_LENGTH) << emp.getPhoneNum() 
                             << setw(EMP_DATE_LENGTH) << emp.getBirthDate().toString() 
                             << setw(EMP_DATE_LENGTH) << emp.getHiredDate().toString() 
                             << setw(EMP_EMAIL_LENGTH) << emp.getEmail() << endl;
                        count++;
                    }
                }
                if (!found) {
                    cout << "\nNo matching records found !!!" << endl;
                } else {
                    cout << "\n" << count << " records found with Phone Number EXACTLY matching '" << phoneToSearch << "'" << endl;
                }
                break;
            }

            case 3: {
                // Partial match search
                bool found = false;
                int count = 0;
                string escapedPattern = escapeRegex(phoneToSearch);
                regex partialMatchPattern(escapedPattern, regex_constants::icase);

                for (int i = 0; i < empDBSize; i++) {
                    if (regex_search(EmployeeRecordsDB[i].getPhoneNum(), partialMatchPattern)) {
                        if (!found) {
                            cout << "\nRecords with Phone Number PARTIALLY matching '" << phoneToSearch << "':" << endl;
                            cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                            cout << left << setw(EMP_IC_LENGTH) << "IC" 
                                 << setw(EMP_NAME_LENGTH) << "Name" 
                                 << setw(EMP_TEL_LENGTH) << "Phone" 
                                 << setw(EMP_DATE_LENGTH) << "Birth Date" 
                                 << setw(EMP_DATE_LENGTH) << "Hired Date" 
                                 << setw(EMP_EMAIL_LENGTH) << "Email" << endl;
                            cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                            found = true;
                        }
                        Employee emp = EmployeeRecordsDB[i];
                        cout << left << setw(EMP_IC_LENGTH) << emp.getIC() 
                             << setw(EMP_NAME_LENGTH) << emp.getName() 
                             << setw(EMP_TEL_LENGTH) << emp.getPhoneNum() 
                             << setw(EMP_DATE_LENGTH) << emp.getBirthDate().toString() 
                             << setw(EMP_DATE_LENGTH) << emp.getHiredDate().toString() 
                             << setw(EMP_EMAIL_LENGTH) << emp.getEmail() << endl;
                        count++;
                    }
                }
                if (!found) {
                    cout << "\nNo matching records found !!!" << endl;
                } else {
                    cout << "\n" << count << " records found with Phone Number PARTIALLY matching '" << phoneToSearch << "'" << endl;
                }
                break;
            }

            case 4:
                // Return to main menu
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (subChoice != 4);
}



void insertNewEmployeeRecord() {
    if (empDBSize >= MAX_NO_OF_RECORDS) {
        cout << "Error: Employee database is full. Cannot insert new records." << endl;
        return;
    }

    string name, ic, email, phoneNum;
    Date birthDate(0, 0, 0), hireDate(0, 0, 0);
	regex specialChar(R"([^a-zA-Z])"); //matches all special characters
    // Input employee name
   
    regex namePattern(R"(^[A-Z][a-z]*(?:[-'][A-Z][a-z]*)*(?: [A-Z][a-z]*| [A-Z]+)*(?:[-'][A-Z][a-z]*)*$)"); //matches name pattern, allow only some special characters
    while(true){
    	cout << "Please type in employee's name (<35 chars) e.g. Ben Tan: ";
		while(true) {
			getline(cin, name);
			cout << endl;
			if (name.empty()) {
				cout << "Input should not be empty. Please enter valid data (< 35 chars) : ";
			}
			else if(name.length() >= 35){
			    cout << "Input too long. Please enter data again (< 35 chars) : ";
			}
			else if (!regex_match(name, namePattern)){
				cout << "Invalid name format. Please enter data again (< 35 chars) : ";
			}
			else {
				break;  // Exit loop only when valid input is received
			}
		}
		// Check for duplicate name
		bool duplicateName = false;
		for (int i = 0; i < empDBSize; i++) {
		    if (EmployeeRecordsDB[i].getName() == name) {
		        string insist;
		        cout << "Error! Name '" << name << "' already exists in employee records DB. Do you INSIST this is correct? (y/n): ";
		        do{
				    getline(cin,insist);
				    cout << endl;
				    if (insist.empty()) {
				    	cout << "Input should not be blank! Please input again : ";
				    }
				    else if ((insist != "y") && (insist != "Y") && (insist != "n") && (insist != "N")){
				    	cout << "Input should only be y or n! Please input again : ";
				    }
				    else if (insist == "y" || insist == "Y") {
				        cout << "Alright you insisted! Sorting (duplicated) Name '"<< name <<" under this Employee Record in DB ..." << endl << endl;
				        duplicateName = false;
				    }
				    else if ((insist == "n") || (insist == "N")){
				    	duplicateName = true;
				    }
		        } while ((insist != "y") && (insist != "Y") && (insist != "n") && (insist != "N"));
		        break;
		    }
		}
		if (!duplicateName) break;
	}

    // Input employee IC
    regex icPattern(R"(^[ST]\d{7}[A-Z]$)");
    while (true) {
        cout << "Please type in employee's IC (< 10 chars e.g. SXXXXXXX(A-Z) or TXXXXXXX(A-Z): ";
        while (true){
        	getline(cin, ic);
			cout << endl;
			if (ic.empty()) {
				cout << "Input cannot be empty. Please enter data again (< 10 chars) : " ;
			}
			else if (ic.length() >= 10) {
				cout << "Input too long. Please enter data again (< 10 chars) : ";
			}
	   		else if (!regex_match(ic, icPattern)){
	   			cout << "Invalid IC format. Please enter data again (< 10 chars) : ";
	   		}
			else {
				break;  // Exit loop only when valid input is received
			}
		}
        // Check for duplicate IC
        bool duplicateIC = false;
        for (int i = 0; i < empDBSize; i++) {
            if (EmployeeRecordsDB[i].getIC() == ic) {
                cout << "Error! IC No. '" << ic << "' already exists in employee records DB. Please try again!" << endl << endl;
                duplicateIC = true;
                break;
            }
        }
        if (!duplicateIC) break;
    }

    // Input employee email
    regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    while (true) {
        cout << "Please type in employee's email (< 35 chars): ";
        while(true) {
		    getline(cin, email);
		    cout << endl;
		    // Validate email format
		    if (email.empty()){
		    	cout << "Input cannot be empty. Please enter data again (< 35 chars) : ";
		    }
		    else if(email.length()>=35){
		    	cout << "Input too long. Please enter data again (< 35 chars) : ";
		    }
		    else if (!regex_match(email, emailPattern)) {
		        cout << "Invalid email format. Please enter a valid email address (< 35 chars) : ";
		    }
		    else {
		    	break;
		    }
        }
        // Check for duplicate email
        bool duplicateEmail = false;
        for (int i = 0; i < empDBSize; i++) {
            if (EmployeeRecordsDB[i].getEmail() == email) {
                cout << "Error! Email '" << email << "' already exists in employee records DB. Please try again!" << endl;
                duplicateEmail = true;
                break;
            }
        }
        if (!duplicateEmail) break;
    }

    // Input employee phone number
    regex phonePattern(R"(^\(\d{2}\)-\d{8}$)"); // Regex to match valid phone numbers (XX)-YYYYYYYY
    while (true) {
        cout << "Please type in employee's phone num (< 15 chars) e.g. (xx)-xxxxxxxx: ";
        while(true) {
		    getline(cin, phoneNum);
			cout << endl;
		    // Validate phone number format
		    if(phoneNum.empty()){
		    	cout << "Input cannot be empty. Please enter data again (< 15 chars) : ";
		    }
		    else if(phoneNum.length()>=15)
		    {
		        cout << "Input too long. Please enter data again (< 15 chars) : ";
		    }
		    else if (!regex_match(phoneNum, phonePattern)) {
		        cout << "Invalid phone number format. Please enter a valid phone number (< 15 chars) : ";
		    }
		    else {
		    	break;
		    }
        }
        
        // Check for duplicate phone number
        bool duplicatePhone = false;
        for (int i = 0; i < empDBSize; i++) {
            if (EmployeeRecordsDB[i].getPhoneNum() == phoneNum) {
                string insist;
                cout << "Error! Phone Num '" << phoneNum << "' already exists in employee records DB. Do you INSIST this is correct? (y/n): ";
                do{
		            getline(cin,insist);
		            if (insist.empty()) {
		            	cout << "Input should not be blank! Please input again : ";
		            }
		            else if ((insist != "y") && (insist != "Y") && (insist != "n") && (insist != "N")){
		            	cout << "Input should only be y or n! Please input again : ";
		            }
		            else if (insist == "y" || insist == "Y") {
		                cout << "Alright you insisted! Sorting (duplicated) Phone Number '"<< phoneNum <<" under this Employee Record in DB ..." << endl;
		                duplicatePhone = false;
		            }
		            else if ((insist == "n") || (insist == "N")){
		            	duplicatePhone = true;
		            }
                } while ((insist != "y") && (insist != "Y") && (insist != "n") && (insist != "N"));
                break;
            }
        }
        if (!duplicatePhone) break;
    }

    // Input employee birth date
    birthDate = inputDate("Enter employee's date of birth (dd-mm-yyyy): ");
    // Input employee hire date
    bool isHireDateValid = true;
    do {
		hireDate = inputDate("Enter employee's date of hire (dd-mm-yyyy): ");
		// Calculate age at hire date
		int ageAtHire = hireDate.year - birthDate.year;
		// Adjust for month/day if hire date hasn't occurred yet in the birth year
		if (hireDate.month < birthDate.month || (hireDate.month == birthDate.month && hireDate.day < birthDate.day)) {
			ageAtHire--; // Subtract 1 year if birthday hasn't passed yet
		}
		if (ageAtHire < 18) {
			isHireDateValid = false;
			cout << "Please ensure new employee is 18 years old!" << endl;
		}
		// Create an Employee object and add it to the database
		else {
			// Create the new employee record
			Employee newEmployee(name, email, ic, phoneNum, hireDate, birthDate);
			// Add the new employee to the database
			EmployeeRecordsDB[empDBSize] = newEmployee;
			empDBSize++; // Update the number of records in the database
			cout << "Done! New employee record inserted. Going back to main menu ..." << endl;
			isHireDateValid = true;
		}
    } while (!isHireDateValid);
}


void Update() {
    int Index;
    string icToSearch;
    string input;
    int subChoice;

    // Use a do-while loop to repeatedly prompt the user
    cout << "Please type in IC value to search for (< 10 chars e.g. SXXXXXXX(A-Z) or TXXXXXXX(A-Z)) : ";
    do {
        getline(cin, icToSearch);
		cout << endl;
        // Check if the input is empty
        if (icToSearch.empty()) {
            cout << "Input cannot be empty. Please enter data again (< 10 chars e.g. SXXXXXXX(A-Z) or TXXXXXXX(A-Z)) : ";
        }
        // Check if the input is too long
        else if (icToSearch.length() >= 10) {
            cout << "Input too long. Please enter data again (< 10 chars e.g. SXXXXXXX(A-Z) or TXXXXXXX(A-Z)) : ";
        }
    } while (icToSearch.empty() || icToSearch.length() >= 10); // Repeat until input is valid

    bool found = false;
    int count = 0;

    // Search for the employee by IC
    for (int i = 0; i < empDBSize; i++) {
        if (EmployeeRecordsDB[i].getIC() == icToSearch) {
            if (!found) {
                cout << "\nRecords with IC EXACTLY matching '" << icToSearch << "':" << endl;
                cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
                cout << left << setw(EMP_IC_LENGTH) << "IC" 
                     << setw(EMP_NAME_LENGTH) << "Name" 
                     << setw(EMP_TEL_LENGTH) << "Phone" 
                     << setw(EMP_DATE_LENGTH) << "Birth Date" 
                     << setw(EMP_DATE_LENGTH) << "Hired Date" 
                     << setw(EMP_EMAIL_LENGTH) << "Email" << endl;
                cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
                found = true;
            }

            Employee emp = EmployeeRecordsDB[i];
            cout << left << setw(EMP_IC_LENGTH) << emp.getIC() 
                 << setw(EMP_NAME_LENGTH) << emp.getName() 
                 << setw(EMP_TEL_LENGTH) << emp.getPhoneNum() 
                 << setw(EMP_DATE_LENGTH) << emp.getBirthDate().toString() 
                 << setw(EMP_DATE_LENGTH) << emp.getHiredDate().toString() 
                 << setw(EMP_EMAIL_LENGTH) << emp.getEmail() << endl;
            count++;
            Index = i;
        }
    }

    if (!found) {
        cout << "Error! IC No. '" << icToSearch <<"' Does Not exist in employee records DB!" << endl;
    } else {
        cout << "\n" << count << " records found, with IC EXACTLY matching " << icToSearch << " !!" << endl;

        // Sub-menu for updating fields
        do {
            Employee Emp = EmployeeRecordsDB[Index];
            cout << "+++++++++++++++++++++++++++++++" << endl;
            cout << "++ Item to be updated ... +++" << endl;
            cout << "+++++++++++++++++++++++++++++++" << endl;
            cout << "1) Update IC               (curr. value = '" << Emp.getIC() << "')" << endl;
            cout << "2) Update EMAIL            (curr. value = '" << Emp.getEmail() << "')" << endl;
            cout << "3) Update Name             (curr. value = '" << Emp.getName() << "')" << endl;
            cout << "4) Update Phone Number     (curr. value = '" << Emp.getPhoneNum() << "')" << endl;
            cout << "5) Update Birth Date       (curr. value = '" << Emp.getBirthDate().toString() << "')" << endl;
            cout << "6) Update Hired Date       (curr. value = '" << Emp.getHiredDate().toString() << "')" << endl;
            cout << "7) Done with updates" << endl;
            cout << "+++++++++++++++++++++++++++++++" << endl;
            cout << "Please enter your choice (1 - 7): ";
            getline(cin, input);
			subChoice = stoi(input); // Convert valid input to integer
            switch (subChoice) {
                case 1: {
                    // Update IC
                    string newIC;
                    regex icPattern(R"(^[ST]\d{7}[A-Z]$)");
                    while (true) {
						cout << "Please type in employee's IC (< 10 chars e.g. SXXXXXXX(A-Z) or TXXXXXXX(A-Z): ";
						while (true){
							getline(cin, newIC);
							cout << endl;
							if (newIC.empty()) {
								cout << "Input cannot be empty. Please enter data again (< 10 chars) : " ;
							}
							else if (newIC.length() >= 10) {
								cout << "Input too long. Please enter data again (< 10 chars) : ";
							}
					   		else if (!regex_match(newIC, icPattern)){
					   			cout << "Invalid IC format. Please enter data again (< 10 chars) : ";
					   		}
							else {
								break;  // Exit loop only when valid input is received
							}
						}
						// Check for duplicate IC
						bool duplicateIC = false;
						for (int i = 0; i < empDBSize; i++) {
							if (EmployeeRecordsDB[i].getIC() == newIC && i != Index) {
								cout << "Error! IC No. '" << newIC << "' already exists in employee records DB. Please try again!" << endl << endl;
								duplicateIC = true;
								break;
							}
						}
						if (!duplicateIC) break;
					}
					EmployeeRecordsDB[Index].setIC(newIC);
					break;
				}

                case 2: {
                    // Update Email
                    string newEmail;
                    regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
                    while (true) {
						cout << "Please type in employee's email (< 35 chars): ";
						while(true) {
							getline(cin, newEmail);
							cout << endl;
							// Validate email format
							if (newEmail.empty()){
								cout << "Input cannot be empty. Please enter data again (< 35 chars) : ";
							}
							else if(newEmail.length()>=35){
								cout << "Input too long. Please enter data again (< 35 chars) : ";
							}
							else if (!regex_match(newEmail, emailPattern)) {
								cout << "Invalid email format. Please enter a valid email address (< 35 chars) : ";
							}
							else {
								break;
							}
						}
						// Check for duplicate email
						bool duplicateEmail = false;
						for (int i = 0; i < empDBSize; i++) {
							if (EmployeeRecordsDB[i].getEmail() == newEmail && i != Index) {
								cout << "Error! Email '" << newEmail << "' already exists in employee records DB. Please try again!" << endl;
								duplicateEmail = true;
								break;
							}
						}
						if (!duplicateEmail) break;
					}
                    EmployeeRecordsDB[Index].setEmail(newEmail);
                    break;
                }

                case 3: {
                    // Update Name
                    string newName;
                    regex namePattern(R"(^[A-Z][a-z]*(?:[-'][A-Z][a-z]*)*(?: [A-Z][a-z]*| [A-Z]+)*(?:[-'][A-Z][a-z]*)*$)"); //matches name pattern, allow only some special characters
                    while(true){
						cout << "Please type in employee's name (<35 chars) e.g. Ben Tan: ";
						while(true) {
							getline(cin, newName);
							cout << endl;
							if (newName.empty()) {
								cout << "Input should not be empty. Please enter valid data (< 35 chars) : ";
							}
							else if(newName.length() >= 35){
								cout << "Input too long. Please enter data again (< 35 chars) : ";
							}
							else if (!regex_match(newName, namePattern)){
								cout << "Invalid name format. Please enter data again (< 35 chars) : ";
							}
							else {
								break;  // Exit loop only when valid input is received
							}
						}
						// Check for duplicate name
						bool duplicateName = false;
						for (int i = 0; i < empDBSize; i++) {
							if (EmployeeRecordsDB[i].getName() == newName && i != Index) {
								string insist;
								cout << "Error! Name '" << newName << "' already exists in employee records DB. Do you INSIST this is correct? (y/n): ";
								do{
									getline(cin,insist);
									cout << endl;
									if (insist.empty()) {
										cout << "Input should not be blank! Please input again : ";
									}
									else if ((insist != "y") && (insist != "Y") && (insist != "n") && (insist != "N")){
										cout << "Input should only be y or n! Please input again : ";
									}
									else if (insist == "y" || insist == "Y") {
										cout << "Alright you insisted! Sorting (duplicated) Name '"<< newName <<" under this Employee Record in DB ..." << endl << endl;
										duplicateName = false;
									}
									else if ((insist == "n") || (insist == "N")){
										duplicateName = true;
									}
								} while ((insist != "y") && (insist != "Y") && (insist != "n") && (insist != "N"));
								break;
							}
						}
						if (!duplicateName) break;
					}
		            EmployeeRecordsDB[Index].setName(newName);
		            break;
		        }

                case 4: {
                    // Update Phone Number
                    string newPhoneNum;
                    regex phonePattern(R"(^\(\d{2}\)-\d{8}$)");
                    while (true) {
						cout << "Please type in employee's phone num (< 15 chars) e.g. (xx)-xxxxxxxx: ";
						while(true) {
							getline(cin, newPhoneNum);
							cout << endl;
							// Validate phone number format
							if(newPhoneNum.empty()){
								cout << "Input cannot be empty. Please enter data again (< 15 chars) : ";
							}
							else if(newPhoneNum.length()>=15)
							{
								cout << "Input too long. Please enter data again (< 15 chars) : ";
							}
							else if (!regex_match(newPhoneNum, phonePattern)) {
								cout << "Invalid phone number format. Please enter a valid phone number (< 15 chars) : ";
							}
							else {
								break;
							}
						}
						
						// Check for duplicate phone number
						bool duplicatePhone = false;
						for (int i = 0; i < empDBSize; i++) {
							if (EmployeeRecordsDB[i].getPhoneNum() == newPhoneNum && i != Index) {
								string insist;
								cout << "Error! Phone Num '" << newPhoneNum << "' already exists in employee records DB. Do you INSIST this is correct? (y/n): ";
								do{
									getline(cin,insist);
									if (insist.empty()) {
										cout << "Input should not be blank! Please input again : ";
									}
									else if ((insist != "y") && (insist != "Y") && (insist != "n") && (insist != "N")){
										cout << "Input should only be y or n! Please input again : ";
									}
									else if (insist == "y" || insist == "Y") {
										cout << "Alright you insisted! Sorting (duplicated) Phone Number '"<< newPhoneNum <<" under this Employee Record in DB ..." << endl;
										duplicatePhone = false;
									}
									else if ((insist == "n") || (insist == "N")){
										duplicatePhone = true;
									}
								} while ((insist != "y") && (insist != "Y") && (insist != "n") && (insist != "N"));
								break;
							}
						}
						if (!duplicatePhone) break;
					}
                    EmployeeRecordsDB[Index].setPhoneNum(newPhoneNum);
                    break;
                }
                case 5: {
                    // Update Birth Date
                    Date birthDate = (0,0,0);
                    Date hireDate = EmployeeRecordsDB[Index].getHiredDate();
                    cout << "Please enter employee's date of birth, please adhere to the format shown in the prompt below ... " << endl;
                    bool isHireDateValid = true;
					do {
						birthDate = inputDate("Enter a date (dd-mm-yyyy): ");
						// Calculate age at hire date
						int ageAtHire = hireDate.year - birthDate.year;
						// Adjust for month/day if hire date hasn't occurred yet in the birth year
						if (hireDate.month < birthDate.month || (hireDate.month == birthDate.month && hireDate.day < birthDate.day)) {
							ageAtHire--; // Subtract 1 year if birthday hasn't passed yet
						}
						if (ageAtHire < 18) {
							isHireDateValid = false;
							cout << "Please ensure employee is 18 years old!" << endl;
						}
						else {
							isHireDateValid = true;
						}
					} while (!isHireDateValid);
                    EmployeeRecordsDB[Index].setBirthDate(birthDate);
                    cout << "Birth date updated successfully!" << endl;
                    break;
                }

                case 6: {
                    // Update Hire Date
                    Date hireDate = (0,0,0);
                    Date birthDate = EmployeeRecordsDB[Index].getBirthDate();
                    cout << "Please enter employee's date of hire, please adhere to the format shown in the prompt below ... " << endl;
                    bool isHireDateValid = true;
					do {
						hireDate = inputDate("Enter a date (dd-mm-yyyy): ");
						// Calculate age at hire date
						int ageAtHire = hireDate.year - birthDate.year;
						// Adjust for month/day if hire date hasn't occurred yet in the birth year
						if (hireDate.month < birthDate.month || (hireDate.month == birthDate.month && hireDate.day < birthDate.day)) {
							ageAtHire--; // Subtract 1 year if birthday hasn't passed yet
						}
						if (ageAtHire < 18) {
							isHireDateValid = false;
							cout << "Please ensure employee is 18 years old!" << endl;
						}
						else {
							isHireDateValid = true;
						}
					} while (!isHireDateValid);
                    EmployeeRecordsDB[Index].setHiredDate(hireDate);
                    cout << "Hire date updated successfully!" << endl;
                    break;
                }

                case 7:{
                    // Done with updates
                    cout << "Returning to main menu ..." << endl;
                    break;
				
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        } 
        }while (subChoice != 7);
	
	}
}

void Delete() {
    string icToDelete;

    // Use a do-while loop to repeatedly prompt the user
    cout << "Please type in IC value to search for (< 10 chars e.g. SXXXXXXX(A-Z) or TXXXXXXX(A-Z)) : ";
    do {
        getline(cin, icToDelete);
		cout << endl;
        // Check if the input is empty
        if (icToDelete.empty()) {
            cout << "Input cannot be empty. Please enter data again (< 10 chars e.g. SXXXXXXX(A-Z) or TXXXXXXX(A-Z)) : ";
        }
        // Check if the input is too long
        else if (icToDelete.length() >= 10) {
            cout << "Input too long. Please enter data again (< 10 chars e.g. SXXXXXXX(A-Z) or TXXXXXXX(A-Z)) : ";
        }
    } while (icToDelete.empty() || icToDelete.length() >= 10); // Repeat until input is valid

    bool found = false;
    int index = -1;

    // Search for the employee by IC
    for (int i = 0; i < empDBSize; i++) {
        if (EmployeeRecordsDB[i].getIC() == icToDelete) {
            found = true;
            index = i;
            break;
        }
    }

    if (!found) {
        cout << "Error! IC No. '" << icToDelete << "' DOES NOT exist in employee records DB!" << endl;
        return;
    }

    // Display the found record
    cout << "\nRecords with IC EXACTLY matching '" << icToDelete << "':" << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
    cout << left << setw(EMP_IC_LENGTH) << "IC" 
         << setw(EMP_NAME_LENGTH) << "Name" 
         << setw(EMP_TEL_LENGTH) << "Phone" 
         << setw(EMP_DATE_LENGTH) << "Birth Date" 
         << setw(EMP_DATE_LENGTH) << "Hired Date" 
         << setw(EMP_EMAIL_LENGTH) << "Email" << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------" << endl;

    Employee emp = EmployeeRecordsDB[index];
    cout << left << setw(EMP_IC_LENGTH) << emp.getIC() 
         << setw(EMP_NAME_LENGTH) << emp.getName() 
         << setw(EMP_TEL_LENGTH) << emp.getPhoneNum() 
         << setw(EMP_DATE_LENGTH) << emp.getBirthDate().toString() 
         << setw(EMP_DATE_LENGTH) << emp.getHiredDate().toString() 
         << setw(EMP_EMAIL_LENGTH) << emp.getEmail() << endl;

    // Confirm deletion
    string confirm;
  do {
        cout << "\nConfirm deletion of the record? (y/n): ";
        getline(cin, confirm);

        // Check if the input is empty
        if (confirm.empty()) {
            cout << "Input cannot be empty. Please try again." << endl;
        }
        // Check if the input is valid (y, Y, n, N)
        else if (confirm != "y" && confirm != "Y" && confirm != "n" && confirm != "N") {
            cout << "Invalid input. Please enter 'y' or 'n'." << endl;
        }
    } while (confirm.empty() || (confirm != "y" && confirm != "Y" && confirm != "n" && confirm != "N")); // Repeat until input is valid

    if (confirm == "y" || confirm == "Y") {
        // Shift records to fill the gap
        for (int i = index; i < empDBSize - 1; i++) {
            EmployeeRecordsDB[i] = EmployeeRecordsDB[i + 1];
        }
        // Decrease the total number of records
        empDBSize--;

        cout << "Record deleted successfully! Going back to main menu ..." << endl;
    } else {
        cout << "Deletion canceled. Going back to main menu ..." << endl;
    }
}

void writeDataToFile() {
    string fileName;
	regex filenamePattern(R"(^[a-zA-Z0-9][a-zA-Z0-9\-_\.]*\.csv$)");//ensures filename ends with .csv
	do{
		// Prompt the user for the output file name
		cout << "Please enter the output (CSV) file's name: ";
		getline(cin, fileName);
		if (!regex_match(fileName, filenamePattern)){ //Check if file is .csv

		    cout << "Invalid input for file name, please input new file name. " << endl << endl;

		}
    }while(!regex_match(fileName, filenamePattern));
    // Open the file for writing
    ofstream outFile(fileName);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file '" << fileName << "' for writing." << endl;
        return;
    }

    // Write the header to the file
    outFile << "Idx,Name,Email,IC,PhoneNum,HireDate,BirthDate" << endl;

    // Write each employee record to the file
    for (int i = 0; i < empDBSize; i++) {
        Employee emp = EmployeeRecordsDB[i];
        outFile << i + 1 << "," // idx (starting from 1)
                << emp.getName() << ","
                << emp.getEmail() << ","
                << emp.getIC() << ","
                << emp.getPhoneNum() << ","
                << emp.getHiredDate().toString() << ","
                << emp.getBirthDate().toString() << endl;
    }

    // Close the file
    outFile.close();

    // Display confirmation message
    cout << "Done! Total no. of records written to output file '" << fileName << "' = " << empDBSize << " records" << endl;
}
