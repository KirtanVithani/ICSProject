#include "hospital.h"

/* Helper: Read and authenticate user from users.txt */
int login(const char *role){
    char user[64], pass[64];
    printf("\n%s Login\n", role);
    printf("Username: ");
    if(scanf("%63s", user)!=1) return 0;
    printf("Password: ");
    if(scanf("%63s", pass)!=1) return 0;
    
    FILE *f = fopen("users.txt", "r");
    if(!f) return 0;
    
    char line[256], r[32], u[64], p[64];
    while(fgets(line, sizeof(line), f)){
        if(line[0]=='#' || line[0]=='\n') continue;
        if(sscanf(line, "%31[^|]|%63[^|]|%63[^|]", r, u, p)==3){
            if(strcmp(r, role)==0 && strcmp(u, user)==0 && strcmp(p, pass)==0){
                fclose(f);
                return 1;
            }
        }
    }
    fclose(f);
    printf("Invalid credentials!\n");
    return 0;
}

/* Helper: Add patient to patients.txt */
void addPatient(){
    FILE *f = fopen("patients.txt", "a");
    if(!f){ printf("Error opening patients.txt\n"); return; }
    
    char id[20], name[64], age[10], gender[10], symptoms[256], diseases[256], doctorId[20];
    printf("\nAdd New Patient\n");
    printf("Patient ID: "); scanf("%19s", id);
    printf("Name: "); scanf("%63s", name);
    printf("Age: "); scanf("%9s", age);
    printf("Gender: "); scanf("%9s", gender);
    printf("Symptoms: "); scanf(" %255[^\n]", symptoms);
    printf("Diseases: "); scanf(" %255[^\n]", diseases);
    printf("Assigned Doctor ID: "); scanf("%19s", doctorId);
    
    fprintf(f, "%s|%s|%s|%s|%s|%s|%s\n", id, name, age, gender, symptoms, diseases, doctorId);
    fclose(f);
    printf("Patient added successfully!\n");
}

/* Helper: View all patients from patients.txt */
void viewPatients(){
    FILE *f = fopen("patients.txt", "r");
    if(!f){ printf("No patients found.\n"); return; }
    
    printf("\n=== All Patients ===\n");
    printf("ID\tName\t\tAge\tGender\tSymptoms\t\tDiseases\t\tDoctor ID\n");
    printf("--------------------------------------------------------------------------------------------\n");
    
    char line[512];
    while(fgets(line, sizeof(line), f)){
        if(line[0]=='#' || line[0]=='\n') continue;
        char id[20], name[64], age[10], gender[10], symptoms[256], diseases[256], doctorId[20];
        if(sscanf(line, "%19[^|]|%63[^|]|%9[^|]|%9[^|]|%255[^|]|%255[^|]|%19[^\n]", 
                  id, name, age, gender, symptoms, diseases, doctorId)==7){
            printf("%s\t%s\t\t%s\t%s\t%s\t%s\t%s\n", id, name, age, gender, symptoms, diseases, doctorId);
        }
    }
    fclose(f);
}

/* Helper: Schedule appointment */
void scheduleAppointment(){
    FILE *f = fopen("appointments.txt", "a");
    if(!f){ printf("Error opening appointments.txt\n"); return; }
    
    char apptId[20], patientId[20], date[20], time[20], doctor[64];
    printf("\nSchedule Appointment\n");
    printf("Appointment ID: "); scanf("%19s", apptId);
    printf("Patient ID: "); scanf("%19s", patientId);
    printf("Date (YYYY-MM-DD): "); scanf("%19s", date);
    printf("Time (HH:MM): "); scanf("%19s", time);
    printf("Doctor Name: "); scanf("%63s", doctor);
    
    fprintf(f, "%s|%s|%s|%s|%s|scheduled\n", apptId, patientId, date, time, doctor);
    fclose(f);
    printf("Appointment scheduled successfully!\n");
}

/* Helper: View appointments */
void viewAppointments(const char *patientId){
    FILE *f = fopen("appointments.txt", "r");
    if(!f){ printf("No appointments found.\n"); return; }
    
    printf("\n=== Appointments");
    if(patientId) printf(" for Patient %s", patientId);
    printf(" ===\n");
    printf("ID\tPatient\tDate\t\tTime\tDoctor\t\tStatus\n");
    printf("----------------------------------------------------------------\n");
    
    char line[256];
    int found = 0;
    while(fgets(line, sizeof(line), f)){
        if(line[0]=='#' || line[0]=='\n') continue;
        char id[20], pid[20], date[20], time[20], doc[64], status[20];
        if(sscanf(line, "%19[^|]|%19[^|]|%19[^|]|%19[^|]|%63[^|]|%19[^\n]", 
                  id, pid, date, time, doc, status)==6){
            if(!patientId || strcmp(pid, patientId)==0){
                printf("%s\t%s\t%s\t%s\t%s\t%s\n", id, pid, date, time, doc, status);
                found = 1;
            }
        }
    }
    if(!found) printf("No appointments found.\n");
    fclose(f);
}

