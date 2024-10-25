#include "Spotify.h"
#include "StrUtil.h"
#include <assert.h>


/////////////////////////////////
std::string Spotify::_Artists::getArtist(const std::string& id) const {
	std::string url = "https://api.spotify.com/v1/artists/" + id;

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
std::string Spotify::_Artists::getArtists(const std::vector<std::string>& ids) const {
	std::string url = "https://api.spotify.com/v1/artists?ids=";
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
		return std::string();
	}

	return response;

}

/////////////////////////////////
std::string Spotify::_Artists::getArtistAlbums(const std::string& id, const int limit, const int offset, const std::string& market) const {
	std::string url = "https://api.spotify.com/v1/artists/" + id + "/albums?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset) + "&market=" + market;

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
std::string Spotify::_Artists::getArtistTopTracks(const std::string& id, const std::string& market) const {
	std::string url = "https://api.spotify.com/v1/artists/" + id + "/top-tracks?market=" + market;

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
std::string Spotify::_Artists::getArtistRelatedArtists(const std::string& id) const {
	std::string url = "https://api.spotify.com/v1/artists/" + id + "/related-artists";

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