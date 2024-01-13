#include "chat_common.h"

int should_exit = 0;

struct myshm *shm_client, *shm_server;

void *reader(void* arg){
    while(!should_exit){
        sem_wait(&(shm_client->sem));
        /* Si il y a eu au moins une écriture */
        if(shm_client->read != shm_client->write || shm_client->nb == MAX_MESS){
            /* On récupère le premier message et on incrémente le nombre lu */
            struct message message = shm_client->messages[shm_client->read];
            shm_client->read = (shm_client->read + 1) % MAX_MESS;
            printf("%s", message.content);
            shm_client->nb--;
        }
        sem_post(&(shm_client->sem));
    }
    pthread_exit(NULL);
    return NULL;
}

void *writer(void* arg){
    while(!should_exit){
        struct message msg;
        msg.type = 1;
        fgets(msg.content, TAILLE_MESS, stdin);
        /* Evite d’envoyer un message avec Ctrl-C dedans */
        if(should_exit) break;
        sem_wait(&(shm_server->sem));
        while(shm_server->nb == MAX_MESS){
            sem_post(&(shm_server->sem));
            sleep(1);
            sem_wait(&(shm_server->sem));
        }
        shm_server->messages[shm_server->write] = msg;
        shm_server->write = (shm_server->write + 1) % MAX_MESS;
        shm_server->nb++;
        sem_post(&(shm_server->sem));
    }
    pthread_exit(NULL);
    return NULL;
}

void exithandler(int sig){
    should_exit = sig; /* i.e. !=0 */
}

int main(int argc, char *argv[]){
    char *shm_client_name, *shm_server_name;
    int shm_client_id, shm_server_id;
    pthread_t tids[2];
    struct message msg;
    struct sigaction action;
    if (argc <= 2) {
        fprintf(stderr, "Usage: %s id_client id_server\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("Connexion à %s sous l'identité %s\n", argv[2], argv[1]);
    /* Met un handler pour arrêter le programme de façon clean avec Ctrl-C */
    action.sa_handler = exithandler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, 0);
    shm_client_name = (argv[1]);
    shm_server_name = (argv[2]);
    /* Crée le segment en lecture écriture */
    if ((shm_client_id = shm_open(shm_client_name, O_RDWR | O_CREAT, 0666)) == -1) {
        perror("shm_open shm_client");
        exit(errno);
    }
    if ((shm_server_id = shm_open(shm_server_name, O_RDWR | O_CREAT, 0666)) == -1) {
        perror("shm_open shm_server");
        exit(errno);
    }
    /* Allouer au segment une taille de NB_MESS messages */
    if(ftruncate(shm_client_id, sizeof(struct myshm)) == -1) {
        perror("ftruncate shm_client");
        exit(errno);
    }
    /* Mappe le segment en read-write partagé */
    if((shm_server = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED,
    shm_server_id, 0)) == MAP_FAILED){
        perror("mmap shm_server");
        exit(errno);
    }
    if((shm_client = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED,
    shm_client_id, 0)) == MAP_FAILED){
        perror("mmap shm_server");
        exit(errno);
    }
    shm_client->read = 0;
    shm_client->write = 0;
    shm_client->nb = 0;
    /* Initialisation du sémaphore (mutex) */
    if(sem_init(&(shm_client->sem), 1, 1) == -1){
        perror("sem_init shm_client");
        exit(errno);
    }
    msg.type = 0;
    strcpy(msg.content, argv[1]);
    sem_wait(&(shm_server->sem));
    shm_server->messages[shm_server->write] = msg;
    shm_server->write = (shm_server->write + 1) % MAX_MESS;
    shm_server->nb++;
    sem_post(&(shm_server->sem));
    /* On crée les threads et on attend qu’ils se terminent */
    pthread_create(&(tids[0]), NULL, reader, NULL);
    pthread_create(&(tids[1]), NULL, writer, NULL);
    pthread_join(tids[0], NULL);
    pthread_join(tids[1], NULL);
    printf("Déconnexion...\n");
    msg.type = 2;
    sem_wait(&(shm_server->sem));
    shm_server->messages[shm_server->write] = msg;
    shm_server->write = (shm_server->write + 1) % MAX_MESS;
    shm_server->nb++;
    sem_post(&(shm_server->sem));
    sem_close(&(shm_client->sem));
    munmap(shm_client, sizeof(struct myshm));
    shm_unlink(shm_client_name);
    return EXIT_SUCCESS;
}