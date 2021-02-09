#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netdb.h>
#include "message.h"

int sd;
static int copy(char *src, char *dst);
static int initialize(void);

static int copy(char *src, char *dst){
  struct message m1;
  long position;
  long client = 110;
  
  
  position = 0;
  do{
    m1.opcode = READ;
    m1.offset = position;
    m1.count = BUF_SIZE;
    strcpy(m1.name, src);
    ifri_send(sd, &m1);
    
    ifri_receive(client, &m1);
    
    /*write the data jsut received to the destination file*/
    
    m1.opcode = WRITE;
    m1.offset = position;
    m1.count  = m1.result;
    strcpy(m1.name, dst);
    ifri_send(sd, &m1);
    ifri_receive(client, &m1);
    position+=m1.result;
  } while(m1.result > 0);
  return(m1.result >= 0 ? OK: m1.result);
}

static int initialize(void){
  struct sockaddr server_addr;
  sd = socket(AF_INET, SOCK_STREAM, 0);
  int salen, err;
  if(resolve_address(&server_addr, &salen, SERVER_ADDR, SERVER_PORT, AF_INET, SOCK_STREAM, IPPROTO_TCP)!= 0){
      fprintf(stderr, "Erreur de configuration de sockaddr\n");
      return -1;
   }
   if((err = connect(sd, &server_addr, salen))!=0){
     fprintf(stderr, "Erreur de connection au server %d\n", errno);
     return -1;
   }
   fprintf(stderr, "Connexion reussi au serveur \n");
   return OK;
}

int main(int argc, char * argv){
  struct message m1;
  char data[BUF_SIZE];
  char *name = "IFRI_GL2_SIRI1";
  initialize();
  m1.source = 120;     /* sender identity*/
  m1.dest   = 220;       /* receiver's identity */
  m1.opcode = READ;     /* requested operation */
  m1.count  = BUF_SIZE;      /* number of bytes to transfert */
  m1.offset = 0;    /* position if the file to start I/O*/
  m1.result = 2;     /* result of the operation */
  m1.name_len = strlen(name);   /* name len */
  memcpy(m1.name, name, strlen(name));  /*name of the file being operated on */
  for(int i = 0; i < BUF_SIZE; i++)
    data[i] = i;   /* data to be read or written*/
  memcpy(m1.data, data, m1.count);
  ifri_send(sd, &m1);
  return 0;
}
