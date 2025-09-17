#!/bin/bash

# Configuration
SERVER="irc.libera.chat"
PORT=6667
NICK="MyNick"
USER="MyNick"
REALNAME="My Name"
CHANNEL="#general"

# Function to handle the connection
(
  # Register nickname and user
  echo "NICK $NICK"
  echo "USER $USER 0 * :$REALNAME"

  # Loop to handle PINGs and server messages
  while read line; do
    echo "$line"      # Display server message
    if [[ "$line" =~ ^PING\ :(.+) ]]; then
      echo "PONG :${BASH_REMATCH[1]}"
    fi
  done
) | nc -C $SERVER $PORT
