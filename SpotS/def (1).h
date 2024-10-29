#pragma once


enum AUTH_TYPE {
	AUTH_TYPE_OAUTH = 0,
	AUTH_TYPE_REFRESHTOKEN = 1
};

namespace Scopes {
	constexpr const char* ugc_image_upload = "ugc-image-upload";
	constexpr const char* user_read_playback_state = "user-read-playback-state";
	constexpr const char* user_modify_playback_state = "user-modify-playback-state";
	constexpr const char* user_read_currently_playing = "user-read-currently-playing";
	constexpr const char* app_remote_control = "app-remote-control";
	constexpr const char* streaming = "streaming";
	constexpr const char* playlist_read_private = "playlist-read-private";
	constexpr const char* playlist_read_collaborative = "playlist-read-collaborative";
	constexpr const char* playlist_modify_private = "playlist-modify-private";
	constexpr const char* playlist_modify_public = "playlist-modify-public";
	constexpr const char* user_follow_modify = "user-follow-modify";
	constexpr const char* user_follow_read = "user-follow-read";
	constexpr const char* user_read_playback_position = "user-read-playback-position";
	constexpr const char* user_top_read = "user-top-read";
	constexpr const char* user_read_recently_played = "user-read-recently-played";
	constexpr const char* user_library_modify = "user-library-modify";
	constexpr const char* user_library_read = "user-library-read";
	constexpr const char* user_read_email = "user-read-email";
	constexpr const char* user_read_private = "user-read-private";
	constexpr const char* user_soa_link = "user-soa-link";
	constexpr const char* user_soa_unlink = "user-soa-unlink";
	constexpr const char* soa_manage_entitlements = "soa-manage-entitlements";
	constexpr const char* soa_manage_partner = "soa-manage-partner";
	constexpr const char* soa_create_partner = "soa-create-partner";
};

/*
struct Artist {
	std::string id;
	std::string name;
	std::string type;
	std::string uri;
};

struct Image {
	std::string url;
	int width = 0;
	int height = 0;
};

struct Album {
	std::string album_type;
	int total_tracks = 0;
	std::vector<std::string> available_markets;
	std::string id;
	std::string name;
	std::string type;
	std::string uri;
	std::string release_data;
	std::vector<Artist> artists;
	std::vector<Image> images;
};

struct Track {
	Album album{};
	std::vector<Artist> artists;
	std::string id;
	std::string name;
	std::string type;
	std::string uri;
	int duration_ms = 0;
	bool is_playable = false;
	bool is_local = false;
};
*/