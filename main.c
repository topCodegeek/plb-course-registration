/***************************************************************
 Course Registration System (single-file)
 - Simple CLI for Admin / Faculty / Student
 - File-based persistence using CSV files:
    students.csv, faculty.csv, courses.csv, enrollments.csv
 - Compile: gcc -o course_reg_system course_reg_system.c
 - Run: ./course_reg_system
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 500
#define MAX_FACULTY 100
#define MAX_COURSES 200
#define MAX_ENROLL 2000
#define LINE 256

/* Data structures */
typedef struct {
    int id;
    char name[80];
    char dob[16];
    char program[64];
} Student;

typedef struct {
    int id;
    char name[80];
    char dept[64];
} Faculty;

typedef struct {
    int id;
    char code[16];     // e.g., CS101
    char title[80];
    int faculty_id;    // which faculty teaches
    int capacity;
    int credits;
} Course;

typedef struct {
    int student_id;
    int course_id;
    int grade;          // -1 if not assigned, else 0-100
    int attendance;     // count of attended sessions
} Enrollment;

/* Global in-memory arrays */
Student students[MAX_STUDENTS];
int student_count = 0;

Faculty faculty[MAX_FACULTY];
int faculty_count = 0;

Course courses[MAX_COURSES];
int course_count = 0;

Enrollment enrolls[MAX_ENROLL];
int enroll_count = 0;

/* File helpers */
void strip_newline(char *s) {
    size_t n = strlen(s);
    if (n && s[n-1] == '\n') s[n-1] = '\0';
}

/* Load/Save CSV functions */
void load_students(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return;
    char line[LINE];
    while (fgets(line, LINE, f)) {
        strip_newline(line);
        if (strlen(line) == 0) continue;
        Student st;
        char *tok = strtok(line, ",");
        if (!tok) continue;
        st.id = atoi(tok);
        tok = strtok(NULL, ","); strncpy(st.name, tok?tok:"", sizeof(st.name));
        tok = strtok(NULL, ","); strncpy(st.dob, tok?tok:"", sizeof(st.dob));
        tok = strtok(NULL, ","); strncpy(st.program, tok?tok:"", sizeof(st.program));
        students[student_count++] = st;
        if (student_count >= MAX_STUDENTS) break;
    }
    fclose(f);
}

void save_students(const char *path) {
    FILE *f = fopen(path, "w");
    if (!f) { perror("save_students"); return; }
    for (int i=0;i<student_count;i++) {
        fprintf(f, "%d,%s,%s,%s\n", students[i].id, students[i].name, students[i].dob, students[i].program);
    }
    fclose(f);
}

void load_faculty(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return;
    char line[LINE];
    while (fgets(line, LINE, f)) {
        strip_newline(line);
        if (strlen(line) == 0) continue;
        Faculty fc;
        char *tok = strtok(line, ",");
        if (!tok) continue;
        fc.id = atoi(tok);
        tok = strtok(NULL, ","); strncpy(fc.name, tok?tok:"", sizeof(fc.name));
        tok = strtok(NULL, ","); strncpy(fc.dept, tok?tok:"", sizeof(fc.dept));
        faculty[faculty_count++] = fc;
        if (faculty_count >= MAX_FACULTY) break;
    }
    fclose(f);
}

void save_faculty(const char *path) {
    FILE *f = fopen(path, "w");
    if (!f) { perror("save_faculty"); return; }
    for (int i=0;i<faculty_count;i++) {
        fprintf(f, "%d,%s,%s\n", faculty[i].id, faculty[i].name, faculty[i].dept);
    }
    fclose(f);
}

void load_courses(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return;
    char line[LINE];
    while (fgets(line, LINE, f)) {
        strip_newline(line);
        if (strlen(line)==0) continue;
        Course c;
        char *tok = strtok(line, ",");
        if (!tok) continue;
        c.id = atoi(tok);
        tok = strtok(NULL, ","); strncpy(c.code, tok?tok:"", sizeof(c.code));
        tok = strtok(NULL, ","); strncpy(c.title, tok?tok:"", sizeof(c.title));
        tok = strtok(NULL, ","); c.faculty_id = tok?atoi(tok):0;
        tok = strtok(NULL, ","); c.capacity = tok?atoi(tok):0;
        tok = strtok(NULL, ","); c.credits = tok?atoi(tok):0;
        courses[course_count++] = c;
        if (course_count >= MAX_COURSES) break;
    }
    fclose(f);
}

void save_courses(const char *path) {
    FILE *f = fopen(path, "w");
    if (!f) { perror("save_courses"); return; }
    for (int i=0;i<course_count;i++) {
        fprintf(f, "%d,%s,%s,%d,%d,%d\n",
            courses[i].id, courses[i].code, courses[i].title,
            courses[i].faculty_id, courses[i].capacity, courses[i].credits);
    }
    fclose(f);
}

