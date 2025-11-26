#include <stdio.h>
#include <string.h>
#include "student.h"
#include "fileio.h"

void student_view_courses() {
    printf("Available courses:\n");
    for(int i=0;i<ccnt;i++){
        int enrolled=0; for(int j=0;j<ecnt;j++) if(enrolls[j].cid==courses[i].id) enrolled++;
        printf(" %d: %s - %s | %d/%d\n", courses[i].id, courses[i].code, courses[i].title, enrolled, courses[i].capacity);
    }
}

static int find_student(int id){ for(int i=0;i<scnt;i++) if(students[i].id==id) return i; return -1; }
static int find_course_idx(int cid){ for(int i=0;i<ccnt;i++) if(courses[i].id==cid) return i; return -1; }
static int is_enrolled(int sid,int cid){ for(int i=0;i<ecnt;i++) if(enrolls[i].sid==sid && enrolls[i].cid==cid) return 1; return 0; }

void student_enroll_flow() {
    int sid; printf("Enter your student id: "); scanf("%d",&sid); getchar();
    if(find_student(sid)<0){ printf("No such student\n"); return; }
    student_view_courses();
    int cid; printf("Course id to enroll: "); scanf("%d",&cid); getchar();
    if(find_course_idx(cid)<0){ printf("No such course\n"); return; }
    if(is_enrolled(sid,cid)){ printf("Already enrolled\n"); return; }
    // check capacity
    int enrolled=0; for(int i=0;i<ecnt;i++) if(enrolls[i].cid==cid) enrolled++;
    int idx=find_course_idx(cid);
    if(enrolled >= courses[idx].capacity){ printf("Course full\n"); return; }
    enrolls[ecnt++]=(Enroll){.sid=sid,.cid=cid}; save_all("data"); printf("Enrolled\n");
}

void faculty_view_courses(){ // simple read-only view
    printf("Courses (faculty view):\n");
    for(int i=0;i<ccnt;i++) printf(" %d: %s - %s\n", courses[i].id, courses[i].code, courses[i].title);
}
