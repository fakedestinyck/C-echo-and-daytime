# C echo and daytime
This is the secure coding project for RPI ITWS4370


# Manual
## echod

**NAME**

echod -- TCP based echo service daemon


**DESCRIPTION**

echod is a network daemon listens for TCP connections on TCP port 7.  Once a
   connection is established any data received is sent back.  This
   continues until the calling user terminates the connection.
   ([reference])(https://tools.ietf.org/html/rfc862)
   
echod listens on port 7. It uses `fork()` to create a child process, and use `seteuid` to drop privilege.
Then it accpets in coming trafic, and sends back no matter what it receives from the client.

If the client drop the connection by sending `q`, the server will terminate the connection as well. 


**NOTE**

_Privilege separation, or `seteuid` only take effects when running with root privileges._


## echo

**NAME**

echo -- TCP based echo service slient

**Usage**

`echo <hostname>`

**DESCRIPTION**

echo is a client that connects to the echod daemon.
It first tries to connect to user specified hostname at port 7. The hostname can be either IPv4 or IPv6. I use `strlcpy()` to prevent buffer overflow from user input, and use `inet_pton()` to check if the input is a valid IPv4 or IPv6 address. (`AF_INET` for IPv4 and `AF_INET6` for IPv6).

The timeout is set to 5 seconds. If the connection could not be established, it will print out an error and exit.
I refer to the code [here](https://blog.csdn.net/chenyulancn/article/details/52371873) when implementing the timeout feature.

Once the connection is established, user can send string to the server, and will get the same string back from the server.

Sending `q` to the server will let server to drop the connection. Client will also disconnect from the server.

## daytimed

**NAME**

daytimed -- TCP based daytime service daemon


**DESCRIPTION**

daytimed is a network daemon listens for TCP connections on TCP port 13.  Once a
   connection is established the current date and time is sent out the
   connection as a ascii character string (and any data received is
   thrown away).  The service closes the connection after sending the
   quote.([reference])(https://tools.ietf.org/html/rfc867)
   
   
eaytimed listens on port 13. It uses `fork()` to create a child process, and use `seteuid` to drop privilege.
Then it accpets in coming trafic, ignores what it get from the client, and sends back the current time in the following format:
```
Wednesday, April 24, 2019 17:18:45-EDT
```

Then the server drops the connection and wait for other connections.


**NOTE**

_Privilege separation, or `seteuid` only take effects when running with root privileges._


## daytime

**NAME**

daytime -- TCP based daytime service slient

**Usage**

`daytime <hostname>`

**DESCRIPTION**

daytime is a client that connects to daytimed daemon.
It first tries to connect to user specified hostname at port 13. The hostname can be either IPv4 or IPv6. I use `strlcpy()` to prevent buffer overflow from user input, and use `inet_pton()` to check if the input is a valid IPv4 or IPv6 address. (`AF_INET` for IPv4 and `AF_INET6` for IPv6).

The timeout is set to 5 seconds. If the connection could not be established, it will print out an error and exit.
I refer to the code [here](https://blog.csdn.net/chenyulancn/article/details/52371873) when implementing the timeout feature.

Once the connection is established, will get the current timestring from the server in this format:
```
Wednesday, April 24, 2019 17:18:45-EDT
```

Then the client close the connection.
