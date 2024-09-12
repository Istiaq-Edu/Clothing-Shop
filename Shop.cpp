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
    // cout << "Encrypting data...." << endl;

    for (int i = 0; i < data.length(); i++)
    {
        data[i] = data[i] ^ KEY[i % KEY.length()];
    }
    return data;
}

string Decrypt(string data)
{
    // cout << "Decrypting data...." << endl;
    for (int i = 0; i < data.length(); i++)
    {
        data[i] = data[i] ^ KEY[i % KEY.length()];
    }
    return data;
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
            cout << "Unable to open Admins.txt for reading" << endl;
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

    ofstream adminFile("Admins.txt", ios::app);
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
    }
    else
    {
        cout << "Error: Unable to open or create Admins.txt for writing." << endl;
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

    cout << "Enter admin username: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, username);
    username.erase(username.find_last_not_of(" \n\r\t") + 1);

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
    ifstream userFile("Users.txt");

    if (!userFile)
    {
        ofstream createFile("Users.txt");
        createFile.close();
        userFile.open("Users.txt");
    }

    string line;
    bool usersExist = false;

    cout << "List of users:" << endl;
    cout << "---------------------------------" << endl;

    if (userFile.is_open())
    {
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
            }
        }
        userFile.close();

        if (!usersExist)
        {
            cout << "No users listed yet." << endl;
        }
    }
    else
    {
        cout << "Unable to open Users.txt" << endl;
    }
    cout << "---------------------------------" << endl;
}

