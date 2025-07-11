import math
# Вычисление количества контрольных битов
def bits_to_bytes(bit_string):
    padding = (8 - len(bit_string) )% 8
    bit_string = bit_string + '0' * padding
    byte_array = bytearray()
    for i in range(0, len(bit_string), 8):
        byte = bit_string[i:i+8]
        byte_array.append(int(byte, 2))
    return byte_array

def bytes_to_bits(byte_data):
    return ''.join(f'{byte:08b}' for byte in byte_data)
def calc_L(k):
    L = 0
    while k > (2 ** L - L - 1):
        L += 1
    return L

# функция декодирования
def decrypted(text, n):
    
    L = calc_L(n)
    
    # разделение строки на блоки
    blocks = [text[i:i+n + L] for i in range(0, len(text), n + L)]
    
    orig = []
    for i in range(len(blocks)):
        # перевод строки в массив 01
        block = [int(c) for c in blocks[i]]
        
        
       
        pos_error = 0
        # поиск ошибки
        for i in range(L):
            control = 2**i
            h = 0
            for j in range(n):
                if (j + 1) & control:
                    h ^= block[j]
            pos_error += h * control
        # исправление ошибки, если в блоке она одна
        if pos_error > 0 and pos_error - 1 < n :
            
            block[pos_error - 1] ^= 1
            
        decr_block = []
        # перевод восстановленного кода хемминга в исходный код
        for x in range(n):
            if math.log2(x+1)%1 != 0:
                decr_block.append(str(block[x]))
        
        orig.extend(decr_block)
    # восстановление файла
    binary_str = ''.join(orig)
    origtext = ''.join(chr(int(binary_str[i:i+8], 2)) for i in range(0, len(binary_str), 8))
    return origtext


# Функция кодирования
def hamming_code(blocks,lenblock):
    bin_enc = []
    k = lenblock
        
    L = calc_L(k)
    print(L+k)
    for b in blocks:
        # добавление нулей, если блок неполный
        block = list(map(int, b.ljust(lenblock, '0')))
        
        k = lenblock
        
        encoded = []
        data_idx = 0
        # добавление в массив информационных и контрольных битов
        for pos in range(0, k + L):
            if (math.log2(pos+1)%1 == 0):  
                encoded.append(0)  
            else:  
                encoded.append(int(block[data_idx]))
                data_idx += 1
        
        # вычисление контрольных битов
        for i in range(L):
            mask = 2**i
            parity = 0
            for j in range(len(encoded)):
                if (j + 1) & mask:
                    parity ^= encoded[j]
            encoded[mask - 1] = parity
        bin_enc.extend(encoded)
        
    # перевод в строку
    bin_enc = ''.join(map(str,bin_enc))
    
    return bin_enc
    


# Перевод текста в массив строк длиной, равной длине блока
def bintext(text, lenblock):
    enc_text =[]
    
    binary_str = ''.join(format(ord(c), '08b') for c in text)
    
    blocks = [binary_str[i:i+lenblock] for i in range(0, len(binary_str), lenblock)]
    
    return blocks
# Сравнение декодированного файла и оригинального
def compare_files(file1, file2):
    
    with open(file1, 'rb') as f1, open(file2, 'rb') as f2:
        return f1.read() == f2.read()    
    


print("Введите длину блока")
lenblock = int(input())

# кодирование файла

with open("random_text.txt", "r") as f: 
    text = f.read()
newtext = hamming_code(bintext(text,lenblock),lenblock)

# для того, чтоб отсеять избыточные символы
lentext = len(text) 

with open("random_text.txt", "wb") as f:
    f.write(bits_to_bytes(newtext))
print("Файл закодирован\n Хотите раскодировать?")
# раскодирование
n = str(input())
if(n):
    with open("random_text.txt", "rb") as f:
        text = f.read()
    bit_string = bytes_to_bits(text)                     
    newtext = decrypted(bit_string, lenblock)


    with open("random_text.txt", "w", encoding="utf-8") as f:   
        f.write(newtext[:lentext])


result = compare_files('random_text.txt', 'random_textorig.txt')
print("Файлы идентичны" if result else "Файл поврежден")






    
