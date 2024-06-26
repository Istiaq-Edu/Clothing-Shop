#include <iostream>
#include <string>
#include <limits>

using namespace std;

class CatalogItem
{
public:
    string itemName;
    double price;
    string Size;
    string code;
    string color;
    CatalogItem()
    {
        price = 0.0;
    }
    CatalogItem(string name, double p, string sz, string cd, string clr)
    {
        itemName = name;
        price = p;
        Size = sz;
        code = cd;
        color = clr;
    }
};

class Admin
{
private:
    string username;
    string password;
    bool isAdminSet;

public:
    Admin()
    {
        isAdminSet=(false);
    }

    void setAdminCredentials(string uname, string pwd)
    {
        username = uname;
        password = pwd;
        isAdminSet = true;
    }

    bool isAdminSetFlag() const
    {
        return isAdminSet;
    }

    bool login(string user, string pass)
    {
        return (isAdminSet && user == username && pass == password);
    }
};

class Catalog
{
private:
    static const int MAX_ITEMS = 100;
    CatalogItem summerItems[MAX_ITEMS];
    CatalogItem winterItems[MAX_ITEMS];
    int numSummerItems = 0;
    int numWinterItems = 0;

public:
    void addSummerItem(string itemName, double price, string Size, string code, string color)
    {
        if (numSummerItems < MAX_ITEMS)
        {
            summerItems[numSummerItems++] = CatalogItem(itemName, price, Size, code, color);
            cout << itemName << " \033[32madded to the summer catalog.\033[0m\n";
        }
        else
        {
            cout << "\033[31mSummer catalog is full. Cannot add more items.\033[0m\n";
        }
    }

    void addWinterItem(string itemName, double price, string size, string code, string color)
    {
        if (numWinterItems < MAX_ITEMS)
        {
            winterItems[numWinterItems++] = CatalogItem(itemName, price, size, code, color);
            cout << itemName << " \033[32madded to the winter catalog.\033[0m\n";
        }
        else
        {
            cout << "\033[31mWinter catalog is full. Cannot add more items.\033[0m\n";
        }
    }

    void removeSummerItem(string code)
    {
        for (int i = 0; i < numSummerItems; ++i)
        {
            if (summerItems[i].code == code)
            {
                for (int j = i; j < numSummerItems - 1; ++j)
                {
                    summerItems[j] = summerItems[j + 1];
                }
                numSummerItems--;
                cout << code << " \033[92mremoved from the summer catalog.\033[0m\n";
                return;
            }
        }
        cout << "\033[31mItem not found in the summer catalog.\033[0m\n";
    }

    void removeWinterItem(string code)
    {
        for (int i = 0; i < numWinterItems; ++i)
        {
            if (winterItems[i].code == code)
            {
                for (int j = i; j < numWinterItems - 1; ++j)
                {
                    winterItems[j] = winterItems[j + 1];
                }
                numWinterItems--;
                cout << code << " \033[92mremoved from the winter catalog.\033[0m\n";
                return;
            }
        }
        cout << "\033[31mItem not found in the winter catalog.\033[0m\n";
    }

    void viewSummerCatalog()
    {
        if (numSummerItems == 0)
        {
            cout << "\033[31mThe summer catalog is empty.\033[0m\n";
        }
        else
        {
            cout << "\033[94mItems in the summer catalog:\033[0m\n\n";
            for (int i = 0; i < numSummerItems; ++i)
            {
                cout << "Code: " << summerItems[i].code<< " | ";
                cout << "Name: " << summerItems[i].itemName<< " | ";
                cout << "Size: " << summerItems[i].Size<< " | ";
                cout << "Color: " << summerItems[i].color<< " | ";
                cout << "Price: " << summerItems[i].price<< "-BTD | ";
                cout << "\n\n";
            }
        }
    }

    void viewWinterCatalog()
    {
        if (numWinterItems == 0)
        {
            cout << "\033[31mThe winter catalog is empty.\033[0m\n";
        }
        else
        {
            cout << "\033[94mItems in the winter catalog:\033[0m\n\n";
            for (int i = 0; i < numWinterItems; ++i)
            {
                cout << "Code: " << winterItems[i].code << " | ";
                cout << "Name: " << winterItems[i].itemName << " | ";
                cout << "Size: " << winterItems[i].Size << " | ";
                cout << "Color: " << winterItems[i].color << " | ";
                cout << "Price: " << winterItems[i].price << "-BTD | ";
                cout << "\n\n";
            }
        }
    }

