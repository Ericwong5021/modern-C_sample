#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BufferContext {
    void *pBuf;
    size_t size;
    void (*processor)(struct BufferContext *p);
} BufferContext; // 内存操作上下文

bool buffer(BufferContext *pThis);

#ifdef __cplusplus
}
#endif


#endif
