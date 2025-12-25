#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NEEDS 10
#define MAX_HELPERS 50
#define DATA_FILE "elderly_complete_project.txt"

/* ---------------- ENUM ---------------- */
typedef enum {
    LIFETIME = 1,
    TEMPORARY
} NeedType;

/* ---------------- HELPER STRUCT ---------------- */
typedef struct {
    int helperID;
    char name[50];
    char skills[50];
    char contact[20];
    char address[100];
} Helper;

/* ---------------- NEED STRUCT ---------------- */
typedef struct {
    char needName[50];
    NeedType type;
    char description[200];
    int duration;          // in days
    int helperID;
    char helperSkill[50];
} Need;

/* ---------------- ELDER STRUCT (LINKED LIST) ---------------- */
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

/* ---------------- GLOBAL VARIABLES ---------------- */
Elder *head = NULL;
int nextElderID = 1001;

/* ---------------- HELPER QUEUE ---------------- */
Helper helperQueue[MAX_HELPERS];
int front = 0, helperCount = 0;

/* ---------------- HELPER STACK ---------------- */
Helper helperStack[100];
int top = -1;

/* ================================================= */
/*              HELPER MANAGEMENT                    */
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
        sprintf(helperQueue[i].address, "Helper Street %d, City", i + 1);
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
/*              ELDER MANAGEMENT                     */
/* ================================================= */

Elder* createElder() {
    Elder *e = (Elder*)malloc(sizeof(Elder));
    e->id = nextElderID++;
    e->needCount = 0;
    e->next = NULL;

    printf("\nEnter Elder Name: ");
    scanf(" %[^\n]", e->name);
    printf("Enter Age: ");
    scanf("%d", &e->age);
    printf("Enter Address: ");
    scanf(" %[^\n]", e->address);
    printf("Enter Contact Number: ");
    scanf(" %[^\n]", e->contact);

    return e;
}

void addElder() {
    Elder *e = createElder();
    e->next = head;
    head = e;
    printf("\nElder added successfully! ID = %d\n", e->id);
}

Elder* findElder(int id) {
    Elder *cur = head;
    while (cur) {
        if (cur->id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

/* ================================================= */
/*                NEED MANAGEMENT                    */
/* ================================================= */

void addNeed(Elder *e) {
    if (e->needCount >= MAX_NEEDS) {
        printf("Maximum needs reached.\n");
        return;
    }

    Need *n = &e->needs[e->needCount];

    printf("\n1. Lifetime Need\n2. Temporary Need\nChoose: ");
    scanf("%d", (int*)&n->type);

    if (n->type == LIFETIME) {
        printf("\nLifetime Needs:\n");
        printf("1. Permanent Caretaker\n2. 24x7 Nurse\n3. Wheelchair\n");
        printf("4. Bedridden Support\n5. Alzheimer Care\n6. Medication Manager\nChoice: ");
        int c; scanf("%d", &c);

        switch (c) {
            case 1: strcpy(n->needName, "Permanent Caretaker");
                    strcpy(n->description, "Full-time caretaker support."); break;
            case 2: strcpy(n->needName, "24x7 Nurse");
                    strcpy(n->description, "Continuous medical nursing."); break;
            case 3: strcpy(n->needName, "Wheelchair Assistance");
                    strcpy(n->description, "Mobility assistance using wheelchair."); break;
            case 4: strcpy(n->needName, "Bedridden Support");
                    strcpy(n->description, "Complete care for bedridden elders."); break;
            case 5: strcpy(n->needName, "Alzheimer Care");
                    strcpy(n->description, "Special Alzheimer patient care."); break;
            case 6: strcpy(n->needName, "Medication Manager");
                    strcpy(n->description, "Daily medicine schedule management."); break;
        }
    } else {
        printf("\nTemporary Needs:\n");
        printf("1. Hospital Visit\n2. Physiotherapy\n3. Emergency Support\n");
        printf("4. Post Surgery Care\n5. Short-Term Nurse\n6. House Cleaning\nChoice: ");
        int c; scanf("%d", &c);

        switch (c) {
            case 1: strcpy(n->needName, "Hospital Visit Help");
                    strcpy(n->description, "Assistance during hospital visits."); break;
            case 2: strcpy(n->needName, "Physiotherapy");
                    strcpy(n->description, "Physical therapy sessions."); break;
            case 3: strcpy(n->needName, "Emergency Support");
                    strcpy(n->description, "Immediate emergency response."); break;
            case 4: strcpy(n->needName, "Post Surgery Care");
                    strcpy(n->description, "Recovery assistance after surgery."); break;
            case 5: strcpy(n->needName, "Short-Term Nurse");
                    strcpy(n->description, "Temporary nursing support."); break;
            case 6: strcpy(n->needName, "House Cleaning");
                    strcpy(n->description, "Household cleaning assistance."); break;
        }
    }

    printf("Enter duration of help (in days): ");
    scanf("%d", &n->duration);

    Helper h = assignHelper();
    n->helperID = h.helperID;
    strcpy(n->helperSkill, h.skills);

    pushHelperHistory(h);
    e->needCount++;

    printf("\nHelper Assigned Automatically!\n");
    printf("Helper ID: %d\nName: %s\nSkill: %s\nContact: %s\nAddress: %s\n",
           h.helperID, h.name, h.skills, h.contact, h.address);
}

/* ================================================= */
/*                DISPLAY DETAILS                    */
/* ================================================= */

void displayAll() {
    Elder *cur = head;
    if (!cur) {
        printf("\nNo records found.\n");
        return;
    }

    while (cur) {
        printf("\n=====================================");
        printf("\nElder ID: %d\nName: %s\nAddress: %s\nContact: %s\n",
               cur->id, cur->name, cur->address, cur->contact);

        for (int i = 0; i < cur->needCount; i++) {
            Need n = cur->needs[i];
            printf("\nNeed: %s\nType: %s\nDuration: %d days\n",
                   n.needName,
                   n.type == LIFETIME ? "Lifetime" : "Temporary",
                   n.duration);
            printf("Helper ID: %d\nSpecialization: %s\nDescription: %s\n",
                   n.helperID, n.helperSkill, n.description);
        }
        cur = cur->next;
    }
}

/* ================================================= */
/*                FILE SAVE                          */
/* ================================================= */

void saveToFile() {
    FILE *fp = fopen(DATA_FILE, "w");
    Elder *cur = head;

    while (cur) {
        fprintf(fp, "%d|%s|%s|%s|%d\n",
                cur->id, cur->name, cur->address,
                cur->contact, cur->needCount);

        for (int i = 0; i < cur->needCount; i++) {
            Need n = cur->needs[i];
            fprintf(fp, "%s|%d|%s|%d|%d|%s\n",
                    n.needName, n.type, n.description,
                    n.duration, n.helperID, n.helperSkill);
        }
        cur = cur->next;
    }
    fclose(fp);
    printf("\nData saved successfully.\n");
}

/* ================================================= */
/*                    MAIN                           */
/* ================================================= */

int main() {
    generateHelpers();

    int choice, id;

    while (1) {
        printf("\n====== ELDERLY HELPING SYSTEM ======\n");
        printf("1. Add Elder\n");
        printf("2. Add Need\n");
        printf("3. Display All Details\n");
        printf("4. Save & Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addElder(); break;
            case 2:
                printf("Enter Elder ID: ");
                scanf("%d", &id);
                Elder *e = findElder(id);
                if (e) addNeed(e);
                else printf("Elder not found.\n");
                break;
            case 3: displayAll(); break;
            case 4:
                saveToFile();
                printf("Project Ended Successfully.\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
}
