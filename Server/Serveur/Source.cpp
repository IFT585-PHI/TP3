#include "Server.h"
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

int main() {
	boost::asio::io_service io_service;
	Server *server = new Server(io_service);
	server->start_accept();
    Server::initializeManager();

    while (true) {
        Server::synchronize();
        this_thread::sleep_for(std::chrono::milliseconds(15000));
    }
}