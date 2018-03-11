#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tweeter {
    char name[16];
    int num; //number of tweets
};

struct node {
    struct tweeter* data;
	  struct node *nextNode;
};

char* getTweeter(char* line, int num) {
    char *tok = (char *) malloc(sizeof(char) * 12);
    int count = 0;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n")) {
        // printf("TOK = %s\n", tok);
        if(count == num) {
            return tok;
        }
        count++;
    }
    return NULL;
}

int findPosition(char* line) {
    char *tok;
    tok = strtok(line,",\n");
    int count = 0;
    while (tok) {
      if (strcmp(tok,"\"name\"")==0) {
        //printf("found\n");
        return count;
      }
      count++;
      tok = strtok(NULL, ",\n");
    }
    return -1;
}

int main(int argc, char *argv[]) {

    char* path = argv[1];
    FILE* stream = fopen(path, "r");
    if (stream==NULL) {
        printf("Invalid Input Format\n");
    } else {
        char line[1024];

        fgets(line,1024,stream);
        int num = findPosition(line);
        if (num==-1) { //can't find position for "name"
            printf("Invalid Input Format\n");
        }
        else {
            struct node* head = NULL;
            struct node* curNode;
            struct node* first;
            int count = 0;
            int invalid = 0;
            while (fgets(line, 1024, stream)) {
                if (count>20000) {
                  printf("Invalid Input Format\n");
                  invalid = 1;
                  break;
                }
                count++;
                char* tmp = strdup(line);
                //printf("line: %s\n",line);
                char* result = getTweeter(tmp, num);
                //printf("RESULT IS = %s\n", result);
                if (head==NULL) { //if there's no elements in the linked list yet
                    struct tweeter* name = (struct tweeter*)malloc(sizeof(struct tweeter));
                    strcpy(name->name, result);
                    name->num = 1;
                    first = (struct node*)malloc(sizeof(struct node));
                    first->data = name;
                    first->nextNode = NULL;
                    head = first;
                } else {
                    curNode = head;
                    if (strcmp(result,curNode->data->name)==0) { //if tweeter is the same as the first entry
                        curNode->data->num++;
                    } else {
                        int found = 0;
                        while (curNode->nextNode!=NULL) {
                            //try finding a match
                            if (strcmp(result,curNode->data->name)==0) {
                                curNode->data->num++;
                                found = 1;
                                break;
                            }
                            curNode = curNode->nextNode;
                        }
                        if (found==0) { //did not find a match, create new struct and add it to linked list
                            struct tweeter* id = (struct tweeter*)malloc(sizeof(struct tweeter));
                            strcpy(id->name, result);
                            id->num = 1;
                            struct node* cont = (struct node*)malloc(sizeof(struct node));
                            cont->data = id;
                            cont->nextNode = NULL;
                            curNode->nextNode = cont;
                        }
                    }
                }
                free(tmp);
            }
            if (invalid==0) {
                struct tweeter* person = NULL;
                struct tweeter* prev = NULL;
                for (int i = 0; i < 10; i++) {
                    curNode = head;
                    if (prev!=NULL && (prev==head->data)) { //changing the head if previous max was the head
                        //printf("same as head!\n");
                        free(head->data);
                        curNode = head->nextNode;
                        free(head);
                        head = curNode;
                    }
                    if (head==NULL) //no more entries
                        break;
                    person = curNode->data;
                    while (curNode!=NULL) {
                        if (curNode->nextNode != NULL && prev != NULL)
                            if (strcmp(curNode->nextNode->data->name,prev->name)==0) { //previous max
                                //printf("prev max: %s\n",curNode->nextNode->data->name);
                                free(curNode->nextNode->data);
                                struct node* temp;
                                temp = curNode->nextNode->nextNode;
                                free(curNode->nextNode);
                                curNode->nextNode = temp;
                        }
                        if (curNode->data->num > person->num) {
                            person = curNode -> data;
                        }
                        curNode = curNode->nextNode;
                    }
                    printf("%s: %d\n",person->name, person->num);
                    prev = person;
                }
                while (head!=NULL) { //free malloced space
                    curNode = head->nextNode;
                    free(head->data);
                    free(head);
                    head=curNode;
                }
            }
        }
    }
}
