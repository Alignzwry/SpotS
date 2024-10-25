#include "Spotify.h"
#include "StrUtil.h"
#include <assert.h>

/////////////////////////////////
// Constructs the authorization URL for the Spotify API login page with required scopes.
//
// Parameters:
// - redirectUri: The URI to which Spotify will redirect after authentication.
// - scopes: A list of permission scopes required for the Spotify application.
//
// Returns:
// - A URL string that the user can visit to authenticate the app with Spotify and grant permissions.
std::string Spotify::getAuthUrl(const std::string& redirectUri, const std::vector<const char*>& scopes) const {

	std::string url = "https://accounts.spotify.com/authorize?response_type=code&client_id=" + 
		this->client_id + 
		"&redirect_uri=" + 
		Net::url_encode(redirectUri) +
		"&scope=";

	for (int i = 0; i < scopes.size(); i++) {
		url += scopes[i];
		if (i < scopes.size() - 1)
			url += "%20";
	}

	url += "&show_dialog=false";

	return url;
}

/////////////////////////////////
// Authenticates with Spotify using either an authorization code or a refresh token.
//
// Parameters:
// - token: The authorization code or refresh token depending on the auth type.
// - authType: Specifies whether to use OAuth authorization (AUTH_TYPE_OAUTH) 
//             or a refresh token (AUTH_TYPE_REFRESHTOKEN) for login.
// - redirectUri: The URI to which Spotify will redirect after authentication. Only required for OAuth login.
//
// Returns:
// - true if authentication is successful and access tokens are retrieved, otherwise false.
bool Spotify::login(const std::string& token, AUTH_TYPE authType, const std::string& redirectUri) {
	switch (authType) {
	case AUTH_TYPE::AUTH_TYPE_OAUTH:
	{
		assert(!redirectUri.empty());

		// Request tokens from Spotify
		Net net("https://accounts.spotify.com/api/token", "POST");
		net.addBody(
			"grant_type=authorization_code&code=" +
			token +
			"&redirect_uri=" + 
			Net::url_encode(redirectUri) +
			"&client_id=" +
			this->client_id +
			"&client_secret=" +
			this->client_secret);

		std::string token;
		long res = net.send(token);
		if (!WEB_SUCCESS(res))
			return false;

		// Extract the access and refresh tokens from the response
		this->refresh_token = util::substrBetween(token, "\"refresh_token\":\"", "\",\"");
		this->access_token = util::substrBetween(token, "\"access_token\":\"", "\",\"");

		return !this->refresh_token.empty() && !this->access_token.empty();
	}
	case AUTH_TYPE::AUTH_TYPE_REFRESHTOKEN:
	{
		this->refresh_token = token;
		Net net("https://accounts.spotify.com/api/token", "POST");
		net.addBody(
			"grant_type=refresh_token&refresh_token=" + 
			this->refresh_token + 
			"&client_id=" +
			this->client_id + 
			"&client_secret=" + 
			this->client_secret);

		std::string token;
		long res = net.send(token);
		if (!WEB_SUCCESS(res)) {
			if (this->error_tracking)
				this->last_message = "ResponseCode = " + std::to_string(res) + "\n" + token;
			return false;
		}

		this->access_token = util::substrBetween(token, "\"access_token\":\"", "\"");

		return !this->access_token.empty();
	}
	}
}

/////////////////////////////////
// Adds standard authorization headers to an HTTP request for Spotify API calls.
//
// Parameters:
// - net: The Net object representing the HTTP request. Headers are added to this object.
void Spotify::addDefaultHeaders(Net& net) const {
	net.addHeader("Authorization: Bearer " + this->access_token);
	net.addHeader("Content-Type: application/json");
	net.addHeader("Accept: application/json");
}

/////////////////////////////////
// Adds only the authorization header to an HTTP request, often for basic authenticated calls.
//
// Parameters:
// - net: The Net object representing the HTTP request. Only the authorization header is added.
void Spotify::addTokenHeaderOnly(Net& net) const {
	net.addHeader("Authorization: Bearer " + this->access_token);
}