void RemoveUser()
{
    ifstream checkFile("Users.txt");
    if (!checkFile)
    {
        cout << "\nNo users available to remove. Users.txt does not exist." << endl;
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
        cout << "\nNo users available to remove. Users.txt is empty." << endl;
        return;
    }

    string usernameToRemove;
    cout << "Enter the username to remove: ";
    cin >> usernameToRemove;

    fstream inFile("Users.txt", ios::in);
    fstream outFile("Temp.txt", ios::out);

    string encryptedUsername, encryptedPassword;
    bool userFound = false;

    if (inFile.is_open() && outFile.is_open())
    {
        while (getline(inFile, line))
        {
            if (!line.empty())
            {
                stringstream ss(line);
                getline(ss, encryptedUsername, ',');
                getline(ss, encryptedPassword);

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
    else
    {
        cout << "Error opening files." << endl;
    }
}

void AddItem()
{
    string code, name, color, size;
    double price;
    int quantity;

    cout << "Enter item code: ";
    cin >> code;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter item name: ";
    getline(cin, name);

    cout << "Enter item color: ";
    getline(cin, color);

    cout << "Enter item size: ";
    getline(cin, size);

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
        itemFile << code << "," << name << "," << color << "," << size << "," << fixed << setprecision(2) << price << "," << quantity << endl;
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
    ifstream checkFile("Items.txt");
    if (!checkFile)
    {
        cout << "\nNo Items available to view. Items.txt does not exist." << endl;
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
        cout << "\nNo Items available to view. Items.txt is empty." << endl;
        return;
    }

    fstream itemFile;
    itemFile.open("Items.txt", ios::in);

    // string line;
    cout << "\nList of Items:" << endl;
    cout << "-------------------------------------------------------------------------------------------------------" << endl;
    cout << setw(10) << "Code" << setw(30) << "Name" << setw(15) << "Color" << setw(15) << "Size" << setw(15) << "Price" << setw(15) << "Quantity" << endl;
    cout << "-------------------------------------------------------------------------------------------------------" << endl;

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
            cout << "-------------------------------------------------------------------------------------------------------" << endl;
        }
        itemFile.close();
    }
    else
    {
        cout << "Error opening file. Unable to view items." << endl;
    }
    // cout << "-------------------------------------------------------------------------------------------------------" << endl;
}

void RemoveItem()
{
    ifstream checkFile("Items.txt");
    if (!checkFile)
    {
        cout << "\nNo Items available to remove. Items.txt does not exist." << endl;
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
        cout << "\nNo Items available to remove. Items.txt is empty." << endl;
        return;
    }

    string codeToRemove;
    cout << "Enter item code to remove: ";
    cin >> codeToRemove;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    fstream inFile("Items.txt", ios::in);
    fstream outFile("Temp.txt", ios::out);

    // string line;
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

void SearchItem()
{
    ifstream checkFile("Items.txt");
    if (!checkFile)
    {
        cout << "\nNo Items available to search. Items.txt does not exist." << endl;
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
        cout << "\nNo Items available to search. Items.txt is empty." << endl;
        return;
    }

    string codeToSearch;
    cout << "Enter item code to search: ";
    cin >> codeToSearch;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    fstream itemFile;
    itemFile.open("Items.txt", ios::in);

    // string line;
    bool itemFound = false;

    cout << "\nSearch Results:" << endl;
    cout << "----------------------------------------------------------------------------------------" << endl;
    cout << setw(10) << "Code" << setw(30) << "Name" << setw(15) << "Color" << setw(15) << "Size" << setw(15) << "Price" << setw(15) << "Quantity" << endl;
    cout << "----------------------------------------------------------------------------------------" << endl;

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

                cout << "----------------------------------------------------------------------------------------" << endl;

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
    // cout << "----------------------------------------------------------------------------------------" << endl;
}

void ModifyItem()
{
    ifstream checkFile("Items.txt");
    if (!checkFile)
    {
        cout << "\nNo Items available to modify. Items.txt does not exist." << endl;
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
        cout << "\no Items available to modify. Items.txt is empty." << endl;
        return;
    }

    string codeToModify;
    cout << "Enter the item code to modify: ";
    cin >> codeToModify;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    fstream itemFile("Items.txt", ios::in);
    fstream tempFile("temp.txt", ios::out);

    if (!itemFile.is_open() || !tempFile.is_open())
    {
        cout << "Error opening files. Unable to modify item." << endl;
        return;
    }

    // string line;
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
            cout << "\nCurrent item details:" << endl;
            cout << "Code: " << code << ", Name: " << name << ", Color: " << color
                 << ", Size: " << size << ", Price: " << price << ", Quantity: " << quantity << endl;

            cout << "\nWhat would you like to modify?" << endl;
            cout << "1. Code\n2. Name\n3. Color\n4. Size\n5. Price\n6. Quantity\n7. All fields" << endl;
            cout << "Enter your choice: ";
            cin >> modifyChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (modifyChoice)
            {
            case 1:
                cout << "Enter new code: ";
                getline(cin, code);
                break;
            case 2:
                cout << "Enter new name: ";
                getline(cin, name);
                break;
            case 3:
                cout << "Enter new color: ";
                getline(cin, color);
                break;
            case 4:
                cout << "Enter new size: ";
                getline(cin, size);
                break;
            case 5:
                cout << "Enter new price: ";
                cin >> price;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            case 6:
                cout << "Enter new quantity: ";
                cin >> quantity;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            case 7:
                cout << "Enter new code: ";
                getline(cin, code);
                cout << "Enter new name: ";
                getline(cin, name);
                cout << "Enter new color: ";
                getline(cin, color);
                cout << "Enter new size: ";
                getline(cin, size);
                cout << "Enter new price: ";
                cin >> price;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter new quantity: ";
                cin >> quantity;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            default:
                cout << "Invalid choice. No modifications made." << endl;
                tempFile << line << endl;
                continue;
            }

            tempFile << code << "," << name << "," << color << "," << size << "," << price << "," << quantity << endl;
            cout << "Item modified successfully!" << endl;
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
        cout << "Item with code '" << codeToModify << "' not found." << endl;
    }
}

void TrackSale()
{
    ifstream checkFile("Items.txt");
    if (!checkFile)
    {
        cout << "\nNo records available to view. Items.txt does not exist." << endl;
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
        cout << "\nNo records available to view. Items.txt is empty." << endl;
        return;
    }

    ifstream saleFile("Sales.txt");
    // string line;
    double totalRevenue = 0;

    cout << "\nSales Tracking:" << endl;
    cout << "-----------------------------------------------------------------------------------------------------" << endl;
    cout << setw(10) << "Code" << setw(30) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << setw(10) << "Subtotal" << endl;
    cout << "-----------------------------------------------------------------------------------------------------" << endl;

    if (saleFile.is_open())
    {
        while (getline(saleFile, line))
        {
            stringstream ss(line);
            string code, name, price;
            int quantity;
            getline(ss, code, ',');
            getline(ss, name, ',');
            getline(ss, price, ',');
            ss >> quantity;

            double itemPrice = stod(price);
            double subtotal = itemPrice * quantity;
            totalRevenue += subtotal;

            cout << setw(10) << code << setw(30) << name << setw(10) << fixed << setprecision(2) << itemPrice
                 << setw(10) << quantity << setw(15) << subtotal << endl;

            cout << "-----------------------------------------------------------------------------------------------------" << endl;
        }
        saleFile.close();
    }
    else
    {
        cout << "Unable to open sales file." << endl;
        return;
    }

    // cout << "------------------------------------------------------------------------------------------------" << endl;
    cout << "Total Revenue: TK" << fixed << setprecision(2) << totalRevenue << endl;
}

void UserManagement()
{
    int choice;
    do
    {
        cout << "\033[38;2;255;128;0m";
        cout << "/|=======================================================================================|\\\n";
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
        cout << "\033[38;2;255;128;0m";
        cout << "/|===============================================================================================|\\\n";
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
            // string codeToRemove;
            // cout << "Enter the item code to remove: ";
            // cin >> codeToRemove;
            RemoveItem();
        }
        break;
        case 4:
        {
            // string codeToSearch;
            // cout << "Enter the item code to search: ";
            // cin >> codeToSearch;
            SearchItem();
        }
        break;
        case 5:
            ModifyItem();
            break;
        case 6:
            cout << "Returning to Admin Dashboard..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);
}
void AdminDashboard()
{
    int choice;
    do
    {
        cout << "\033[38;2;255;0;255m";
        cout << " _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ \n";
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
        cout << "|_| 4. Back to Admin Menu                                                                          |_|\n";
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
            cout << "Returning to Admin Menu..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 4);
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
        cout << " |   | 3. Back to main menu                                                                  |   |\n";
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
            cout << "Returning to main menu..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);
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
            cout << "Unable to open Users.txt for reading" << endl;
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

    ofstream userFile("Users.txt", ios::app);
    if (userFile.is_open())
    {
        userFile << encryptedUsername << "," << encryptedPassword << "\n";
        userFile.close();
        cout << "User sign up successful!" << endl;
    }
    else
    {
        cout << "Unable to open Users.txt for writing." << endl;
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

    cout << "Enter user username: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, username);
    username.erase(username.find_last_not_of(" \n\r\t") + 1);

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
    cout << "----------------------------------------------------------------------------------------" << endl;
    cout << setw(10) << "Code" << setw(20) << "Name" << setw(15) << "Color" << setw(10) << "Size" << setw(10) << "Price" << setw(15) << "Availability" << endl;
    cout << "----------------------------------------------------------------------------------------" << endl;

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

            cout << setw(10) << code << setw(20) << name << setw(15) << color << setw(10) << size << setw(10) << price;
            if (quantity > 0)
            {
                cout << setw(15) << quantity << endl;
            }
            else
            {
                cout << setw(15) << "Unavailable" << endl;
            }
        }
        itemFile.close();
    }
    else
    {
        cout << "Error opening file. Unable to view items." << endl;
    }
    cout << "----------------------------------------------------------------------------------------" << endl;
}

void UserSearchItem(string codeToSearch)
{
    fstream itemFile;
    itemFile.open("Items.txt", ios::in);

    string line;
    bool itemFound = false;

    cout << "\nSearch Results:" << endl;
    cout << "----------------------------------------------------------------------------------------" << endl;
    cout << setw(10) << "Code" << setw(20) << "Name" << setw(15) << "Color" << setw(10) << "Size" << setw(10) << "Price" << setw(15) << "Availability" << endl;
    cout << "----------------------------------------------------------------------------------------" << endl;

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

                cout << setw(10) << code << setw(20) << name << setw(15) << color << setw(10) << size << setw(10) << price;
                if (quantity > 0)
                {
                    cout << setw(15) << quantity << endl;
                }
                else
                {
                    cout << setw(15) << "Unavailable" << endl;
                }
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
    cout << "----------------------------------------------------------------------------------------" << endl;
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
                    cout << "Enter quantity to add to cart (max " << quantity << "): ";
                    cin >> quantityToAdd;

                    if (quantityToAdd > 0 && quantityToAdd <= quantity)
                    {
                        cartFile << code << "," << name << "," << price << "," << quantityToAdd << "\n";
                        cout << "Added " << quantityToAdd << " of item " << name << " to cart." << endl;
                        itemFound = true;
                    }
                    else
                    {
                        cout << "Invalid quantity. Item not added to cart." << endl;
                    }
                }
                else
                {
                    cout << "Item is out of stock." << endl;
                }
                break;
            }
        }
        itemFile.close();
        cartFile.close();

        if (!itemFound)
        {
            cout << "Item with code '" << itemCode << "' not found." << endl;
        }
    }
    else
    {
        cout << "Error opening files. Unable to add item to cart." << endl;
    }
}

