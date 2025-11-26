#include <stdio.h>
#include <string.h>
#include "admin.h"
#include "fileio.h"

void admin_add_student() {
    if(scnt >= MAX_ITEMS) { printf("student storage full\n"); return; }
    Student s; s.id = next_id();
    printf("New student id %d\nName: ", s.id); fgets(s.name,64,stdin); s.name[strcspn(s.name,"\r\n")]=0;
    students[scnt++]=s; save_all("data"); printf("Student added.\n");
}

void admin_add_course() {
    if(ccnt >= MAX_ITEMS) { printf("course storage full\n"); return; }
    Course c; c.id = next_id();
    printf("New course id %d\nCode: ", c.id); fgets(c.code,16,stdin); c.code[strcspn(c.code,"\r\n")]=0;
    printf("Title: "); fgets(c.title,64,stdin); c.title[strcspn(c.title,"\r\n")]=0;
    printf("Capacity: "); scanf("%d",&c.capacity); getchar();
    courses[ccnt++]=c; save_all("data"); printf("Course added.\n");
}

void admin_list_all() {
    printf("Students:\n");
    for(int i=0;i<scnt;i++) printf(" %d: %s\n", students[i].id, students[i].name);
    printf("Courses:\n");
    for(int i=0;i<ccnt;i++){
        int enrolled=0; for(int j=0;j<ecnt;j++) if(enrolls[j].cid==courses[i].id) enrolled++;
        printf(" %d: %s - %s | cap %d | enrolled %d\n", courses[i].id, courses[i].code, courses[i].title, courses[i].capacity, enrolled);
    }
}
