#include <iostream>
#include <vector>
#include <regex>
#include <string>

// Enum to categorize tokens
enum TokenType {
    KEYWORD, IDENTIFIER, STRING_CONSTANT, NUMERIC_CONSTANT, HEXADECIMAL_NUMBER,
    DECIMAL_NUMBER, OPERATOR, DELIMITER, PREPROCESSOR_DIRECTIVE, COMMENT, UNKNOWN
};

// Token structure
struct Token {
    std::string value;
    TokenType type;
};

// Function declarations
bool isKeyword(const std::string& token);
bool isIdentifier(const std::string& token);
bool isStringConstant(const std::string& token);
bool isNumericConstant(const std::string& token);
bool isOperator(const std::string& token);
bool isDelimiter(const std::string& token);
bool isPreprocessorDirective(const std::string& token);
std::vector<Token> tokenize(const std::string& code);
void analyze(const std::vector<Token>& tokens);
void displayTokens(const std::vector<Token>& tokens);

// Main function
int main() {
    std::string code = R"(
using System;
#pragma
#define
namespace HelloWorld {
    class Program {
        static void Main(string[] args) {
            Console.WriteLine("Hello, my dear friends!");

            // Identifiers
            int decimalNumber = 123; // Decimal number
            double floatingPointNumber = 3.14; // Floating-point number
            string text = "Example text identifier";

            // Keywords
            bool isTrue = true;
            if (isTrue) {
                Console.WriteLine("This is the 'true' keyword.");
            }

            // Hexadecimal Numbers
            int hexNumber = 0x1a3;
            int bitShift = hexNumber << 4;
            int bitAnd = hexNumber & 0x2B;
            int bitOr = hexNumber | 0x2B;
            double hexDouble = 0x1.2p3; // Hexadecimal floating-point number

            // Decimal Numbers
            decimal decimalValue = 123.45m; // Decimal number with 'm' suffix

            // Numeric Constants
            const int constantInteger = 42;
            const double constantDouble = 2.71828;

            // Comments
            // Single-line comment
            /*
                Multi-line comment
            */
        }
    }
}

    )";
    std::vector<Token> tokens = tokenize(code);
    analyze(tokens);
    displayTokens(tokens);
    return 0;
}

// Function implementations
bool isKeyword(const std::string& token) {
    // List of C# keywords
    std::vector<std::string> keywords = { "abstract", "as", "base", "bool", "break", "byte", "case", "catch", "char", "checked", "class", "const", "continue", "decimal", "default", "delegate", "do", "double", "else", "enum", "event", "explicit", "extern", "false", "finally", "fixed", "float", "for", "foreach", "goto", "if", "implicit", "in", "int", "interface", "internal", "is", "lock", "long", "namespace", "new", "null", "object", "operator", "out", "override", "params", "private", "protected", "public", "readonly", "ref", "return", "sbyte", "sealed", "short", "sizeof", "stackalloc", "static", "string", "struct", "switch", "this", "throw", "true", "try", "typeof", "uint", "ulong", "unchecked", "unsafe", "ushort", "using", "virtual", "void", "volatile", "while" };

    // Check if token is a keyword
    return std::find(keywords.begin(), keywords.end(), token) != keywords.end();
}

bool isIdentifier(const std::string& token) {
    // Regular expression for C# identifiers: [_a-zA-Z][_a-zA-Z0-9]*
    std::regex identifier_regex("[_a-zA-Z][_a-zA-Z0-9]*");
    return std::regex_match(token, identifier_regex);
}

bool isStringConstant(const std::string& token) {
    // Regular expression for string constant: "([^"\\]|\\.)*"
    std::regex string_regex("\"([^\"\\\\]|\\\\.)*\"");
    return std::regex_match(token, string_regex);
}

