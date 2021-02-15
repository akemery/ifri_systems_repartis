#include <string.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

#include "message.h"

int csd;
int sd;
static int do_create(struct message *m1, struct message *m2);
static int do_read(struct message *m1, struct message *m2);
static int do_write(struct message *m1, struct message *m2);
static int do_delete(struct message *m1, struct message *m2);

static int initialize(void);
static int release(void);


int main(int argc, char *argv[]){
   struct message m1, m2; /*incomming and outgoing message*/
   int r;   /* result code*/
   memset(&m1,0, sizeof(m1)); 
   memset(&m2,0, sizeof(m2));              
   initialize();
   /*fprintf(stderr,"%ld:%ld:%ld:%ld:%ld:%ld:%ld:%s:%s",m1.source, m1.dest, m1.opcode,
            m1.count, m1.offset, m1.result,m1.name_len, m1.name, m1.data);
   for(int i = 0; i< m1.count; i++)
     fprintf(stderr, "%c ",m1.data[i]);*/
   
   while(TRUE){           /*server runs forever*/
     //ifri_receive(FILE_SERVER, &m1); /* block waiting for a message*/
     ifri_receive(csd, &m1);
     switch(m1.opcode){
        /*case CREATE: 
          r = do_create(&m1, &m2);
          break;*/
        case READ:
          r = do_read(&m1, &m2);
          break;
        case WRITE:
          r = do_write(&m1, &m2);
          break;
        /*case DELETE:
          r = do_delete(&m1, &m2);
          break;*/
        default:
          r = E_BAD_OPCODE;
     }
     m2.result = r;  /* return result to client */
     ifri_send(m1.source, &m2); /* send reply*/
  }
  release();
}

static int do_create(struct message *m1, struct message *m2){
  return OK;
}

static int do_read(struct message *m1, struct message *m2){
  int fd, r;
  if((fd = open(m1->name, O_RDONLY))<0){
    return E_IO;
  }
  lseek(fd, m1->offset, SEEK_SET);
  r = read(fd, m2->data, m1->count);
  fprintf(stderr, "from read %d\n", r);
  if(r < 0)
     return  E_IO;
  return r;
}

static int do_write(struct message *m1, struct message *m2){
  int fd, r;
  if((fd = open(m1->name, O_WRONLY)!=0)){
    return E_IO;
  }
  lseek(fd, m1->offset, SEEK_SET);
  r = write(fd, m2->data, m1->count);
  if(r < 0)
     return  E_IO;
  return r;
}

static int do_delete(struct message *m1, struct message *m2){
  return OK;
}


static int initialize(void){
   struct sockaddr server_addr, client_addr;
   int salen, clen;
   sd = socket(AF_INET, SOCK_STREAM, 0);
   if(resolve_address(&server_addr, &salen, SERVER_ADDR, SERVER_PORT, AF_INET, SOCK_STREAM, IPPROTO_TCP)!= 0){
      fprintf(stderr, "Erreur de configuration de sockaddr\n");
      return -1;
   }
    if(bind(sd, &server_addr, salen)!=0){
      fprintf(stderr, "Un petit problème lors du bind: %d\n", errno);
      return -1;
    }
    if(listen(sd, 10)!=0){
      fprintf(stderr, "Un petit problème lors du listen %d\n", errno);
      return -1;
    }
    if((csd = accept(sd, &client_addr, &clen)) < 0){
      fprintf(stderr, "Un petit problème lors du accept %d\n", errno);
      return -1;
    }
    fprintf(stderr, "Connexion acceptée %d \n", csd);   
}

static int release(void){
  return (close(sd) || close(csd));
}
