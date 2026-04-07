#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "parser/ast.h"

struct LoadedModule {
    std::string name;
    std::string path;
    std::unique_ptr<Program> program;
    std::unordered_set<std::string> exports;
    bool linked = false;
};

class ModuleLoader {
public:
    std::unique_ptr<Program> linkFromEntryFile(const std::string& entryFile);

private:
    std::unordered_map<std::string, std::unique_ptr<LoadedModule>> modulesByPath;
    std::unordered_set<std::string> loading;
    std::unordered_set<std::string> globalNames;

    LoadedModule* loadModuleFromPath(const std::string& filePath);
    std::unique_ptr<Program> parseFile(const std::string& filePath);

    std::string resolveImportPath(const std::string& currentFile, const std::string& moduleName);
    std::string normalizePath(const std::string& path);
    std::string moduleNameFromPath(const std::string& path);

    void collectExports(LoadedModule& mod);
    void validateExportsExist(const LoadedModule& mod);
    void linkModuleRecursive(LoadedModule* mod, Program& out, bool isEntry);

    void mergeAllDeclarations(LoadedModule* mod, Program& out, bool includeMain);
    void ensureExportedMembersExist(const LoadedModule& imported, const IncludeStmt& inc);
    void ensureNoDuplicateGlobal(const std::string& name);

    bool hasDeclarationNamed(const LoadedModule& mod, const std::string& name) const;
};
