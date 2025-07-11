# Lab2
import re
# Функция, анализирующая шифрограмму и возвращающая списки отсортированных частот каждых букв в шифрограмме и русском алфавите в виде (буква, частота)
def analises(my_cryptogram):

    rus_letter_freq = {
    'а': 0.0801, 'б': 0.0159, 'в': 0.0454, 'г': 0.0170, 'д': 0.0298,
    'е': 0.0845, 'ё': 0.0003, 'ж': 0.0094, 'з': 0.0165, 'и': 0.0735,
    'й': 0.0121, 'к': 0.0349, 'л': 0.0440, 'м': 0.0321, 'н': 0.0670,
    'о': 0.1097, 'п': 0.0281, 'р': 0.0473, 'с': 0.0547, 'т': 0.0626,
    'у': 0.0262, 'ф': 0.0026, 'х': 0.0097, 'ц': 0.0048, 'ч': 0.0144,
    'ш': 0.0073, 'щ': 0.0036, 'ъ': 0.0004, 'ы': 0.0190, 'ь': 0.0174,
    'э': 0.0032, 'ю': 0.0064, 'я': 0.0201
    }

    sorted_freq = sorted(rus_letter_freq.items(), key = lambda x: -x[1])
    
    # Генератор, который очищает текст от небуквенных символов
    clean_my_cryptogram = ''.join([char for char in my_cryptogram if char.isalpha()])
    
    cryptogram_letter_freq = {}
    len_cryptogram = len(clean_my_cryptogram)
    # цикл, добавляющий в список букву и ее количество в шифрограмме
    for letter in clean_my_cryptogram:
        if letter in cryptogram_letter_freq:
            cryptogram_letter_freq[letter] += 1
        else:
            cryptogram_letter_freq[letter] = 1

    for letter in cryptogram_letter_freq:
        cryptogram_letter_freq[letter] = round(cryptogram_letter_freq[letter] / len_cryptogram, 4)

    sorted_my_freq = sorted(cryptogram_letter_freq.items(), key = lambda x: -x[1])
    
    return sorted_my_freq, sorted_freq

# Функция, выводящая на экран слова, отсортированные по количеству букв
def print_group(my_cryptogram):
    # Разделение текста на слова и удаление небуквенных символов
    
    words = my_cryptogram.split()
    delited_waste = []
    for word in words:
        del_waste = ''.join([char for char in word if char.isalpha()])
        delited_waste.append(del_waste)


    len_matrix = max(len(word) for word in delited_waste)
    matrix = [[] for _ in range(len_matrix+1)] 
    # Заполняем двумерный список словами и после выводим его
    for word in delited_waste:
        matrix[len(word)].append(word)
    for i in range(1,len_matrix+1):
        if matrix[i]:
            print(f"слова с {i} букв", matrix[i])


# Функция, которая выводит на экран слова, отсортированные по количеству нерасшифрованных букв
def print_decrypted(my_cryptogram, decr_letters):

    # Разделение текста на слова и удаление небуквенных символов
    words = my_cryptogram.split()
    delited_waste = []
    for word in words:
        del_waste = ''.join([char for char in word if char.isalpha()])
        delited_waste.append(del_waste)
    
    # Длелаем множество из букв, которые нерасшифрованы
   
    letters = [char for char in my_cryptogram if char.isalpha()]
    decr_letters = list(decr_letters)
    
    
    letters = [char for char in letters if char not in decr_letters]
 
    
 
    # Создаем двумерный массив и заполняем его словами по порядку нерасшифрованных букв и выводим его
    len_letters = max(len(x) for x in delited_waste)
    matrix = [[] for _ in range(len_letters+1)] 
    for word in delited_waste:
        cnt = 0
        for char in word:
            if char in letters:
                cnt +=1
        matrix[cnt].append(word)
    for i in range(len_letters,0, -1):
        if matrix[i]:
            print(f"нерасшифрованно {i} букв: ", matrix[i])

def caesar_cipher(text, shift):
    text = text.lower()
    alphabet = 'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'
    shifted_alphabet = alphabet[shift:] + alphabet[:shift]
    table = str.maketrans(alphabet, shifted_alphabet)
    return text.translate(table)
def is_russian_letter(c):
    return c in 'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'
def clean_text(text):
    return re.sub(r'[^а-яё\s]', '', text.lower())
