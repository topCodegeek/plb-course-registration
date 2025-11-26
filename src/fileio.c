#include <stdio.h>
#include <string.h>
#include "fileio.h"

// Globals
Student students[MAX_ITEMS]; int scnt = 0;
Course courses[MAX_ITEMS];   int ccnt = 0;
Enroll enrolls[MAX_ITEMS];   int ecnt = 0;

/* ---- Loader Callbacks ---- */

static void load_student_line(char *s) {
    int id;
    char name[64];
    if (sscanf(s, "%d|%63[^\n]", &id, name) == 2) {
        students[scnt].id = id;
        strncpy(students[scnt].name, name, 64);
        scnt++;
    }
}

static void load_course_line(char *s) {
    int id, cap;
    char code[16], title[64];
    if (sscanf(s, "%d|%15[^|]|%63[^|]|%d", &id, code, title, &cap) == 4) {
        courses[ccnt].id = id;
        strncpy(courses[ccnt].code, code, 16);
        strncpy(courses[ccnt].title, title, 64);
        courses[ccnt].capacity = cap;
        ccnt++;
    }
}

static void load_enroll_line(char *s) {
    int sid, cid;
    if (sscanf(s, "%d|%d", &sid, &cid) == 2) {
        enrolls[ecnt].sid = sid;
        enrolls[ecnt].cid = cid;
        ecnt++;
    }
}

/* ---- Generic line loader ---- */

static void load_simple(const char *path, void (*cb)(char*)) {
    FILE *f = fopen(path, "r");
    if (!f) return;

    char buf[256];
    while (fgets(buf, sizeof(buf), f)) {
        buf[strcspn(buf, "\r\n")] = 0;
        if (buf[0]) cb(buf);
    }
    fclose(f);
}

/* ---- Saver Callbacks ---- */

static void save_students(FILE *f) {
    for (int i = 0; i < scnt; i++)
        fprintf(f, "%d|%s\n", students[i].id, students[i].name);
}

static void save_courses(FILE *f) {
    for (int i = 0; i < ccnt; i++)
        fprintf(f, "%d|%s|%s|%d\n",
            courses[i].id,
            courses[i].code,
            courses[i].title,
            courses[i].capacity);
}

static void save_enrolls(FILE *f) {
    for (int i = 0; i < ecnt; i++)
        fprintf(f, "%d|%d\n", enrolls[i].sid, enrolls[i].cid);
}

static void save_lines(const char *path, void (*cb)(FILE*)) {
    FILE *f = fopen(path, "w");
    if (!f) return;
    cb(f);
    fclose(f);
}

/* ---- Public functions ---- */

void load_all(const char *dir) {
    char path[256];

    snprintf(path, sizeof(path), "%s/students.txt", dir);
    load_simple(path, load_student_line);

    snprintf(path, sizeof(path), "%s/courses.txt", dir);
    load_simple(path, load_course_line);

    snprintf(path, sizeof(path), "%s/enrollments.txt", dir);
    load_simple(path, load_enroll_line);
}

void save_all(const char *dir) {
    char path[256];

    snprintf(path, sizeof(path), "%s/students.txt", dir);
    save_lines(path, save_students);

    snprintf(path, sizeof(path), "%s/courses.txt", dir);
    save_lines(path, save_courses);

    snprintf(path, sizeof(path), "%s/enrollments.txt", dir);
    save_lines(path, save_enrolls);
}

int next_id() {
    int m = 1000;

    for (int i = 0; i < scnt; i++)
        if (students[i].id > m) m = students[i].id;

    for (int i = 0; i < ccnt; i++)
        if (courses[i].id > m) m = courses[i].id;

    return m + 1;
}
