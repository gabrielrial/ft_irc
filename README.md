# ft_irc

I'm not sure if any of you would read this. But first sorry for the obscure naming of this branch, I didnt know where I was going or what I was doing. That said, the hs_test_server folder contains the mini server(s) like gabriel's. 

Running ` c++ -Wall -Wextra -Werror -std=c++98 mini_server_multi_client.cpp -o mc ` creates the executable `./mc` that runs the server.

To test it:
1. run hexchat.
2. go to network list (Ctrl+S).
3. add a network & give name.
4. edit the network.
5. add 127.0.0.1/6667 (or something else but has to be the same as the server)
6. then write a couple of things.
7. open another instance of hexchat (or maybe other IRC client) and test multi client.