#include <iostream>
#include <string>
#include "thread_pool_4_24.cpp"

void test1(){
    std::cout << "no args task" << std::endl;
}
void test2(int a, std::string s){
    std::cout << "args task: " << a << " " << s << std::endl; 
}
int test3(int a, int b){
    return a+b;
}

int main(){
    ThreadPool pool(4);
    pool.submit(test1);
    pool.submit(test2, 100, "hello");
    auto f = pool.submit(test3, 10, 20);
    std::cout << "return: " << f.get() << std::endl;
    auto f2 = pool.submit([](int x){return x*2;}, 50);
    std::cout << "lamba return: " << f2.get() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}