#include <bits/stdc++.h>

using namespace std;

string KEY = "SECRETKEY"; // Encryption key

// XOR encryption method.
string encrypt(string data)
{
    cout << "Encrypting data...." << endl;

    for (int i = 0; i < data.length(); i++)
    {
        data[i] = data[i] ^ KEY[i % KEY.length()];
    }
    return data;
}

// XOR decryption method.
string decrypt(string data)
{
    cout << "Decrypting data...." << endl;
    for (int i = 0; i < data.length(); i++)
    {
        data[i] = data[i] ^ KEY[i % KEY.length()];
    }
    return data;
}

bool userExists(string username)
{
    fstream userFile;
    userFile.open("Users.txt", ios::in);

    string storedUsername, storedPassword;

    if (userFile.is_open())
    {
        cout << "Checking if user exists..." << endl;

        while (userFile >> storedUsername >> storedPassword)
        {
            string decryptedUsername = decrypt(storedUsername);

            if (decryptedUsername == username)
            {
                userFile.close();
                return true;
            }
        }

        userFile.close();
    }

    return false;
}

void signUp()
{
    string username, password;
    cout << "Enter your name: ";
    cin >> username;

    if (userExists(username))
    {
        cout << "Username already exists. Choose a different username." << endl;
        return;
    }

    cout << "Enter password: ";
    cin >> password;

    string encryptedUsername = encrypt(username);
    string encryptedPassword = encrypt(password);

    fstream userFile;
    userFile.open("Users.txt", ios::app);

    if (userFile.is_open())
    {
        userFile << encryptedUsername << " " << encryptedPassword << endl;
        userFile.close();
        cout << "Sign up successful!" << endl;
    }
}

bool adminExists(string username)
{
    fstream adminFile;
    adminFile.open("Admins.txt", ios::in);

    string storedUsername, storedPassword;

    if (adminFile.is_open())
    {
        cout << "Checking if admin exists..." << endl;

        while (adminFile >> storedUsername >> storedPassword)
        {
            string decryptedUsername = decrypt(storedUsername);

            if (decryptedUsername == username)
            {
                adminFile.close();
                return true;
            }
        }
        adminFile.close();
    }
    return false;
}

void adminSignUp()
{
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;

    if (adminExists(username))
    {
        cout << "Admin username already exists. Choose a different username." << endl;
        return;
    }

    cout << "Enter admin password: ";
    cin >> password;

    string encryptedUsername = encrypt(username);
    string encryptedPassword = encrypt(password);

    fstream adminFile;

    adminFile.open("Admins.txt", ios::app);

    if (adminFile.is_open())
    {
        adminFile << encryptedUsername << " " << encryptedPassword << endl;
        adminFile.close();
        cout << "Admin sign up successful!" << endl;
    }
}

bool authenticateAdmin(string username, string password)
{
    fstream adminFile;

    adminFile.open("admins.txt", ios::in);

    string storedUsername, storedPassword;

    if (adminFile.is_open())
    {
        cout << "Authenticating admin..." << endl;

        while (adminFile >> storedUsername >> storedPassword)
        {
            string decryptedUsername = decrypt(storedUsername);
            string decryptedPassword = decrypt(storedPassword);

            if (decryptedUsername == username && decryptedPassword == password)
            {
                adminFile.close();

                cout << "Admin authentication successful." << endl;
                return true;
            }
        }
        adminFile.close();

        cout << "Admin authentication failed." << endl;
    }
    return false;
}

bool adminLogin()
{
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;

    if (!adminExists(username))
    {
        cout << "Admin doesn't exist. Sign up first." << endl;
        return false;
    }

    int attempts = 3;
    while (attempts > 0)
    {
        cout << "Enter admin password: ";
        cin >> password;

        if (authenticateAdmin(username, password))
        {
            return true;
        }
        else
        {
            attempts--;
            if (attempts > 0)
            {
                cout << "Incorrect password. You have " << attempts << " attempts left." << endl;
            }
            else
            {
                cout << "No more attempts left. Returning to main menu." << endl;
            }
        }
    }
    return false;
}

