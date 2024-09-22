# Shop Management System

![C++ Version](https://img.shields.io/badge/C++-17-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

A comprehensive C++ console application for managing a shop's inventory, sales, and user accounts. This system provides separate interfaces for administrators and regular users, ensuring secure access and efficient management of the shop's operations.

## 🌟 Features

### 👑 Admin Features
- Authentication
  - Sign up for a new account
  - Log in to existing account
  - Reset password using unique code
- User Management
  - List all users
  - Remove users
- Inventory Management
  - Add new items
  - View all items
  - Remove items
  - Search for items
  - Modify existing items
- Sales Tracking
  - View sales reports and total revenue
- Account Recovery
  - View recovery code
  - Reset password using unique code

### 👤 User Features
- Authentication
  - Sign up for a new account
  - Log in to existing account
  - Reset password using unique code
- Shop Interaction
  - View available items
  - Search for items by code
  - Add items to cart
  - View cart contents
  - Remove items from cart
  - Checkout and complete purchase

### 🔐 Security Features
- **Password Protection**: User and admin passwords are never stored in plain text.
- **Data Encryption**: Custom XOR encryption method for data security.
- **Login Attempts Limit**: Users have a limited number of login attempts for security.
- **Unique Recovery Code**: Generated during account creation for password reset.

### :computer: User Interface
- **Console-based Menu System**: Easy-to-navigate menus for both users and admins.
- **Clear User Prompts**: Provides clear instructions and feedback to users.
- **ASCII Art**: Decorative ASCII art for headers and menus to enhance visual appeal.

### :sparkler: UI Improvements
- **ASCII Color Implementation**: The program uses ASCII color codes to enhance the visual appeal and improve user experience.
- **Formatted Output**: Utilizes setw() for aligned and organized display of information.

## 🛠️ System Architecture

The Shop Management System is built with the following key components:

1. **User Authentication**: Secure login and signup for both admins and regular users.
2. **File-based Data Storage**: Uses text files to store user accounts, inventory, and sales data.
3. **Encryption**: Custom encryption/decryption functions to secure sensitive data.
4. **Admin Dashboard**: Comprehensive management interface for administrators.
5. **User Dashboard**: User-friendly interface for customers to browse and purchase items.
6. **Shopping Cart**: Allows users to add, view, and remove items before checkout.
7. **Sales Tracking**: Records and summarizes all completed sales.
8. **Password Recovery**: Implements a secure method for users to reset their passwords.

## :wrench: Technical Implementation

- **Language**: C++
- **Encryption**: Custom XOR encryption method for data security.
- **Modular Design**: Functions are separated based on their functionality for better code organization.
- **File Handling**: Extensive use of file operations for data persistence.
- **Input Validation**: Implements checks to ensure valid user inputs.


## :notebook: Note

This program is designed for educational purposes and demonstrates concepts of user authentication, file handling, simple encryption, and basic e-commerce functionalities in C++. For production use, more robust security measures and error handling would be necessary.

## 📝 License

This project is [MIT](https://choosealicense.com/licenses/mit/) licensed.


---

Developed with ❤️ by Istiaq