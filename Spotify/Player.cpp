#include "Spotify.h"
#include "StrUtil.h"
#include <assert.h>


/////////////////////////////////
std::string Spotify::_Player::getPlaybackState(const std::string& market, const std::string& additional_types) const {
	std::string url = "https://api.spotify.com/v1/me/player";

	if (!market.empty())
		url += "?market=" + market;
	if (!additional_types.empty())
		url += std::string(market.empty() ? "?" : "&") + "additional_types=" + additional_types;

	Net net(url, "PUT");
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
bool Spotify::_Player::transferPlayback(const std::string& device_id, bool play) const {
	std::string url = "https://api.spotify.com/v1/me/player";

	Net net(url, "PUT");
	this->spotify->addTokenHeaderOnly(net);

	// Body
	std::string body = "{ \"device_ids\": [\"" + device_id + "\"], \"play\": " + (play ? "true" : "false") + " }";
	net.addBody(body);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return false;
	}

	return true;
}

/////////////////////////////////
std::string Spotify::_Player::getAvailableDevices() const {
	std::string url = "https://api.spotify.com/v1/me/player/devices";

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
std::string Spotify::_Player::getCurrentlyPlaying(const std::string& market, const std::string& additional_types) const {
	std::string url = "https://api.spotify.com/v1/me/player/currently-playing";

	if (!market.empty())
		url += "?market=" + market;
	if (!additional_types.empty())
		url += std::string(market.empty() ? "?" : "&") + "additional_types=" + additional_types;

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
bool Spotify::_Player::resume(const std::string& device_id) const {
	std::string url = "https://api.spotify.com/v1/me/player/play" + (device_id.empty() ? "" : "?device_id=" + device_id);

	Net net(url, "PUT");
	this->spotify->addTokenHeaderOnly(net);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return false;
	}

	return true;
}

/////////////////////////////////
bool Spotify::_Player::pause(const std::string& device_id) const {
	std::string url = "https://api.spotify.com/v1/me/player/pause" + (device_id.empty() ? "" : "?device_id=" + device_id);

	Net net(url, "PUT");
	this->spotify->addTokenHeaderOnly(net);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return false;
	}

	return true;
}

/////////////////////////////////
bool Spotify::_Player::playTracks(const std::vector<std::string>& ids, const int start_position_ms, const int offset, const std::string& device_id) const {
	std::string url = "https://api.spotify.com/v1/me/player/play" + (device_id.empty() ? "" : "?device_id=" + device_id);

	Net net(url, "PUT");
	this->spotify->addDefaultHeaders(net);

	// Body
	std::string body = "{\"uris\": [\"";
	for (std::string id : ids) {
		body += "spotify:track:" + id + "\",\"";
	}
	body = body.substr(0, body.length() - 2);
	body += "], ";

	if (start_position_ms)
		body += "\"position_ms\": " + std::to_string(start_position_ms);
	if (offset)
		body += ", \"offset\":{\"position\":" + std::to_string(offset) + "}";

	body += "}";

	net.addBody(body);

	std::string response;
	long res = net.send(response);
	if (!WEB_SUCCESS(res)) {
		RetError;
		return false;
	}

	return true;
}