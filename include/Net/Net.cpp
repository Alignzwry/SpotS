#include "net.h"
#include <stdexcept>

Net::Net(std::string _url, std::string _method) {
    this->curl = curl_easy_init();
    if (!this->curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }
    this->url = _url;
	this->method = _method;
}

Net::~Net() {
    curl_easy_cleanup(curl);
}

// Add headers to the request
void Net::addHeader(std::string header) {
    this->headers.push_back(header);
}

// Add headers to the request
void Net::addHeader(std::vector<std::string>& _headers) {
    for (auto& header : _headers) {
        this->headers.push_back(header);
    }
}

// Add a body to the request
void Net::addBody(std::string _body) {
    this->body = _body;
}


// Write callback function to store the response in a string
inline size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    s->append((char*)contents, newLength);
    return newLength;
}

// Write callback function to store the response in a buffer
inline size_t WriteCallbackBin(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::vector<unsigned char>* buffer = (std::vector<unsigned char>*)userp;
    buffer->insert(buffer->end(), (unsigned char*)contents, (unsigned char*)contents + totalSize);
    return totalSize;
}

CURLcode Net::send(std::vector<unsigned char>& response) {
    CURLcode res = CURLE_OK;

    // Set the URL for the request
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Set the method
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());

    // Specify custom headers
    struct curl_slist* _headers = NULL;
    for (std::string& header : headers) {
        _headers = curl_slist_append(_headers, header.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, _headers);

    // Set the callback function to store the response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackBin);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Disable verbose output
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

    // Perform the request
    res = curl_easy_perform(curl);

    // Cleanup
    curl_slist_free_all(_headers);
    curl_easy_reset(curl);

    // Clear headers and body
    headers.clear();
    body.clear();

    // Return
    return res;
}

long Net::send(std::string& response) {

    // Set the URL for the request
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Set the method
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());

    // Set the POST data
    if (method != "GET") {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    }

    // Specify custom headers
    struct curl_slist* _headers = NULL;
    for (std::string& header : headers) {
        _headers = curl_slist_append(_headers, header.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, _headers);

    // Set the callback function to store the response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);

    long Return = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &Return);

    // Cleanup
    curl_slist_free_all(_headers);
    curl_easy_reset(curl);
    headers.clear();
    body.clear();

    //return
    return res == CURLE_OK ? Return : -1;
}


// Static function

std::string Net::url_encode(const std::string& decoded)
{
    const auto encoded_value = curl_easy_escape(nullptr, decoded.c_str(), static_cast<int>(decoded.length()));
    std::string result(encoded_value);
    curl_free(encoded_value);
    return result;
}

std::string Net::url_decode(const std::string& encoded)
{
    int output_length;
    const auto decoded_value = curl_easy_unescape(nullptr, encoded.c_str(), static_cast<int>(encoded.length()), &output_length);
    std::string result(decoded_value, output_length);
    curl_free(decoded_value);
    return result;
}