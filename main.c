#include <stdio.h>
#include <stdlib.h>
#define N 10

struct node_s;
typedef struct node_s{
    struct node_s *prev;
    struct node_s *next;
    char* word;
    unsigned int number;
}node_t;
typedef struct{
    node_t *head;
    unsigned int size;
}list_t;
int my_strlen(char* pC){
    int k = 0;
    while (*pC != 0){
        k++;
        pC++;
    }
    return k;
}
int my_strcmp(char* s1, char* s2){
    if (my_strlen(s1) == my_strlen(s2)){
        while (*s1 != 0 && *s1 == *s2){
            s1++;
            s2++;
        }
        return *s1 - *s2;
    }
    else
    if (my_strlen(s1) < my_strlen(s2))
        return -1;
    else return 1;
}
void push_start(list_t *l, char* mas){
    node_t* n = (node_t*) malloc(sizeof (node_t));
    n -> word = mas;
    l -> head -> prev = n;
    n -> next = l -> head;
    n -> prev = NULL;
    l -> head = n;
    l -> size++;
}
void destroy(list_t *l){
    node_t *cur = l -> head;
    node_t *prev = NULL;
    while (cur != NULL){
        prev = cur;
        cur = cur -> next;
        free(prev -> word);
        free(prev);
    }
}
void init(list_t *l){
    l -> head = NULL ;
    l -> size = 0;
}
void push_back(list_t *l, char* mas){
    node_t *n, *cur;
    n = (node_t *) malloc(sizeof(node_t));
    n -> word = mas; n -> next = NULL;
    if (l -> head == NULL){
        n -> prev = NULL;
        l -> head = n;
    }
    else {
        cur = l -> head;
        while (cur -> next != NULL)
            cur = cur -> next;
        cur -> next = n;
        n -> prev = cur;
    }
    l -> size ++;
}
void insert (list_t *l , node_t* cur , char* mas){
    node_t* n = (node_t*) malloc(sizeof (node_t));
    n -> word = mas;
    n -> next = cur -> next;
    n -> prev = cur;
    cur -> next -> prev = n;
    cur -> next = n;
    l -> size++;
}
void print (list_t *l){
    unsigned int flag = 0;
    node_t *cur = l -> head;
    while (cur != NULL){
        cur -> number = ++flag;
        printf("%u)", cur -> number);
        printf ("%s", cur -> word);
        printf ("\n");
        cur = cur -> next;
    }
}
void erase (list_t *l, node_t *cur){
    if(cur == l -> head){
        l -> head = cur -> next;
        if (cur -> next != NULL)
            cur -> next -> prev = NULL;
        free(cur -> word);
        free(cur);
    }
    else{
        cur -> prev -> next = cur -> next;
        if (cur -> next != NULL)
            cur -> next -> prev = cur -> prev;
        free(cur -> word);
        free(cur);
    }
    l -> size--;
}
int make_list(char* word, list_t *l){
    node_t *cur;
    if (l -> head == NULL){
        push_back(l, word);
        return 0;
    }
    else{
        cur = l -> head;
        while (cur != NULL){
            if (my_strcmp(word, cur -> word) <= 0){
                if (l -> head == cur){
                    push_start(l, word);
                    return 0;
                }
                else{
                    cur = cur -> prev;
                    insert(l, cur, word);
                    return 0;
                }}
            cur = cur -> next;
        }
        push_back(l,word);
    }
    return 0;
}
int make_word(list_t *l){
    FILE *f = fopen("C:\\Users\\User\\Desktop\\file.txt","rt");
    if (f == NULL)
        return -1;
    char* mas = NULL;
    char c;
    int letter_count = 0, start_word_position = 0, i;
    while((c = fgetc(f)) == ' ' || c == '\n' || c == '\t'){
        if (c != '\n')
            start_word_position++;
        else
            start_word_position += 2;
    }
    fseek(f, start_word_position, SEEK_SET);
    while((c = fgetc(f)) != EOF){
        if (c == ' ' || c == '\n' || c == '\t'){
            mas = (char*)malloc((letter_count+1) * sizeof(char));
            fseek(f, start_word_position, SEEK_SET);
            start_word_position = start_word_position + letter_count;
            i = 0;
            while(letter_count != 0){
                mas[i++] = fgetc(f);
                letter_count--;
            }
            mas[i] = 0;
            make_list(mas, l);
            while((c = fgetc(f)) == ' ' || c == '\n' || c == '\t'){
                if (c != '\n')
                    start_word_position++;
                else
                    start_word_position += 2;
            }
            fseek(f, start_word_position, SEEK_SET);
        }
        else letter_count++;
    }
    if (letter_count > 0){
        mas = malloc((letter_count+1) * sizeof(char));
        fseek(f, start_word_position, SEEK_SET);
        i = 0;
        while(letter_count != 0){
            mas[i++] = fgetc(f);
            letter_count--;
        }
        mas[i] = 0;
        make_list(mas, l);
    }
    fclose(f);
    return 0;
}
void erase_numbers(list_t *l){
    node_t *cur;
    int k;
    unsigned int key;
    char ch;
    while (l -> size > 0){
        k = scanf("%5u", &key);
        if (k != 1){
            while ((ch = getchar ()) != '\n' && ch != EOF);
            break;
        }
        if (key <= l -> size && key != 0){
            cur = l -> head;
            while (cur != NULL){
                if (cur -> number == key){
                    erase(l, cur);
                    break;
                }
                cur = cur -> next;
            }
            while ((ch = getchar ()) != '\n' && ch != EOF);
            print(l);
        }
        else{
            if (key == 0){
                while ((ch = getchar ()) != '\n' && ch != EOF);
                break;
            }
            else printf("unknown number\n");
        }
    }
}
void destroy_before_word(list_t *l, char* word){
    node_t* cur = l -> head;
    while (l -> head != NULL){
        if(my_strcmp(l -> head -> word, word) >= 0)
            break;
        else{
                l -> head = l -> head -> next;
                free(cur -> word);
                free(cur);
                cur = l -> head;
        }
    }
}
/*char* enter_word(){
    printf("If you want deleted list before word - write the word, else enter '0'\n");
    int str_len = 10, j = 0;
    int k;
    char c;
    char* str = NULL;
    str = malloc(str_len*sizeof(char));
    start:
        str_len += N;
        k = N;
        str = realloc(str, str_len * sizeof(char));
        while ((k != 0) && ((c = getchar()) != 10)){
            if (c == ' '){
                printf("You write spaces in your word, you cant find word with space\n");
                return NULL;
            }
            str[j++] = c;
            k--;
        }
        if (k == 0)
            goto start;
        else
            str[j] = 0;
        return str;
    }*/
char* enter_word() {
    printf("If you want deleted list before word - write the word, else enter '0'\n");
    char c;
    char* str = (char*)malloc(N * sizeof(char));
    unsigned int str_len = 0;
    while((c = getchar()) != '\n'){
        if ((c == ' ') || (c == '\t'))
            break;
        str[str_len++] = c;
        if (str_len % N == 0)
            str = realloc(str,(str_len + N) * sizeof(char));
    }
    str[str_len++] = 0;
    str = realloc(str,str_len * sizeof(char));
    return str;
}

int main() {
    list_t l;
    init(&l);
    int k = make_word(&l);
    if (k == -1)
        printf("error");
    print(&l);
    printf("Enter the numbers of the words to be deleted, when finished deleting enter '0' \n");
    erase_numbers(&l);
    char* word = enter_word();
    if (word != NULL) destroy_before_word(&l, word);
    print(&l);
    destroy(&l);
    free(word);
    return 0;
}//функция которая берёт список и слово функция удаляет все элементы которые лексикографически меньше введённого слова