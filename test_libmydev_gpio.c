#include "libmydev_gpio.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

int main()
{
    int fd = 0, ret = 0;
    char buff[10] = { 0, };
    unsigned char gpio_info[2] = { 0, };

    while (1)
    {
        printf("                       \n");
        printf("=======================\n");
        printf(" 1. Set GPIO Direction \n");
        printf(" 2. Set GPIO Value     \n");
        printf(" 3. Get GPIO Value     \n");
        printf(" q. Exit               \n");
        printf("=======================\n");
        printf("\nInput> ");

        fgets(buff, sizeof(buff), stdin);

        if (buff[0] == 'q')
            break;
        else if (buff[0] < '1' || buff[0] > '3')
            continue;

        switch (buff[0])
        {
            case '1':
                printf("Input GPIO Number: ");
                scanf("%d", &gpio_info[0]);
                printf("Input Direction (0:IN, 1:OUT): ");
                scanf("%d", &gpio_info[1]);
                ret = MYDEV_GPIO_SetDirection(gpio_info[0], gpio_info[1]);
                break;
            case '2':
                printf("Input GPIO Number: ");
                scanf("%d", &gpio_info[0]);
                printf("Input Value (0:IN, 1:OUT): ");
                scanf("%d", &gpio_info[1]);
                ret = MYDEV_GPIO_SetValue(gpio_info[0], gpio_info[1]);
                break;
            case '3':
                printf("Input GPIO Number: ");
                scanf("%d", &gpio_info[0]);
                ret = MYDEV_GPIO_GetValue(gpio_info[0], &gpio_info[1]);
                printf("Value: %d\n", gpio_info[1]);
                break;
            default:
                break;
        }
        getchar();
    }
    close(fd);
    return 0;
}