void load_enrollments(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return;
    char line[LINE];
    while (fgets(line, LINE, f)) {
        strip_newline(line);
        if (strlen(line)==0) continue;
        Enrollment e;
        char *tok = strtok(line, ",");
        if (!tok) continue;
        e.student_id = atoi(tok);
        tok = strtok(NULL, ","); e.course_id = tok?atoi(tok):0;
        tok = strtok(NULL, ","); e.grade = tok?atoi(tok):-1;
        tok = strtok(NULL, ","); e.attendance = tok?atoi(tok):0;
        enrolls[enroll_count++] = e;
        if (enroll_count >= MAX_ENROLL) break;
    }
    fclose(f);
}

void save_enrollments(const char *path) {
    FILE *f = fopen(path, "w");
    if (!f) { perror("save_enrollments"); return; }
    for (int i=0;i<enroll_count;i++) {
        fprintf(f, "%d,%d,%d,%d\n", enrolls[i].student_id, enrolls[i].course_id, enrolls[i].grade, enrolls[i].attendance);
    }
    fclose(f);
}

/* Utility finders */
Student* find_student_by_id(int id) {
    for (int i=0;i<student_count;i++) if (students[i].id==id) return &students[i];
    return NULL;
}
Faculty* find_faculty_by_id(int id) {
    for (int i=0;i<faculty_count;i++) if (faculty[i].id==id) return &faculty[i];
    return NULL;
}
Course* find_course_by_id(int id) {
    for (int i=0;i<course_count;i++) if (courses[i].id==id) return &courses[i];
    return NULL;
}
int count_enrolled_in_course(int course_id) {
    int c=0;
    for (int i=0;i<enroll_count;i++) if (enrolls[i].course_id==course_id) c++;
    return c;
}
int is_student_enrolled_in_course(int sid, int cid) {
    for (int i=0;i<enroll_count;i++) if (enrolls[i].student_id==sid && enrolls[i].course_id==cid) return i;
    return -1;
}

/* Admin operations */
void admin_add_student() {
    if (student_count >= MAX_STUDENTS) { printf("Student storage full.\n"); return; }
    Student s;
    printf("Enter new student id: "); scanf("%d", &s.id); getchar();
    if (find_student_by_id(s.id)) { printf("Student with id %d exists.\n", s.id); return; }
    printf("Name: "); fgets(s.name, sizeof(s.name), stdin); strip_newline(s.name);
    printf("DOB (YYYY-MM-DD): "); fgets(s.dob, sizeof(s.dob), stdin); strip_newline(s.dob);
    printf("Program: "); fgets(s.program, sizeof(s.program), stdin); strip_newline(s.program);
    students[student_count++] = s;
    save_students("students.csv");
    printf("Student added.\n");
}

void admin_add_faculty() {
    if (faculty_count >= MAX_FACULTY) { printf("Faculty storage full.\n"); return; }
    Faculty f;
    printf("Enter new faculty id: "); scanf("%d", &f.id); getchar();
    if (find_faculty_by_id(f.id)) { printf("Faculty with id %d exists.\n", f.id); return; }
    printf("Name: "); fgets(f.name, sizeof(f.name), stdin); strip_newline(f.name);
    printf("Dept: "); fgets(f.dept, sizeof(f.dept), stdin); strip_newline(f.dept);
    faculty[faculty_count++] = f;
    save_faculty("faculty.csv");
    printf("Faculty added.\n");
}

void admin_add_course() {
    if (course_count >= MAX_COURSES) { printf("Course storage full.\n"); return; }
    Course c;
    printf("Enter new course id (int): "); scanf("%d", &c.id); getchar();
    if (find_course_by_id(c.id)) { printf("Course with id %d exists.\n", c.id); return; }
    printf("Course code (e.g. CS101): "); fgets(c.code, sizeof(c.code), stdin); strip_newline(c.code);
    printf("Title: "); fgets(c.title, sizeof(c.title), stdin); strip_newline(c.title);
    printf("Faculty id (0 for unassigned): "); scanf("%d", &c.faculty_id); getchar();
    if (c.faculty_id != 0 && !find_faculty_by_id(c.faculty_id)) { printf("Warning: faculty id not found, setting to 0.\n"); c.faculty_id = 0; }
    printf("Capacity (max students): "); scanf("%d", &c.capacity); getchar();
    printf("Credits: "); scanf("%d", &c.credits); getchar();
    courses[course_count++] = c;
    save_courses("courses.csv");
    printf("Course added.\n");
}

