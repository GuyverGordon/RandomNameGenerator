/*
Name: Jonathan Oesch
Date: May 3, 2017
Program: GeoLab Random Name Generator 2.0
Description:  Selects a name a random from a list.

Modifications:
	- now truly random
	- added ability to add or delete names
	- added ability to view list
*/

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <vector>

using namespace std;

//Function prototypes
void intro();										//displays introduction
void createList(vector<string> &);						//Produce current employee list
int displayMenu(unsigned int, unsigned int, string);		//displays menu
void mainMenu();										//displays main menu options
void editViewMenu();								//displays edit/view menu options
void editView(vector<string> &);						//user selects option to edit/view list
void sortList(vector<string> &list);						//sorts list
void randomName(vector<string> &);						//computer displays random name
bool doAnother(string);							//Allow user to repeat
void cleanCin();										//clears up residual input
void addName(vector<string> &);								//user adds name(s)
void writeToFile(vector<string>);						//writes current list to file
void deleteName(vector<string> &);					//user deletes name(s)
bool correct(string);									//ensure user input is correct
void viewList(vector<string>);								//display entire contents of vector
void endMessage();												//End program message

int main()
{
	//Introduce program
	intro();

	vector<string> name;		//To hold the list of names

	//Create employee list
	createList(name);

	//Seed random number
	srand(time(0));

	unsigned int menuSelection;

	do
	{
		//Display and select main menu options
		menuSelection = displayMenu(1, 3, "main");

		switch (menuSelection)
		{
			case 1:			//Pick Random Name
				randomName(name);
				break;
			case 2:			//Edit/View Employee Names
				editView(name);
		}
		system("cls");
	} while (menuSelection != 3);

	//End program message
	endMessage();

	system("pause");
	return 0;
}

//This function introduces the program.
void intro()
{
	cout << " ___   ___           __      _\n";
	cout << "|   | |   | |\\    | |  \\    / \\   /\\   /\\ \n";
	cout << "|   | |   | | \\   | |   \\  |   | |  \\/   | \n";
	cout << "|---  |---| |  \\  | |    | |   | |       | \n";
	cout << "|  \\  |   | |   \\ | |   /  |   | |       | \n";
	cout << "|   \\ |   | |    \\| |__/    \\_/  |       | \n\n";
	cout << "         ___            ____\n";
	cout << "|\\    | |   |  /\\  /\\  | \n";
	cout << "| \\   | |   | |  \\/  | | \n";
	cout << "|  \\  | |---| |      | |--- \n";
	cout << "|   \\ | |   | |      | | \n";
	cout << "|    \\| |   | |      | |____ \n\n";
	cout << " ____   ____          ____  ___   ___  _____    _    ___\n";
	cout << "|    | |     |\\    | |     |   | |   |   |     / \\  |   |\n";
	cout << "|      |     | \\   | |     |   | |   |   |    |   | |   |\n";
	cout << "|      |---  |  \\  | |---  |---  |---|   |    |   | |---\n";
	cout << "|  --| |     |   \\ | |     |  \\  |   |   |    |   | |  \\\n";
	cout << "|____| |____ |    \\| |____ |   \\ |   |   |     \\_/  |   \\\n\n";
}										//intro

//This function produces a list of names found on file
void createList(vector<string> &name)
{
	//Open reading file
	ifstream readFile("employee.dat");
	string temp;

	//File check -- see if it is open
	if (readFile.fail())
	{
		cout << "Error -- file did not open.  You may need to recreate the file by adding\n";
		cout << "employees to the list\n";
		system("pause");
	}
	else						//if file did open
	{
		//Try for the first name on the list
		getline(readFile, temp);

		//check to see if there are any contents on file
		if (readFile.eof())
		{
			cout << "There seems to be nothing in your employee file.\n";
			cout << "Be sure to add employees before choosing a random name.\n";
			system("pause");
			cout << endl;
		}
		else
		{
			name.push_back(temp);
			while (!(readFile.eof()))
			{
				getline(readFile, temp);
				name.push_back(temp);
			}

			//Close file once everything is read
			readFile.close();
		}
	}
}										//createList

