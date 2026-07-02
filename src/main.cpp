#include <iostream>
#include "lexer/lexer.h"
#include "compiler/compiler.h"
#include "vm/vm.h"
#include "compiler/utils/serializer.h"
#include "compiler/utils/deserializer.h"
#include "exceptions/error.h"
#include "dependency_graph/dependency_graph.h"
#include "dependency_graph/dependency_scanner.h"
#include "utils/utils.h"
#include "manifest/manifest_loader.h"

static void printInfo() {
    std::cout << "Aham Programming Language\n";
    std::cout << "Version: 0.1.0\n\n";

    std::cout << "Usage:\n";
    std::cout << "  aham [options] <file>\n\n";

    std::cout << "Options:\n";
    std::cout << "  -c <file>    Compile a source file (.aha) to bytecode (.ahc)\n";
    std::cout << "  -r <file>    Execute a compiled bytecode file (.ahc)\n";
    std::cout << "  --version    Display version information\n";
    std::cout << "  -v           Display version information\n";
    std::cout << "  --help       Display help message\n";
    std::cout << "  -h           Display help message\n";
    std::cout << "  build        Build the current project\n";
    std::cout << "  run          Build (if needed) and run the current project\n";
}

static void printVersion() {
    std::cout << "Aham Programming Language\n";
    std::cout << "Version: 0.1.0\n";
}

static void print_help(){
    std::cout << "Usage:\n";
    std::cout << "  aham [options] <file>\n\n";

    std::cout << "Options:\n";
    std::cout << "  -c <file>    Compile a source file (.aha) to bytecode (.ahc)\n";
    std::cout << "  -r <file>    Execute a compiled bytecode file (.ahc)\n";
    std::cout << "  --version    Display version information\n";
    std::cout << "  -v           Display version information\n";
    std::cout << "  build        Build the current project\n";
    std::cout << "  run          run the current project\n";
}

