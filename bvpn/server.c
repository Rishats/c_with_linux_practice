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

// Дешифрование данных с использованием нового API OpenSSL 3.0
int decrypt_data(unsigned char *enc_data, int enc_data_len, unsigned char *decrypted) {
    EVP_PKEY *privkey = NULL;
    FILE *privkey_file = fopen("private_key.pem", "rb");

    if (!privkey_file) {
        perror("Ошибка открытия приватного ключа");
        return -1;
    }

    privkey = PEM_read_PrivateKey(privkey_file, NULL, NULL, NULL);
    fclose(privkey_file);

    if (!privkey) {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(privkey, NULL);
    if (!ctx) {
        EVP_PKEY_free(privkey);
        return -1;
    }

    if (EVP_PKEY_decrypt_init(ctx) <= 0) {
        EVP_PKEY_free(privkey);
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    size_t decrypted_len;
    if (EVP_PKEY_decrypt(ctx, NULL, &decrypted_len, enc_data, enc_data_len) <= 0) {
        EVP_PKEY_free(privkey);
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    if (EVP_PKEY_decrypt(ctx, decrypted, &decrypted_len, enc_data, enc_data_len) <= 0) {
        EVP_PKEY_free(privkey);
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    EVP_PKEY_free(privkey);
    EVP_PKEY_CTX_free(ctx);
    return decrypted_len;
}

// UDP сервер для получения данных
void udp_receive(int tun_fd) {
    int sockfd;
    struct sockaddr_in servaddr, clientaddr;
    char buffer[BUF_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Ошибка создания сокета");
        return;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Ошибка привязки сокета");
        close(sockfd);
        return;
    }

    int len = sizeof(clientaddr);
    while (1) {
        int n = recvfrom(sockfd, buffer, BUF_SIZE, MSG_WAITALL, (struct sockaddr *)&clientaddr, &len);
        buffer[n] = '\0';

        // Дешифрование данных
        unsigned char decrypted[BUF_SIZE];
        int decrypted_len = decrypt_data((unsigned char *)buffer, n, decrypted);

        if (decrypted_len > 0) {
            write(tun_fd, decrypted, decrypted_len);  // Передаем данные в TUN интерфейс
        }
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

    udp_receive(tun_fd);  // Ожидание данных от клиента и передача их в TUN интерфейс

    close(tun_fd);
    return 0;
}

