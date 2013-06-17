#ifndef EXTERN_SORT_H
#define EXTERN_SORT_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

class ExternSort {
    public:
        void sort() {
            time_t start = time(NULL);
            int file_count = memory_sort();
            merge_sort(file_count);
            time_t end = time(NULL);
            printf("total time:%f",(end-start)*1000.0 / CLOCKS_PER_SEC);
        }

        ExternSort(const char * input_file,const char *out_file,int count) {
            m_count = count;
            m_in_file = new char[strlen(input_file) + 1];
            strcpy(m_in_file,input_file);
            m_out_file = new char[strlen(out_file) + 1];
            strcpy(m_out_file,out_file);
        }

        virtual ~ExternSort() {
            delete [] m_in_file;
            delete [] m_out_file;
        }

    private:
        int m_count;
        char * m_in_file;
        char * m_out_file;

    protected:
        int read_data(FILE *f,int a[],int n) {
            int i = 0;
            while (i < n && (fscanf(f,"%d",&a[i]) != EOF))
                i++;
            printf("read :%d integer\n",i);
            return i;
        }

        void write_data(FILE *f,int a[],int n) {
            for(int i = 0; i < n; i++)
                fprintf(f,"%d ",a[i]);
        }

        char * temp_filename(int index) {
            char * tempfile = new char[100];
            sprintf(tempfile,"temp%d.txt",index);
            return tempfile;
        }

        static int cmp_int(const void* a, const void* b) {
            return *(int*)a - *(int*)b;
        }

        int memory_sort() {
            FILE * fin = fopen(m_in_file,"rt");
            int n = 0,file_count = 0;
            int *array = new int[m_count];
            while ((n = read_data(fin,array,m_count)) > 0) {
                qsort(array,n,sizeof(int),cmp_int);
                std::cout << array[0] << std::endl;
                char *fileName = temp_filename(file_count++);
                FILE * tempFile = fopen(fileName,"w");
                free(fileName);
                write_data(tempFile,array,n);
                fclose(tempFile);
            }
            delete[] array;
            fclose(fin);
            return file_count;
        }

        void merge_sort(int file_count) {
            if(file_count <= 0) return;
            //归并临时文件
            FILE *fout = fopen(m_out_file, "wt");
            FILE* *farray = new FILE*[file_count];
            int i;
            for(i = 0; i < file_count; ++i)
            {
                char* fileName = temp_filename(i);
                farray[i] = fopen(fileName, "rt");
                free(fileName);
            }

            int *data = new int[file_count];//存储每个文件当前的一个数字
            bool *hasNext = new bool[file_count];//标记文件是否读完
            memset(data, 0, sizeof(int) * file_count);
            memset(hasNext, 1, sizeof(bool) * file_count);

            for(i = 0; i < file_count; ++i)
            {
                if(fscanf(farray[i], "%d", &data[i]) == EOF)//读每个文件的第一个数到data数组
                    hasNext[i] = false;
            }
            while(true)
            {
                int max = data[0];
                //求data中可用的最小的数字，并记录对应文件的索引
                int j = 0;
                for(i = 0; i < file_count; ++i)
                {
                    if(hasNext[i] && max > data[i])
                    {
                        max = data[i];
                        j = i;
                    }
                }
                if(j == 0 && !hasNext[0]) break; //没有可取的数字，终止归并

                if(fscanf(farray[j], "%d", &data[j]) == EOF) //读取文件的下一个元素
                    hasNext[j] = false;
                fprintf(fout, "%d ",max);
            }

            delete [] hasNext;
            delete [] data;

            for(i = 0; i < file_count; ++i)
            {
                fclose(farray[i]);
            }
            delete [] farray;
            fclose(fout);
        }
};

#endif
