#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NEEDS 10
#define MAX_HELPERS 50
#define DATA_FILE "elderly_data.txt"
#define MAX_DOCTORS 10

/* ================= ENUM ================= */
typedef enum {
    LIFETIME = 1,
    TEMPORARY
} NeedType;

/* ================= DOCTOR ================= */
typedef struct {
    int doctorID;
    char name[50];
    char specialization[50];
    char address[100];
} Doctor;

/* ================= WEARABLE DATA ================= */
typedef struct {
    int heartRate;
    int steps;
    int fallDetected;
    int exists; // 0 = no wearable, 1 = has wearable
} WearableData;

/* ================= EHR DATA ================= */
typedef struct {
    char disease[100];
    char medication[100];
    Doctor doctor;
    NeedType diseaseType;
} EHRData;

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
    int duration;
    char durationUnit[10];   // Days / Months / Years
    int helperID;
    char helperName[50];
    char helperSkill[50];
    char helperContact[20];
    char helperAddress[100];
} Need;

/* ================= ELDER ================= */
typedef struct Elder {
    int id;
    char name[50];
    int age;
    char address[120];
    char contact[20];
    int hasPhone;
    int hasFamily;
    WearableData wearable;
    EHRData ehr;
    Need needs[MAX_NEEDS];
    int needCount;
    int communitySupport;
    struct Elder *next;
} Elder;

/* ================= GLOBAL ================= */
Elder *head = NULL;
int nextElderID = 1001;
Helper helperQueue[MAX_HELPERS];
int front = 0, helperCount = 0;
Doctor doctorList[MAX_DOCTORS];

/* ================================================= */
/*               HELPER MANAGEMENT                   */
/* ================================================= */
void generateHelpers() {
    char *names[] = {"Ramesh","Suresh","Anita","Meena","Rahul",
                     "Kavya","Arun","Divya","Vikram","Pooja"};
    char *skills[] = {"Nurse","Caretaker","Physiotherapist","Medical Helper","Wheelchair Assistant"};

    for (int i = 0; i < MAX_HELPERS; i++) {
        helperQueue[i].helperID = 5000 + i;
        sprintf(helperQueue[i].name,"%s_%d",names[i%10],i);
        strcpy(helperQueue[i].skills,skills[i%5]);
        sprintf(helperQueue[i].contact,"9%09d",100000000+i);
        sprintf(helperQueue[i].address,"Community Area %d, City",i+1);
        helperCount++;
    }
}

Helper assignHelperBySkill(char *skill) {
    if(helperCount <= 0) {
        Helper empty = {0,"N/A","N/A","N/A","N/A"};
        return empty;
    }
    Helper h = helperQueue[front];
    front = (front + 1) % MAX_HELPERS;
    helperCount--;
    return h;
}

/* ================================================= */
/*                DOCTOR MANAGEMENT                  */
/* ================================================= */
void generateDoctors() {
    char *names[] = {"Dr. Kumar","Dr. Sharma","Dr. Reddy","Dr. Mehta","Dr. Singh",
                     "Dr. Verma","Dr. Gupta","Dr. Iyer","Dr. Joshi","Dr. Chawla"};
    char *specializations[] = {"Infectious","Chronic","Deficiency","Genetic","Physiological",
                               "Cardiology","Neurology","Orthopedic","Pulmonology","General"};
    char *addresses[] = {"City Hospital 1","City Hospital 2","Clinic A","Clinic B","Community Center",
                         "Health Center 1","Health Center 2","Hospital X","Hospital Y","Clinic Z"};

    for(int i=0;i<MAX_DOCTORS;i++){
        doctorList[i].doctorID = 100+i;
        strcpy(doctorList[i].name,names[i]);
        strcpy(doctorList[i].specialization,specializations[i]);
        strcpy(doctorList[i].address,addresses[i]);
    }
}

Doctor assignDoctor(char *diseaseCategory){
    for(int i=0;i<MAX_DOCTORS;i++){
        if(strstr(doctorList[i].specialization,diseaseCategory)!=NULL){
            return doctorList[i];
        }
    }
    return doctorList[0]; // default doctor
}