//This function displays a certain menu and returns an integer number.
int displayMenu(unsigned int min, unsigned int max, string menu)
{
	unsigned int number;

	//Display correct menu
	if (menu == "main")
		mainMenu();
	else
		editViewMenu();
	cout << "Please make a selection: ";

	cin >> number;

	while (cin.fail() || number < min || number > max)
	{
		if (cin.fail())
			cleanCin();

		cout << "Error -- please enter a number between " << min << " and " << max << endl << endl;

		//Display correct menu
		if (menu == "main")
			mainMenu();
		else
			editViewMenu();
		cout << "Please make a selection: ";

		cin >> number;
	}

	return number;
}										//displayMenu

//Displays the main menu options
void mainMenu()
{
	cout << "MAIN MENU\n";
	cout << "---------\n";
	cout << "1. Pick Random Name\n";
	cout << "2. Edit/View Employee List\n";
	cout << "3. Quit\n\n";
}										//mainMenu

//Displays the edit/view options
void editViewMenu()
{
	cout << "EDIT/VIEW EMPLOYEE LIST\n";
	cout << "-----------------------\n";
	cout << "1. Add Employee Name\n";
	cout << "2. Delete Employee Name\n";
	cout << "3. View Entire Employee List\n";
	cout << "4. Back to Main Menu\n";
}										//editViewMenu

//This function displays choices for the edit/view menu and executes them
void editView(vector<string> &name)
{
	system("cls");

	unsigned int menuOption;

	do
	{
		menuOption = displayMenu(1, 4, "edit");

		switch (menuOption)
		{
		case 1:			//Add Employee Name
			addName(name);
			break;
		case 2:			//Delete Employee Name
			if (name.size() > 0)
				deleteName(name);
			else
			{
				cout << "\nYou do not have anyone in your list to delete\n";
				system("pause");
			}

			break;
		case 3:			//View Entire Employee List
			viewList(name);
			system("pause");
		}

		system("cls");
	} while (menuOption != 4);
}										//editView

//This function sorts the list of names
void sortList(vector<string> &list)
{
	int startScan, minIndex;	//index variables
	string minAlpha;		//content variable

	for (startScan = 0; startScan < (list.size() - 1); startScan++)
	{
		minIndex = startScan;
		minAlpha = list[startScan];

		for (int index = startScan + 1; index < list.size(); index++)
		{
			if (list[index] < minAlpha)
			{
				minAlpha = list[index];
				minIndex = index;
			}
		}

		list[minIndex] = list[startScan];
		list[startScan] = minAlpha;
	}
}										//sortList

//This function takes a random number and uses it to display a random name.  User is allowed to repeat as often as desired.
void randomName(vector<string> &list)
{
	if (list.empty())
	{
		cout << "\nThere are no names to pick.  Please add names to the employee list.\n";
		system("pause");
		return;
	}

	do
	{
		system("cls");
		//Generate random number
		int num = rand() % list.size();

		//Display Name
		cout << list[num];
	} while (doAnother("pick another random name"));
}										//randomName

//This function asks user if he/she wants to continue activity.  Returns true if "yes", else returns false.
bool doAnother(string question)
{
	char response;

	cout << "\n\nWould you like " << question << " (y for yes, n for no)? ";
	cin >> response;
	cleanCin();

	while (tolower(response) != 'y' && tolower(response) != 'n')
	{
		cout << "Error -- please type y for yes or n for no\n";
		cout << "Would you like to " << question << "? ";
		cin >> response;
		cleanCin();
	}

	cout << endl;

	if (tolower(response) == 'y')
		return true;
	else
		return false;
}										//doAnother

//This function cleans up flags and buffered chars
void cleanCin()
{
	cin.clear();
	cin.ignore(80, '\n');
}										//cleanCin

