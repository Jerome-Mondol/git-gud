#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;


const string RESET   = "\033[0m";
const string GREEN   = "\033[32m";
const string RED     = "\033[31m";
const string CYAN    = "\033[36m";
const string BOLD    = "\033[1m";
const string PURPLE  = "\033[35m"; 
const string BLUE    = "\033[34m"; 
const string LIGHT_BLUE = "\033[94m"; 



void printWelcomeBanner() {
    cout << PURPLE << BOLD << R"(
   ______  ____ ______     ______ _    __ ____  _ 
  / ____/ /  _//_  __/    / ____// /   / // __ \| |
 / / __   / /   / /      / / __ / /   / // / / /| |
/ /_/ / _/ /   / /      / /_/ // /___/ // /_/ / |_|
\____/ /___/  /_/       \____//_____//_/_____/  (_)
                                                   
    )" << RESET << endl;

    cout << CYAN << "  ===[ A Command-Line Git Text Adventure ]===" << RESET << "\n\n";
    cout << "  You are trapped inside a completely corrupted repository.\n";
    cout << "  Use your git skills to debug the facility and escape.\n\n";
    cout << "  Type " << GREEN << "git status" << RESET << " to look around. Type " << RED << "exit" << RESET << " to quit.\n";
    cout << "  --------------------------------------------------------\n\n";
}


int main() {
    // force windoes to enable ANSI color codes 
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
    #endif


    string currentBranch = "master";
    string userInput;


    printWelcomeBanner();
    while(true) {
        cout << "(" << RED << currentBranch << RESET << ") $ ";
        getline(cin, userInput);
        
        if(userInput == "exit") {
            cout << GREEN << "Goodbye, developer!\n" << RESET;
            break;
        }
        
        if (userInput == "git status") {
            std::cout << "\nOn branch " << currentBranch << "\n";
            std::cout << "Untracked files exist. Use \"git add\" to stage them.\n";
            std::cout << "  -> flashlight.txt\n\n";
        } else if (!userInput.empty()) {
            std::cout << RED << "git-gud: '" << userInput << "' is not a recognized spell or command.\n" << RESET;
        }

    }
}