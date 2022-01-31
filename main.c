#include <stdio.h>
#include "student.h"
#include "course.h"
#define MAX_COURSE_LOAD 15

struct course {
    int         ref_count;
    enum subject     subject;
    uint16_t    code;
};

struct student {
    struct student_id   student_id;
    bool                is_graduate;
    struct grade*       grades[MAX_COURSE_LOAD];
    int                 numOfCourses;     
};

struct grade {
    struct course*  course;
    float          grade;
};

void test_student();

int main () {
    printf("Program!");

    printf("\nTesting!");
    test_student();
    printf("\nTesting Completed!");

    return 0;
}

/*
 *   Test creating student, freeing student, student taking course, getting 
 *   student mark from course, getting the student average of passed courses,
 *   and if student is promotable.
 */
void test_student() {

    printf("\nCreating course");
    struct course* engi = course_create(SUBJ_ENGI, 1);
    struct course* civ = course_create(SUBJ_CIV, 2);
    struct course* ece = course_create(SUBJ_ECE, 3);
    struct course* math = course_create(SUBJ_MATH, 4);
    struct course* phys = course_create(SUBJ_PHYS, 5);
    struct course* mech = course_create(SUBJ_MECH, 6);
    struct course* onae = course_create(SUBJ_ONAE, 7);
    struct course* proc = course_create(SUBJ_PROC, 8);
    struct course* chem = course_create(SUBJ_CHEM, 9);
    struct course* engl = course_create(SUBJ_ENGL, 10);
    printf("\nCourse created");

    printf("\nCreating Amber's ID");
    struct student_id amberId = {
        .sid_serial = 0x2111,
        .sid_year = 0x232F
    };
    printf("\nAmber's ID Created");

    printf("\nCreating student Amber");

    struct student* amber = student_create(amberId, true);
    printf("\nStudent Amber created");

    printf("\nCourses taken: %d", amber->numOfCourses);

    printf("\nIs Amber a graduate: %s", amber->is_graduate ? "true" : "false");

    // printf("Creating Ashley's ID");
    // struct student_id ashleyId = {12213, 121221};
    // printf("Ashley's ID Created");

    // printf("Creating student Ashley");
    // struct student* ashley = student_create(ashleyId, true);
    // printf("Student Ashley created");

    // printf("Creating Preston's ID");
    // struct student_id prestonId = {12214, 121221};
    // printf("Preston's ID Created");

    // printf("Creating student Preston");
    // struct student* preston = student_create(prestonId, false);
    // printf("Student Preston Created");

    // printf("Testing students taking a course");

    printf("\nRecord Amber taking 10 courses!");
    student_take(amber, engi, 99);
    student_take(amber, civ, 89);
    student_take(amber, ece, 50);
    student_take(amber, math, 49);
    printf("\nAmber grades recorded!");

    // printf("\nAmber's grade in %u is %f", amber->grades[0]->course->subject, amber->grades[0]->grade);

    //int numOfCourses = sizeof(amber->grades)/sizeof(amber->grades[0]);

    //printf("\nSize of grades array: %d numOfCourses: %d", numOfCourses, amber->numOfCourses);

    printf("\n Is amber promotable? %s", student_promotable(amber) ? "true" : "false");
    //printf("\nGrade is %d", student_grade(amber, chem));
    //printf("\npassed avergae is %f", student_passed_average(amber));

    // int x = 0;
    // while (x < amber->numOfCourses) {
    //     printf("Trying...");
    //     printf("Amber's grade in %u is %f", amber->grades[x]->course->subject, amber->grades[x]->grade);
    //     x = x + 1;
    // }

    // printf("Record Ashley taking Engi with a 80%% grade");
    // student_take(ashley, engi, 80);
    // printf("Ashley recorded!");

    // printf("Record Preston taking Engi with a 65%% grade");
    // student_take(preston, engi, 65);
    // printf("Preston recorded!");

    // printf("Testing getting student course grade");

    // printf("\nAmber's grade in Engi is: %d", student_grade(amber, engi));
    // printf("Ashley's grade in Engi is: %d", student_grade(ashley, engi));
    // printf("Preston's grade in Engi is: %d", student_grade(preston, engi));

    // printf("Testing if student is promotable");

    // printf("\nIs Amber promotable: %s", student_promotable(amber) ? "true" : "false");
    // printf("Is Ashley promotable: %s", student_promotable(ashley) ? "true" : "false");
    // printf("Is Preston promotable: %s", student_promotable(preston) ? "true" : "false");
}