void addName(vector<string> &name)
{
	system("cls");
	cleanCin();

	string getName;

	cout << "ADD NAME(S)\n";
	cout << "-----------\n";

	//Loop as long as user wants
	do
	{
		bool found = false;

		//Prompt for name to add
		do
		{
			cout << "Please enter the employee's full name (type 0 to cancel): ";
			getline(cin, getName);

			//Check to see if name already exists
			for (int count = 0; count < name.size() && !found; count++)
				found = (name[count] == getName);

			if (found)
			{
				cout << "That person is already on the list!\n";
				break;
			}

			cout << endl;
		} while (getName != "0" && !correct(getName));	//Allow the user to retype in case of mistake

		if (getName == "0")		//Cancel
			return;

		if (!found)
		{
			name.push_back(getName);
			cout << "Name Added!";
		}

	} while (doAnother("add another employee"));

	//sort list alphabetically once all the names have been added
	sortList(name);

	//Write additions to file
	writeToFile(name);
}										//addName

//This function takes the current vector and writes it to file, replacing the old file
void writeToFile(vector<string> name)
{
	//Open file

	//ofstream writeFile("test.dat");		//for testing purposes
	
	ofstream writeFile("employee.dat");		//actual file

	//Enter first name to file
	if (!name.empty())
	{
		writeFile << name[0];

		//Enter the rest of the list
		for (unsigned int count = 1; count < name.size(); count++)
			writeFile << endl << name[count];
	}
	//Close file
	writeFile.close();
}										//writeToFile

//This function asks the users for confirmation on input.  Returns true if y, or false if n.
bool correct(string name)
{
	char response;

	cout << name << ", is that correct(y for yes, n for no)? ";
	cin >> response;
	cleanCin();

	while (tolower(response) != 'y' && tolower(response) != 'n')
	{
		cout << "Error -- please type y for yes or n for no\n";
		cout << name << ", is that correct(y for yes, n for no)? ";
		cin >> response;
		cleanCin();
	}

	if (tolower(response) == 'y')
		return true;
	else
		return false;
}										//correct

//This function asks the user for names to delete from the list
void deleteName(vector<string> &list)
{
	do
	{
		if (list.empty())
		{
			cout << "\nThere is no one left to delete\n";
			writeToFile(list);
			system("pause");
			return;
		}

		//Show list of current names
		viewList(list);

		//Prompt for choice
		unsigned int choice;

		cout << "Please select which name to delete (select " << list.size() + 1 << " to cancel): ";
		cin >> choice;

		while (cin.fail() || choice < 1 || choice > list.size() + 1)
		{
			if (cin.fail())
				cleanCin();

			cout << "Error -- invalid choice -- please choose 1 - " << list.size() + 1 << endl;
			cout << "Please select which name to delete (select " << list.size() + 1 << " to cancel): ";
			cin >> choice;
		}

		if (choice == list.size() + 1)
		{
			writeToFile(list);
			return;
		}
		else
		{
			//shift names listed after "deleted" name up by one
			while (choice < list.size())
			{
				list[choice - 1] = list[choice];
				choice++;
			}

			//decrease vector size
			list.pop_back();

			//inform user
			cout << "\nName deleted!\n";
		}
	} while (doAnother("delete another employee"));

	//Write changes to file
	writeToFile(list);
}										//deleteName

//This function lists all the names found in the vector
void viewList(vector<string> list)
{
	if (list.size() > 0)
	{
		system("cls");

		cout << "LIST OF CURRENT EMPLOYEES\n";
		cout << "-------------------------\n";

		for (unsigned int count = 0; count < list.size(); count++)
			cout << count + 1 << ": " << list[count] << endl;

		cout << endl;
	}
	else
		cout << "\nThere is currently no one to list!\n";
}										//viewList

void endMessage()
{
	cout << "Thank you for using this program!\n\n";
}										//endMessage