#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const string KEY = "SECRETKEY"; // Encryption key

string encrypt(string data)
{
    cout << "Encrypting data: " << data << endl;
    for (int i = 0; i < data.length(); i++)
    {
        data[i] = data[i] ^ KEY[i % KEY.length()];
    }
    cout << "Encrypted result: " << data << endl;
    return data;
}

string decrypt(string data)
{
    cout << "Decrypting data: " << data << endl;
    for (int i = 0; i < data.length(); i++)
    {
        data[i] = data[i] ^ KEY[i % KEY.length()];
    }
    cout << "Decrypted result: " << data << endl;
    return data;
}

void signUp()
{
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    string encryptedUsername = encrypt(username);
    string encryptedPassword = encrypt(password);

    fstream userFile;
    userFile.open("users.txt", ios::app);
    if (userFile.is_open())
    {
        cout << "Saving encrypted credentials to file..." << endl;
        userFile << encryptedUsername << " " << encryptedPassword << endl;
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
        cout << "Checking if user exists..." << endl;
        while (userFile >> storedUsername >> storedPassword)
        {
            string decryptedUsername = decrypt(storedUsername);
            cout << "Comparing " << decryptedUsername << " with " << username << endl;
            if (decryptedUsername == username)
            {
                userFile.close();
                cout << "User found." << endl;
                return true;
            }
        }
        userFile.close();
        cout << "User not found." << endl;
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
        cout << "Authenticating user..." << endl;
        while (userFile >> storedUsername >> storedPassword)
        {
            string decryptedUsername = decrypt(storedUsername);
            string decryptedPassword = decrypt(storedPassword);
            cout << "Comparing " << decryptedUsername << " with " << username << endl;
            cout << "Comparing " << decryptedPassword << " with " << password << endl;
            if (decryptedUsername == username && decryptedPassword == password)
            {
                userFile.close();
                cout << "Authentication successful." << endl;
                return true;
            }
        }
        userFile.close();
        cout << "Authentication failed." << endl;
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
        cout << "\n1. Sign up" << endl;
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