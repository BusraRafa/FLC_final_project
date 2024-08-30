#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct symbol_table {
    int id;
    char name[50];
    char id_type[10];
    char data_type[10];
    char scope[50];
    char value[100];
}st_t;
st_t *st_r[100];
int count = 0;

int main() {
    remove_comment();
    remove_new_line();
    remove_space();
    add_space();
    tokenize();
    remove_token_name();
    generate_table();
    modify_file();
    write_table();
    table_functions();
    printf("\n");
    printf("**input**\n");
    show_output("input1.txt");
    printf("**space removed**\n");
    show_output("output1c.txt");
    printf("**added space**");
    show_output("output2a.txt");
     printf("**tokenization**");
    show_output("output2b.txt");
     printf("**categorized identifiers**");
    show_output("output3a.txt");
    printf("**symbol table**");
    show_output("output3b.txt");
     printf("**modified**");
    show_output("output3c.txt");
    printf("**error msg**");
    find_errors();
    return 0;
}

void remove_comment() {
    FILE *fp_in = fopen("input1.txt", "r");
    FILE *fp_out = fopen("output1a.txt", "w");
    char c;
    while((c=fgetc(fp_in)) != EOF) {
        if(c == '/') {
            c = fgetc(fp_in);
            if(c == '/') {
                while((c=fgetc(fp_in)) != '\n') continue;
                fputc(c, fp_out);
            }
            if(c == '*') {
                while((c=fgetc(fp_in)) != '*') continue;
                if(c=fgetc(fp_in) == '/') continue;
            }
        }
        else
            fputc(c, fp_out);
    }
    fclose(fp_in);
    fclose(fp_out);
}

void remove_new_line() {
    FILE *fp_in = fopen("output1a.txt", "r");
    FILE *fp_out = fopen("output1b.txt", "w");
    char c;
    int i = 0;
    while((c=fgetc(fp_in)) != EOF) {
        if(i == 0 && c == '\n') {
            continue;
            i++;
        }
        if(c == '\n') fputc(' ', fp_out);
        else fputc(c, fp_out);
    }
    fclose(fp_in);
    fclose(fp_out);
}

void remove_space() {
    FILE *fp_in = fopen("output1b.txt", "r");
    FILE *fp_out = fopen("output1c.txt", "w");
    char c, prev;
    int i = 0;
    while((c=fgetc(fp_in)) != EOF) {
        if(c == ' ' && prev == ' ') continue;
        else fputc(c, fp_out);
        prev = c;
    }
    fclose(fp_in);
    fclose(fp_out);
}

void add_space() {
    FILE *fp_in = fopen("output1c.txt", "r");
    FILE *fp_out = fopen("output2a.txt", "w");
    char c, prev;
    int status = 0;
    while((c=fgetc(fp_in)) != EOF) {
        if((c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == ',' || c == '+' || c == '-' || c == '*' || c == '/' || c == '>' || c == '<') && prev != ' ') {
            fputc(' ', fp_out);
            fputc(c, fp_out);
        }
        else if(c != ' ' && (prev == ';' || prev == '(' || prev == ')' || prev == '{' || prev == '}' || prev == ',' || prev == '=')) {
            fputc(' ', fp_out);
            fputc(c, fp_out);
        }

        else if (c == '=' &&  prev != ' ' && (prev != '+' && prev != '-' && prev != '*' && prev != '/' && prev != '>' && prev != '<')) {
            fputc(' ', fp_out);
            fputc(c, fp_out);
        }

        else fputc(c, fp_out);
        prev = c;
    }
    fclose(fp_in);
    fclose(fp_out);
}

