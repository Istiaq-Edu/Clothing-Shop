#include <bits/stdc++.h>
#include <conio.h>

using namespace std;

string KEY = "SECRETKEY"; // Encryption key

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

// XOR encryption method.
string Encrypt(string data)
{
    cout << "Encrypting data...." << endl;

    for (int i = 0; i < data.length(); i++)
    {
        data[i] = data[i] ^ KEY[i % KEY.length()];
    }
    return data;
}

// XOR decryption method.
string Decrypt(string data)
{
    cout << "Decrypting data...." << endl;
    for (int i = 0; i < data.length(); i++)
    {
        data[i] = data[i] ^ KEY[i % KEY.length()];
    }
    return data;
}

bool UserExists(string username)
{
    fstream userFile;
    userFile.open("Users.txt", ios::in);

    string storedUsername, storedPassword;

    if (userFile.is_open())
    {
        cout << "Checking if user exists..." << endl;

        while (userFile >> storedUsername >> storedPassword)
        {
            string decryptedUsername = Decrypt(storedUsername);

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

void UserSignUp()
{
    string username, password;
    cout << "Enter your name: ";
    cin >> username;

    if (UserExists(username))
    {
        cout << "Username already exists. Choose a different username." << endl;
        return;
    }

    password = GetPasswordInput();

    string encryptedUsername = Encrypt(username);
    string encryptedPassword = Encrypt(password);

    fstream userFile;
    userFile.open("Users.txt", ios::app);

    if (userFile.is_open())
    {
        userFile << encryptedUsername << " " << encryptedPassword << endl;
        userFile.close();
        cout << "Sign up successful!" << endl;
    }
}

bool AdminExists(string username)
{
    fstream adminFile;
    adminFile.open("Admins.txt", ios::in);

    string storedUsername, storedPassword;

    if (adminFile.is_open())
    {
        cout << "Checking if admin exists..." << endl;

        while (adminFile >> storedUsername >> storedPassword)
        {
            string decryptedUsername = Decrypt(storedUsername);

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

void AdminSignUp()
{
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;

    if (AdminExists(username))
    {
        cout << "Admin username already exists. Choose a different username." << endl;
        return;
    }

    password = GetPasswordInput();

    string encryptedUsername = Encrypt(username);
    string encryptedPassword = Encrypt(password);

    fstream adminFile;

    adminFile.open("Admins.txt", ios::app);

    if (adminFile.is_open())
    {
        adminFile << encryptedUsername << " " << encryptedPassword << endl;
        adminFile.close();
        cout << "Admin sign up successful!" << endl;
    }
}

bool AuthenticateAdmin(string username, string password)
{
    fstream adminFile;

    adminFile.open("admins.txt", ios::in);

    string storedUsername, storedPassword;

    if (adminFile.is_open())
    {
        cout << "Authenticating admin..." << endl;

        while (adminFile >> storedUsername >> storedPassword)
        {
            string decryptedUsername = Decrypt(storedUsername);
            string decryptedPassword = Decrypt(storedPassword);

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

bool AdminLogin()
{
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;

    if (!AdminExists(username))
    {
        cout << "Admin doesn't exist. Sign up first." << endl;
        return false;
    }

    int attempts = 3;
    while (attempts > 0)
    {
        password = GetPasswordInput();

        if (AuthenticateAdmin(username, password))
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

void AdminDashboard()
{
    int choice;
    do
    {
        cout << "\nAdmin Dashboard" << endl;
        cout << "1. List Users" << endl;
        cout << "2. Remove User" << endl;
        cout << "3. Add Item" << endl;
        cout << "4. View Items" << endl;
        cout << "5. Remove Item" << endl;
        cout << "6. Search Item" << endl;
        cout << "7. Back to Admin Menu" << endl;
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
            AddItem();
            break;
        case 4:
            ViewItems();
            break;
        case 5:
        {
            string codeToRemove;
            cout << "Enter the item code to remove: ";
            cin >> codeToRemove;
            RemoveItem(codeToRemove);
        }
        break;
        case 6:
        {
            string codeToSearch;
            cout << "Enter the item code to search: ";
            cin >> codeToSearch;
            SearchItem(codeToSearch);
        }
        break;
        case 7:
            cout << "Returning to Admin Menu..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 7);
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

bool AuthenticateUser(string username, string password)
{
    fstream userFile;

    userFile.open("Users.txt", ios::in);

    string storedUsername, storedPassword;

    if (userFile.is_open())
    {
        while (userFile >> storedUsername >> storedPassword)
        {
            string decryptedUsername = Decrypt(storedUsername);
            string decryptedPassword = Decrypt(storedPassword);

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

bool UserLogin()
{
    string username, password;
    cout << "Enter your username: ";
    cin >> username;

    if (!UserExists(username))
    {
        cout << "User doesn't exist. Sign up first." << endl;
        return false;
    }

    int attempts = 3;
    while (attempts > 0)
    {
        password = GetPasswordInput();

        if (AuthenticateUser(username, password))
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