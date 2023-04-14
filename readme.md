# Client Tester

This tester creates multiple clients that connect to the server and send messages at
random times.  Each client has its own thread and will connect to the server, to avoid all the clients trying to connect at one time and send messages all at once, a random delay is introduced on each client.  For example, the command line client_tester 2 127.0.0.1 6667 will create 2 clients (threads), during the creation of the clients, client 1 may receive a delay of 5 seconds and client 2 may receive a delay of 30 seconds.  Durring the loop execution, client 1 will loop every 5 seconds and client 2 will loop every 30 seconds.  This creates an even distribution of clients connecting and sending loop messages to the server and spreads the load over all the connecting clients, rather than having a huge spike of client connections and messages a delay then another huge spike of messages which is unrealistic in a real server setup.

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

Every tag `<password>` will be replaced by xxxx, similar to `<nick>` which will be replaced by the word client in all the .conf files.
<br>


## connect.conf

The connect.conf file is executed during the connection of a client to the server, the execution only happens once.  Commands in this file are sent in rapid succession.
<br>

## after-connect.conf

The after-connect.conf file is executed after establishing the connection to the server and are executed only once.
A delay can be added between commands, see the section delay between commands
<br>

## loop.conf

The loop.conf file contains commands that are executed in a loop, sent at a random interval of time.
<br>


## disconnect.conf

The disconnect.conf file is executed when the client disconnects from the server.  Since the client tester is running in an infinite loop, a control C will stop the client tester and the disconnect.conf commands will be executed on the server.
<br>

## **Delay between commands**
The Delay between commands can be used in the connect.conf, after-connect.conf and loop.conf files.
You can add a delay between commands with [1000] between commands, this will delay by 1000 miliseconds or 1 second.  An example is shown below.

```
PART <channel>
[1000]
JOIN <channel>
```
The above will part the channel ( the channel name replaced by the tags specified in replace.conf ) wait 1 second ( 1000 milisecond ) and then join the channel again.  The program will loop ( if used in the loop.conf ) after a random amount of time determined during startup.

The Special Variables can also be used in combination with the above to use the random time delay like the following:
```
PART <channel>
[{3}000]
JOIN <channel>
```
Since the special variables {3} is the random timer interval in second, it will be replaced with the time interval determined during startup and then converted to delay in miliseconds, in this case if {3} delays for 5 seconds ( determined during the startup and randomly assigned ), the replacement will be [5000] which is 5 seconds ( 5000 miliseconds ).

<br>

## **Special Variables**

There are some special variables that can be used in all the .conf files that get replaced by the client-tester.  The following values can be used:

{0} = replaced with the IP Address specified on the command line during startup

{1} = replaced with the port number specified on the command line during startup

{2} = replaced with the ClientId.  The ClientId is a internal numbers that is assigned durinmg the starting of each client.  The ClientID starts from 0...n, where n is the number of clients.  Each clientId is incremented by 1 on each client creation.

{3} = replaced with the time interval of the clients send loop, and ranges from 3 to 60 seconds.  This can also be used in combination with the loop delay.  Each client (thread) will have its own random delay that is calculated when the client is started up.

{4} = replaced with the internal File Descriptor that is used by each client to connect to the server.  Like the ClientId, the operating system will increment on each open / connect to the server.
<br>
<br>

As always, if you find an issue... Please submit a Pull Request.

\<opsec-infosec\> Ravenclaw Rocks!!!  GO MAY PISCINE!!!

<br>

***NOTE: This client tester has only been tested on Linux, specifically Debian Bullseye, YMMV***