/* ================================================= */
/*               FILE SAVE FUNCTION                  */
/* ================================================= */
void saveToFile(Elder *e) {
    FILE *fp = fopen(DATA_FILE,"a");
    if(!fp) { printf("Error opening file.\n"); return; }

    fprintf(fp,"ID:%d|Name:%s|Age:%d|Address:%s|Contact:%s|HasPhone:%d|HasFamily:%d|CommunitySupport:%d\n",
            e->id,e->name,e->age,e->address,e->contact,e->hasPhone,e->hasFamily,e->communitySupport);

    fprintf(fp,"Wearable:%d|HeartRate:%d|Steps:%d|FallDetected:%d\n",
            e->wearable.exists,e->wearable.heartRate,e->wearable.steps,e->wearable.fallDetected);

    fprintf(fp,"EHR:Disease:%s|Medication:%s|DoctorID:%d|DoctorName:%s|Specialization:%s|DoctorAddress:%s|Type:%d\n",
            e->ehr.disease,e->ehr.medication,
            e->ehr.doctor.doctorID,e->ehr.doctor.name,e->ehr.doctor.specialization,e->ehr.doctor.address,
            e->ehr.diseaseType);

    for(int i=0;i<e->needCount;i++){
        Need n = e->needs[i];
        fprintf(fp,"Need:%s|Type:%d|Duration:%d|Unit:%s|Helper:%s|Skill:%s|Contact:%s|Address:%s\n",
                n.needName,n.type,n.duration,n.durationUnit,
                n.helperName,n.helperSkill,n.helperContact,n.helperAddress);
    }
    fprintf(fp,"--------------------------------------------\n");

    fclose(fp);
}

/* ================================================= */
/*               DATA COLLECTION                     */
/* ================================================= */
void collectWearableData(WearableData *w) {
    printf("Does the elder have a wearable device? (1-Yes 0-No): ");
    scanf("%d",&w->exists);
    if(w->exists){
        printf("Enter Heart Rate: "); scanf("%d",&w->heartRate);
        printf("Enter Steps Count: "); scanf("%d",&w->steps);
        printf("Fall Detected (1-Yes 0-No): "); scanf("%d",&w->fallDetected);
    }
}

void collectEHRData(EHRData *e) {
    int typeChoice;
    printf("\nSelect Disease Type:\n1.Lifetime\n2.Temporary\nChoice: ");
    scanf("%d",&typeChoice);
    e->diseaseType = (typeChoice==1)?LIFETIME:TEMPORARY;

    printf("\nSelect Disease Category:\n");
    printf("1.Infectious\n2.Chronic\n3.Deficiency\n4.Genetic\n5.Physiological\nChoice: ");
    int catChoice; scanf("%d",&catChoice);

    char *categories[] = {"","Infectious","Chronic","Deficiency","Genetic","Physiological"};
    char *diseases[6][5] = {
        {} , {"COVID-19","Flu","TB","Malaria","Hepatitis"},
        {"Cancer","Diabetes","Heart Disease","Asthma","Arthritis"},
        {"Scurvy","Goitre","Kwashiorkor","Iron Deficiency","Vitamin D Deficiency"},
        {"Cystic Fibrosis","Huntington's","Hemophilia","Sickle Cell","PKU"},
        {"Lupus","Allergies","Stroke","Injuries","Autoimmune"}
    };
    char *medications[6][5] = {
        {} , {"Antivirals","Rest","Antibiotics","Antimalarial","Vaccines"},
        {"Chemotherapy","Insulin","Heart Medication","Inhaler","Pain Relief"},
        {"Vitamin C","Iodine","Protein Supplement","Iron Supplement","Vitamin D"},
        {"Specialist Care","Gene Therapy","Blood transfusion","Supportive Care","Diet"},
        {"Immune Therapy","Antihistamines","Rehabilitation","Physiotherapy","Medication"}
    };

    int diseaseIndex = rand()%5;
    strcpy(e->disease,diseases[catChoice][diseaseIndex]);
    strcpy(e->medication,medications[catChoice][diseaseIndex]);

    e->doctor = assignDoctor(categories[catChoice]);
}

