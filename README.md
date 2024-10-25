# Spotify API Wrapper

A lightweight, efficient Spotify API wrapper written in C++17, designed to facilitate integration with Spotify's API. This library enables you to manage playlists, control playback, and access user data effortlessly and way more.

## Features

- **Simple Authentication**: Easily authenticate using OAuth with Spotify’s API.
- **Playback Control**: Play, pause, skip, and manage Spotify playback remotely.
- **Playlist Management**: Retrieve, create, edit, and delete playlists.
- **Track and Album Access**: Fetch detailed metadata for tracks, albums, and artists.
- **Multithreading Support**: Designed to work seamlessly with multithreaded applications.

## Requirements

- **C++17 or higher**
- **Spotify Developer Account**: [Register here](https://developer.spotify.com/) to obtain a client ID and client secret.
- **Dependencies (included in project)**:
  - `cURL` - for handling HTTP requests.
  - `json` (optional) - for parsing JSON responses (recommended library: [nlohmann/json](https://github.com/nlohmann/json)).

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/spotify-api-wrapper.git
