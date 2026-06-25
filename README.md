# IRC - Internet Relay Chat Server

> A lightweight IRC server implementation in C++98 developed as part of the 42 curriculum.

## Description

### Project

`ft_irc` is a network programming project from 42 School that consists of building an IRC (Internet Relay Chat) server compatible with standard IRC clients.

The goal of the project is to gain experience with:

* TCP/IP networking
* Socket programming
* Non-blocking I/O
* Event-driven architectures
* Client-server communication
* IRC protocol basics (RFC 1459 / RFC 2812)

### Internet Relay Chat (IRC)

IRC is a real-time text communication protocol created in 1988 by Jarkko Oikarinen. It enables users to communicate through public channels and private one-to-one messages over a client-server architecture.

IRC became one of the earliest and most influential online communication systems, providing a lightweight and efficient way for users to exchange messages in real time.

The protocol is defined by several RFC documents, including:

* RFC 1459 – Internet Relay Chat Protocol
* RFC 2810 – Internet Relay Chat: Architecture
* RFC 2811 – Channel Management
* RFC 2812 – Client Protocol

These specifications define how IRC clients and servers communicate, manage channels, authenticate users, and exchange messages.

This project implements a simplified IRC server in C++98 that follows the requirements of the 42 School `ft_irc` subject while remaining compatible with standard IRC clients such as HexChat, irssi, and WeeChat.

## Installation

### Prerequisites

* C++ compiler with C++98 support
* Make

### Building the Project

1. Clone the repository:

```bash
git clone https://github.com/gabrielrial/ft_irc.git
cd ft_irc
```

2. Compile the project:

```bash
make
```

This will create an executable named `ircserv`.

## Usage

### Commands

```bash
./ircserv <port> <password>
```

Where:

* `<port>`: The port number on which the server will listen for incoming connections.
* `<password>`: The password required for clients to authenticate with the server.

Example:

```bash
./ircserv 1988 superpassword
```

## Connecting with an IRC Client
You can connect to the server using any standard IRC client such as HexChat, irssi, or WeeChat.
Configure the client with:
- Server IP address
- Port
- Password
- Nickname / Username

Once connected, you can start interacting with channels and other users.
For a complete list of supported commands, see:

[IRC Commands](docs/commands.md)


## 📚 References

- [RFC 1459 - Internet Relay Chat Protocol](https://tools.ietf.org/html/rfc1459)

- [RFC 2810 - Internet Relay Chat: Architecture](https://tools.ietf.org/html/rfc2810)

- [RFC 2811 - Internet Relay Chat: Channel Management](https://tools.ietf.org/html/rfc2811)

- [RFC 2812 - Internet Relay Chat: Client Protocol](https://tools.ietf.org/html/rfc2812)

- [RFC 2813 - Internet Relay Chat: Server Protocol](https://tools.ietf.org/html/rfc2813)

- [RFC 7194 - Default Port for Internet Relay Chat (IRC) via TLS/SSL](https://tools.ietf.org/html/rfc7194)

## Authors

* gabrielrial
* grr-ace 
* howset

