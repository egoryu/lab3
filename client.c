#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000
#define DEBUGFS_PATH "/sys/kernel/debug/lab20/memblock_dentry_info"

int main(int argc, char *argv[]) {
    int fd;
    char buffer[BUFFER_SIZE];
    
    if (argc != 3) {
        perror("Неправильное количество аргументов\n");
        return 1;
    }

    fd = open(DEBUGFS_PATH, O_RDWR);
    if (fd < 0) {
        perror("Ошибка открытия файла интерфейса");
        return 1;
    }

    if (atoi(argv[1]))
        sprintf(buffer, "%s %ld %s", argv[1], strlen(argv[2]), argv[2]);
    else
        sprintf(buffer, "%d %d", atoi(argv[1]), atoi(argv[2]));
    ssize_t bytesWrite = write(fd, buffer, strlen(buffer));
    if (bytesWrite < 0) {
        perror("Ошибка записи запроса модулю ядра");
        close(fd);
        return 1;
    }
    close(fd);

    fd = open(DEBUGFS_PATH, O_RDWR);
    ssize_t bytesRead = read(fd, buffer, BUFFER_SIZE);

    if (bytesRead < 0) {
        perror("Ошибка чтения ответа от модуля ядра");
        close(fd);
        return 1;
    }
    
    printf("Результат:\n%s\n", buffer);
    close(fd);
    
    return 0;
}