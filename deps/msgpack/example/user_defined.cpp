#include <msgpack.hpp>
#include <vector>
#include <string>
#include <iostream>

class myclass {
public:
    std::string m_str;
    std::vector<int> m_vec;
    MSGPACK_DEFINE(m_str, m_vec);
};


int main(void) {
    std::vector<myclass> vec;
    // add some elements into vec...
    myclass mc1;
    mc1.m_str = "aa";
    mc1.m_vec.push_back(123);
    mc1.m_vec.push_back(456);

    vec.push_back(mc1);

    // you can serialize myclass directly
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, vec);

    msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());

    msgpack::object obj = oh.get();

    // you can convert object to myclass directly
    std::vector<myclass> rvec;
    obj.convert(rvec);

    myclass mc2 = rvec.at(0);

    std::cout << mc2.m_str << mc2.m_vec.at(1) << std::endl;
}