/* Helper: Record visit */
void recordVisit(){
    FILE *f = fopen("visits.txt", "a");
    if(!f){ printf("Error opening visits.txt\n"); return; }
    
    char visitId[20], patientId[20], date[20], diagnosis[128], treatment[128];
    printf("\nRecord Visit\n");
    printf("Visit ID: "); scanf("%19s", visitId);
    printf("Patient ID: "); scanf("%19s", patientId);
    printf("Date (YYYY-MM-DD): "); scanf("%19s", date);
    printf("Diagnosis: "); scanf(" %127[^\n]", diagnosis);
    printf("Treatment: "); scanf(" %127[^\n]", treatment);
    
    fprintf(f, "%s|%s|%s|%s|%s\n", visitId, patientId, date, diagnosis, treatment);
    fclose(f);
    printf("Visit recorded successfully!\n");
}

/* Doctor menu with full functionality */
void doctorMenu(void){
    int ch;
    do{
        printf("\n=== Doctor Menu ===\n");
        printf("1. View All Patients\n");
        printf("2. Add New Patient\n");
        printf("3. Schedule Appointment\n");
        printf("4. View All Appointments\n");
        printf("5. Record Visit\n");
        printf("6. View All Visits\n");
        printf("7. Logout\n");
        printf("Choice: ");
        scanf("%d", &ch);
        
        switch(ch){
            case 1: viewPatients(); break;
            case 2: addPatient(); break;
            case 3: scheduleAppointment(); break;
            case 4: viewAppointments(NULL); break;
            case 5: recordVisit(); break;
            case 6: viewAllVisits(); break;
            case 7: printf("Logging out...\n"); break;
            default: printf("Invalid choice!\n");
        }
    }while(ch!=7);
}

/* Patient menu with full functionality */
void patientMenu(const char *id){
    int ch;
    do{
        printf("\n=== Patient Menu (ID: %s) ===\n", id);
        printf("1. View My Appointments\n");
        printf("2. View My Visit History\n");
        printf("3. Logout\n");
        printf("Choice: ");
        scanf("%d", &ch);
        
        switch(ch){
            case 1: viewAppointments(id); break;
            case 2: {
                FILE *f = fopen("visits.txt", "r");
                if(!f){ printf("No visits found.\n"); break; }
                printf("\n=== Visit History for Patient %s ===\n", id);
                printf("Visit ID\tDate\t\tDiagnosis\t\tTreatment\n");
                printf("----------------------------------------------------------------\n");
                char line[256];
                int found = 0;
                while(fgets(line, sizeof(line), f)){
                    if(line[0]=='#' || line[0]=='\n') continue;
                    char vid[20], pid[20], date[20], diag[128], treat[128];
                    if(sscanf(line, "%19[^|]|%19[^|]|%19[^|]|%127[^|]|%127[^\n]", 
                              vid, pid, date, diag, treat)==5){
                        if(strcmp(pid, id)==0){
                            printf("%s\t%s\t%s\t%s\n", vid, date, diag, treat);
                            found = 1;
                        }
                    }
                }
                if(!found) printf("No visit history found.\n");
                fclose(f);
                break;
            }
            case 3: printf("Logging out...\n"); break;
            default: printf("Invalid choice!\n");
        }
    }while(ch!=3);
}

/* View all visits */
void viewAllVisits(void){
    FILE *f = fopen("visits.txt", "r");
    if(!f){ printf("No visits recorded yet.\n"); return; }
    
    printf("\n=== All Visits ===\n");
    printf("Visit ID\tPatient ID\tDate\t\tDiagnosis\t\tTreatment\n");
    printf("------------------------------------------------------------------------\n");
    
    char line[256];
    int found = 0;
    while(fgets(line, sizeof(line), f)){
        if(line[0]=='#' || line[0]=='\n') continue;
        char vid[20], pid[20], date[20], diag[128], treat[128];
        if(sscanf(line, "%19[^|]|%19[^|]|%19[^|]|%127[^|]|%127[^\n]", 
                  vid, pid, date, diag, treat)==5){
            printf("%s\t%s\t\t%s\t%s\t%s\n", vid, pid, date, diag, treat);
            found = 1;
        }
    }
    if(!found) printf("No visits found.\n");
    fclose(f);
}

int main(){
    int ch;
    do{
        printf("\n=== Hospital Management System ===\n1.Doctor Login\n2.Patient Login\n3.View All Visits\n4.Exit\nChoice:");
        scanf("%d",&ch);
        if(ch==1 && login("doctor")) doctorMenu();
        else if(ch==2 && login("patient")){
            char id[20]; printf("Enter your Patient ID: "); scanf("%s",id);
            patientMenu(id);
        }
        else if(ch==3) viewAllVisits();
    }while(ch!=4);
    return 0;
}
