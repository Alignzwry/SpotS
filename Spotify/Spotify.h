#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <Net/Net.h>
#include "def.h"


class Spotify {
public:
	Spotify(std::string client_id, std::string client_secret) : client_id(client_id), client_secret(client_secret) {};
	~Spotify() {};

#pragma region auth
	// Constructs the authorization URL for the Spotify API login page with required scopes.
	std::string getAuthUrl(const std::string& redirectUri, const std::vector<const char*>& scopes) const;

	// Authenticates with Spotify using either an authorization code.
	bool auth(const std::string& authCode, const std::string& redirectUri);

	// Get the access token using your refresh token.
	bool refresh();

	// Sets the refresh token.
	void setRefreshToken(const std::string& refreshToken);

	// A flag indicating if the access token should be renewed.
	bool should_renew = false;
#pragma endregion

#pragma region errors
	// Enables error tracking and logging.
	void enableErrors() { this->error_tracking = true; }

	// Disables error tracking and logging.
	void disableErrors() { this->error_tracking = false; }

private:
	bool error_tracking = true;  // Indicates if error tracking is enabled.
	std::string last_message;    // Stores the last error message encountered.

public:
	// Retrieves the last error message encountered.
	// Returns:
	// - A string containing the last error message.
	std::string getLastError() const { return this->last_message; }
#pragma endregion

#pragma region child classes
	class _Albums {
	private:
		Spotify* spotify;
	public:
		explicit _Albums(Spotify* spotify) : spotify(spotify) {}
		std::string getAlbum(const std::string& id, const std::string& market = "") const;
		std::string getAlbums(const std::vector<std::string>& ids, const std::string& market = "") const;
		std::string getAlbumTracks(const std::string& id, const int limit = 10, const int offset = 0, const std::string& market = "") const;
		std::string getUserSavedAlbums(const int limit = 10, const int offset = 0, const std::string& market = "") const;
		bool saveAlbums(const std::vector<std::string>& ids) const;
		bool unsaveAlbums(const std::vector<std::string>& ids) const;
		std::vector<bool> getAlbumsIsSaved(const std::vector<std::string>& ids) const;
	};

	class _Artists {
	private:
		Spotify* spotify;
	public:
		explicit _Artists(Spotify* spotify) : spotify(spotify) {}
		std::string getArtist(const std::string& id) const;
		std::string getArtists(const std::vector<std::string>& ids) const;
		std::string getArtistAlbums(const std::string& id, const int limit = 10, const int offset = 0, const std::string& market = "") const;
		std::string getArtistTopTracks(const std::string& id, const std::string& market = "") const;
		std::string getArtistRelatedArtists(const std::string& id) const;
	};

	class _Episodes {
	private:
		Spotify* spotify; // Reference to the parent Spotify object.
	public:
		explicit _Episodes(Spotify* spotify) : spotify(spotify) {}
		std::string getEpisode(const std::string& id, const std::string& market = "") const;
		std::string getEpisodes(const std::vector<std::string>& ids, const std::string& market = "") const;
		std::string getSavedEpisodes(const int limit = 10, const int offset = 0, const std::string& market = "") const;
		bool saveEpisodes(const std::vector<std::string>& ids) const;
		bool unsaveEpisodes(const std::vector<std::string>& ids) const;
		std::vector<bool> getEpisodesIsSaved(const std::vector<std::string>& ids) const;
	};

	class _Player {
	private:
		Spotify* spotify; // Reference to the parent Spotify object.
	public:
		explicit _Player(Spotify* spotify) : spotify(spotify) {}
		std::string getPlaybackState(const std::string& market = "", const std::string& additional_types = "") const;
		bool transferPlayback(const std::string& device_id, bool play = true) const;
		std::string getAvailableDevices() const;
		std::string getCurrentlyPlaying(const std::string& market = "", const std::string& additional_types = "") const;
		bool resume(const std::string& device_id = "") const;
		bool pause(const std::string& device_id = "") const;
		bool playTracks(const std::vector<std::string>& ids, const int start_position_ms = 0, const int offset = 0, const std::string& device_id = "") const;
	};

