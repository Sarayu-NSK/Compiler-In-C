#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_IDENTIFIER_LENGTH 50
#define MAX_KEYWORDS 10
#define MAX_TOKENS 100

// Structure to represent a token
typedef struct {
    char lexeme[MAX_IDENTIFIER_LENGTH];
    int tokenType;
} Token;

// Enumeration of token types
enum {
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    RELATIONAL_OPERATOR,
    STRING,  // Added STRING token type
    SEMICOLON, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, SLASH, BACKSLASH
};

// Keyword array
char *keywords[MAX_KEYWORDS] = {
    "if", "else", "while", "for", "int", "float", "char", "return", "break", "continue"
};

// Symbol table entry structure
typedef struct {
    char lexeme[MAX_IDENTIFIER_LENGTH];
} SymbolEntry;

// Symbol table
SymbolEntry symbolTable[MAX_KEYWORDS];
int symbolTableSize = 0;

// Function to check if a string is a keyword
int isKeyword(char *str) {
    for (int i = 0; i < MAX_KEYWORDS; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to add an entry to the symbol table
void addToSymbolTable(char *lexeme) {
    if (symbolTableSize < MAX_KEYWORDS) {
        strcpy(symbolTable[symbolTableSize].lexeme, lexeme);
        symbolTableSize++;
    }
}

// Function to display the symbol table
void displaySymbolTable() {
    printf("\nSymbol Table:\n");
    for (int i = 0; i < symbolTableSize; i++) {
        printf("%d: %s\n", i + 1, symbolTable[i].lexeme);
    }
}

// Function to perform lexical analysis
void lexicalAnalysis(FILE *inputFile, Token *tokens, int *numTokens) {
    char currentChar;
    char lexeme[MAX_IDENTIFIER_LENGTH];
    int lexemeIndex = 0;

    while ((currentChar = fgetc(inputFile)) != EOF) {
        if (isalpha(currentChar)) { // Start of a keyword or identifier
            lexemeIndex = 0;
            lexeme[lexemeIndex++] = currentChar;

            while (isalnum(currentChar = fgetc(inputFile)) || currentChar == '_') {
                lexeme[lexemeIndex++] = currentChar;
            }
            lexeme[lexemeIndex] = '\0';

            ungetc(currentChar, inputFile); // Put back the non-alphanumeric character

            if (isKeyword(lexeme)) {
                printf("<KEYWORD, %s>\n", lexeme);
                tokens[*numTokens].tokenType = KEYWORD;
            } else {
                printf("<IDENTIFIER, %s>\n", lexeme);
                tokens[*numTokens].tokenType = IDENTIFIER;
                addToSymbolTable(lexeme);
            }

            strcpy(tokens[*numTokens].lexeme, lexeme);
            (*numTokens)++;
        } else if (isdigit(currentChar)) { // Integer
            lexemeIndex = 0;
            lexeme[lexemeIndex++] = currentChar;

            while (isdigit(currentChar = fgetc(inputFile))) {
                lexeme[lexemeIndex++] = currentChar;
            }
            lexeme[lexemeIndex] = '\0';

            printf("<INTEGER, %s>\n", lexeme);
            ungetc(currentChar, inputFile); // Put back the non-digit character

            strcpy(tokens[*numTokens].lexeme, lexeme);
            tokens[*numTokens].tokenType = INTEGER;
            (*numTokens)++;
        } else if (currentChar == '<' || currentChar == '>' || currentChar == '=' || currentChar == '!') {
            lexemeIndex = 0;
            lexeme[lexemeIndex++] = currentChar;

            if ((currentChar = fgetc(inputFile)) == '=') {
                lexeme[lexemeIndex++] = currentChar;
                lexeme[lexemeIndex] = '\0';
                printf("<RELATIONAL_OPERATOR, %s>\n", lexeme);
            } else {
                lexeme[lexemeIndex] = '\0';
                printf("<RELATIONAL_OPERATOR, %s>\n", lexeme);
                ungetc(currentChar, inputFile); // Put back the non-'=' character
            }

            strcpy(tokens[*numTokens].lexeme, lexeme);
            tokens[*numTokens].tokenType = RELATIONAL_OPERATOR;
            (*numTokens)++;
        } else if (currentChar == '"') { // String
            lexemeIndex = 0;
            lexeme[lexemeIndex++] = currentChar;

            while ((currentChar = fgetc(inputFile)) != '"') {
                if (currentChar == '\\') {
                    lexeme[lexemeIndex++] = currentChar; // Include the escape character
                    currentChar = fgetc(inputFile);
                }
                lexeme[lexemeIndex++] = currentChar;
            }

            lexeme[lexemeIndex++] = currentChar; // Include the closing double quote
            lexeme[lexemeIndex] = '\0';
            printf("<STRING, %s>\n", lexeme);

            strcpy(tokens[*numTokens].lexeme, lexeme);
            tokens[*numTokens].tokenType = STRING;
            (*numTokens)++;
        } else if (currentChar == ';') {
            printf("<SEMICOLON, ;>\n");

            strcpy(tokens[*numTokens].lexeme, ";");
            tokens[*numTokens].tokenType = SEMICOLON;
            (*numTokens)++;
        } else if (currentChar == '(') {
            printf("<LEFT_PAREN, (>\n");

            strcpy(tokens[*numTokens].lexeme, "(");
            tokens[*numTokens].tokenType = LEFT_PAREN;
            (*numTokens)++;
        } else if (currentChar == ')') {
            printf("<RIGHT_PAREN, )>\n");

            strcpy(tokens[*numTokens].lexeme, ")");
            tokens[*numTokens].tokenType = RIGHT_PAREN;
            (*numTokens)++;
        } else if (currentChar == '{') {
            printf("<LEFT_BRACE, {>\n");

            strcpy(tokens[*numTokens].lexeme, "{");
            tokens[*numTokens].tokenType = LEFT_BRACE;
            (*numTokens)++;
        } else if (currentChar == '}') {
            printf("<RIGHT_BRACE, }>\n");

            strcpy(tokens[*numTokens].lexeme, "}");
            tokens[*numTokens].tokenType = RIGHT_BRACE;
            (*numTokens)++;
        } else if (currentChar == '/') {
            printf("<SLASH, />\n");

            strcpy(tokens[*numTokens].lexeme, "/");
            tokens[*numTokens].tokenType = SLASH;
            (*numTokens)++;
        } else if (currentChar == '\\') {
            printf("<BACKSLASH, \\>\n");

            strcpy(tokens[*numTokens].lexeme, "\\");
            tokens[*numTokens].tokenType = BACKSLASH;
            (*numTokens)++;
        } else if (currentChar != ' ' && currentChar != '\n' && currentChar != '\r') {
            // Ignore whitespace characters
            printf("Error: Unknown character '%c'\n", currentChar);
        }
    }
}

// Enumeration of non-terminal types
enum {
    PROGRAM,
    DECLARATION,
    TYPE,
    VARIABLE,
    STATEMENT,
    EXPRESSION
};

// Function to perform parsing
void parse(Token *tokens, int numTokens);

// Function to parse the program
void parseProgram(Token *tokens, int *currentTokenIndex, int numTokens);

// Function to parse variable declarations
void parseDeclaration(Token *tokens, int *currentTokenIndex, int numTokens);

// Function to parse types
void parseType(Token *tokens, int *currentTokenIndex, int numTokens);

// Function to parse variables
void parseVariable(Token *tokens, int *currentTokenIndex, int numTokens);



void parse(Token *tokens, int numTokens) {
    int currentTokenIndex = 0;

    // Start parsing the program
    parseProgram(tokens, &currentTokenIndex, numTokens);
}

void parseProgram(Token *tokens, int *currentTokenIndex, int numTokens) {
    // In a real parser, you would have rules to recognize the structure of a program.
    // For simplicity, let's assume a program consists of variable declarations.

    while (*currentTokenIndex < numTokens) {
        parseDeclaration(tokens, currentTokenIndex, numTokens);
    }
}

void parseDeclaration(Token *tokens, int *currentTokenIndex, int numTokens) {
    // In a real parser, you would have rules to recognize variable declarations.
    // For simplicity, let's assume a declaration consists of a type and a variable.

    parseType(tokens, currentTokenIndex, numTokens);
    parseVariable(tokens, currentTokenIndex, numTokens);

    // For simplicity, let's assume a declaration ends with a semicolon.
    if (tokens[*currentTokenIndex].tokenType == SEMICOLON) {
        printf("Parsed: Declaration\n");
        (*currentTokenIndex)++;
    } else {
        printf("Error: Expected semicolon after declaration\n");
    }
}

void parseType(Token *tokens, int *currentTokenIndex, int numTokens) {
    // In a real parser, you would have rules to recognize types.
    // For simplicity, let's assume only basic types like int, float, char.

    if (tokens[*currentTokenIndex].tokenType == KEYWORD) {
        printf("Parsed: Type - %s\n", tokens[*currentTokenIndex].lexeme);
        (*currentTokenIndex)++;
    } else {
        printf("Error: Expected type\n");
    }
}

void parseVariable(Token *tokens, int *currentTokenIndex, int numTokens) {
    // In a real parser, you would have rules to recognize variables.
    // For simplicity, let's assume a variable is an identifier.

    if (tokens[*currentTokenIndex].tokenType == IDENTIFIER) {
        printf("Parsed: Variable - %s\n", tokens[*currentTokenIndex].lexeme);
        (*currentTokenIndex)++;
    } else {
        printf("Error: Expected variable\n");
    }
}

typedef struct TreeNode {
    char lexeme[MAX_IDENTIFIER_LENGTH];
    int nodeType;  // Represents the type of AST node
    struct TreeNode* children[2];  // Assume at most two children for simplicity
} TreeNode;

enum {
    AST_PROGRAM,
    AST_DECLARATION,
    AST_TYPE,
    AST_VARIABLE
};

// Function to create a new AST node
TreeNode* createNode(char* lexeme, int nodeType) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(newNode->lexeme, lexeme);
    newNode->nodeType = nodeType;
    newNode->children[0] = NULL;
    newNode->children[1] = NULL;
    return newNode;
}

// Function to free the AST
void freeAST(TreeNode* root) {
    if (root == NULL) {
        return;
    }

    freeAST(root->children[0]);
    freeAST(root->children[1]);
    free(root);
}

// Function to perform parsing and build AST
TreeNode* parseAndBuildAST(Token* tokens, int numTokens);

// Function to parse the program and build AST
TreeNode* parseProgramAndBuildAST(Token* tokens, int* currentTokenIndex, int numTokens);

// Function to parse variable declarations and build AST
TreeNode* parseDeclarationAndBuildAST(Token* tokens, int* currentTokenIndex, int numTokens);

// Function to parse types and build AST
TreeNode* parseTypeAndBuildAST(Token* tokens, int* currentTokenIndex, int numTokens);

// Function to parse variables and build AST
TreeNode* parseVariableAndBuildAST(Token* tokens, int* currentTokenIndex, int numTokens);

// Function to display the AST
void displayAST(TreeNode* root, int level);

TreeNode* parseAndBuildAST(Token* tokens, int numTokens) {
    int currentTokenIndex = 0;

    // Start parsing the program and build AST
    return parseProgramAndBuildAST(tokens, &currentTokenIndex, numTokens);
}

TreeNode* parseProgramAndBuildAST(Token* tokens, int* currentTokenIndex, int numTokens) {
    TreeNode* programNode = createNode("Program", AST_PROGRAM);

    while (*currentTokenIndex < numTokens) {
        TreeNode* declarationNode = parseDeclarationAndBuildAST(tokens, currentTokenIndex, numTokens);

        // Add declaration to program node
        if (declarationNode != NULL) {
            if (programNode->children[0] == NULL) {
                programNode->children[0] = declarationNode;
            } else {
                programNode->children[1] = declarationNode;
            }
        }
    }

    return programNode;
}

TreeNode* parseDeclarationAndBuildAST(Token* tokens, int* currentTokenIndex, int numTokens) {
    TreeNode* declarationNode = createNode("Declaration", AST_DECLARATION);

    // In a real parser, you would have rules to recognize variable declarations.
    // For simplicity, let's assume a declaration consists of a type and a variable.

    TreeNode* typeNode = parseTypeAndBuildAST(tokens, currentTokenIndex, numTokens);
    TreeNode* variableNode = parseVariableAndBuildAST(tokens, currentTokenIndex, numTokens);

    // Add type and variable to declaration node
    if (typeNode != NULL && variableNode != NULL) {
        declarationNode->children[0] = typeNode;
        declarationNode->children[1] = variableNode;
    } else {
        // Error handling if either type or variable is not present
        printf("Error: Incomplete declaration\n");
        freeAST(declarationNode);
        return NULL;
    }

    // For simplicity, let's assume a declaration ends with a semicolon.
    if (tokens[*currentTokenIndex].tokenType == SEMICOLON) {
        (*currentTokenIndex)++;
    } else {
        printf("Error: Expected semicolon after declaration\n");
        freeAST(declarationNode);
        return NULL;
    }

    return declarationNode;
}

TreeNode* parseTypeAndBuildAST(Token* tokens, int* currentTokenIndex, int numTokens) {
    // In a real parser, you would have rules to recognize types.
    // For simplicity, let's assume only basic types like int, float, char.

    if (tokens[*currentTokenIndex].tokenType == KEYWORD) {
        TreeNode* typeNode = createNode(tokens[*currentTokenIndex].lexeme, AST_TYPE);
        (*currentTokenIndex)++;
        return typeNode;
    } else {
        printf("Error: Expected type\n");
        return NULL;
    }
}

TreeNode* parseVariableAndBuildAST(Token* tokens, int* currentTokenIndex, int numTokens) {
    // In a real parser, you would have rules to recognize variables.
    // For simplicity, let's assume a variable is an identifier.

    if (tokens[*currentTokenIndex].tokenType == IDENTIFIER) {
        TreeNode* variableNode = createNode(tokens[*currentTokenIndex].lexeme, AST_VARIABLE);
        (*currentTokenIndex)++;
        return variableNode;
    } else {
        printf("Error: Expected variable\n");
        return NULL;
    }
}

void displayAST(TreeNode* root, int level) {
    if (root == NULL) {
        return;
    }

    // Indentation based on the level
    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    // Display node information
    printf("%s (%d)\n", root->lexeme, root->nodeType);

    // Recursively display children
    displayAST(root->children[0], level + 1);
    displayAST(root->children[1], level + 1);
}
enum {
    OP_ASSIGN,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_LABEL,
    OP_JUMP,
    OP_IF,
    OP_GOTO,
    OP_READ,
    OP_WRITE,
    OP_RETURN
};
enum {
    ASM_MOVE,   // Move operation (similar to assignment)
    ASM_ADD,    // Add operation
    ASM_SUB,    // Subtract operation
    ASM_MUL,    // Multiply operation
    ASM_DIV,
    ASM_STR,
    ASM_LOAD,    // Divide operation
    // Add more operations as needed
};


typedef struct {
    int op; // Operation type
    char arg1[MAX_IDENTIFIER_LENGTH];
    char arg2[MAX_IDENTIFIER_LENGTH];
    char result[MAX_IDENTIFIER_LENGTH];
} IntermediateCode;

// Function to generate intermediate code for the AST
void generateIntermediateCode(TreeNode* root, IntermediateCode* code, int* codeIndex);

// Helper function to create temporary variables
char* createTempVar(int index);

// Helper function to generate three-address code for variable declarations
void generateDeclarationCode(TreeNode* declarationNode, IntermediateCode* code, int* codeIndex);

// Helper function to generate three-address code for assignments
void generateAssignmentCode(TreeNode* assignmentNode, IntermediateCode* code, int* codeIndex);

// Helper function to generate three-address code for expressions
void generateExpressionCode(TreeNode* expressionNode, IntermediateCode* code, int* codeIndex);

void generateIntermediateCode(TreeNode* root, IntermediateCode* code, int* codeIndex) {
    if (root == NULL) {
        return;
    }

    switch (root->nodeType) {
        case AST_PROGRAM:
            // Traverse children
            generateIntermediateCode(root->children[0], code, codeIndex);
            generateIntermediateCode(root->children[1], code, codeIndex);
            break;

        case AST_DECLARATION:
            generateDeclarationCode(root, code, codeIndex);
            break;

        case AST_TYPE:
            // Nothing to do for types in this example
            break;

        case AST_VARIABLE:
            // Nothing to do for variables in this example
            break;

        // Add cases for other node types as needed

        default:
            printf("Error: Unknown AST node type\n");
            break;
    }
}

void generateDeclarationCode(TreeNode* declarationNode, IntermediateCode* code, int* codeIndex) {
    TreeNode* typeNode = declarationNode->children[0];
    TreeNode* variableNode = declarationNode->children[1];

    // For simplicity, let's assume a variable declaration initializes the variable to zero
    strcpy(code[*codeIndex].result, variableNode->lexeme);
    strcpy(code[*codeIndex].arg1, "0");
    code[*codeIndex].op = OP_ASSIGN;
    (*codeIndex)++;
}

// Helper function to create temporary variables
char* createTempVar(int index) {
    char* tempVar = (char*)malloc(MAX_IDENTIFIER_LENGTH);
    snprintf(tempVar, MAX_IDENTIFIER_LENGTH, "temp%d", index);
    return tempVar;
}

void generateAssignmentCode(TreeNode* assignmentNode, IntermediateCode* code, int* codeIndex) {
    // Assuming the assignment node has two children: variable and expression
    TreeNode* variableNode = assignmentNode->children[0];
    TreeNode* expressionNode = assignmentNode->children[1];

    // Generate intermediate code for the expression
    generateExpressionCode(expressionNode, code, codeIndex);

    // Create a new temporary variable to store the result of the expression
    code[*codeIndex].op = ASM_MOVE;
    strcpy(code[*codeIndex].arg1, expressionNode->lexeme);
    strcpy(code[*codeIndex].result, variableNode->lexeme);
    (*codeIndex)++;
}

void generateExpressionCode(TreeNode* expressionNode, IntermediateCode* code, int* codeIndex) {
    if (expressionNode == NULL) {
        return;
    }

    switch (expressionNode->nodeType) {
        case AST_VARIABLE:
            // If the expression is a variable, use its name as the argument
            //strcpy(code[*codeIndex].result, expressionNode->lexeme);
            break;

        case AST_TYPE:
            // For simplicity, ignore types in expressions
            break;

        // Add cases for other node types as needed

        default:
            printf("Error: Unsupported expression node type\n");
            break;
    }
}





void generateAssemblyCode(IntermediateCode* intermediateCode, int codeIndex);

void generateAssemblyCode(IntermediateCode* intermediateCode, int codeIndex) {
    printf("\n5.Generated Assembly Code:\n");
    printf("\n");

    for (int i = 0; i < codeIndex; i++) {
        switch (intermediateCode[i].op) {
            case ASM_MOVE:
                printf("MOV %s, %s\n", intermediateCode[i].result, intermediateCode[i].arg1);
                break;
            case ASM_ADD:
                printf("ADD %s, %s, %s\n", intermediateCode[i].result, intermediateCode[i].arg1, intermediateCode[i].arg2);
                break;
            case ASM_SUB:
                printf("SUB %s, %s, %s\n", intermediateCode[i].result, intermediateCode[i].arg1, intermediateCode[i].arg2);
                break;
            case ASM_MUL:
                printf("MUL %s, %s, %s\n", intermediateCode[i].result, intermediateCode[i].arg1, intermediateCode[i].arg2);
                break;
            case ASM_DIV:
                printf("DIV %s, %s, %s\n", intermediateCode[i].result, intermediateCode[i].arg1, intermediateCode[i].arg2);
                break;
            // Add more cases for other operations as needed
            default:
                printf("Unknown operation\n");
                break;
        }
    }
}

int main() {
    FILE *inputFile;
    Token tokens[MAX_TOKENS];
    int numTokens = 0;

    inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    printf("1.Lexical Analysis:\n");
    printf("\n");
    lexicalAnalysis(inputFile, tokens, &numTokens);
    printf("**************************************\n");

    // Perform parsing
    printf("\n2.Parsing:\n");
    printf("\n");
    parse(tokens, numTokens);
    printf("Completed Parsing\n");
     printf("**************************************\n");

    // Display the symbol table
    displaySymbolTable();
     printf("**************************************\n");

    printf("\n3.Semantic Analysis:\n");
    TreeNode* astRoot = parseAndBuildAST(tokens, numTokens);
    //printf("Completed Parsing and Building AST\n");



    // Display the AST
     printf("\nAbstract Syntax Tree (Tree Representation):\n");
    displayAST(astRoot, 0);
     printf("**************************************\n");

    printf("\n4.Intermediate Code Generation:\n");
    IntermediateCode intermediateCode[MAX_TOKENS];
    int codeIndex = 0;
    generateIntermediateCode(astRoot, intermediateCode, &codeIndex);

    // Display generated intermediate code
    printf("\nGenerated Intermediate Code:\n");
    for (int i = 0; i < codeIndex; i++) {
        printf("%d: ", i + 1);

        switch (intermediateCode[i].op) {
            case OP_ASSIGN:
                printf("%s = %s\n", intermediateCode[i].result, intermediateCode[i].arg1);
                break;
            // Add cases for other operations as needed
            default:
                printf("Unknown operation\n");
                break;
        }
    }
     printf("**************************************\n");
     generateAssemblyCode(intermediateCode, codeIndex);
      printf("**************************************\n");


    // ... (Free AST and close file as before)


    // Free the AST
    freeAST(astRoot);

    fclose(inputFile);

    return 0;
}


