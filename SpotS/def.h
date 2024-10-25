#pragma once

// Macro to determine if a web response code indicates success (i.e., within the 200–299 range).
#define WEB_SUCCESS(res) (res >= 200 && res <= 299)

// Namespace containing various Spotify API permission scopes.
// Each scope represents a specific permission the application can request access to.
namespace Scopes {
	constexpr const char* ugc_image_upload = "ugc-image-upload";                  // Permission to upload user-generated content images.
	constexpr const char* user_read_playback_state = "user-read-playback-state";  // Permission to read the user's playback state.
	constexpr const char* user_modify_playback_state = "user-modify-playback-state"; // Permission to control playback.
	constexpr const char* user_read_currently_playing = "user-read-currently-playing"; // Permission to read the user's currently playing track.
	constexpr const char* app_remote_control = "app-remote-control";              // Permission for remote control of the app.
	constexpr const char* streaming = "streaming";                                // Permission to play music and control playback.
	constexpr const char* playlist_read_private = "playlist-read-private";        // Permission to view private playlists.
	constexpr const char* playlist_read_collaborative = "playlist-read-collaborative"; // Permission to view collaborative playlists.
	constexpr const char* playlist_modify_private = "playlist-modify-private";    // Permission to modify private playlists.
	constexpr const char* playlist_modify_public = "playlist-modify-public";      // Permission to modify public playlists.
	constexpr const char* user_follow_modify = "user-follow-modify";              // Permission to modify who the user follows.
	constexpr const char* user_follow_read = "user-follow-read";                  // Permission to read who the user follows.
	constexpr const char* user_read_playback_position = "user-read-playback-position"; // Permission to read the user's playback position.
	constexpr const char* user_top_read = "user-top-read";                        // Permission to view the user's top tracks and artists.
	constexpr const char* user_read_recently_played = "user-read-recently-played"; // Permission to view the user's recently played items.
	constexpr const char* user_library_modify = "user-library-modify";            // Permission to modify items in the user's library.
	constexpr const char* user_library_read = "user-library-read";                // Permission to read the user's library.
	constexpr const char* user_read_email = "user-read-email";                    // Permission to read the user's email.
	constexpr const char* user_read_private = "user-read-private";                // Permission to read the user's private account details.
	constexpr const char* user_soa_link = "user-soa-link";                        // Special permission for linking user SOA (Spotify One Account).
	constexpr const char* user_soa_unlink = "user-soa-unlink";                    // Special permission for unlinking user SOA.
	constexpr const char* soa_manage_entitlements = "soa-manage-entitlements";    // Permission for managing user entitlements.
	constexpr const char* soa_manage_partner = "soa-manage-partner";              // Permission for managing partner SOA.
	constexpr const char* soa_create_partner = "soa-create-partner";              // Permission for creating partner SOA.
};

// Macro for handling HTTP response errors and setting error tracking if enabled.
// If the response code is 401 (unauthorized), a flag is set to indicate token renewal is needed.
#define RetError if (this->spotify->error_tracking) \
	this->spotify->last_message = "ResponseCode: " + std::to_string(res) + "\nReponse: " + response; \
    if (res == 401) \
        this->spotify->should_renew = true

// Macro for handling HTTP responses, logging errors if enabled, and returning an empty string on failure.
// Used for functions that expect a string response.
#define RetErrorString std::string response; \
long res = net.send(response); \
if (!WEB_SUCCESS(res)) { \
	RetError; \
	return std::string(); \
} \
return response

// Macro for handling HTTP responses, logging errors if enabled, and returning `false` on failure.
// Used for functions that expect a boolean response.
#define RetErrorBool std::string response; \
long res = net.send(response); \
if (!WEB_SUCCESS(res)) { \
	RetError; \
	return false; \
} \
return true
