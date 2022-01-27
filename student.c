#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "student.h"
#include "course.h"
#define MAX_COURSE_LOAD 6

/* Forward declaration of course type. */
struct course;

struct grade {
    struct course*  course;
    double          grade;
};

/** A student ID is composed of an application year and a serial number. */
struct student_id;

/** Opaque type representing a student. */
struct student {
    struct student_id   student_id;
    bool                is_graduate;
    struct grade*       grades[MAX_COURSE_LOAD];
    int                 numOfCourses;
    struct course*      courses[];   
};

/**
 * Create a new student object.
 *
 * The caller is responsible for freeing the returned memory.
 */
struct student*	student_create(struct student_id student_id, bool grad_student) {

    struct student* p = malloc(sizeof(struct student));

    p->student_id = student_id;
    p->is_graduate = grad_student;
    p->numOfCourses = 0;

    return p;
}

/**
 * Release a student object.
 */
void student_free(struct student* student) {
    for (int i = 0; i < student->numOfCourses; i++) {
        course_release(student->courses[i]);
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

    struct grade* g = malloc(sizeof(struct grade));

    g->course = course;
    g->grade = grade;

    int numOfCourses = student->numOfCourses;

    if (numOfCourses >= MAX_COURSE_LOAD) return;
    
    student->grades[numOfCourses] = g;
    student->numOfCourses = numOfCourses + 1;

    course_hold(course);
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
    for (int i = 0; i < student->numOfCourses; i++) {
        if (course_code(student->grades[i]->course) == course_code(course)) 
            return student->grades[i]->grade;
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
            sumGrades += student->grades[i]->grade;
            passedCourses++;
        }
    }

    if (passedCourses == 0) return false;

    printf("\nThe student passed average: %d", (sumGrades/passedCourses));
    return (sumGrades/passedCourses);
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

            if (student->grades[i]->grade < 50) {
                fails++;
                printf("\nFailed course! Total failures: %d", fails);
                if (fails > 1) return false;
            }
        }
    } else {
        int sumGrade = 0;
        for (int i = 0; i < student->numOfCourses; i++) {
            sumGrade += student->grades[i]->grade;
        }
        printf("\nCourse Average: %d", sumGrade/student->numOfCourses);
        if (sumGrade/student->numOfCourses < 60) return false;
    }

    return true;
}

