#Лаба 1

import random

from datetime import datetime
from sympy import randprime

def caesar_cipher(text, shift, alphabet):
    shifted_alphabet = alphabet[shift:] + alphabet[:shift]
    table = str.maketrans(alphabet, shifted_alphabet)
    return text.translate(table)

def gcd(a, b):
    while b:
        a, b = b, a % b
    return a


def miller_rabin(n, k=5):

    t = n - 1
    s = 1
   
   
    for m in range(k) :
        a = random.randint(1,n-1)
        
        x = pow(a, t, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(s - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

def modgsd(a, m):
    x0, x1, m0 = 0, 1, m
    while a > 1:
        q = a // m
        a, m = m, a % m
        x0, x1 = x1 - q * x0, x0
    if a != 1:
        raise ValueError("Обратного элемента не существует")
    return x1 % m0


def generate_rsa_keys():
    p = randprime(2**20, 2**22)
    q = randprime(2**20, 2**22)
    n = p * q
    phi = (p - 1) * (q - 1)
    
    e = 65537
    while gcd(e, phi) != 1:
        e = random.randint(2, phi - 1)
    
    d = modgsd(e, phi)
    return e, d, n

def task1(N_g,N_n,letter):
    alphabet = 'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'
    Phi3 = alphabet.index(letter.lower()) + 1
    print(Phi3)
    result = ((N_g + N_n)**11 + Phi3) % 11
    print(f"1.Результат:{result}")

def task2(full_name, k):
    alphabet = 'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'
    
    encrypted = caesar_cipher(full_name.lower(), k, alphabet)
    print(f"Зашифрованное ФИО: {encrypted}")


def task3(N_g, N_n, birth_date):

    A = (N_g * (8 + (N_n % 7))) ** 2

    B = int(birth_date.replace('.', ''))


    gcd1 = gcd(A, (B % 95) + 900)
    gcd2 = gcd(A, ((B + 50) % 97) + 700)
    gcd3 = gcd(A, ((B + 20) % 101) + 1500)
    gcd4 = gcd(A, ((B - 40) % 103) + 2500)
    print(B, ((B - 40) % 103))
    print(f"3. A = {A}, B = {B}")
    print(f"    НОД1 = {gcd1}, НОД2 = {gcd2}, НОД3 = {gcd3}, НОД4 = {gcd4}")


def task4():
 
    N = 16809  
    is_prime_N = miller_rabin(N)

   
    N1 = 101  
    is_prime_N1 = miller_rabin(N1)

    print(f"4. Число {N} {'составное' if not is_prime_N else 'простое'}")
    print(f"   Число {N1} {'составное' if not is_prime_N1 else 'простое'}")
def task5():
    p = 49081
    q = 86453
    n = p * q
    phi_n = (p - 1) * (q - 1)

    
    e = 5  
    while gcd(e, phi_n) != 1:
        e += 1

   
    d = modgsd(e, phi_n)

    print(f"5. RSA параметры:")
    print(f"   p = {p}, q = {q}, n = {n}, φ(n) = {phi_n}")
    print(f"   e = {e}, d = {d}")

def task6():
    e, d, n = generate_rsa_keys()
    x = "я в порядке"
    
   
    encrypted = [pow(ord(char), e, n) for char in x]
 
    decrypted = ''.join([chr(pow(num, d, n)) for num in encrypted])
    
    print(f"6. Оригинальный текст: {x}")
    print(f"    Шифрование: {encrypted}")
    print(f"    Дешифрование: {decrypted}")

def task7():
    e, d, n = generate_rsa_keys()
    x = "выйди погуляй"
    
    s = [pow(ord(char), d, n) for char in x]
    verified = ''.join([chr(pow(num, e, n)) for num in s])
    print(f"7. Текст: {x} \n   Подпись:{s}\n   Проверка: {verified}" )
   
def task8():
    
    p = randprime(2**20, 2**22)
    q = randprime(2**20, 2**22)
    n = p * q
    phi = (p - 1) * (q - 1)
    a = random.randint(2, 2**63)
    
  
    x = random.randint(2, phi - 1)
    A = pow(a, x, n)
    
   
    y = random.randint(2, phi - 1)
    B = pow(a, y, n)
    
   
    key_A = pow(B, x, n)
    key_B = pow(A, y, n)
    
   
    print(f"А приватный: {x}, публичный: {A}")
    print(f"В приватный: {y}, публичный: {B}")
    print(f"Сеансовые ключи: {key_A} (A), {key_B} (B)")
    print("Ключи одинаковые" if key_A == key_B else "неодинаковые")
    ax_y = pow(pow(a, x, n), y, n)
    ay_x = pow(pow(a, y, n), x, n)
    axy = pow(a, x * y, n)

    if ax_y == ay_x == axy:
        print("Равенство верно: (a^x)^y ≡ (a^y)^x ≡ a^(xy) mod n")


N_g = 1      # номер группы
N_n = 10       # порядковый номер в списке
last_name = "Кириллов"  # фамилия
full_name = "Кириллов Даниил Андреевич"  # ФИО
birth_date = "16.12.2005"  # дата рождения
third_letter = last_name[2]  # третья буква фамилии
task1(N_g, N_n, third_letter)
k = 10  # сдвиг
task2(full_name,k)
task3(N_g, N_n, birth_date)
task4()
task5()
task6()
task7()
task8()