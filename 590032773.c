/*
 * AI-Assisted Student Record and Result Management System
 * Course : Programming in C (PRC1001)  |  B.Tech CSE, Semester I
 * Student: Yash Pandey
 * SAP ID : 590032773
 * File   : 590032773.c
 *
 * Features: add / search / display / delete records, CGPA, year-wise
 * average CGPA, sorting by SAP ID and by Name, and a rule-based
 * "Academic Advisor" (if-else logic designed with AI assistance).
 */
 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
 
#define MAX_STUDENTS 100
#define MAX_SEM      8
#define NAME_LEN     50
#define PHONE_LEN    11
#define EMAIL_LEN    50
 
typedef struct {
    long  sapId;
    int   rollNo;
    char  name[NAME_LEN];
    char  phone[PHONE_LEN + 1];
    char  email[EMAIL_LEN];
    int   year;                 /* 1 to 4                                   */
    int   semester;             /* latest semester with a declared result   */
    float sgpa[MAX_SEM];        /* sgpa[0] = Sem 1 ... sgpa[7] = Sem 8      */
} Student;
 
Student students[MAX_STUDENTS];
int     count = 0;
 
/* ---------------------- input helpers ---------------------- */
 
void readLine(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    if (fgets(buf, size, stdin) == NULL) { buf[0] = '\0'; return; }
    buf[strcspn(buf, "\n")] = '\0';
}
 
