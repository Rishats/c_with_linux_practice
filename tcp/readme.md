## Client-Side Socket Programming in C

This is a simple client-side C program that demonstrates how to establish a TCP connection with a server, send an HTTP GET request, and receive a response using socket programming.

### Prerequisites
- C compiler (e.g., GCC)
- Networking libraries included in the system (such as `<arpa/inet.h>`, `<sys/socket.h>`)
- Access to a server running on port `5000` at the IP address `164.90.228.239`

### Description
This program sends an HTTP request to a server located at IP `164.90.228.239` on port `5000` and receives a response. It demonstrates the basic TCP socket workflow, including:
- Socket creation
- Connection to the server
- Sending the HTTP request
- Receiving and printing the server's response
- Closing the connection

### Steps to Compile and Run
1. **Compile the program:**
   ```bash
   gcc -o client client.c
   ```

2. **Run the program:**
   ```bash
   ./client
   ```

### Program Walkthrough
- **Socket Creation:**  
  A socket is created using the `socket()` function, which initializes the TCP connection.
  
- **Connection:**  
  The `connect()` function is used to establish a connection to the server at the specified IP and port.

- **HTTP Request:**  
  An HTTP GET request is built in a string format and sent using the `send()` function. The request is for the resource `/static/alerts.html` on the server.

- **Response:**  
  The server's response is received via the `read()` function and printed to the console.

- **Connection Closure:**  
  The connection is closed using the `close()` function after the data exchange is complete.

### Example HTTP Request
The request sent to the server looks like this:
```
GET /static/alerts.html HTTP/1.1
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9,ru;q=0.8
Cache-Control: max-age=0
Connection: close
Host: 164.90.228.239:5000
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36
```

---

### How TCP Works: Wireshark Analysis

TCP (Transmission Control Protocol) ensures reliable, ordered, and error-checked delivery of data between applications over a network. In Wireshark, a TCP connection will typically exhibit the following sequence:

| TCP Step | Description | Wireshark Packet Info | Flags Used |
|----------|-------------|-----------------------|------------|
| **1. SYN** | The client initiates a connection by sending a SYN (synchronize) message to the server. | `SYN` sent from client to server (port 5000). | `SYN` |
| **2. SYN-ACK** | The server responds with a SYN-ACK (synchronize-acknowledge) message, indicating it is ready to establish a connection. | `SYN-ACK` from server to client. | `SYN`, `ACK` |
| **3. ACK** | The client sends an ACK (acknowledge) message, completing the three-way handshake and establishing the connection. | `ACK` from client to server. | `ACK` |
| **4. Data Transfer** | The client sends the HTTP GET request to the server, and the server responds with the requested data. | HTTP GET request followed by HTTP response. | `PSH`, `ACK` (for pushing data) |
| **5. FIN** | After the data is transferred, the client or server can initiate a connection teardown by sending a FIN (finish) message. | `FIN` from client or server to close the connection. | `FIN` |
| **6. ACK** | The other side acknowledges the FIN message and sends its own FIN message to close the connection. | `ACK` followed by `FIN-ACK` from client or server. | `ACK`, `FIN` |

---

### How to Analyze TCP with Wireshark
1. **Open Wireshark** and start capturing on the network interface that will communicate with the server.
2. **Run the client program** to initiate the TCP connection and send the HTTP request.
3. **Filter by the TCP connection** using the server's IP address and port:
   ```
   tcp.port == 5000 && ip.addr == 164.90.228.239
   ```
4. **Inspect the packets**:
   - Look for the three-way handshake: `SYN`, `SYN-ACK`, `ACK`.
   - Look for the HTTP request and the server's response.
   - Look for the connection termination: `FIN`, `ACK`.

---

### Troubleshooting
- If the socket creation fails, ensure that your machine is connected to the network and the server is reachable.
- If the connection fails, verify that the server is running and that no firewall rules are blocking the port (5000).
