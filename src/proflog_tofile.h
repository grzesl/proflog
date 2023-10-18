#ifndef PROFLOG_TOFILE_H
#define PROFLOG_TOFILE_H


void tofile_init(char * filename);
void tofile_append(char * data, unsigned int len);
void tofile_write(unsigned int no, char * data, unsigned int len);
void tofile_close();

#endif //PROFLOG_TOFILE_H