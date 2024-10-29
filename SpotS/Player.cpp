#include "Spotify.h"

/////////////////////////////////
// Retrieves the current playback state for the user.
//
// Parameters:
// - market: The Spotify market (country) to be used for content restrictions. Optional.
// - additional_types: Additional media types to include in the response, such as "episode". Optional.
//
// Returns:
// - A JSON-formatted string containing playback state details if successful, otherwise an empty string.
std::string Spotify::_Player::getPlaybackState(const std::string& market, const std::string& additional_types) const
{
	std::string url = "https://api.spotify.com/v1/me/player";

	if (!market.empty())
		url += "?market=" + market;
	if (!additional_types.empty())
		url += std::string(market.empty() ? "?" : "&") + "additional_types=" + additional_types;

	Net net(url, "PUT");
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Transfers playback to a new device and optionally starts playback on the device.
//
// Parameters:
// - device_id: The ID of the device to transfer playback to.
// - play: If true, playback will start immediately on the new device. If false, playback remains paused.
//
// Returns:
// - true if the transfer is successful, otherwise false.
bool Spotify::_Player::transferPlayback(const std::string& device_id, bool play) const
{
	std::string url = "https://api.spotify.com/v1/me/player";

	Net net(url, "PUT");
	this->spotify->addTokenHeaderOnly(net);

	// Body
	std::string body = "{ \"device_ids\": [\"" + device_id + "\"], \"play\": " + (play ? "true" : "false") + " }";
	net.addBody(body);

	RetErrorBool;
}

/////////////////////////////////
// Retrieves a list of devices currently available for playback.
//
// Returns:
// - A JSON-formatted string with details of available devices if successful, otherwise an empty string.
std::string Spotify::_Player::getAvailableDevices() const
{
	std::string url = "https://api.spotify.com/v1/me/player/devices";

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Retrieves details of the currently playing track or episode for the user.
//
// Parameters:
// - market: The Spotify market (country) to be used for content restrictions. Optional.
// - additional_types: Additional media types to include in the response, such as "episode". Optional.
//
// Returns:
// - A JSON-formatted string containing currently playing media details if successful, otherwise an empty string.
std::string Spotify::_Player::getCurrentlyPlaying(const std::string& market, const std::string& additional_types) const
{
	std::string url = "https://api.spotify.com/v1/me/player/currently-playing";

	if (!market.empty())
		url += "?market=" + market;
	if (!additional_types.empty())
		url += std::string(market.empty() ? "?" : "&") + "additional_types=" + additional_types;

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}

/////////////////////////////////
// Resumes playback on the specified device or on the current device if none is specified.
//
// Parameters:
// - device_id: The ID of the device to resume playback on. Optional.
//
// Returns:
// - true if playback resumes successfully, otherwise false.
bool Spotify::_Player::resume(const std::string& device_id) const
{
	std::string url = "https://api.spotify.com/v1/me/player/play" + (device_id.empty() ? "" : "?device_id=" + device_id);

	Net net(url, "PUT");
	this->spotify->addTokenHeaderOnly(net);

	RetErrorBool;
}

/////////////////////////////////
// Pauses playback on the specified device or on the current device if none is specified.
//
// Parameters:
// - device_id: The ID of the device to pause playback on. Optional.
//
// Returns:
// - true if playback pauses successfully, otherwise false.
bool Spotify::_Player::pause(const std::string& device_id) const
{
	std::string url = "https://api.spotify.com/v1/me/player/pause" + (device_id.empty() ? "" : "?device_id=" + device_id);

	Net net(url, "PUT");
	this->spotify->addTokenHeaderOnly(net);

	RetErrorBool;
}

/////////////////////////////////
// Starts playback of specific tracks on a specified device, with options for position and offset.
//
// Parameters:
// - ids: A list of track IDs to play.
// - start_position_ms: The playback start position in milliseconds. Optional.
// - offset: Track index offset within the provided track list. Optional.
// - device_id: The ID of the device to play the tracks on. Optional.
//
// Returns:
// - true if playback starts successfully, otherwise false.
bool Spotify::_Player::playTracks(const std::vector<std::string>& ids, const int start_position_ms, const int offset, const std::string& device_id) const
{
	std::string url = "https://api.spotify.com/v1/me/player/play" + (device_id.empty() ? "" : "?device_id=" + device_id);

	Net net(url, "PUT");
	this->spotify->addDefaultHeaders(net);

	// Body
	std::string body = "{\"uris\": [\"";
	for (std::string id : ids) {
		body += "spotify:track:" + id + "\",\"";
	}
	body = body.substr(0, body.length() - 2);  // Remove the trailing comma and quotation mark
	body += "], ";

	if (start_position_ms)
		body += "\"position_ms\": " + std::to_string(start_position_ms);
	if (offset)
		body += ", \"offset\":{\"position\":" + std::to_string(offset) + "}";

	body += "}";

	net.addBody(body);

	RetErrorBool;
}

/////////////////////////////////
// Retrieves the user's recently played tracks.
//
// Parameters:
// - limit: Number of tracks to retrieve (default is 10).
// - after: Unix timestamp in milliseconds. Only retrieve tracks played after this time. Optional.
// - before: Unix timestamp in milliseconds. Only retrieve tracks played before this time. Optional.
//
// Returns:
// - A JSON string containing recently played tracks or an error message.
std::string Spotify::_Player::getRecentlyPlayed(const int limit, const int after, const int before) const {
	std::string url = "https://api.spotify.com/v1/me/player/recently-played?limit=" + std::to_string(limit) + (after != unused_arg_int ? "&after=" + std::to_string(after) : "") + (before != unused_arg_int ? "&before=" + std::to_string(before) : "");

	Net net(url);
	this->spotify->addDefaultHeaders(net);

	RetErrorString;
}