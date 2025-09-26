# ft_irc

## DCC Transfer
### Overview
1. Initiation Phase
	- The sender initiates by sending a DCC SEND request through the IRC server
	- This message contains:
		- The filename
		- The sender's IP address
		- A port number the sender will listen on
		- The file size
2. Connection Establishment
	- The sender opens a TCP socket and listens for incoming connections
	- The recipient receives the DCC SEND request via the IRC server
	- If the recipient accepts, they connect directly to the sender's IP and port
	- This creates a direct connection between the two clients, bypassing the IRC server
3. Transfer Phase
	- Once connected, the file transfer happens directly between the two clients
	- The IRC server is not involved in the actual file transfer

### Flow
1. Initiation (`handle_dcc_send`):
	- When a client wants to send a file, they call `handle_dcc_send`
	- The DCCManager creates a new socket (`create_dcc_socket`)
	- A new DCCTransfer object is created with sender and receiver info
2. Setup Phase:
	- The sender's socket is created and bound to a port
	- The port number and file details are sent to the receiver via IRC server
	- The sender waits for the receiver to accept
3. Acceptance (`handle_dcc_accept`):
	- When receiver accepts, they call `handle_dcc_accept`
	- The receiver connects directly to the sender's socket
	- A new DCCTransfer object tracks the transfer state
4. Transfer Monitoring:
	- The `check_transfers()` method (called in Server's main loop) monitors active transfers
	- It processes ongoing transfers using `process_transfer`
	- Handles timeouts via `handle_timeout`
	- Tracks bytes transferred and completion status
5. Cleanup:
	- When transfer completes, `cleanup_transfer` is called
	- Sockets are closed and transfer objects are removed from `_transfers`

### How to use
1. Open hexchat, connect to server
2. Command: `/dcc send <user> <absolute path to file>`

Note: AFAIK hexchat only has access to the Downloads folder in cluster's computers