void listUsers()
{
    fstream userFile;
    userFile.open("Users.txt", ios::in);

    string encryptedUsername, encryptedPassword;

    cout << "List of users:" << endl;
    cout << "-------------" << endl;

    if (userFile.is_open())
    {
        while (userFile >> encryptedUsername >> encryptedPassword)
        {
            string username = decrypt(encryptedUsername);
            cout << username << endl;
        }
        userFile.close();
    }
    cout << "-------------" << endl;
}

void removeUser(string usernameToRemove)
{
    fstream inFile("Users.txt", ios::in);
    fstream outFile("Temp.txt", ios::out);

    string encryptedUsername, encryptedPassword;

    bool userFound = false;

    if (inFile.is_open() && outFile.is_open())
    {
        while (inFile >> encryptedUsername >> encryptedPassword)
        {
            string username = decrypt(encryptedUsername);

            if (username != usernameToRemove)
            {
                outFile << encryptedUsername << " " << encryptedPassword << endl;
            }
            else
            {
                userFound = true;
            }
        }

        inFile.close();
        outFile.close();

        if (userFound)
        {
            remove("Users.txt");
            rename("Temp.txt", "Users.txt");

            cout << "User '" << usernameToRemove << "' has been removed." << endl;
        }
        else
        {
            remove("Temp.txt");

            cout << "User '" << usernameToRemove << "' not found." << endl;
        }
    }
}

void adminDashboard()
{
    int choice;
    do
    {
        cout << "\nAdmin Dashboard" << endl;
        cout << "1. List Users" << endl;
        cout << "2. Remove User" << endl;
        cout << "3. Back to Admin Menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            listUsers();
            break;
        case 2:
        {
            string usernameToRemove;
            cout << "Enter the username to remove: ";
            cin >> usernameToRemove;

            removeUser(usernameToRemove);
        }
        break;
        case 3:
            cout << "Returning to Admin Menu..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);
}
void adminAccess()
{
    int choice;
    do
    {
        cout << "\n1. Admin Sign up" << endl;
        cout << "2. Admin Login" << endl;
        cout << "3. Back to main menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            adminSignUp();
            break;
        case 2:
            if (adminLogin())
            {
                adminDashboard();
            }
            break;
        case 3:
            cout << "Returning to main menu..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);
}

bool authenticateUser(string username, string password)
{
    fstream userFile;

    userFile.open("Users.txt", ios::in);

    string storedUsername, storedPassword;

    if (userFile.is_open())
    {
        while (userFile >> storedUsername >> storedPassword)
        {
            string decryptedUsername = decrypt(storedUsername);
            string decryptedPassword = decrypt(storedPassword);

            if (decryptedUsername == username && decryptedPassword == password)
            {
                userFile.close();
                return true;
            }
        }
        userFile.close();
    }
    return false;
}

bool login()
{
    string username, password;
    cout << "Enter your username: ";
    cin >> username;

    if (!userExists(username))
    {
        cout << "User doesn't exist. Sign up first." << endl;
        return false;
    }

    int attempts = 3;
    while (attempts > 0)
    {
        cout << "Enter your password: ";
        cin >> password;

        if (authenticateUser(username, password))
        {
            return true;
        }
        else
        {
            attempts--;
            if (attempts > 0)
            {
                cout << "Incorrect password. You have " << attempts << " attempts left." << endl;
            }
            else
            {
                cout << "No more attempts left. Returning to main menu." << endl;
            }
        }
    }
    return false;
}

void userDashboard()
{
    cout << "Welcome to the Shop" << endl;
}

void userAccess()
{
    int choice;
    do
    {
        cout << "\n1. Sign up" << endl;
        cout << "2. Login" << endl;
        cout << "3. Back to main menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            signUp();
            break;
        case 2:
            if (login())
            {
                userDashboard();
            }
            break;
        case 3:
            cout << "Returning to main menu..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);
}
int main()
{
    int choice;

    do
    {
        cout << "\n1. Admin access" << endl;
        cout << "2. User access" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            adminAccess();
            break;
        case 2:
            userAccess();
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