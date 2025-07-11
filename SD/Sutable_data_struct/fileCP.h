


void delcom(FILE* input, FILE* output) {
    short flag = 0; // 0 - текст, 1 - строка, 2 - возможный комментарий, 3 - однострочный комментарий, 4 - многострочный комментарий, 5 - символьный литерал
    char simb = 0;
    char presimb = 0;
    
    while ((simb = fgetc(input)) != EOF) {

        
        if (flag == 1) {
            fputc(simb, output);
            if (simb == '\\') {
                
                simb = fgetc(input);
                if (simb == EOF) {
                    break;
                }
                
                fputc(simb, output);
                continue;
            }
            if (simb == '"') {
                flag = 0; 
            }
            else if (simb == '\n') {
                flag = 0; 
            }

            continue;
        }

        
        if (flag == 5) {
            fputc(simb, output);
            if (simb == '\\') {
                
                simb = fgetc(input);
                if (simb == EOF) {
                    break;
                }
                /*if (simb == '\n') {
                    flag = 0;
                }*/
                fputc(simb, output);
                continue;
            }
            if (simb == '\'') {
                flag = 0; 
            }
            else if (simb == '\n') {
                flag = 0; 
            }

            continue;
        }

       
        if (flag == 2) {
            if (simb == '/') {
                flag = 3; 
            }
            else if (simb == '*') {
                flag = 4; 

            }
            else {
                fputc('/', output);
                if (simb == '"') {
                    fputc(simb, output);
                    flag = 1;
                    continue;
                }
                if (simb == '\'') {
                    fputc(simb, output);
                    flag = 5;
                    continue;
                }
                fputc(simb, output);
                flag = 0;
            }
            continue;
        }

        
        if (flag == 3) {
            if (simb == '\\') {
                simb = fgetc(input);  

                if (simb == EOF) {
                    break;
                }
                continue;
            }
            if (simb == '\n') {
                fputc(simb, output);
                flag = 0;
            }
            continue;
        }

        
        if (flag == 4) {
            if (presimb == '*' && simb == '/') {
                flag = 0; 
                presimb = 0; 
            }
            else if (simb == '*') {
                presimb = '*'; 
            }
            else {
                presimb = 0; 
            }
            continue;
        }

        
        if (simb == '"' && flag == 0) {
            flag = 1;
            fputc(simb, output);
            continue;
        }

        
        if (simb == '\'' && flag == 0) {
            flag = 5;
            fputc(simb, output);
            continue;
        }

        
        if (simb == '/' && flag == 0) {
            flag = 2;
            continue;
        }

        
        if (flag == 0) {
            fputc(simb, output);
        }
    }
    if (flag == 2) {
        fputc('/', output);
    }
}








