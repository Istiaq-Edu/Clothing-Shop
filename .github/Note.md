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


###Dont show password on console###

1. `string password = "";`: Initializes an empty string to store the password.

2. `char ch;`: Declares a character variable to store each keystroke.

3. `cout << "Enter password: ";`: Prompts the user to enter their password.

4. `while ((ch = _getch()) != 13) {`: This is the main loop that captures keystrokes.
   - `_getch()` is a function from `<conio.h>` that reads a single character from the console without echoing it to the screen.
   - It continues to loop until the Enter key (ASCII 13) is pressed.

5. `if (ch == 8) {`: Checks if the Backspace key (ASCII 8) was pressed.

6. `if (!password.empty()) {`: If Backspace was pressed and the password is not empty:
   - `cout << "\b \b";`: This prints a backspace, a space, and another backspace. It effectively erases the last asterisk from the console.
   - `password.pop_back();`: Removes the last character from the password string.

7. `} else {`: If any other key was pressed:
   - `password.push_back(ch);`: Adds the character to the password string.
   - `cout << '*';`: Prints an asterisk to the console, masking the actual character.

8. After the loop ends (when Enter is pressed):
   - `cout << endl;`: Moves to a new line.
   - `return password;`: Returns the completed password string.
