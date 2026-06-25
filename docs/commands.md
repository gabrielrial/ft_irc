## 💬 Basic IRC Commands

Once connected to the server, you can use the following IRC commands:

### Join a Channel

Join an existing channel or create it if it does not exist:

```irc
JOIN #general
```

### Leave a Channel

Leave a channel:

```irc
PART #general
```

Optionally, include a reason:

```irc
PART #general :Goodbye everyone!
```

### Send a Message to a Channel

Send a message to all users in a channel:

```irc
PRIVMSG #general :Hello everyone!
```

### Send a Private Message

Send a direct message to another user:

```irc
PRIVMSG alice :Hello Alice!
```

### Change Your Nickname

```irc
NICK new_nickname
```

### Invite a User to a Channel

```irc
INVITE alice #general
```

### Kick a User from a Channel

```irc
KICK #general alice :Removed by operator
```

### View or Change a Channel Topic

View the current topic:

```irc
TOPIC #general
```

Set a new topic:

```irc
TOPIC #general :Welcome to our IRC channel
```

### Channel Modes

Make a channel invite-only:

```irc
MODE #general +i
```

Allow only operators to change the topic:

```irc
MODE #general +t
```

Set a channel password:

```irc
MODE #general +k secret
```

Set a user limit:

```irc
MODE #general +l 20
```

Grant operator privileges:

```irc
MODE #general +o alice
```

### Disconnect from the Server

```irc
QUIT
```

Or provide a quit message:

```irc
QUIT :Leaving the server
```
