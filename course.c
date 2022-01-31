#include "course.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/** Opaque course type. */
struct course {
    int         ref_count;
    enum subject     subject;
    uint16_t    code;
};

struct grade;

/**
 * Create a new Course.
 *
 * Returns an object with a refcount of 1.
 */
struct course*	course_create(enum subject subject, uint16_t code) {

    struct course* c = malloc(sizeof(struct course));

    c->ref_count = 1;
    c->subject = subject;
    c->code = code;

    printf("Course created: %u", subject);

    return c;
}

/** Retrieve a course's subject. */
enum subject	course_subject(const struct course* course) {
    return course->subject;
}

/** Retrieve a course's course code. */
uint16_t	course_code(const struct course* course) {
    return course->code;
}

/** Increment a course's refcount. */
void		course_hold(struct course* course) {
    printf("\nHolding course! %u", course->subject);
    course->ref_count = course->ref_count + 1;
    printf("\nRef count is now: %d", course->ref_count);
}

/** Decrement a course's refcount (optionally freeing it). */
void		course_release(struct course* course) {
    printf("\nTrying to release course.");
    if (course->ref_count > 0) {
        printf("\nDecrementing ref count.");
        course->ref_count = course->ref_count - 1;
    }
}

/** Retrieve the current reference count of a course. */
int		course_refcount(const struct course* course) {
    return course->ref_count;
}