void tokenize() {
    FILE *fp_in = fopen("output2a.txt", "r");
    FILE *fp_out = fopen("output2b.txt", "w");
    int i = 0;
    char w[100];
    char ch;
    while((ch = fgetc(fp_in)) != EOF) {
        if(ch != ' ') {
            w[i] = ch;
            i++;
        }
        else {
            w[i] = '\0';
            i = 0;
            if(isKeyWord(w)) {
                fputs("[kw ", fp_out);
                fputs(w, fp_out);
                fputs("] ", fp_out);
            }
            else if(isSeparator(w)) {
                fputs("[sep ", fp_out);
                fputs(w, fp_out);
                fputs("] ", fp_out);
            }
            else if(isIdentifier(w)) {
                fputs("[id ", fp_out);
                fputs(w, fp_out);
                fputs("] ", fp_out);
            }
            else if(isParenthesis(w)) {
                fputs("[par ", fp_out);
                fputs(w, fp_out);
                fputs("] ", fp_out);
            }
            else if(isBracket(w)) {
                fputs("[brc ", fp_out);
                fputs(w, fp_out);
                fputs("] ", fp_out);
            }
            else if(isOperator(w)) {
                fputs("[op ", fp_out);
                fputs(w, fp_out);
                fputs("] ", fp_out);
            }
            else if(isNumerical(w)) {
                fputs("[num ", fp_out);
                fputs(w, fp_out);
                fputs("] ", fp_out);
            }
            else if (w[i] == '\0') {
                continue;
            }
            else {
                fputs("[unknown ", fp_out);
                fputs(w, fp_out);
                fputs("] ", fp_out);
            }

        }
    }
    fclose(fp_in);
    fclose(fp_out);
}

void remove_token_name() {
    FILE *fp_in = fopen("output2b.txt", "r");
    FILE *fp_out = fopen("output3a.txt", "w");
    char c, w[100];
    int i = 0;
    while((c = fgetc(fp_in)) != EOF) {
        if(c == '[') {
            fputc(c, fp_out);
            while((c = fgetc(fp_in)) != ' ') {
                w[i] = c;
                i++;
            }
            w[i] = '\0';
            i = 0;

            if(!strcmp(w, "id")) fputs(w, fp_out);
            else continue;
        }
        fputc(c, fp_out);
    }
    fclose(fp_in);
    fclose(fp_out);
}

void generate_table() {
    FILE *fp_in = fopen("output3a.txt", "r");
    char c, w[100], name[50], scope[50] = "global";
    int i = 0, is_data_type_found = 0, is_name_found = 0;
    while((c = fgetc(fp_in)) != EOF) {
        if(c == '[') {
            while((c = fgetc(fp_in)) != ']')
            {
                w[i] = c;
                i++;
            }
            w[i] = '\0';
            i = 0;
            if(isName(w)) {
                char *temp = w + 3;
                strcpy(name, temp);
            }
            if(isDataType(w)){
                count++;
                st_r[count] = (st_t *)malloc(sizeof(st_t));
                st_r[count] -> id = count;
                strcpy(st_r[count] -> data_type, w);
                strcpy(st_r[count] -> scope, scope);
                strcpy(st_r[count] -> value, " ");
                is_data_type_found = 1;
            }
            else if(isName(w) && is_data_type_found) {
                char *temp = w + 3;
                strcpy(st_r[count] -> name, temp);
                is_data_type_found = 0;
                is_name_found = 1;
            }
            else if(is_name_found && !strcmp(w, "(")) {
                strcpy(st_r[count] -> id_type, "func");
                strcpy(scope, st_r[count] -> name);
                is_name_found = 0;
            }
            else if(is_name_found) {
                strcpy(st_r[count] -> id_type, "var");
                is_name_found = 0;
            }
            else if(!strcmp(w, "return")) strcpy(scope, "global");
            else if(isNumerical(w))
                for(int i = 1; i <= count; i++)
                    if(!strcmp(st_r[i] -> name, name))
                        strcpy(st_r[i] -> value, w);
        }
    }
    fclose(fp_in);
}

void table_functions(){
    while(1){
        printf("1 = insert a name\n2 = attribute\n3 = all entry\n4 = for search\n5 = display the table\n6 = run\nEnter: ");
        int n;
        scanf("%d", &n);
        switch (n) {
        case 1:
            insert_into_table();
            break;
        case 2:
            set_attribute();
            break;
        case 3:
            Free();
            break;
        case 4:
            lookup();
            break;
        case 5:
            display();
            break;
        case 6:
            return;
        default:
            break;
        }
    }
}

