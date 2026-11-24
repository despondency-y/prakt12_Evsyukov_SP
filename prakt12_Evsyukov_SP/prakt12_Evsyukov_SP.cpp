// prakt12_Evsyukov_SP.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

struct ProcessInfo {
    int Id;
    wchar_t Name[MAX_PATH];
    string Status;
    HANDLE hProcess;
};

ProcessInfo proc_list[10];
int process_count = 0;

void UpdateProcessStatuses() {
    for (int i = 0; i < process_count; i++) {
        if (proc_list[i].Status == "работает") {
            DWORD exit_code;
            if (GetExitCodeProcess(proc_list[i].hProcess, &exit_code)) {
                if (exit_code != STILL_ACTIVE) {
                    proc_list[i].Status = "завершен";
                }
            }
        }
    }
}

void ShowStatus() {
    UpdateProcessStatuses();
    int running_count = 0;

    cout << "\n\t--- СПИСОК СОЗДАННЫХ ПРОЦЕССОВ ---" << endl;
    for (int i = 0; i < process_count; i++) {
        cout << "ID: " << proc_list[i].Id
            << " \tПрограмма: " << proc_list[i].Name
            << "\tСостояние: " << proc_list[i].Status << endl;
        if (proc_list[i].Status == "работает") running_count++;
    }
    cout << "---------------" << endl;
    cout << "Работающих процессов: " << running_count << "\n" << endl;
}

void StartNewProcess(const wchar_t* Appname, const wchar_t* args = NULL) {
    if (process_count >= 10) {
        cout << "Ошибка: лимит процессов (максимум 10)" << endl;
        return;
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    wchar_t cmdLine[512];

    if (args != NULL) {
        swprintf_s(cmdLine, L"%s %s", Appname, args);
    }
    else {
        wcscpy_s(cmdLine, Appname);
    }

    if (!CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        cout << "Ошибка создания! Код: " << GetLastError() << endl;
        return;
    }

    cout << "Процесс успешно создан" << endl;

    proc_list[process_count].Id = process_count + 1;
    wcscpy_s(proc_list[process_count].Name, Appname);
    proc_list[process_count].Status = "работает";
    proc_list[process_count].hProcess = pi.hProcess;

    process_count++; 

    CloseHandle(pi.hThread);


    // 11 слайд
}

void KillProcess(int manager_id) {
    if (manager_id < 1 || manager_id > process_count) {
        cout << "Неверный ID процесса" << endl;
        return;
    }

    ProcessInfo& p = proc_list[manager_id - 1];
    if (p.Status == "завершен") {
        cout << "Этот процесс уже закрыт" << endl;
        return;
    }

    if (TerminateProcess(p.hProcess, 0)) {
        p.Status = "завершен";
        cout << "Процесс успешно завершился" << endl;
    }
    else {
        cout << "Не удалось завершить. Ошибка: " << GetLastError() << endl;
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    char choice;

    do {
        cout << "\n\t--- МЕНЕДЖЕР ПРОЦЕССОВ ---" << endl;
        cout << "1. Запустить Диспетчер задач" << endl;
        cout << "2. Запустить Пинг" << endl;
        cout << "3. Запустить Myprocess" << endl;
        cout << "4. Показать статус процессов" << endl;
        cout << "5. Завершить процесс по ID" << endl;
        cout << "6. Выход" << endl;
        cout << "Выберите пункт: ";

        cin >> choice;

        if (choice == '1') {
            StartNewProcess(L"C:\\Windows\\System32\\Taskmgr.exe");
        }
        else if (choice == '2') {
            StartNewProcess(L"C:\\Windows\\System32\\ping.exe", L"127.0.0.1");
        }
        else if (choice == '3') {
            int start, end;
            cout << "Введите начало диапазона: "; cin >> start;
            cout << "Введите конец диапазона: "; cin >> end;

            wchar_t args[100];
            swprintf_s(args, L"%d %d", start, end);

            StartNewProcess(L"C:\\Users\\Admin\\Desktop\\Myprocess\\x64\\Debug\\Myprocess.exe", args);
        }
        else if (choice == '4') {
            ShowStatus();
        }
        else if (choice == '5') {
            int id;
            cout << "Введите ID для закрытия: ";
            cin >> id;
            KillProcess(id);
        }
        else if (choice == '6') {
            cout << "Выход";
        }
        else {
            cout << "Ошибка: неверный пункт меню.";
        }

    } while (choice != '6');

    for (int i = 0; i < process_count; i++) {
        CloseHandle(proc_list[i].hProcess);
    }
    

}


