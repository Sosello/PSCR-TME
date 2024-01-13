#include "chat_common.h"

int should_exit = 0;

void exit_handler(int sig) {
    should_exit = sig;
}

struct user {
    char *name;
    struct myshm *shm;
}*users[MAX_USERS];

int main(int argc, char *argv[]){
    char *shm_name;
    int shm_id, i;
    struct myshm *shm_pere;
    struct sigaction action;

    if(argc <= 1){
        printf("Usage: %s id_server\n", argv[0]);
        exit(EXIT_FAILURE);
    }   

    action.sa_handler = exit_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, NULL);

    shm_name = (argv[1]);

    shm_id = shm_open(shm_name, O_RDWR | O_CREAT , 0666);

    if( shm_id == -1){
        perror("shm_open shm_pere");
        exit(EXIT_FAILURE);
    }

    if(ftruncate(shm_id, sizeof(struct myshm)) == -1){
        perror("ftruncate shm_pere");
        exit(EXIT_FAILURE);
    }

    if((shm_pere = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0)) == MAP_FAILED){
        perror("mmap shm_pere");
        exit(EXIT_FAILURE);
    }

    shm_pere->read = 0;
    shm_pere->write = 0;
    shm_pere->nb = 0;

    if(sem_init(&(shm_pere->sem), 1, 1) == -1){
        perror("sem_init shm_pere");
        exit(EXIT_FAILURE);
    }   

    for(i = 0; i < MAX_USERS; i++){
        users[i] = NULL;
    }

    while(!should_exit){
        sem_wait(&(shm_pere->sem));
        if(shm_pere->read != shm_pere->write || shm_pere->nb == MAX_MESS){
            struct message mess = shm_pere->messages[shm_pere->read];

            switch(mess.type){
                case 0:{
                    char *username;
                    int shm_id_user;
                    i = 0;

                    while(i < MAX_USERS && users[i] != NULL) i++;
                    if(i == MAX_USERS){
                        perror("Server is full");
                        exit(EXIT_FAILURE);
                    }

                    users[i] = malloc(sizeof(struct user));
                    if(users[i] == NULL){
                        perror("malloc user");
                        exit(EXIT_FAILURE);
                    }

                    users[i]->name = malloc((strlen(mess.content) + 1)*sizeof(char));
                    strcpy(users[i]->name, mess.content);
                    users[i]->name[strlen(mess.content)] = '\0';

                    printf("User %s connected\n", users[i]->name);

                    username = mess.content;
                    if(shm_id_user = shm_open(username, O_RDWR | O_CREAT , 0666) == -1){
                        perror("shm_open shm_fils");
                        exit(EXIT_FAILURE);
                    }

                    users[i]->shm = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, shm_id_user, 0);

                    if( users[i]->shm == MAP_FAILED){
                        perror("mmap shm_fils");
                        exit(EXIT_FAILURE);
                    }

                    break;
                    
                }

                case 1:{
                    int temps = 0;
                    printf("Message from server %s",  mess.content);
                    for(i=0; i<MAX_USERS; i++){
                        if(users[i] != NULL){
                            struct message msg;
                            struct myshm *shm_fils = users[i]->shm;
                            temps++;

                            msg.type = 1;
                            strcpy(msg.content, mess.content);

                            sem_wait(&(shm_fils->sem));

                            while(shm_fils->nb == MAX_MESS){
                                sem_post(&(shm_fils->sem));
                                sleep(1);
                                sem_wait(&(shm_fils->sem));
                            }

                            printf("\t\tEnvoi à %s\n", users[i]->name);

                            shm_fils->messages[shm_fils->write] = msg;
                            shm_fils->write = (shm_fils->write + 1) % MAX_MESS;
                            shm_fils->nb++;
                            sem_post(&(shm_fils->sem));
                        }
                    }

                    if(temps == 0){
                        printf("No user connected\n");
                    }
                    break;
                }

                case 2:{
                    printf("Déconnexion de %s\n", mess.content);
                    i=0;

                    while(i < MAX_USERS && (users[i] == NULL || strcmp(users[i]->name, mess.content) != 0)) i++;
                    if(i == MAX_USERS){
                        perror("User not found");
                        exit(EXIT_FAILURE);
                    }

                    free(users[i]->name);
                    munmap(users[i]->shm, sizeof(struct myshm));
                    free(users[i]);
                    users[i] = NULL;

                    break;
                }
            }
            shm_pere->read = (shm_pere->read + 1) % MAX_MESS;   
            shm_pere->nb--;
        }
        sem_post(&(shm_pere->sem));
    }
    sem_close(&(shm_pere->sem));
    munmap(shm_pere, sizeof(struct myshm));
    shm_unlink(shm_name);

    return EXIT_SUCCESS;
}