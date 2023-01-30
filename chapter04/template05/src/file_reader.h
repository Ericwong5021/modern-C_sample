#ifndef _FILE_READER_H_
#define _FILE_READER_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FileReaderContext {
    const char * const pFname;
    void (* const processor)(struct FileReaderContext *pThis, FILE *fp);
} FileReaderContext; // 可以给read_file函数提供文件操作上下文

int read_file(FileReaderContext *pFileCtx);

#ifdef __cplusplus
}
#endif

#endif
