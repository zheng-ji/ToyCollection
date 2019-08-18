#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

// 注意不要使用using namespace std; 
// 因为会和std命名空间的iterator_traits冲突
// 自定义迭代器
template<class T>
struct MyIter
{
    typedef T value_type;
    T *ptr;
    MyIter(T *p = 0):ptr(p){}
    T& operator*() const { return *ptr; }
};
// 型别萃取泛化版
template<class I>
struct iterator_traits
{
    typedef typename I::value_type value_type;
};
// 型别萃取原生指针特化版
template<class I>
struct iterator_traits<I*>
{
    typedef I value_type;
};
// 型别萃取常量指针特化版
template<class I>
struct iterator_traits<const I*>
{
    typedef I value_type;
};
// 萃取value_type
template<class I>
typename iterator_traits<I>::value_type
value_type(I ite)
{
    return *ite;
}
// 输出类型泛化版
template<class T>
void print(T)
{
    cout << "T" << endl;
}
// 输出类型int特化版
template<>
void print(int)
{
    cout << "int" << endl;
}
// 输出类型char特化版
template<>
void print(char)
{
    cout << "char" << endl;
}
int main(void)
{
    int i = 0;
    char c = 0;
    int *p = &i;
    const int *cp = &i;
    MyIter<int> iiter(&i);
    MyIter<char> citer(&c);
    print(value_type(p));
    print(value_type(cp));
    print(value_type(iiter));
    print(value_type(citer));
    return 0;
}