void insert_into_table(){
    printf("Enter Name : ");
    char name[50];
    scanf("%s", name);
    count++;
    st_r[count] = (st_t *)malloc(sizeof(st_t));
    st_r[count] -> id = count;
    strcpy(st_r[count] -> name, name);
}

void set_attribute() {
    printf("Enter Name : ");
    char name[50];
    scanf("%s", name);
    int temp;
    for(int i = 1; i <= count; i++ )
        if(!strcmp(name, st_r[i] -> name)) temp = i;
    printf("Set Id Type : ");
    scanf("%s", st_r[temp] -> id_type);
    printf("Set Data Type : ");
    scanf("%s", st_r[temp] -> data_type);
    printf("Set Scope : ");
    scanf("%s", st_r[temp] -> scope);
    if(!strcmp(st_r[temp] -> id_type, "func"))
        strcpy(st_r[temp] -> value, " ");
    else
    {
        printf("Set Value : ");
        scanf("%s", st_r[temp] -> value);
    }
}

void Free() {
    for(int i = 1; i <= count; i++) {
        strcpy(st_r[i] -> name, "");
        strcpy(st_r[i] -> id_type, "");
        strcpy(st_r[i] -> data_type,"");
        strcpy(st_r[i] -> scope,"");
        strcpy(st_r[i] -> value,"");
        free(st_r[i]);
    }
    count = 0;
}

void lookup() {
    printf("Enter Name : ");
    char name[50];
    scanf("%s", name);
    for(int i = 1; i <= count; i++ )
        if(!strcmp(name, st_r[i] -> name))
            printf("Symbol Index No. %d\n", st_r[i] -> id);
}

void display() {
    printf("SI. No.\tName\tId Type\t\tData Type\tScope\tValue\n");
    for(int i = 1; i <= count; i++) {
        printf("%d\t%s\t%s\t\t%s\t\t%s\t%s\t\n", st_r[i] -> id, st_r[i] -> name, st_r[i] -> id_type, st_r[i] -> data_type, st_r[i] -> scope, st_r[i] -> value);
    }
}

void write_table() {
    FILE *table_output = fopen("output3b.txt", "w");
    fprintf(table_output, "SI. No.\tName\tId Type\t\tData Type\tScope\tValue\n");
    for(int i = 1; i <= count; i++) {
        fprintf(table_output, "%d\t%s\t%s\t\t%s\t\t%s\t%s\t\n", st_r[i] -> id, st_r[i] -> name, st_r[i] -> id_type, st_r[i] -> data_type, st_r[i] -> scope, st_r[i] -> value);
    }
    fclose(table_output);
}

