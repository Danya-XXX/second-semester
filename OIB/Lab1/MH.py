import random
from math import gcd
# Расширенный алгоритм евклида
def modgsd(a, m):
    x0, x1, m0 = 0, 1, m
    while a > 1:
        q = a // m
        a, m = m, a % m
        x0, x1 = x1 - q * x0, x0
    if a != 1:
        raise ValueError("Обратного элемента не существует")
    return x1 % m0
# Генерация супервозрастающей последовательности
def gsp(n):
    w = []
    max = 0
    for _ in range(n):
        next_val = max + random.randint(1, 10)
        w.append(next_val)
        max += next_val
    return w
# Генерация ключей
def generate_keys(numb=8):

    w = gsp(numb) #Геренация последовательности
    m = sum(w) + random.randint(1,100) #модуль преобразования
    # Поиск числа, взаимопростого с m
    while True:
        n = random.randint(2, m - 1)
        if gcd(n, m) == 1:
            break
    # Генерация публичного ключа
    b = [(n * wi) % m for wi in w]
    return {
        'private': {'w': w, 'm': m, 'n': n},
        'public': b
    }
def encrypt(message, public_key):
    bits = []
    for char in message:
        bin_str = format(ord(char), '08b')
        bits_char = []
        bits_char.extend([int(bit) for bit in bin_str])
        bits.append(bits_char)
    
    # Шифрование каждого набора битов
    enctext = []
    for bit in bits:
        c = sum([bit * b for bit, b in zip(bit, public_key)])
        enctext.append(c)
    return enctext

def decrypt(enctext, private_key):
    w, m, n = private_key['w'], private_key['m'], private_key['n']
    n_inv = modgsd(n, m) # Обратный элемент для n по модулю m
    text_bits = []
    #задача ранца
    for c in enctext:
        c_prime = (c * n_inv) % m
        bits = []
         # Жадный алгоритм для супервозрастающей последовательности
        for wi in reversed(w):
            if wi <= c_prime:
                bits.insert(0, 1)
                c_prime -= wi
            else:
                bits.insert(0, 0)
        text_bits.extend(bits)
    # Преобразование битовой последовательности обратно в текст
    chars = []
    for i in range(0, len(text_bits), 8):
        byte = text_bits[i:i+8]
        value = int(''.join(map(str, byte)), 2)
        chars.append(chr(value))
    return ''.join(chars)

# --- Главная программа с вводом ---
if __name__ == "__main__":
   
    message = input("Введите сообщение для шифрования: ")

    keys = generate_keys()
    public_key = keys['public']
    private_key = keys['private']

  
    print("Публичный ключ:", public_key)
    print("Приватный ключ:", private_key)

    cipher = encrypt(message, public_key)
    print("\nЗашифрованное сообщение:", cipher)

    decrypted = decrypt(cipher, private_key)
    print("\nРасшифрованное сообщение:", decrypted)