# mr-gamelister
Produce a list of installed MiSTer games in the format of system-gameName i.e. the same format as required by mr_marquee

# Description
This program reads from a zaproo SQLite database (default: media.db) and lists game names in the format "SystemName-GameName.jpg".
Make sure to update your zaproo media database before running this program to ensure it has the latest game information!

 `Usage: gameLister.exe [path_to_database]`

 Compile the code with: gcc gameLister.c -lsqlite3 -o gameLister.exe
 
 Insure to install the SQLite3 development libraries i.e. apt install sqlite3 libsqlite3-dev