void modify_file() {
    FILE *fp_in = fopen("output3a.txt", "r");
    FILE *fp_out = fopen("output3c.txt", "w");
    char c, w[100], temp_scope[50], scope[50] = "global";
    int i = 0, function_status = 0, scope_status = 0;
    while((c = fgetc(fp_in)) != EOF) {
        if(c == '[') {
            fputc(c, fp_out);
            while((c = fgetc(fp_in)) != ']') {
                w[i] = c;
                i++;
            }
            w[i] = '\0';
            i = 0;
            if(isDataType(w)) function_status = 1;
            if(starts_with(w, "id")) {
                fputs("id ", fp_out);
                char *temp = w + 3;
                for(int i = 1; i <= count; i++) {
                    if(!strcmp(temp, st_r[i]->name) && !strcmp(scope, st_r[i]->scope)) {
                        fprintf(fp_out, "%d", st_r[i]->id);
                        if(function_status && !strcmp(st_r[i]->id_type, "func")) {
                           scope_status = 1;
                           strcpy(temp_scope,st_r[i] -> name);
                        }
                        break;
                    }
                    else if(!strcmp(temp, st_r[i]->name) && !strcmp("func", st_r[i]->id_type))
                        fprintf(fp_out, "%d", st_r[i]->id);
                }
                function_status = 0;
            }
            else fputs(w, fp_out);
            if(scope_status == 1) strcpy(scope, temp_scope);
            if(scope_status == 2) strcpy(scope, "global");
            if(!strcmp(w, "return")) scope_status = 2;
            fputs("] ", fp_out);
        }
    }
    fclose(fp_in);
    fclose(fp_out);
}
void find_errors(){
    FILE *p1, *p2;
    char c,parenthesis[15], buffer[10];
    bool semicolon = false, else_without_if = false;
    int index_for_par = -1,iterator = 0;
    p1 = fopen("output1a.txt","r");
    p2 = fopen("output4a.txt", "w");
    iterator++;

    itoa(iterator, buffer, 10);

    fputs(buffer, p2);
    fputc(' ', p2);
    while((c = fgetc(p1)) != EOF){
        if(c == '\n'){
            fputc(c, p2);
            iterator++;
            itoa(iterator, buffer, 10);
            fputs(buffer, p2);
            fputc(' ', p2);
        }
        else{
            if(c == '/'){
                c = fgetc(p1);
                if(c == '*'){
                    while(c != '/')
                    {
                        c = fgetc(p1);
                    }
                }
            }
            else{
                fputc(c, p2);
            }
        }
    }
    fclose(p1);
    fclose(p2);
     printf("\n\n");
    p1 = fopen("output4a.txt", "r");
    while((c = fgetc(p1)) != EOF){
        printf("%c", c);
    }
    fclose(p1);
    printf("\n\n");

    int line_number = 1;
    bool for_loop_found = false;
    p1 = fopen("output4a.txt", "r");
    while((c = fgetc(p1)) != EOF){
        if(c == '\n'){
            int i = 0;
            while(c != ' '){
                c = fgetc(p1);
                if(c != ' '){
                    buffer[i++] = c;
                }
            }
            buffer[i] = '\0';
            line_number = atoi(buffer);
        }
        else{
            if(c == 'f'){
                if((c = fgetc(p1)) == 'o'){
                    if((c = fgetc(p1)) == 'r'){
                        for_loop_found = true;
                    }
                }
            }
            if(c == ';' && semicolon == false && for_loop_found == false){
                semicolon = true;


            }
            else if(c == ';' && semicolon == true){
                printf("\nDuplicate token at line %d, ", line_number);
                semicolon = false;
            }
            else{
                if(c != ';' && semicolon == true){
                    if(c != ' '){
                        semicolon = false;
                    }
                }
                if(c != ';'){
                    if(c == 'i'){
                        if((c = fgetc(p1)) == 'f')
                        {
                            else_without_if = true;
                        }
                    }
                    else if(c == 'e'){
                        if((c = fgetc(p1)) == 'l'){
                            if((c = fgetc(p1)) == 's'){
                                if((c = fgetc(p1)) == 'e'){
                                    if(!else_without_if){
                                       printf("\nUnmatched 'else' at line %d, ", line_number);
                                    }
                                    else{
                                        else_without_if = false;
                                    }
                                }
                            }
                        }
                    }
                    else if(c == '(' || c == '{' || c == '['){
                        index_for_par++;
                        parenthesis[index_for_par] = c;
                    }
                    else if(c == ')' || c == '}' || c == ']'){
                        if(c == ')'){
                            if(parenthesis[index_for_par] == '('){
                                index_for_par--;
                            }
                            else{
                                printf("\nMisplaced ')' at line %d, ", line_number);
                            }
                            if(for_loop_found == true){
                                for_loop_found = false;
                            }
                        }
                        else if(c == '}'){
                            if(parenthesis[index_for_par] == '{'){
                                index_for_par--;
                            }
                            else{
                                printf("\nMisplaced '}' at line %d, ", line_number);
                            }
                        }
                        else if(c == ']'){
                            if(parenthesis[index_for_par] == '['){
                                index_for_par--;
                            }
                            else{
                                printf("\nMisplaced ']' at line %d, ", line_number);
                            }
                        }
                    }
                }
            }
        }
    }
    if(index_for_par != -1){
        printf("\nUnmatched opening parenthesis detected ");
    }
    fclose(p1);
   // return 0;
}

