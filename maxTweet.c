#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tweeter {
    char* name;
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

    //char* path = argv[0];
    char* path = "cl-tweets-shorter.csv";
    FILE* stream = fopen(path, "r");

    // struct tweeter* headTweeter;
    char line[1024];
    fgets(line,1024,stream);
    struct node* head = NULL;
    struct node* curNode;
    while (fgets(line, 1024, stream)) {
        char* tmp = strdup(line);
        char* result = getTweeter(tmp, 8);
        printf("RESULT IS = %s\n", result);
        if (head==NULL) {
            struct tweeter* name = (struct tweeter*)malloc(sizeof(struct tweeter));
            name->name = result;
            name->num = 1;
            struct node* first = (struct node*)malloc(sizeof(struct node));
            first->data = name; //might have to be a reference
            first->nextNode = NULL;
            head = first;
            //printf("head: %s\n",head->data->name);
        } else {
            curNode = head;
            if (result==curNode->data->name) {
                curNode->data->num++;
            } else {
                int found = 0;
                while (curNode->nextNode!=NULL) {
                    //try finding a match, if not then create a new struct and place it
                    if (result==curNode->data->name) {
                        curNode->data->num++;
                        found = 1;
                        break;
                    }
                    curNode = curNode->nextNode;
                }
                if (found==0) {
                    struct tweeter* id = (struct tweeter*)malloc(sizeof(struct tweeter));
                    id->name = result;
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
