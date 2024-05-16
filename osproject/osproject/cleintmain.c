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
#include<stdbool.h>

struct product {
    int id;
    char pname[50];
    int quantity;
    int price;
};
int main()
{
    struct sockaddr_in serv,cli;
    int sd = socket(AF_UNIX,SOCK_STREAM,0);
    serv.sin_family = AF_UNIX;
    serv.sin_addr.s_addr= INADDR_ANY;
    serv.sin_port = htons(7006);
    
    if(connect(sd,(const struct sockaddr *)(&serv),sizeof(serv))==-1)
    {
        perror("connection failed");
    }
    initial:
    printf("want to see all the products, type 1\n");
    printf("want to add items to the cart ? type 2\n");
    int choice;scanf("%d",&choice);
    char buf[50];
    if(choice==1)
    {
        char msg[] = "Display";
        write(sd,msg,8);
        while(1)
        {
            struct product pr;
            read(sd,&pr,sizeof(pr));
            if(pr.id != -1)
            {
                printf("message from server: %d %s %d %d\n",pr.id,pr.pname,pr.quantity,pr.price);
            }
            else break;
        }
        goto initial;
    }
    else if(choice==2)
    {
       
        char buf10[50],buf11[50];
         buy:
        write(sd,"Add to cart",sizeof("Add to cart"));
        read(sd,buf11,sizeof(buf11));
        printf("message from server: %s\n",buf11);
        write(sd,"Ok",sizeof("Ok"));
        while(1){
        read(sd,buf,50);
        printf("Message from server: %s ",buf);
        int p,q;
        scanf("%d",&p);
        write(sd,&p,4);
        scanf("%d",&q);
       write(sd,&q,4);
       if(p==-1) break;
       read(sd,buf10,sizeof(buf10));
       printf("Message from server : %s \n",buf10);
       write(sd,"Ok",sizeof("Ok"));
        }

        printf("want to see you cart ? type 3\n");
        scanf("%d",&choice);
        if(choice==3)
        {
            char ubuf1[100],ubuf2[100],ubuf3[100];
            write(sd,"Display cart",sizeof("Display cart"));
            while(1)
            {
                struct product prn;
                read(sd,&prn,sizeof(prn));
                if(prn.id!=-1)
                printf("message from server: %d %s %d %d\n",prn.id,prn.pname,prn.quantity,prn.price);
                else
                break;
            }
        }
        else
        {
            write(sd,"No",sizeof("No"));
        }
        printf("want to update your cart  ? type 4\n");
        scanf("%d",&choice);
        if(choice==4)
        {
            write(sd,"Update",sizeof("Update"));  
            goto buy;

        }
        else
        write(sd,"No",sizeof("No"));
    
        char pmsg[50];
        read(sd,pmsg,sizeof(pmsg));
        printf("message from server: %s\n",pmsg);
        if(strcmp(pmsg,"Cart is empty..")==0)
        exit(1);
        write(sd,"Ok",sizeof("Ok"));
        int tc;int amt=10;
        char buf1[50],buf2[50],buf3[50];
        read(sd,buf1,sizeof(buf));
        printf("message from the server: %s\n",buf1);
        write(sd,"Ok",sizeof("Ok"));
        read(sd,&tc,sizeof(tc));
        printf("message froM the server: %d\n",tc);
        printf("enter the required amount:\n");
        scanf("%d",&amt);
        write(sd,&amt,4);

       
       read(sd,buf2,sizeof(buf2));
       printf("message from the server: %s\n",buf2);
       write(sd,"Bye",sizeof("Bye"));
       if(strcmp(buf2,"fail")==0)
       exit(1);
       char chmsg[50];
       read(sd,chmsg,sizeof(chmsg));
       printf("message from server: %s\n",chmsg);
       write(sd,"Ok",sizeof("Ok"));
       int change;
       read(sd,&change,sizeof(change));
       printf("message from server: %d\n",change);
       write(sd,"Ok",sizeof("Ok"));
       char rmsg[50];
       read(sd,rmsg,sizeof(rmsg));
       printf("msg from the server : %s\n",rmsg);
       char cmsg[50];
       scanf("%s",cmsg);
       write(sd,cmsg,sizeof(cmsg));
       if(strcmp(cmsg,"y")==0)
       {
          while(1)
          {
            struct product ppro;
            read(sd,&ppro,sizeof(ppro));
            if(ppro.id!=-1)
            printf("message from server : %d %s %d %d\n",ppro.id,ppro.pname,ppro.quantity,ppro.price);
            else
            break;
          }
          char lsmsg[50];
          read(sd,lsmsg,sizeof(lsmsg));
          printf("message from server: %s\n",lsmsg);
          write(sd,"Ok",sizeof("Ok"));
          int final_amt;
          read(sd,&final_amt,sizeof(final_amt));
          printf("message from server: %d\n",final_amt);
      }
    }
        
}