# Simple VPN using TUN Interface and OpenSSL (C Implementation)

This project demonstrates the creation of a simple VPN (Virtual Private Network) using a TUN interface, UDP for communication, and asymmetric encryption with OpenSSL. The VPN establishes a secure tunnel between a client and a server, allowing data to be routed securely between them.

## Features

- TUN interface for routing IP packets.
- Asymmetric encryption using OpenSSL (public/private keys).
- UDP communication between client and server.
- NAT configuration on the server for routing traffic through the external network.

## Requirements

- **Linux** (with support for TUN/TAP interfaces)
- **OpenSSL 3.0+**
- **GCC** (for compiling the C code)
- **root** privileges (for creating and configuring TUN interfaces)

## Setup

### 1. Install Dependencies

Ensure that the required libraries and tools are installed on both the client and server:

```bash
sudo apt update
sudo apt install libssl-dev gcc make
```

### 2. Generate Public/Private Key Pairs

On **both the server and client**, generate a pair of RSA keys for encryption and decryption:

```bash
openssl genpkey -algorithm RSA -out private_key.pem -aes256
openssl rsa -pubout -in private_key.pem -out public_key.pem
```

- The **server** will use `private_key.pem` to decrypt data.
- The **client** will use `public_key.pem` to encrypt data.

### 3. Compile the Code

Compile both the client and server programs using the following commands:

- **Server**:

  ```bash
  gcc -o server server.c -lssl -lcrypto
  ```

- **Client**:

  ```bash
  gcc -o client client.c -lssl -lcrypto
  ```

### 4. Configure the Server

1. **Run the server**:

    ```bash
    sudo ./server
    ```

2. **Set up the TUN interface** and configure NAT:

    ```bash
    sudo ip addr add 10.0.0.1/24 dev bvpn
    sudo ip link set dev bvpn up
    sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
    echo 1 | sudo tee /proc/sys/net/ipv4/ip_forward
    ```

    - `10.0.0.1/24` is the IP range for the VPN network.
    - Replace `eth0` with the actual network interface you want to use for routing traffic.

### 5. Configure the Client

1. **Run the client**:

    ```bash
    sudo ./client
    ```

2. **Set up the TUN interface** and routing:

    ```bash
    sudo ip addr add 10.0.0.2/24 dev bvpn
    sudo ip link set dev bvpn up
    sudo ip route add default via 10.0.0.1 dev bvpn
    ```

    This configures the client to send all traffic through the VPN.

### 6. Testing the VPN

Once the VPN is set up, test the connection:

- **Ping from client to server**:

    ```bash
    ping 10.0.0.1
    ```

- **Ping from server to client**:

    ```bash
    ping 10.0.0.2
    ```

If the connection works, you should see the ping responses. All traffic from the client should now be routed through the server.

## Notes

- Make sure the server has a **public IP** and the client can reach it over the internet.
- This VPN is a **basic implementation** intended for educational purposes. It can be extended with additional security features (e.g., mutual authentication, stronger encryption, etc.).

## License

