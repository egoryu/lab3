#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1000
#define DEBUGFS_PATH "/sys/kernel/debug/lab20/memblock_dentry_info"

int main(int argc, char *argv[]) {
    int fd;
    char buffer[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    
    if (argc != 3) {
        perror("Неправильное количество аргументов\n");
        return 1;
    }

    perror("kek");
    fd = open(DEBUGFS_PATH, O_RDWR);
    if (fd < 0) {
        perror("Ошибка открытия файла интерфейса");
        return 1;
    }
    perror("kek2");
    sprintf(buffer, "%s %s", argv[1], argv[2]);
    ssize_t bytesWrite = write(fd, buffer, strlen(buffer));
    if (bytesWrite < 0) {
        perror("Ошибка записи запроса модулю ядра");
        close(fd);
        return 1;
    }
    close(fd);
    fd = open(DEBUGFS_PATH, O_RDWR);
    perror("kek1");
    ssize_t bytesRead = read(fd, buffer2, BUFFER_SIZE);
    perror("kek");
    if (bytesRead < 0) {
        perror("Ошибка чтения ответа от модуля ядра");
        close(fd);
        return 1;
    }
    
    printf("Результат:\n %s\n", buffer2);
    perror("kek");
    
    close(fd);
    
    return 0;
}