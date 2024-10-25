#pragma once

#define WEB_SUCCESS(res) (res >= 200 && res <= 299)

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


#define RetError if (this->spotify->error_tracking) \
	this->spotify->last_message = "ResponseCode: " + std::to_string(res) + "\nReponse: " + response; \
    if (res == 401) \
        this->spotify->should_renew = true