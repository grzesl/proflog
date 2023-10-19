#include "proflog_tofile.h"
#include "proflog_config.h"
#include "proflog_fmt.h"

#if LOG_LEVEL != 5
#define MINIZ_NO_STDIO
//#define MINIZ_NO_TIME
#define MINIZ_NO_ZLIB_APIS


#include "miniz.h"


#include <stdio.h>

char *filenameptr;
static const char *s_pComment = "This is a comment";

char tofile_buff[MIN_LOG_SAVE_LEN + MAX_LOG_LINE_LEN + 1];
FmtLine tofile_builder; 
unsigned int tofile_logno = 1;

void tofile_init(char *filename)
{
    filenameptr = filename;
    // printf("miniz.c version: %s\n", MZ_VERSION);
    remove(filenameptr);

    fmt_init(&tofile_builder,tofile_buff, MIN_LOG_SAVE_LEN + MAX_LOG_LINE_LEN );
}

void tofile_flush(unsigned long long fileSize)
{
    tofile_write(tofile_logno++, tofile_builder.data, tofile_builder.pos);
    fmt_init(&tofile_builder, tofile_buff, MIN_LOG_SAVE_LEN + MAX_LOG_LINE_LEN);
}

void tofile_append(char *data, unsigned int len) {
    fmt_append_mem(&tofile_builder, data, len);
    if(tofile_builder.pos >= MIN_LOG_SAVE_LEN)
        tofile_flush(tofile_builder.pos);
}

void tofile_write(unsigned int no, char *data, unsigned int len)
{
    char *archive_name_ptr;
    char archive_filename[64];
    //sprintf(archive_filename, "%08u.log.txt", no);
    archive_name_ptr = itoa_vitaut1(archive_filename, no);
    strcat(archive_name_ptr,".log.txt");
    mz_zip_add_mem_to_archive_file_in_place(filenameptr, archive_name_ptr, data, len, s_pComment, (unsigned short)strlen(s_pComment), MZ_BEST_COMPRESSION);
}

void tofile_close()
{
    tofile_flush(tofile_builder.pos);
}


#endif
