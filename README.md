# C echo and daytime
This is the secure coding project for RPI ITWS4370


# Manual
## echod

**NAME**

echod -- TCP based echo service daemon


**DESCRIPTION**

Echod is a network daemon listens for TCP connections on TCP port 7.  Once a
   connection is established any data received is sent back.  This
   continues until the calling user terminates the connection.
   
   
echod listens on port 7. It uses `fork()` to create a child process, and use `seteuid` to drop privilege.
Then it accpets in coming trafic, and sends back no matter what it receives from the client.


**NOTE**

_Privilege separation, or `seteuid` only take effects when running with root privileges._


## echo

**NAME**

echo -- TCP based echo service slient

**Usage**

`echo <hostname>

**DESCRIPTION**

Echo is a client that connects to echod daemon.
It first tries to connect to user specified hostname at port 7. 

The timeout is set to 5 seconds. If the connection could not be established, it will print out an error and exit.
I refer to the code [here](https://blog.csdn.net/chenyulancn/article/details/52371873) when implementing the timeout feature.

Once the connection is established, user can send string to the server, and will get the same string back from the server.

