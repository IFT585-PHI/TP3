#include "Server.h"
#include <chrono>
#include <thread>

int main() {
    Server::initializeManager();

    while (true) {
        Server::synchronize();
        this_thread::sleep_for(std::chrono::milliseconds(15000));
    }
}