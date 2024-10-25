#include "Spotify.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

/////////////////////////////////
// Retrieves details about a specific track by its ID.
//
// Parameters:
// - id: The Spotify track ID.
// - market: The Spotify market code (default is MARKET).
//
// Returns:
// - A JSON string containing track details or an error message.
std::string Spotify::_Tracks::getTrack(const std::string& id, const std::string& market) const
{
	std::string url = "https://api.spotify.com/v1/tracks/" + id + (!market.empty() ? "?market=" + market : "");

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves details about multiple tracks by their IDs.
//
// Parameters:
// - ids: A list of Spotify track IDs.
// - market: The Spotify market code (default is MARKET).
//
// Returns:
// - A JSON string containing details of the requested tracks or an error message.
std::string Spotify::_Tracks::getTracks(const std::vector<std::string>& ids, const std::string& market) const
{
	std::string url = "https://api.spotify.com/v1/tracks?ids=";

	for (std::string id : ids) {
		url += id + ",";
	}
	url.pop_back();
	if (!market.empty())
		url += "&market=" + market;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves the user's saved tracks.
//
// Parameters:
// - limit: Number of tracks to retrieve (default is 10).
// - offset: Index of the first track to retrieve (default is 0).
// - market: The Spotify market code (default is MARKET).
//
// Returns:
// - A JSON string containing saved tracks or an error message.
std::string Spotify::_Tracks::getSavedTracks(const int limit, const int offset, const std::string& market) const
{
	std::string url = "https://api.spotify.com/v1/me/tracks?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset) + (!market.empty() ? "?market=" + market : "");

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Saves tracks to the user's library.
//
// Parameters:
// - ids: A list of Spotify track IDs to save.
//
// Returns:
// - true if the tracks were successfully saved, otherwise false.
bool Spotify::_Tracks::saveTracks(const std::vector<std::string>& ids) const
{
	std::string url = "https://api.spotify.com/v1/me/tracks";

	Net net(url, "PUT");
	this->spotify->addDefaultHeaders(net);

	// Body
	std::string body = "{ \"ids\": [";
	for (std::string id : ids) {
		body += "\"" + id + "\",";
	}
	body = body.substr(0, body.length() - 1);
	body += "] }";
	net.addBody(body);

	RetErrorBool;
}

/////////////////////////////////
// Removes tracks from the user's library.
//
// Parameters:
// - ids: A list of Spotify track IDs to remove.
//
// Returns:
// - true if the tracks were successfully removed, otherwise false.
bool Spotify::_Tracks::unsaveTracks(const std::vector<std::string>& ids) const
{
	std::string url = "https://api.spotify.com/v1/me/tracks";

	Net net(url, "DELETE");
	this->spotify->addDefaultHeaders(net);

	// Body
	std::string body = "{ \"ids\": [";
	for (std::string id : ids) {
		body += "\"" + id + "\",";
	}
	body = body.substr(0, body.length() - 1);
	body += "] }";
	net.addBody(body);

	RetErrorBool;
}

/////////////////////////////////
// Checks if specific tracks are saved in the user's library.
//
// Parameters:
// - ids: A list of Spotify track IDs to check.
//
// Returns:
// - A vector of booleans indicating if each track is saved.
std::vector<bool> Spotify::_Tracks::getTracksIsSaved(const std::vector<std::string>& ids) const
{
	std::string url = "https://api.spotify.com/v1/me/tracks/contains?&ids=";
	for (std::string id : ids) {
		url += id + ",";
	}
	url.pop_back();

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

/////////////////////////////////
// Retrieves audio features for multiple tracks.
//
// Parameters:
// - ids: A list of Spotify track IDs.
//
// Returns:
// - A JSON string containing audio features for each track or an error message.
std::string Spotify::_Tracks::getMultipleTracksAudioFeatures(const std::vector<std::string>& ids) const
{
	std::string url = "https://api.spotify.com/v1/audio-features?ids=";

	for (std::string id : ids) {
		url += id + ",";
	}
	url.pop_back();

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves audio analysis data for a specific track.
//
// Parameters:
// - id: The Spotify track ID.
//
// Returns:
// - A JSON string containing detailed audio analysis or an error message.
std::string Spotify::_Tracks::getAudioAnalysis(const std::string& id) const
{
	std::string url = "https://api.spotify.com/v1/audio-analysis/" + id;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves audio features for a specific track.
//
// Parameters:
// - id: The Spotify track ID.
//
// Returns:
// - A JSON string containing audio features or an error message.
std::string Spotify::_Tracks::getAudioFeatures(const std::string& id) const
{
	std::string url = "https://api.spotify.com/v1/audio-features/" + id;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves recommended tracks based on seed tracks, artists, genres, and specified filters.
//
// Parameters:
// - seed_tracks: A list of seed track IDs.
// - seed_artists: A list of seed artist IDs.
// - seed_genres: A list of seed genres.
// - limit: The number of recommendations to retrieve.
// - market: The Spotify market code.
// - filter: An AudioFeaturesFilter struct containing target and range filters for track attributes.
//
// Returns:
// - A JSON string containing recommended tracks or an error message.
std::string Spotify::_Tracks::getRecommendations(const std::vector<std::string>& seed_tracks, const std::vector<std::string>& seed_artists, const std::vector<std::string>& seed_genres, const int limit, const std::string& market, AudioFeaturesFilter filter) const
{
	std::string url = "https://api.spotify.com/v1/recommendations?limit=" + std::to_string(limit) + "&market=" + market;
	if (!seed_tracks.empty())
	{
		url += "&seed_tracks=";
		for (std::string id : seed_tracks)
		{
			url += id + ",";
		}
		url.pop_back();
	}
	if (!seed_artists.empty())
	{
		url += "&seed_artists=";
		for (std::string id : seed_artists)
		{
			url += id + ",";
		}
		url.pop_back();
	}
	if (!seed_genres.empty())
	{
		url += "&seed_genres=";
		for (std::string id : seed_genres)
		{
			url += id + ",";
		}
		url.pop_back();
	}

	// Append filter parameters if specified
	if (filter.min_acousticness != -1) url += "&min_acousticness=" + std::to_string(filter.min_acousticness);
	// Similar lines for other filter attributes...

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}
