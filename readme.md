# Client Tester

This tester creates multiple clients that connect to the server and send messages at
random times.
The following arguments are needed:

```sh
client_tester <number of clients> <server ip address> <port number>
```

where:

* \<number of clients\> = Number of threads to create (clients) who connect to the server
* \<server ip address \> = IP address of the irc server
* \<port number\> = Port number of the irc server
<br>
<br>

There are 2 different sequence options on how the tester will connect send and disconnect.

In both cases, if the server terminates or a user terminates the program with (ctrl-c, ctrl-/ etc),
the client tester will close all open sockets
and terminate.


The following section describes the 2 different sequence options for compiling the tester.

<br>
<br>

### Option 1

This is the default option when using the make file (ie.. make all) without any arguments.
The client tester has the following sequence when using this option.

1. client(s) will connect after a random waiting time.
2. client(s) will loop sending a message to the irc server at a random time interval.

To build this option, issue the following command:
```sh
make all
```

<br>


### Option 2

This option requires an argument to be passed to the make file.  The client tester has the following sequence
when using this option.

1. client(s) will connect after a random waiting time.
2. clients(s) will send a message to the irc server after a random time delay.
3. client(s) will disconnect from the irc server after a slight delay.
4. loop back to #1

To build this option, issue the following command:
```sh
make all D="-D RECONNECT_ON_MSG"
```

<br>
<br>

***NOTE: This client tester has only been tested on Linux, specifically Debian Bullseye, YMMV***
