#include <bits/stdc++.h>
#include <conio.h>

using namespace std;

const string KEY = "SECRETKEY";

string GetPasswordInput()
{
    string password = "";
    char ch;
    cout << "Enter password: ";
    while ((ch = _getch()) != 13)
    {
        if (ch == 8)
        {
            if (!password.empty())
            {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else
        {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

string Encrypt(string data)
{
    cout << "Encrypting data...." << endl;

    for (int i = 0; i < data.length(); i++)
    {
        data[i] = data[i] ^ KEY[i % KEY.length()];
    }
    return data;
}

string Decrypt(string data)
{
    cout << "Decrypting data...." << endl;
    for (int i = 0; i < data.length(); i++)
    {
        data[i] = data[i] ^ KEY[i % KEY.length()];
    }
    return data;
}

string EncryptDecrypt(string data)
{
    for (int i = 0; i < data.length(); i++)
    {
        data[i] = data[i] ^ KEY[i % KEY.length()];
    }
    return data;
}

bool AdminExists(string username)
{
    ifstream adminFile("Admins.txt", ios::binary);
    if (!adminFile.is_open())
    {
        cout << "Unable to open Admins.txt for reading" << endl;
        return false;
    }

    string line, storedUsername;
    cout << "Checking for username: '" << username << "'" << endl;
    cout << "File contents:" << endl;

    while (getline(adminFile, line))
    {
        cout << "Read line: '" << line << "'" << endl;
        stringstream ss(line);
        getline(ss, storedUsername, ',');

        cout << "Encrypted stored username: '" << storedUsername << "'" << endl;
        string decryptedUsername = EncryptDecrypt(storedUsername);
        cout << "Decrypted stored username: '" << decryptedUsername << "'" << endl;

        if (decryptedUsername == username)
        {
            adminFile.close();
            cout << "Admin found!" << endl;
            return true;
        }
    }

    if (adminFile.fail() && !adminFile.eof())
    {
        cout << "Failed to read file" << endl;
    }
    else
    {
        cout << "Reached end of file" << endl;
    }

    adminFile.close();
    cout << "Admin not found." << endl;
    return false;
}

void AdminSignUp()
{
    string username, password;

    cout << "Enter admin username: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, username);
    username.erase(username.find_last_not_of(" \n\r\t") + 1);

    if (AdminExists(username))
    {
        cout << "Admin username already exists. Choose a different username." << endl;
        return;
    }

    password = GetPasswordInput();
    password.erase(password.find_last_not_of(" \n\r\t") + 1);

    string encryptedUsername = Encrypt(username);
    string encryptedPassword = Encrypt(password);

    ofstream adminFile("Admins.txt", ios::app | ios::binary);
    if (adminFile.is_open())
    {
        adminFile << encryptedUsername << "," << encryptedPassword << "\n";
        if (adminFile.fail())
        {
            cout << "Error: Failed to write to file." << endl;
            adminFile.close();
            return;
        }
        adminFile.close();
        cout << "Admin sign up successful!" << endl;

        // Print file contents
        cout << "File contents after sign up:" << endl;
        ifstream readFile("Admins.txt", ios::binary);
        string line;
        while (getline(readFile, line))
        {
            cout << "'" << line << "'" << endl;
        }
        if (readFile.fail() && !readFile.eof())
        {
            cout << "Error: Failed to read file." << endl;
        }
        readFile.close();
    }
    else
    {
        cout << "Error: Unable to open Admins.txt for writing." << endl;
    }
}

bool AuthenticateAdmin(string username, string password)
{
    string filePath = "Admins.txt";
    ifstream adminFile(filePath, ios::binary);

    if (!adminFile.is_open())
    {
        cout << "Error: Unable to open " << filePath << endl;
        return false;
    }

    string line, storedUsername, storedPassword;
    while (getline(adminFile, line))
    {
        stringstream ss(line);
        getline(ss, storedUsername, ',');
        getline(ss, storedPassword);

        // Trim any whitespace from the end of storedPassword
        storedPassword.erase(storedPassword.find_last_not_of(" \n\r\t") + 1);

        string decryptedUsername = Decrypt(storedUsername);
        string decryptedPassword = Decrypt(storedPassword);

        // Trim any whitespace from the end of decryptedPassword
        decryptedPassword.erase(decryptedPassword.find_last_not_of(" \n\r\t") + 1);

        // Debug output
        cout << "Stored username: " << storedUsername << endl;
        cout << "Decrypted username: " << decryptedUsername << endl;
        cout << "Stored password: " << storedPassword << endl;
        cout << "Decrypted password: " << decryptedPassword << endl;

        if (decryptedUsername == username && decryptedPassword == password)
        {
            adminFile.close();
            return true;
        }
    }

    adminFile.close();
    return false;
}

bool AdminLogin()
{
    string username, password;

    cout << "Enter admin username: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, username);
    username.erase(username.find_last_not_of(" \n\r\t") + 1);

    cout << "Entered username: '" << username << "'" << endl;

    if (!AdminExists(username))
    {
        cout << "Admin doesn't exist. Sign up first." << endl;
        return false;
    }

    int attempts = 3;
    while (attempts > 0)
    {
        password = GetPasswordInput();
        password.erase(password.find_last_not_of(" \n\r\t") + 1);
        cout << "Entered password: '" << password << "'" << endl;

        cout << "Attempting to authenticate..." << endl;
        if (AuthenticateAdmin(username, password))
        {
            cout << "Admin login successful." << endl;
            return true;
        }
        else
        {
            attempts--;
            if (attempts > 0)
            {
                cout << "Incorrect credentials. You have " << attempts << " attempts left." << endl;
            }
            else
            {
                cout << "No more attempts left. Returning to main menu." << endl;
            }
        }
    }
    return false;
}

void ListUsers()
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
            string username = Decrypt(encryptedUsername);
            cout << username << endl;
        }
        userFile.close();
    }
    cout << "-------------" << endl;
}

void RemoveUser(string usernameToRemove)
{
    fstream inFile("Users.txt", ios::in);
    fstream outFile("Temp.txt", ios::out);

    string encryptedUsername, encryptedPassword;

    bool userFound = false;

    if (inFile.is_open() && outFile.is_open())
    {
        while (inFile >> encryptedUsername >> encryptedPassword)
        {
            string username = Decrypt(encryptedUsername);

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

void AddItem()
{
    string code, name, color;
    double price;
    int quantity;

    cout << "Enter item code: ";
    cin >> code;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter item name: ";
    getline(cin, name);

    cout << "Enter item color: ";
    getline(cin, color);

    cout << "Enter item price: ";
    cin >> price;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter item quantity: ";
    cin >> quantity;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    fstream itemFile;
    itemFile.open("Items.txt", ios::app);

    if (itemFile.is_open())
    {
        itemFile << code << "," << name << "," << color << "," << fixed << setprecision(2) << price << "," << quantity << endl;
        itemFile.close();
        cout << "Item added successfully!" << endl;
    }
    else
    {
        cout << "Error opening file. Item not added." << endl;
    }
}

void ViewItems()
{
    fstream itemFile;
    itemFile.open("Items.txt", ios::in);

    string line;
    cout << "\nList of Items:" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << setw(10) << "Code" << setw(20) << "Name" << setw(15) << "Color" << setw(10) << "Price" << setw(10) << "Quantity" << endl;
    cout << "----------------------------------------------------" << endl;

    if (itemFile.is_open())
    {
        while (getline(itemFile, line))
        {
            stringstream ss(line);
            string code, name, color, price, quantity;

            getline(ss, code, ',');
            getline(ss, name, ',');
            getline(ss, color, ',');
            getline(ss, price, ',');
            getline(ss, quantity, ',');

            cout << setw(10) << code << setw(20) << name << setw(15) << color << setw(10) << price << setw(10) << quantity << endl;
        }
        itemFile.close();
    }
    else
    {
        cout << "Error opening file. Unable to view items." << endl;
    }
    cout << "----------------------------------------------------" << endl;
}

void RemoveItem(string codeToRemove)
{
    fstream inFile("Items.txt", ios::in);
    fstream outFile("Temp.txt", ios::out);

    string line;
    bool itemFound = false;

    if (inFile.is_open() && outFile.is_open())
    {
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string code;
            getline(ss, code, ',');

            if (code != codeToRemove)
            {
                outFile << line << endl;
            }
            else
            {
                itemFound = true;
            }
        }

        inFile.close();
        outFile.close();

        if (itemFound)
        {
            remove("Items.txt");
            rename("Temp.txt", "Items.txt");
            cout << "Item with code '" << codeToRemove << "' has been removed." << endl;
        }
        else
        {
            remove("Temp.txt");
            cout << "Item with code '" << codeToRemove << "' not found." << endl;
        }
    }
    else
    {
        cout << "Error opening file. Unable to remove item." << endl;
    }
}

void SearchItem(string codeToSearch)
{
    fstream itemFile;
    itemFile.open("Items.txt", ios::in);

    string line;
    bool itemFound = false;

    cout << "\nSearch Results:" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << setw(10) << "Code" << setw(20) << "Name" << setw(15) << "Color" << setw(10) << "Price" << setw(10) << "Quantity" << endl;
    cout << "----------------------------------------------------" << endl;

    if (itemFile.is_open())
    {
        while (getline(itemFile, line))
        {
            stringstream ss(line);
            string code, name, color, price, quantity;

            getline(ss, code, ',');

            if (code == codeToSearch)
            {
                getline(ss, name, ',');
                getline(ss, color, ',');
                getline(ss, price, ',');
                getline(ss, quantity, ',');

                cout << setw(10) << code << setw(20) << name << setw(15) << color << setw(10) << price << setw(10) << quantity << endl;

                itemFound = true;
                break;
            }
        }
        itemFile.close();

        if (!itemFound)
        {
            cout << "Item with code '" << codeToSearch << "' not found." << endl;
        }
    }
    else
    {
        cout << "Error opening file. Unable to search for item." << endl;
    }
    cout << "----------------------------------------------------" << endl;
}

void UserManagement()
{
    int choice;
    do
    {
        cout << "\nUser Management" << endl;
        cout << "1. List Users" << endl;
        cout << "2. Remove User" << endl;
        cout << "3. Back to Admin Dashboard" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            ListUsers();
            break;
        case 2:
        {
            string usernameToRemove;
            cout << "Enter the username to remove: ";
            cin >> usernameToRemove;
            RemoveUser(usernameToRemove);
        }
        break;
        case 3:
            cout << "Returning to Admin Dashboard..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);
}

void ItemManagement()
{
    int choice;
    do
    {
        cout << "\nItem Management" << endl;
        cout << "1. Add Item" << endl;
        cout << "2. View Items" << endl;
        cout << "3. Remove Item" << endl;
        cout << "4. Search Item" << endl;
        cout << "5. Back to Admin Dashboard" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            AddItem();
            break;
        case 2:
            ViewItems();
            break;
        case 3:
        {
            string codeToRemove;
            cout << "Enter the item code to remove: ";
            cin >> codeToRemove;
            RemoveItem(codeToRemove);
        }
        break;
        case 4:
        {
            string codeToSearch;
            cout << "Enter the item code to search: ";
            cin >> codeToSearch;
            SearchItem(codeToSearch);
        }
        break;
        case 5:
            cout << "Returning to Admin Dashboard..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);
}
void AdminDashboard()
{
    int choice;
    do
    {
        cout << "\nAdmin Dashboard" << endl;
        cout << "1. User Management" << endl;
        cout << "2. Item Management" << endl;
        cout << "3. Back to Admin Menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            UserManagement();
            break;
        case 2:
            ItemManagement();
            break;
        case 3:
            cout << "Returning to Admin Menu..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);
}
void AdminAccess()
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
            AdminSignUp();
            break;
        case 2:
            if (AdminLogin())
            {
                AdminDashboard();
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

bool UserExists(string username)
{
    ifstream userFile("Users.txt", ios::binary);
    if (!userFile.is_open())
    {
        cout << "Unable to open Users.txt for reading" << endl;
        return false;
    }

    string line, storedUsername;
    cout << "Checking for username: '" << username << "'" << endl;
    cout << "File contents:" << endl;

    while (getline(userFile, line))
    {
        cout << "Read line: '" << line << "'" << endl;
        stringstream ss(line);
        getline(ss, storedUsername, ',');

        cout << "Encrypted stored username: '" << storedUsername << "'" << endl;
        string decryptedUsername = EncryptDecrypt(storedUsername);
        cout << "Decrypted stored username: '" << decryptedUsername << "'" << endl;

        if (decryptedUsername == username)
        {
            userFile.close();
            cout << "User found!" << endl;
            return true;
        }
    }

    if (userFile.fail() && !userFile.eof())
    {
        cout << "Failed to read file" << endl;
    }
    else
    {
        cout << "Reached end of file" << endl;
    }

    userFile.close();
    cout << "User not found." << endl;
    return false;
}

void UserSignUp()
{
    string username, password;

    cout << "Enter user username: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, username);
    username.erase(username.find_last_not_of(" \n\r\t") + 1);

    if (UserExists(username))
    {
        cout << "User username already exists. Choose a different username." << endl;
        return;
    }

    password = GetPasswordInput();
    password.erase(password.find_last_not_of(" \n\r\t") + 1);

    string encryptedUsername = Encrypt(username);
    string encryptedPassword = Encrypt(password);

    ofstream userFile("Users.txt", ios::app | ios::binary);
    if (userFile.is_open())
    {
        userFile << encryptedUsername << "," << encryptedPassword << "\n";
        if (userFile.fail())
        {
            cout << "Error: Failed to write to file." << endl;
            userFile.close();
            return;
        }
        userFile.close();
        cout << "User sign up successful!" << endl;

        // Print file contents
        cout << "File contents after sign up:" << endl;
        ifstream readFile("Users.txt", ios::binary);
        string line;
        while (getline(readFile, line))
        {
            cout << "'" << line << "'" << endl;
        }
        if (readFile.fail() && !readFile.eof())
        {
            cout << "Error: Failed to read file." << endl;
        }
        readFile.close();
    }
    else
    {
        cout << "Error: Unable to open Users.txt for writing." << endl;
    }
}

bool AuthenticateUser(string username, string password)
{
    string filePath = "Users.txt";
    ifstream userFile(filePath, ios::binary);

    if (!userFile.is_open())
    {
        cout << "Error: Unable to open " << filePath << endl;
        return false;
    }

    string line, storedUsername, storedPassword;
    while (getline(userFile, line))
    {
        stringstream ss(line);
        getline(ss, storedUsername, ',');
        getline(ss, storedPassword);

        // Trim any whitespace from the end of storedPassword
        storedPassword.erase(storedPassword.find_last_not_of(" \n\r\t") + 1);

        string decryptedUsername = Decrypt(storedUsername);
        string decryptedPassword = Decrypt(storedPassword);

        // Trim any whitespace from the end of decryptedPassword
        decryptedPassword.erase(decryptedPassword.find_last_not_of(" \n\r\t") + 1);

        // Debug output
        cout << "Stored username: " << storedUsername << endl;
        cout << "Decrypted username: " << decryptedUsername << endl;
        cout << "Stored password: " << storedPassword << endl;
        cout << "Decrypted password: " << decryptedPassword << endl;

        if (decryptedUsername == username && decryptedPassword == password)
        {
            userFile.close();
            return true;
        }
    }

    userFile.close();
    return false;
}

bool UserLogin()
{
    string username, password;

    cout << "Enter user username: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, username);
    username.erase(username.find_last_not_of(" \n\r\t") + 1);

    cout << "Entered username: '" << username << "'" << endl;

    if (!UserExists(username))
    {
        cout << "User doesn't exist. Sign up first." << endl;
        return false;
    }

    int attempts = 3;
    while (attempts > 0)
    {
        password = GetPasswordInput();
        password.erase(password.find_last_not_of(" \n\r\t") + 1);
        cout << "Entered password: '" << password << "'" << endl;

        cout << "Attempting to authenticate..." << endl;
        if (AuthenticateUser(username, password))
        {
            cout << "User login successful." << endl;
            return true;
        }
        else
        {
            attempts--;
            if (attempts > 0)
            {
                cout << "Incorrect credentials. You have " << attempts << " attempts left." << endl;
            }
            else
            {
                cout << "No more attempts left. Returning to main menu." << endl;
            }
        }
    }
    return false;
}

void UserViewItems()
{
    fstream itemFile;
    itemFile.open("Items.txt", ios::in);

    string line;
    cout << "\nList of Available Items:" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << setw(10) << "Code" << setw(20) << "Name" << setw(15) << "Color" << setw(10) << "Price" << endl;
    cout << "----------------------------------------------------" << endl;

    if (itemFile.is_open())
    {
        while (getline(itemFile, line))
        {
            stringstream ss(line);
            string code, name, color, price;
            int quantity;

            getline(ss, code, ',');
            getline(ss, name, ',');
            getline(ss, color, ',');
            getline(ss, price, ',');
            ss >> quantity;

            if (quantity > 0)
            {
                cout << setw(10) << code << setw(20) << name << setw(15) << color << setw(10) << price << endl;
            }
        }
        itemFile.close();
    }
    else
    {
        cout << "Error opening file. Unable to view items." << endl;
    }
    cout << "----------------------------------------------------" << endl;
}

void UserSearchItem(string codeToSearch)
{
    fstream itemFile;
    itemFile.open("Items.txt", ios::in);

    string line;
    bool itemFound = false;

    cout << "\nSearch Results:" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << setw(10) << "Code" << setw(20) << "Name" << setw(15) << "Color" << setw(10) << "Price" << endl;
    cout << "----------------------------------------------------" << endl;

    if (itemFile.is_open())
    {
        while (getline(itemFile, line))
        {
            stringstream ss(line);
            string code, name, color, price;
            int quantity;

            getline(ss, code, ',');

            if (code == codeToSearch)
            {
                getline(ss, name, ',');
                getline(ss, color, ',');
                getline(ss, price, ',');
                ss >> quantity;

                if (quantity > 0)
                {
                    cout << setw(10) << code << setw(20) << name << setw(15) << color << setw(10) << price << endl;
                    itemFound = true;
                }
                break;
            }
        }
        itemFile.close();

        if (!itemFound)
        {
            cout << "Item with code '" << codeToSearch << "' not found or out of stock." << endl;
        }
    }
    else
    {
        cout << "Error opening file. Unable to search for item." << endl;
    }
    cout << "----------------------------------------------------" << endl;
}
void UserDashboard()
{
    int choice;
    do
    {
        cout << "\nWelcome to the Shop" << endl;
        cout << "1. View Items" << endl;
        cout << "2. Search Item by Code" << endl;
        cout << "3. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            UserViewItems();
            break;
        case 2:
        {
            string codeToSearch;
            cout << "Enter the item code to search: ";
            cin >> codeToSearch;
            UserSearchItem(codeToSearch);
        }
        break;
        case 3:
            cout << "Logging out..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);
}

void UserAccess()
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
            UserSignUp();
            break;
        case 2:
            if (UserLogin())
            {
                UserDashboard();
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
            AdminAccess();
            break;
        case 2:
            UserAccess();
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