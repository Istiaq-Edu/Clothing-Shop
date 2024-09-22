

// --------------> Requesting to make your terminal full screen for best output results. <--------------

#include <bits/stdc++.h>
#include <conio.h>

using namespace std;

const string KEY = "SECRETKEY";
string username;

string GetPasswordInput()
{
    string password = "";
    char ch;

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
    // cout << "Encrypting data...." << endl;

    for (int i = 0; i < data.length(); i++)
    {
        char encryptedChar = data[i] ^ KEY[i % KEY.length()];
        // If the encrypted char is a comma, shift it by 1
        if (encryptedChar == ',')
        {
            encryptedChar++;
        }
        data[i] = encryptedChar;
    }
    return data;
}

string Decrypt(string data)
{
    // cout << "Decrypting data...." << endl;
    for (int i = 0; i < data.length(); i++)
    {
        // If the char is one after comma, shift it back
        if (data[i] == ',' + 1)
        {
            data[i]--;
        }
        data[i] = data[i] ^ KEY[i % KEY.length()];
    }
    return data;
}

string GenerateUniqueCode()
{
    const string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string uniqueCode;
    srand(time(0));
    for (int i = 0; i < 8; ++i)
    {
        uniqueCode += chars[rand() % chars.length()];
    }
    return uniqueCode;
}

bool AdminExists(string username)
{
    ifstream adminFile("Admins.txt", ios::in | ios::binary);
    if (!adminFile.is_open())
    {
        ofstream CreateFile("Admins.txt");
        CreateFile.close();

        adminFile.open("Admins.txt", ios::in);
        if (!adminFile.is_open())
        {
            cout << "\033[91m";
            cout << "=========================================";
            cout << "\nUnable to open Admins.txt for reading\n";
            cout << "=========================================";
            cout << "\033[0m" << endl;
            return false;
        }
    }

    string line, storedUsername;

    while (getline(adminFile, line))
    {
        stringstream ss(line);
        getline(ss, storedUsername, ',');

        string decryptedUsername = Decrypt(storedUsername);

        if (decryptedUsername == username)
        {
            adminFile.close();
            return true;
        }
    }

    adminFile.close();
    return false;
}

void AdminSignUp()
{
    string username, password, uniqueCode;

    cout << "\n\033[94m~~> Enter admin username: \033[0m";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, username);
    username.erase(username.find_last_not_of(" \n\r\t") + 1);

    if (AdminExists(username))
    {
        cout << "\033[91m";
        cout << "===========================================================";
        cout << "\nAdmin username already exists. Choose a different username.\n";
        cout << "===========================================================";
        cout << "\033[0m" << endl;
        return;
    }

    cout << "\n\033[94m~~> Enter password: \033[0m";
    password = GetPasswordInput();
    password.erase(password.find_last_not_of(" \n\r\t") + 1);

    uniqueCode = GenerateUniqueCode();
    cout << "\033[92m";
    cout << " _______________________________________________________________\n";
    cout << "|                                                               |\n";
    cout << "|                   UNIQUE CODE GENERATED                       |\n";
    cout << "|_______________________________________________________________|\n";
    cout << "|                                                               |\n";
    cout << "| Your unique code for password reset is: \033[1m" << uniqueCode << "\033[0m\033[92m              |\n";
    cout << "|_______________________________________________________________|\n";
    cout << "|                                                               |\n";
    cout << "| \033[93mPLEASE NOTE:                                                  \033[92m|\n";
    cout << "| \033[93m- Store this code in a safe and secure location.              \033[92m|\n";
    cout << "| \033[93m- You will need this code to reset your password in future.   \033[92m|\n";
    cout << "| \033[93m- Do not share this code with anyone.                         \033[92m|\n";
    cout << "|_______________________________________________________________|\n\n";
    cout << "\033[0m";

    string encryptedUsername = Encrypt(username);
    string encryptedPassword = Encrypt(password);
    string encryptedUniqueCode = Encrypt(uniqueCode);

    ofstream adminFile("Admins.txt", ios::app);
    if (adminFile.is_open())
    {
        adminFile << encryptedUsername << "," << encryptedPassword << "," << encryptedUniqueCode << "\n";
        if (adminFile.fail())
        {
            cout << "\033[91m";
            cout << "=========================================";
            cout << "\nFailed to write to file.\n";
            cout << "=========================================";
            cout << "\033[0m" << endl;
            adminFile.close();
            return;
        }
        adminFile.close();
        cout << "\033[92m";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout << "\nAdmin sign up successful!\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout << "\033[0m" << endl;
    }
    else
    {
        cout << "\033[91m";
        cout << "===========================================================";
        cout << "\nUnable to open or create Admins.txt for writing.\n";
        cout << "===========================================================";
        cout << "\033[0m" << endl;
    }
}

bool AuthenticateAdmin(string username, string password)
{
    string filePath = "Admins.txt";
    ifstream adminFile(filePath, ios::binary);

    if (!adminFile.is_open())
    {
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nUnable to open " << filePath << endl;
        cout << "=========================================";
        cout << "\033[0m" << endl;
        return false;
    }

    string line, storedUsername, storedPassword, storedUniqueCode;
    while (getline(adminFile, line))
    {
        stringstream ss(line);
        getline(ss, storedUsername, ',');
        getline(ss, storedPassword, ',');
        getline(ss, storedUniqueCode);

        storedPassword.erase(storedPassword.find_last_not_of(" \n\r\t") + 1);

        string decryptedUsername = Decrypt(storedUsername);
        string decryptedPassword = Decrypt(storedPassword);

        decryptedPassword.erase(decryptedPassword.find_last_not_of(" \n\r\t") + 1);

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

    cout << "\n\033[94m~~> Enter admin username: \033[0m";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, username);
    username.erase(username.find_last_not_of(" \n\r\t") + 1);

    if (!AdminExists(username))
    {
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nAdmin doesn't exist. Sign up first.\n";
        cout << "=========================================";
        cout << "\033[0m" << endl;
        return false;
    }

    int attempts = 3;
    while (attempts > 0)
    {
        cout << "\n\033[94m~~> Enter password: \033[0m";
        password = GetPasswordInput();
        password.erase(password.find_last_not_of(" \n\r\t") + 1);

        if (AuthenticateAdmin(username, password))
        {
            cout << "\033[92m";
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
            cout << "\nAdmin login successful.\n";
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
            cout << "\033[0m" << endl;
            ::username = username; // Set the global username
            return true;
        }
        else
        {
            attempts--;
            if (attempts > 0)
            {
                cout << "\033[91m";
                cout << "=============================================";
                cout << "\nIncorrect credentials. You have " << attempts << " attempts left.\n";
                cout << "=============================================";
                cout << "\033[0m" << endl;
            }
            else
            {
                cout << "\033[91m";
                cout << "=============================================";
                cout << "\nNo more attempts left. Returning to main menu.\n";
                cout << "=============================================";
                cout << "\033[0m" << endl;
            }
        }
    }
    return false;
}

bool UserExists(string username);

void RecoverPassword(bool Recover)
{
    string uniqueCode, newUsername, newPassword;
    string fileName;
    if (Recover)
    {
        fileName = "Admins.txt";
    }
    else
    {
        fileName = "Users.txt";
    }

    cout << "\n\033[94m~~> Enter your unique code: \033[0m";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, uniqueCode);
    uniqueCode.erase(uniqueCode.find_last_not_of(" \n\r\t") + 1);

    ifstream inFile(fileName, ios::binary);
    ofstream outFile("Temp.txt", ios::binary);

    if (!inFile.is_open() || !outFile.is_open())
    {
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nError opening files.\n";
        cout << "=========================================";
        cout << "\033[0m" << endl;
        return;
    }

    string line;
    bool userFound = false;

    while (getline(inFile, line))
    {
        stringstream ss(line);
        string storedUsername, storedPassword, storedUniqueCode;

        getline(ss, storedUsername, ',');
        getline(ss, storedPassword, ',');
        getline(ss, storedUniqueCode);

        storedUniqueCode.erase(storedUniqueCode.find_last_not_of(" \n\r\t") + 1);

        if (Decrypt(storedUniqueCode) == uniqueCode)
        {
            userFound = true;
            bool validUsername = false;
            while (!validUsername)
            {
                cout << "\n\033[94m~~> Enter new username: \033[0m";
                getline(cin, newUsername);
                newUsername.erase(newUsername.find_last_not_of(" \n\r\t") + 1);

                if ((Recover && AdminExists(newUsername)) || (!Recover && UserExists(newUsername)))
                {
                    cout << "\033[91m";
                    cout << "================================================================";
                    cout << "\nUsername already exists. Please choose a different username.\n";
                    cout << "================================================================";
                    cout << "\033[0m" << endl;
                }
                else
                {
                    validUsername = true;
                }
            }

            cout << "\n\033[94m~~> Enter new password: \033[0m";
            newPassword = GetPasswordInput();
            newPassword.erase(newPassword.find_last_not_of(" \n\r\t") + 1);

            outFile << Encrypt(newUsername) << "," << Encrypt(newPassword) << "," << storedUniqueCode << endl;
        }
        else
        {
            outFile << line << endl;
        }
    }

    inFile.close();
    outFile.close();

    if (userFound)
    {
        remove(fileName.c_str());
        rename("Temp.txt", fileName.c_str());

        cout << "\033[92m";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout << "\nUsername and password reset successful!\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout << "\033[0m" << endl;
    }
    else
    {
        remove("Temp.txt");

        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nUnique code not found.\n";
        cout << "=========================================";
        cout << "\033[0m" << endl;
    }
}

