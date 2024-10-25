#include "Spotify.h"
#include "StrUtil.h"
#include <assert.h>


std::string Spotify::_Playlists::getPlaylist(const std::string& id, const std::string& market, const std::string& fields, const std::string& additional_types) const {
	std::string url = "https://api.spotify.com/v1/playlists/" + id;
	bool has_params = false;
	if (!market.empty()) {
		url += "?market=" + market;
		has_params = true;
	}
	if (!fields.empty()) {
		if (has_params)
			url += "&";
		else
			url += "?";
		url += "fields=" + fields;
		has_params = true;
	}
	if (!additional_types.empty()) {
		if (has_params)
			url += "&";
		else
			url += "?";
		url += "additional_types=" + additional_types;
	}

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

std::string Spotify::_Playlists::updateDetails(const std::string& id, const std::string& name, const std::string& description, const int is_public) const {
	std::string url = "https://api.spotify.com/v1/playlists/" + id;

	Net net(url, "PUT");
	this->spotify->addDefaultHeaders(net);

	// Body
	std::string body = "{ ";
	if (!name.empty())
		body += "\"name\": \"" + name + "\"";
	if (!description.empty())
		body += ", \"description\": \"" + description + "\"";
	if (is_public != -1)
		body += ", \"public\": " + std::string(is_public ? "true" : "false");
	body += " }";

	net.addBody(body);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return std::string();
	}

	return response;
}

std::string Spotify::_Playlists::getItems(const std::string& id, const int limit, const int offset, const std::string& market, const std::string& fields, const std::string& additional_types) const {
	std::string url = "https://api.spotify.com/v1/playlists/" + id + "/tracks?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset);
	if (!market.empty())
		url += "&market=" + market;
	if (!fields.empty())
		url += "&fields=" + fields;
	if (!additional_types.empty())
		url += "&additional_types=" + additional_types;

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

bool Spotify::_Playlists::updateItems(const std::string& id, const std::vector<std::string>& ids, const int range_start, const int insert_before, const int range_length, const std::string& snapshot_id) const {
	std::string url = "https://api.spotify.com/v1/playlists/" + id + "/tracks";

	Net net(url, "PUT");
	this->spotify->addDefaultHeaders(net);

	// Body
	std::string body = "{ \"uris\": [";

	for (std::string id : ids) {
		body += "spotify:track:" + id + "\",\"";
	}
	body = body.substr(0, body.length() - 2) + "]";
	if (!snapshot_id.empty())
		body += ", \"snapshot_id\": \"" + snapshot_id + "\"";
	body += ", \"range_start\": " + std::to_string(range_start);
	body += ", \"insert_before\": " + std::to_string(insert_before);
	body += ", \"range_length\": " + std::to_string(range_length) + " }";

	net.addBody(body);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return false;
	}

	return true;
}

#include <iostream>
bool Spotify::_Playlists::addItems(const std::string& id, const std::vector<std::string>& ids, const int position) const {
	std::string url = "https://api.spotify.com/v1/playlists/" + id + "/tracks";

	Net net(url, "POST");
	this->spotify->addDefaultHeaders(net);

	// Body
	std::string body = "{ \"uris\": [";

	for (std::string id : ids) {
		body += "\"spotify:track:" + id + "\",\"";
	}
	body = body.substr(0, body.length() - 2) + "]";
	if (position != -1)
		body += ", \"position\": " + std::to_string(position);
	body += " }";

	net.addBody(body);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return false;
	}

	return true;
}

bool Spotify::_Playlists::removeItems(const std::string& id, const std::vector<std::string>& ids, const std::string& snapshot_id) const {
	std::string url = "https://api.spotify.com/v1/playlists/" + id + "/tracks";

	Net net(url, "DELETE");
	this->spotify->addDefaultHeaders(net);

	// Body
	std::string body = "{ \"tracks\": [";

	for (std::string id : ids) {
		body += "{\"uri\": \"spotify:track:" + id + "\"}, ";
	}
	body = body.substr(0, body.length() - 2) + "]";
	if (!snapshot_id.empty())
		body += ", \"snapshot_id\": \"" + snapshot_id + "\"";
	body += " }";
	
	std::cout << body << std::endl;

	net.addBody(body); 

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return false;
	}

	return true;
}

std::string Spotify::_Playlists::getCurrentUserPlaylists(const int limit, const int offset) {

	return "";
}

std::string Spotify::_Playlists::getUsersPlaylist(const std::string& user_id, const int limit, const int offset) {

	return "";
}

bool Spotify::_Playlists::createPlaylist(const std::string& user_id, const std::string& name, const std::string& description, const bool is_public) const {

	return false;
}

std::string Spotify::_Playlists::getFeaturesPlaylists(const std::string& locale, const int limit, const int offset) const {

	return "";
}

std::string Spotify::_Playlists::getCategoriesPlaylists(const std::string& category, const int limit, const int offset) const {

	return "";
}

std::string Spotify::_Playlists::getPlaylistCover(const std::string& id) const {

	return "";
}

bool Spotify::_Playlists::setPlaylistCover(const std::string& id, const std::string& base64) const {

	return false;
}