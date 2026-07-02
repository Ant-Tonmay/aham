#include "loader/module_loader.h"
#include "exceptions/error.h"

std::string ModuleResolver::resolve(
    const std::string& currentFile,
    const std::string& moduleName,
    const std::string& extension
    )
{
    namespace fs = std::filesystem;

     std::string path = moduleName;
        
    std::replace(path.begin(),path.end(),'.',fs::path::preferred_separator);

    // if Manifest file exists look for manifest file 
    if(loader.exists(currentFile)){
        const Manifest& manifest = loader.load(currentFile);
        std::vector<fs::path> candidates ;
        //while building
        if (extension == ".aha")
        {
            candidates = {
                manifest.sourceDir / (path + extension),
                manifest.libraryDir / (path + extension)
            };
        }
        else if (extension == ".ahc")
        {   // while running
            candidates = {
                manifest.buildDir / (path + extension)
            };
        }
        else
        {
            throw std::runtime_error(
                "Unsupported module extension: " + extension);
        }
        for (const auto& candidate : candidates)
        {
            if (fs::exists(candidate))
            {
                return fs::canonical(candidate).string();
            }
        }
    }else{
        fs::path currentDir =
        fs::path(currentFile).parent_path();
        while (true)
        {
            fs::path candidate =
                currentDir /
                (path + extension);

            if (fs::exists(candidate))
            {
                return fs::canonical(candidate).string();
            }

            if (currentDir == currentDir.parent_path())
            {
                break;
            }

            currentDir =
                currentDir.parent_path();
        }
    }

    throw RuntimeError(
        "Cannot find module '" +
        moduleName + "'"
    );
}