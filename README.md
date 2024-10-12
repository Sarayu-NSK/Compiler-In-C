# Compiler-In-C

Embark on a captivating journey into the heart of programming languages with our
Mini Compiler project. This endeavor unfolds in five straightforward steps,
demystifying the process of transforming human-readable code into executable
machine instructions.

![image](https://github.com/user-attachments/assets/b5c6736f-742a-473f-8eb2-da93268abc79)

The provided code performs lexical analysis, parsing, generates an Abstract Syntax
Tree (AST), generates intermediate code, and finally, generates assembly code. Below
is the algorithmic breakdown of the major components:
Lexical Analysis
1. Define a structure Token to represent tokens with lexeme and token type.
2. Define token types using an enumeration.
3. Define keywords and a symbol table for identifiers.
4. Implement the isKeyword function to check if a string is a keyword.
5. Implement the addToSymbolTable function to add identifiers to the symbol
table.
6. Implement the lexicalAnalysis function to tokenize the input file, identifying
keywords, identifiers, integers, relational operators, strings, and other
symbols.
Parsing
1. Define non-terminal types using an enumeration.
2. Implement parsing functions for the program, declarations, types, and
variables.
3. Implement a generic parsing function to traverse the AST.
4. Use the parsing functions to build the AST.
Abstract Syntax Tree (AST)
1. Define a structure TreeNode to represent AST nodes.
2. Define node types using an enumeration.
3. Implement functions to create nodes, free the AST, and display the AST.
4. Implement parsing functions to build the AST.
Intermediate Code Generation
1. Define operations using an enumeration for the intermediate code.
2. Define a structure IntermediateCode to represent intermediate code
instructions.
3. Implement a function to generate intermediate code from the AST.
4. Generate intermediate code for variable declarations and assignments.
5. Use temporary variables for intermediate results.
6. Display the generated intermediate code.
11
Assembly Code Generation
1. Define operations using an enumeration for the assembly code.
2. Implement a function to generate assembly code from the intermediate code.
3. Generate assembly code for MOV, ADD, SUB, MUL, DIV operations.
4. Display the generated assembly code.
Main Function
1. Open the input file for lexical analysis.
2. Perform lexical analysis to generate tokens.
3. Display the tokens.
4. Perform parsing to build the AST.
5. Display the symbol table.
6. Perform semantic analysis (if needed).
7. Generate intermediate code.
8. Display the AST and intermediate code.
9. Generate assembly code.
10. Free the AST and close the input file.
Note
● The provided code does not implement the full range of language features.
Additional functionalities such as handling expressions, control flow
statements, and more complex data types would require further development.
Please note that the given code structure is a simplified example, and a complete
programming language implementation would require more features and error
handling.