void admin_list_all() {
    printf("Students:\n");
    for (int i=0;i<student_count;i++) printf("  %d: %s | %s | %s\n", students[i].id, students[i].name, students[i].dob, students[i].program);
    printf("\nFaculty:\n");
    for (int i=0;i<faculty_count;i++) printf("  %d: %s | %s\n", faculty[i].id, faculty[i].name, faculty[i].dept);
    printf("\nCourses:\n");
    for (int i=0;i<course_count;i++) {
        Course *c = &courses[i];
        Faculty *fc = find_faculty_by_id(c->faculty_id);
        printf("  %d: %s - %s | taught by: %s (%d) | cap %d | credits %d | enrolled %d\n",
            c->id, c->code, c->title, fc?fc->name:"<unassigned>", c->faculty_id, c->capacity, c->credits, count_enrolled_in_course(c->id));
    }
    printf("\nEnrollments: %d\n", enroll_count);
}

/* Student operations */
void student_view_available_courses() {
    printf("Available courses:\n");
    for (int i=0;i<course_count;i++) {
        Course *c = &courses[i];
        printf(" %d | %s - %s | cap %d | enrolled %d | credits %d\n",
            c->id, c->code, c->title, c->capacity, count_enrolled_in_course(c->id), c->credits);
    }
}

void student_enroll(int sid) {
    student_view_available_courses();
    printf("Enter course id to enroll: ");
    int cid; scanf("%d", &cid); getchar();
    Course *c = find_course_by_id(cid);
    if (!c) { printf("Course not found.\n"); return; }
    if (is_student_enrolled_in_course(sid, cid) >=0) { printf("Already enrolled.\n"); return; }
    int enrolled = count_enrolled_in_course(cid);
    if (enrolled >= c->capacity) { printf("Course full (capacity %d).\n", c->capacity); return; }
    if (enroll_count >= MAX_ENROLL) { printf("Enrollment storage full.\n"); return; }
    Enrollment e = { .student_id = sid, .course_id = cid, .grade = -1, .attendance = 0 };
    enrolls[enroll_count++] = e;
    save_enrollments("enrollments.csv");
    printf("Enrolled successfully.\n");
}

void student_drop(int sid) {
    printf("Enter course id to drop: ");
    int cid; scanf("%d", &cid); getchar();
    int idx = is_student_enrolled_in_course(sid, cid);
    if (idx < 0) { printf("Not enrolled in that course.\n"); return; }
    // remove by shifting
    for (int i=idx;i<enroll_count-1;i++) enrolls[i] = enrolls[i+1];
    enroll_count--;
    save_enrollments("enrollments.csv");
    printf("Dropped successfully.\n");
}

void student_view_schedule(int sid) {
    printf("Your courses:\n");
    for (int i=0;i<enroll_count;i++) if (enrolls[i].student_id == sid) {
        Course *c = find_course_by_id(enrolls[i].course_id);
        Faculty *f = c?find_faculty_by_id(c->faculty_id):NULL;
        printf(" %d | %s - %s | Faculty: %s | Grade: %s | Attendance: %d\n",
            c?c->id:0, c?c->code:"?", c?c->title:"?", f?f->name:"<unassigned>",
            (enrolls[i].grade>=0? ({ static char gbuf[8]; sprintf(gbuf, "%d", enrolls[i].grade); gbuf; }) : "N/A"),
            enrolls[i].attendance);
    }
}

/* Faculty operations */
void faculty_view_my_courses(int fid) {
    printf("Courses taught by faculty %d:\n", fid);
    for (int i=0;i<course_count;i++) if (courses[i].faculty_id == fid) {
        Course *c = &courses[i];
        printf(" %d | %s - %s | cap %d | enrolled %d\n", c->id, c->code, c->title, c->capacity, count_enrolled_in_course(c->id));
    }
}

void faculty_take_attendance(int fid) {
    faculty_view_my_courses(fid);
    printf("Enter course id to take attendance: "); int cid; scanf("%d", &cid); getchar();
    Course *c = find_course_by_id(cid);
    if (!c || c->faculty_id != fid) { printf("Invalid course or not your course.\n"); return; }
    printf("Marking attendance: enter student ids present, -1 to stop.\n");
    while (1) {
        printf("Student id (or -1): ");
        int sid; scanf("%d", &sid); getchar();
        if (sid == -1) break;
        int idx = is_student_enrolled_in_course(sid, cid);
        if (idx >= 0) { enrolls[idx].attendance += 1; printf("Marked present for %d\n", sid); }
        else printf("Not enrolled: %d\n", sid);
    }
    save_enrollments("enrollments.csv");
    printf("Attendance updated.\n");
}

