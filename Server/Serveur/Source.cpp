#include "Server.h"
#include "Group.h"
#include "UserManager.h"
#include "ReaderFromClient.h"
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

int main() {
	boost::asio::io_service* io_service = new boost::asio::io_service();
    /*File fileTest{ "Test", ".txt", "C:\\", 1U };
    File fileTest2{ "Test", ".txt", "C:\\", 2U };
    Group bob{ 3U, "bob", "Jaime les chats", 4U};

    map<File, unsigned int> files;
    files.insert(make_pair(fileTest, 5U));
    files.insert(make_pair(fileTest2, 6U));

    ShareFolder share{7U, "test2", files};

    rapidjson::StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);



    writer.StartArray();
    fileTest.serialize(writer);
    bob.serialize(writer);
    share.serialize(writer);
    writer.EndArray();
    
    puts(sb.GetString());*/

    /*string json = " { \"function\":\"Register\", \"username\":\"jjjj\", \"password\":\"bob\" } ";

    string test = Server::LookUpClientFunction(json);*/

	

	Server *server = new Server(io_service);
	Server::initializeManager();
	server->run();

    while (true) {
        Server::synchronize();
        this_thread::sleep_for(std::chrono::milliseconds(15000));
    }
}