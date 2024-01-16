#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define DEBUGFS_PATH "/sys/kernel/debug/lab/memblock_dentry_info"

int main() {
    int fd;
    char buffer[BUFFER_SIZE];
    
    // Открываем файловый дескриптор для интерфейса модуля ядра
    fd = open(DEBUGFS_PATH, O_RDWR);
    if (fd < 0) {
        perror("Ошибка открытия файла интерфейса");
        return 1;
    }
    
    // Отправляем запрос на модуль ядра
    const char *request = "memblock_type";
    write(fd, request, strlen(request));
    
    // Читаем ответ от модуля ядра
    ssize_t bytesRead = read(fd, buffer, BUFFER_SIZE);
    if (bytesRead < 0) {
        perror("Ошибка чтения ответа от модуля ядра");
        close(fd);
        return 1;
    }
    
    // Выводим результат на уровень пользователя
    printf("Результат: %s\n", buffer);
    
    // Закрываем файловый дескриптор
    close(fd);
    
    return 0;
}