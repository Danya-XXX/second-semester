#!/usr/bin/env python3
import re
import sys
from getpass import getpass
def find_errors(password):
    errors = []
    if len(password) < 10:
        errors.append("Недостаточная длинна пароля")
    if not re.search(r'[A-Z]', password):
        errors.append("Должна быть хотя бы одна заглавная буква")
    if not re.search(r'[a-z]', password):
        errors.append("Должна быть хотя бы одна строчная буква")
    if not re.search(r'[0-9]', password):
        errors.append("Должна быть хотя бы одна цифра")
    if not re.search(r'[^A-Za-z0-9]', password):
        errors.append("Должен быть хотя бы один спецсимвол")
    return errors    


def main():

    max_try = 5
    Try = 0
    while Try < max_try:

        password = getpass("Введите пароль: ")
        re_password = getpass("Подтвердите пароль: ")

        if password != re_password:
            print("Пароли не совпадают")
            Try += 1
            continue
        errors = find_errors(password)
        if not errors:
            print("Пароль сохранен")
            sys.exit(0)
        print("\nОшибка в пароле: ")
        for error in errors:
            print(f"{error}  ")
        
        Try += 1
    print("Превышено максимальное количество попыток")
    sys.exit(1)
if __name__ == "__main__":
    main()

