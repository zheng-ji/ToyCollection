#ifndef EXTERN_SORT_H
#define EXTERN_SORT_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

class ExternSort {
    public:
        void sort();
        ExternSort(const char * input_file,const char* out_file,int count,int target_num);
        virtual ~ExternSort();
        
    private:
        int m_count;
        int m_target_num;
        char * m_in_file;
        char * m_out_file;

    protected:
        int read_data(FILE *f,int a[],int n);
        void write_data(FILE *f,int a[],int n);
        char * temp_filename(int index);
        static int cmp_int(const void *a,const void *b);
        int memory_sort();
        void merge_sort(int file_count);

};

#endif
