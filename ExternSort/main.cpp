#include <iostream>
#include <ctime>
#include <fstream>
#include "ExternSort.h"
using namespace std;

const unsigned int count = 100; // 文件里数据的行数

const unsigned int number_to_sort = 10; //在内存中一次排序的数量

const char *unsort_file = "unsort_data.txt"; //原始未排序的文件名

const char *sort_file = "sort_data.txt"; //已排序的文件名

void init_data(unsigned int num); //随机生成数据文件

int main(int argc, char* *argv)
{
    srand(time(NULL));
    init_data(count);
    ExternSort extSort(unsort_file, sort_file, number_to_sort);
    extSort.sort();
    return 0;
}

void init_data(unsigned int num)
{
    FILE* f = fopen(unsort_file, "wt");
    for(int i = 0; i < num; ++i)
        fprintf(f, "%d ", rand());
    fclose(f);
}
