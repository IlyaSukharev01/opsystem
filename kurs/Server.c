#include <sys/types.h>
#include <sched.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int checkTurn(int a[3][3]){

    if (a[0][0] == a[1][1] && a[1][1] == a[2][2] && a[0][0] != 0){
        return a[1][1];

    }
    else if (a[0][2] == a[1][1] && a[1][1] == a[2][0] && a[0][2] != 0){
        return a[1][3];
    }
    else if (a[0][0] == a[1][0] && a[1][0] == a[2][0] && a[0][0] != 0){
        return a[1][1];
    }

    else if (a[0][1] == a[1][1] && a[1][1] == a[2][1] && a[0][1] != 0){
        return a[1][2];
    }
    else if (a[0][2] == a[1][2] && a[1][2] == a[2][2] && a[0][2] != 0){
        return a[1][3];
    }
    else if (a[2][0] == a[2][1] && a[2][1] == a[2][2] && a[2][0] != 0){
        return a[3][1];

    }
    else if (a[1][0] == a[1][1] && a[1][1] == a[1][2] && a[1][0] != 0){
        return a[2][1];

    }
    else if (a[0][0] == a[0][1] && a[0][1] == a[0][2] && a[0][0] != 0){
        return a[1][1];

    }
    else{
        return 0;
    }
}


int main()
{
    int firstplayerfd, secondplayerfd, sockfd;

    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror(NULL);
        exit(1);
    }
 bzero(&servaddr, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_port = htons(9990);
 servaddr.sin_addr.s_addr = INADDR_ANY;

if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
    perror(NULL);
    close(sockfd);
    exit(1);
}
if (listen(sockfd, 5) < 0){
    perror(NULL);
    close(sockfd);
    exit(1);
}

printf("\nWaiting both players");
pid_t pid;
pid = fork();

    if( pid < 0){
        perror("\nChildren below zero");
        close(sockfd);
        exit(0);
    }
    else if(pid == 0)
    {
        while(1){
            if((firstplayerfd = accept(sockfd, 0, 0)) < 0){
            perror("Client first initializ failed\n");
            close(sockfd);
            exit(0);
            }

            if((secondplayerfd = accept(sockfd,0, 0)) < 0){
            perror("Client second initializ failed\n");
            close(sockfd);
            exit(0);
            }

            pid = fork();
            if(pid < 0){
                perror("\nChildren bellow zero st2");//step 2
                close(sockfd);
                exit(0);
            }
            else if(pid == 0)
            {
                    int a[3][3];
                    for (int i = 0; i < 3; i++){
                    for (int j = 0; j < 3; j++){
                        a[i][j] = 0;
                        }
                 }

                close(sockfd);
                printf("\nSuccess");
                int buff = 0;//0 - Стартовый выбор  ролей
                int x, y; //Элементы матрицы
                int callback_whowins; // Если у нас статус стал 0-м,то выиграл первый сокет && Если 1-ей, тогда победа за 2 сокетом
                send(firstplayerfd, &buff, sizeof(int), 0);
                buff = 0; send(firstplayerfd, &buff, sizeof(int), 0);

                buff = 0; send(secondplayerfd, &buff, sizeof(int), 0);
                buff = 1; send(secondplayerfd, &buff, sizeof(int), 0);

                recv(firstplayerfd, &buff, sizeof(int), 0);
                if(buff == 1)
                {
                    while(1)
                     {
                        recv(firstplayerfd, &x, sizeof(int), 0);
                        recv(firstplayerfd, &y, sizeof(int), 0);
                        a[x][y] = 5; // крестик
                        buff = 1;
                        send(secondplayerfd, &buff, sizeof(int), 0);
                        send(secondplayerfd, &x, sizeof(int), 0);
                        send(secondplayerfd, &y, sizeof(int), 0);
                        if (checkTurn(a) != 0){
                            callback_whowins = 0;
                            break;              //+++ закрытие сокетов?///
                        }   //Здесь должна быть проверка матрицы.

                        buff = 1;
                        recv(secondplayerfd, &buff, sizeof(buff),0);
                        recv(secondplayerfd, &x, sizeof(int), 0);
                        recv(secondplayerfd, &y, sizeof(int), 0);
                        a[x][y] = 6;//Нолик
                        send(firstplayerfd, &buff, sizeof(int), 0);
                        send(firstplayerfd, &x, sizeof(int), 0);
                        send(firstplayerfd, &y, sizeof(int), 0);
                        if (checkTurn(a) != 0){
                            callback_whowins = 1;
                            break;
                        }    //Здесь должна быть проверка матрицы.

                        }
                        buff = 2;
                        if (callback_whowins == 0){
                            send(firstplayerfd, &buff, sizeof(int), 0);
                            send(secondplayerfd, &buff, sizeof(int), 0);
                            buff = 0;
                            send(firstplayerfd, &buff, sizeof(int), 0);
                            send(secondplayerfd, &buff, sizeof(int), 0);
                        }
                        else if(callback_whowins == 1){
                            send(secondplayerfd, &buff, sizeof(int), 0);
                            send(firstplayerfd, &buff, sizeof(int), 0);
                            buff = 1;
                            send(secondplayerfd, &buff, sizeof(int), 0);
                            send(firstplayerfd, &buff, sizeof(int), 0);
                        }
                        else
                        {
                            printf("\nChek_turn() returned unavailable number");
                        }
                    }

            }
            else{
                close(firstplayerfd);
                close(secondplayerfd);
            }
        }
        close(sockfd);
    }
    else
    {
        waitpid(pid,0,0);
    }

    }

