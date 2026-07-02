#include "lexer/lexer.h"
#include <cctype>
#include <vector>
#include <string>
#include "exceptions/error.h"

Token::Token(TokenType type, const std::string& lexeme, SourceLocation location)
    : type(type), lexeme(lexeme), location(location) {}

Token::Token(TokenType type, const std::string& lexeme)
    : type(type), lexeme(lexeme), location{"", 0, 0} {}

std::string Token::toString() const {
    std::string typeStr;
    switch (type) {
        case TokenType::IDENTIFIER: typeStr = "IDENTIFIER"; break;
        case TokenType::NUMBER: typeStr = "NUMBER"; break;
        case TokenType::PLUS: typeStr = "PLUS"; break;
        case TokenType::MINUS: typeStr = "MINUS"; break;
        case TokenType::STAR: typeStr = "STAR"; break;
        case TokenType::SLASH: typeStr = "SLASH"; break;
        case TokenType::EQUAL: typeStr = "EQUAL"; break;
        case TokenType::EQUAL_EQUAL: typeStr = "EQUAL_EQUAL"; break;
        case TokenType::LESS: typeStr = "LESS"; break;
        case TokenType::GREATER: typeStr = "GREATER"; break;
        case TokenType::LPAREN: typeStr = "LPAREN"; break;
        case TokenType::RPAREN: typeStr = "RPAREN"; break;
        case TokenType::LBRACKET: typeStr = "LBRACKET"; break;
        case TokenType::RBRACKET: typeStr = "RBRACKET"; break;
        case TokenType::LBRACE: typeStr = "LBRACE"; break;
        case TokenType::RBRACE: typeStr = "RBRACE"; break;
        case TokenType::SEMICOLON: typeStr = "SEMICOLON"; break;
        case TokenType::COMMA: typeStr = "COMMA"; break;
        case TokenType::STRING: typeStr = "STRING"; break;
        case TokenType::CHAR: typeStr = "CHAR"; break;
        case TokenType::KEYWORD: typeStr = "KEYWORD"; break;
        case TokenType::EOF_TOKEN: typeStr = "EOF"; break;
        default: typeStr = "UNKNOWN"; break;
    }
    return "Token(" + typeStr + ", \"" + lexeme + "\")";
}

Lexer::Lexer(const std::string& src)
    : source(src), current(0) {
        size_t end = source.find('\n', 0);
        if (end == std::string::npos) end = source.length();
        current_line = source.substr(0, end);
    }