bool isHexadecimalNumber(const std::string& token) {
    // Regular expression for hexadecimal numbers (e.g., 0x1a3, 0X1A3)
    std::regex hexPattern("^0[xX][0-9a-fA-F]+$");
    return std::regex_match(token, hexPattern);
}

bool isDecimalNumber(const std::string& token) {
    // Regular expression for decimal numbers (e.g., 1.23, .23, 1.)
    std::regex decimalPattern("^\\d*\\.\\d+|\\d+\\.$");
    return std::regex_match(token, decimalPattern);
}

bool isNumericConstant(const std::string& token) {
    // Regular expression for numeric constant: integer or floating-point
    std::regex numeric_regex("([0-9]*\\.[0-9]+|[0-9]+)");
    return std::regex_match(token, numeric_regex);
}

bool isOperator(const std::string& token) {
    // List of C# operators
    std::vector<std::string> operators = { "+", "-", "*", "/", "%", "&", "|", "^", "!", "~", "++", "--", "<<", ">>", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "??", "?:", "=>", "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", "[]", "()", ".", "->" };

    // Check if token is an operator
    return std::find(operators.begin(), operators.end(), token) != operators.end();
}

bool isDelimiter(const std::string& token) {
    // List of C# delimiters
    std::vector<std::string> delimiters = { ",", ";", "{", "}", "(", ")", "[", "]", ":", "?" };

    // Check if token is a delimiter
    return std::find(delimiters.begin(), delimiters.end(), token) != delimiters.end();
}

bool isPreprocessorDirective(const std::string& token) {
    // Regular expression for preprocessor directives: #<directive>
    std::regex preprocessor_regex("#[a-z]+");
    return std::regex_match(token, preprocessor_regex);
}

std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;
    std::string token;
    bool isString = false;
    bool isSingleLineComment = false;
    bool isMultiLineComment = false;

    // Iterate over the code character by character
    for (size_t i = 0; i < code.length(); ++i) {
        char c = code[i];

        // Handle comments
        if (c == '/' && !isString && !isMultiLineComment) {
            if (i + 1 < code.length()) {
                if (code[i + 1] == '/') isSingleLineComment = true;
                else if (code[i + 1] == '*') isMultiLineComment = true;
            }
        }
        // Handle preprocessor directives
        if (c == '#' && !isString && !isSingleLineComment && !isMultiLineComment) {
            token += c;
            while (i + 1 < code.length() && !std::isspace(code[i + 1])) {
                token += code[++i];
            }
            tokens.push_back({ token, PREPROCESSOR_DIRECTIVE });
            token.clear();
            continue;
        }

        // Handle end of comments
        if (isSingleLineComment && c == '\n') {
            isSingleLineComment = false;
            tokens.push_back({ token, COMMENT });
            token.clear();
        }
        if (isMultiLineComment && c == '*' && i + 1 < code.length() && code[i + 1] == '/') {
            isMultiLineComment = false;
            token += "*/";
            tokens.push_back({ token, COMMENT });
            token.clear();
            ++i;  // Skip the closing '/'
            continue;
        }

        // Collect characters inside comments
        if (isSingleLineComment || isMultiLineComment) {
            token += c;
            continue;
        }

        // Handle strings
        if (c == '\"') {
            if (isString) {
                token += c;
                tokens.push_back({ token, STRING_CONSTANT });
                token.clear();
            }
            else {
                if (!token.empty()) {
                    tokens.push_back({ token, UNKNOWN });  // Add the previous token as UNKNOWN for now
                    token.clear();
                }
            }
            isString = !isString;
        }
        else if (isString) {
            token += c;
        }
        // Handle other tokens
        else if (std::isspace(c)) {
            if (!token.empty()) {
                tokens.push_back({ token, UNKNOWN });  // Add the previous token as UNKNOWN for now
                token.clear();
            }
        }
        else if (std::ispunct(c)) {
            if (!token.empty()) {
                tokens.push_back({ token, UNKNOWN });  // Add the previous token as UNKNOWN for now
                token.clear();
            }
            tokens.push_back({ std::string(1, c), UNKNOWN });  // Add the punctuation as UNKNOWN for now
        }
        else {
            token += c;
        }
    }

    // Check for any remaining token
    if (!token.empty()) {
        tokens.push_back({ token, UNKNOWN });  // Add the last token as UNKNOWN for now
    }

    // Categorize the tokens
    for (Token& t : tokens) {
        if (isKeyword(t.value)) t.type = KEYWORD;
        else if (isIdentifier(t.value)) t.type = IDENTIFIER;
        else if (isHexadecimalNumber(t.value)) t.type = HEXADECIMAL_NUMBER;
        else if (isDecimalNumber(t.value)) t.type = DECIMAL_NUMBER;
        else if (isNumericConstant(t.value)) t.type = NUMERIC_CONSTANT;
        else if (isOperator(t.value)) t.type = OPERATOR;
        else if (isDelimiter(t.value)) t.type = DELIMITER;
        else if (isPreprocessorDirective(t.value)) t.type = PREPROCESSOR_DIRECTIVE;
        // String constants and comments are already categorized
    }

    return tokens;
}

