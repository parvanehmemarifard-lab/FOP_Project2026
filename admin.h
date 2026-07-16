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

#endif // ADMIN_H
