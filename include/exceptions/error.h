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
    CompileError(std::string msg)
        : message(msg),loc{"", 0, 0}{}
};

class RuntimeError : public std::exception {
public:
    std::string message;
    SourceLocation loc;

    RuntimeError(std::string msg, SourceLocation l)
        : message(msg), loc(l) {}

    RuntimeError(std::string msg)
        : message(msg),loc{"", 0, 0} {}
};
// Internal signal used by throwBuiltinException to jump back to the dispatch loop
struct ThrowExceptionSignal {};
