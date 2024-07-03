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

