# ft_irc

This branch is intended for the module to parse the raw text line sent by a client to the server.

~~Currently I have no concrete plan of how to achieve this, other than to use the test mini_server as a temporary substitute before the real server is up and running.~~

Current plan:
- make every raw text line an object
- store the relevant parts of the line separately
	- let `_prefix` be empty
	- store `_command` & convert to uppercase
	- store `_params` as vector
	- store `_trailing` as it is
- this means parse the text line carefully



