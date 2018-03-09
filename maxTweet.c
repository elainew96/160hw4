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

char* getTweeter(char* line, int num)
{
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

int main(int argc, char *argv[]) {

    char* path = argv[1];
    //char* path = "cl-tweets-shorter.csv";
    FILE* stream = fopen(path, "r");

    // struct tweeter* headTweeter;
    char line[1024];
    fgets(line,1024,stream);
    struct node* head = NULL;
    struct node* curNode;
    struct node* first;
    while (fgets(line, 1024, stream)) {
        char* tmp = strdup(line); //this line messes up the stored stuff
        //printf("line: %s\n",line);
        char* result = getTweeter(tmp, 8);
        printf("RESULT IS = %s\n", result);
        if (head==NULL) {
            struct tweeter* name = (struct tweeter*)malloc(sizeof(struct tweeter));
            strcpy(name->name, result);
            name->num = 1;
            first = (struct node*)malloc(sizeof(struct node));
            first->data = name; //might have to be a reference
            first->nextNode = NULL;
            head = first;
        } else {
            curNode = head;
            if (strcmp(result,curNode->data->name)==0) {
                curNode->data->num++;
            } else {
                int found = 0;
                while (curNode->nextNode!=NULL) {
                  //  printf("curNode: %s\n",curNode->data->name);
                    //try finding a match, if not then create a new struct and place it
                    if (strcmp(result,curNode->data->name)==0) {
                        curNode->data->num++;
                        found = 1;
                        break;
                    }
                    curNode = curNode->nextNode;
                }
                if (found==0) {
                    struct tweeter* id = (struct tweeter*)malloc(sizeof(struct tweeter));
                    strcpy(id->name, result);
                    id->num = 1;
                    struct node* cont = (struct node*)malloc(sizeof(struct node));
                    cont->data = id; //might have to be a reference
                    cont->nextNode = NULL;
                    curNode->nextNode = cont;
                }
            }
        }
        free(tmp);
    }
    curNode = head;
    while (curNode!=NULL) {
      printf("name: %s appearances: %d\n",curNode->data->name,curNode->data->num);
      curNode = curNode->nextNode;
    }
}
