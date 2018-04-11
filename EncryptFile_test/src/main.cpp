#include <conio.h>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int main(int /*argc*/, char** /*argv*/){
    MessageBox(0, _T("EncryptFile()"), _T("DBG"), 0);
    //_getch();
    int r = EncryptFile(_T("e:\\fileEncrypted.txt"));
    if(r){
        printf("EncryptFile() = %d\n", r);
    }else{
        printf("EncryptFile() = %d GetLastError()=%d\n", r, GetLastError());
    }
    printf("Press any key...");
    _getch();
    return 0;
}
