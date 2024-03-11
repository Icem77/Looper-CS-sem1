# Looper-CS-sem1

The following program is an implementation of the Looper language.

The program reads and executes commands:
💭 Printing the value of a variable.
💭 Executing a program in the Looper language.
💭 Before executing the first command, the values of all variables are set to 0.
💭Variables retain their values after executing a command. They are not reset before each program execution.

💭 The syntax of the language is described by the grammar with the starting symbol Program:
Program → SequenceOfInstructions
SequenceOfInstructions → ε | SequenceOfInstructions Instruction
Instruction → Increment | Repeat
Increment → Variable
Repeat → '(' Variable SequenceOfInstructions ')'
Variable → 'a' | .. | 'z'

💭 The program consists of characters enclosed in apostrophes as defined in the grammar. Apart from them, no other characters, not even spaces or line breaks, can appear in the source code. The program has access to 26 variables, whose values are non-negative integers.

💭 Instructions derived from the SequenceOfInstructions symbol are executed sequentially from the first to the last. The Increment instruction, in the form of a variable, is equivalent to the 'C' language instruction:
++variable;
The Repeat instruction, in the form of (variable...), is equivalent to the 'C' language instruction:
while (variable > 0) {
--variable;
...
}

💭 If no Repeat instruction is nested within another Repeat instruction, meaning that there is a sequence of variables Variable0, ..., VariableN, for N >= 0, and if none of the variables Variable1, ..., VariableN is equal to the variable Variable0, the compiler generates optimized code by adding the value of the variable Variable0 to variables Variable(1-N) and setting its value to zero.
If the conditions allowing the generation of optimized code for the instruction are not met, the compiler generates regular code.

💭 INPUT:
The program data consists of a sequence of lines, with one command per line.
A command to print the value of a variable starts with the '=' character, followed by the variable name.
A line containing a command to execute the program contains source code in the Petlik language.
Each input line ends with a newline character.

💭 OUTPUT:
The program result is the effect of executing commands to print the value of a variable.
The value of the variable is written in decimal, on one line, without leading insignificant zeros.
The value 0 is represented by the digit 0, and positive values start with a nonzero digit.
Each output line ends with a newline character.

💭 ASSUMPTIONS:
Memory overflow will not occur during program execution.
The length of the program in the Petlik language will not exceed INT_MAX - 1.
Decimal representations of variable values will have fewer than 1000 digits.

❗In the code, both variable names and comments are in Polish because this program was one of the projects for the first semester of computer science studies. Sorry for the inconvenience.❗️

