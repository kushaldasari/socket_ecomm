
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include<string.h>


struct product 
{
    int id;
    char pname[50];
    int quantity;
    int price;
};
int main()
{
    struct product p1,p2,p3;
    p1.id=1;
    p1.quantity = 10;
    p1.price = 15;
    strcpy(p1.pname,"Apple");
    p2.id = 2;
    p2.quantity = 20;
    p2.price = 13;
    strcpy(p2.pname,"Orange");
    p3.id = 3;
    p3.quantity  = 25;
    p3.price = 20;
    strcpy(p3.pname,"Mangoe");
    int fd = open("data.txt",O_RDWR);
    write(fd,&p1,sizeof(struct product));
    write(fd,&p2,sizeof(struct product));
    write(fd,&p3,sizeof(struct product));
    close(fd);
    int fd1= open("data.txt",O_RDWR);
            struct product p;
            //lseek(fd,0,SEEK_SET);
            lseek(fd1,0,SEEK_SET);
    while (read(fd1,&p,sizeof(struct product)))  
                {
                    printf("hi");
                    printf("%d %ss %d %d\n",p.id,p.pname,p.quantity,p.price);;
                }
}