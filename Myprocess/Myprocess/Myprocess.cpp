// Myprocess.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <cstdlib>

using namespace std;

int getSumOfDigits(int num) {
    int sum = 0;
    num = abs(num);
    while (num > 0) {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "rus");

    int start, end;

    if (argc == 3) {
        start = atoi(argv[1]);
        end = atoi(argv[2]);
    }
    else {
        cout << "Ошибка передачи аргументов";
        return 1;
    }

    int max_sum = -1;
    int target_number = start;

    for (int i = start; i <= end; i++) {
        int current_sum = getSumOfDigits(i);
        cout << "Число: " << i << " -> Сумма цифр: " << current_sum << endl;

        if (current_sum > max_sum) {
            max_sum = current_sum;
            target_number = i;
        }
    }

    cout << "\nЧисло с наибольшей суммой: " << target_number << " (Сумма = " << max_sum << ")" << endl;
    cout << "\nНажмите любую клавишу...";
    _getch();

    return 0;
}

