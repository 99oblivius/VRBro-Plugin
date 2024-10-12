# VRBro Plugin for OBS Studio

![VRBro Banner](assets/VRBro_banner.png)

## Introduction

VRBro Plugin is an OBS Studio plugin designed to bridge the gap between virtual reality and content creation. 
It works in tandem with a SteamVR overlay application to provide seamless control of OBS Studio directly from within VR.

## GUI

![VRBro GUI](assets/VRBro_Server_Settings.png)

## Features

- Creates and manages a simplified WebSocket server for fast, performant control of OBS from in-game VR environments
- Enables VR users to control OBS functions without leaving their VR experience
- Supports various OBS actions such as starting/stopping streaming, recording, and replay buffer
- Provides a user-friendly settings interface within OBS Studio

## How It Works

1. The VRBro Plugin sets up a WebSocket server within OBS Studio.
2. A companion SteamVR overlay application (sold separately) connects to this WebSocket server.
3. VR users can configure their VR controller bindings to send commands to OBS via the overlay.
4. The plugin receives these commands and executes the corresponding actions in OBS Studio using the Frontend API.

## Installation

1. Download the latest release of the VRBro Plugin from the [Releases](https://github.com/yourusername/vrbro-plugin/releases) page.
2. Extract the contents of the zip file to your OBS Studio plugins folder:
   - Windows: `C:\Program Files\obs-studio\obs-plugins\64bit\`
   - macOS: `/Applications/OBS.app/Contents/PlugIns/`
   - Linux: `/usr/lib/obs-plugins/` or `~/.obs-studio/plugins/`
3. Restart OBS Studio.

## Configuration

1. In OBS Studio, go to Tools > VRBro Server Settings.
2. Enable the "Auto Buffer" feature to automatically start the replay buffer when OBS loads, if desired.
3. Optionally, set a custom listening address and port for the WebSocket server.

## Usage

1. Ensure the VRBro Plugin is properly configured in OBS Studio.
2. Launch your VR game and the companion SteamVR overlay.
3. Use your configured VR controller bindings to send commands to OBS.

## Supported Commands

- Start/Stop Streaming
- Start/Stop Recording
- Start/Stop Replay Buffer
- Save Replay Buffer
- Split Recording File

## Development

This plugin is built using C++ and relies on the following libraries:

- OBS Studio API
- Asio (for asynchronous I/O)
- nlohmann/json (for JSON parsing)

To build the plugin from source:

1. Clone the repository
2. Ensure you have CMake and a C++ compiler installed
3. Run the following commands:
   ```
   cmake -B build -S .
   cmake --build build
   ```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the [GNU General Public License v2.0](LICENSE).

## Acknowledgements

- OBS Studio team for their excellent software and API
- Asio and nlohmann/json library authors

## Contact

For support, feature requests, or bug reports, please [open an issue](https://github.com/yourusername/vrbro-plugin/issues) on GitHub.
