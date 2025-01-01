#include "../thirdParty/httplib.h"

int main(int argc, char* argv) {
	httplib::Server server;

	server.Post("/hello", [&](const httplib::Request& req, httplib::Response& res) {
			std::cout << "Hello from client!" << std::endl;
            res.set_content("Hello from server!", "text/plain");
		});

	server.listen("localhost", 25565);

	return 0;
}