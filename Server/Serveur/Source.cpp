#include "Server.h"
#include "Group.h"
#include "UserManager.h"
#include "ReaderFromClient.h"
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

int main() {
	boost::asio::io_service* io_service = new boost::asio::io_service();

	Server *server = new Server(io_service);
	server->run();

    while (true) {
        this_thread::sleep_for(std::chrono::milliseconds(15000));
    }
}