	class _Playlists {
	private:
		Spotify* spotify; // Reference to the parent Spotify object.
	public:
		explicit _Playlists(Spotify* spotify) : spotify(spotify) {}
		std::string getPlaylist(const std::string& id, const std::string& market = "", const std::string& fields = "", const std::string& additional_types = "") const;
		std::string updateDetails(const std::string& id, const std::string& name = "", const std::string& description = "", const int is_public = -1) const;
		std::string getItems(const std::string& id, const int limit = 10, const int offset = 0, const std::string& market = "", const std::string& fields = "", const std::string& additional_types = "") const;
		bool updateItems(const std::string& id, const std::vector<std::string>& ids, const int range_start, const int insert_before, const int range_length, const std::string& snapshot_id = "") const;
		bool addItems(const std::string& id, const std::vector<std::string>& ids, const int position = -1) const;
		bool removeItems(const std::string& id, const std::vector<std::string>& ids, const std::string& snapshot_id = "") const;
		std::string getCurrentUserPlaylists(const int limit = 10, const int offset = 0) const;
		std::string getUsersPlaylist(const std::string& user_id, const int limit = 10, const int offset = 0) const;
		bool createPlaylist(const std::string& user_id, const std::string& name, const std::string& description = "", const int is_public = -1) const;
		std::string getFeaturesPlaylists(const std::string& locale = "", const int limit = 10, const int offset = 0) const;
		std::string getCategoriesPlaylists(const std::string& category, const int limit = 10, const int offset = 0) const;
		std::string getPlaylistCover(const std::string& id) const;
		bool setPlaylistCover(const std::string& id, const std::string& base64) const;
	};

	class _Search {
	private:
		Spotify* spotify; // Reference to the parent Spotify object.
	public:
		explicit _Search(Spotify* spotify) : spotify(spotify) {}
		std::string search(const std::string& q, const std::vector<const char*>& types, const int limit = 10, const int offset = 0, const std::string& market = "") const;
	};

	class _Tracks {
	private:
		Spotify* spotify; // Reference to the parent Spotify object.
	public:
		explicit _Tracks(Spotify* spotify) : spotify(spotify) {}
		struct AudioFeaturesFilter {
			const int min_acousticness = -1, max_acousticness = -1, target_acousticness = -1,
				min_danceability = -1, max_danceability = -1, target_danceability = -1,
				min_duration_ms = -1, max_duration_ms = -1, target_duration_ms = -1,
				min_energy = -1, max_energy = -1, target_energy = -1,
				min_instrumentalness = -1, max_instrumentalness = -1, target_instrumentalness = -1,
				min_key = -1, max_key = -1, target_key = -1,
				min_liveness = -1, max_liveness = -1, target_liveness = -1,
				min_loudness = -1, max_loudness = -1, target_loudness = -1,
				min_mode = -1, max_mode = -1, target_mode = -1,
				min_popularity = -1, max_popularity = -1, target_popularity = -1,
				min_speechiness = -1, max_speechiness = -1, target_speechiness = -1,
				min_tempo = -1, max_tempo = -1, target_tempo = -1,
				min_time_signature = -1, max_time_signature = -1, target_time_signature = -1,
				min_valence = -1, max_valence = -1, target_valence = -1;
		};

		std::string getTrack(const std::string& id, const std::string& market = "") const;
		std::string getTracks(const std::vector<std::string>& ids, const std::string& market = "") const;
		std::string getSavedTracks(const int limit = 10, const int offset = 0, const std::string& market = "") const;
		bool saveTracks(const std::vector<std::string>& ids) const;
		bool unsaveTracks(const std::vector<std::string>& ids) const;
		std::vector<bool> getTracksIsSaved(const std::vector<std::string>& ids) const;
		std::string getMultipleTracksAudioFeatures(const std::vector<std::string>& ids) const;
		std::string getAudioAnalysis(const std::string& id) const;
		std::string getAudioFeatures(const std::string& id) const;
		std::string getRecommendations(const std::vector<std::string>& seed_tracks = {}, const std::vector<std::string>& seed_artists = {}, const std::vector<std::string>& seed_genres = {}, const int limit = 10, const std::string& market = "", AudioFeaturesFilter = {}) const;
	};

	class _Users {
	private:
		Spotify* spotify; // Reference to the parent Spotify object.
	public:
		explicit _Users(Spotify* spotify) : spotify(spotify) {}
		enum class TIME_RANGE : std::uint8_t {
			Short_term = 0,
			Medium_term = 1,
			Long_term = 2
		};
		enum class ITEM_TYPE : std::uint8_t {
			Track = 0,
			Artist = 1
		};
		enum class ACCOUNT_TYPE : std::uint8_t {
			Artist = 0,
			User = 1
		};
		std::string getCurrentUser() const;
		std::string getTopItem(const ITEM_TYPE type, const TIME_RANGE time_range = TIME_RANGE::Medium_term, const int limit = 10, const int offset = 0) const;
		std::string getUsersProfile(const std::string& user_id) const;
		bool followPlaylist(const std::string& playlist_id, const int is_public = -1) const;
		bool unfollowPlaylist(const std::string& playlist_id) const;
		std::string getFollowedArtists(const std::string& after = "", const int limit = 10, const int offset = 0) const;
		bool followAccount(const std::vector<std::string>& ids, const ACCOUNT_TYPE type) const;
		bool unfollowAccount(const std::vector<std::string>& ids, const ACCOUNT_TYPE type) const;
		std::vector<bool> checkUserFollowsAccount(const std::vector<std::string>& ids, const ACCOUNT_TYPE type) const;
		bool checkUserFollowsPlaylist(const std::string& playlist_id) const;
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