std::vector<Token> Lexer::tokenize() {
    while (!isAtEnd()) {
        char c = advance();

        switch (c) {
            case '+': 
                if(peek() =='='){
                    advance();
                    addToken(TokenType::PLUS_EQUAL,"+=");
                }else{
                    addToken(TokenType::PLUS,"+");
                }
                break;
            case '-':
                if(peek() =='='){
                    advance();
                    addToken(TokenType::MINUS_EQUAL,"-=");
                }else{
                    addToken(TokenType::MINUS,"-");
                }
                break;
            case '*': 
                if(peek() == '='){
                    advance();
                    addToken(TokenType::STAR_EQUAL,"*=");
                }else{
                    addToken(TokenType::STAR, "*");
                }
                break;
            case '%':
                if(peek() == '='){
                        advance();
                        addToken(TokenType::MOD_OP_EQUAL,"%=");
                }else{
                        addToken(TokenType::MOD_OP, "%");
                }
                break; 
            case '/': 
                if (peek() == '/') {
                    while (peek() != '\n' && !isAtEnd()) advance();
                }else if (peek() == '*'){
                    advance();
                    int depth = 1;
                    while(!isAtEnd() && depth>0){
                        char c = advance();
                        if(c == '\n' ){
                            continue;
                        }else if(c == '/'){
                            if (!isAtEnd() && peek() == '*') {
                                 advance(); // consume '*'
                                 depth++;
                            }
                            
                        }else if(c == '*'){
                            if (!isAtEnd() && peek() == '/') {
                                advance(); // consume '/'
                                depth--;
                            }
                        }
                    }
                    if (depth != 0) {
                        throw CompileError("Unterminated multi-line comment.",{current_line, line_num, col_num});
                    }
                }
                else if(peek() == '='){
                    advance();
                    addToken(TokenType::SLASH_EQUAL,"/=");
                }else {
                    addToken(TokenType::SLASH, "/");
                }
                break;
            case '&' :
                if(peek() == '&'){
                    advance();
                    addToken(TokenType::AND, "&&");
                }else if(peek() == '='){
                    advance();
                    addToken(TokenType::BITWISE_AND_EQUAL, "&=");
                }else{
                    addToken(TokenType::BITWISE_AND, "&");
                }
                break;
            case '|' :
                if(peek() == '|'){
                    advance();
                    addToken(TokenType::OR, "||");
                }else if(peek() == '='){
                    advance();
                    addToken(TokenType::BITWISE_OR_EQUAL, "|=");
                }else{
                    addToken(TokenType::BITWISE_OR, "|");
                }
                break;
            case '^' :
                if(peek() =='='){
                    advance();
                    addToken(TokenType::XOR_EQUAL,"^=");
                }else{
                    addToken(TokenType::BITWISE_XOR, "^");
                }
                break;
            case '!' :
                if(peek() == '='){
                    advance();
                    addToken(TokenType::NOT_EQUAL, "!=");
                }else{
                    addToken(TokenType::NOT, "!");
                }
                break;
            case '=':
                if (peek() == '=') {
                    advance();
                    addToken(TokenType::EQUAL_EQUAL, "==");
                } else {
                    addToken(TokenType::EQUAL, "=");
                }
                break;
            case '<': 
                if(peek() =='='){
                    advance();
                    addToken(TokenType::LESS_EQUAL, "<=");
                }else if (peek() == '<'){
                    advance();
                    addToken(TokenType::LEFT_SHIFT, "<<");
                }else{
                    addToken(TokenType::LESS, "<");
                }
                break;
            case '>': 
                if(peek() == '='){
                    advance();  
                    addToken(TokenType::GREATER_EQUAL, ">=");
                }else if(peek() == '>'){
                    advance();
                    addToken(TokenType::RIGHT_SHIFT, ">>");
                }else{
                    addToken(TokenType::GREATER, ">");
                }
                break;
            case '(': addToken(TokenType::LPAREN, "("); break;
            case ')': addToken(TokenType::RPAREN, ")"); break;
            case '[': addToken(TokenType::LBRACKET, "["); break;
            case ']': addToken(TokenType::RBRACKET, "]"); break;
            case '{': addToken(TokenType::LBRACE, "{"); break;
            case '}': addToken(TokenType::RBRACE, "}"); break;
            case ';': addToken(TokenType::SEMICOLON, ";"); break;
            case ':': addToken(TokenType::COLON, ":"); break;
            case ',': addToken(TokenType::COMMA, ","); break;
            case '.': addToken(TokenType::DOT, "."); break;
            case '"': _string(); break;
            case '\'': _char(); break;

            case ' ':
            case '\t':
            case '\n':
            case '\r':
                break; // ignore whitespace

            default:
                if (isdigit(c)) {
                    number();
                } else if (isalpha(c) || c == '_') {
                    identifier();
                }  else {
                    throw CompileError("Unexpected character: " + std::string(1, c), {current_line, line_num, col_num});
                }
            }
        }

    size_t end_of_line = source.find('\n', line_start);
    if (end_of_line == std::string::npos) end_of_line = source.length();
    std::string current_line_str = source.substr(line_start, end_of_line - line_start);
    SourceLocation loc{current_line_str, line_num, col_num};
    tokens.emplace_back(TokenType::EOF_TOKEN, "", loc);
    return tokens;
}
