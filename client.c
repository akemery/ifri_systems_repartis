#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netdb.h>
#include "message.h"


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
    ifri_send(FILE_SERVER, &m1);
    
    ifri_receive(client, &m1);
    
    /*write the data jsut received to the destination file*/
    
    m1.opcode = WRITE;
    m1.offset = position;
    m1.count  = m1.result;
    strcpy(m1.name, dst);
    ifri_send(FILE_SERVER, &m1);
    ifri_receive(client, &m1);
    position+=m1.result;
  } while(m1.result > 0);
  return(m1.result >= 0 ? OK: m1.result);
}

static int initialize(void){
  struct sockaddr server_addr;
  int sd = socket(AF_INET, SOCK_STREAM, 0);
  int salen, err;
  if(resolve_address(&server_addr, &salen, "127.0.0.1", "4443", AF_INET, SOCK_STREAM, IPPROTO_TCP)!= 0){
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
  initialize();
  return 0;
}