    int getNumSummerItems() const
    {
        return numSummerItems;
    }

    int getNumWinterItems() const
    {
        return numWinterItems;
    }

    const CatalogItem* getSummerItems() const
    {
        return summerItems;
    }

    const CatalogItem* getWinterItems() const
    {
        return winterItems;
    }
};

CatalogItem findItemInCatalog(string itemCode, Catalog& catalog)
{
    for (int i = 0; i < catalog.getNumSummerItems(); ++i)
    {
        if (catalog.getSummerItems()[i].code == itemCode)
        {
            return catalog.getSummerItems()[i];
        }
    }

    for (int i = 0; i < catalog.getNumWinterItems(); ++i)
    {
        if (catalog.getWinterItems()[i].code == itemCode)
        {
            return catalog.getWinterItems()[i];
        }
    }

    return CatalogItem();
}

class CustomerCatalog
{
private:
    static const int MAX_CART_ITEMS = 20;
    string customerCart[MAX_CART_ITEMS];
    double cartTotal = 0.0;
    int numItemsInCart = 0;

public:
    void viewSummerCatalog(Catalog& catalog)
    {
        catalog.viewSummerCatalog();
    }

    void viewWinterCatalog(Catalog& catalog)
    {
        catalog.viewWinterCatalog();
    }

    void addToCart(const string& itemCode, Catalog& catalog)
    {
        const CatalogItem item = findItemInCatalog(itemCode, catalog);
        if (item.code != "")
        {
            if (numItemsInCart < MAX_CART_ITEMS)
            {
                customerCart[numItemsInCart++] = itemCode;
                cartTotal += item.price;
                cout << item.itemName << " \033[92madded to your cart.\033[0m\n";
            }
            else
            {
                cout << "\033[31mYour cart is full. Cannot add more items.\033[0m\n";
            }
        }
        else
        {
            cout << "\033[31mItem not found in the catalog.\033[0m\n";
        }
    }
    void viewCart(Catalog& catalog)
    {
        if (numItemsInCart == 0)
        {
            cout << "\033[31mYour cart is empty.\033[0m\n";
        }
        else
        {
            cout << "\033[94mItems in your cart:\033[0m\n";
            for (int i = 0; i < numItemsInCart; ++i)
            {
                CatalogItem item = findItemInCatalog(customerCart[i], catalog);
                cout << i + 1 << ". " << "Code: " << item.code << " | ";
                cout << "Name: " << item.itemName << " | ";
                cout << "Size: " << item.Size << " | ";
                cout << "Color: " << item.color << " | ";
                cout << "Price: " << item.price << "-BTD | ";
                cout << "\n\n";
            }
            cout << "Total: \033[0m" << cartTotal << "-BTD\n";
        }
    }