void ViewCart()
{
    ifstream cartFile("Cart.txt");
    string line;
    double total = 0;

    cout << "\nYour Cart:" << endl;
    cout << "------------------------------------------------------------------------------------------------" << endl;
    cout << setw(10) << "Code" << setw(20) << "Name" << setw(10) << "Size" << setw(10) << "Price" << setw(10) << "Quantity" << setw(15) << "Subtotal" << endl;
    cout << "------------------------------------------------------------------------------------------------" << endl;

    if (cartFile.is_open())
    {
        while (getline(cartFile, line))
        {
            stringstream ss(line);
            string code, name, price, size;
            int quantity;
            getline(ss, code, ',');
            getline(ss, name, ',');
            getline(ss, size, ',');
            getline(ss, price, ',');
            ss >> quantity;

            double itemPrice = stod(price);
            double subtotal = itemPrice * quantity;
            total += subtotal;

            cout << setw(10) << code << setw(20) << name << setw(10) << size << setw(10) << fixed << setprecision(2) << itemPrice
                 << setw(10) << quantity << setw(15) << subtotal << endl;
        }
        cartFile.close();
    }
    else
    {
        cout << "Unable to open cart file." << endl;
    }

    cout << "------------------------------------------------------------------------------------------------" << endl;
    cout << "Total: TK" << fixed << setprecision(2) << total << endl;
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
            cout << "Item removed from cart." << endl;
        }
        else
        {
            cout << "Item not found in cart." << endl;
        }
    }
    else
    {
        cout << "Unable to open cart file." << endl;
    }
}

