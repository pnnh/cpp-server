#include <msgpack.hpp>
#include <iostream>
#include <string>
#include <boost/thread.hpp>

boost::mutex mutex;
msgpack::sbuffer buffer;
int n = 80;

void hello()
{
    while (n-- > 0) {

        mutex.lock();

        msgpack::packer<msgpack::sbuffer> pk(&buffer);
        pk.pack(std::string("Log message ... " + std::to_string(n)));
        //pk.pack(std::string("Log message ... 2"));
        pk.pack(std::string("=================="));

        mutex.unlock();

        boost::this_thread::sleep(boost::posix_time::seconds(1));

    }
}

int main(void) {
    boost::thread thrd(hello);

    while(n > 0) {
        mutex.lock();

        msgpack::unpacker pac;

        // feeds the buffer.
        pac.reserve_buffer(buffer.size());
        memcpy(pac.buffer(), buffer.data(), buffer.size());
        pac.buffer_consumed(buffer.size());

        // now starts streaming deserialization.
        msgpack::object_handle oh;
        while(pac.next(oh)) {
            std::cout << oh.get() << std::endl;
        }

        mutex.unlock();
        boost::this_thread::sleep(boost::posix_time::seconds(2));
    }


    thrd.join();
}