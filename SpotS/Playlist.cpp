#include "Spotify.h"

/////////////////////////////////
// Fetches detailed information of a specific playlist.
//
// Parameters:
// - id: The Spotify playlist ID.
// - market: Optional market (country code) to adjust availability filtering.
// - fields: Optional fields parameter to specify the playlist data to retrieve.
// - additional_types: Optional additional types parameter.
//
// Returns:
// - A JSON string with the playlist's details if successful, else an error string.
std::string Spotify::_Playlists::getPlaylist(const std::string& id, const std::string& market, const std::string& fields, const std::string& additional_types) const
{
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

	RetErrorString;
}

/////////////////////////////////
// Updates details of an existing playlist.
//
// Parameters:
// - id: The Spotify playlist ID.
// - name: New name for the playlist.
// - description: New description for the playlist.
// - is_public: Sets playlist visibility (true for public, false for private).
//
// Returns:
// - A JSON string with the updated playlist details if successful, else an error string.
std::string Spotify::_Playlists::updateDetails(const std::string& id, const std::string& name, const std::string& description, const int is_public) const
{
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

	RetErrorString;
}

/////////////////////////////////
// Retrieves items (tracks) in a specified playlist.
//
// Parameters:
// - id: The Spotify playlist ID.
// - limit: The number of items to return (max 100).
// - offset: The index of the first item to return.
// - market, fields, additional_types: Optional parameters for filtering and response customization.
//
// Returns:
// - A JSON string with playlist items if successful, else an error string.
std::string Spotify::_Playlists::getItems(const std::string& id, const int limit, const int offset, const std::string& market, const std::string& fields, const std::string& additional_types) const
{
	std::string url = "https://api.spotify.com/v1/playlists/" + id + "/tracks?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset);
	if (!market.empty())
		url += "&market=" + market;
	if (!fields.empty())
		url += "&fields=" + fields;
	if (!additional_types.empty())
		url += "&additional_types=" + additional_types;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Updates the positions of items within a playlist.
//
// Parameters:
// - id: The Spotify playlist ID.
// - ids: List of track URIs to reorder.
// - range_start: Start position of the range to move.
// - insert_before: Position where range should be inserted.
// - range_length: Length of the range to be moved.
// - snapshot_id: Optional snapshot ID for playlist consistency.
//
// Returns:
// - true if items were successfully updated, false otherwise.
bool Spotify::_Playlists::updateItems(const std::string& id, const std::vector<std::string>& ids, const int range_start, const int insert_before, const int range_length, const std::string& snapshot_id) const
{
	std::string url = "https://api.spotify.com/v1/playlists/" + id + "/tracks";

	Net net(url, "PUT");
	this->spotify->addDefaultHeaders(net);

	// Body
	std::string body = "{ \"uris\": [";
	for (std::string id : ids) {
		body += "\"spotify:track:" + id + "\",";
	}
	body = body.substr(0, body.length() - 1) + "]";
	if (!snapshot_id.empty())
		body += ", \"snapshot_id\": \"" + snapshot_id + "\"";
	body += ", \"range_start\": " + std::to_string(range_start);
	body += ", \"insert_before\": " + std::to_string(insert_before);
	body += ", \"range_length\": " + std::to_string(range_length) + " }";

	net.addBody(body);

	RetErrorBool;
}

/////////////////////////////////
// Adds items to a playlist.
//
// Parameters:
// - id: The Spotify playlist ID.
// - ids: List of track URIs to add.
// - position: The position to insert the items (optional).
//
// Returns:
// - true if items were successfully added, false otherwise.
bool Spotify::_Playlists::addItems(const std::string& id, const std::vector<std::string>& ids, const int position) const
{
	std::string url = "https://api.spotify.com/v1/playlists/" + id + "/tracks";

	Net net(url, "POST");
	this->spotify->addDefaultHeaders(net);

	// Body
	std::string body = "{ \"uris\": [";
	for (std::string id : ids) {
		body += "\"spotify:track:" + id + "\",";
	}
	body = body.substr(0, body.length() - 1) + "]";
	if (position != -1)
		body += ", \"position\": " + std::to_string(position);
	body += " }";

	net.addBody(body);

	RetErrorBool;
}

/////////////////////////////////
// Removes items from a playlist.
//
// Parameters:
// - id: The Spotify playlist ID.
// - ids: List of track URIs to remove.
// - snapshot_id: Optional snapshot ID to ensure playlist consistency.
//
// Returns:
// - true if items were successfully removed, false otherwise.
bool Spotify::_Playlists::removeItems(const std::string& id, const std::vector<std::string>& ids, const std::string& snapshot_id) const
{
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

	net.addBody(body);

	RetErrorBool;
}

/////////////////////////////////
// Retrieves playlists of the current user.
//
// Parameters:
// - limit: Maximum number of playlists to return.
// - offset: The index of the first playlist to return.
//
// Returns:
// - A JSON string with the user's playlists if successful, else an error string.
std::string Spotify::_Playlists::getCurrentUserPlaylists(const int limit, const int offset) const
{
	std::string url = "https://api.spotify.com/v1/me/playlists?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset);

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves playlists of a specific user.
//
// Parameters:
// - user_id: The Spotify user ID.
// - limit: Maximum number of playlists to return.
// - offset: The index of the first playlist to return.
//
// Returns:
// - A JSON string with the user's playlists if successful, else an error string.
std::string Spotify::_Playlists::getUsersPlaylist(const std::string& user_id, const int limit, const int offset) const
{
	std::string url = "https://api.spotify.com/v1/users/" + user_id + "/playlists?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset);

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Creates a new playlist for a specific user.
//
// Parameters:
// - user_id: The Spotify user ID.
// - name: The name of the new playlist.
// - description: The description for the playlist.
// - is_public: Sets playlist visibility (true for public, false for private).
//
// Returns:
// - true if the playlist was successfully created, false otherwise.
bool Spotify::_Playlists::createPlaylist(const std::string& user_id, const std::string& name, const std::string& description, const int is_public) const
{
	std::string url = "https://api.spotify.com/v1/users/" + user_id + "/playlists";

	Net net(url, "POST");
	this->spotify->addDefaultHeaders(net);

	// Body
	std::string body = "{ \"name\": \"" + name + "\"";
	if (!description.empty())
		body += ", \"description\": \"" + description + "\"";
	if (is_public != -1)
		body += ", \"public\": " + std::string(is_public ? "true" : "false");
	body += " }";

	net.addBody(body);

	RetErrorBool;
}

/////////////////////////////////
// Retrieves featured playlists.
//
// Parameters:
// - locale: Optional locale to get specific playlists based on location.
// - limit: The number of playlists to return.
// - offset: The index of the first playlist to return.
//
// Returns:
// - A JSON string with featured playlists if successful, else an error string.
std::string Spotify::_Playlists::getFeaturesPlaylists(const std::string& locale, const int limit, const int offset) const
{
	std::string url = "https://api.spotify.com/v1/browse/featured-playlists?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset) + (locale.empty() ? "" : "&locale=" + locale);

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves playlists for a specific category.
//
// Parameters:
// - category: The category ID for which to retrieve playlists.
// - limit: The number of playlists to return.
// - offset: The index of the first playlist to return.
//
// Returns:
// - A JSON string with playlists of the specified category if successful, else an error string.
std::string Spotify::_Playlists::getCategoriesPlaylists(const std::string& category, const int limit, const int offset) const
{
	std::string url = "https://api.spotify.com/v1/browse/categories/" + category + "/playlists?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset);

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves the cover image of a playlist.
//
// Parameters:
// - id: The Spotify playlist ID.
//
// Returns:
// - A JSON string with the playlist's cover image details if successful, else an error string.
std::string Spotify::_Playlists::getPlaylistCover(const std::string& id) const
{
	std::string url = "https://api.spotify.com/v1/playlists/" + id + "/images";

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Sets a new cover image for a playlist using base64-encoded image data.
//
// Parameters:
// - id: The Spotify playlist ID.
// - base64: Base64-encoded string of the image.
//
// Returns:
// - true if the cover image was successfully set, false otherwise.
bool Spotify::_Playlists::setPlaylistCover(const std::string& id, const std::string& base64) const
{
	std::string url = "https://api.spotify.com/v1/users/" + id + "/images";

	Net net(url, "PUT");
	this->spotify->addDefaultHeaders(net);

	net.addBody(base64);

	RetErrorBool;
}
