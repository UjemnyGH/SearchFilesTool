/**
 * @file search_files.cpp
 * @author Piotr UjemnyGH Plombon (github.com/UjemnyGH/)
 * @brief 
 * @version 0.1
 * @date 2024-05-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

// All found keywords with indexes
std::vector<std::string> gFoundKeywords;
// Should info log stay silent
bool gSilent = false;
// Did user set keuword to search
bool gKeywordSet = false;
// Did user set path to search through
bool gPathSet = false;
// Should we show also directories in info log
bool gShowDirectory = false;

// Our path and keyword
std::string gPath, gKeyword;
// Our excluded extensions from search
std::vector<std::string> gExcludeExtensions;
// Our included extensions in search
std::vector<std::string> gSelectedExtensions;

/**
 * @brief Print text on console but can be disabled on will
 * 
 * @param msg 
 */
void PrintInfo(std::string msg) {
    if(!gSilent) {
        std::cout << msg;
    }
}

/**
 * @brief Check if selected extension (ext) matches desired extensions (gSelectedExtensions) to be included in output
 * 
 * @param ext checked extension
 * @return true 
 * @return false 
 */
bool CheckSelectedExt(std::string ext) {
    if(gSelectedExtensions.size() == 0) {
        return true;
    }

    for(const auto & extension : gSelectedExtensions) {
        if(extension == ext) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Check if selected extension (ext) matches undesired extension (gExcludeExtensions) to be excluded form output
 * 
 * @param ext checked extension
 * @return true 
 * @return false 
 */
bool CheckExcludedExt(std::string ext) {
    if(gExcludeExtensions.size() == 0) {
        return false;
    }

    for(const auto & extension : gExcludeExtensions) {
        if(extension == ext) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Recursivly searches through directories
 * 
 * @param path path to head directory
 */
void SearchFiles(std::string path) {
    for(const auto & entry : std::filesystem::directory_iterator(path)) {
        // If we have directory, go recursive
        if(entry.is_directory()) {
            if(gShowDirectory) {
                PrintInfo(std::string("[INFO]: Found directory:\t") + entry.path().string() + std::string("\n"));
            }

            SearchFiles(entry.path().string());
        }

        // If not print to output, ofc if user doesn`t silenced output
        if(CheckSelectedExt(entry.path().extension().string()) && !CheckExcludedExt(entry.path().extension().string())) {
            PrintInfo(std::string("[INFO]: Found file:\t") + entry.path().string() + std::string("\n"));
        }
    }
}

/**
 * @brief Recursivly searches through directories, but also open files to check is desired keyword is in them
 * 
 * @param path 
 * @param keyword 
 */
void SearchFilesForKeyword(std::string path, std::string keyword) {
    for(const auto & entry : std::filesystem::directory_iterator(path)) {
        // If we have directory, go recursive
        if(entry.is_directory()) {
            if(gShowDirectory) {
                PrintInfo(std::string("[INFO]: Found directory:\t") + entry.path().string() + std::string("\n"));
            }

            SearchFilesForKeyword(entry.path().string(), keyword);
        }
        // If not check file for keyword, could be more keywords in file than 1, but it adds first index
        else {
            if(CheckSelectedExt(entry.path().extension().string()) && !CheckExcludedExt(entry.path().extension().string())) {
                PrintInfo(std::string("[INFO]: Searching through:\t") + entry.path().string() + std::string("\n"));

                std::ifstream f(entry.path().c_str(), std::ios::binary | std::ios::ate);

                uint32_t len = f.tellg();
                f.seekg(0, std::ios::beg);

                std::string src;
                src.resize(len);

                f.read(src.data(), src.size());


                size_t index = src.find(keyword);
                // If there is desired keyword, push it to vector
                if(index != std::string::npos) {
                    gFoundKeywords.push_back(entry.path().string() + std::string(" at index ") + std::to_string(index));
                }

                f.close();
            }
        }
    }
}

/**
 * @brief Simple function for spliting string into vector of strings
 * 
 * @param text 
 * @param strings 
 * @param ch 
 * @return size_t 
 */
size_t SplitString(const std::string &text, std::vector<std::string> &strings, char ch) {
    size_t pos = text.find(ch);
    size_t initial_pos = 0;

    // No npos (-1ull) = good
    while(pos != std::string::npos) {
        strings.push_back(text.substr(initial_pos, pos - initial_pos));
        initial_pos = pos + 1;

        pos = text.find(ch, initial_pos);
    }

    strings.push_back(text.substr(initial_pos, std::min(pos, text.size()) - initial_pos + 1));

    return strings.size();
} 

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: Type %s --help or %s -h for more info!\n", argv[0], argv[0]);
        
        return 0;
    }

        // Check for arguments and based on them do the work
    for(int i = 0; i < argc; i++) {
        std::string arg = std::string(argv[i]);

        if(arg == "-h" || arg == "--help") {
            std::cout << "\n\n" << argv[0] << " usage info:\n"
            "\t-h\t|\t--help\t\t-\tThis prompt that you looking at right now\n"
            "\t-p\t|\t--path\t\t-\tPath to search through\n"
            "\t-k\t|\t--keyword\t-\tKeyword to search through files and find\n"
            "\t-d\t|\t--dir\t\t-\tShow directories among files\n"
            "\t-e\t|\t--exclude\t-\tList of excluded extensions (write all in one parameter separated by space eg. ... -e \".ext .ex2 .etc\" or left blank for no exclusion)\n"
            "\t-i\t|\t--include\t-\tList of included extensions (write all in one parameter separated by space eg. ... -i \".ext .ex2 .etc\" or left blank to search through everything)\n"
            "\t-s\t|\t--silent\t-\tDisable all info about file\n\n"
            "\tThis tool is used to search throughout all files specyfied by path\n\n";

            return 0;
        }
        else if(arg == "-k" || arg == "--keyword") {
            gKeyword = argv[i + 1];
            gKeywordSet = true;
        }
        else if(arg == "-p" || arg == "--path") {
            gPath = argv[i + 1];
            gPathSet = true;
        }
        else if(arg == "-s" || arg == "--silent") {
            gSilent = true;
        }
        else if(arg == "-d" || arg == "--dir") {
            gShowDirectory = true;
        }
        else if(arg == "-e" || arg == "--exclude") {
            SplitString(std::string(argv[i + 1]), gExcludeExtensions, ' ');
        }
        else if(arg == "-i" || arg == "--include") {
            SplitString(std::string(argv[i + 1]), gSelectedExtensions, ' ');
        }
    }

    // Show user current path (don`t know for what purpose, but maybe one day someone thought that it is useful)
    std::cout << "Current path is: " << std::filesystem::current_path().string() << "\n\n";

    // If we have path and keyword, do keyword search
    if(gPathSet && gKeywordSet) {
        SearchFilesForKeyword(gPath, gKeyword);

        // If there is no keyword, inform about it
        if(gFoundKeywords.size() == 0) {
            printf("\nNo keyword \"%s\" occured in files\n", gKeyword.c_str());
        }

        printf("\nKeyword \"%s\" occured in files: \n", gKeyword.c_str());

        // If there are keywords, show where are they
        for(std::string found : gFoundKeywords) {
            printf("\t%s\n", found.c_str());
        }
    }

    // If we onlu have path do normal search
    if(gPathSet && !gKeywordSet) {
        SearchFiles(gPath);
    }

    // Flush output stream to be sure that all data was shown
    fflush(stdout);

    return 0;
}