void analyze(const std::vector<Token>& tokens) {
    int keywordCount = 0;
    int identifierCount = 0;
    int stringConstantCount = 0;
    int numericConstantCount = 0;
    int operatorCount = 0;
    int delimiterCount = 0;
    int preprocessorDirectiveCount = 0;
    int unknownCount = 0;

    // Count the number of tokens of each type
    for (const Token& token : tokens) {
        switch (token.type) {
        case KEYWORD: ++keywordCount; break;
        case IDENTIFIER: ++identifierCount; break;
        case STRING_CONSTANT: ++stringConstantCount; break;
        case NUMERIC_CONSTANT: ++numericConstantCount; break;
        case OPERATOR: ++operatorCount; break;
        case DELIMITER: ++delimiterCount; break;
        case PREPROCESSOR_DIRECTIVE: ++preprocessorDirectiveCount; break;
        case UNKNOWN: ++unknownCount; break;
        }
    }

    // Display the counts
    std::cout << "Analysis Results:" << std::endl;
    std::cout << "Keywords: " << keywordCount << std::endl;
    std::cout << "Identifiers: " << identifierCount << std::endl;
    std::cout << "String Constants: " << stringConstantCount << std::endl;
    std::cout << "Numeric Constants: " << numericConstantCount << std::endl;
    std::cout << "Operators: " << operatorCount << std::endl;
    std::cout << "Delimiters: " << delimiterCount << std::endl;
    std::cout << "Preprocessor Directives: " << preprocessorDirectiveCount << std::endl;
    std::cout << "Unknown: " << unknownCount << std::endl;
}

void displayTokens(const std::vector<Token>& tokens) {
    // Loop through each token in tokens vector, printing its value and type
    for (const Token& token : tokens) {
        std::cout << "Lexeme: " << token.value << " - Type: ";
        switch (token.type) {
        case KEYWORD: std::cout << "Keyword"; break;
        case IDENTIFIER: std::cout << "Identifier"; break;
        case STRING_CONSTANT: std::cout << "String Constant"; break;
        case NUMERIC_CONSTANT: std::cout << "Numeric Constant"; break;
        case HEXADECIMAL_NUMBER: std::cout << "Hexadecimal Number"; break;
        case DECIMAL_NUMBER: std::cout << "Decimal Number"; break;
        case OPERATOR: std::cout << "Operator"; break;
        case DELIMITER: std::cout << "Delimiter"; break;
        case PREPROCESSOR_DIRECTIVE: std::cout << "Preprocessor Directive"; break;
        case COMMENT: std::cout << "Comment"; break;
        case UNKNOWN: std::cout << "Unknown"; break;
        }
        std::cout << std::endl;
    }
}
