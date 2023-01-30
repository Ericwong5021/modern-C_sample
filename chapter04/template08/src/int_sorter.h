#ifndef _INT_SORTER_H_
#define _INT_SORTER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ERR_CAT_OK = 0,
    ERR_CAT_FILE,
    ERR_CAT_MEMORY
} IntSorterError; // Error codes

typedef struct {
    const char * const pFname;
    int errorCategory;
} Context; // 应用程序的上下文

IntSorterError int_sorter(const char *pFname);

#ifdef __cplusplus
}
#endif

#endif