void CheckOut()
{
    ifstream cartFile("Cart.txt");
    ofstream saleFile("Sales.txt", ios::app);
    string line;
    double total = 0;

    cout << "\nCheckout - Items Purchased:" << endl;
    cout << "------------------------------------------------------------------------------------------------" << endl;
    cout << setw(10) << "Code" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << setw(15) << "Subtotal" << endl;
    cout << "------------------------------------------------------------------------------------------------" << endl;

    if (cartFile.is_open() && saleFile.is_open())
    {
        while (getline(cartFile, line))
        {
            stringstream ss(line);
            string code, name, price, size;
            int quantity;
            getline(ss, code, ',');
            getline(ss, name, ',');
            getline(ss, size, ',');
            getline(ss, price, ',');
            ss >> quantity;

            double itemPrice = stod(price);
            double subtotal = itemPrice * quantity;
            total += subtotal;

            cout << setw(10) << code << setw(20) << name << setw(10) << fixed << setprecision(2) << itemPrice
                 << setw(10) << quantity << setw(15) << subtotal << endl;

            saleFile << code << "," << name << "," << price << "," << quantity << "\n";
        }
        cartFile.close();
        saleFile.close();
    }
    else
    {
        cout << "Unable to open cart or sales file." << endl;
        return;
    }

    cout << "------------------------------------------------------------------------------------------------" << endl;
    cout << "Total Amount: TK" << fixed << setprecision(2) << total << endl;

    ofstream clearCart("Cart.txt", ios::trunc);
    clearCart.close();

    cout << "\033[38;2;255;128;0m";
    cout << " /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\ \n";
    cout << "( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )\n";
    cout << " > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ < \n";
    cout << " /\\_/\\                                                                                                                         /\\_/\\ \n";
    cout << "( o.o )       ::::::::::: :::    :::     :::     ::::    ::: :::    :::      :::   :::  ::::::::  :::    :::         :::      ( o.o )\n";
    cout << " > ^ <           :+:     :+:    :+:   :+: :+:   :+:+:   :+: :+:   :+:       :+:   :+: :+:    :+: :+:    :+:         :+:        > ^ < \n";
    cout << " /\\_/\\          +:+     +:+    +:+  +:+   +:+  :+:+:+  +:+ +:+  +:+         +:+ +:+  +:+    +:+ +:+    +:+         +:+         /\\_/\\ \n";
    cout << "( o.o )        +#+     +#++:++#++ +#++:++#++: +#+ +:+ +#+ +#++:++           +#++:   +#+    +:+ +#+    +:+         +#+         ( o.o )\n";
    cout << " > ^ <        +#+     +#+    +#+ +#+     +#+ +#+  +#+#+# +#+  +#+           +#+    +#+    +#+ +#+    +#+         +#+           > ^ < \n";
    cout << " /\\_/\\       #+#     #+#    #+# #+#     #+# #+#   #+#+# #+#   #+#          #+#    #+#    #+# #+#    #+#                        /\\_/\\ \n";
    cout << "( o.o )     ###     ###    ### ###     ### ###    #### ###    ###         ###     ########   ########          ###            ( o.o )\n";
    cout << " > ^ <                                                                                                                         > ^ < \n";
    cout << " /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\  /\\_/\\ \n";
    cout << "( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )\n";
    cout << " > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ < \n";
    cout << "\033[0m";
    cout << endl;
}

