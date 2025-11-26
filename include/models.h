#ifndef MODELS_H
#define MODELS_H
#define MAX_ITEMS 256
typedef struct { int id; char name[64]; } Student;
typedef struct { int id; char code[16]; char title[64]; int capacity; } Course;
typedef struct { int sid; int cid; } Enroll;
extern Student students[MAX_ITEMS]; extern int scnt;
extern Course courses[MAX_ITEMS];   extern int ccnt;
extern Enroll enrolls[MAX_ITEMS];   extern int ecnt;
#endif
