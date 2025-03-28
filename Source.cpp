#include <iostream>
#include <string>
#include <windows.h> 

using namespace std;

int main() 
{
    string url = "https://cataas.com/cat";

    cout << "Press Enter to open a cat (or 'q' for exit):" << endl;
    cout << "Old tab doesn't close automatically, use F5 in browser for update." << endl;

    bool firstRun = true;

    while (true) 
    {
        string input;
        getline(cin, input);

        if (input == "q")
        {
            break;
        }

        if (firstRun)
        {
            ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
            cout << "Tab open. For new cat press Enter or F5 in browser." << endl;
            firstRun = false;
        }
        else
        {
            ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
            cout << "New cat open. Old tab can stay opened." << endl;
        }

        cout << "Press Enter for new cat (or 'q' for exit):" << endl;
    }

    cout << "Program completed" << endl;
    return 0;
}