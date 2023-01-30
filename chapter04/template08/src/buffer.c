#include <stdlib.h>

#include "buffer.h"

bool buffer(BufferContext *pThis) { // 内存操作模板
    pThis->pBuf = malloc(pThis->size);
    if (pThis->pBuf == NULL) return false;

    pThis->processor(pThis);

    free(pThis->pBuf);
    return true;
}
