# Edenfell Launcher
This is a tiny launcher to assist with launching Edenfell, mainly to give command line arguments and connection info to the process.

It works only on Windows, and can be easily adapted to work with other games and processes if you wish.

## Setup

The launcher uses premake to manage the build process, and generating project files. More info on Premake can be found here: https://premake.github.io

To setup the project to get coding and compile, follow these instructions:

1. Clone the project:
   
  `git clone https://github.com/wmcnamara/edenfell-launcher/`

3. Go inside the Edenfell launcher directory

  `cd edenfell-launcher`

4. Run Premake with your editor as the command line arguments.
   For Visual Studio 2022:
   
  `./Premake5.exe vs2022`
  
   This will generate a solution file. For info on other parameters for other IDE's check Premake's docs.

Then, open the project files, and start coding!
   
# Output Format

The launcher will run "edenfell.exe" in its current working directory with the command line arguments like as follows:

`edenfell.exe -host -127.0.0.1 -7777`
