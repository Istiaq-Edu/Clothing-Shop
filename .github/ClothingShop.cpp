#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void signUp()
{
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    fstream userFile;
    userFile.open("users.txt", ios::app);
    if (userFile.is_open())
    {
        userFile << username << " " << password << endl;
        userFile.close();
        cout << "Sign up successful!" << endl;
    }
    else
    {
        cout << "Unable to open file." << endl;
    }
}

bool userExists(string username)
{
    fstream userFile;
    userFile.open("users.txt", ios::in);
    string storedUsername, storedPassword;
    if (userFile.is_open())
    {
        while (userFile >> storedUsername >> storedPassword)
        {
            if (storedUsername == username)
            {
                userFile.close();
                return true;
            }
        }
        userFile.close();
    }
    return false;
}

bool authenticate(string username, string password)
{
    fstream userFile;
    userFile.open("users.txt", ios::in);
    string storedUsername, storedPassword;
    if (userFile.is_open())
    {
        while (userFile >> storedUsername >> storedPassword)
        {
            if (storedUsername == username && storedPassword == password)
            {
                userFile.close();
                return true;
            }
        }
        userFile.close();
    }
    return false;
}

void login()
{
    string username, password;
    int limit = 3;
    while (limit > 0)
    {
        cout << "Please enter your username: ";
        cin >> username;

        if (!userExists(username))
        {
            cout << "User doesn't exist. Please sign up or try again." << endl;
            cout << "You have " << --limit << " attempts left" << endl;
        }
        else
        {
            cout << "Please enter your password: ";
            cin >> password;

            if (authenticate(username, password))
            {
                cout << "Login successful!" << endl;
                return;
            }
            else
            {
                cout << "Incorrect password." << endl;
                cout << "You have " << --limit << " attempts left" << endl;
            }
        }
    }
    cout << "No more attempts left. Returning to main menu." << endl;
}

int main()
{
    int choice;

    do
    {
        cout << "1. Sign up" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            signUp();
            break;
        case 2:
            login();
            break;
        case 3:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);

    return 0;
}