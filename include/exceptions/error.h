#pragma once
#include <string>
#include <exception>
#include "lexer/lexer.h"

class CompileError : public std::exception {
public:
    std::string message;
    SourceLocation loc;

    CompileError(std::string msg, SourceLocation l)
        : message(msg), loc(l) {}
};

class RuntimeError : public std::exception {
public:
    std::string message;
    SourceLocation loc;

    RuntimeError(std::string msg, SourceLocation l)
        : message(msg), loc(l) {}
};
// Internal signal used by throwPenguinException to jump back to the dispatch loop
struct PenguinThrow {};
