#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NEEDS 10
#define MAX_HELPERS 50
#define DATA_FILE "elderly_complete_project.txt"

/* ================= ENUM ================= */
typedef enum {
    LIFETIME = 1,
    TEMPORARY
} NeedType;

/* ================= HELPER ================= */
typedef struct {
    int helperID;
    char name[50];
    char skills[50];
    char contact[20];
    char address[100];
} Helper;

/* ================= NEED ================= */
typedef struct {
    char needName[50];
    NeedType type;
    char description[200];
    int duration;
    int helperID;
    char helperSkill[50];
} Need;

/* ================= ELDER (LINKED LIST) ================= */
typedef struct Elder {
    int id;
    char name[50];
    int age;
    char address[120];
    char contact[20];
    Need needs[MAX_NEEDS];
    int needCount;
    struct Elder *next;
} Elder;

/* ================= GLOBALS ================= */
Elder *head = NULL;
int nextElderID = 1001;

/* ================= QUEUE ================= */
Helper helperQueue[MAX_HELPERS];
int front = 0, helperCount = 0;

/* ================= STACK ================= */
Helper helperStack[100];
int top = -1;

/* ================================================= */
/*                  HELPER MANAGEMENT                */
/* ================================================= */

void generateHelpers() {
    char *names[] = {"Ramesh","Suresh","Anita","Meena","Rahul",
                     "Kavya","Arun","Divya","Vikram","Pooja"};

    char *skills[] = {"Nurse","Caretaker","Wheelchair Assistant",
                      "Physiotherapist","Medical Helper"};

    for (int i = 0; i < MAX_HELPERS; i++) {
        helperQueue[i].helperID = 5000 + i;
        sprintf(helperQueue[i].name, "%s_%d", names[i % 10], i);
        strcpy(helperQueue[i].skills, skills[i % 5]);
        sprintf(helperQueue[i].contact, "9%09d", 100000000 + i);
        sprintf(helperQueue[i].address, "Helper Street %d", i + 1);
        helperCount++;
    }
}

Helper assignHelper() {
    Helper empty = {0,"N/A","N/A","N/A","N/A"};
    if (helperCount == 0) return empty;

    Helper h = helperQueue[front];
    front = (front + 1) % MAX_HELPERS;
    helperCount--;
    return h;
}

void pushHelperHistory(Helper h) {
    if (top < 99)
        helperStack[++top] = h;
}

/* ================================================= */
/*               LINKED LIST OPERATIONS              */
/* ================================================= */

Elder* createElder() {
    Elder *e = (Elder*)malloc(sizeof(Elder));
    e->id = nextElderID++;
    e->needCount = 0;
    e->next = NULL;

    printf("Name: "); scanf(" %[^\n]", e->name);
    printf("Age: "); scanf("%d", &e->age);
    printf("Address: "); scanf(" %[^\n]", e->address);
    printf("Contact: "); scanf(" %[^\n]", e->contact);

    return e;
}

/* INSERTION */
void addElder() {
    Elder *e = createElder();
    e->next = head;
    head = e;
    printf("Elder Added Successfully (ID=%d)\n", e->id);
}

/* SEARCH */
Elder* findElder(int id) {
    Elder *cur = head;
    while (cur) {
        if (cur->id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

/* UPDATE */
void updateElder() {
    int id;
    printf("Enter Elder ID to update: ");
    scanf("%d", &id);

    Elder *e = findElder(id);
    if (!e) {
        printf("Elder not found!\n");
        return;
    }

    printf("New Name: "); scanf(" %[^\n]", e->name);
    printf("New Age: "); scanf("%d", &e->age);
    printf("New Address: "); scanf(" %[^\n]", e->address);
    printf("New Contact: "); scanf(" %[^\n]", e->contact);

    printf("Elder updated successfully.\n");
}

/* DELETION */
void deleteElder() {
    int id;
    printf("Enter Elder ID to delete: ");
    scanf("%d", &id);

    Elder *cur = head, *prev = NULL;

    while (cur) {
        if (cur->id == id) {
            if (prev) prev->next = cur->next;
            else head = cur->next;

            free(cur);
            printf("Elder deleted successfully.\n");
            return;
        }
        prev = cur;
        cur = cur->next;
    }
    printf("Elder not found.\n");
}

/* ================================================= */
/*                 NEED MANAGEMENT                   */
/* ================================================= */

void addNeed(Elder *e) {
    if (e->needCount >= MAX_NEEDS) {
        printf("Maximum needs reached.\n");
        return;
    }

    Need *n = &e->needs[e->needCount];

    printf("1.Lifetime  2.Temporary : ");
    scanf("%d", (int*)&n->type);

    if (n->type == LIFETIME) {
        strcpy(n->needName, "Permanent Care");
        strcpy(n->description, "Lifetime elder care support");
    } else {
        strcpy(n->needName, "Temporary Support");
        strcpy(n->description, "Short-term assistance");
    }

    printf("Duration (days): ");
    scanf("%d", &n->duration);

    Helper h = assignHelper();
    n->helperID = h.helperID;
    strcpy(n->helperSkill, h.skills);

    pushHelperHistory(h);
    e->needCount++;

    printf("Helper Assigned: %s (%s)\n", h.name, h.skills);
}

/* ================================================= */
/*                 DISPLAY                           */
/* ================================================= */

void displayAll() {
    Elder *cur = head;
    if (!cur) {
        printf("No records found.\n");
        return;
    }

    while (cur) {
        printf("\nID:%d  Name:%s  Age:%d\nAddress:%s\nContact:%s\n",
               cur->id, cur->name, cur->age,
               cur->address, cur->contact);

        for (int i = 0; i < cur->needCount; i++) {
            Need n = cur->needs[i];
            printf("Need:%s | Duration:%d | HelperID:%d | Skill:%s\n",
                   n.needName, n.duration, n.helperID, n.helperSkill);
        }
        cur = cur->next;
    }
}

/* ================================================= */
/*                 FILE SAVE                         */
/* ================================================= */

void saveToFile() {
    FILE *fp = fopen(DATA_FILE, "w");
    Elder *cur = head;

    while (cur) {
        fprintf(fp, "%d|%s|%d|%s|%s|%d\n",
                cur->id, cur->name, cur->age,
                cur->address, cur->contact, cur->needCount);

        for (int i = 0; i < cur->needCount; i++) {
            Need n = cur->needs[i];
            fprintf(fp, "%s|%d|%d|%d|%s\n",
                    n.needName, n.type, n.duration,
                    n.helperID, n.helperSkill);
        }
        cur = cur->next;
    }
    fclose(fp);
    printf("Data saved successfully.\n");
}

/* ================================================= */
/*                     MAIN                          */
/* ================================================= */

int main() {
    generateHelpers();
    int ch, id;

    while (1) {
        printf("\n====== ELDERLY HELPING SYSTEM ======\n");
        printf("1.Add Elder\n2.Update Elder\n3.Delete Elder\n");
        printf("4.Add Need\n5.Display All\n6.Save & Exit\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: addElder(); break;
            case 2: updateElder(); break;
            case 3: deleteElder(); break;
            case 4:
                printf("Enter Elder ID: ");
                scanf("%d", &id);
                Elder *e = findElder(id);
                if (e) addNeed(e);
                else printf("Elder not found.\n");
                break;
            case 5: displayAll(); break;
            case 6:
                saveToFile();
                printf("Project Exit Successfully.\n");
                exit(0);
            default: printf("Invalid Choice\n");
        }
    }
}