    void makePayment(int paymentOption)
    {
        if (numItemsInCart == 0)
        {
            cout << "\033[31mYour cart is empty. Cannot proceed to payment.\033[0m\n";
            return;
        }
        switch (paymentOption)
        {
        case 1:
            cout << "\n\033[92mPayment successful! Paid with card.\033[0m\n";
            cout << "\033[38;2;255;128;0m";
            cout << "  _____ _                 _           __                    _     _ _   _             \n";
            cout << " |_   _| |__   __ _ _ __ | | _____   / _| ___  _ __  __   _(_)___(_) |_(_)_ __   __ _ \n";
            cout << "   | | | '_ \\ / _` | '_ \\| |/ / __| | |_ / _ \\| '__| \\ \\ / / / __| | __| | '_ \\ / _` |\n";
            cout << "   | | | | | | (_| | | | |   <\\__ \\ |  _| (_) | |     \\ V /| \\__ \\ | |_| | | | | (_| |\n";
            cout << "   |_| |_| |_|\\__,_|_| |_|_|\\_\\___/ |_|  \\___/|_|      \\_/ |_|___/_|\\__|_|_| |_|\\__, |\n";
            cout << "                                                                                 |___/ \n";
            cout << "\033[0m";
            break;
        case 2:
            cout << "\n\033[92mPayment successful! Paid with cash.\033[0m\n";
            cout << "\033[38;2;255;128;0m";
            cout << "  _____ _                 _           __                    _     _ _   _             \n";
            cout << " |_   _| |__   __ _ _ __ | | _____   / _| ___  _ __  __   _(_)___(_) |_(_)_ __   __ _ \n";
            cout << "   | | | '_ \\ / _` | '_ \\| |/ / __| | |_ / _ \\| '__| \\ \\ / / / __| | __| | '_ \\ / _` |\n";
            cout << "   | | | | | | (_| | | | |   <\\__ \\ |  _| (_) | |     \\ V /| \\__ \\ | |_| | | | | (_| |\n";
            cout << "   |_| |_| |_|\\__,_|_| |_|_|\\_\\___/ |_|  \\___/|_|      \\_/ |_|___/_|\\__|_|_| |_|\\__, |\n";
            cout << "                                                                                 |___/ \n";
            cout << "\033[0m";
            break;
        default:
            cout << "\033[31mInvalid payment option\033[0m\n";
            return;
        }

        clearCart();
    }

    void clearCart()
    {
        numItemsInCart = 0;
        cartTotal = 0.0;
    }
};

class SignupLogin
{
private:
    static const int MAX_USERS = 100;
    string users[MAX_USERS][2];
    int numUsers = 0;
    Admin admin;

public:
    bool adminLogin()
    {
        if (!admin.isAdminSetFlag())
        {
            cout << "\033[31mAdmin credentials not set.\033[0m\n";
            cout << "\033[92mPlease set admin credentials first.\033[0m\n";
            return false;
        }

        string adminUsername, adminPassword;

        cout << "\033[96mEnter admin username: \033[0m";
        getline(cin, adminUsername);

        cout << "\033[96mEnter admin password: \033[0m";
        getline(cin,adminPassword);

        /////decode
        for(int i=0; i<adminPassword.length(); i++)
        {
            adminPassword[i] += 1;
        }


        if (admin.login(adminUsername, adminPassword))
        {
            cout << "\n\033[92mADMIN LOGIN SUCCESSFUL!\033[0m\n";
            return true;
        }
        else
        {
            cout << "\033[31mInvalid admin credentials\033[0m\n";
            return false;
        }
    }

    void setAdminCredentials()
    {
        cin.ignore();
        if (!admin.isAdminSetFlag())
        {
            string adminUsername, adminPassword;

            cout << "\033[96mSet admin username: \033[0m";
            getline(cin, adminUsername);

            cout << "\033[96mSet admin password: \033[0m";
            getline(cin, adminPassword);

            /////////////////encode
            for(int i=0; i<adminPassword.length(); i++)
            {
                adminPassword[i] += 1;
            }

            admin.setAdminCredentials(adminUsername, adminPassword);
            cout << "\033[92mAdmin credentials set successfully!\033[0m\n";
        }
        else
        {
            cout << "\033[31mAdmin credentials already set.\033[0m\n";
        }
    }

    void customerSignup()
    {
        cin.ignore();
        string username, password;

        cout << "\033[96mEnter username: \033[0m";
        getline(cin,username);

        bool usernameExists = false;
        for (int i = 0; i < numUsers; i++)
        {
            if (users[i][0] == username)
            {
                usernameExists = true;
                break;
            }
        }

        if (usernameExists)
        {
            cout << "\033[31mUsername already exists!\033[0m\n";
            return;
        }

        cout << "\033[96mEnter password: \033[0m";
        getline(cin, password);

        if (password.length() < 1)
        {
            cout << "\033[31mPassword too short!\033[0m\n";
            return;
        }
        ////encode
        for(int i=0; i<password.length(); i++)
        {
            password[i] += 1;
        }

        users[numUsers][0] = username;
        users[numUsers][1] = password;
        numUsers++;

        cout << "\033[92mAccount created!\033[0m\n\n";
    }
    bool customerLogin()
    {
        cin.ignore();
        string username, password;

        cout << "\033[96mEnter username: \033[0m";
        getline(cin, username);

        cout << "\033[96mEnter password: \033[0m";
        getline(cin, password);

        ////decode
        for(int i=0; i<password.length(); i++)
        {
            password[i] += 1;
        }

        bool credentialsValid = false;
        for (int i = 0; i < numUsers; i++)
        {
            if (users[i][0] == username && users[i][1] == password)
            {
                credentialsValid = true;
                cout << "\n\033[35mWelcome " << username << "!\033[0m\n\n";
                break;
            }
        }

        if (!credentialsValid)
        {
            cout << "\n\033[31mInvalid credentials\033[0m\n";
        }

        return credentialsValid;
    }

