#include <stdio.h>
#include <stdlib.h>
#include "fileio.h"
#include "admin.h"
#include "student.h"

int main(){
    load_all("data");
    if(scnt==0 && ccnt==0){ // seed small demo
        students[scnt++] = (Student){1001,"Alice"};
        students[scnt++] = (Student){1002,"Bob"};
        courses[ccnt++] = (Course){2001,"CS101","Intro",30};
        courses[ccnt++] = (Course){2002,"MA101","Calc",25};
        save_all("data");
    }

    while(1){
        printf("1) Admin \n2) Student \n3) Faculty \n0) Exit\nChoice: ");
        int ch; if(scanf("%d",&ch)!=1) break; getchar();
        if(ch==0) break;
        if(ch==1){
            printf("\nAdmin: \n1) Add Student \n2) Add Course \n3) List \n0) Back. \nYour choice: ");
            int a; scanf("%d",&a); getchar();
            rintf("\n");
            if(a==1) admin_add_student();
            else if(a==2) admin_add_course();
            else if(a==3) admin_list_all();
        } else if(ch==2) student_enroll_flow();
        else if(ch==3) faculty_view_courses();
    }
    save_all("data");
    printf("\nThanks for using. Have a great day!\n");
    return 0;
}
