#ifndef CHAI_LATTE_H
#define CHAI_LATTE_E

typedef struct chai_t char_t;
typedef int (*chai_handler_t)(chai_t *, char *esc);

chai_t *chai_create(int rows, int cols, int buf_sz);
void chai_destroy(chai_t *);
void chai_resize(chai_t *, int rows, int cols);

int chai_getpty(chai_t *);

#endif