    void customerForgotPassword()
    {
        cin.ignore();
        string username;
        cout << "\033[96mEnter your username: \033[0m";
        getline(cin, username);

        bool usernameFound = false;
        for (int i = 0; i < numUsers; i++)
        {
            if (users[i][0] == username)
            {
                usernameFound = true;
                break;
            }
        }

        if (!usernameFound)
        {
            cout << "\033[31mUsername not found!\033[0m\n";
            return;
        }
        string newPassword;
        cout << "\033[96mEnter new password: \033[0m";
        getline(cin, newPassword);

        if (newPassword.length() < 1)
        {
            cout << "\033[31mPassword too short!\033[0m\n";
            return;
        }
///encode
        for (int i = 0; i < newPassword.length(); i++)
        {
            newPassword[i] += 1;
        }

        for (int i = 0; i < numUsers; i++)
        {
            if (users[i][0] == username)
            {
                users[i][1] = newPassword;
                break;
            }
        }

        cout << "\033[92mPassword changed successfully!\033[0m\n";
    }
};

void adminMenu(SignupLogin &auth, Catalog &catalog)
{
    int choice;

    while (1)
    {
        cout << "\033[90m**********************************************************************************************************************\033[0m\n";
        cout << "\n\033[33mAdmin Menu:\033[0m\n";
        cout << "1. View Summer Catalog\n";
        cout << "2. Add Item to Summer Catalog\n";
        cout << "3. Remove Item from Summer Catalog\n";
        cout << "4. View Winter Catalog\n";
        cout << "5. Add Item to Winter Catalog\n";
        cout << "6. Remove Item from Winter Catalog\n";
        cout << "7. Logout\n";
        cout << "8. Exit\n\n";
        cout << "\033[90m**********************************************************************************************************************\033[0m\n";

        while (true)
        {
            cout << "\033[96mEnter your choice: \033[0m";
            if (cin >> choice)
            {
                break;
            }
            else
            {
                cout << "\033[31mInvalid input. Please enter a number.\033[0m\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        cin.ignore();
        switch (choice)
        {
        case 1:
            catalog.viewSummerCatalog();
            break;
        case 2:
        {
            string itemName, size, code, color;
            double price;
            cout << "\033[96mEnter the item name to add to the summer catalog: \033[0m";
            getline(cin, itemName);
            cout << "\033[96mEnter the price of the item: \033[0m";
            cin >> price;
            cout << "\033[96mEnter the size of the item: \033[0m";
            cin >> size;
            cout << "\033[96mEnter the code of the item: \033[0m";
            cin >> code;
            cout << "\033[96mEnter the color of the item: \033[0m";
            cin >> color;
            catalog.addSummerItem(itemName, price, size, code, color);
            break;
        }
        case 3:
        {
            string code;
            cout << "\033[96mEnter the item code to remove from the summer catalog: \033[0m";
            getline(cin, code);
            catalog.removeSummerItem(code);
            break;
        }
        case 4:
            catalog.viewWinterCatalog();
            break;
        case 5:
        {
            string itemName, size, code, color;
            double price;
            cout << "\033[96mEnter the item name to add to the winter catalog: \033[0m";
            getline(cin, itemName);
            cout << "\033[96mEnter the price of the item: \033[0m";
            cin >> price;
            cout << "\033[96mEnter the size of the item: \033[0m";
            cin >> size;
            cout << "\033[96mEnter the code of the item: \033[0m";
            cin >> code;
            cout << "\033[96mEnter the color of the item: \033[0m";
            cin >> color;
            catalog.addWinterItem(itemName, price, size, code, color);
            break;
        }
        case 6:
        {
            string code;
            cout << "\033[96mEnter the item code to remove from the winter catalog: \033[0m";
            getline(cin, code);
            catalog.removeWinterItem(code);
            break;
        }
        case 7:
            return;
        case 8:
            exit(0);
        default:
            cout << "\033[31mInvalid choice\033[0m\n";
        }
    }
}

int main()
{
    SignupLogin auth;
    Catalog catalog;

    while (true)
    {
        int userType;
        /*       // Basic colors
               cout << "\033[30mBlack\033[0m" << endl;   // Black
               cout << "\033[31mRed\033[0m" << endl;     // Red
               cout << "\033[32mGreen\033[0m" << endl;   // Green
               cout << "\033[33mYellow\033[0m" << endl;  // Yellow
               cout << "\033[34mBlue\033[0m" << endl;    // Blue
               cout << "\033[35mMagenta\033[0m" << endl; // Magenta
               cout << "\033[36mCyan\033[0m" << endl;    // Cyan
               cout << "\033[37mWhite\033[0m" << endl;   // White
        // Bright colors
               cout << "\033[90mBright Black\033[0m" << endl;   // Bright Black
               cout << "\033[91mBright Red\033[0m" << endl;     // Bright Red
               cout << "\033[92mBright Green\033[0m" << endl;   // Bright Green
               cout << "\033[93mBright Yellow\033[0m" << endl;  // Bright Yellow
               cout << "\033[94mBright Blue\033[0m" << endl;    // Bright Blue
               cout << "\033[95mBright Magenta\033[0m" << endl; // Bright Magenta
               cout << "\033[96mBright Cyan\033[0m" << endl;    // Bright Cyan
               cout << "\033[97mBright White\033[0m" << endl;   // Bright White
        */
        cout << "\033[38;2;255;128;0m";
        cout << " __        __   _                            _          _   _                _                 \n";
        cout << " \\ \\      / /__| | ___ ___  _ __ ___   ___  | |_ ___   | |_| |__   ___   ___| |__   ___  _ __  \n";
        cout << "  \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\  | __| '_ \\ / _ \\ / __| '_ \\ / _ \\| '_ \\ \n";
        cout << "   \\ V  V /  __/ | (_| (_) | | | | | |  __/ | || (_) | | |_| | | |  __/ \\__ \\ | | | (_) | |_) |\n";
        cout << "    \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/   \\__|_| |_|\\___| |___/_| |_|\\___/| .__/ \n";
        cout << "                                                                                        |_|     \n";
        cout << "\033[0m";
        cout << "\033[90m**********************************************************************************************************************\033[0m\n";
        cout << "\033[33mSelect user type:\033[0m\n";
        cout << "1. Customer\n";
        cout << "2. Admin\n";
        cout << "3. Exit\n";
        cout << "\033[90m**********************************************************************************************************************\033[0m\n";
        while (true)
        {
            cout << "\033[96mEnter your choice: \033[0m";
            if (cin >> userType)
            {
                break;
            }
            else
            {
                cout << "\033[31mInvalid input. Please enter a valid choice.\033[0m\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        if (userType == 1)
        {
            int customerChoice;

            do
            {
                cout << "\033[90m**********************************************************************************************************************\033[0m\n";
                cout << "\n\033[33mCustomer Menu:\033[0m\n";
                cout << "1. Signup\n";
                cout << "2. Login\n";
                cout << "3. Forgot Password\n";
                cout << "4. Exit\n\n";
                cout << "\033[90m**********************************************************************************************************************\033[0m\n";
                while (true)
                {
                    cout << "\033[96mEnter your choice: \033[0m";
                    if (cin >> customerChoice)
                    {
                        break;
                    }
                    else
                    {
                        cout << "\033[31mInvalid input. Please enter a number.\033[0m\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                }

                switch (customerChoice)
                {
                case 1:
                    auth.customerSignup();
                    break;
                case 2:
                    if (auth.customerLogin())
                    {
                        CustomerCatalog customerCatalog;
                        int customerCategory;
                        do
                        {
                            cout << "\033[90m**********************************************************************************************************************\033[0m\n";
                            cout << "\033[33mChoose a category:\033[0m\n";
                            cout << "1. Summer Items\n";
                            cout << "2. Winter Items\n";
                            cout << "3. Exit\n\n";
                            cout << "\033[90m**********************************************************************************************************************\033[0m\n";
                            while (true)
                            {
                                cout << "\033[96mEnter your choice: \033[0m";
                                if (cin >> customerCategory)
                                {
                                    break;
                                }
                                else
                                {
                                    cout << "\033[31mInvalid input. Please enter a number.\033[0m\n";
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                }
                            }

                            switch (customerCategory)
                            {
                            case 1:
                                customerCatalog.viewSummerCatalog(catalog);
                                break;
                            case 2:
                                customerCatalog.viewWinterCatalog(catalog);
                                break;
                            case 3:
                                return 0;
                            default:
                                cout << "\033[31mInvalid category choice\033[0m\n";
                            }

                            int cartAction;
                            do
                            {
                                cout << "\033[90m**********************************************************************************************************************\033[0m\n";
                                cout << "\033[33mChoose an action:\033[0m\n";
                                cout << "1. Add to Cart\n";
                                cout << "2. View Cart\n";
                                cout << "3. Make Payment\n";
                                cout << "4. Back to Main Menu\n";
                                cout << "\033[90m**********************************************************************************************************************\033[0m\n";
                                while (true)
                                {
                                    cout << "\033[96mEnter your choice: \033[0m";
                                    if (cin >> cartAction)
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        cout << "\033[31mInvalid input. Please enter a number.\033[0m\n";
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    }
                                }
                                
                                CatalogItem item;

                                switch (cartAction)
                                {
                                case 1:
                                    do
                                    {
                                        string itemCode;
                                        cout << "\033[96mEnter the item code to add to the cart: \033[0m";
                                        cin.ignore();
                                        getline(cin, itemCode);
                                        CatalogItem item = findItemInCatalog(itemCode, catalog);
                                        if (item.code != "")
                                            customerCatalog.addToCart(itemCode, catalog);
                                        else
                                            cout << "\033[31mItem not found in catalog.\033[0m\n";

                                        cout << "\033[93mDo you want to add more items to the cart? (1: Yes, 0: No): \033[0m";
                                        cin >> cartAction;
                                    }
                                    while (cartAction == 1);
                                    break;
                                case 2:
                                    customerCatalog.viewCart(catalog);
                                    break;
                                case 3:
                                    int paymentOption;
                                    cout << "\033[90m**********************************************************************************************************************\033[0m\n";
                                    cout << "\033[33mChoose a payment option:\033[0m\n";
                                    cout << "1. Card\n";
                                    cout << "2. Cash\n";
                                    cout << "\033[90m**********************************************************************************************************************\033[0m\n";
                                    while (true)
                                    {
                                        cout << "\033[96mEnter your choice: \033[0m";
                                        if (cin >> paymentOption)
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            cout << "\033[31mInvalid input. Please enter a number.\033[0m\n";
                                            cin.clear();
                                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        }
                                    }

                                    customerCatalog.makePayment(paymentOption);
                                    break;
                                case 4:
                                    break;
                                default:
                                    cout << "\033[31mInvalid choice\033[0m\n";
                                }
                            }
                            while (cartAction != 4);
                        }
                        while (customerCategory != 3);
                    }
                    break;
                case 3:
                    auth.customerForgotPassword();
                    break;
                case 4:
                    return 0;
                default:
                    cout << "\033[31mInvalid choice\033[0m\n";
                }
            }
            while (customerChoice != 4);
        }
        else if (userType == 2)
        {
            auth.setAdminCredentials();

            while (!auth.adminLogin())
            {
                cout << "\033[31mInvalid credentials. Please try again.\033[0m\n";
            }
            adminMenu(auth, catalog);
        }
        else if (userType == 3)
        {
            break;
        }
        else
        {
            cout << "\033[31mInvalid user type\033[0m\n";
        }
    }
    return 0;
}
