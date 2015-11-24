#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 

#define MSGSZ     128


typedef struct msgbuf
{
  long mtype;
  char mtext[MSGSZ];
} message_buf;



void send_message()
{
  int msqid;
  int msgflg = IPC_CREAT | 0666;
  key_t key = 1234;
  message_buf sbuf;
  size_t buf_length;
  
  while(1)
  {
    char msg[20];
    scanf("%s", msg);  
    
    printf("msgget: Calling msgget(%#lx,%#o)\n", key, msgflg);
    if( (msqid = msgget(key, msgflg ) ) < 0)
    {
      perror("msgget");
      exit(1);
    }
    else
    {
      printf("msgget: msgget succeeded: msqid = %d\n", msqid);
    }
    
    sbuf.mtype = 1;
    
    strcpy(sbuf.mtext, msg);
    buf_length = strlen(sbuf.mtext) + 1;
    
    if(msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0)
    {
      printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
      perror("msgsnd");
      exit(1);
    }
    else printf("Sent message: %s\n", sbuf.mtext);
  }
  
  exit(0);
}



void rec_message()
{
  int msqid;
  key_t key = 1234;
  message_buf  rbuf;
  
  while(1)
  {
    if( (msqid = msgget(key, 0666)) < 0)
    {
     perror("msgget");
      exit(1);
    }

    if(msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0)
    {
      perror("msgrcv");
      exit(1);
    }

    printf("Received message: %s\n", rbuf.mtext);
  }
  
  exit(0);
}

void main(void)
{
  pid_t  pid;

  pid = fork();
  if(pid == 0) 
    send_message();
  else 
    rec_message();
    
  exit(0);
}

