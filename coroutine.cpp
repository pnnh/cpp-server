
#include <boost/coroutine/coroutine.hpp>
#include <string>

int main(int argc, char* argv[])
{
    // 类型声明类似boost.function不过这里不是函数原型，而算是调用与返回的通讯协议
    typedef boost::coroutines::coroutine<std::string(std::string)>  myCoro_t;

    // 函数体的原型是被固定为void(T::caller_type&)类型的和作者原版不太一样
    myCoro_t coro([](myCoro_t::caller_type& caller){
        while(caller.has_result())
        {
            if (caller.get()=="exit")
                return;
            //std::cout << caller.get() << std::endl;
            caller("ok_" + caller.get());
        }
    },"this your arg");// 初始化可以不传入参数，但是调用get前需要判断has_result否则触发断言
    if (coro.has_result())
        std::cout << coro.get() << std::endl;

    coro("give you the arg again");
    if (coro.has_result())
        std::cout << coro.get() << std::endl;

    coro("exit");
    if (!coro)// 执行完毕或者被设置空后为false
        std::cout << "the coroutine is complete" << std::endl;

    return 0;
}