int main(int argc, char* argv[]) {

        // ---- No arguments ----
    if (argc == 1) {
        std::cerr << "Error: no input file\n";
        std::cerr << "Use: aham <file.aha> or aham --info\n";
        return 1;
    }

    // ---- Handle flags ----
    std::string arg1 = argv[1];

    if (arg1 == "--info") {
        printInfo();
        return 0;
    }

    if (arg1 == "--help" || arg1 == "-h") {
        print_help();
        return 0;
    }

    if (arg1 == "--version" || arg1 == "-v") {
        printVersion();
        return 0;
    }

    enum class Mode {
        UNKNOWN_MODE,
        COMPILE_TO_FILE,
        RUN_FROM_FILE,
        BUILD_PROJECT,
        RUN_PROJECT,
    };
    Mode mode = Mode::UNKNOWN_MODE;
    std::string filename;

    if (arg1 == "-r") {
        mode = Mode::RUN_FROM_FILE;
        if (argc != 3) {
            std::cerr << "Usage: aham -r <file.ahc>\n";
            printError("Argument Error", "Usage: aham -r <file.ahc>", 0, 0, "");
            return 1;
        }
        filename = argv[2];
    } else if (arg1 == "-c") {
        mode = Mode::COMPILE_TO_FILE;
        if (argc != 3) {
            printError("Argument Error", "Usage: aham -c <file.aha>", 0, 0, "");
            return 1;
        }
        filename = argv[2];
    }else if (arg1 == "build")
    {
        mode = Mode::BUILD_PROJECT;

        if (argc == 3)
            filename = argv[2];
    }
    else if (arg1 == "run")
    {
        mode = Mode::RUN_PROJECT;

        if (argc == 3)
            filename = argv[2];
    }

    // these are used to report errors 
    if (mode == Mode::RUN_FROM_FILE) {
        if (!hasExtension(filename, ".ahc")) {
            printError("FileTypeError", "file must have .ahc extension", 0, 0, "");
            return 1;
        }
        
        std::vector<vm::FunctionObject*> compiledFunctions;
        vm::FunctionObject* script = nullptr;

        if (!vm::Deserializer::deserialize(
                filename,
                compiledFunctions,
                script))
        {
            return 1;
        }

        vm::VM vmInstance;
        // VM::run will create a mainModule and assign it to script->module,
        // then register builtins. We need to register compiled functions
        // as globals AFTER that, so we pass them in via a pre-registration step.
        // 
        // But VM::run creates the module internally, so we must instead
        // set up the module here and have VM::run reuse it.
        auto* mainModule = vmInstance.allocate<vm::ModuleObject>();
        mainModule->name = "main";
        mainModule->filePath = filename; 
        script->module = mainModule;

        // Register all compiled functions on the module
        for (auto* fn : compiledFunctions) {
            fn->module = mainModule;
            if (!fn->isMethod) {
               mainModule->globals[fn->name] = fn;
            }
        }

        try {
            vmInstance.run(script, compiledFunctions);
        } catch (const RuntimeError& e) {
    
            printError("RuntimeError", e.message, e.loc.line_num, e.loc.col_num, e.loc.line);
            return 1;
        } catch (const std::exception& e) {
            printError("RuntimeError", e.what(), 0, 0,"");
            return 1;
        }
        return 0;
    }else if (mode == Mode::COMPILE_TO_FILE) {
        if (!hasExtension(filename, ".aha")) {
            printError("FileTypeError", "file must have .aha extension", 0, 0, "");
            return 1;
        }
        DependencyGraph graph;
        try{
            graph.build(filename);
            auto buildOrder = graph.getBuildOrder();
            for (const auto& file : buildOrder)
            {
                auto program = parseFile(file);
                vm::Compiler compiler;

                auto* script = compiler.compile(program.get());
                vm::Serializer::serialize(
                    changeExtension(file, ".ahc"),
                    compiler.compiledFunctions,
                    script
                );
            }
        }
        catch (const CompileError& e) {
            printError("CompileError", e.message, e.loc.line_num, e.loc.col_num, e.loc.line);
            return 1;
        } catch (const RuntimeError& e) {
            printError("RuntimeError", e.message, e.loc.line_num, e.loc.col_num, e.loc.line);
            return 1;
        } catch (const std::exception& e) {
            printError("RuntimeError", e.what(), 0, 0, "");
            return 1;
        }
    }else if(mode == Mode::BUILD_PROJECT){
         DependencyGraph graph;
         ManifestLoader loader;
        try{
            Manifest manifest = loader.load(std::filesystem::current_path());
            graph.build(manifest.entry.string());
            auto buildOrder = graph.getBuildOrder();
            // std::cout << "Source: " << manifest.sourceDir << '\n';
            // std::cout << "Library: " << manifest.libraryDir << '\n';
            // std::cout << "Build: " << manifest.buildDir << '\n';
            // std::cout << "Entry: " << manifest.entry << '\n';
            for (const auto& file : buildOrder){   
                auto program = parseFile(file);
                auto output = getBuildOutputPath(file, manifest);
                vm::Compiler compiler;

                auto* script = compiler.compile(program.get());
                std::filesystem::create_directories(output.parent_path());
                vm::Serializer::serialize(
                    output.string(),
                    compiler.compiledFunctions,
                    script
                );
            }
        }
        catch (const CompileError& e) {
            printError("CompileError", e.message, e.loc.line_num, e.loc.col_num, e.loc.line);
            return 1;
        } catch (const RuntimeError& e) {
            printError("RuntimeError", e.message, e.loc.line_num, e.loc.col_num, e.loc.line);
            return 1;
        } catch (const std::exception& e) {
            printError("RuntimeError", e.what(), 0, 0, "");
            return 1;
        }
    }else if(mode == Mode::RUN_PROJECT){
        try {
                ManifestLoader loader;
                Manifest manifest = loader.load(std::filesystem::current_path());
        
                auto bytecode = getBuildOutputPath(manifest.entry.string(),manifest);
                std::vector<vm::FunctionObject*> compiledFunctions;
                vm::FunctionObject* script = nullptr;
        
                if (!vm::Deserializer::deserialize(
                        bytecode.string(),
                        compiledFunctions,
                        script))
                {
                    return 1;
                }
        
                vm::VM vmInstance;
        
                auto* mainModule = vmInstance.allocate<vm::ModuleObject>();
                mainModule->name = "main";
                mainModule->filePath = bytecode.string();
        
                script->module = mainModule;
        
                for (auto* fn : compiledFunctions)
                {
                    fn->module = mainModule;
        
                    if (!fn->isMethod)
                    {
                        mainModule->globals[fn->name] = fn;
                    }
                }
        
                vmInstance.run(script, compiledFunctions);
            
        }catch(const CompileError& e){
            printError("CompileError", e.message,e.loc.line_num, e.loc.col_num, e.loc.line);;
            return 1;
        }catch(const RuntimeError& e){
            printError("RuntimeError", e.message, e.loc.line_num, e.loc.col_num, e.loc.line);
            return 1;
        }catch(std::exception& e){
            printError("RuntimeError", e.what(), 0, 0, "");
            return 1;
        }
        
    }
    
    else{
         printError("Unknown", "No specific options found . Use -r to run and -c to compile", 0, 0, "");
    }

    return 0;
}



