#include<stdio.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>
struct product {
    int id;
    char pname[50];
    int quantity;
    int price;
};
int main()
{

    printf("Hello Admin!!\n");
    printf("want to access the products ? type y or n");
    char ch;scanf("%c",&ch);
    if(ch=='y')
    {
            int fd= open("data.txt",O_RDWR);
            struct product p;
            lseek(fd,0,SEEK_SET);
            while (read(fd,&p,sizeof(struct product)))  
            {
                    printf("%d %d %d\n",p.id,p.quantity,p.price);;
            }
            
        
        while(1)
        {
            printf("type 1 to ADD a product\n");
            printf("type 2 to DEL a product\n");            
            printf("type 3 to UPDATE the quantity or price of a product\n");
            int choice ; scanf("%d",&choice);
            if(choice == 1)
            {   
                struct product newp;
                printf("Enter the new product id :");
                int pd ; scanf("%d",&(newp.id));
                printf("Enter it's initial quantity: ");
                int qn ; scanf("%d",&(newp.quantity));
                printf("Enter the price of product:");
                int pr; scanf("%d",&(newp.price));
                char prnam[50];printf("Enter the product name:");scanf("%s",newp.pname);
                struct product delp;
                lseek(fd,0,SEEK_SET);int v=0;
                lseek(fd,0,SEEK_SET);
                {
                lseek(fd,0,SEEK_END);  
                write(fd,&(newp),sizeof(struct product));}  
            
            }
            else if(choice==2)
            {
                
                printf("Enter the id of the product you want to delete:");
                int did;scanf("%d",&did);
                struct product delp;
                int t=-1;char tc[50];strcpy(tc,"NULL");
                lseek(fd,0,SEEK_SET);
                while(read(fd,&delp,sizeof(struct product)))
                {
                    if(delp.id==did)
                    {
                        int loc=lseek(fd,-1*(sizeof(struct product)),SEEK_CUR);
                        struct flock fl;
                        fl.l_type = F_WRLCK;
                        fl.l_whence = 0;
                        fl.l_start=loc;
                        fl.l_len = sizeof(struct product);
                        fl.l_pid = getpid(); 
                        printf("Before locking..\n");
                        fcntl(fd,F_SETLKW,&fl);
                        printf("Are you sure to delete this product ? say y or n\n");
                        char opt[10];scanf("%s",opt);
                        if(strcmp(opt,"y")==0)
                        {
                        lseek(fd,loc,SEEK_SET);
                        write(fd,&t,4);
                        char del[50];
                        strcpy(del,"");
                        write(fd,del,sizeof(del));
                        write(fd,&t,4);
                        write(fd,&t,4);
                        lseek(fd,0,SEEK_SET);}
                        lseek(fd,-1*(sizeof(struct product)),SEEK_CUR);
                        fl.l_type = F_UNLCK;
                        fcntl(fd,F_SETLK,&fl);

                        break;
                    }
                }
                    
                
            }
            else if(choice==3)
            {
                printf("Enter the id of the product you want to update:");
                int uid ; scanf("%d",&uid);
                
                struct product uppro;
                lseek(fd,0,SEEK_SET);
                while(read(fd,&uppro,sizeof(struct product)))
                {

                    if(uppro.id==uid)
                    {
                        int loc=lseek(fd,-1*(sizeof(struct product)),SEEK_CUR);
                        struct flock fl;
                        fl.l_type = F_WRLCK;
                        fl.l_whence = SEEK_CUR;
                        fl.l_start=0;
                        fl.l_len = sizeof(struct product);
                        fl.l_pid = getpid(); 
                        printf("Before locking..\n");
                        printf("%d",loc);
                        int newpr,newqn;
                        fcntl(fd,F_SETLKW,&fl);
                        printf("Enter the new price:");scanf("%d",&newpr);
                        printf("Enter the new quantity:");scanf("%d",&newqn);
                        lseek(fd,sizeof(struct product),SEEK_CUR);
                        lseek(fd,-8,SEEK_CUR);
                        write(fd,&newqn,4);
                        write(fd,&newpr,4);
                        lseek(fd,-1*(sizeof(struct product)),SEEK_CUR);
                        fl.l_type = F_UNLCK;
                        fcntl(fd,F_SETLK,&fl);
                        
                        break;
                    }
                }
            }
            else {break;}
        }
        
        close(fd);

    }
           int fd= open("data.txt",O_RDWR);
            lseek(fd,0,SEEK_SET);
            struct product p;
            while (read(fd,&p,sizeof(struct product)))  
                {
                    printf("%d %d %d\n",p.id,p.quantity,p.price);;
                }

}