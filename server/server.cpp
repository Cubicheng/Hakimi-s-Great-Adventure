#include "../thirdParty/httplib.h"

#include <mutex>
#include <iostream>
#include <string>
#include <fstream>

std::mutex g_mutex;

std::string str_text;

int progress_1 = -1;
int progress_2 = -1;

int main(int argc, char* argv) {

	std::ifstream file("text.txt");

	if (!file.good()) {
		MessageBox(nullptr, L"无法打开文本文档 text.txt！", L"启动失败！", MB_ICONERROR);
		return -1;
	}

	std::stringstream str_stream;
    str_stream << file.rdbuf();
    str_text = str_stream.str();

	file.close();

	httplib::Server server;

	server.Post("/login", [&](const httplib::Request& req, httplib::Response& res) {
		std::lock_guard<std::mutex> lock(g_mutex);

		if (progress_1 >= 0 && progress_2 >= 0) {
			res.set_content("-1", "text/plain");
			return;
		}

		res.set_content(progress_1 == -1 ? "1" : "2", "text/plain");
		progress_1 == -1 ? progress_1 = 0 : progress_2 = 0;

		});

	server.Post("/query_text",[&](const httplib::Request& req, httplib::Response& res) {
		res.set_content(str_text, "text/plain");
		});

	server.Post("/update_1", [&](const httplib::Request& req, httplib::Response& res) {
		std::lock_guard<std::mutex> lock(g_mutex);
		progress_1 = std::stoi(req.body);
		res.set_content(std::to_string(progress_2), "text/plain");
		});

	server.Post("/update_2", [&](const httplib::Request& req, httplib::Response& res) {
		std::lock_guard<std::mutex> lock(g_mutex);
		progress_2 = std::stoi(req.body);
		res.set_content(std::to_string(progress_1), "text/plain");
		});


	std::cout << "Server is running on port 25565" << std::endl;

    server.listen("0.0.0.0", 25565);

	return 0;
}