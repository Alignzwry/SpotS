#include "Spotify.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

/////////////////////////////////
// Retrieves the current user's profile information.
//
// Returns:
// - A JSON string containing details of the authenticated user's profile, such as display name and email.
std::string Spotify::_Users::getCurrentUser() const
{
	std::string url = "https://api.spotify.com/v1/me";

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Fetches the user's top items (artists or tracks) over a specified time range.
//
// Parameters:
// - type: Specifies if the top item should be "artists" or "tracks".
// - time_range: The time range over which to calculate top items (Short_term, Medium_term, Long_term).
// - limit: Number of items to retrieve (maximum Spotify allows is 50).
// - offset: The index of the first item to return.
//
// Returns:
// - A JSON string with details of the user's top items.
std::string Spotify::_Users::getTopItem(const ITEM_TYPE type, const TIME_RANGE time_range, const int limit, const int offset) const
{
	std::string url = "https://api.spotify.com/v1/me/top/" + std::string(type == ITEM_TYPE::Artist ? "artists" : "tracks") + "?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset) + "&time_range=" + std::string(time_range == TIME_RANGE::Short_term ? "short_term" : time_range == TIME_RANGE::Medium_term ? "medium_term" : "long_term");

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves another user's profile information.
//
// Parameters:
// - user_id: The Spotify ID of the user whose profile information is requested.
//
// Returns:
// - A JSON string containing details of the specified user's profile.
std::string Spotify::_Users::getUsersProfile(const std::string& user_id) const
{
	std::string url = "https://api.spotify.com/v1/users/" + user_id;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Follows a specific playlist.
//
// Parameters:
// - playlist_id: The ID of the playlist to follow.
// - is_public: Specifies whether the playlist should be public (default is -1, which leaves current setting unchanged).
//
// Returns:
// - true if the operation is successful; false otherwise.
bool Spotify::_Users::followPlaylist(const std::string& playlist_id, const int is_public) const
{
	std::string url = "https://api.spotify.com/v1/playlists/" + playlist_id + "/followers";

	Net net(url, "PUT");
	this->spotify->addDefaultHeaders(net);

	if (is_public != -1) {
		net.addBody("{ \"public\": " + std::string(is_public ? "true" : "false") + " }");
	}

	RetErrorBool;
}

/////////////////////////////////
// Unfollows a specific playlist.
//
// Parameters:
// - playlist_id: The ID of the playlist to unfollow.
//
// Returns:
// - true if the operation is successful; false otherwise.
bool Spotify::_Users::unfollowPlaylist(const std::string& playlist_id) const
{
	std::string url = "https://api.spotify.com/v1/playlists/" + playlist_id + "/followers";

	Net net(url, "PUT");
	this->spotify->addDefaultHeaders(net);

	RetErrorBool;
}

/////////////////////////////////
// Retrieves the artists followed by the current user.
//
// Parameters:
// - after: Pagination parameter to specify the last artist ID received, allowing retrieval of the next set.
// - limit: Number of items to retrieve (maximum Spotify allows is 50).
// - offset: The index of the first item to return.
//
// Returns:
// - A JSON string containing details of the followed artists.
std::string Spotify::_Users::getFollowedArtists(const std::string& after, const int limit, const int offset) const
{
	std::string url = "https://api.spotify.com/v1/me/following?type=artist&limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset) + (after.empty() ? "" : "&after=" + after);

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Follows specific Spotify accounts (artists or users).
//
// Parameters:
// - ids: A list of Spotify IDs of the accounts to follow.
// - type: Specifies if the accounts to follow are "artist" or "user".
//
// Returns:
// - true if the operation is successful; false otherwise.
bool Spotify::_Users::followAccount(const std::vector<std::string>& ids, const ACCOUNT_TYPE type) const
{
	std::string url = "https://api.spotify.com/v1/me/following?type=" + std::string(type == ACCOUNT_TYPE::Artist ? "artist" : "user");

	Net net(url, "PUT");
	this->spotify->addDefaultHeaders(net);

	std::string body = "{ \"ids\": [";
	for (std::string id : ids) {
		body += id + ",";
	}
	body = body.substr(0, body.length() - 1);
	body += "] }";

	net.addBody(body);

	RetErrorBool;
}

/////////////////////////////////
// Unfollows specific Spotify accounts (artists or users).
//
// Parameters:
// - ids: A list of Spotify IDs of the accounts to unfollow.
// - type: Specifies if the accounts to unfollow are "artist" or "user".
//
// Returns:
// - true if the operation is successful; false otherwise.
bool Spotify::_Users::unfollowAccount(const std::vector<std::string>& ids, const ACCOUNT_TYPE type) const
{
	std::string url = "https://api.spotify.com/v1/me/following?type=" + std::string(type == ACCOUNT_TYPE::Artist ? "artist" : "user");

	Net net(url, "DELETE");
	this->spotify->addDefaultHeaders(net);

	std::string body = "{ \"ids\": [";
	for (std::string id : ids) {
		body += id + ",";
	}
	body = body.substr(0, body.length() - 1);
	body += "] }";

	net.addBody(body);

	RetErrorBool;
}

/////////////////////////////////
// Checks if the current user follows specified accounts (artists or users).
//
// Parameters:
// - ids: A list of Spotify IDs to check if the user follows.
// - type: Specifies if the accounts are "artist" or "user".
//
// Returns:
// - A vector of booleans, each indicating whether the user follows the corresponding account in 'ids'.
std::vector<bool> Spotify::_Users::checkUserFollowsAccount(const std::vector<std::string>& ids, const ACCOUNT_TYPE type) const
{
	std::string url = "https://api.spotify.com/v1/me/following/contains?type=" + std::string(type == ACCOUNT_TYPE::Artist ? "artist" : "user") + "&ids=";
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
// Checks if the current user follows a specified playlist.
//
// Parameters:
// - playlist_id: The ID of the playlist to check.
//
// Returns:
// - true if the user follows the playlist; false otherwise.
bool Spotify::_Users::checkUserFollowsPlaylist(const std::string& playlist_id) const
{
	std::string url = "https://api.spotify.com/v1/playlists/" + playlist_id + "/followers/contains";

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return false;
	}

	try {
		json j = json::parse(response);
		return j.at(0);
	}
	catch (...) {
		RetError;
		return false;
	}
}