void ShowRecoveryCode(bool Recovery)
{
    string fileName = Recovery ? "Admins.txt" : "Users.txt";

    ifstream file(fileName, ios::binary);
    if (!file.is_open())
    {
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nUnable to open " << fileName << " for reading\n";
        cout << "=========================================";
        cout << "\033[0m" << endl;
        return;
    }

    string line, storedUsername, storedPassword, storedUniqueCode;
    bool userFound = false;

    while (getline(file, line))
    {
        stringstream ss(line);
        getline(ss, storedUsername, ',');
        getline(ss, storedPassword, ',');
        getline(ss, storedUniqueCode);

        storedUniqueCode.erase(storedUniqueCode.find_last_not_of(" \n\r\t") + 1);

        string decryptedUsername = Decrypt(storedUsername);

        if (decryptedUsername == ::username)
        {
            userFound = true;
            cout << "\033[93m";
            cout << "=======================================";
            cout << "\nYour recovery code is: " << Decrypt(storedUniqueCode);
            cout << "\n=======================================";
            cout << "\033[0m" << endl;
            break;
        }
    }

    file.close();

    if (!userFound)
    {
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nUsername not found in " << fileName << "\n";
        cout << "=========================================";
        cout << "\033[0m" << endl;
    }
}

void ListUsers()
{
    ifstream userFile("Users.txt", ios::binary);

    if (!userFile)
    {
        cout << "\033[91m";
        cout << "==============================================";
        cout << "\nNo users found. The Users.txt file doesn't exist.\n";
        cout << "==============================================";
        cout << "\033[0m" << endl;
        return;
    }

    string line;
    bool usersExist = false;

    cout << "\033[93m";
    cout << "\n.=========================================================." << endl;
    cout << "|  _     _     _            __   _   _                    |" << endl;
    cout << "| | |   (_)___| |_    ___  / _| | | | |___  ___ _ __ ___  |" << endl;
    cout << "| | |   | / __| __|  / _ \\| |_  | | | / __|/ _ \\ '__/ __| |" << endl;
    cout << "| | |___| \\__ \\ |_  | (_) |  _| | |_| \\__ \\  __/ |  \\__ \\ |" << endl;
    cout << "| |_____|_|___/\\__|  \\___/|_|    \\___/|___/\\___|_|  |___/ |" << endl;
    cout << "'========================================================='" << endl;
    cout << "\033[0m" << endl;

    while (getline(userFile, line))
    {
        if (!line.empty())
        {
            usersExist = true;
            stringstream ss(line);
            string encryptedUsername;
            getline(ss, encryptedUsername, ',');

            string username = Decrypt(encryptedUsername);
            cout << username << endl;
            cout << "\033[93m---------------------------------------------------------\033[0m" << endl;
        }
    }

    if (!usersExist)
    {
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nNo users listed yet.\n";
        cout << "=========================================";
        cout << "\033[0m" << endl;
    }

    userFile.close();
}

void RemoveUser()
{
    ifstream checkFile("Users.txt", ios::binary);
    if (!checkFile)
    {
        cout << "\033[91m";
        cout << "===========================================================";
        cout << "\nNo users available to remove. Users.txt does not exist.\n";
        cout << "===========================================================";
        cout << "\033[0m" << endl;
        return;
    }

    string line;
    bool usersExist = false;
    while (getline(checkFile, line))
    {
        string encryptedUsername;
        stringstream ss(line);
        if (getline(ss, encryptedUsername, ','))
        {
            string username = Decrypt(encryptedUsername);
            if (!username.empty())
            {
                usersExist = true;
                break;
            }
        }
    }
    checkFile.close();

    if (!usersExist)
    {
        cout << "\033[91m";
        cout << "=================================================================";
        cout << "\nNo users available to remove. Users.txt contains no valid users." << endl;
        cout << "=================================================================";
        cout << "\033[0m" << endl;
        return;
    }

    string usernameToRemove;

    cout << "\n\033[94m~~> Enter the username to remove: \033[0m";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, usernameToRemove);

    ifstream inFile("Users.txt", ios::binary);
    ofstream outFile("TempUsers.txt", ios::binary);

    if (!inFile.is_open() || !outFile.is_open())
    {
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nError opening files.\n";
        cout << "=========================================";
        cout << "\033[0m" << endl;
        return;
    }

    bool userFound = false;

    while (getline(inFile, line))
    {
        stringstream ss(line);
        string encryptedUsername;
        if (getline(ss, encryptedUsername, ','))
        {
            string username = Decrypt(encryptedUsername);
            if (username != usernameToRemove)
            {
                outFile << line << endl;
            }
            else
            {
                userFound = true;
            }
        }
    }

    inFile.close();
    outFile.close();

    if (userFound)
    {
        if (remove("Users.txt") != 0)
        {
            return;
        }
        if (rename("TempUsers.txt", "Users.txt") != 0)
        {
            return;
        }
        cout << "\033[92m";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout << "\nUser '" << usernameToRemove << "' has been removed.\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout << "\033[0m" << endl;
    }
    else
    {
        remove("TempUsers.txt");
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nUser '" << usernameToRemove << "' not found.\n";
        cout << "=========================================";
        cout << "\033[0m" << endl;
    }
}

