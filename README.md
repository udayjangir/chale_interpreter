
# Chale Interpreter 1.0.1

Chale Interpreter is based on C++ language that evaluates addition expressions written in C++ language, and prints them.
Using lexical analysis, it recognises the tokens and check for the syntax structure to perform correct operation or handle errors.

### Features

- Declaration as well as initialisation, both are handled.
- Mulitple variables can be declared at a time using comma.
- Working with negative integers as well.
- Can interpret mutliple statements in a single line (separated by semicolon)

## Installation

To run the program, make sure to have a C++ compiler installed in your system.  
First, change the current working directory to "chale_interpreter/src/" using command:

```bash
cd src/
```

Now, compile the two files "main.cpp" and "lexer.cpp" with C++ compiler, and run using command:

 ```bash
g++ -o chale.exe main.cpp lexer.cpp
./chale.exe
 ```

Note: For windows cmd, run the file using:  

```bash
chale.exe
```

You can replace the output file name ("chale.exe") to any filename of your choice ("output_filename.exe"). Just make sure to run the executable file with correct filename.  
That's all, now the interpreter should be running in your terminal window !

```bash
Chale Interpreter 1.0.1
Type 'exit;' to terminate the program


```

## Usage/Examples
Try running the following sequence of statements:

```bash
Chale Interpreter 1.0.1
Type 'exit;' to terminate the program

int num1 = 12;
int num2 = 54;
int num3;
num3 = num1 + num2;
cout << num3;
```

First initialise an int variable "num1" with value 12. Then initialise another variable "num2" with value = 54. Now, declare a variable num3 to store the addition of num1 and num2. And in next line, assign it to num3.  
Now, print the value stored in num3 using "cout" and it shows the correct result num1 + num2 = 66.  

Try to run some other statement that cause an error, like redeclaring an already declared variable:

```bash
Chale Interpreter 1.0.1
Type 'exit;' to terminate the program

int var = 90;
cout << var;
int var = 21;
```

Second statement prints the value stored in var (i.e. 90) but third line causes an error:

```bash
Error: Type(Syntax error): Identifier with name 'var' is already declared
```

Try running different statements, as long as they are valid and lexically correct, the program runs fine and handles errors if encountered.

## Authors

- Someshwari Vraj (20114093)
- Uday Jangir (20114102)
- Youwan Soni (20114107)
