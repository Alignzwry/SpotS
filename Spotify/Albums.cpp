#include "Spotify.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

/////////////////////////////////
// Retrieves detailed information for a specific album.
//
// Parameters:
// - id: The Spotify ID of the album.
// - market: The market code (e.g., "US") to retrieve album availability in a specific region.
//
// Returns:
// - A JSON string with album details if the request is successful, or an empty string if there’s an error.
std::string Spotify::_Albums::getAlbum(const std::string& id, const std::string& market) const
{
	std::string url = "https://api.spotify.com/v1/albums/" + id + "?market=" + market;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves information for multiple albums at once.
//
// Parameters:
// - ids: A vector of Spotify album IDs to retrieve information for.
// - market: The market code (e.g., "US") to get album availability in a specific region.
//
// Returns:
// - A JSON string with details for each album if the request is successful, or an empty string if there’s an error.
std::string Spotify::_Albums::getAlbums(const std::vector<std::string>& ids, const std::string& market) const
{
	std::string url = "https://api.spotify.com/v1/albums?ids=";
	for (std::string id : ids) {
		url += id + ",";
	}
	url = url.substr(0, url.length() - 1);
	url += "&market=" + market;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves track information for a specific album, with pagination support.
//
// Parameters:
// - id: The Spotify ID of the album.
// - limit: The maximum number of tracks to return (default is 20, max is 50).
// - offset: The index of the first track to return (useful for pagination).
// - market: The market code to get track availability in a specific region.
//
// Returns:
// - A JSON string containing the album tracks if the request is successful, or an empty string if there’s an error.
std::string Spotify::_Albums::getAlbumTracks(const std::string& id, const int limit, const int offset, const std::string& market) const
{
	std::string url = "https://api.spotify.com/v1/albums/" + id + "/tracks?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset) + "&market=" + market;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves a list of the user's saved albums with pagination support.
//
// Parameters:
// - limit: The maximum number of albums to return (default is 20, max is 50).
// - offset: The index of the first album to return (useful for pagination).
// - market: The market code to get album availability in a specific region.
//
// Returns:
// - A JSON string containing the user's saved albums if the request is successful, or an empty string if there’s an error.
std::string Spotify::_Albums::getUserSavedAlbums(const int limit, const int offset, const std::string& market) const
{
	std::string url = "https://api.spotify.com/v1/me/albums?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset) + "&market=" + market;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Saves one or more albums to the user's library.
//
// Parameters:
// - ids: A vector of Spotify album IDs to save.
//
// Returns:
// - true if the albums were successfully saved, otherwise false.
bool Spotify::_Albums::saveAlbums(const std::vector<std::string>& ids) const
{
	std::string url = "https://api.spotify.com/v1/me/albums?ids=";
	for (std::string id : ids) {
		url += id + ",";
	}
	url = url.substr(0, url.length() - 1);

	Net net(url, "PUT");
	this->spotify->addTokenHeaderOnly(net);

	RetErrorBool;
}

/////////////////////////////////
// Removes one or more albums from the user's library.
//
// Parameters:
// - ids: A vector of Spotify album IDs to remove.
//
// Returns:
// - true if the albums were successfully removed, otherwise false.
bool Spotify::_Albums::unsaveAlbums(const std::vector<std::string>& ids) const
{
	std::string url = "https://api.spotify.com/v1/me/albums?ids=";
	for (std::string id : ids) {
		url += id + ",";
	}
	url = url.substr(0, url.length() - 1);

	Net net(url, "DELETE");
	this->spotify->addTokenHeaderOnly(net);

	RetErrorBool;
}

/////////////////////////////////
// Checks if one or more albums are saved in the user's library.
//
// Parameters:
// - ids: A vector of Spotify album IDs to check.
//
// Returns:
// - A vector of booleans indicating if each album is saved (true) or not (false).
std::vector<bool> Spotify::_Albums::getAlbumsIsSaved(const std::vector<std::string>& ids) const
{
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
