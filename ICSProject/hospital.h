/* hospital.h - public header for the small Hospital Management program */

#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function prototypes used by main (hospital.c) */
int login(const char *role);
void doctorMenu(void);
void patientMenu(const char *id);
void viewAllVisits(void);

#endif /* HOSPITAL_H */
