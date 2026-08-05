#ifndef ADMIN_H
#define ADMIN_H
#include "admin.h"
#include "offering.h"

typedef struct{
	int type;
	Offering offering;
	int new_capacity;
} Request;

extern int calender[4];
extern Request* requests[100];
extern int request_count;

void adminDashboard();

void adminCalender();
void adminStudents();
void adminFaculty();
void adminOfferings();

void adminRequests();
int addNewOffering(Offering offering);
int removeOffering(Offering offering);
int increaseCapacity(Offering Offering, int new_capacity);
#endif // ADMIN_H
