import random
import re
import os
import json

CONFIG_PATH = 'config.json'

def load_config(path=CONFIG_PATH):
    with open(path, 'r') as f:
        return json.load(f)

def obfuscate(input_file, output_file, config):
    def load_names_from_file(filename='names.txt'):
        if not os.path.exists(filename):
            default_names = ['alpha', 'beta', 'gamma', 'delta', 'omega', 'zulu']
            with open(filename, 'w') as f:
                f.write('\n'.join(default_names))
            return default_names
        with open(filename, 'r') as f:
            return [line.strip() for line in f if line.strip()]

    def remove_comments(code):
        code = re.sub(r'//.*', '', code)
        code = re.sub(r'/\*.*?\*/', '', code, flags=re.DOTALL)
        return code

    def remove_spaces_tabs(code):
        if config.get("remove_tabs", False):
            code = code.replace('\t', '')
        if config.get("remove_spaces", False):
            code = re.sub(r' {2,}', ' ', code)
        return code

    def remove_newlines(code):
        code = re.sub(r'\n{3,}', '\n\n', code)
        code = re.sub(r'\n+\s*}', '}', code)
        code = re.sub(r'{\n+', '{', code)
        code = re.sub(r'\n\s*\n', '\n', code)
        return code

    def collapse_to_one_line(code):
        code = re.sub(r'\s*\n\s*', ' ', code)
        code = re.sub(r'\s*([{};])\s*', r'\1 ', code)
        code = re.sub(r'\s+', ' ', code)
        return code.strip()

    def clean_code(code):
        lines = code.splitlines()
        preprocessor_lines = [line for line in lines if line.strip().startswith('#')]
        other_lines = [line for line in lines if not line.strip().startswith('#')]
        other_code = '\n'.join(other_lines)

        if config.get("remove_comments", False):
            other_code = remove_comments(other_code)
        other_code = remove_spaces_tabs(other_code)
        if config.get("remove_newlines", False):
            other_code = remove_newlines(other_code)
        if config.get("collapse_all_to_one_line", False):
            other_code = collapse_to_one_line(other_code)

        return '\n'.join(preprocessor_lines) + '\n' + other_code.strip()

    def generate_random_name():
        return f"{random.choice(name_pool)}_{random.randint(1000, 9999)}"

    def generate_trash_function():
        ret_type = random.choice(ALL_TYPES)
        param_type = random.choice(ALL_TYPES)
        param_name = generate_random_name()
        var_name = generate_random_name()

        # Значение по типу, char — только безопасные значения
        if param_type == 'char':
            var_value = random.randint(0, 127)
        else:
            var_value = random.randint(0, 1000)

        return f"""{ret_type} {generate_random_name()}({param_type} {param_name}) {{
    {param_type} {var_name} = {var_value};
    if ({random.randint(0,100)} > {random.randint(0,100)}) {{
        return {random.randint(0, 100)};
    }}
    return {param_name} + {random.randint(1, 100)};
}}"""

    def insert_trash_functions(code):
        last_include_pos = 0
        for match in re.finditer(r'#(include|define)\b', code):
            last_include_pos = match.end()
        if last_include_pos > 0:
            end_pos = code.find('\n', last_include_pos)
            if end_pos == -1: end_pos = len(code)
            code = code[:end_pos] + "\n" + generate_trash_function() + code[end_pos:]

        main_pos = code.find('int main(')
        if main_pos != -1:
            code = code[:main_pos] + generate_trash_function() + "\n" + code[main_pos:]
        return code

    code = input_file.read()
    name_pool = load_names_from_file()
    ALL_TYPES = ['int', 'float', 'double', 'long int', 'long long int', 'char']

    if config.get("add_trash_functions", False):
        code = insert_trash_functions(code)

    main_match = re.search(r'int main\s*\([^)]*\)\s*\{', code)
    if main_match and (config.get("add_trash_variables", False) or config.get("add_trash_ifs", False)):
        main_start = main_match.end()
        inserts = []
        if config.get("add_trash_variables", False):
            for _ in range(random.randint(3, 6)):
                var_type = random.choice(ALL_TYPES)
                name = generate_random_name()
                if var_type == 'char':
                    value = f"'{random.choice('ABCDE')}'"
                else:
                    value = random.randint(1, 1000)
                inserts.append(f"{var_type} {name} = {value};")
        if config.get("add_trash_ifs", False):
            inserts.append(f"if ({random.randint(1,100)} > {random.randint(1,100)}) {{ int {generate_random_name()} = {random.randint(0,999)}; }}")
        code = code[:main_start] + "\n" + '\n'.join(inserts) + "\n" + code[main_start:]

    code = clean_code(code)
    output_file.write(code)

if __name__ == "__main__":
    with open('/home/user/Учеба/OIBLab6/givepasswd.c', 'r') as infile:
        with open('/home/user/Учеба/OIBLab6/shitcode.c', 'w') as outfile:
            config = load_config()
            obfuscate(infile, outfile, config)

    print("Обфускация успешно завершена!")