void faculty_assign_grade(int fid) {
    faculty_view_my_courses(fid);
    printf("Enter course id to grade: "); int cid; scanf("%d", &cid); getchar();
    Course *c = find_course_by_id(cid);
    if (!c || c->faculty_id != fid) { printf("Invalid course or not your course.\n"); return; }
    printf("Enter student id to grade (or -1 to stop):\n");
    while (1) {
        printf("Student id (or -1): ");
        int sid; scanf("%d", &sid); getchar();
        if (sid == -1) break;
        int idx = is_student_enrolled_in_course(sid, cid);
        if (idx < 0) { printf("Not enrolled.\n"); continue; }
        printf("Enter grade (0-100): "); int g; scanf("%d", &g); getchar();
        if (g < 0 || g > 100) { printf("Invalid grade.\n"); continue; }
        enrolls[idx].grade = g;
        printf("Grade set for %d in %s: %d\n", sid, c->code, g);
    }
    save_enrollments("enrollments.csv");
}

/* Simple login menus (no auth, choose id) */
void student_menu(int sid) {
    Student *s = find_student_by_id(sid);
    if (!s) { printf("Student not found.\n"); return; }
    while (1) {
        printf("\nStudent: %s (id %d)\n1) View available courses\n2) Enroll in course\n3) Drop course\n4) View my schedule & grades\n0) Logout\nChoice: ", s->name, s->id);
        int ch; scanf("%d", &ch); getchar();
        if (ch==0) break;
        switch(ch){
            case 1: student_view_available_courses(); break;
            case 2: student_enroll(sid); break;
            case 3: student_drop(sid); break;
            case 4: student_view_schedule(sid); break;
            default: printf("Invalid.\n");
        }
    }
}

void faculty_menu(int fid) {
    Faculty *f = find_faculty_by_id(fid);
    if (!f) { printf("Faculty not found.\n"); return; }
    while (1) {
        printf("\nFaculty: %s (id %d)\n1) View my courses\n2) Take attendance\n3) Assign/update grades\n0) Logout\nChoice: ", f->name, f->id);
        int ch; scanf("%d", &ch); getchar();
        if (ch==0) break;
        switch(ch){
            case 1: faculty_view_my_courses(fid); break;
            case 2: faculty_take_attendance(fid); break;
            case 3: faculty_assign_grade(fid); break;
            default: printf("Invalid.\n");
        }
    }
}

void admin_menu() {
    while (1) {
        printf("\nAdmin Menu\n1) Add student\n2) Add faculty\n3) Add course\n4) List all\n0) Logout\nChoice: ");
        int ch; scanf("%d", &ch); getchar();
        if (ch==0) break;
        switch(ch){
            case 1: admin_add_student(); break;
            case 2: admin_add_faculty(); break;
            case 3: admin_add_course(); break;
            case 4: admin_list_all(); break;
            default: printf("Invalid.\n");
        }
    }
}

/* Small helper to seed default data if files missing */
void seed_demo_data_if_empty() {
    if (student_count==0) {
        Student s1 = {1001, "Alice Kumar", "2002-05-10", "B.Tech CSE"};
        Student s2 = {1002, "Rahul Sharma", "2001-11-02", "B.Tech ECE"};
        students[student_count++] = s1; students[student_count++] = s2;
        save_students("students.csv");
    }
    if (faculty_count==0) {
        Faculty f1 = {2001, "Dr. Priya N", "Computer Science"};
        Faculty f2 = {2002, "Dr. Amit Singh", "Electronics"};
        faculty[faculty_count++] = f1; faculty[faculty_count++] = f2;
        save_faculty("faculty.csv");
    }
    if (course_count==0) {
        Course c1 = {3001, "CS101", "Intro to Programming", 2001, 30, 4};
        Course c2 = {3002, "EC201", "Signals & Systems", 2002, 25, 3};
        courses[course_count++] = c1; courses[course_count++] = c2;
        save_courses("courses.csv");
    }
}

/* Main */
int main() {
    /* Load data files */
    load_students("students.csv");
    load_faculty("faculty.csv");
    load_courses("courses.csv");
    load_enrollments("enrollments.csv");

    /* If empty, create small demo */
    seed_demo_data_if_empty();

    printf("Course Registration System\n");
    while (1) {
        printf("\nLogin as:\n1) Admin\n2) Faculty\n3) Student\n0) Exit\nChoice: ");
        int role; scanf("%d", &role); getchar();
        if (role == 0) break;
        if (role == 1) {
            admin_menu();
        } else if (role == 2) {
            printf("Enter your faculty id: ");
            int fid; scanf("%d", &fid); getchar();
            faculty_menu(fid);
        } else if (role == 3) {
            printf("Enter your student id: ");
            int sid; scanf("%d", &sid); getchar();
            student_menu(sid);
        } else {
            printf("Invalid choice.\n");
        }
    }

    /* Save all before exit */
    save_students("students.csv");
    save_faculty("faculty.csv");
    save_courses("courses.csv");
    save_enrollments("enrollments.csv");
    printf("Data saved. Exiting.\n");
    return 0;
}
