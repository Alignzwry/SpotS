Here's a README template tailored for your C++ Spotify API wrapper project:

---

# Spotify C++ API Wrapper

A lightweight, modern C++17 wrapper for the [Spotify Web API](https://developer.spotify.com/documentation/web-api/) that allows easy integration with Spotify's functionality, including authentication, playlist management, track playback control, album and artist data retrieval, and more.

## Features

- **Authentication**: Supports authorization flows with access and refresh tokens.
- **Albums**: Retrieve albums, tracks in albums, and manage user-saved albums.
- **Artists**: Access artist details, albums, top tracks, and related artists.
- **Episodes**: Manage and retrieve podcast episodes.
- **Player**: Control playback on devices, transfer playback, and retrieve playback state.
- **Playlists**: Manage and retrieve playlists, add or remove items, and customize details.
- **Search**: Basic search functionality.
- **Tracks**: Basic track functionality.
- **Users**: User profile and data management.
- **Error Logging**: Enable or disable error tracking for debugging.

## Requirements

- **C++17** or later
- [Spotify Developer Account](https://developer.spotify.com/dashboard/applications)
- Network library for HTTP requests (e.g., `Net.h` in your code)

## Getting Started

### Installation

Clone this repository and include it in your project:

```bash
git clone https://github.com/Alignzwry/SpotS.git
```

Include the necessary headers in your code:

```cpp
#include "Spotify.h"
```

### Initialization

1. Obtain your Spotify `client_id` and `client_secret` from the Spotify Developer Dashboard.
2. Create an instance of the `Spotify` class:

   ```cpp
   #include "Spotify.h"

   Spotify spotify("your_client_id", "your_client_secret");
   ```

### Authentication

To authenticate users, create an authorization URL and redirect them to Spotify's login page:

```cpp
std::vector<const char*> scopes = {Scopes::user_modify_playback_state, Scopes::user_read_playback_state};
std::string authUrl = spotify.getAuthUrl("your_redirect_uri", scopes);
// Redirect user to authUrl and obtain the token from the redirect URI
```

Once you receive the token, log in:

```cpp
spotify.auth("authcode_from_previous_step", "your_redirect_uri");
```

Now you should save the refresh token (spotify.refresh_token) into a file, to load it again if needed:

```cpp
spotify.refresh_token = "your_refresh_token";
spotify.refresh();
```

### Usage

#### Albums

```cpp
std::string albumInfo = spotify.Albums().getAlbum("album_id");
std::string userAlbums = spotify.Albums().getUserSavedAlbums();
```

#### Player Control

```cpp
spotify.Player().pause();
spotify.Player().playTracks({"track_id_1", "track_id_2"});
```

#### Playlists

```cpp
std::string playlist = spotify.Playlists().getPlaylist("playlist_id");
spotify.Playlists().createPlaylist("user_id", "My New Playlist", "Description here", true);
```

### Error Handling

Enable or disable error tracking with:

```cpp
spotify.enableErrors();
std::string lastError = spotify.getLastError();
```

## Examples

- Retrieve currently playing track:

  ```cpp
  std::string currentTrack = spotify.Player().getCurrentlyPlaying();
  ```

- Add an album to saved albums:

  ```cpp
  spotify.Albums().saveAlbums({"album_id"});
  ```

## Contributing

1. Fork the repository.
2. Create a new branch.
3. Make your changes and test them.
4. Submit a pull request.

## License

This project is licensed under the MIT License.
