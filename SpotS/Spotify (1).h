#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <Net/Net.h>
#include "def.h"
#define MARKET "ES"

using json = nlohmann::json;


class Spotify {
public:
	Spotify(std::string client_id, std::string client_secret) : client_id(client_id), client_secret(client_secret) {};
	~Spotify() {};

#pragma region auth
	std::string getAuthUrl(const std::string& redirectUri, const std::vector<const char*>& scopes) const;

	bool login(const std::string& token, AUTH_TYPE authType, const std::string& redirectUri = "");
#pragma endregion

#pragma region child classes
	class _Albums {
	private:
		Spotify* spotify;
	public:
		_Albums(Spotify* spotify) : spotify(spotify) {}
		_Albums() = delete;
	public:
		json getAlbum(const std::string& id, const std::string& market = MARKET) const;
		json getAlbums(const std::vector<std::string>& ids, const std::string& market = MARKET) const;
		json getAlbumTracks(const std::string& id, const int limit = 10, const int offset = 0, const std::string& market = MARKET) const;
		json getUserSavedAlbums(const int limit = 10, const int offset = 0, const std::string& market = MARKET) const;
		bool saveAlbums(const std::vector<std::string>& ids) const;
		bool unsaveAlbums(const std::vector<std::string>& ids) const;
		std::vector<bool> getAlbumsIsSaved(const std::vector<std::string>& ids) const;
	};

	class _Artists {
	private:
		Spotify* spotify;
	public:
		_Artists(Spotify* spotify) : spotify(spotify) {}
	public:
		json getArtist(std::string id) const;
		json getArtists(const std::vector<std::string>& ids) const;
		json getArtistAlbums(const std::string& id, const int limit = 10, const int offset = 0, const std::string& market = MARKET) const;
		json getArtistTopTracks(const std::string& id, const std::string& market = MARKET) const;
		json getArtistRelatedArtists(const std::string& id) const;
	};

	class _Episodes {
	private:
		Spotify* spotify;
	public:
		_Episodes(Spotify* spotify) : spotify(spotify) {}
	public:

	};

	class _Player {
	private:
		Spotify* spotify;
	public:
		_Player(Spotify* spotify) : spotify(spotify) {}
	public:

	};

	class _Playlists {
	private:
		Spotify* spotify;
	public:
		_Playlists(Spotify* spotify) : spotify(spotify) {}
	public:

	};

	class _Search {
	private:
		Spotify* spotify;
	public:
		_Search(Spotify* spotify) : spotify(spotify) {}
	public:

	};

	class _Tracks {
	private:
		Spotify* spotify;
	public:
		_Tracks(Spotify* spotify) : spotify(spotify) {}
	public:

	};

	class _Users {
	private:
		Spotify* spotify;
	public:
		_Users(Spotify* spotify) : spotify(spotify) {}
	public:

	};
#pragma endregion

	// Access Child classes
	_Albums Albums() { return _Albums(this); }
	_Artists Artists() { return _Artists(this); }
	_Episodes Episodes() { return _Episodes(this); }
	_Player Player() { return _Player(this); }
	_Playlists Playlists() { return _Playlists(this); }
	_Search Search() { return _Search(this); }
	_Tracks Tracks() { return _Tracks(this); }
	_Users Users() { return _Users(this); }

	// Net functions
	void addDefaultHeaders(Net& net) const;

public:
	std::string access_token, refresh_token;

private:
	std::string client_id, client_secret;
};