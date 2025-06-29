# Shiden Visual Novel Editor

[Japanese](https://github.com/HANON-games/Shiden/blob/main/README.md) / English

<br>
<p align="center">
    <img src="https://github.com/user-attachments/assets/e1b16acb-e6e2-4f85-ae70-af718be2836d" width="60%" />
</p>
<br>

## Introduction

Shiden Visual Novel Editor is a plugin that allows easy creation of visual novel games in UE5. <br>
It is designed to be simple and flexible, allowing for more than just visual novel games.

![Shiden Visual Novel Editor](https://github.com/user-attachments/assets/3029cdc2-5460-45dd-8463-c78356569b76)

## Features

- Editor with built-in preview
    - Includes an intuitive editor that allows for easy manipulation.
    - You can preview the current editing line and see the results in real-time (some commands are not supported in preview).
    - You can save combinations of frequently used commands and parameters as presets.
    - The macro function allows you to combine frequently used commands into a single command.
    - Import/export functionality in CSV/JSON format.
- Various features required for visual novel games
    - 37 types of basic commands
    - Basic save functionality
    - Simple variable functionality
        - Includes both variables managed per save slot and system variables.
    - Line-level read tracking
    - Parallel processing (strictly speaking, concurrent processing)
    - Simple backlog functionality
    - Simple fade functionality
- High extensibility
    - Commands can be implemented in either C++ or Blueprint, allowing for easy addition of commands without modifying the main plugin.
    - It is also possible to distribute additional commands as plugins.
    - The editor section is implemented in Blueprint, allowing for easy extension.

## System Requirements

- UE5.3 ～ 5.6
- Windows, Mac, Android

It is highly likely to work on iOS as well, but editing of the uplugin file is required.

## Installation

Unzip the downloaded file from [Releases](https://github.com/HANON-games/Shiden/releases/latest) and place the Shiden folder into the Plugins folder of your project.

## Usage

Please refer to the [Wiki](https://github.com/HANON-games/Shiden/wiki/Home-(en)).

## License

MIT

## Created By

[葉乃音 (HANON)](https://twitter.com/HanonHeartKnows)

## Bug Reports

If you encounter any issues, please report them on [issues](https://github.com/HANON-games/Shiden/issues). <br>
Please note that due to the author's busy schedule, there may be delays in response.

## Changelog

Please refer to [Announcements](https://github.com/HANON-games/Shiden/discussions/categories/announcements-%E3%81%8A%E7%9F%A5%E3%82%89%E3%81%9B).
