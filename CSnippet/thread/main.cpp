#include <iostream>
#include <thread>
#include <future>
#include <stdexcept>
#include <limits>

void thread_func(int num) {
    std::cout << std::this_thread::get_id() <<"\tnum:" << num << std::endl;
}

struct functor{
    void operator()(int a, int b) {
        std::cout << a << "+" << b << "=" << a+b <<std::endl;
    }
};

class C {
    public:
        C(int data):data_(data) {}
        void member_func() {std::cout << this->data_ << std::endl;}
        int data_;
};

std::uint32_t add(std::uint32_t a, std::uint32_t b)
{
    std::cout << "thread id = " << std::this_thread::get_id() << ", a = " << a << ", b = " << b << std::endl;
    if(std::numeric_limits<std::uint32_t>::max() - a < b) {
        throw std::overflow_error("overflow_error");
    }
    return a + b;
}

void add_wrapper(std::promise<std::uint32_t> promise, std::uint32_t a, std::uint32_t b)
{
    try {
        // 设置值为供std::future对象获取
        promise.set_value(add(a, b));
    }
    catch(...) {
        // 设置异常在std::future获取值时抛出
        promise.set_exception(std::current_exception());
    }
}


int main() {
    // first
    std::thread td(thread_func, 3);
    td.join();

    // second
    std::thread td1([](int a, int b) {std::cout << a << "+" << b << "=" << a+b << std::endl;}, 1, 2);
    td1.join();

    // third
    std::thread td2(functor(), 1, 3);
    td2.join();

    // fourth
    C obj(10);
    std::thread t3(std::bind(&C::member_func, &obj));
    t3.join();

    std::thread t4([&obj]() {obj.member_func();});
    t4.join();

    // 使用std::async启动异步操作并返回std::future对象
    // 通过std::future::get等待异步操作完成并取得返回值
    std::future<std::uint32_t> f1 = std::async(std::launch::async, add, 1ul, 2ul);
    std::uint32_t sum1 = f1.get();
    std::cout << "thread id = " << std::this_thread::get_id() << ", sum1 = " << sum1 << std::endl;

    // 4000000000ul + 4000000000ul会抛出异常，异常会被延迟到std::future::get或std::future::wait时抛出
    std::future<std::uint32_t> f2 = std::async(std::launch::async, add, 4000000000ul, 4000000000ul);
    try {
        std::uint32_t sum2 = f2.get();
        std::cout << "thread id = " << std::this_thread::get_id() << ", sum2 = " << sum2 << std::endl;
    } catch(const std::overflow_error& e) {
        std::cout << "thread id = " << std::this_thread::get_id() << ", e.what() = " << e.what() << std::endl;
    }

    // 创建std::promise对象
    std::promise<std::uint32_t> promise1;
    // 获取关联的std::future对象
    std::future<std::uint32_t> f3 = promise1.get_future();
    // 启动线程执行add函数的包装函数
    std::thread(add_wrapper, std::move(promise1), 1ul, 2ul).detach();
    // 等待并获取异步操作的结果
    std::uint32_t sum3 = f3.get();
    std::cout << "thread id = " << std::this_thread::get_id() << ", sum3 = " << sum3 << std::endl;

    std::promise<std::uint32_t> promise2;
    std::future<std::uint32_t> f4 = promise2.get_future();
    // 4000000000ul + 4000000000ul会抛出异常
    std::thread(add_wrapper, std::move(promise2), 4000000000ul, 4000000000ul).detach();
    try {
        std::uint32_t sum2 = f4.get();
        std::cout << "thread id = " << std::this_thread::get_id() << ", sum2 = " << sum2 << std::endl;
    }
    catch(const std::overflow_error& e) {
        std::cout << "thread id = " << std::this_thread::get_id() << ", e.what() = " << e.what() << std::endl;
    }
    return 0;
}

