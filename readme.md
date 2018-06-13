# Carcassonne
## College project
Simple carcassone-like game written in c.

Windows
---
To build on windows you need MinGW - tested on mingw32-base 2013072200.

Best, use the Git Bash with MinGW and slightly mod-ed .bashrc script:
	...
	$ cat ~/.bashrc
	#! /bin/bash
	eval `ssh-agent -s`
	ssh-add ~/.ssh/id_rsa
	
	PATH="/c/MinGW/bin:"$PATH
	alias make="mingw32-make.exe"
	...

See, that after Shebang you have one normal *space*!
See also, that PATH should be modificated with MinGW installation path coresponding to your previous decisions...

Linux
---
Use GCC 5+.

Mac OS X
---
Use GCC 5+ and SDK 10.12 at least.

http://www.wikigain.com/install-macos-sierra-10-12-virtualbox/
