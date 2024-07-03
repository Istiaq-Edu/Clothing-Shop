#include <bits/stdc++.h>

using namespace std;

string KEY = "SECRETKEY"; // Encryption key

// XOR encryption method.
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

// XOR decryption method.
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

bool userExists(string username)
{
    fstream userFile;
    userFile.open("users.txt", ios::in);

    string line;

    if (userFile.is_open())
    {
        cout << "Checking if user exists..." << endl;

        while (getline(userFile, line))
        {
            stringstream ss(line);
            string storedUsername;
            /*
            What is stringstream?
            `stringstream` is part of the C++ Standard Library and is used to perform input and output operations on strings.
            It allows you to treat strings like streams, similar to how you use `cin` for input and `cout` for output.

            How stringstream Works?
            Create a `stringstream` Object:

            `stringstream ss(line);`

            This line creates a `stringstream` object named `ss` and initializes it with the string `line`.
            Now, `ss` behaves like an input stream that reads from the string `line`.

            Extract Data from the `stringstream`:

            `ss >> storedUsername;`
            This line extracts data from `ss` and stores it in the variable `storedUsername`.
            The extraction operator `>>` reads data from the stream up to the first whitespace (space, tab, newline, etc.)
            and assigns it to `storedUsername`.
            */
            ss >> storedUsername;

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
    cout << "Enter username: ";
    cin >> username;

    if (userExists(username))
    {
        cout << "Username already exists. Please choose a different username." << endl;
        return;
    }

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
}

bool adminExists(string username)
{
    fstream adminFile;
    adminFile.open("admins.txt", ios::in);

    string line;

    if (adminFile.is_open())
    {
        cout << "Checking if admin exists..." << endl;

        while (getline(adminFile, line))
        {
            stringstream ss(line);
            string storedUsername;
            ss >> storedUsername;

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

bool authenticateAdmin(string username, string password)
{
    fstream adminFile("admins.txt", ios::in);
    string storedUsername, storedPassword;

    if (adminFile.is_open())
    {
        cout << "Authenticating admin..." << endl;
        string line;

        while (getline(adminFile, line))
        {
            stringstream ss(line);

            if (getline(ss, storedUsername, ' ') && getline(ss, storedPassword))
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
        }
        adminFile.close();
        cout << "Admin authentication failed." << endl;
    }

    return false;
}

/*
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

*/
void adminSignUp()
{
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;

    if (adminExists(username))
    {
        cout << "Admin username already exists. Please choose a different username." << endl;
        return;
    }

    cout << "Enter admin password: ";
    cin >> password;

    string encryptedUsername = encrypt(username);
    string encryptedPassword = encrypt(password);

    fstream adminFile;
    adminFile.open("admins.txt", ios::app);
    if (adminFile.is_open())
    {
        cout << "Saving encrypted admin credentials to file..." << endl;
        adminFile << encryptedUsername << " " << encryptedPassword << endl;
        adminFile.close();
        cout << "Admin sign up successful!" << endl;
    }
}

bool adminLogin()
{
    string username, password;
    cout << "Please enter admin username: ";
    cin >> username;

    if (!adminExists(username))
    {
        cout << "Admin doesn't exist. Please sign up first." << endl;
        return false;
    }

    int attempts = 3;
    while (attempts > 0)
    {
        cout << "Please enter admin password: ";
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
    userFile.open("users.txt", ios::in);
    string line;
    cout << "List of users:" << endl;
    cout << "-------------" << endl;
    if (userFile.is_open())
    {
        while (getline(userFile, line))
        {
            stringstream ss(line);
            string encryptedUsername;
            ss >> encryptedUsername;
            string username = decrypt(encryptedUsername);
            cout << username << endl;
        }
        userFile.close();
    }
    else
    {
        cout << "Unable to open users file." << endl;
    }
    cout << "-------------" << endl;
}

void removeUser(string usernameToRemove)
{
    fstream inFile("users.txt", ios::in);
    fstream outFile("users_temp.txt", ios::out);
    string line;
    bool userFound = false;

    if (inFile.is_open() && outFile.is_open())
    {
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string encryptedUsername;
            ss >> encryptedUsername;
            string username = decrypt(encryptedUsername);

            if (username != usernameToRemove)
            {
                outFile << line << endl;
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
            remove("users.txt");
            rename("users_temp.txt", "users.txt");
            cout << "User '" << usernameToRemove << "' has been removed." << endl;
        }
        else
        {
            remove("users_temp.txt");
            cout << "User '" << usernameToRemove << "' not found." << endl;
        }
    }
    else
    {
        cout << "Unable to open files." << endl;
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

bool login()
{
    string username, password;
    cout << "Please enter your username: ";
    cin >> username;

    if (!userExists(username))
    {
        cout << "User doesn't exist. Please sign up first." << endl;
        return false;
    }

    int attempts = 3;
    while (attempts > 0)
    {
        cout << "Please enter your password: ";
        cin >> password;

        if (authenticate(username, password))
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
    cout << "Welcome to User Dashboard" << endl;
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