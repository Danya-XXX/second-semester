
import itertools
CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
def base64(string):
    alf = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
    bytes_string = string.encode('ascii')
    bin_string = ''.join(f"{byte:08b}" for byte in bytes_string)

    while(len(bin_string)%6!=0):
        bin_string += '0'
    encoded = []
    for i in range(0,len(bin_string),6):
        encoded.append(alf[int(bin_string[i:i+6],2)])
    encoded = "".join(encoded)
    while len(encoded) % 4 != 0:
        encoded += '='
    return encoded


def brutforce(en_string):
    for len in range(1,5):
        for x in itertools.product(CHARS, repeat=len):
            password = ''.join(x)
            encoded = base64(password)
            if encoded == en_string:
                return password
    return None

your_password = input("Введите пароль ")
result = brutforce(your_password)
if result:
    print(f"Найден пароль: {result}")
else:
    print("Пароль не найден.")