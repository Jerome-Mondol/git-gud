#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

const string RESET = "\033[0m";
const string GREEN = "\033[32m";
const string RED = "\033[31m";
const string CYAN = "\033[36m";
const string BOLD = "\033[1m";
const string PURPLE = "\033[35m";
const string BLUE = "\033[34m";
const string LIGHT_BLUE = "\033[94m";

void enableANSIInWindows()
{
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode))
        {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif
}

void printWelcomeBanner()
{
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

vector<string> takeInputFromUser(string input)
{
    stringstream ss(input);
    string word;
    vector<string> tokens;
    while (ss >> word)
    {
        tokens.push_back(word);
    }
    return tokens;
}

int main()
{
    // State Tracking
    string currentBranch = "master";
    string userInput;
    
    string masterStatus = "untracked"; 
    
    string basementStatus = "locked";  

    enableANSIInWindows();
    printWelcomeBanner();

    while (true)
    {
        cout << "(" << RED << currentBranch << RESET << ") $ ";
        getline(cin, userInput);

        if (userInput == "exit")
        {
            cout << GREEN << "Goodbye, developer!\n" << RESET;
            break;
        }

        vector<string> words = takeInputFromUser(userInput);
        if (words.empty()) continue;

        if (words[0] != "git")
        {
            cout << RED << "git-gud: Only git commands work in this simulation.\n" << RESET;
            continue;
        }

        string action = (words.size() > 1) ? words[1] : "";

        // ==================== GIT STATUS ====================
        if (action == "status")
        {
            if (currentBranch == "master")
            {
                if (masterStatus == "untracked")
                {
                    cout << "\nOn branch master\nNo commits yet.\n\nUntracked files:\n"
                         << "  (use \"git add <file>...\" to include in what will be committed)\n"
                         << RED << "\tflashlight.txt\n" << RESET << "\n"
                         << "nothing added to commit but untracked files present (use \"git add\" to track)\n\n";
                }
                else if (masterStatus == "staged")
                {
                    cout << "\nOn branch master\nNo commits yet.\n\nChanges to be committed:\n"
                         << "  (use \"git rm --cached <file>...\" to unstage)\n"
                         << GREEN << "\tnew file:   flashlight.txt\n" << RESET << "\n";
                }
                else if (masterStatus == "committed")
                {
                    cout << "\nOn branch master\nnothing to commit, working tree clean\n\n"
                         << "You see a hatch leading down to the " << CYAN << "basement" << RESET << " branch.\n"
                         << "Type " << GREEN << "git checkout basement" << RESET << " to investigate.\n\n";
                }
            }
            else if (currentBranch == "basement")
            {
                if (basementStatus == "untracked")
                {
                    cout << "\nOn branch basement\nYour branch is up to date with 'origin/basement'.\n\n"
                         << "Untracked files:\n"
                         << "  (use \"git add <file>...\" to include in what will be committed)\n"
                         << RED << "\texit_key.enc\n" << RESET << "\n"
                         << "nothing added to commit but untracked files present\n\n";
                }
                else if (basementStatus == "staged")
                {
                    cout << "\nOn branch basement\nChanges to be committed:\n"
                         << GREEN << "\tnew file:   exit_key.enc\n" << RESET << "\n";
                }
            }
        }

        // ==================== GIT ADD ====================
        else if (action == "add")
        {
            if (words.size() < 3)
            {
                cout << RED << "Nothing specified, nothing added.\n" << RESET << "\n";
                continue;
            }

            string targetFile = words[2];

            if (currentBranch == "master")
            {
                if (targetFile == "flashlight.txt" || targetFile == ".")
                {
                    if (masterStatus == "untracked")
                    {
                        masterStatus = "staged";
                        cout << GREEN << "\nStaged changes successfully." << RESET << "\n"
                             << "The file " << CYAN << "flashlight.txt" << RESET << " has been moved to the staging area.\n"
                             << "Type " << GREEN << "git commit -m \"your message\"" << RESET << " to save your progress!\n\n";
                    }
                    else
                    {
                        cout << "File is already staged.\n\n";
                    }
                }
                else
                {
                    cout << RED << "fatal: pathspec '" << targetFile << "' did not match any files.\n" << RESET << "\n";
                }
            }
            else if (currentBranch == "basement")
            {
                if (targetFile == "exit_key.enc" || targetFile == ".")
                {
                    if (basementStatus == "untracked")
                    {
                        basementStatus = "staged";
                        cout << GREEN << "\nStaged changes successfully." << RESET << "\n"
                             << "The file " << CYAN << "exit_key.enc" << RESET << " is staged.\n"
                             << "Commit it using " << GREEN << "git commit -m \"message\"" << RESET << " to compile the main exit code!\n\n";
                    }
                    else
                    {
                        cout << "File is already staged.\n\n";
                    }
                }
                else
                {
                    cout << RED << "fatal: pathspec '" << targetFile << "' did not match any files.\n" << RESET << "\n";
                }
            }
        }

        // ==================== GIT COMMIT ====================
        else if (action == "commit")
        {
            bool hasM = false;
            for (size_t i = 2; i < words.size(); ++i) {
                if (words[i] == "-m") {
                    hasM = true;
                    break;
                }
            }

            if (!hasM)
            {
                cout << RED << "error: switch `-m` is required to commit in this simulator.\n" << RESET;
                cout << "Hint: Try typing: git commit -m \"your message\"\n\n";
                continue;
            }

            if (currentBranch == "master")
            {
                if (masterStatus == "staged")
                {
                    masterStatus = "committed";
                    basementStatus = "untracked";

                    cout << GREEN << "\n[master 4f9a2c1] " << RESET << "Committed changes successfully.\n"
                         << " 1 file changed, 1 insertion(+)\n\n"
                         << BOLD << PURPLE << "=========================================================\n"
                         << "*** CLICK! THE FLASHLIGHT SNAPS ON! ***\n"
                         << "=========================================================\n" << RESET
                         << "The beam cuts through the pitch black darkness, lighting up the room.\n"
                         << "You spot a rusted steel hatch leading down to the " << CYAN << "basement" << RESET << " branch.\n"
                         << "Type " << GREEN << "git checkout basement" << RESET << " to climb down.\n\n";
                }
                else if (masterStatus == "untracked")
                {
                    cout << "On branch master\nnothing to commit, working tree clean (untracked files present)\n\n";
                }
                else
                {
                    cout << "On branch master\nnothing to commit, working tree clean\n\n";
                }
            }
            else if (currentBranch == "basement")
            {
                if (basementStatus == "staged")
                {
                    basementStatus = "escaped";
                    cout << GREEN << "\n[basement e7b2c9f] " << RESET << "Final patch committed.\n"
                         << " 1 file changed, 128 insertions(+)\n\n"
                         << BOLD << GREEN << "=========================================================\n"
                         << "*** ACCESS GRANTED: MAIN DOOR OPENING! ***\n"
                         << "=========================================================\n" << RESET
                         << "The encrypted key parses correctly. The blast doors grind open, exposing\n"
                         << "blindingly bright sunshine outside. You have escaped the broken repo!\n\n"
                         << CYAN << "Congratulations! You won the game! Use 'exit' to close out.\n\n" << RESET;
                }
                else if (basementStatus == "untracked")
                {
                    cout << "On branch basement\nnothing to commit, working tree clean (untracked files present)\n\n";
                }
            }
        }

        // ==================== GIT CHECKOUT ====================
        else if (action == "checkout")
        {
            if (words.size() < 3)
            {
                cout << RED << "error: branch name not specified.\n" << RESET << "\n";
                continue;
            }

            string targetBranch = words[2];

            if (targetBranch == "basement")
            {
                if (basementStatus == "locked")
                {
                    cout << RED << "error: pathspec 'basement' did not match any file(s) known to git.\n" << RESET
                         << "Hint: It's pitch black! You can't safely navigate or find the hatch pathway yet.\n\n";
                }
                else
                {
                    currentBranch = "basement";
                    cout << "Switched to branch '" << CYAN << "basement" << RESET << "'\n"
                         << "You climb down the slippery ladder into the server underground floor.\n"
                         << "The main emergency doors are right here, but they require a compiled master structural key.\n"
                         << "Run " << GREEN << "git status" << RESET << " to see what materials are left behind down here.\n\n";
                }
            }
            else if (targetBranch == "master")
            {
                currentBranch = "master";
                cout << "Switched to branch '" << CYAN << "master" << RESET << "'\n\n";
            }
            else
            {
                cout << RED << "error: pathspec '" << targetBranch << "' did not match any branches.\n" << RESET << "\n";
            }
        }
        
        // ==================== UNKNOWN GIT ACTION ====================
        else
        {
            cout << RED << "git-gud: '" << action << "' is not supported in this training sequence.\n" << RESET << "\n";
        }
    }

    return 0;
}