void AddItem()
{
    string code, name, color, size;
    double price;
    int quantity;

    cout << "\n\033[94m~~> Enter item code: \033[0m";
    cin >> code;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n\033[94m~~> Enter item name: \033[0m";
    getline(cin, name);

    cout << "\n\033[94m~~> Enter item color: \033[0m";
    getline(cin, color);

    cout << "\n\033[94m~~> Enter item size: \033[0m";
    getline(cin, size);

    cout << "\n\033[94m~~> Enter item price: \033[0m";
    cin >> price;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n\033[94m~~> Enter item quantity: \033[0m";
    cin >> quantity;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    fstream itemFile;
    itemFile.open("Items.txt", ios::app);

    if (itemFile.is_open())
    {
        itemFile << code << "," << name << "," << color << "," << size << "," << fixed << setprecision(2) << price << "," << quantity << endl;
        itemFile.close();
        cout << "\033[92m";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout << "\nItem added successfully!\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout << "\033[0m" << endl;
    }
    else
    {
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nError opening file. Item not added.\n";
        cout << "=========================================";
        cout << "\033[0m" << endl;
    }
}

void ViewItems()
{
    ifstream checkFile("Items.txt");
    if (!checkFile)
    {
        cout << "\033[91m";
        cout << "============================================================";
        cout << "\nNo Items available to view. Items.txt does not exist.\n";
        cout << "============================================================";
        cout << "\033[0m" << endl;
        return;
    }

    string line;
    bool fileIsEmpty = true;
    while (getline(checkFile, line))
    {
        if (!line.empty())
        {
            fileIsEmpty = false;
            break;
        }
    }
    checkFile.close();

    if (fileIsEmpty)
    {
        cout << "\033[91m";
        cout << "====================================================";
        cout << "\nNo Items available to view. Items.txt is empty.\n";
        cout << "====================================================";
        cout << "\033[0m" << endl;
        return;
    }

    fstream itemFile;
    itemFile.open("Items.txt", ios::in);

    cout << "\033[93m";
    cout << "\n\n.^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^." << endl;
    cout << "|                      _     _     _            __   ___ _                                              |" << endl;
    cout << "|                     | |   (_)___| |_    ___  / _| |_ _| |_ ___ _ __ ___  ___                          |" << endl;
    cout << "|                     | |   | / __| __|  / _ \\| |_   | || __/ _ \\ '_ ` _ \\/ __|                         |" << endl;
    cout << "|                     | |___| \\__ \\ |_  | (_) |  _|  | || ||  __/ | | | | \\__ \\                         |" << endl;
    cout << "|                     |_____|_|___/\\__|  \\___/|_|   |___|\\__\\___|_| |_| |_|___/                         |" << endl;
    cout << "'vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv'" << endl;
    cout << "\033[0m" << endl;
    // cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";
    // cout << "\nList of Items:" << endl;
    cout << "\033[93m-------------------------------------------------------------------------------------------------------\033[0m" << endl;
    cout << setw(10) << "Code" << setw(30) << "Name" << setw(15) << "Color" << setw(15) << "Size" << setw(15) << "Price" << setw(15) << "Quantity" << endl;
    cout << "\033[93m-------------------------------------------------------------------------------------------------------\033[0m" << endl;

    if (itemFile.is_open())
    {
        while (getline(itemFile, line))
        {
            stringstream ss(line);
            string code, name, color, size, price, quantity;

            getline(ss, code, ',');
            getline(ss, name, ',');
            getline(ss, color, ',');
            getline(ss, size, ',');
            getline(ss, price, ',');
            getline(ss, quantity, ',');

            cout << setw(10) << code << setw(30) << name << setw(15) << color << setw(15) << size << setw(15) << price << setw(15) << quantity << endl;
            cout << "\033[93m-------------------------------------------------------------------------------------------------------\033[0m" << endl;
        }
        itemFile.close();
    }
    else
    {
        cout << "\033[91m";
        cout << "==========================================";
        cout << "\nError opening file. Unable to view items.\n";
        cout << "==========================================";
        cout << "\033[0m" << endl;
    }
}

void RemoveItem()
{
    ifstream checkFile("Items.txt");
    if (!checkFile)
    {
        cout << "\033[91m";
        cout << "============================================================";
        cout << "\nNo Items available to remove. Items.txt does not exist.\n";
        cout << "============================================================";
        cout << "\033[0m" << endl;
        return;
    }

    string line;
    bool fileIsEmpty = true;
    while (getline(checkFile, line))
    {
        if (!line.empty())
        {
            fileIsEmpty = false;
            break;
        }
    }
    checkFile.close();

    if (fileIsEmpty)
    {
        cout << "\033[91m";
        cout << "======================================================";
        cout << "\nNo Items available to remove. Items.txt is empty.\n";
        cout << "======================================================";
        cout << "\033[0m" << endl;
        return;
    }

    string codeToRemove;
    cout << "\n\033[94m~~> Enter item code to remove: \033[0m";
    cin >> codeToRemove;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    fstream inFile("Items.txt", ios::in);
    fstream outFile("Temp.txt", ios::out);

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
            cout << "\033[92m";
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
            cout << "\nItem with code '" << codeToRemove << "' has been removed.\n";
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
            cout << "\033[0m" << endl;
        }
        else
        {
            remove("Temp.txt");
            cout << "\033[91m";
            cout << "=========================================";
            cout << "\nItem with code '" << codeToRemove << "' not found.\n";
            cout << "=========================================";
            cout << "\033[0m" << endl;
        }
    }
    else
    {
        cout << "\033[91m";
        cout << "============================================";
        cout << "\nError opening file. Unable to remove item.\n";
        cout << "============================================";
        cout << "\033[0m" << endl;
    }
}

void SearchItem()
{
    ifstream checkFile("Items.txt");
    if (!checkFile)
    {
        cout << "\033[91m";
        cout << "============================================================";
        cout << "\nNo Items available to search. Items.txt does not exist.\n";
        cout << "============================================================";
        cout << "\033[0m" << endl;
        return;
    }

    string line;
    bool fileIsEmpty = true;
    while (getline(checkFile, line))
    {
        if (!line.empty())
        {
            fileIsEmpty = false;
            break;
        }
    }
    checkFile.close();

    if (fileIsEmpty)
    {
        cout << "\033[91m";
        cout << "===================================================";
        cout << "\nNo Items available to search. Items.txt is empty.\n";
        cout << "===================================================";
        cout << "\033[0m" << endl;
        return;
    }

    string codeToSearch;
    cout << "\n\033[94m~~> Enter item code to search: \033[0m";
    cin >> codeToSearch;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    fstream itemFile;
    itemFile.open("Items.txt", ios::in);

    bool itemFound = false;

    cout << "\033[93m";
    cout << "\n.^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^." << endl;
    cout << "|                    ____                      _                           _ _                          |" << endl;
    cout << "|                   / ___|  ___  __ _ _ __ ___| |__    _ __ ___  ___ _   _| | |_                        |" << endl;
    cout << "|                   \\___ \\ / _ \\/ _` | '__/ __| '_ \\  | '__/ _ \\/ __| | | | | __|                       |" << endl;
    cout << "|                    ___) |  __/ (_| | | | (__| | | | | | |  __/\\__ \\ |_| | | |_                        |" << endl;
    cout << "|                   |____/ \\___|\\__,_|_|  \\___|_| |_| |_|  \\___||___/\\__,_|_|\\__|                       |" << endl;
    cout << "'vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv'" << endl;
    cout << "\033[0m" << endl;

    cout << "\033[93m-------------------------------------------------------------------------------------------------------\033[0m" << endl;
    cout << setw(10) << "Code" << setw(30) << "Name" << setw(15) << "Color" << setw(15) << "Size" << setw(15) << "Price" << setw(15) << "Quantity" << endl;
    cout << "\033[93m-------------------------------------------------------------------------------------------------------\033[0m" << endl;

    if (itemFile.is_open())
    {
        while (getline(itemFile, line))
        {
            stringstream ss(line);
            string code, name, color, size, price, quantity;

            getline(ss, code, ',');

            if (code == codeToSearch)
            {
                getline(ss, name, ',');
                getline(ss, color, ',');
                getline(ss, size, ',');
                getline(ss, price, ',');
                getline(ss, quantity, ',');

                cout << setw(10) << code << setw(30) << name << setw(15) << color << setw(15) << size << setw(15) << price << setw(15) << quantity << endl;

                cout << "\033[93m-------------------------------------------------------------------------------------------------------\033[0m" << endl;

                itemFound = true;
                break;
            }
        }
        itemFile.close();

        if (!itemFound)
        {
            cout << "\033[91m";
            cout << "============================================";
            cout << "\nItem with code '" << codeToSearch << "' not found.\n";
            cout << "============================================";
            cout << "\033[0m" << endl;
        }
    }
    else
    {
        cout << "\033[91m";
        cout << "=================================================";
        cout << "\nError opening file. Unable to search for item.\n";
        cout << "=================================================";
        cout << "\033[0m" << endl;
    }
}

void ModifyItem()
{
    ifstream checkFile("Items.txt");
    if (!checkFile)
    {
        cout << "\033[91m";
        cout << "============================================================";
        cout << "\nNo Items available to modify. Items.txt does not exist.\n";
        cout << "============================================================";
        cout << "\033[0m" << endl;
        return;
    }

    string line;
    bool fileIsEmpty = true;
    while (getline(checkFile, line))
    {
        if (!line.empty())
        {
            fileIsEmpty = false;
            break;
        }
    }
    checkFile.close();

    if (fileIsEmpty)
    {
        cout << "\033[91m";
        cout << "===================================================";
        cout << "\nNo Items available to modify. Items.txt is empty.\n";
        cout << "===================================================";
        cout << "\033[0m" << endl;
        return;
    }

    string codeToModify;
    cout << "\n\033[94m~~> Enter the item code to modify: \033[0m";
    cin >> codeToModify;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    fstream itemFile("Items.txt", ios::in);
    fstream tempFile("temp.txt", ios::out);

    if (!itemFile.is_open() || !tempFile.is_open())
    {
        cout << "\033[91m";
        cout << "==============================================";
        cout << "\nError opening files. Unable to modify item.\n";
        cout << "==============================================";
        cout << "\033[0m" << endl;
        return;
    }

    bool itemFound = false;

    while (getline(itemFile, line))
    {
        stringstream ss(line);
        string code, name, color, size, price;
        int quantity;

        getline(ss, code, ',');
        getline(ss, name, ',');
        getline(ss, color, ',');
        getline(ss, size, ',');
        getline(ss, price, ',');
        ss >> quantity;

        if (code == codeToModify)
        {
            itemFound = true;
            int modifyChoice;

            cout << "\033[93m";
            cout << "\n\n.^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^." << endl;
            cout << "|                                    Current Item Details                                               |" << endl;
            cout << "'vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv'" << endl;
            cout << "\033[0m" << endl;

            cout << "\033[93m-------------------------------------------------------------------------------------------------------\033[0m" << endl;
            cout << setw(10) << "Code" << setw(30) << "Name" << setw(15) << "Color" << setw(15) << "Size" << setw(15) << "Price" << setw(15) << "Quantity" << endl;
            cout << "\033[93m-------------------------------------------------------------------------------------------------------\033[0m" << endl;
            cout << setw(10) << code << setw(30) << name << setw(15) << color << setw(15) << size << setw(15) << price << setw(15) << quantity << endl;
            cout << "\033[93m-------------------------------------------------------------------------------------------------------\033[0m" << endl;
            cout << "\033[0m" << endl;

            cout << "\033[96m+-------------------------------------+" << endl;
            cout << "|     What would you like to modify?  |" << endl;
            cout << "+-------------------------------------+\033[0m" << endl;

            cout << "\033[97m+-------------------------------------+" << endl;
            cout << "|  1. Code                            |" << endl;
            cout << "|  2. Name                            |" << endl;
            cout << "|  3. Color                           |" << endl;
            cout << "|  4. Size                            |" << endl;
            cout << "|  5. Price                           |" << endl;
            cout << "|  6. Quantity                        |" << endl;
            cout << "|  7. All fields                      |" << endl;
            cout << "+-------------------------------------+\033[0m" << endl;

            cout << "\n\n\033[34m> Enter your choice: \033[0m";
            cin >> modifyChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (modifyChoice)
            {
            case 1:
                cout << "\n\033[94m~~> Enter new code: \033[0m";
                getline(cin, code);
                break;
            case 2:
                cout << "\n\033[94m~~> Enter new name: \033[0m";
                getline(cin, name);
                break;
            case 3:
                cout << "\n\033[94m~~> Enter new color: \033[0m";
                getline(cin, color);
                break;
            case 4:
                cout << "\n\033[94m~~> Enter new size: \033[0m";
                getline(cin, size);
                break;
            case 5:
                cout << "\n\033[94m~~> Enter new price: \033[0m";
                cin >> price;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            case 6:
                cout << "\n\033[94m~~> Enter new quantity: \033[0m";
                cin >> quantity;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            case 7:
                cout << "\n\033[94m~~> Enter new code: \033[0m";
                getline(cin, code);
                cout << "\n\033[94m~~> Enter new name: \033[0m";
                getline(cin, name);
                cout << "\n\033[94m~~> Enter new color: \033[0m";
                getline(cin, color);
                cout << "\n\033[94m~~> Enter new size: \033[0m";
                getline(cin, size);
                cout << "\n\033[94m~~> Enter new price: \033[0m";
                cin >> price;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n\033[94m~~> Enter new quantity: \033[0m";
                cin >> quantity;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            default:
                cout << "\033[91m";
                cout << "=========================================";
                cout << "\nInvalid choice. No modifications made.\n";
                cout << "=========================================";
                cout << "\033[0m" << endl;
                tempFile << line << endl;
                continue;
            }

            tempFile << code << "," << name << "," << color << "," << size << "," << price << "," << quantity << endl;
            cout << "\033[92m";
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
            cout << "\nItem modified successfully!\n";
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
            cout << "\033[0m" << endl;
        }
        else
        {
            tempFile << line << endl;
        }
    }

    itemFile.close();
    tempFile.close();

    if (itemFound)
    {
        remove("Items.txt");
        rename("temp.txt", "Items.txt");
    }
    else
    {
        remove("temp.txt");
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nItem with code '" << codeToModify << "' not found.\n";
        cout << "=========================================";
        cout << "\033[0m" << endl;
    }
}

void TrackSale()
{
    ifstream checkFile("Items.txt");
    if (!checkFile)
    {
        cout << "\033[91m";
        cout << "============================================================";
        cout << "\nNo records available to view. Items.txt does not exist.\n";
        cout << "============================================================";
        cout << "\033[0m" << endl;
        return;
    }

    string line;
    bool fileIsEmpty = true;
    while (getline(checkFile, line))
    {
        if (!line.empty())
        {
            fileIsEmpty = false;
            break;
        }
    }
    checkFile.close();

    if (fileIsEmpty)
    {
        cout << "\033[91m";
        cout << "===================================================";
        cout << "\nNo records available to view. Items.txt is empty.\n";
        cout << "===================================================";
        cout << "\033[0m" << endl;
        return;
    }

    ifstream saleFile("Sales.txt");
    double totalRevenue = 0;

    cout << "\033[93m";
    cout << ".^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^." << endl;
    cout << "|                   ____        _             _____               _    _                                |" << endl;
    cout << "|                  / ___|  __ _| | ___  ___  |_   _| __ __ _  ___| | _(_)_ __   __ _                    |" << endl;
    cout << "|                  \\___ \\ / _` | |/ _ \\/ __|   | || '__/ _` |/ __| |/ / | '_ \\ / _` |                   |" << endl;
    cout << "|                   ___) | (_| | |  __/\\__ \\   | || | | (_| | (__|   <| | | | | (_| |                   |" << endl;
    cout << "|                  |____/ \\__,_|_|\\___||___/   |_||_|  \\__,_|\\___|_|\\_\\_|_| |_|\\__, |                   |" << endl;
    cout << "|                                                                              |___/                    |" << endl;
    cout << "'vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv'" << endl;
    cout << "\033[0m";

    cout << "\033[93m-------------------------------------------------------------------------------------------------------\033[0m" << endl;
    cout << setw(15) << "User" << setw(10) << "Code" << setw(20) << "Name" << setw(10) << "Color" << setw(10) << "Size"
         << setw(10) << "Price" << setw(10) << "Quantity" << setw(15) << "Total" << endl;
    cout << "\033[93m-------------------------------------------------------------------------------------------------------\033[0m" << endl;

    if (saleFile.is_open())
    {
        while (getline(saleFile, line))
        {
            stringstream ss(line);
            string username, code, name, color, size, price;
            int quantity;
            getline(ss, username, ',');
            getline(ss, code, ',');
            getline(ss, name, ',');
            getline(ss, color, ',');
            getline(ss, size, ',');
            getline(ss, price, ',');
            ss >> quantity;

            double itemPrice = stod(price);
            double total = itemPrice * quantity;
            totalRevenue += total;

            cout << setw(15) << username << setw(10) << code << setw(20) << name << setw(10) << color << setw(10) << size
                 << setw(10) << fixed << setprecision(2) << itemPrice << setw(10) << quantity << setw(15) << total << endl;

            cout << "\033[93m-------------------------------------------------------------------------------------------------------\033[0m" << endl;
        }
        saleFile.close();
    }
    else
    {
        cout << "\033[91m";
        cout << "===============================";
        cout << "\nUnable to open sales file.\n";
        cout << "===============================";
        cout << "\033[0m" << endl;
        return;
    }
    cout << "\033[93m";
    cout << "\nTotal Revenue: TK" << fixed << setprecision(2) << totalRevenue << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    cout << "\033[0m" << endl;
}

void UserManagement()
{
    int choice;
    do
    {
        cout << "\033[38;2;255;128;0m";
        cout << "\n\n/|=======================================================================================|\\\n";
        cout << "||                                                                                  _    ||\n";
        cout << "||  /\\ /\\  ___  ___ _ __    /\\/\\   __ _ _ __   __ _  __ _  ___ _ __ ___   ___ _ __ | |_  ||\n";
        cout << "|| / / \\ \\/ __|/ _ \\ '__|  /    \\ / _` | '_ \\ / _` |/ _` |/ _ \\ '_ ` _ \\ / _ \\ '_ \\| __| ||\n";
        cout << "|| \\ \\_/ /\\__ \\  __/ |    / /\\/\\ \\ (_| | | | | (_| | (_| |  __/ | | | | |  __/ | | | |_  ||\n";
        cout << "||  \\___/ |___/\\___|_|    \\/    \\/\\__,_|_| |_|\\__,_|\\__, |\\___|_| |_| |_|\\___|_| |_|\\__| ||\n";
        cout << "||                                                  |___/                                ||\n";
        cout << "||                                                                                       ||\n";
        cout << "||=======================================================================================||\n";
        cout << "||                                                                                       ||\n";
        cout << "|| 1. List Users                                                                         ||\n";
        cout << "|| 2. Remove User                                                                        ||\n";
        cout << "|| 3. Back to Admin Dashboard                                                            ||\n";
        cout << "||                                                                                       ||\n";
        cout << "\\|=======================================================================================|/\n";
        cout << "\033[0m";

        cout << "\n\n\033[34m> Enter your choice: \033[0m";
        cin >> choice;

        switch (choice)
        {
        case 1:
            ListUsers();
            break;
        case 2:
        {
            RemoveUser();
        }
        break;
        case 3:
            cout << "\033[93m";
            cout << "***********************************";
            cout << "\nReturning to Admin Dashboard...\n";
            cout << "***********************************";
            cout << "\033[0m" << endl;
            break;
        default:
            cout << "\033[91m";
            cout << "=========================================";
            cout << "\nInvalid choice. Please try again.\n";
            cout << "=========================================";
            cout << "\033[0m" << endl;
        }
    } while (choice != 3);
}

void ItemManagement()
{
    int choice;
    do
    {
        cout << "\033[38;2;255;128;0m";
        cout << "\n\n/|===============================================================================================|\\\n";
        cout << "||   _____ _                                                                                _    ||\n";
        cout << "||   \\_   \\ |_ ___ _ __ ___  ___    /\\/\\   __ _ _ __   __ _  __ _  ___ _ __ ___   ___ _ __ | |_  ||\n";
        cout << "||    / /\\/ __/ _ \\ '_ ` _ \\/ __|  /    \\ / _` | '_ \\ / _` |/ _` |/ _ \\ '_ ` _ \\ / _ \\ '_ \\| __| ||\n";
        cout << "|| /\\/ /_ | ||  __/ | | | | \\__ \\ / /\\/\\ \\ (_| | | | | (_| | (_| |  __/ | | | | |  __/ | | | |_  ||\n";
        cout << "|| \\____/  \\__\\___|_| |_| |_|___/ \\/    \\/\\__,_|_| |_|\\__,_|\\__, |\\___|_| |_| |_|\\___|_| |_|\\__| ||\n";
        cout << "||                                                          |___/                                ||\n";
        cout << "||                                                                                               ||\n";
        cout << "||===============================================================================================||\n";
        cout << "||                                                                                               ||\n";
        cout << "|| 1. Add Item                                                                                   ||\n";
        cout << "|| 2. View Items                                                                                 ||\n";
        cout << "|| 3. Remove Item                                                                                ||\n";
        cout << "|| 4. Search Item                                                                                ||\n";
        cout << "|| 5. Modify Item                                                                                ||\n";
        cout << "|| 6. Back to Admin Dashboard                                                                    ||\n";
        cout << "||                                                                                               ||\n";
        cout << "\\|===============================================================================================|/\n";
        cout << "\033[0m";

        cout << "\n\n\033[34m> Enter your choice: \033[0m";
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
            RemoveItem();
        }
        break;
        case 4:
        {
            SearchItem();
        }
        break;
        case 5:
            ModifyItem();
            break;
        case 6:
            cout << "\033[93m";
            cout << "***********************************";
            cout << "\nReturning to Admin Dashboard...\n";
            cout << "***********************************";
            cout << "\033[0m" << endl;
            break;
        default:
            cout << "\033[91m";
            cout << "=========================================";
            cout << "\nInvalid choice. Please try again.\n";
            cout << "=========================================";
            cout << "\033[0m" << endl;
        }
    } while (choice != 6);
}
void AdminDashboard()
{
    int choice;
    do
    {
        cout << "\033[38;2;255;0;255m";
        cout << "\n\n _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ \n";
        cout << "|_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_|\n";
        cout << "|_|                                                                                                |_|\n";
        cout << "|_|   _______  ______   _______ _________ _          _______  _______  _        _______  _         |_|\n";
        cout << "|_|  (  ___  )(  __  \\ (       )\\__   __/( (    /|  (  ____ )(  ___  )( (    /|(  ____ \\( \\        |_|\n";
        cout << "|_|  | (   ) || (  \\  )| () () |   ) (   |  \\  ( |  | (    )|| (   ) ||  \\  ( || (    \\/| (        |_|\n";
        cout << "|_|  | (___) || |   ) || || || |   | |   |   \\ | |  | (____)|| (___) ||   \\ | || (__    | |        |_|\n";
        cout << "|_|  |  ___  || |   | || |(_)| |   | |   | (\\ \\) |  |  _____)|  ___  || (\\ \\) ||  __)   | |        |_|\n";
        cout << "|_|  | (   ) || |   ) || |   | |   | |   | | \\   |  | (      | (   ) || | \\   || (      | |        |_|\n";
        cout << "|_|  | )   ( || (__/  )| )   ( |___) (___| )  \\  |  | )      | )   ( || )  \\  || (____/\\| (____/\\  |_|\n";
        cout << "|_|  |/     \\|(______/ |/     \\|\\_______/|/    )_)  |/       |/     \\||/    )_)(_______/(_______/  |_|\n";
        cout << "|_| _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ |_|\n";
        cout << "|_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_|\n";
        cout << "|_|                                                                                                |_|\n";
        cout << "|_| 1. User Management                                                                             |_|\n";
        cout << "|_| 2. Item Management                                                                             |_|\n";
        cout << "|_| 3. Track Sales                                                                                 |_|\n";
        cout << "|_| 4. Show Recovery Code                                                                          |_|\n";
        cout << "|_| 5. Back to Admin Menu                                                                          |_|\n";
        cout << "|_| _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ |_|\n";
        cout << "|_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_|\n";
        cout << "\033[0m";
        cout << "\n\n\033[34m> Enter your choice: \033[0m";
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
            TrackSale();
            break;
        case 4:
            ShowRecoveryCode(true);
            break;
        case 5:
            cout << "\033[93m";
            cout << "***********************************";
            cout << "\nReturning to Admin Menu...\n";
            cout << "***********************************";
            cout << "\033[0m" << endl;
            break;
        default:
            cout << "\033[91m";
            cout << "=========================================";
            cout << "\nInvalid choice. Please try again.\n";
            cout << "=========================================";
            cout << "\033[0m" << endl;
        }
    } while (choice != 5);
}

void AdminAccess()
{
    int choice;
    do
    {
        cout << "\033[38;2;0;255;255m";
        cout << " _____                                                                                       _____ \n";
        cout << "( ___ )-------------------------------------------------------------------------------------( ___ )\n";
        cout << " |   |                                                                                       |   | \n";
        cout << " |   | ____ ___  _  _ _ _  _    ____ _  _ ___ _  _ ____ _  _ ___ _ ____ ____ ___ _ ____ _  _ |   | \n";
        cout << " |   | |__| |  \\ |\\/| | |\\ |    |__| |  |  |  |__| |___ |\\ |  |  | |    |__|  |  | |  | |\\ | |   | \n";
        cout << " |   | |  | |__/ |  | | | \\|    |  | |__|  |  |  | |___ | \\|  |  | |___ |  |  |  | |__| | \\| |   | \n";
        cout << " |___|                                                                                       |___| \n";
        cout << " |___|---------------------------------------------------------------------------------------|___|\n";
        cout << " |   |                                                                                       |   | \n";
        cout << " |   | 1. Admin Sign up                                                                      |   |\n";
        cout << " |   | 2. Admin Login                                                                        |   |\n";
        cout << " |   | 3. Recover Password                                                                   |   |\n";
        cout << " |   | 4. Back to main menu                                                                  |   |\n";
        cout << " |___|                                                                                       |___|\n";
        cout << "(_____)-------------------------------------------------------------------------------------(_____)\n";
        cout << "\033[0m";
        cout << "\n\n\033[34m> Enter your choice: \033[0m";
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
            RecoverPassword(true);
            break;
        case 4:
            cout << "\033[93m";
            cout << "***********************************";
            cout << "\nReturning to main menu...\n";
            cout << "***********************************";
            cout << "\033[0m" << endl;
            break;
        default:
            cout << "\033[91m";
            cout << "=========================================";
            cout << "\nInvalid choice. Please try again.\n";
            cout << "=========================================";
            cout << "\033[0m" << endl;
        }
    } while (choice != 4);
}

bool UserExists(string username)
{
    ifstream userFile("Users.txt", ios::in | ios::binary);
    if (!userFile.is_open())
    {
        ofstream CreateFile("Users.txt");
        CreateFile.close();

        userFile.open("Users.txt", ios::in);
        if (!userFile.is_open())
        {
            cout << "\033[91m";
            cout << "=========================================";
            cout << "\nUnable to open Users.txt for reading\n";
            cout << "=========================================";
            cout << "\033[0m" << endl;
            return false;
        }
    }

    string line, storedUsername;

    while (getline(userFile, line))
    {
        stringstream ss(line);
        getline(ss, storedUsername, ',');

        string decryptedUsername = Decrypt(storedUsername);

        if (decryptedUsername == username)
        {
            userFile.close();
            return true;
        }
    }

    userFile.close();
    return false;
}

void UserSignUp()
{
    string username, password, uniqueCode;

    cout << "\n\033[94m~~>Enter user username: \033[0m";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, username);
    username.erase(username.find_last_not_of(" \n\r\t") + 1);

    if (UserExists(username))
    {
        cout << "\033[91m";
        cout << "================================================================";
        cout << "\nUser username already exists. Choose a different username.\n";
        cout << "================================================================";
        cout << "\033[0m" << endl;
        return;
    }

    cout << "\n\033[94m~~> Enter password: \033[0m";
    password = GetPasswordInput();
    password.erase(password.find_last_not_of(" \n\r\t") + 1);

    uniqueCode = GenerateUniqueCode();
    cout << "\033[92m";
    cout << " _______________________________________________________________\n";
    cout << "|                                                               |\n";
    cout << "|                   UNIQUE CODE GENERATED                       |\n";
    cout << "|_______________________________________________________________|\n";
    cout << "|                                                               |\n";
    cout << "| Your unique code for password reset is: \033[1m" << uniqueCode << "\033[0m\033[92m              |\n";
    cout << "|_______________________________________________________________|\n";
    cout << "|                                                               |\n";
    cout << "| \033[93mPLEASE NOTE:                                                  \033[92m|\n";
    cout << "| \033[93m- Store this code in a safe and secure location.              \033[92m|\n";
    cout << "| \033[93m- You will need this code to reset your password in future.   \033[92m|\n";
    cout << "| \033[93m- Do not share this code with anyone.                         \033[92m|\n";
    cout << "|_______________________________________________________________|\n";
    cout << "\033[0m";

    string encryptedUsername = Encrypt(username);
    string encryptedPassword = Encrypt(password);
    string encryptedUniqueCode = Encrypt(uniqueCode);

    ofstream userFile("Users.txt", ios::app);
    if (userFile.is_open())
    {
        userFile << encryptedUsername << "," << encryptedPassword << "," << encryptedUniqueCode << "\n";
        userFile.close();
        cout << "\033[92m";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout << "\nUser sign up successful!\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout << "\033[0m" << endl;
    }
    else
    {
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nUnable to open Users.txt for writing.\n";
        cout << "=========================================";
        cout << "\033[0m" << endl;
    }
}

bool AuthenticateUser(string username, string password)
{
    string filePath = "Users.txt";
    ifstream userFile(filePath, ios::binary);

    if (!userFile.is_open())
    {
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nUnable to open " << filePath << endl;
        cout << "=========================================";
        cout << "\033[0m" << endl;
        return false;
    }

    string line, storedUsername, storedPassword, storedUniqueCode;
    while (getline(userFile, line))
    {
        stringstream ss(line);
        getline(ss, storedUsername, ',');
        getline(ss, storedPassword, ',');
        getline(ss, storedUniqueCode);

        storedPassword.erase(storedPassword.find_last_not_of(" \n\r\t") + 1);

        string decryptedUsername = Decrypt(storedUsername);
        string decryptedPassword = Decrypt(storedPassword);

        decryptedPassword.erase(decryptedPassword.find_last_not_of(" \n\r\t") + 1);

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

    cout << "\n\033[94m~~>Enter user username: \033[0m";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, username);
    username.erase(username.find_last_not_of(" \n\r\t") + 1);

    if (!UserExists(username))
    {
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nUser doesn't exist. Sign up first.\n";
        cout << "=========================================";
        cout << "\033[0m" << endl;
        return false;
    }

    int attempts = 3;
    while (attempts > 0)
    {
        cout << "\n\033[94m~~> Enter password: \033[0m";
        password = GetPasswordInput();
        password.erase(password.find_last_not_of(" \n\r\t") + 1);

        if (AuthenticateUser(username, password))
        {
            cout << "\033[92m";
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
            cout << "\nUser login successful.\n";
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
            cout << "\033[0m" << endl;
            ::username = username;
            return true;
        }
        else
        {
            attempts--;
            if (attempts > 0)
            {
                cout << "\033[91m";
                cout << "================================================================";
                cout << "\nIncorrect credentials. You have " << attempts << " attempts left.\n";
                cout << "================================================================";
                cout << "\033[0m" << endl;
            }
            else
            {
                cout << "\033[91m";
                cout << "=================================================";
                cout << "\nNo more attempts left. Returning to main menu.\n";
                cout << "=================================================";
                cout << "\033[0m" << endl;
            }
        }
    }
    return false;
}

void UserViewItems()
{
    ifstream itemFile("Items.txt", ios::in);

    string line;

    cout << "\033[93m";
    cout << " __| |___________________________________________________________________________________________________________________________________| |__" << endl;
    cout << " __   ___________________________________________________________________________________________________________________________________   __" << endl;
    cout << "   | |                                                                                                                                   | |  " << endl;
    cout << "   | |  ....###....##.....##....###....####.##..........###....########..##.......########....####.########.########.##.....##..######.  | |  " << endl;
    cout << "   | |  ...##.##...##.....##...##.##....##..##.........##.##...##.....##.##.......##...........##.....##....##.......###...###.##....##  | |  " << endl;
    cout << "   | |  ..##...##..##.....##..##...##...##..##........##...##..##.....##.##.......##...........##.....##....##.......####.####.##......  | |  " << endl;
    cout << "   | |  .##.....##.##.....##.##.....##..##..##.......##.....##.########..##.......######.......##.....##....######...##.###.##..######.  | |  " << endl;
    cout << "   | |  .#########..##...##..#########..##..##.......#########.##.....##.##.......##...........##.....##....##.......##.....##.......##  | |  " << endl;
    cout << "   | |  .##.....##...##.##...##.....##..##..##.......##.....##.##.....##.##.......##...........##.....##....##.......##.....##.##....##  | |  " << endl;
    cout << "   | |  .##.....##....###....##.....##.####.########.##.....##.########..########.########....####....##....########.##.....##..######.  | |  " << endl;
    cout << " __| |___________________________________________________________________________________________________________________________________| |__" << endl;
    cout << " __   ___________________________________________________________________________________________________________________________________   __" << endl;
    cout << "   | |                                                                                                                                   | |  ";
    cout << "\033[0m" << endl;

    cout << setw(20) << "Code" << setw(30) << "Name" << setw(20) << "Color" << setw(20) << "Size" << setw(20) << "Price" << setw(20) << "Availability" << endl;
    cout << "\033[93m __| |___________________________________________________________________________________________________________________________________| |__\033[0m" << endl;

    if (itemFile.is_open())
    {
        while (getline(itemFile, line))
        {
            stringstream ss(line);
            string code, name, color, size, price;
            int quantity;

            getline(ss, code, ',');
            getline(ss, name, ',');
            getline(ss, color, ',');
            getline(ss, size, ',');
            getline(ss, price, ',');
            ss >> quantity;

            cout << setw(20) << code << setw(30) << name << setw(20) << color << setw(20) << size << setw(20) << price;

            if (quantity > 0)
            {
                cout << setw(20) << quantity << endl;
            }
            else
            {
                cout << setw(20) << "Unavailable\n";
            }
            cout << "\033[93m __| |___________________________________________________________________________________________________________________________________| |__\033[0m" << endl;
        }
        itemFile.close();
    }
    else
    {
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nError opening file. Unable to view items.\n";
        cout << "=========================================";
        cout << "\033[0m" << endl;
    }
}

void UserSearchItem(string codeToSearch)
{
    fstream itemFile;
    itemFile.open("Items.txt", ios::in);

    string line;
    bool itemFound = false;
    cout << "\033[93m";
    cout << "\n.^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^." << endl;
    cout << "|                    ____                      _                           _ _                          |" << endl;
    cout << "|                   / ___|  ___  __ _ _ __ ___| |__    _ __ ___  ___ _   _| | |_                        |" << endl;
    cout << "|                   \\___ \\ / _ \\/ _` | '__/ __| '_ \\  | '__/ _ \\/ __| | | | | __|                       |" << endl;
    cout << "|                    ___) |  __/ (_| | | | (__| | | | | | |  __/\\__ \\ |_| | | |_                        |" << endl;
    cout << "|                   |____/ \\___|\\__,_|_|  \\___|_| |_| |_|  \\___||___/\\__,_|_|\\__|                       |" << endl;
    cout << "'vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv'" << endl;
    cout << "\033[0m" << endl;

    cout << "\033[93m--------------------------------------------------------------------------------------------------------\033[0m" << endl;
    cout << setw(10) << "Code" << setw(30) << "Name" << setw(15) << "Color" << setw(15) << "Size" << setw(15) << "Price" << setw(15) << "Availability" << endl;
    cout << "\033[93m--------------------------------------------------------------------------------------------------------\033[0m" << endl;

    if (itemFile.is_open())
    {
        while (getline(itemFile, line))
        {
            stringstream ss(line);
            string code, name, color, size, price;
            int quantity;

            getline(ss, code, ',');

            if (code == codeToSearch)
            {
                getline(ss, name, ',');
                getline(ss, color, ',');
                getline(ss, size, ',');
                getline(ss, price, ',');
                ss >> quantity;

                cout << setw(10) << code << setw(30) << name << setw(15) << color << setw(15) << size << setw(15) << price;
                if (quantity > 0)
                {
                    cout << setw(15) << quantity << endl;
                }
                else
                {

                    cout << setw(15) << "Unavailable" << endl;
                }
                cout << "\033[93m--------------------------------------------------------------------------------------------------------\033[0m" << endl;

                itemFound = true;
                break;
            }
        }
        itemFile.close();

        if (!itemFound)
        {
            cout << "\033[91m";
            cout << "==============================================";
            cout << "\nItem with code '" << codeToSearch << "' not found.\n";
            cout << "==============================================";
            cout << "\033[0m" << endl;
        }
    }
    else
    {
        cout << "\033[91m";
        cout << "=================================================";
        cout << "\nError opening file. Unable to search for item.\n";
        cout << "=================================================";
        cout << "\033[0m" << endl;
    }
}

void AddToCart(string itemCode)
{
    fstream itemFile;
    itemFile.open("Items.txt", ios::in);
    fstream cartFile;
    cartFile.open("Cart.txt", ios::app);

    string line;
    bool itemFound = false;

    if (itemFile.is_open() && cartFile.is_open())
    {
        while (getline(itemFile, line))
        {
            stringstream ss(line);
            string code, name, color, size, price;
            int quantity;

            getline(ss, code, ',');
            if (code == itemCode)
            {
                getline(ss, name, ',');
                getline(ss, color, ',');
                getline(ss, size, ',');
                getline(ss, price, ',');
                ss >> quantity;

                if (quantity > 0)
                {
                    int quantityToAdd;
                    cout << "\n\033[94m~~>Enter quantity to add to cart (max " << quantity << "): \033[0m";
                    cin >> quantityToAdd;

                    if (quantityToAdd > 0 && quantityToAdd <= quantity)
                    {
                        cartFile << code << "," << name << "," << color << "," << size << "," << price << "," << quantityToAdd << "\n";
                        cout << "\033[92m";
                        cout << "=================================================";
                        cout << "\nAdded " << quantityToAdd << " of item " << name << " to cart.\n";
                        cout << "=================================================";
                        cout << "\033[0m" << endl;
                        itemFound = true;
                    }
                    else
                    {
                        cout << "\033[91m";
                        cout << "=============================================";
                        cout << "\nInvalid quantity. Item not added to cart.\n";
                        cout << "=============================================";
                        cout << "\033[0m" << endl;
                    }
                }
                else
                {
                    cout << "\033[91m";
                    cout << "==========================";
                    cout << "\nItem is out of stock.\n";
                    cout << "==========================";
                    cout << "\033[0m" << endl;
                }
                break;
            }
        }
        itemFile.close();
        cartFile.close();

        if (!itemFound)
        {
            cout << "\033[91m";
            cout << "=================================================";
            cout << "\nItem with code '" << itemCode << "' not found.\n";
            cout << "=================================================";
            cout << "\033[0m" << endl;
        }
    }
    else
    {
        cout << "\033[91m";
        cout << "=================================================";
        cout << "\nError opening files. Unable to add item to cart.\n";
        cout << "=================================================";
        cout << "\033[0m" << endl;
    }
}

void ViewCart()
{
    ifstream cartFile("Cart.txt");
    string line;
    double total = 0;

    cout << "\033[93m";
    cout << " :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: " << endl;
    cout << " :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: " << endl;
    cout << " ::                __  __   ______   __  __   ______        ______   ________   ______   _________                 :: " << endl;
    cout << " ::               /_/\\/_/\\ /_____/\\ /_/\\/_/\\ /_____/\\      /_____/\\ /_______/\\ /_____/\\ /________/\\                :: " << endl;
    cout << " ::               \\ \\ \\ \\ \\:::_ \\ \\:\\ \\:\\ \\:::_ \\ \\     \\:::__\\/ \\::: _  \\ \\:::_ \\ \\__.::.__\\/                :: " << endl;
    cout << " ::                \\:\\_\\ \\ \\:\\ \\ \\ \\:\\ \\:\\ \\:(_) ) )_    \\:\\ \\  __\\::(_)  \\ \\:(_) ) )_ \\::\\ \\                  :: " << endl;
    cout << " ::                 \\::::_\\/ \\:\\ \\ \\ \\:\\ \\:\\ \\: __ `\\ \\    \\:\\ \\/_/\\:: __  \\ \\: __ `\\ \\ \\::\\ \\                 :: " << endl;
    cout << " ::                   \\::\\ \\  \\:\\_\\ \\ \\:\\_\\:\\ \\ \\ `\\ \\ \\    \\:\\_\\ \\ \\:.\\ \\  \\ \\ \\ `\\ \\ \\ \\::\\ \\                :: " << endl;
    cout << " ::                    \\__\\/   \\_____\\/ \\_____\\/ \\_\\/ \\_\\/     \\_____\\/ \\__\\/\\__\\/ \\_\\/ \\_\\/  \\__\\/                :: " << endl;
    cout << " ::                                                                                                                :: " << endl;
    cout << " :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: " << endl;
    cout << " :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: " << endl;
    cout << "\033[0m" << endl;
    cout << "\033[93m______________________________________________________________________________________________________________________\033[0m" << endl;
    cout << setw(10) << "Code" << setw(30) << "Name" << setw(15) << "Color" << setw(15) << "Size" << setw(15) << "Price" << setw(15) << "Quantity" << setw(15) << "Subtotal" << endl;
    cout << "\033[93m______________________________________________________________________________________________________________________\033[0m" << endl;

    if (cartFile.is_open())
    {
        while (getline(cartFile, line))
        {
            stringstream ss(line);
            string code, name, color, size, price;
            int quantity;

            getline(ss, code, ',');
            getline(ss, name, ',');
            getline(ss, color, ',');
            getline(ss, size, ',');
            getline(ss, price, ',');
            ss >> quantity;

            double itemPrice = atof(price.c_str());
            double subtotal = itemPrice * quantity;
            total += subtotal;

            cout << setw(10) << code << setw(30) << name << setw(15) << color << setw(15) << size << setw(15) << fixed << setprecision(2) << itemPrice << setw(15) << quantity << setw(15) << subtotal << endl;
            cout << "\033[93m______________________________________________________________________________________________________________________\033[0m" << endl;
        }
        cartFile.close();
    }
    else
    {
        cout << "\033[91m";
        cout << "============================";
        cout << "\nUnable to open cart file.\n";
        cout << "============================";
        cout << "\033[0m" << endl;
        return;
    }

    cout << "\033[93m______________________________________________________________________________________________________________________\033[0m" << endl;
    cout << "\033[93m";
    cout << "Total: TK" << fixed << setprecision(2) << total << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "\033[0m" << endl;
}

void RemoveFromCart(string itemCode)
{
    ifstream cartFile("Cart.txt");
    ofstream tempFile("TempCart.txt");
    string line;
    bool itemFound = false;

    if (cartFile.is_open() && tempFile.is_open())
    {
        while (getline(cartFile, line))
        {
            stringstream ss(line);
            string code;
            getline(ss, code, ',');

            if (code != itemCode)
            {
                tempFile << line << endl;
            }
            else
            {
                itemFound = true;
            }
        }

        cartFile.close();
        tempFile.close();

        remove("Cart.txt");
        rename("TempCart.txt", "Cart.txt");

        if (itemFound)
        {
            cout << "\033[92m";
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
            cout << "\nItem removed from cart.\n";
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
            cout << "\033[0m" << endl;
        }
        else
        {
            cout << "\033[91m";
            cout << "============================";
            cout << "\nItem not found in cart.\n";
            cout << "============================";
            cout << "\033[0m" << endl;
        }
    }
    else
    {
        cout << "\033[91m";
        cout << "============================";
        cout << "\nUnable to open cart file.\n";
        cout << "============================";
        cout << "\033[0m" << endl;
    }
}

void CheckOut()
{

    ifstream cartFile("Cart.txt");
    ofstream saleFile("Sales.txt", ios::app);
    string line;
    double total = 0;

    cout << "\033[93m";
    cout << "  .+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.  " << endl;
    cout << " (                                                                                                                 ) " << endl;
    cout << "  )          ____  __  __  ____   ___  _   _    __    ___  ____  ____     ____  ____  ____  __  __  ___           (  " << endl;
    cout << " (          (  _ \\(  )(  )(  _ \\ / __)( )_( )  /__\\  / __)( ___)(  _ \\   (_  _)(_  _)( ___)(  \\/  )/ __)           ) " << endl;
    cout << "  )          )___/ )(__)(  )   /( (__  ) _ (  /(__)\\ \\__ \\ )__)  )(_) )   _)(_   )(   )__)  )    ( \\__ \\          (  " << endl;
    cout << " (          (__)  (______)(_)\\_) \\___)(_) (_)(__)(__)(___/(____)(____/   (____) (__) (____)(_/\\/\\_)(___/           ) " << endl;
    cout << "  )                                                                                                               (  " << endl;
    cout << " (                                                                                                                 ) " << endl;
    cout << "  '+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'+.+'  " << endl;
    cout << "\033[0m" << endl;
    cout << "\033[93m______________________________________________________________________________________________________________________\033[0m" << endl;
    cout << setw(10) << "Code" << setw(30) << "Name" << setw(15) << "Color" << setw(15) << "Size" << setw(15) << "Price" << setw(15) << "Quantity" << setw(15) << "Subtotal" << endl;
    cout << "\033[93m______________________________________________________________________________________________________________________\033[0m" << endl;

    if (cartFile.is_open() && saleFile.is_open())
    {
        while (getline(cartFile, line))
        {
            stringstream ss(line);
            string code, name, color, size, price;
            int quantity;
            getline(ss, code, ',');
            getline(ss, name, ',');
            getline(ss, color, ',');
            getline(ss, size, ',');
            getline(ss, price, ',');
            ss >> quantity;

            double itemPrice = stod(price);
            double subtotal = itemPrice * quantity;
            total += subtotal;

            cout << setw(10) << code << setw(30) << name << setw(15) << color << setw(15) << size << setw(15) << fixed << setprecision(2) << itemPrice << setw(10) << quantity << setw(15) << subtotal << endl;

            saleFile << ::username << "," << code << "," << name << "," << color << "," << size << "," << price << "," << quantity << "\n";
        }
        cartFile.close();
        saleFile.close();
    }
    else
    {
        cout << "\033[91m";
        cout << "=========================================";
        cout << "\nUnable to open cart or sales file.\n";
        cout << "=========================================";
        cout << "\033[0m" << endl;
        return;
    }

    cout << "\033[93m______________________________________________________________________________________________________________________\033[0m" << endl;
    cout << "\033[93m";
    cout << "Total Amount: TK" << fixed << setprecision(2) << total << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "\033[0m" << endl;

    ofstream clearCart("Cart.txt", ios::trunc);
    clearCart.close();

    cout << "\n\n\n\033[96m";
    cout << "                /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\ \n";
    cout << "               ( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )\n";
    cout << "                > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ < \n";
    cout << "                /\\_/\\                                                                                                                         /\\_/\\ \n";
    cout << "               ( o.o )       ::::::::::: :::    :::     :::     ::::    ::: :::    :::      :::   :::  ::::::::  :::    :::         :::      ( o.o )\n";
    cout << "                > ^ <           :+:     :+:    :+:   :+: :+:   :+:+:   :+: :+:   :+:       :+:   :+: :+:    :+: :+:    :+:         :+:        > ^ < \n";
    cout << "                /\\_/\\          +:+     +:+    +:+  +:+   +:+  :+:+:+  +:+ +:+  +:+         +:+ +:+  +:+    +:+ +:+    +:+         +:+         /\\_/\\ \n";
    cout << "               ( o.o )        +#+     +#++:++#++ +#++:++#++: +#+ +:+ +#+ +#++:++           +#++:   +#+    +:+ +#+    +:+         +#+         ( o.o )\n";
    cout << "                > ^ <        +#+     +#+    +#+ +#+     +#+ +#+  +#+#+# +#+  +#+           +#+    +#+    +#+ +#+    +#+         +#+           > ^ < \n";
    cout << "                /\\_/\\       #+#     #+#    #+# #+#     #+# #+#   #+#+# #+#   #+#          #+#    #+#    #+# #+#    #+#                        /\\_/\\ \n";
    cout << "               ( o.o )     ###     ###    ### ###     ### ###    #### ###    ###         ###     ########   ########          ###            ( o.o )\n";
    cout << "                > ^ <                                                                                                                         > ^ < \n";
    cout << "                /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\ \n";
    cout << "               ( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )\n";
    cout << "                > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ < \n";
    cout << "\033[0m";
    cout << endl;
}

void UserDashboard()
{
    int choice;
    do
    {
        cout << "\n\n\n\n\n\n\033[38;2;255;128;0m";
        cout << " _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ \n";
        cout << "(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)\n\n\n";
        cout << "$$\\      $$\\           $$\\                                          $$$$$$$$\\     $$$$$$$$\\ $$\\                  $$$$$$\\  $$\\                           \n";
        cout << "$$ |$$\\  $$ |          $$ |                                         \\__$$  __|    \\__$$  __|$$ |                $$  __$$\\ $$ |                          \n";
        cout << "$$ |$$$\\ $$ | $$$$$$\\  $$ | $$$$$$$\\  $$$$$$\\  $$$$$$\\$$$$\\   $$$$$$\\  $$ | $$$$$$\\  $$ |   $$$$$$$\\   $$$$$$\\  $$ /  \\__|$$$$$$$\\   $$$$$$\\   $$$$$$\\  \n";
        cout << "$$ $$ $$\\$$ |$$  __$$\\ $$ |$$  _____|$$  __$$\\ $$  _$$  _$$\\ $$  __$$\\ $$ |$$  __$$\\ $$ |   $$  __$$\\ $$  __$$\\ \\$$$$$$\\  $$  __$$\\ $$  __$$\\ $$  __$$\\ \n";
        cout << "$$$$  _$$$$ |$$$$$$$$ |$$ |$$ /      $$ /  $$ |$$ / $$ / $$ |$$$$$$$$ |$$ |$$ /  $$ |$$ |   $$ |  $$ |$$$$$$$$ | \\____$$\\ $$ |  $$ |$$ /  $$ |$$ /  $$ |\n";
        cout << "$$$  / \\$$$ |$$   ____|$$ |$$ |      $$ |  $$ |$$ | $$ | $$ |$$   ____|$$ |$$ |  $$ |$$ |   $$ |  $$ |$$   ____|$$\\   $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |\n";
        cout << "$$  /   \\$$ |\\$$$$$$$\\ $$ |\\$$$$$$$\\ \\$$$$$$  |$$ | $$ | $$ |\\$$$$$$$\\ $$ |\\$$$$$$  |$$ |   $$ |  $$ |\\$$$$$$$\\ \\$$$$$$  |$$ |  $$ |\\$$$$$$  |$$$$$$$  |\n";
        cout << "\\__/     \\__| \\_______|\\__| \\_______| \\______/ \\__| \\__| \\__| \\_______|\\__| \\______/ \\__|   \\__|  \\__| \\_______| \\______/ \\__|  \\__| \\______/ $$  ____/ \n";
        cout << "                                                                                                                                              $$ |      \n";
        cout << "                                                                                                                                              $$ |      \n";
        cout << "                                                                                                                                              \\__|      \n";
        cout << " _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ \n";
        cout << "(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)\n";
        cout << "(_)                                                                                                                                                   (_)\n";
        cout << "(_) 1. View Items                                                                                                                                     (_)\n";
        cout << "(_) 2. Search Item by Code                                                                                                                            (_)\n";
        cout << "(_) 3. Add to Cart                                                                                                                                    (_)\n";
        cout << "(_) 4. View Cart                                                                                                                                      (_)\n";
        cout << "(_) 5. Remove from Cart                                                                                                                               (_)\n";
        cout << "(_) 6. Checkout                                                                                                                                       (_)\n";
        cout << "(_) 7. Show Recovery Code                                                                                                                             (_)\n";
        cout << "(_) 8. Logout                                                                                                                                         (_)\n";
        cout << "(_) _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ (_)\n";
        cout << "(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)\n";
        cout << "\033[0m";

        cout << "\n\n\033[34m> Enter your choice: \033[0m";
        cin >> choice;

        switch (choice)
        {
        case 1:
            UserViewItems();
            break;
        case 2:
        {
            string codeToSearch;
            cout << "\n\033[94m~~>Enter the item code to search: \033[0m";
            cin >> codeToSearch;
            UserSearchItem(codeToSearch);
        }
        break;
        case 3:
        {
            string itemCode;
            cout << "\n\033[94m~~>Enter the item code to add to cart: \033[0m";
            cin >> itemCode;
            AddToCart(itemCode);
        }
        break;
        case 4:
            ViewCart();
            break;
        case 5:
        {
            string itemCode;
            cout << "\n\033[94m~~>Enter the item code to remove from cart: \033[0m";
            cin >> itemCode;
            RemoveFromCart(itemCode);
        }
        break;
        case 6:
            CheckOut();
            break;
        case 7:
            ShowRecoveryCode(false);
            break;
        case 8:
            cout << "\033[93m";
            cout << "***************************************";
            cout << "\n         Logging out...\n";
            cout << "***************************************";
            cout << "\033[0m" << endl;
            break;
        default:
            cout << "\033[91m";
            cout << "=========================================";
            cout << "\nInvalid choice. Please try again.\n";
            cout << "=========================================";
            cout << "\033[0m" << endl;
        }
    } while (choice != 8);
}

void UserAccess()
{
    int choice;
    do
    {
        cout << "\033[38;2;0;255;255m";
        cout << " _____                                                                                     _____ \n";
        cout << "( ___ )-----------------------------------------------------------------------------------( ___ )\n";
        cout << " |   |                                                                                     |   | \n";
        cout << " |   | _  _ ____ ____ ____    ____ _  _ ___ _  _ ____ _  _ ___ _ ____ ____ ___ _ ____ _  _ |   | \n";
        cout << " |   | |  | [__  |___ |__/    |__| |  |  |  |__| |___ |\\ |  |  | |    |__|  |  | |  | |\\ | |   | \n";
        cout << " |   | |__| ___] |___ |  \\    |  | |__|  |  |  | |___ | \\|  |  | |___ |  |  |  | |__| | \\| |   | \n";
        cout << " |___|                                                                                     |___| \n";
        cout << " |___|-------------------------------------------------------------------------------------|___|\n";
        cout << " |   |                                                                                     |   | \n";
        cout << " |   | 1. Sign up                                                                          |   | \n";
        cout << " |   | 2. Login                                                                            |   | \n";
        cout << " |   | 3. Recover Password                                                                 |   | \n";
        cout << " |   | 4. Back to main menu                                                                |   | \n";
        cout << " |___|                                                                                     |___|\n";
        cout << "(_____)-----------------------------------------------------------------------------------(_____)\n";
        cout << "\033[0m";
        cout << "\n\n\033[34m> Enter your choice: \033[0m";
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
            RecoverPassword(false);
            break;
        case 4:
            cout << "\033[93m";
            cout << "***********************************";
            cout << "\nReturning to main menu...\n";
            cout << "***********************************";
            cout << "\033[0m" << endl;
            break;
        default:
            cout << "\033[91m";
            cout << "=========================================";
            cout << "\nInvalid choice. Please try again.\n";
            cout << "=========================================";
            cout << "\033[0m" << endl;
        }
    } while (choice != 4);
}