/* ================================================= */
/*               ELDER MANAGEMENT                    */
/* ================================================= */
Elder* createElder() {
    Elder *e = (Elder*)malloc(sizeof(Elder));
    e->id = nextElderID++;
    e->needCount = 0;
    e->next = NULL;

    printf("Enter Name: "); scanf(" %[^\n]",e->name);
    printf("Enter Age: "); scanf("%d",&e->age);
    printf("Enter Address: "); scanf(" %[^\n]",e->address);
    printf("Does the elder have a phone? (1-Yes 0-No): "); scanf("%d",&e->hasPhone);
    printf("Does the elder have family support? (1-Yes 0-No): "); scanf("%d",&e->hasFamily);
    printf("Enter Contact (if available, else N/A): "); scanf(" %[^\n]",e->contact);

    collectWearableData(&e->wearable);
    collectEHRData(&e->ehr);

    // Community support if no phone, wearable, or family
    if(e->hasPhone == 0 && e->wearable.exists == 0 && e->hasFamily == 0){
        e->communitySupport = 1;
        Helper h = assignHelperBySkill("Medical Helper");
        strcpy(e->needs[e->needCount].needName,"Community Monitoring");
        e->needs[e->needCount].type = TEMPORARY;
        strcpy(e->needs[e->needCount].durationUnit,"Continuous");
        e->needs[e->needCount].duration = 0;
        e->needs[e->needCount].helperID = h.helperID;
        strcpy(e->needs[e->needCount].helperName,h.name);
        strcpy(e->needs[e->needCount].helperSkill,h.skills);
        strcpy(e->needs[e->needCount].helperContact,h.contact);
        strcpy(e->needs[e->needCount].helperAddress,h.address);
        e->needCount++;
    } else e->communitySupport = 0;

    saveToFile(e); // Save immediately

    return e;
}

void addElder() {
    Elder *e = createElder();
    e->next = head;
    head = e;
    printf("Elder Added and Saved to file (ID=%d)\n",e->id);
}

/* ================================================= */
/*               NEED MANAGEMENT                     */
/* ================================================= */
void addNeed(Elder *e) {
    if(e->needCount>=MAX_NEEDS){ printf("Maximum needs reached.\n"); return; }

    Need *n = &e->needs[e->needCount];

    printf("Enter Need Name: ");
    scanf(" %[^\n]",n->needName);

    printf("Select Need Type:\n1.Lifetime\n2.Temporary\nChoice: ");
    int typeChoice; scanf("%d",&typeChoice);
    n->type = (typeChoice==1)?LIFETIME:TEMPORARY;

    if(n->type==TEMPORARY){
        printf("Enter Duration Unit (Days/Months/Years): "); scanf(" %[^\n]",n->durationUnit);
        printf("Enter Duration Value: "); scanf("%d",&n->duration);
    } else {
        strcpy(n->durationUnit,"Lifetime"); n->duration=0;
    }

    Helper h = assignHelperBySkill("Nurse");
    n->helperID = h.helperID;
    strcpy(n->helperName,h.name);
    strcpy(n->helperSkill,h.skills);
    strcpy(n->helperContact,h.contact);
    strcpy(n->helperAddress,h.address);

    e->needCount++;
    saveToFile(e);
    printf("Need added and saved successfully.\n");
}

/* ================================================= */
/*               DISPLAY                             */
/* ================================================= */
void displayAll() {
    Elder *cur = head;
    while(cur){
        printf("\nID:%d | Name:%s | Age:%d\n",cur->id,cur->name,cur->age);
        printf("Phone:%s | Family:%s | Community Support:%s\n",
               cur->hasPhone?"Yes":"No", cur->hasFamily?"Yes":"No",
               cur->communitySupport?"Yes":"No");
        printf("Doctor:%s (%s) | Address:%s | ID:%d\n",
               cur->ehr.doctor.name,cur->ehr.doctor.specialization,
               cur->ehr.doctor.address,cur->ehr.doctor.doctorID);
        printf("Disease:%s | Medication:%s | Type:%s\n",
               cur->ehr.disease,cur->ehr.medication,
               cur->ehr.diseaseType==LIFETIME?"Lifetime":"Temporary");

        for(int i=0;i<cur->needCount;i++){
            Need n = cur->needs[i];
            printf("Need:%s | %d %s | Helper:%s (%s)\n",
                   n.needName,n.duration,n.durationUnit,n.helperName,n.helperSkill);
        }
        cur = cur->next;
    }
}

/* ================================================= */
/*               MAIN                                */
/* ================================================= */
int main(){
    srand(time(NULL));
    generateHelpers();
    generateDoctors();

    int ch;
    while(1){
        printf("\n1.Add Elder\n2.Add Need\n3.Display\n4.Exit\nChoice: ");
        scanf("%d",&ch);
        switch(ch){
            case 1: addElder(); break;
            case 2:
                printf("Enter Elder ID: "); int id; scanf("%d",&id);
                {
                    Elder *e = head;
                    while(e && e->id!=id) e=e->next;
                    if(e) addNeed(e);
                    else printf("Elder not found.\n");
                }
                break;
            case 3: displayAll(); break;
            case 4: exit(0);
            default: printf("Invalid choice\n");
        }
    }
}
