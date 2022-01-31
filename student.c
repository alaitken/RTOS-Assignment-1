#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "student.h"
#include "course.h"
#define MAX_COURSE_LOAD 15

/* Forward declaration of course type. */
struct course {
    int         ref_count;
    enum subject     subject;
    uint16_t    code;
};

struct grade {
    struct course*  course;
    float           grade;
};

/** A student ID is composed of an application year and a serial number. */
struct student_id;

/** Opaque type representing a student. */
struct student {
    struct student_id   student_id;
    bool                is_graduate;
    struct grade*       grades[MAX_COURSE_LOAD];
    int                 numOfCourses;
};

/**
 * Create a new student object.
 *
 * The caller is responsible for freeing the returned memory.
 */
struct student*	student_create(struct student_id student_id, bool grad_student) {

    struct student* student = malloc(sizeof(struct student));

    student->student_id = student_id;
    student->is_graduate = grad_student;
    student->numOfCourses = 0;

    return student;
}

/**
 * Release a student object.
 */
void student_free(struct student* student) {
    int numOfCourses = sizeof(student->grades)/sizeof(student->grades[0]);
    for (int i = 0; i < student->numOfCourses; i++) {
        course_release(student->grades[i]->course);
    }
    free(student);
}

/**
 * Note that a student has taken a course.
 *
 * This student will now take a reference to (i.e., increment the refcount of)
 * the course that is passed in.
 */
void student_take(struct student *student, struct course* course, uint8_t grade) {
    if (student->numOfCourses > MAX_COURSE_LOAD) return;

    printf("\n Student: %u-%u took: %u grade: %u", student->student_id.sid_year, student->student_id.sid_serial, course_code(course), grade);
    bool isRepeat = false;
    
    for (int i = 0; i < student->numOfCourses; i++) {
       if (course_code(student->grades[i]->course) == course_code(course)) {
           printf("\nAlready taken subject! Updating!");
           student->grades[i]->grade = grade;
           isRepeat = true;
       }
    }

    if (!isRepeat) {
        struct grade* g = malloc(sizeof(struct grade));
        g->course = course;
        g->grade = grade;
        student->grades[student->numOfCourses] = g;
        student->numOfCourses = student->numOfCourses + 1;
        printf("\nNum of courses: %d", student->numOfCourses);
        course_hold(course);
    }
}

/**
 * Retrieve a student's mark in a course.
 *
 * This will retrieve the most recent grade that a student achieved in a
 * particular course.
 *
 * @returns    a grade, or -1 if the student has not taken the course
 */
int student_grade(struct student* student, struct course* course) {
    printf("\nChecking if student is taking course");
    for (int i = 0; i < student->numOfCourses; i++) {
        if (course_code(student->grades[i]->course) == course_code(course)) {
            printf("\nStudent: %u-%u is taking course: %u Grade: %f", student->student_id.sid_year,student->student_id.sid_serial, course_code(course), student->grades[i]->grade);
            return student->grades[i]->grade;
        }
    }
    return -1;
}

/**
 * Determine the average grade in the courses this student has passed.
 *
 * This will average the grades in courses that the student has passed, meaning
 * that they scored at least 50 (if undergraduate) or 65 (if graduate).
 *
 * @returns     the average, or 0 if no courses have been passed
 */
double student_passed_average(const struct student* student) {

    int sumGrades = 0;
    int passedCourses = 0;

    for (int i = 0; i < student->numOfCourses; i++) {
        if ((student->is_graduate && student->grades[i]->grade >= 65) || 
            (!(student->is_graduate) && student->grades[i]->grade >= 50)) {
            sumGrades = sumGrades + student->grades[i]->grade;
            passedCourses++;
        }
    }

    if (passedCourses == 0) return false;
    printf("\nTotalled to: %d PassedCourses: %d", (sumGrades/passedCourses), passedCourses);
    printf("\nThe student passed average: %d", (sumGrades/passedCourses));
    return ((double)sumGrades/(double)passedCourses);
}

/**
 * Determine whether or not this student is promotable.
 *
 * (note: this is not how promotion really works... simplified for assignment)
 *
 * For graduate students, determine whether they have passed all of their
 * courses apart from (up to) one.
 *
 * For undergraduate students, determine whether or not the cumulative average
 * of all courses is at least 60%.
 */
bool student_promotable(const struct student* student) {

    if (student->is_graduate) {
        printf("\nIs a graduate");
        int fails = 0;

        for (int i = 0; i < student->numOfCourses; i++) {

            if (student->grades[i]->grade < 65) {
                fails++;
                printf("\nFailed course! Total failures: %d", fails);
                if (fails > 1) {
                    printf("\nToo many failures. Not promotable.");
                    return false;
                }
            }
        }
    } else {
        int sumGrade = 0;
        for (int i = 0; i < student->numOfCourses; i++) {
            sumGrade = sumGrade + student->grades[i]->grade;
        }
        printf("\nCourse Average: %d NumOfCourse: %d Avg: %d", sumGrade, student->numOfCourses, sumGrade/student->numOfCourses);
        if (sumGrade/student->numOfCourses < 60) return false;
    }

    return true;
}

