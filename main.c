#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    char word[20];
    char files[20][20];
    struct Node* left;
    struct Node* right;
    int counter;
}node;
void Add_File_Name(char File_Name[20], node* curr_data){
    int i = 0;
    int unique = 0;
    while (i <= curr_data->counter && unique == 0) {
        if (strcmp(File_Name, curr_data->files[i]) == 0) {
            unique = 1;
        }
        i++;
    }
    if (unique == 0) {
        strcpy(curr_data->files[curr_data->counter], File_Name);
        curr_data->counter++;
    }
}
void Search_Word(char word[20], node* head){
    node* curr_data = head;
    int i;
    if (curr_data != NULL) {
        printf("%s ", curr_data->word); //path to the searching word
        if (strcmp(word, curr_data->word) > 0) {
            if (curr_data->right == NULL) {
                Search_Word(word, head->left);
            }else{
                Search_Word(word, head->right);
            }
        }else if (strcmp(word, curr_data->word) < 0){
            if (curr_data->left == NULL) {
                Search_Word(word, head->right);
            }else{
                Search_Word(word, head->left);
            }
        }else{
            printf("\nKelimenin bulundugu dosyalar: ");
            for (i = 0; i < curr_data->counter; i++) {
                printf("\n%s", curr_data->files[i]);
            }
        }
    }else{
        printf("\nAradiginiz kelime bulunamamistir.");
    }
}
node* Tree_Check(char* token, char File_Name[20], node* head){
    node* curr_data = head;
    if (curr_data != NULL) {
        if (strcmp(token, curr_data->word) > 0) {
            curr_data->right = Tree_Check(token, File_Name, curr_data->right);
        }else if (strcmp(token, curr_data->word) < 0){
            curr_data->left = Tree_Check(token, File_Name, curr_data->left);
        }else{
            Add_File_Name(File_Name, curr_data);
        }
    }else{
        curr_data=(node*)malloc(sizeof(node));
        curr_data->left = NULL;
        curr_data->right = NULL;
        curr_data->counter = 0;
        strcpy(curr_data->word, token);
        printf("\n%s", curr_data->word);
        Add_File_Name(File_Name, curr_data);
    }
    return curr_data;
}
void Split_Data(node* holder,char buff[255],char delimiter[2], char File_Name[20], node* head){
    char *token;
    token=strtok(buff,delimiter);
    while (token != NULL) {
        head = Tree_Check(token, File_Name, head);
        token = strtok(NULL,delimiter);
    }
}
void File_Read(char File_Name[20],node* head){
    FILE *fp=fopen(File_Name,"r");
    char delimiter[2] = " ";
    char buf[255];
    if(fgets(buf,255,fp)!=NULL){
        printf("%s",buf);
        Split_Data(head,buf,delimiter,File_Name,head);
    }
    fclose(fp);
}
void Delete_This_Element(node* prev_data2, node* head){
    node* curr_data = head;
    node* prev_data;
    node* tmp_data = head;
    if (prev_data2 == NULL) {
        if (curr_data->left != NULL && curr_data->right != NULL) {
            tmp_data = curr_data->right;
            curr_data = curr_data->left;
            prev_data = curr_data;
            while (prev_data->right != NULL) {
                prev_data = prev_data->right;
            }
            prev_data->right = tmp_data;
            head = curr_data;
        }else if (curr_data->left != NULL && curr_data->right == NULL){
            curr_data = curr_data->left;
            head = curr_data;
        }else if (curr_data->left == NULL && curr_data->right != NULL){
            curr_data = curr_data->right;
            head = curr_data;
        }else{
            curr_data = NULL;
            head = curr_data;
        }
    }else{
        if (curr_data->left != NULL && curr_data->right != NULL) {
            tmp_data = curr_data->right;
            curr_data = curr_data->left;
            if (prev_data2->right == head) {
                prev_data2->right = curr_data;
            }else{
                prev_data2->left = curr_data;
            }
            prev_data = curr_data;
            while (prev_data->right != NULL) {
                prev_data = prev_data->right;
            }
            prev_data->right = tmp_data;
        }else if (curr_data->left != NULL && curr_data->right == NULL){
            curr_data = curr_data->left;
            if (prev_data2->right == head) {
                prev_data2->right = curr_data;
            }else{
                prev_data2->left = curr_data;
            }
        }else if (curr_data->left == NULL && curr_data->right != NULL){
            curr_data = curr_data->right;
            if (prev_data2->right == head) {
                prev_data2->right = curr_data;
            }else{
                prev_data2->left = curr_data;
            }
        }else{
            curr_data = NULL;
            if (prev_data2->right == head) {
                prev_data2->right = curr_data;
            }else{
                prev_data2->left = curr_data;
            }
        }
    }
}
void Delete_File(node* prev_data, node* head, char File_Name[20]){
    node* curr_data = head;
    int i = 0, j = 0;
    int unique = 0;
    if (curr_data != NULL) {
        while (i < curr_data->counter && unique == 0) {
            if (strcmp(curr_data->files[i], File_Name) == 0) {
                curr_data->counter--;
                for (j = 0; j < 20; j++) {
                    curr_data->files[i][j] = 0;
                }
                unique = 1;
                j = i;
                for (i = j; i <= curr_data->counter; i++) {
                    strcpy(curr_data->files[i], curr_data->files[i+1]);
                }
            }
            i++;
        }
        if (curr_data->counter == 0) {
            Delete_This_Element(prev_data, curr_data);
        }
        Delete_File(curr_data, curr_data->left, File_Name);
        Delete_File(curr_data, curr_data->right, File_Name);
    }
}
int menu(){
    int chs = 0;
    printf("\nYapmak istediginiz islemi seciniz\nDosya ekleme-(1)\nDosya silme-(2)");
    printf("\nKelime arama-(3)\nCikis-(4)\n");
    scanf(" %d", &chs);
    if (chs < 1 || chs > 4) {
        printf("\nYanlis bir secim yaptiniz. Lutfen tekrar deneyiniz.");
        chs = menu();
    }
    return chs;
}
int main(int argc, const char * argv[]) {
    node* head=(node*)malloc(sizeof(node));
    head->left = NULL;
    head->right = NULL;
    head->counter = 0;
    char File_Name[20];
    char word[20];
    int chs = 0;
    while (chs != 4) {
        chs = menu();
        if (chs == 1) {
            printf("\nEklemek istediginiz dosyanin adini giriniz.\n");
            scanf(" %s", File_Name);
            File_Read(File_Name, head);
        }else if (chs == 2){
            printf("\nSilmek istediginiz dosyanin adini giriniz.\n");
            scanf(" %s", File_Name);
            Delete_File(NULL, head, File_Name);
        }else if (chs == 3){
            printf("\nAramak istediginiz kelimeyi giriniz.\n");;
            scanf(" %s", word);
            Search_Word(word, head);
        }
    }
    return 0;
}
