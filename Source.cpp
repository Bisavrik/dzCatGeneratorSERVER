#include <iostream>
#include <string>
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

using namespace std;

bool DownloadCatImage(const string& url, const string& filename) {
    struct WinHttpHandle {
        HINTERNET handle;
        WinHttpHandle(HINTERNET h) : handle(h) {}
        ~WinHttpHandle() { if (handle) WinHttpCloseHandle(handle); }
        operator HINTERNET() const { return handle; }
    };

    WinHttpHandle hSession = WinHttpOpen(L"Cat Downloader/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);
    if (!hSession) {
        return false;
    }

    WinHttpHandle hConnect = WinHttpConnect(hSession, L"cataas.com",
        INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        return false;
    }

    WinHttpHandle hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/cat",
        NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE);
    if (!hRequest) {
        return false;
    }

    if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        return false;
    }

    if (!WinHttpReceiveResponse(hRequest, NULL)) {
        return false;
    }

    FILE* file = nullptr;
    errno_t err = fopen_s(&file, filename.c_str(), "wb");
    if (err != 0 || file == nullptr) {
        return false;
    }

    struct FileCloser {
        FILE* f;
        FileCloser(FILE* file) : f(file) {}
        ~FileCloser() { if (f) fclose(f); }
    } fileCloser(file);

    DWORD bytesRead;
    BYTE buffer[4096];
    while (WinHttpReadData(hRequest, buffer, sizeof(buffer), &bytesRead)) {
        if (bytesRead == 0) break;
        fwrite(buffer, 1, bytesRead, file);
    }

    return true;
}

int main() {
    system("chcp 1251 > nul");

    string url = "https://cataas.com/cat";
    int catNumber = 1;

    cout << "Press Enter to download a cat image (or type 'q' to exit):" << endl;
    cout << "Images will be saved in the current folder as cat1.jpg, cat2.jpg, etc." << endl;

    while (true) {
        string input;
        getline(cin, input);

        if (input == "q") {
            break;
        }

        string filename = "cat" + to_string(catNumber) + ".jpg";

        if (DownloadCatImage(url, filename)) {
            cout << "Cat #" << catNumber << " successfully saved as " << filename << endl;
            catNumber++;
        }
        else {
            cout << "Error downloading cat image" << endl;
        }

        cout << "Press Enter for the next cat (or type 'q' to exit):" << endl;
    }

    cout << "Program terminated" << endl;
    return 0;
}