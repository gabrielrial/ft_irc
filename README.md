# ft_irc

Howard:
JOIN now sends/updates the list of users (clients) in the channel to all users if a new user joins. 
This has not been integrated to main. 

I merged the most updated main to this branch, but I have compilation errors and cant get the password to work.
So I edited `register_client` for the compilation (`get_hostname` --> `gethostname`) and comment out the password part in `process_line`.

I will postpone merging the latest iteration of JOIN until the main is confirmed.

## To do:
1. ~~make the server response to use the actual IP/hostname of the server instead of just "localhost".~~ done (dammit maybe just use SERVER NAME macro?)
2. ~~rethink if `names_list` should be a method in `Channel` or better be moved to be directly under `cmd_join` because it sends a server response regarding JOIN and maybe it makes no sense to have it under `Channel`.~~ now under `cmd_join`
3. must/should make the JOIN command support multiple channels as the parameter
4. make an implementation of WHO (?).
5. maybe change the branch name to better reflect the current progress.
6. if WHO is too difficult, maybe just try PART or TOPIC.

## Notes:
`WHO [<mask> [<o>]]`
- <mask> can be a channel name or a wildcard pattern matching users
- <o> is an optional parameter that, when set to "o", only shows operators

### Important
1. channel query
	- when used with a channel name: WHO #channel
	- lists all visible users in that channel
	- shows information like username, hostname, server, nickname, and status
2. pattern matching
	- can use wildcards to match usernames/hostnames
	- example: WHO *.com would show users from .com domains
3. server response `:server 352 <requesting_user> <channel> <username> <host> <server> <nick> <flags> :<hopcount> <realname>`
	- Flags:
		- `H` - User is not away (Here)
		- `G` - User is away (Gone)
		- `*` - User is an IRC operator
		- `@` - Channel operator in the channel
		- `+` - Voice privilege in the channel