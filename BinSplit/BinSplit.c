#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

typedef struct FHeader
{
    char magic[4];
    int32_t partnum;
} FHeader;

typedef struct FSection
{
    int32_t offset;
    int32_t size;
} FSection;

int
FSection_comparator(const void *_t1, const void *_t2)
{
    FSection *t1 = (FSection *)_t1;
    FSection *t2 = (FSection *)_t2;

    return t1->offset - t2->offset;
}

void
out_to_file(const char *file_name,
    char *file_buf, FHeader *hdr, FSection *sec,
    int first_sec, int sec_num)
{
    FILE *out = fopen(file_name, "w");

    FHeader cur_hdr = *hdr;
    cur_hdr.partnum = sec_num;

    fwrite(&cur_hdr, sizeof(FHeader), 1, out);

    int cur_offset = sizeof(FHeader);
    FSection *sec_buf = (FSection *)calloc(sec_num, sizeof(FSection));
    int sec_buf_k = 0;

    for (int i = first_sec; i < first_sec + sec_num; i++) {
        sec_buf[sec_buf_k] = sec[i];
        sec_buf[sec_buf_k].offset = cur_offset;
        sec_buf_k++;

        fwrite(file_buf + sec[i].offset, sizeof(char), sec[i].size, out);
        cur_offset += sec[i].size;
    }

    fwrite(sec_buf, sizeof(FSection), sec_buf_k, out);

    fclose(out);
    free(sec_buf);
}

int
main(int argc, char *argv[])
{
    FILE *in = fopen(argv[1], "r");
    int out_files;
    sscanf(argv[2], "%d", &out_files);

    fseek(in, 0, SEEK_END);
    unsigned file_size = ftell(in);
    fseek(in, 0, SEEK_SET);
    
    char *file_buf = (char *)calloc(file_size, sizeof(char));
    fread(file_buf, sizeof(char), file_size, in);
    fclose(in);

    FHeader *hdr = (FHeader *)file_buf;
    FSection *sec = (FSection *)(file_buf + file_size) - hdr->partnum;

    qsort(sec, hdr->partnum, sizeof(FSection), FSection_comparator);

    int sec_in_file = hdr->partnum / out_files, extra_sec_file = hdr->partnum % out_files;
    for (int i = 0, first_sec = 0; i < out_files; i++) {
        if (sec_in_file == 0 && extra_sec_file < i)
            break;

        char tmp_name[PATH_MAX];
        snprintf(tmp_name, PATH_MAX, "%s-%d", argv[1], i + 1);

        int sec_num = sec_in_file + (i < extra_sec_file);
        out_to_file(tmp_name, file_buf, hdr, sec, first_sec, sec_num);
        first_sec += sec_num;
    }

    free(file_buf);
    return 0;
}
