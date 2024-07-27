/*

                                        _     _
                                       | |   | |
                  _ __   ___  _   _  __| | __| |_   _
                 | '_ \ / _ \| | | |/ _` |/ _` | | | |
                 | | | | (_) | |_| | (_| | (_| | |_| |
                 |_| |_|\___/ \__,_|\__,_|\__,_|\__, |
                                                 __/ |
                                                |___/


                 Date : 27.7.2024
                 Author : nouddy
                 Github : github.com/nouddy

                 Info : S1mple cheat kojem daje igracu brzinu (putem game time / brzina odvijanja igre oko igraca)

                Setup : Sve sto je potrebno jeste da se compile .cpp fajl (dok ste povezani na server) i to je to, u 90% slucajeva anti-cheat na serveru nece primjetiti da ste brzi on foot
                        jer im je anti-cheat (bar na balkanu) za jedan veliki pimpek.

                Credits : https://gtamods.com/wiki/Memory_Addresses_(SA)
*/

//      >> [ LIBRARY'S ] <<

#include <iostream>
#include <chrono>
#include <windows.h>
#include <thread>


using namespace std;

//      >> [ CONSTANTS ] <<

const DWORD TIME_SPEED_ADRESS = 0xB7CB64; // Memorijska adresa za brzinu vremena (Game Time)



void setSpeed(HANDLE process) {

    //Funkcionise tako da bi preko 0xB7CB64 adrese trebalo podesiti igracu vrijeme (Brzina odvijanja vremena / Game Time Speed)

    SIZE_T bytesWritten, bytesRead;

    while (true) {

        float currentSpeed;
        if (!ReadProcessMemory(process, reinterpret_cast<LPCVOID>(TIME_SPEED_ADRESS), &currentSpeed, sizeof(currentSpeed), &bytesRead)) {

            cerr << "Nije moguce ucitati trenutacnu brzinu... Error Code ) " << GetLastError() << endl;
            break;
        }

        cout << "Current time speed : " << currentSpeed << endl;

        float newSpeed = 150.00f;
        float totalSpeed = currentSpeed + newSpeed;

        if (!WriteProcessMemory(process, reinterpret_cast<LPVOID>(TIME_SPEED_ADRESS), &totalSpeed, sizeof totalSpeed, &bytesWritten)) {

            cerr << "Nije moguc upis u memorisjku adresu za igracevu brzinu... Error Code ) " << GetLastError() << endl;
            break;
        }

        if (bytesWritten != sizeof totalSpeed) {

            cerr << "Greska pri upisivanju vrijednosti : " << GetLastError() << endl;
        }

        cout << "Igraceva brzina (Brzina vremena) : " << totalSpeed << endl;
    }
}


int main() {

    HWND hwnd_app = FindWindow(NULL, L"GTA:SA:MP");
    if (hwnd_app == NULL) {
        cerr << "GTA:SA:MP nije aktivan (morate biti konektovani na server)" << endl;
        return 1;
    }

    DWORD pid;
    GetWindowThreadProcessId(hwnd_app, &pid);


    HANDLE process = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
    if (process == NULL) {
        cerr << "Failed to open process! Error code: " << GetLastError() << endl;
        return 1;
    }

    
    setSpeed(process);

    CloseHandle(process);

    return 0;
}