int readInt(const char *prompt, int min, int max) {
    char line[64];
    int  value;
    while (1) {
        readLine(prompt, line, sizeof(line));
        if (sscanf(line, "%d", &value) == 1 && value >= min && value <= max)
            return value;
        printf("  Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}
 
long readLong(const char *prompt) {
    char line[64];
    long value;
    while (1) {
        readLine(prompt, line, sizeof(line));
        if (sscanf(line, "%ld", &value) == 1 && value > 0) return value;
        printf("  Invalid input. Enter a positive number.\n");
    }
}
 
float readSgpa(const char *prompt) {
    char  line[64];
    float value;
    while (1) {
        readLine(prompt, line, sizeof(line));
        if (sscanf(line, "%f", &value) == 1 && value >= 0.0f && value <= 10.0f)
            return value;
        printf("  Invalid SGPA. Enter a value between 0.0 and 10.0.\n");
    }
}
 
int isValidPhone(const char *p) {
    int i;
    if (strlen(p) != 10) return 0;
    for (i = 0; i < 10; i++) if (!isdigit((unsigned char)p[i])) return 0;
    return 1;
}
 
int isValidEmail(const char *e) {
    char *at = strchr(e, '@');
    return at != NULL && at != e && strchr(at, '.') != NULL;
}
 
/* Case-insensitive string comparison (used for sorting by name) */
int compareNames(const char *a, const char *b) {
    while (*a && *b) {
        int ca = tolower((unsigned char)*a), cb = tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        a++; b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}
 
/* ---------------------- core helpers ----------------------- */
 
int findBySap(long sap) {
    int i;
    for (i = 0; i < count; i++)
        if (students[i].sapId == sap) return i;
    return -1;
}
 
/* CGPA = average of the SGPA values available (semesters 1..semester) */
float calculateCGPA(const Student *s) {
    float sum = 0.0f;
    int   i;
    for (i = 0; i < s->semester; i++) sum += s->sgpa[i];
    return sum / s->semester;
}
 
const char *yearSuffix(int y) {
    return (y == 1) ? "st" : (y == 2) ? "nd" : (y == 3) ? "rd" : "th";
}
 
void printStudent(const Student *s) {
    int i;
    printf("\n  SAP ID   : %ld\n", s->sapId);
    printf("  Roll No. : %d\n", s->rollNo);
    printf("  Name     : %s\n", s->name);
    printf("  Phone    : %s\n", s->phone);
    printf("  Email    : %s\n", s->email);
    printf("  Year     : %d%s   Semester: %d\n", s->year, yearSuffix(s->year), s->semester);
    printf("  SGPA     : ");
    for (i = 0; i < MAX_SEM; i++) {
        if (i < s->semester) printf("S%d=%.2f  ", i + 1, s->sgpa[i]);
        else                 printf("S%d=--  ", i + 1);
    }
    printf("\n  CGPA     : %.2f\n", calculateCGPA(s));
}
 
void printTableHeader(void) {
    printf("\n%-10s %-6s %-20s %-11s %-26s %-4s %-4s %-5s\n",
           "SAP ID", "Roll", "Name", "Phone", "Email", "Yr", "Sem", "CGPA");
    printf("--------------------------------------------------------------------------------------\n");
}
 
void printTableRow(const Student *s) {
    printf("%-10ld %-6d %-20s %-11s %-26s %-4d %-4d %-5.2f\n",
           s->sapId, s->rollNo, s->name, s->phone, s->email,
           s->year, s->semester, calculateCGPA(s));
}
 
/* ----------------------- Part A functions ------------------ */
 
void addStudent(void) {
    Student s;
    int i;
    if (count >= MAX_STUDENTS) { printf("Record list is full.\n"); return; }
 
    printf("\n--- Add Student ---\n");
    s.sapId = readLong("SAP ID: ");
    if (findBySap(s.sapId) != -1) { printf("A student with this SAP ID already exists.\n"); return; }
    s.rollNo = readInt("Roll No.: ", 1, 99999);
 
    do { readLine("Name: ", s.name, NAME_LEN);
         if (s.name[0] == '\0') printf("  Name cannot be empty.\n");
    } while (s.name[0] == '\0');
 
    do { readLine("Phone (10 digits): ", s.phone, sizeof(s.phone));
         if (!isValidPhone(s.phone)) printf("  Phone must be exactly 10 digits.\n");
    } while (!isValidPhone(s.phone));
 
    do { readLine("Email ID: ", s.email, EMAIL_LEN);
         if (!isValidEmail(s.email)) printf("  Enter a valid email (e.g. name@example.com).\n");
    } while (!isValidEmail(s.email));
 
    while (1) {
        s.year     = readInt("Year (1-4): ", 1, 4);
        s.semester = readInt("Latest semester with declared result (1-8): ", 1, MAX_SEM);
        if (s.semester <= s.year * 2 && s.semester >= (s.year - 1) * 2) break;
        printf("  Semester %d is not possible in year %d. Try again.\n", s.semester, s.year);
    }
 
    for (i = 0; i < MAX_SEM; i++) s.sgpa[i] = 0.0f;
    for (i = 0; i < s.semester; i++) {
        char prompt[40];
        sprintf(prompt, "SGPA of Semester %d: ", i + 1);
        s.sgpa[i] = readSgpa(prompt);
    }
 
    students[count++] = s;
    printf("Student added successfully. CGPA = %.2f\n", calculateCGPA(&s));
}
 
void searchStudent(void) {
    long sap;
    int  idx;
    printf("\n--- Search Student ---\n");
    sap = readLong("Enter SAP ID to search: ");
    idx = findBySap(sap);
    if (idx == -1) printf("No student found with SAP ID %ld.\n", sap);
    else           printStudent(&students[idx]);
}
 
void displayAll(void) {
    int i;
    if (count == 0) { printf("No records available.\n"); return; }
    printf("\n--- All Student Records (%d) ---", count);
    for (i = 0; i < count; i++) printStudent(&students[i]);
}
 
void displayCGPAAll(void) {
    int i;
    if (count == 0) { printf("No records available.\n"); return; }
    printf("\n--- CGPA of Each Student ---\n");
    printf("%-10s %-20s %-5s\n", "SAP ID", "Name", "CGPA");
    printf("------------------------------------\n");
    for (i = 0; i < count; i++)
        printf("%-10ld %-20s %-5.2f\n", students[i].sapId, students[i].name,
               calculateCGPA(&students[i]));
}
 
void yearWiseAverage(void) {
    float sum[5] = {0};
    int   n[5]   = {0};
    int   i, y;
    for (i = 0; i < count; i++) {
        sum[students[i].year] += calculateCGPA(&students[i]);
        n[students[i].year]++;
    }
    printf("\n--- Year-wise Average CGPA ---\n");
    for (y = 1; y <= 4; y++) {
        if (n[y] > 0) printf("  Year %d (%d student%s): %.2f\n", y, n[y], n[y] == 1 ? "" : "s", sum[y] / n[y]);
        else          printf("  Year %d: no students\n", y);
    }
}
 
void deleteStudent(void) {
    long sap;
    int  idx, i;
    printf("\n--- Delete Student ---\n");
    sap = readLong("Enter SAP ID to delete: ");
    idx = findBySap(sap);
    if (idx == -1) { printf("No student found with SAP ID %ld.\n", sap); return; }
    for (i = idx; i < count - 1; i++) students[i] = students[i + 1];
    count--;
    printf("Record deleted.\n");
}
 
/* Sorting works on a COPY, so the original record order is untouched. */
void displaySorted(int bySap) {
    Student copy[MAX_STUDENTS], temp;
    int i, j;
    if (count == 0) { printf("No records available.\n"); return; }
    for (i = 0; i < count; i++) copy[i] = students[i];
 
    for (i = 0; i < count - 1; i++) {                 /* bubble sort */
        for (j = 0; j < count - 1 - i; j++) {
            int swap = bySap ? (copy[j].sapId > copy[j + 1].sapId)
                             : (compareNames(copy[j].name, copy[j + 1].name) > 0);
            if (swap) { temp = copy[j]; copy[j] = copy[j + 1]; copy[j + 1] = temp; }
        }
    }
    printf("\n--- Records in increasing order of %s ---", bySap ? "SAP ID" : "Name");
    printTableHeader();
    for (i = 0; i < count; i++) printTableRow(&copy[i]);
}
 
/* ----------------------- Part B: AI-assisted advisor ------- */
 
/* Categories were suggested by an AI tool; thresholds were checked and
 * adjusted by me to match the 10-point SGPA/CGPA scale. */
void academicAdvisor(const Student *s) {
    float cgpa = calculateCGPA(s);
    int   i, lowest = 0;
 
    printf("\n  Academic Advisor for %s (CGPA %.2f)\n", s->name, cgpa);
 
    if (cgpa >= 9.0f)
        printf("  Category      : Excellent Performance\n"
               "  Recommendation: Excellent performance - maintain your current study pattern.\n");
    else if (cgpa >= 7.5f)
        printf("  Category      : Good Performance\n"
               "  Recommendation: Good performance - revise regularly and aim for 9+ next semester.\n");
    else if (cgpa >= 6.0f)
        printf("  Category      : Needs Improvement\n"
               "  Recommendation: Needs improvement - focus more on subjects with lower SGPA.\n");
    else
        printf("  Category      : Requires Immediate Attention\n"
               "  Recommendation: Requires immediate attention - meet your faculty advisor and make a study plan.\n");
 
    /* Extra personalised hint: weakest semester (only if there is a real gap) */
    for (i = 1; i < s->semester; i++)
        if (s->sgpa[i] < s->sgpa[lowest]) lowest = i;
    if (s->semester > 1 && cgpa - s->sgpa[lowest] >= 0.5f)
        printf("  Note          : Semester %d (SGPA %.2f) was your weakest - revisit its subjects.\n",
               lowest + 1, s->sgpa[lowest]);
}
 
void advisorMenu(void) {
    long sap;
    int  idx;
    printf("\n--- AI-Assisted Academic Advisor ---\n");
    sap = readLong("Enter SAP ID: ");
    idx = findBySap(sap);
    if (idx == -1) printf("No student found with SAP ID %ld.\n", sap);
    else           academicAdvisor(&students[idx]);
}
 
/* ----------------------- sample data ----------------------- */
 
void addSample(long sap, int roll, const char *name, const char *phone,
               const char *email, int year, int sem, const float *g) {
    Student *s = &students[count++];
    int i;
    s->sapId = sap; s->rollNo = roll; s->year = year; s->semester = sem;
    strcpy(s->name, name); strcpy(s->phone, phone); strcpy(s->email, email);
    for (i = 0; i < MAX_SEM; i++) s->sgpa[i] = (i < sem) ? g[i] : 0.0f;
}
 
void loadSampleData(void) {
    float a[] = {9.2f, 9.4f, 9.1f, 9.5f, 9.3f, 9.6f};
    float b[] = {8.1f, 7.9f, 8.4f, 8.0f};
    float c[] = {6.5f, 6.8f, 6.2f};
    float d[] = {5.2f, 4.8f};
    float e[] = {7.0f, 7.5f, 8.0f, 8.5f, 8.2f, 8.8f, 9.0f, 9.1f};
    addSample(500012001L, 1, "Aarav Sharma", "9876543210", "aarav.s@stu.upes.ac.in", 3, 6, a);
    addSample(500012002L, 2, "Diya Verma",   "9876501234", "diya.v@stu.upes.ac.in",  2, 4, b);
    addSample(500012003L, 3, "Rohan Gupta",  "9812345678", "rohan.g@stu.upes.ac.in", 2, 3, c);
    addSample(500012004L, 4, "Kabir Singh",  "9898989898", "kabir.s@stu.upes.ac.in", 1, 2, d);
    addSample(500012005L, 5, "Ananya Rao",   "9123456780", "ananya.r@stu.upes.ac.in",4, 8, e);
}
 
/* ----------------------- main ------------------------------ */
 
void showMenu(void) {
    printf("\n========== STUDENT RECORD & RESULT MANAGEMENT ==========\n"
           " 1. Add student\n"
           " 2. Search student (by SAP ID)\n"
           " 3. Display all students\n"
           " 4. Display CGPA of each student\n"
           " 5. Year-wise average CGPA\n"
           " 6. Delete student\n"
           " 7. Display sorted by SAP ID\n"
           " 8. Display sorted by Name\n"
           " 9. AI-Assisted Academic Advisor\n"
           " 0. Exit\n"
           "========================================================\n");
}
 
int main(void) {
    int choice;
    loadSampleData();                      /* 5 students pre-loaded */
    do {
        showMenu();
        choice = readInt("Enter your choice: ", 0, 9);
        switch (choice) {
            case 1: addStudent();       break;
            case 2: searchStudent();    break;
            case 3: displayAll();       break;
            case 4: displayCGPAAll();   break;
            case 5: yearWiseAverage();  break;
            case 6: deleteStudent();    break;
            case 7: displaySorted(1);   break;
            case 8: displaySorted(0);   break;
            case 9: advisorMenu();      break;
            case 0: printf("Thank you. Goodbye!\n"); break;
        }
    } while (choice != 0);
    return 0;
}
