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

# Configuration Files

Located in the ./conf directory are several configuration files for sending messages, in this case to an irc server.  The files are as follows

1. replace.conf
2. connect.conf
3. after-connect.conf
4. loop.conf
5. disconnect.conf

There are special paramters {0}, {1}... {4} that replace internal variables in the .conf files.  Further description on the different variables available are listed below.
<br>
<br>
## replace.conf

The replace.conf file is used to replace text in the  < > with the value specified.

```
<password> xxxx
<nick> client
```

Every tag `<password>` will be replaced by xxxx, similare to `<nick>` which will be replaced by the word client in all the .conf files.
<br>


## connect.conf

The connect.conf file is executed during the connection of a client to the server, the execution only happens once.  Commands in this file are send in rapid succession.
<br>
### after-connect.conf

The after-connect.conf file is executed after establishing the connection to the server and are executed only once.

<br>

## loop.conf

The loop.conf file contains commands that are executed in a loop, sent at a random interval of time.
<br>

### disconnect.conf

The disconnect.conf file is executed when the client disconnects from the server.  Since the client tester is running in an infinite loop, a control C will stop the client tester and the disconnect.conf commands will be executed on the server.

<br>

## Special Variables

There are some special variables that can be used in all the .conf files that get replaced by the client-tester.  The following values can be used:

{0} = replaced with the IP Address specified on the command line during startup

{1} = replaced with the port number specified on the command line during startup

{2} = replaced with the ClientId.  The ClientId is a internal numbers that is assigned durinmg the starting of each client.  The ClientID starts from 0...n, where n is the number of clients.  Each clientId is incremented by 1 on each client creation.

{3} = replaced with the time interval of the clients send loop, and ranges from 3 to 60 seconds.

{4} = replaced with the internal File Descriptor that is used to connect to the server.  Like the ClientId, the operating system will increment on each open / connect to the server.
<br>
<br>

***NOTE: This client tester has only been tested on Linux, specifically Debian Bullseye, YMMV***