int main()
{
    int choice;

    do
    {
        cout << "\033[38;2;255;215;0m";
        cout << "  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ \n";
        cout << " |_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_|\n";
        cout << " |_|                                                                              |_|\n";
        cout << " |_|  ___  ___      _        ______          _     _                         _    |_|\n";
        cout << " |_|  |  \\/  |     (_)       |  _  \\        | |   | |                       | |   |_|\n";
        cout << " |_|  | .  . | __ _ _ _ __   | | | |__ _ ___| |__ | |__   ___   __ _ _ __ __| |   |_|\n";
        cout << " |_|  | |\\/| |/ _` | | '_ \\  | | | / _` / __| '_ \\| '_ \\ / _ \\ / _` | '__/ _` |   |_|\n";
        cout << " |_|  | |  | | (_| | | | | | | |/ / (_| \\__ \\ | | | |_) | (_) | (_| | | | (_| |   |_|\n";
        cout << " |_|  \\_|  |_/\\__,_|_|_| |_| |___/ \\__,_|___/_| |_|_.__/ \\___/ \\__,_|_|  \\__,_|   |_|\n";
        cout << " |_| _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ |_|\n";
        cout << " |_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_|\n";
        cout << " |_|                                                                              |_|\n";
        cout << " |_| 1. Admin access                                                              |_|\n";
        cout << " |_| 2. User access                                                               |_|\n";
        cout << " |_| 3. Exit                                                                      |_|\n";
        cout << " |_|                                                                              |_|\n";
        cout << " |_| _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ |_|\n";
        cout << " |_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_|\n";
        cout << "\033[0m";
        cout << "\n\n\033[34m> Enter your choice: \033[0m";
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
            cout << "\033[93m";
            cout << "***********************************";
            cout << "\n          Exiting...\n";
            cout << "***********************************";
            cout << "\033[0m" << endl;
            break;
        default:
            cout << "\033[91m";
            cout << "=========================================";
            cout << "\nInvalid choice. Please try again.\n";
            cout << "=========================================";
            cout << "\033[0m" << endl;
        }
    } while (choice != 3);

    return 0;
}