#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "int_sorter.h"
#include "file_accessor.h"
#include "buffer.h"

static void reader(FileAccessorContext *pThis, FILE *fp);
static long file_size(const char *pFname);
static void size_reader(FileAccessorContext *pThis, FILE *fp);
static void do_with_buffer(BufferContext *p);
static void writer(FileAccessorContext *pThis, FILE *fp);
static int comparator(const void *p1, const void *p2);
static void file_error(Context *pCtx);

typedef struct {
    BufferContext base;
    Context *pAppCtx;
} MyBufferContext;// 带错误信息的内存操作上下文

typedef struct {
    FileAccessorContext base;
    MyBufferContext *pBufCtx;
} MyFileAccessorContext; // 包含文件和内存两种操作模板的上下文

typedef struct {
    FileAccessorContext base;
    long size;
} SizeGetterContext; // 获取size的文件操作上下文

IntSorterError int_sorter(const char *pFname) { // 对文件内容进行排序
    Context ctx = {pFname, ERR_CAT_OK};

    long size = file_size(pFname); // 先获取文件大小
    if (size == -1) {
        file_error(&ctx); // 设置errorCategory为文件错误
        return ctx.errorCategory;
    }

    MyBufferContext bufCtx = {{NULL, size, do_with_buffer}, &ctx};
    if (! buffer(&bufCtx.base)) {
        ctx.errorCategory = ERR_CAT_MEMORY; // 设置errorCategory为内存错误
    }

    return ctx.errorCategory;
}

static void do_with_buffer(BufferContext *p) {
    MyBufferContext *pBufCtx = (MyBufferContext *)p;
    MyFileAccessorContext readFileCtx = {{pBufCtx->pAppCtx->pFname, "rb", reader}, pBufCtx};

    if (! access_file(&readFileCtx.base)) {
        file_error(pBufCtx->pAppCtx);
        return;
    }

    qsort(p->pBuf, p->size / sizeof(int), sizeof(int), comparator);

    MyFileAccessorContext writeFileCtx = {{pBufCtx->pAppCtx->pFname, "wb", writer}, pBufCtx};
    if (! access_file(&writeFileCtx.base)) {
        file_error(pBufCtx->pAppCtx);
        return;
    }
}

static void reader(FileAccessorContext *p, FILE *fp) {
    MyFileAccessorContext *pFileCtx = (MyFileAccessorContext *)p;
    MyBufferContext *pBufCtx = pFileCtx->pBufCtx;

    if (pBufCtx->base.size != fread(pBufCtx->base.pBuf, 1, pBufCtx->base.size, fp)) {
        file_error(pBufCtx->pAppCtx);
    }
}

static void writer(FileAccessorContext *p, FILE *fp) {
    MyFileAccessorContext *pFileCtx = (MyFileAccessorContext *)p;
    MyBufferContext *pBufCtx = pFileCtx->pBufCtx;

    if (fwrite(pBufCtx->base.pBuf, 1, pBufCtx->base.size, fp) != pBufCtx->base.size) {
        file_error(pBufCtx->pAppCtx);
    }
}

static int comparator(const void *p1, const void *p2) {
    int i1 = *(const int *)p1;
    int i2 = *(const int *)p2;
    if (i1 < i2) return -1;
    if (i1 > i2) return 1;
    return 0;
}

static void file_error(Context *pCtx) {
    fprintf(stderr, "%s: %s\n", pCtx->pFname, strerror(errno));
    pCtx->errorCategory = ERR_CAT_FILE;
}

static long file_size(const char *pFname) { // 获取文件size的文件模板操作
    SizeGetterContext ctx = {{pFname, "rb", size_reader}, 0}; // 这里的提供了size方法的实现

    if (! access_file(&ctx.base)) {
        return -1;
    }

    return ctx.size;
}

static void size_reader(FileAccessorContext *p, FILE *fp) { // size方法的具体实现
    SizeGetterContext *pThis = (SizeGetterContext *)p;
    pThis->size = -1;

    if (fseek(fp, 0, SEEK_END) == 0)
        pThis->size = ftell(fp);
}
