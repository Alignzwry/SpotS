#include "Spotify.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

/////////////////////////////////
// Fetches a single episode's details from Spotify by episode ID.
//
// Parameters:
// - id: The unique Spotify ID of the episode to retrieve.
// - market: The market (country code) to filter the episode availability (optional).
//
// Returns:
// - JSON response as a string containing the episode details, or an empty string if unsuccessful.
std::string Spotify::_Episodes::getEpisode(const std::string& id, const std::string& market) const
{
	std::string url = "https://api.spotify.com/v1/episodes/" + id;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Fetches multiple episodes' details by a list of episode IDs.
//
// Parameters:
// - ids: A vector of Spotify episode IDs to retrieve details for.
// - market: The market (country code) to filter episode availability (optional).
//
// Returns:
// - JSON response as a string containing details of the episodes, or an empty string if unsuccessful.
std::string Spotify::_Episodes::getEpisodes(const std::vector<std::string>& ids, const std::string& market) const
{
	std::string url = "https://api.spotify.com/v1/episodes?ids=";
	for (std::string id : ids) {
		url += id + ",";
	}
	url = url.substr(0, url.length() - 1);

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Fetches the list of episodes saved by the current user.
//
// Parameters:
// - limit: The maximum number of saved episodes to retrieve (1-50).
// - offset: The index of the first item to retrieve (for pagination).
// - market: The market (country code) to filter episode availability.
//
// Returns:
// - JSON response as a string containing saved episodes, or an empty string if unsuccessful.
std::string Spotify::_Episodes::getSavedEpisodes(const int limit, const int offset, const std::string& market) const
{
	std::string url = "https://api.spotify.com/v1/me/episodes?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset) + "&market=" + market;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Saves multiple episodes to the user's library by their Spotify IDs.
//
// Parameters:
// - ids: A vector of Spotify episode IDs to save.
//
// Returns:
// - true if the episodes were successfully saved, otherwise false.
bool Spotify::_Episodes::saveEpisodes(const std::vector<std::string>& ids) const
{
	std::string url = "https://api.spotify.com/v1/me/episodes?ids=";
	for (std::string id : ids) {
		url += id + ",";
	}
	url = url.substr(0, url.length() - 1);

	Net net(url, "PUT");
	this->spotify->addTokenHeaderOnly(net);

	RetErrorBool;
}

/////////////////////////////////
// Removes multiple episodes from the user's saved library by their Spotify IDs.
//
// Parameters:
// - ids: A vector of Spotify episode IDs to unsave.
//
// Returns:
// - true if the episodes were successfully removed, otherwise false.
bool Spotify::_Episodes::unsaveEpisodes(const std::vector<std::string>& ids) const
{
	std::string url = "https://api.spotify.com/v1/me/episodes?ids=";
	for (std::string id : ids) {
		url += id + ",";
	}
	url = url.substr(0, url.length() - 1);

	Net net(url, "DELETE");
	this->spotify->addTokenHeaderOnly(net);

	RetErrorBool;
}

/////////////////////////////////
// Checks if multiple episodes are saved in the user's library.
//
// Parameters:
// - ids: A vector of Spotify episode IDs to check.
//
// Returns:
// - A vector of booleans indicating if each episode is saved (true) or not (false),
//   or an empty vector if the request is unsuccessful.
std::vector<bool> Spotify::_Episodes::getEpisodesIsSaved(const std::vector<std::string>& ids) const
{
	std::string url = "https://api.spotify.com/v1/me/episodes/contains?ids=";
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