def analyze_decryption(original_text, decrypted_text):
    """
    Анализирует качество автоматической расшифровки, сравнивая с оригинальным текстом.
    Возвращает статистику по правильно расшифрованным буквам и процент совпадений.
    """
    cleaned_original = clean_text(original_text)
    cleaned_decrypted = clean_text(decrypted_text)


    # Фильтруем только буквы для сравнения
    original_letters = [c for c in cleaned_original.lower() if is_russian_letter(c)]
    decrypted_letters = [c for c in cleaned_decrypted.lower() if is_russian_letter(c)]
    
 
    
    
    correct_count = 0
    correct_letters = set()
    incorrect_letters = set()
    
    # Сравниваем буквы одну за другой
    for orig, decr in zip(original_letters, decrypted_letters):
        if orig == decr:
            correct_count += 1
            correct_letters.add(orig)
        else:
            incorrect_letters.add((orig, decr))
    
    total_letters = len(original_letters)
    accuracy = (correct_count / total_letters) * 100 if total_letters > 0 else 0
    
    return {
        'total_letters': total_letters,
        'correct_count': correct_count,
        'accuracy': accuracy,
        'correct_letters': sorted(correct_letters),
        'incorrect_letters': sorted(incorrect_letters)
    }

def main():
    
    
    with open("xxx.txt", 'r', encoding='utf-8') as f:
        original = f.read()
        original = clean_text(original)
       
       
    ceasar = caesar_cipher(original,7)
    
    newtext = ceasar    
    history = []
    substitutions = {}

    while(True):
        
        print("Выберите действие:\n\
            1.Анализ частоты и вывод предполагаемых замен.\n\
            2.Вывод всех слов, сгруппирированных по количеству букв.\n\
            3.Вывод всех слов, сгруппирированных по количестку нерасшифрованных букв.\n\
            4.Замена букв в криптограмме\n\
            5.Откат истории.\n\
            6.Автоматическая замена букв.\n\
            7.Вывод текущего текста и замен.\n\
            0.Выход из программы")
        action = int(input())
        if action not in [1,2,3,4,5,6,7,0]:
            print("Нет такого действия")
        match action:
        
            case 1:
                
                my_freq, rus_lett_freq = analises(ceasar)
                
                min_len = min(len(my_freq),len(rus_lett_freq))
                
                print("Предполагаемые замены:\n")
            
                for i in range(min_len):
                    print(f"{my_freq[i][0]} -> {rus_lett_freq[i][0]}  {my_freq[i][1]}  {rus_lett_freq[i][1]}")
                print(f"Необработанные буквы {rus_lett_freq[len(my_freq):]}")

            case 2:
                print_group(newtext)
            case 3:
                decrypted_letters = substitutions.values()
                print_decrypted(newtext, decrypted_letters)
            case 4:
                
                letter1, letter2 = input(f"Заменить букву X на Y (введите через пробел): ").split()
                alphabet = ['а', 'б', 'в', 'г', 'д', 'е', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'ъ', 'ы', 'ь', 'э', 'ю', 'я','ё']
                if (letter1 not in alphabet or letter2 not in alphabet):
                    print("Нет такой буквы в алфавите")
                    continue
                substitutions[letter1] = letter2.upper()
                history.append((letter1, letter2.upper()))
                
                
                newtext = ''.join([substitutions.get(char, char) for char in ceasar])
                
                
                
            case 5:
                n = len(history)

                steps = int(input(f"Введите на сколько шагов вернуть назад (вы можете вернуться не более, чем на {n} шагов): "))
                if steps > n:
                    print("Нельзя вернуться на столько шагов")
                    continue
                for _ in range(steps):
                    old_char, _ = history.pop()
                    del substitutions[old_char]
                
                
                temp_text = ceasar
                for enc, dec in substitutions.items():
                    temp_text = temp_text.replace(enc, dec)
                ceasar = temp_text
            case 6:
                    
                
                my_freq, rus_freq = analises(ceasar)
                substitutions = {}
                
                for i in range(len(rus_freq)):
                    substitutions[my_freq[i][0]] = rus_freq[i][0]
                    history.append((my_freq[i][0], rus_freq[i][0]))
                
                newtext = ''.join([substitutions.get(char, char) for char in ceasar])
              
                
                
                # Анализ качества расшифровки
                analysis = analyze_decryption(original.lower(), newtext.lower())
                
                if analysis:
                    print("\nАнализ автоматической расшифровки:")
                    print(f"Всего букв: {analysis['total_letters']}")
                    print(f"Правильно расшифровано: {analysis['correct_count']}")
                    print(f"Точность: {analysis['accuracy']:.2f}%")
                    
                    print("\nПравильно расшифрованные буквы:")
                    print(', '.join(analysis['correct_letters']))
                    
                    print("\nНеправильные замены (оригинал -> ошибочная замена):")
                    for orig, decr in analysis['incorrect_letters']:
                        print(f"{orig} -> {decr}")
            
            case 7:

                print(f"Текущий текст:\n\n{newtext}")
                print("Текущие замены:\n\n")
                for old, new in substitutions.items():
                    print(old,"->",new)
            case 0:
                
                break

main()