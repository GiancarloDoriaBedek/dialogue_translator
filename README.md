# Dialogue Translator

## Table of Contents
* [Problem Description](#problem-description)
* [Technologies Used](#technologies-used)
* [Usage](#usage)

## Problem Description
- Dialogue data in the game I am developing is serialized into .json files. Those files are generated with a tool within the game engine.
- Since a lot of lines are repeated across multiple dialogue pints, actual conversation data is not stored within those dialogue files, they only contain keys that point to strings. Those strings are stored
- in seperate resource .json files. Editing those strings can be done by just editing resource file directly, but I have noticed that this approach can lead to edits that semantically fall out of place in their actual use.
- This is the problem this application tries to solve by allowing user to edit strings within the context of actual dialogue in which they are used.

## Technologies Used
- C++ 17
- MFC

## Usage
Load first and/or second resource file (if second file is not loaded, raw dialogue json will be displayed containg keys instead of values). After resource/s have been loaded, open
dialogue json with `Ctrl+o` or clicking "Open" button on the toolbar. Edit values in the left display. `Ctrl+s` or clicking "Save" button on a toolbar to save changes to first resource file.
