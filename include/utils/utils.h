#pragma once

#include <memory>
#include <string>
#include "manifest/manifest.h"

class Program;

std::unique_ptr<Program> parseFile(
    const std::string& filePath);

std::string changeExtension(
    const std::string& filePath,
    const std::string& extension);

std::filesystem::path getBuildOutputPath(const std::string&file,const Manifest& manifest);

void printError(std::string phase,std::string msg , int line_num , int col_num , std::string line);
bool hasExtension(const std::string& filePath, const std::string& extension);