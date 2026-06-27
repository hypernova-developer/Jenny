#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <ctime>

namespace fs = std::filesystem;

struct Config 
{
    std::string gcc;
    std::string gxx;
    std::string gfortran;
    std::string csc;
    std::string python;
    std::string syntax_inc;
    std::string raylib_inc;
    std::string raylib_lib;
    std::string quarantine_dir;
    std::string ollama_model;
};

Config loadConfig()
{
    Config cfg;
    std::ifstream file("jenny.conf");
    if (file.is_open())
    {
        std::getline(file, cfg.gcc);
        std::getline(file, cfg.gxx);
        std::getline(file, cfg.gfortran);
        std::getline(file, cfg.csc);
        std::getline(file, cfg.python);
        std::getline(file, cfg.syntax_inc);
        std::getline(file, cfg.raylib_inc);
        std::getline(file, cfg.raylib_lib);
        std::getline(file, cfg.quarantine_dir);
        std::getline(file, cfg.ollama_model);
        file.close();
    }
    return cfg;
}

void log_operation(const std::string& message) 
{
    std::ofstream log_file("compilex_history.log", std::ios::app);
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char buf[26];
    ctime_s(buf, sizeof(buf), &now);
    std::string ts(buf);
    ts.pop_back();
    log_file << "[" << ts << "] " << message << std::endl;
}

int main(int argc, char* argv[]) 
{
    Config cfg = loadConfig();
    
    if (argc < 2) 
    {
        std::cout << "[CompileX] Error: No input file." << std::endl;
        return 1;
    }

    std::string arg1 = argv[1];

    if (arg1 == "--version") 
    {
        std::cout << "---------------------------------------------------" << std::endl;
        std::cout << "[CompileX Engine]" << std::endl;
        std::cout << "Version: 3.0.1-LTS \"Character-Safe Edition\"" << std::endl;
        std::cout << "Developer: hypernova-developer" << std::endl;
        std::cout << "Focus: C++, C, Fortran, C#, Python" << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
        return 0;
    }

    fs::path source_path = fs::path(reinterpret_cast<const char8_t*>(arg1.c_str()));
    
    if (!fs::exists(source_path)) 
    {
        std::cout << "[CompileX] Error: Source file not found." << std::endl;
        return 1;
    }

    std::string ext = source_path.extension().string();
    std::string exe_name = source_path.stem().string() + ".exe";
    std::string cmd = "";
    std::string lang = "";

    if (ext == ".cpp") 
    {
        lang = "C++";
        cmd = "\"\"" + cfg.gxx + "\" -std=c++23 -O3 -s -static -static-libgcc -static-libstdc++ -I\"" + cfg.syntax_inc + "\" -I\"" + cfg.raylib_inc + "\" \"" + arg1 + "\" -o \"" + exe_name + "\" -L\"" + cfg.raylib_lib + "\" -lraylib -lopengl32 -lgdi32 -lwinmm\"";
    } 
    else if (ext == ".c") 
    {
        lang = "C";
        cmd = "\"\"" + cfg.gcc + "\" -O3 -s -static -static-libgcc -I\"" + cfg.syntax_inc + "\" \"" + arg1 + "\" -o \"" + exe_name + "\"\"";
    } 
    else if (ext == ".f90") 
    {
        lang = "Fortran";
        cmd = "\"\"" + cfg.gfortran + "\" -O3 -static \"" + arg1 + "\" -o \"" + exe_name + "\"\"";
    } 
    else if (ext == ".cs") 
    {
        lang = "C#";
        cmd = "\"\"" + cfg.csc + "\" /nologo /optimize+ /out:\"" + exe_name + "\" \"" + arg1 + "\"\"";
    } 
    else if (ext == ".py") 
    {
        std::cout << "[CompileX] Executing " << arg1 << " (Python)..." << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
        int py_res = system(std::string("\"\"" + cfg.python + "\" \"" + arg1 + "\"\"").c_str());
        std::cout << "---------------------------------------------------" << std::endl;
        log_operation("Executed " + arg1 + " (Python)");
        return py_res;
    }

    std::cout << "[CompileX] Compiling " << arg1 << " (" << lang << ")..." << std::endl;
    int res = system(cmd.c_str());

    if (res == 0) 
    {
        std::cout << "---------------------------------------------------" << std::endl;
        std::cout << "[CompileX] Success: Build complete." << std::endl;
        
        std::ofstream built_list(".compilex_built", std::ios::app);
        built_list << exe_name << "\n";
        
        log_operation("Compiled " + exe_name + " (" + lang + ")");
        
        std::cout << "[CompileX] Executing binary..." << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
        system(std::string("\"" + exe_name + "\"").c_str());
    } 
    else 
    {
        std::cout << "---------------------------------------------------" << std::endl;
        std::cout << "[CompileX] Error: Compilation failed." << std::endl;
        log_operation("Failed to compile " + arg1 + " (" + lang + ")");
        return 1;
    }

    return 0;
}
