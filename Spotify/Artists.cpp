#include "Spotify.h"

/////////////////////////////////
// Retrieves information about a specific artist by their Spotify ID.
//
// Parameters:
// - id: The unique Spotify ID of the artist.
//
// Returns:
// - A JSON string containing artist information if the request is successful, or an empty string on failure.
std::string Spotify::_Artists::getArtist(const std::string& id) const
{
	std::string url = "https://api.spotify.com/v1/artists/" + id;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves information about multiple artists based on their Spotify IDs.
//
// Parameters:
// - ids: A vector of Spotify artist IDs.
//
// Returns:
// - A JSON string containing information about the artists if the request is successful, or an empty string on failure.
std::string Spotify::_Artists::getArtists(const std::vector<std::string>& ids) const
{
	std::string url = "https://api.spotify.com/v1/artists?ids=";
	for (std::string id : ids) {
		url += id + ",";
	}
	url = url.substr(0, url.length() - 1); // Remove trailing comma

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves albums from a specific artist.
//
// Parameters:
// - id: The Spotify artist ID.
// - limit: The maximum number of albums to retrieve (default max: 50).
// - offset: The index of the first item to return.
// - market: The Spotify market (e.g., "US") to filter the albums.
//
// Returns:
// - A JSON string with album information if the request is successful, or an empty string on failure.
std::string Spotify::_Artists::getArtistAlbums(const std::string& id, const int limit, const int offset, const std::string& market) const
{
	std::string url = "https://api.spotify.com/v1/artists/" + id + "/albums?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset) + "&market=" + market;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves the top tracks for a specific artist based on the market.
//
// Parameters:
// - id: The Spotify artist ID.
// - market: The market (e.g., "US") in which to filter the top tracks.
//
// Returns:
// - A JSON string with the top track information if the request is successful, or an empty string on failure.
std::string Spotify::_Artists::getArtistTopTracks(const std::string& id, const std::string& market) const
{
	std::string url = "https://api.spotify.com/v1/artists/" + id + "/top-tracks?market=" + market;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves a list of artists related to a specific artist.
//
// Parameters:
// - id: The Spotify artist ID.
//
// Returns:
// - A JSON string with related artist information if the request is successful, or an empty string on failure.
std::string Spotify::_Artists::getArtistRelatedArtists(const std::string& id) const
{
	std::string url = "https://api.spotify.com/v1/artists/" + id + "/related-artists";

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}
