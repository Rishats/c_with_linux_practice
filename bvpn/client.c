#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/if_tun.h>
#include <net/if.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define PORT 12345
#define BUF_SIZE 1024

// Функция создания TUN интерфейса
int tun_alloc(char *dev, int flags) {
    struct ifreq ifr;
    int fd, err;

    if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
        perror("Ошибка открытия TUN/TAP");
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = flags;

    if (*dev) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if ((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
        perror("Ошибка создания интерфейса");
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}

// Шифрование данных с использованием нового API OpenSSL 3.0
int encrypt_data(unsigned char *data, int data_len, unsigned char *encrypted) {
    EVP_PKEY *pubkey = NULL;
    FILE *pubkey_file = fopen("public_key.pem", "rb");

    if (!pubkey_file) {
        perror("Ошибка открытия публичного ключа");
        return -1;
    }

    pubkey = PEM_read_PUBKEY(pubkey_file, NULL, NULL, NULL);
    fclose(pubkey_file);

    if (!pubkey) {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pubkey, NULL);
    if (!ctx) {
        EVP_PKEY_free(pubkey);
        return -1;
    }

    if (EVP_PKEY_encrypt_init(ctx) <= 0) {
        EVP_PKEY_free(pubkey);
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    size_t encrypted_len;
    if (EVP_PKEY_encrypt(ctx, NULL, &encrypted_len, data, data_len) <= 0) {
        EVP_PKEY_free(pubkey);
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    if (EVP_PKEY_encrypt(ctx, encrypted, &encrypted_len, data, data_len) <= 0) {
        EVP_PKEY_free(pubkey);
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    EVP_PKEY_free(pubkey);
    EVP_PKEY_CTX_free(ctx);
    return encrypted_len;
}

// UDP клиент для отправки данных
void udp_send(char *ip, int tun_fd) {
    int sockfd;
    struct sockaddr_in servaddr;
    unsigned char buffer[BUF_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Ошибка создания сокета");
        return;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, ip, &servaddr.sin_addr);

    while (1) {
        int n = read(tun_fd, buffer, BUF_SIZE);  // Чтение данных из TUN интерфейса

        unsigned char encrypted[BUF_SIZE];
        int enc_len = encrypt_data(buffer, n, encrypted);

        sendto(sockfd, encrypted, enc_len, MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    }

    close(sockfd);
}

int main() {
    char tun_name[IFNAMSIZ] = "bvpn";
    int tun_fd = tun_alloc(tun_name, IFF_TUN | IFF_NO_PI);

    if (tun_fd < 0) {
        fprintf(stderr, "Ошибка создания TUN интерфейса\n");
        return 1;
    }

    printf("TUN интерфейс %s создан\n", tun_name);

    char *server_ip = "161.35.213.153";  // IP-адрес сервера
    udp_send(server_ip, tun_fd);  // Отправка данных серверу

    close(tun_fd);
    return 0;
}

