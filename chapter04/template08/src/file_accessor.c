#include "file_accessor.h"

bool access_file(FileAccessorContext *pCtx) { // 文件操作模板
    FILE *fp = fopen(pCtx->pFname, pCtx->pMode);
    if (fp == NULL) return false;

    pCtx->processor(pCtx, fp);

    fclose(fp);
    return true;
}
