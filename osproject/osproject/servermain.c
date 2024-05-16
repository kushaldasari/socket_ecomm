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
#include<sys/sem.h>
#include<stdbool.h>

int v=0;
bool var = false;
struct product {
    int id;
    char pname[50];
    int quantity;
    int price;
};
 int cart[100],quan[100],cost[100];int ind=0;
     char pnm[100][50];
int buy(int p,int q,int fd)
{      
        struct product pr;
        int loc=-1;
        lseek(fd,0,SEEK_SET);
     while((read(fd,&pr,sizeof(struct product))))
        {
            if(pr.id==p)
            {
                loc = lseek(fd,-1*(sizeof(struct product)),SEEK_CUR);
                break;
            }
        }
        if(loc!=-1){
        struct flock fl;
        fl.l_type = F_RDLCK;
        fl.l_whence = 0;
        fl.l_start = loc;
        fl.l_len = sizeof(struct product);
        fl.l_pid = getpid(); 
        printf("Before locking..\n");
        fcntl(fd,F_SETLKW,&fl);
        printf("Entered the critical section in buying...");
        printf("%d\n",loc);
        lseek(fd,loc,SEEK_SET);
        read(fd,&pr,sizeof(struct product));
        for(int i=0;i<ind;i++)
        {
            if(cart[i]==p){
            v=i;
            var=true;}
        }
        if(var==true)
        {
            quan[v]=q;
            cost[v]=q*(pr.price);
            if(pr.quantity < quan[v] && pr.quantity!=-1)
            {
            quan[v] = pr.quantity;
            cost[v] = quan[v]*pr.price;
            }
            else if(pr.quantity==-1)
            {
              quan[v]=0;
              cost[v]=0;
            }
            fl.l_type = F_UNLCK;
            fcntl(fd,F_SETLK,&fl);
        }
        else {
        cart[ind]=p;
        quan[ind]=q;
        cost[ind]=q*(pr.price);
        if(pr.quantity < quan[ind]&&pr.quantity!=-1)
        {
            quan[ind] = pr.quantity;
            cost[ind] = quan[ind]*pr.price;
        }
        else if(pr.quantity==-1)
        {
          quan[ind]=0;
          cost[ind]=0;
        }
        strcpy(pnm[ind],pr.pname);
        fl.l_type = F_UNLCK;
        fcntl(fd,F_SETLK,&fl);
        }}
        return loc;

}
int main()
{
    int loca[100];
      int p,q;
    int fd = open("data.txt",O_RDONLY);int fd3;
    for(int i=0;i<100;i++)
    {
        cart[i]=0;
        quan[i]=0;
        cost[i]=0;
        strcpy(pnm[i],"");
    }
   
    struct sockaddr_in serv,cli;
    int sd = socket(AF_UNIX,SOCK_STREAM,0);
    serv.sin_family = AF_UNIX;
    serv.sin_addr.s_addr= INADDR_ANY;
    serv.sin_port = htons(7006);
    char buf[50];
    bind(sd,(const struct sockaddr*)(&serv),sizeof(serv));
    listen(sd,5);
    int sz = sizeof(cli);
    
    while(1){
    int nsd = accept(sd,(struct sockaddr*)(&cli),(socklen_t *)&sz);
    if(!fork()){
    close(sd);
    int c;
     initial:
    read(nsd,buf,sizeof(buf));
    printf("message from client: %s\n",buf);
    
    if(strcmp(buf,"Display")==0)
    {
        printf("gp");
        int fd = open("data.txt",O_RDONLY);
        struct flock fl;
        fl.l_type = F_RDLCK;
        fl.l_whence = 0;
        fl.l_start=0;
        fl.l_len = 0;
        fl.l_pid = getpid(); 
        fcntl(fd,F_SETLKW,&fl);
        struct product pr;
        printf("%s %s %s %s\n","product id ","product name ","product quantity ","product price ");
        while(read(fd,&pr,sizeof(struct product)))
        {
            if(pr.quantity>=0)
            {
                write(nsd,&pr,sizeof(pr));
            }
        }
        struct product endp;
        endp.id = -1;strcpy(endp.pname,"");endp.quantity = -1;endp.price=-1;
        write(nsd,&endp,sizeof(endp));
        fl.l_type= F_UNLCK;
        fcntl(fd,F_SETLK,&fl);
        goto initial;
    }
    else if(strcmp(buf,"Add to cart")==0)
    {
  
      char buf10[10];
      buy:
      write(nsd,"press -1 to stop adding/updating to the cart",sizeof("press -1 to stop adding/updating to the cart"));
      read(nsd,buf10,sizeof(buf10));
      while(1){
      write(nsd,"Enter the product id and quantity:",35);
      read(nsd,&p,4);
      printf("message from client(product id) : %d\n", p);
      read(nsd,&q,4);
      printf("message from client(quantity): %d\n",q );
      if(p==-1) break;
      int location=buy(p,q,fd);
      if(var==false){
      loca[ind]=location;
      ind++;}int x;
      var=false;
      for(int i=0;i<ind;i++)
      {
          if(cart[i]==p)
          {    
            x=i;
          }
      }
      if(quan[x]==0&&location!=-1&&q!=0)
      {
        write(nsd,"Sorry that product in not in stock!",sizeof("Sorry that product in not in stock!"));
      }
      else if(quan[x]<q&&location!=-1)
      {
        write(nsd,"Sorry fewer items are only left..",sizeof("Sorry fewer items are only left.."));
      }
      else if(location==-1)
      {
        write(nsd,"please enter valid product id..",sizeof("please enter valid product id.."));
        ind--;
      }
      else {
        write(nsd,"continue",sizeof("continue"));
      }
      char omsg[50];read(nsd,omsg,sizeof(omsg));
      printf("message from cleint: %s\n",omsg);
      }  

        char buf5[50];
        read(nsd,buf5,sizeof(buf5));
        printf("message from client : %s \n",buf5);
        if(strcmp(buf5,"Display cart")==0)
        {
            printf("id pname quantity amount\n");
            struct product prn;
            int fdn = open("data.txt",O_RDONLY);
           for(int i=0;i<ind;i++)
           { 
              lseek(fdn,loca[i],SEEK_SET);    
              read(fdn,&prn,sizeof(prn));
              if(prn.quantity<quan[i]&&prn.quantity!=-1)
              {
                 quan[i] = prn.quantity;
              }
              else if(prn.quantity==-1)
              {
                quan[i]=0;
              }
              cost[i]=prn.price * quan[i];    
              prn.id=cart[i];
              strcpy(prn.pname,pnm[i]);  
              prn.quantity = quan[i];
              prn.price = cost[i];
              if(quan[i]>0)
              write(nsd,&prn,sizeof(prn));        
           }
           prn.id = -1;prn.quantity = -1;strcpy(prn.pname,"");prn.price=-1;
           write(nsd,&prn,sizeof(prn));
         }
         char buf6[50];
         read(nsd,buf6,sizeof(buf6));
         printf("message from client: %s\n",buf6);
         if(strcmp(buf6,"Update")==0)
         {
            goto buy;
         }

            int count=0;
            for(int i=0;i<ind;i++){
              if(quan[i]>0) count++;
            }
            if(count>0){   
            char omsg[50];
            write(nsd,"Entering into payment section",sizeof("Entering into payment section"));
            read(nsd,omsg,sizeof(omsg));
            printf("message from server: %s\n",omsg); 
            char buf1[50];
            struct flock fl1;
            int fd1 = open("data.txt",O_RDWR);
            printf("Before entering intooo the payment..\n");
            for(int i=0;i<ind;i++){
            fl1.l_type = F_WRLCK;
            fl1.l_whence = 0;
            fl1.l_start = loca[i];
            fl1.l_len = sizeof(struct product);
            fl1.l_pid = getpid(); 
            fcntl(fd1,F_SETLKW,&fl1);
            }
            printf("entered critical section in payment....\n");
            for(int i=0;i<ind;i++){
            lseek(fd1,loca[i],SEEK_SET);
            struct product pro;read(fd1,&pro,sizeof(struct product));
            int qu = pro.quantity;
            if(qu<quan[i]&&qu!=-1)
            {
                quan[i]=qu;
            }
            else if(qu==-1)
            {
              quan[i]=0;
            }
            cost[i] = quan[i] * pro.price;
            }

            int totcost=0,final_amt;
            for(int i=0;i<ind;i++) totcost+=cost[i];
        
             int amt;
             write(nsd,"Total amount to be paid:",sizeof("Total amount to be paid:"));
             read(nsd,&buf1,sizeof(buf1));
             printf("msg from client: %s\n",buf1);
             final_amt=totcost;
             write(nsd,&totcost,sizeof(totcost));
          
            read(nsd,&amt,sizeof(amt));
            printf("message from client: %d\n",amt);
            if(amt>=totcost){
            for(int i=0;i<ind;i++){
            lseek(fd1,loca[i],SEEK_SET);
            struct product pro;read(fd1,&pro,sizeof(struct product));
            int qu = pro.quantity;
            lseek(fd1,loca[i],SEEK_SET);
            pro.quantity = pro.quantity - quan[i];
            write(fd1,&pro,sizeof(struct product));}
            printf("paymeny section unlocked\n");
            
            for(int i=0;i<ind;i++){
            fl1.l_type = F_UNLCK;
            fl1.l_whence = 0;
            fl1.l_start = loca[i];
            fl1.l_len = sizeof(struct product);
            fcntl(fd1,F_SETLK,&fl1);
            printf("Uncloked..\n");
            }
            write(nsd,"success",sizeof("success"));
            char lmsg[50];
            read(nsd,lmsg,sizeof(lmsg));
            printf("message from client : %s\n",lmsg);
            write(nsd,"Returning amount:",sizeof("Returning amount:"));
            char tmsg[50];
            read(nsd,tmsg,sizeof(tmsg));
            printf("message from server: %s\n",tmsg);
            int change = amt-totcost;
            write(nsd,&change,sizeof(change));
            read(nsd,tmsg,sizeof(tmsg));
            printf("message from server: %s\n",tmsg);
            write(nsd,"Do you want receipt ? say y or n",sizeof("Do you want receipt ? say y or n"));
            char cmsg[50];
            read(nsd,cmsg,sizeof(cmsg));
            printf("message from the client : %s\n",cmsg);
            if(strcmp(cmsg,"y")==0)
            {
            int ldn = open("logfl.txt",O_RDWR | O_CREAT | O_TRUNC,0644);
            int newind=0;
            for(int i=0;i<ind;i++)
            {
              if(quan[i]>0){
              write(ldn,&cart[i],sizeof(cart[i]));
              write(ldn,pnm[i],sizeof(pnm[i]));
              write(ldn,&quan[i],sizeof(quan[i]));
              write(ldn,&cost[i],sizeof(cost[i]));
              newind++;
              }
            }
              struct product ppro;
              int id,qn,pr;char pnme[50];
              lseek(ldn,0,SEEK_SET);
              for(int i=0;i<newind;i++)
              {
                 read(ldn,&ppro.id,sizeof(ppro.id));
                 read(ldn,ppro.pname,sizeof(ppro.pname));
                 read(ldn,&ppro.quantity,sizeof(ppro.quantity));
                 read(ldn,&ppro.price,sizeof(ppro.price));
                write(nsd,&ppro,sizeof(ppro));
              }
              ppro.id = -1;strcpy(ppro.pname,"");ppro.quantity=-1;ppro.price=-1;
              write(nsd,&ppro,sizeof(ppro));
              write(nsd,"Total amount ",sizeof("Total amount "));
              char var[50];read(nsd,var,sizeof(var));
              printf("%s\n",var);
              write(nsd,&final_amt,sizeof(final_amt));
            }

            }
            else
            {
                for(int i=0;i<ind;i++){
                fl1.l_type = F_UNLCK;
                fl1.l_whence = 0; fl1.l_start = loca[i]; fl1.l_len = sizeof(struct product);
            fcntl(fd1,F_SETLK,&fl1);
            printf("Uncloked..\n");}
            write(nsd,"fail",sizeof("fail"));
            char lmsg[50];
            read(nsd,lmsg,sizeof(lmsg));
            printf("last message from our beloved client : %s",lmsg);
            } 
            }
            else
            {
              write(nsd,"Cart is empty..",sizeof("Cart is empty.."));
            }  
        }   
        
      exit(1);  
    }
    
    else
    {
        close(nsd);
    }
    }
    
    }
      
    
    
    