void UserDashboard()
{
    int choice;
    do
    {
        cout << "\033[38;2;255;128;0m";
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
        cout << "(_) 7. Logout                                                                                                                                         (_)\n";
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
            cout << "Enter the item code to search: ";
            cin >> codeToSearch;
            UserSearchItem(codeToSearch);
        }
        break;
        case 3:
        {
            string itemCode;
            cout << "Enter the item code to add to cart: ";
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
            cout << "Enter the item code to remove from cart: ";
            cin >> itemCode;
            RemoveFromCart(itemCode);
        }
        break;
        case 6:
            CheckOut();
            break;
        case 7:
            cout << "Logging out..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 7);
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
        cout << " |   | 3. Back to main menu                                                                |   | \n";
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
        cout << "\033[38;2;255;215;0m";
        cout << " _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ \n";
        cout << "|_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_|\n";
        cout << "|_|                                                                              |_|\n";
        cout << "|_|  ___  ___      _        ______          _     _                         _    |_|\n";
        cout << "|_|  |  \\/  |     (_)       |  _  \\        | |   | |                       | |   |_|\n";
        cout << "|_|  | .  . | __ _ _ _ __   | | | |__ _ ___| |__ | |__   ___   __ _ _ __ __| |   |_|\n";
        cout << "|_|  | |\\/| |/ _` | | '_ \\  | | | / _` / __| '_ \\| '_ \\ / _ \\ / _` | '__/ _` |   |_|\n";
        cout << "|_|  | |  | | (_| | | | | | | |/ / (_| \\__ \\ | | | |_) | (_) | (_| | | | (_| |   |_|\n";
        cout << "|_|  \\_|  |_/\\__,_|_|_| |_| |___/ \\__,_|___/_| |_|_.__/ \\___/ \\__,_|_|  \\__,_|   |_|\n";
        cout << "|_| _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ |_|\n";
        cout << "|_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_|\n";
        cout << "|_|                                                                              |_|\n";
        cout << "|_| 1. Admin access                                                              |_|\n";
        cout << "|_| 2. User access                                                               |_|\n";
        cout << "|_| 3. Exit                                                                      |_|\n";
        cout << "|_|                                                                              |_|\n";
        cout << "|_| _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ |_|\n";
        cout << "|_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_||_|\n";
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
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);

    return 0;
}