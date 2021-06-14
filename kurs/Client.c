#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>



bool status; // Принимает роль

void Receive(int a[3][3], int sockfd){
        // Функция
    int buff, x, y;
    recv(sockfd, &buff, sizeof(int), 0);   //ход сделан// кто-то победил
    switch(buff){
        case 0: // Назначение ролей
            recv(sockfd, &status, sizeof(bool), 0);
            break;
        case 1:
            recv(sockfd, &x, sizeof(int), 0);
            recv(sockfd, &y, sizeof(int), 0);
            if(status){
                a[x][y] = 6;

            }
            else{
                a[x][y] = 5;
            }
            break;
        case 2:
            recv(sockfd, &buff, sizeof(int), 0);
            if(buff == 0){
                printf("\nFirst player win");
            }
            else{
                printf("\nSecond player win");
            }
            break;
        default:
            exit(0);

  }



}

void SendTurn(int sockfd, int x, int y){
    int buff = 1;
    write(sockfd, &buff, sizeof(int));
    write(sockfd, &x, sizeof(int));
    write(sockfd, &y, sizeof(int));
}

void Draw_map(int a[3][3])
{
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (a[i][j] == 5){
                printf("O");
            }
            else if(a[i][j] == 6){
                printf("X");
            }
            else{

            printf("%i", a[i][j]);
            }
        }
        printf("\n");
    }
}




int main(int argc, char *argv[]){
  typedef int MatrixArray[3];

  int sockfd;
  struct sockaddr_in servaddr;

 if(argc != 2){
     printf("Usage Ip-Adress");
     exit(1);
 }

  if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
      perror("Failed create socket");
      close(sockfd);
      exit(1);
  }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9990);
    if(inet_aton(argv[1], &servaddr.sin_addr) == 0){
        printf("Invalid IP adress\n");
        close(sockfd);
        exit(1);
    }
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }
    int arrayDef[3][3];
    MatrixArray *a = arrayDef;
    for (int i = 0; i < 3; i++)
    {
        for (int y = 0; y < 3; y++)
            {
                a[i][y] = 0;
            }
    }

    int x, y;
    Receive(a, sockfd);
    bool input = false;
    if (status)
    {
        printf("Ure second player\n");
        Draw_map(a);
        Receive(a, sockfd);//Первый ход получен!!!
        Draw_map(a);
        scanf("%i %i", &x, &y);//Второй ход!!!
        if(a[x][y] == 0){
            a[x][y] = 6; // крестик
        }
        else{
            while(!input){
                printf("Wrnog cell, repeat input \n");
                scanf("%d %d", &x, &y);
                if(a[x][y] == 0){
                    a[x][y] == 6;
                    input = true;
                }
            }
        }
        Draw_map(a);
        SendTurn(sockfd, x, y);//Второй ход отправлен!!!
    }
    else{
        printf("Ure first player\n");
        input = false;
        Draw_map(a);
        scanf("%i %i", &x, &y);//Второй ход!!!
        if(a[x][y] == 0)
        {
            a[x][y] = 5; // Нолик
        }
        else{
            while(!input){
                printf("Wrong cell, repeat input \n");
                scanf("%d %d", &x, &y);
                if(a[x][y] == 0){
                    a[x][y] == 5;
                    input = true;
                }
            }
        }

        SendTurn(sockfd, x, y);//Первый ход!!!!
        Draw_map(a);
        Receive(a, sockfd);
        Draw_map(a);//Нарисовали второй ход
        scanf("%i %i", &x, &y);//3 ход!!!
        if(a[x][y] == 0){
            a[x][y] = 5;
        }
        Draw_map(a);//Нарисовали 3 ход
        SendTurn(sockfd, x, y);//3 ход!!!!




    }


close(sockfd);
}

