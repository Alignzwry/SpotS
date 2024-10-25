#pragma once
#include <string>
#include <vector>
#include <curl/curl.h>


class Net {
public:
	Net(std::string url, std::string method = "GET");
	~Net();

	void addHeader(std::string header);
	void addHeader(std::vector<std::string>& headers);

	void addBody(std::string body);

	long send(std::string& response);
	CURLcode send(std::vector<unsigned char>& response);

	// Static functions
	static std::string url_encode(const std::string& decoded);
	static std::string url_decode(const std::string& encoded);

private:
	std::string url;
	std::vector<std::string> headers;
	std::string body;
	std::string method;
	CURL* curl = nullptr;
};