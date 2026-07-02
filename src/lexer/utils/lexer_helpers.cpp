#include "lexer/lexer.h"
#include "exceptions/error.h"
#include <unordered_map>

bool Lexer::isAtEnd() const {
    return current >= source.size();
}
char Lexer::advance() {
    char c = source[current++];
    col_num++;
    if (c == '\n') {
        line_num++;
        col_num = 1;
        line_start = current;
        size_t end = source.find('\n', line_start);
        if (end == std::string::npos) end = source.length();
        current_line = source.substr(line_start, end - line_start);

    }
    return c;
}
char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[current];
}
void Lexer::addToken(TokenType type, const std::string& lexeme) {
    int token_col = col_num - lexeme.length();
    if (token_col < 1) token_col = 1;

    SourceLocation loc{current_line, line_num, token_col};
    tokens.emplace_back(type, lexeme, loc);
}

void Lexer::number() {
    size_t start = current - 1;
    while (isdigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isdigit(source[current + 1])) {
        // Consume the "."
        advance();

        while (isdigit(peek())) advance();
    }

    std::string value = source.substr(start, current - start);
    addToken(TokenType::NUMBER, value);
}

void Lexer::_string() {
    size_t start = current;
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            // Handle newlines if tracked
        }
        advance();
    }

    if (isAtEnd()) {
        throw CompileError("Unterminated string.", {current_line,line_num, col_num});
        return;
    }

    std::string value = source.substr(start, current - start);
    advance(); // Consume the closing "
    addToken(TokenType::STRING, value);
}

void Lexer::_char(){
    if (isAtEnd()) {
        throw CompileError("Unterminated char.", {current_line,line_num, col_num});
        return;
    }

    char value = advance();

    if (peek() != '\'') {
        throw CompileError("Char literal must contain exactly one character.", {current_line,line_num, col_num});
        return;
    }

    advance();
    addToken(TokenType::CHAR, std::string(1, value));
}

void Lexer::identifier() {
    size_t start = current - 1;
    while (isalnum(peek()) || peek() == '_') advance();

    std::string value = source.substr(start, current - start);
    
    static const std::unordered_map<std::string, TokenType> keywords = {
        {"if", TokenType::KEYWORD},
        {"else", TokenType::KEYWORD},
        {"while", TokenType::KEYWORD},
        {"return", TokenType::KEYWORD},
        {"func", TokenType::KEYWORD},
        {"true", TokenType::KEYWORD},
        {"false", TokenType::KEYWORD},
        {"break", TokenType::KEYWORD},
        {"continue", TokenType::KEYWORD},
        {"for",TokenType::KEYWORD},
        {"class",TokenType::KEYWORD},
        {"dec",TokenType::KEYWORD},
        {"private",TokenType::KEYWORD},
        {"public",TokenType::KEYWORD},
        {"protected",TokenType::KEYWORD},
        {"inherits",TokenType::KEYWORD},
        {"shared",TokenType::KEYWORD},
        {"trait",TokenType::KEYWORD},
        {"impl",TokenType::KEYWORD},
        {"include",TokenType::KEYWORD},
        {"from",TokenType::KEYWORD},
        {"alias",TokenType::KEYWORD},
        {"export",TokenType::KEYWORD},
        {"try",TokenType::KEYWORD},
        {"catch",TokenType::KEYWORD},
        {"finally",TokenType::KEYWORD},
        {"throw",TokenType::KEYWORD},
        {"super",TokenType::KEYWORD},
    };

    if (keywords.count(value)) {
        addToken(TokenType::KEYWORD, value);
    } else {
        addToken(TokenType::IDENTIFIER, value);
    }
}