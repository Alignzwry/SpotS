#include "Spotify.h"
#include "StrUtil.h"
#include <assert.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


/////////////////////////////////
std::string Spotify::_Albums::getAlbum(const std::string& id, const std::string& market) const {
	std::string url = "https://api.spotify.com/v1/albums/" + id + "?market=" + market;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return std::string();
	}

	return response;
}

/////////////////////////////////
std::string Spotify::_Albums::getAlbums(const std::vector<std::string>& ids, const std::string& market) const {
	std::string url = "https://api.spotify.com/v1/albums?ids=";
	for (std::string id : ids) {
		url += id + ",";
	}
	url = url.substr(0, url.length() - 1);
	url += "&market=" + market;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return std::string();
	}

	return response;
}

/////////////////////////////////
std::string Spotify::_Albums::getAlbumTracks(const std::string& id, const int limit, const int offset, const std::string& market) const {
	std::string url = "https://api.spotify.com/v1/albums/" + id + "/tracks?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset) + "&market=" + market;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return std::string();
	}

	return response;
}

/////////////////////////////////
std::string Spotify::_Albums::getUserSavedAlbums(const int limit, const int offset, const std::string& market) const {

	std::string url = "https://api.spotify.com/v1/me/albums?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset) + "&market=" + market;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return std::string();
	}

	return response;
}

/////////////////////////////////
bool Spotify::_Albums::saveAlbums(const std::vector<std::string>& ids) const {
	std::string url = "https://api.spotify.com/v1/me/albums?ids=";
	for (std::string id : ids) {
		url += id + ",";
	}
	url = url.substr(0, url.length() - 1);

	Net net(url, "PUT");
	this->spotify->addTokenHeaderOnly(net);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return false;
	}

	return true;
}

/////////////////////////////////
bool Spotify::_Albums::unsaveAlbums(const std::vector<std::string>& ids) const {
	std::string url = "https://api.spotify.com/v1/me/albums?ids=";
	for (std::string id : ids) {
		url += id + ",";
	}
	url = url.substr(0, url.length() - 1);

	Net net(url, "DELETE");
	this->spotify->addTokenHeaderOnly(net);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return false;
	}

	return true;
}

/////////////////////////////////
std::vector<bool> Spotify::_Albums::getAlbumsIsSaved(const std::vector<std::string>& ids) const {
	std::string url = "https://api.spotify.com/v1/me/albums/contains?ids=";
	for (std::string id : ids) {
		url += id + ",";
	}
	url = url.substr(0, url.length() - 1);

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return std::vector<bool>();
	}

	try {
		json j = response;
		std::vector<bool> result;
		for (json::iterator it = j.begin(); it != j.end(); ++it) {
			result.push_back(*it);
		}
		return result;
	}
	catch (...) {
		RetError;
		return std::vector<bool>();
	}
}