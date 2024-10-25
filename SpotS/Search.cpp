#include "Spotify.h"

/////////////////////////////////
// Performs a search query on the Spotify API for various types of items (e.g., tracks, artists, albums).
//
// Parameters:
// - q: The search query string (e.g., song name, artist name).
// - types: A list of item types to search for (e.g., "track", "artist").
// - limit: Maximum number of results to return.
// - offset: The index of the first result to return (used for pagination).
// - market: Optional. A country code (e.g., "US") to filter the search results by availability.
//
// Returns:
// - A JSON string containing the search results if successful; otherwise, an error string.
std::string Spotify::_Search::search(const std::string& q, const std::vector<const char*>& types, const int limit, const int offset, const std::string& market) const {
	std::string url = "https://api.spotify.com/v1/search?q=" + q + "&limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset) + (market.empty() ? "" : "&market=" + market);

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}