int isKeyWord(char *s) {
    const char *keywords[] = { "auto", "break", "case", "char", "continue", "do", "default", "const",
                                "double", "else", "enum", "extern", "for", "if", "goto", "float",
                                "int", "long", "register", "return", "signed", "static", "sizeof", "short",
                                "struct", "switch", "typedef", "union", "void", "while", "volatile", "unsigned"  };
    int i;
    for(i = 0; i < 32; i++) {
        if(strcmp(s, keywords[i]) == 0) return 1;
    }
    return 0;
}

int isIdentifier(char *s) {
    if(islower(s[0]) || isupper(s[0]) || s[0] == '_') {
        if(strlen(s) == 1) return 1;
        int i;
        for ( i = 1 ; i < strlen(s); i++)
        {
            if( islower(s[0]) || isupper(s[0]) || s[0] == '_' || isdigit(s[0]) )
                return 1;
            else
                return 0;
        }
    }
    return 0;
}

int isNumerical(char *s) {
    int i, n, x;
    i = 0;
    if(isdigit(s[i])) {
        x = 1;
        i++;
    }
    else if(s[i]=='.') {
        x = 2;
        i++;
    }
    else x = 0;
    n = strlen(s);
    if(x == 1)

        for(i = 1; i < n; i++) {
            if(isdigit(s[i]))
                x = 1;
            else if(s[i]=='.') {
                x = 2;
                i++;
                break;
            }
            else {
                x=0;
                break;
            }
        }


    if(x == 2) {
        if(isdigit(s[i])) {
            x = 3;
            i++;
        }
        else x = 0;
    }


    if(x == 3) {
        for(; i < n; i++)
        {
            if(isdigit(s[i])) x = 3;
            else {
                x = 0;
                break;
            }
        }
    }
    if(x==3) x = 1;
    return x;
}

int isSeparator(char *s) {
    if( s[0] == ';' || s[0] == ',' || s[0] == '\'') return 1;
    return 0;
}

int isParenthesis(char *s) {
    if( s[0] == '(' || s[0] == ')' || s[0] == '[' || s[0] == ']' ) return 1;
    return 0;
}

int isBracket(char *s) {
    if( s[0] == '{' || s[0] == '}' ) return 1;
    return 0;
}

int isOperator(char *s) {
    if(strlen(s) == 1)
        if( s[0] == '+' || s[0] == '-' || s[0] == '*' || s[0] == '/' || s[0] == '%' || s[0] == '=' || s[0] == '<' || s[0] == '>' || s[0] == '!' || s[0] == '&' || s[0] == '|' || s[0] == '~' || s[0] == '^' ) return 1;

    if(strlen(s) == 2)
        if( (s[0] == '+' && s[1] == '+') || (s[0] == '-' && s[1] == '-') || (s[0] == '=' && s[1] == '=') || (s[0] == '!' && s[1] == '=') || (s[0] == '|' && s[1] == '|') || (s[0] == '&' && s[1] == '&') || (s[0] == '>' && s[1] == '=') || (s[0] == '<' && s[1] == '=') ) return 1;

    return 0;
}

int starts_with(char *s, char *m){
    int status = 0;
    for (int i = 0; m[i] != '\0'; i++) {
        if (s[i] != m[i]) {
            status = 0;
            break;
        }
        status = 1;
    }
    return status;
}

int isDataType(char *s) {
    const char *keywords[] = { "char", "double", "float", "int", "struct", "void"};
    for(int i = 0; i < 5; i++) {
        if(strcmp(s, keywords[i]) == 0) return 1;
    }
    return 0;
}

int isName(char *s) {
    int status = 0;
    if(islower(s[0]) || isupper(s[0]) || s[0] == '_') {
        if(strlen(s) == 1) return 1;
        for (int i = 1 ; i < strlen(s); i++) {
            if( islower(s[0]) || isupper(s[0]) || s[0] == '_' || isdigit(s[0]) ) status = 1;
            else status = 0;
        }
    }
    return status;
}

void show_output(char *file_name) {
    FILE *fp_in = fopen(file_name, "r");
    if(fp_in == NULL) {
        printf("File not found.");
        exit(1);
    }
    char c;
    printf("\n");
    while((c = fgetc(fp_in)) != EOF) {
        printf("%c", c);
    }
    printf("\n\n");
}
