#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <Net/Net.h>
#include "def.h"
#define MARKET "ES"


class Spotify {
public:
	Spotify(std::string client_id, std::string client_secret) : client_id(client_id), client_secret(client_secret) {};
	~Spotify() {};

#pragma region auth
	std::string getAuthUrl(const std::string& redirectUri, const std::vector<const char*>& scopes) const;

	bool login(const std::string& token, AUTH_TYPE authType, const std::string& redirectUri = "");

	bool should_renew = false;
#pragma endregion

#pragma region errors
	void enableErrors() { this->error_tracking = true; }
	void disableErrors() { this->error_tracking = false; }
private:
	bool error_tracking = true;
	std::string last_message;
public:
	std::string getLastError() const { return this->last_message; }
#pragma endregion

#pragma region child classes
	class _Albums {
	private:
		Spotify* spotify;
	public:
		_Albums(Spotify* spotify) : spotify(spotify) {}
		_Albums() = delete;
	public:
		std::string getAlbum(const std::string& id, const std::string& market = MARKET) const;
		std::string getAlbums(const std::vector<std::string>& ids, const std::string& market = MARKET) const;
		std::string getAlbumTracks(const std::string& id, const int limit = 10, const int offset = 0, const std::string& market = MARKET) const;
		std::string getUserSavedAlbums(const int limit = 10, const int offset = 0, const std::string& market = MARKET) const;
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
		std::string getArtist(const std::string& id) const;
		std::string getArtists(const std::vector<std::string>& ids) const;
		std::string getArtistAlbums(const std::string& id, const int limit = 10, const int offset = 0, const std::string& market = MARKET) const;
		std::string getArtistTopTracks(const std::string& id, const std::string& market = MARKET) const;
		std::string getArtistRelatedArtists(const std::string& id) const;
	};

	class _Episodes {
	private:
		Spotify* spotify;
	public:
		_Episodes(Spotify* spotify) : spotify(spotify) {}
	public:
		std::string getEpisode(const std::string& id, const std::string& market = MARKET) const;
		std::string getEpisodes(const std::vector<std::string>& ids, const std::string& market = MARKET) const;
		std::string getSavedEpisodes(const int limit = 10, const int offset = 0, const std::string& market = MARKET) const;
		bool saveEpisodes(const std::vector<std::string>& ids) const;
		bool unsaveEpisodes(const std::vector<std::string>& ids) const;
		std::vector<bool> getEpisodesIsSaved(const std::vector<std::string>& ids) const;
	};

	class _Player {
	private:
		Spotify* spotify;
	public:
		_Player(Spotify* spotify) : spotify(spotify) {}
	public:
		std::string getPlaybackState(const std::string& market = MARKET, const std::string& additional_types = "") const;
		bool transferPlayback(const std::string& device_id, bool play = true) const;
		std::string getAvailableDevices() const;
		std::string getCurrentlyPlaying(const std::string& market = MARKET, const std::string& additional_types = "") const;
		bool resume(const std::string& device_id = "") const;
		bool pause(const std::string& device_id = "") const;
		bool playTracks(const std::vector<std::string>& ids, const int start_position_ms = 0, const int offset = 0, const std::string& device_id = "") const;
	};

	class _Playlists {
	private:
		Spotify* spotify;
	public:
		_Playlists(Spotify* spotify) : spotify(spotify) {}
	public:
		std::string getPlaylist(const std::string& id, const std::string& market = MARKET, const std::string& fields = "", const std::string& additional_types = "") const;
		std::string updateDetails(const std::string& id, const std::string& name = "", const std::string& description = "", const int is_public = -1) const;
		std::string getItems(const std::string& id, const int limit = 10, const int offset = 0, const std::string& market = MARKET, const std::string& fields = "", const std::string& additional_types = "") const;
		bool updateItems(const std::string& id, const std::vector<std::string>& ids, const int range_start, const int insert_before, const int range_length, const std::string& snapshot_id = "") const;
		bool addItems(const std::string& id, const std::vector<std::string>& ids, const int position = -1) const;
		bool removeItems(const std::string& id, const std::vector<std::string>& ids, const std::string& snapshot_id = "") const;
		std::string getCurrentUserPlaylists(const int limit = 10, const int offset = 0);
		std::string getUsersPlaylist(const std::string& user_id, const int limit = 10, const int offset = 0);
		bool createPlaylist(const std::string& user_id, const std::string& name, const std::string& description = "", const bool is_public = false) const;
		std::string getFeaturesPlaylists(const std::string& locale = "", const int limit = 10, const int offset = 0) const;
		std::string getCategoriesPlaylists(const std::string& category, const int limit = 10, const int offset = 0) const;
		std::string getPlaylistCover(const std::string& id) const;
		bool setPlaylistCover(const std::string& id, const std::string& base64) const;
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
	void addTokenHeaderOnly(Net& net) const;

public:
	std::string access_token, refresh_token;

private:
	std::string client_id, client_secret;
};