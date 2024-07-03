# Shop Management System

This C++ program implements a robust shop management system with user and admin functionalities. It provides a secure environment for user authentication, product management, and shopping cart features.

## Current Features

### User Management
- **User Sign Up**: New users can create accounts with unique usernames and passwords.
- **User Login**: Existing users can log in with their credentials.
- **User Authentication**: Implements a secure authentication system for users.

### Admin Management
- **Admin Sign Up**: Allows creation of admin accounts.
- **Admin Login**: Secure login system for administrators.
- **Admin Dashboard**: Provides admin-specific functionalities.

### Admin Functionalities
- **List Users**: Admins can view a list of all registered users.
- **Remove Users**: Admins have the ability to remove users from the system.

### Security Features
- **Password Protection**: User and admin passwords are never stored in plain text.
- **Data Encryption**: Implements XOR encryption for storing sensitive data.
- **Login Attempts Limit**: Users have a limited number of login attempts for security.

### File Handling
- Utilizes file I/O operations to store user and admin data persistently.
- Separate files for users (`Users.txt`) and admins (`Admins.txt`).

### User Interface
- **Console-based Menu System**: Easy-to-navigate menus for both users and admins.
- **Clear User Prompts**: Provides clear instructions and feedback to users.

## Upcoming Features

### Enhanced Admin Dashboard
- **Add Products**: Admins will be able to add new products to the inventory.
- **View Products**: A feature to display all products in the inventory.
- **Rename Products**: Ability to modify product names.
- **Remove Products**: Option to remove products from the inventory.
- **Product File**: All product information will be stored in a separate file for easy management.

### Improved User Dashboard
- **Shopping Cart System**: Users will be able to:
  - Add products to their cart
  - View products in their cart
  - Remove products from their cart

### UI Improvements
- **ASCII Color Implementation**: The program will use ASCII color codes to enhance the visual appeal and improve user experience.

## Technical Implementation

- **Language**: C++
- **Data Structures**: Uses `string`, `fstream` for file operations, and `stringstream` for string parsing.
- **Encryption**: Custom XOR encryption method for data security.
- **Modular Design**: Functions are separated based on their functionality for better code organization.

## How to Use

1. Compile the C++ file using a C++ compiler.
2. Run the executable.
3. Choose between admin access, user access, or exit.
4. Follow the on-screen prompts to navigate through the system.

## Note

This program is designed for educational purposes and demonstrates concepts of user authentication, file handling, simple encryption, and basic e-commerce functionalities in C++. For production use, more robust security measures and error handling would be necessary.
