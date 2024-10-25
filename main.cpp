#include <Windows.h>
#include <curl/curl.h>
#include "SpotS/Spotify.h"
#include "SpotS/StrUtil.h"
#include <iostream>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

#define PORT 3823
bool auth();
Spotify spotify("your_client_id", "your_client_secret");
std::string auth_code = "";

///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////              --Example Code (Don't use this)--                //////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

int main() 
{
	// Check for refresh token
	const fs::path path = fs::current_path() / "refresh_token";
	if (fs::exists(path)) {
		std::ifstream file(path);
		std::string refresh_token;
		std::getline(file, refresh_token);
		file.close();

		spotify.refresh_token = refresh_token;

		// Login using refresh token
		if (!spotify.refresh())
		{
			std::cerr << "[ ERROR ] : Failed to login." << std::endl;
		}
		else {
			std::cout << "[ SUCCESS ] : Logged in." << std::endl;
			goto _run;
		}
	}

	// Get auth code
	if (!auth())
	{
		std::cerr << "[ ERROR ] : Failed to authenticate." << std::endl;
		return 1;
	}
	std::cout << "[ SUCCESS ] : Authenticated." << std::endl;

	// Login using auth code
	if (!spotify.auth(auth_code, "http://localhost:" + std::to_string(PORT) + "/callback"))
	{
		std::cerr << "[ ERROR ] : Failed to login. error:" << std::endl;
		std::cout << spotify.getLastError() << std::endl;
		return 1;
	}
	std::cout << "[ SUCCESS ] : Logged in." << std::endl;

	// Save refresh token
	{
		std::ofstream file(path);
		file.write(spotify.refresh_token.c_str(), spotify.refresh_token.size());
		file.close();
	}

_run:
	//---Use Spotify API---//

	
	// Add Item to playlist
	if (!spotify.Playlists().addItems("your_playlist_id", { "your_track_id" })) {
		std::cerr << "[ ERROR ] : Failed to add items to playlist. error:" << std::endl;
		std::cout << spotify.getLastError() << std::endl;
		return 1;
	}

	// Remove Item
	if (!spotify.Playlists().removeItems("your_playlist_id", { "your_track_id" })) {
		std::cerr << "[ ERROR ] : Failed to remove items from playlist. error:" << std::endl;
		std::cout << spotify.getLastError() << std::endl;
		return 1;
	}

	// Play a song
	if (!spotify.Player().playTracks({ "your_track_id1", "your_track_id2" }, 20000, 0)) {
		std::cerr << "[ ERROR ] : Failed to play tracks. error:" << std::endl;
		std::cout << spotify.getLastError() << std::endl;
		return 1;
	}
}

void on_client_connect(SOCKET client, std::string* auth_code)
{
	std::string html_response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n";
	html_response += R"(
<!DOCTYPE html>
<html lang=\"en\">
   <head>
      <meta charset=\"UTF-8\">
      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
      <title>Success!</title>
      <style> body { margin: 0; padding: 0; height: 100vh; background: linear-gradient(to bottom right, rgba(38, 38, 44, 1), rgba(44, 41, 46, 1)); background: -webkit-linear-gradient(to bottom right, rgba(38, 38, 44, 1), rgba(44, 41, 46, 1)); background: -moz-linear-gradient(to bottom right, rgba(38, 38, 44, 1), rgba(44, 41, 46, 1)); background: -o-linear-gradient(to bottom right, rgba(38, 38, 44, 1), rgba(44, 41, 46, 1)); background: -ms-linear-gradient(to bottom right, rgba(38, 38, 44, 1), rgba(44, 41, 46, 1)); display: flex; justify-content: center; align-items: center; font-family: Arial, sans-serif; text-align: center; } h1 { color: yellow; }</style>
   </head>
   <body>
      <div>
         <h1>You can close this window now!</h1>
      </div>
   </body>
</html>
)";

	// Send HTML response
	send(client, html_response.c_str(), html_response.size(), 0);

	// Set a receive timeout for the socket (e.g., 5 seconds)
	int timeout = 10000; // in milliseconds
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

	char buffer[1024] = { 0 };
	while (true) {
		int recv_size = recv(client, buffer, sizeof(buffer), 0);
		if (recv_size <= 0) {
			// Timeout or connection closed, exit loop
			break;
		}
		std::string str(buffer, recv_size);
		*auth_code = util::subStrBtw(str, "/callback?code=", " HTTP/1.1");
		if (!auth_code->empty()) {
			break;
		}
	}

	// Cleanup and close socket after the client has finished loading the page
	shutdown(client, 0x01);  // Gracefully shut down the send-side
	closesocket(client);
}

bool auth() {

	WSADATA wsa_data;
	SOCKADDR_IN server_addr, client_addr;
	WSAStartup(MAKEWORD(2, 2), &wsa_data);

	// Create a socket to listen on
	const auto server = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	// Bind and listen on the port
	::bind(server, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(server_addr));
	listen(server, 0);

	int client_addr_size = sizeof(client_addr);

	// Get Auth Url
	std::string auth_url = spotify.getAuthUrl("http://localhost:" + std::to_string(PORT) + "/callback", {
		Scopes::app_remote_control,
		Scopes::playlist_modify_private,
		Scopes::playlist_modify_public,
		Scopes::playlist_read_collaborative,
		Scopes::playlist_read_private,
		Scopes::streaming,
		Scopes::user_follow_modify,
		Scopes::user_follow_read,
		Scopes::user_library_modify,
		Scopes::user_library_read,
		Scopes::user_modify_playback_state,
		Scopes::user_read_currently_playing,
		Scopes::user_read_email,
		Scopes::user_read_playback_position,
		Scopes::user_read_playback_state,
		Scopes::user_read_private,
		Scopes::user_read_recently_played
		});

	// Open the authorization URL in the default browser
	ShellExecute(0, 0, auth_url.c_str(), 0, 0, SW_SHOW);

	// Accept the client connection and get the auth code
	while (auth_code.empty()) {
		SOCKET client = accept(server, reinterpret_cast<SOCKADDR*>(&client_addr), &client_addr_size);
		if (client != INVALID_SOCKET) {
			on_client_connect(client, &auth_code);
		}

		// Check for socket errors
		const auto last_error = WSAGetLastError();
		if (last_error > 0) {
			break;
		}
	}

	// Cleanup server socket
	closesocket(server);
	WSACleanup();

	return !auth_code.empty();
}