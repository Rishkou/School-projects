#include <stdio.h>  //printf xD
#include <stdlib.h> // strtol, atoi
#include <sys/mman.h> // zdielana pamat
#include <semaphore.h> // semafor
#include <unistd.h> // fork
#include <sys/wait.h> // wait
#include <time.h> // cas na tvorenie intervalu cakania
#include <signal.h> // kill(), SIGTERM

typedef struct Scontainer
{
    int cart_count;
    int visit_count;
    int cart_capacity;
    int ride_duration;
    int queue_duration;
    int cart_delay;
}container;
typedef struct Sshared_data
{
    int x;
    sem_t mutex; 
    int visitor_queue;
    int cart_queue;
    sem_t board_visitor;
    sem_t leave_visitor;
    sem_t cart_start;
    sem_t FIFO[10];
    int cart_number;
    sem_t visitor_ready;
    int closed;
    sem_t cart_tranzit;
    int visitors_boarded;
    

}shared_data;

//funkcia na kontrolu vstupu, a ziskanie vstupynch hodnot
int arg_check(int argc,char* argv[],container *container);
//funckia na alokaciu zdielanej pamete
shared_data* memory_allocation(container);
// funkcia na cistenie pamete
void clean(shared_data *data,container container,FILE *output);
// funkcia na rozbehnutie procesov programu
int process_init(shared_data *data, container *container,FILE *output);
void process_clean(pid_t *process_list,int process_index);
void dispatcher(shared_data *data,container *container,FILE *output);
void visitors(int id,shared_data *data, container *container,FILE *output);
void carts(int id,shared_data *data, container *container,FILE *output);

int main(int argc,char* argv[]){
    container container;
    FILE* output = fopen("proj2.out","w");
    if(output == NULL)
        return 1;
    setbuf(output,NULL);  //vypiname file buffer 
    if( arg_check(argc,argv,&container) == -1)
    {
        fprintf(stderr,"Error: wrong input\n");
        return 1;
    }
    shared_data *data = memory_allocation(container);
    if(data == NULL)
    {
        fprintf(stderr,"Error: failed allocating memory\n");
        return 1;
    }

    if( process_init(data,&container,output)== -1)
    {
        fprintf(stderr,"Error: failed fork\n");
        clean(data,container,output);
        return 1;
    }

    clean(data,container,output);
    return 0;
}
/** 
 *@brief function that checks if user inputs were within limits, and stores them
 *@param argc amount of characters
 *@param argv[] list of inputs
 *@param container pointer to a container where the function stores inputed values
 *@return -1 if an error occurs, 0 if container was filled properly
*/
int arg_check(int argc,char* argv[],container *container)
{
    //vo vstupe ma byt 6 argumentov
    if(argc != 7)
        return -1;

    // konvertovanie argv na inty 
    //strtol na kontrlu vstupu, atoi na naslednu konverziu, aby sme mali inty
    char *endptr;
    for(int i =1;i<argc;i++)
    {
        strtol(argv[i],&endptr,10);
        if(*endptr != '\0')
            return -1;
    }
    container->cart_count = atoi(argv[1]);
    container->visit_count = atoi(argv[2]);
    container->cart_capacity = atoi(argv[3]);
    container->ride_duration = atoi(argv[4]);
    container->queue_duration = atoi(argv[5]);
    container->cart_delay = atoi(argv[6]);

    // 0<V<10
    if(container->cart_count <=0 || container->cart_count>=10)
        return -1;
    //0<N<10000 
    if(container->visit_count<=0 || container->visit_count>=10000)
        return -1; 
    //4<=K<=40
    if(container->cart_capacity<4 || container->cart_capacity>40)
        return -1; 
    //0<=TV<=1000
    if(container->ride_duration<0 || container->ride_duration >1000)
        return -1;
    //0<=TN<=1000
    if(container->queue_duration<0 || container->queue_duration>1000)
        return -1;
    //0<O<=100
    if(container->cart_delay<= 0 ||container->cart_delay>100)
        return -1;  
    return 0;

}
/**
 * @brief function that creates shared memory and creates initial values
 * @return NULL if allocation fails, otherwise pointer to shared memory
 */
shared_data* memory_allocation(container container)
{
    shared_data *data = mmap(NULL,sizeof(shared_data),PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
    if(data == MAP_FAILED)
        return NULL;

    data->x = 1;
    data->visitor_queue = 0;
    data->cart_queue = 0;
    data->closed = 0;   
    data->visitors_boarded = 0;
    data->cart_number =0;

    sem_init(&data->mutex,1,1); // semafor zdielany medzi procesmi, s pociatocnou hodnotou 1 aby ho mohol mat len 1 proces
    sem_init(&data->board_visitor,1,0); //semafor board visitor, zdielany 2. parameter ==1 , caka na pokyn 3.parameter == 0
    sem_init(&data->leave_visitor,1,0);
    sem_init(&data->cart_start,1,0);
    for (int i =0;i<container.cart_count; i++)
    {
        sem_init(&data->FIFO[i], 1, 0);
    }
    sem_post(&data->FIFO[0]);
    sem_init(&data->visitor_ready,1,0);
    sem_init(&data->cart_tranzit,1,0);
    return data;
}
/** *
*@brief clean function for destroying semaphores, cleaning memory allocation etc..
*/
void clean(shared_data *data,container container,FILE *output)
{
    sem_destroy(&data->mutex);
    sem_destroy(&data->board_visitor);
    sem_destroy(&data->leave_visitor);
    sem_destroy(&data->cart_start);
    for (int i =0;i<container.cart_count; i++)
    {
        sem_destroy(&data->FIFO[i]);
    }
    sem_destroy(&data->visitor_ready);
    sem_destroy(&data->cart_tranzit);   
    fclose(output);
    munmap(data,sizeof(shared_data));
}

int process_init(shared_data *data, container *container,FILE *output)
{
    int visitor_count = container->visit_count;
    int cart_count = container->cart_count;
    pid_t pid = fork();


    //pole procesov
    pid_t *process_list = malloc((visitor_count +cart_count +1) *sizeof(pid_t));
    if (process_list == NULL)
        return -1;

    int process_index = 0;

    if (pid <0)
    {   
        free(process_list);
        return -1;
    }
    //DISPATCHER
    if(pid == 0)
    {
        dispatcher(data,container,output);
        exit(0);
    }
    process_list[process_index++] = pid;

    if(pid>0)
    {
        //VOZIKY
        for(int i = 1; i<=cart_count;i++)
        {
            pid_t pid_v = fork();
            if(pid_v<0)
            {
                process_clean(process_list,process_index);
                free(process_list);
                return -1;    
            }
            if(pid_v == 0)
            {
                carts(i,data,container,output);                
                exit(0);
            }
            process_list[process_index++] = pid_v;
        }
        //NAVSTEVNICI
        for(int i = 1; i<=visitor_count;i++)
                {
            pid_t pid_v = fork();
            if(pid_v<0)
            {
                process_clean(process_list,process_index);
                free(process_list);
                return -1;
            }
            if(pid_v == 0)
            {
                visitors(i,data,container,output);
                exit(0);
            }
            process_list[process_index++] = pid_v;
        }
        while (wait(NULL)>0);

        free(process_list);
    }
    return 0;
}
void process_clean(pid_t *process_list,int process_index){
    for(int i =0;i<process_index;i++)
    {
        kill(process_list[i],SIGTERM);
    }
    while(wait(NULL)>0);

}
/**
 * @brief function for dispatcher logic
 * @param id personal number of the cart
 * @param data shared data used by the processes
 * @param container
*/ 
void dispatcher(shared_data *data,container *container,FILE *output)
{
    // startovanie drahy
    sem_wait(&data->mutex);
    fprintf(output,"%i: D: started\n",data->x);
    data->x++;
    sem_post(&data->mutex);

    //TODO 
    while(1)
    {
        // kontrola ci este niekto caka
        sem_wait(&data->mutex);
        int visitor_boarded = data->visitors_boarded;
        sem_post(&data->mutex);
        if(visitor_boarded == container->visit_count)
        {
            data->closed = 1;
            // vsetkym vozikom hovorime ze koncime
            sem_wait(&data->mutex);
            fprintf(output,"%i: D: closing\n",data->x);
            data->x++;
            sem_post(&data->mutex);
            for(int i =0; i<container->cart_count;i++)
                sem_post(&data->cart_start);
            break;
        }
        // nastupovanie vozikov
        sem_wait(&data->mutex);
        fprintf(output,"%i: D: next cart\n",data->x);
        data->x++;
        sem_post(&data->mutex);
        sem_post(&data->cart_start);

        //cakame na odjazd voziku,a  cakame bezpecnu vzdialenost
        sem_wait(&data->cart_tranzit);
        usleep(container->cart_delay);
    }

}
/**
 * @brief function for cart logic
 * @param id personal number of the cart
 * @param data shared data used by the processes
 * @param container user input data
*/ 
void carts(int id,shared_data *data, container *container,FILE *output)
{        
    //randomizujeme cas zvlast pre kazdy call funkcie
    srand(time(NULL) ^ getpid()); 
    //pociatocny started vypis

    sem_wait(&data->mutex);
    fprintf(output,"%i: V %i: started\n",data->x,id);
    data->x++;
    data->cart_queue++;
    sem_post(&data->mutex);

    int sleep_time = container->ride_duration;
    while(1) // voziky chodia nonstop 
    {
        if(sleep_time > 0)
            sleep_time = (rand() % (container->ride_duration - (container->ride_duration /2) +1) ) + (container->ride_duration /2); 

        //Cakame na pokyn a pripravujeme vozik na odchod, popripade ak je zatvorena atrakcia, koncime voziky
        sem_wait(&data->cart_start);
        if(data->closed == 1)
        {
            sem_wait(&data->mutex);
            fprintf(output,"%i: V %i: closed\n",data->x,id);
            data->x++;
            sem_post(&data->mutex);
            break;
        }
        sem_wait(&data->mutex);
        fprintf(output,"%i: V %i: boarding started\n",data->x,id);
        data->x++;

        // davame pokyn na nastup K-krat pricom K je mensie cislo z dvojice kapacity a rady nastupenych ludi
        int unboarded_total = container->visit_count - data->visitors_boarded;
        
        int boarding = container->cart_capacity;
        if (boarding > unboarded_total)
            boarding = unboarded_total;

        sem_post(&data->mutex);
        for(int i=0;i<boarding;i++ )
        {
            sem_post(&data->board_visitor);     // davame pokyn na nastup
            sem_wait(&data->visitor_ready);   // cakame kym nastupia vsetci pasazieri
        }

        sem_wait(&data->mutex);
        fprintf(output,"%i: V %i: boarding complete\n",data->x,id);
        data->x++;
        sem_post(&data->mutex);

        // vozik vchadza na trat
        sem_post(&data->cart_tranzit);
        sem_wait(&data->mutex);
        int my_num = data->cart_number % container->cart_count;
        
        data->cart_number++;
        sem_post(&data->mutex);

        usleep(sleep_time);
        sem_wait(&data->FIFO[my_num]); //cakame na uvolnenie vystupnej stanice
        sem_wait(&data->mutex);
        fprintf(output,"%i: V %i: leaving started\n",data->x,id);
        data->x++;
        sem_post(&data->mutex);

        for(int i =0; i<boarding;i++)
        {
            sem_post(&data->leave_visitor);
            sem_wait(&data->visitor_ready);
        }
        sem_wait(&data->mutex);
        fprintf(output,"%i: V %i: leaving complete\n",data->x,id);
        data->x++;
        sem_post(&data->mutex);
        int next_num =( my_num +1 )% container->cart_count;
        sem_post(&data->FIFO[next_num]);
        // zaradujeme sa naspat do fronty:
    }
}
/**
 * @brief function for visitor logic
 * @param id personal number of the cart
 * @param data shared data used by the processes
 * @param pid pid number of the process
 * @param container user input data
*/ 
void visitors(int id,shared_data *data, container *container,FILE *output)
{
    //randomizujeme cas zvlast pre kazdy call funkcie
    srand(time(NULL) ^ getpid());
    int sleep_time = container->queue_duration;
    if(container->queue_duration > 0)
        sleep_time = rand() % (container->queue_duration +1); // QUEUE_DURATION+1 aby nam mohli padnut aj maximalna hodnota
    // pociatocny started vypis
    sem_wait(&data->mutex);
    fprintf(output,"%i: N %i: started\n",data->x,id);
    data->x++;
    sem_post(&data->mutex);

    // zaradovanie do queue
    usleep(sleep_time);
    sem_wait(&data->mutex);
    fprintf(output,"%i: N %i: queue\n",data->x,id);
    data->x++;
    data->visitor_queue++;
    sem_post(&data->mutex);

    // nastupovanie do voziku
    sem_wait(&data->board_visitor);

    sem_wait(&data->mutex);
    fprintf(output,"%i: N %i: boarding\n",data->x,id);
    data->x++;
    data->visitor_queue--;
    data->visitors_boarded++;
    sem_post(&data->visitor_ready); // som nastupeny!!!
    sem_post(&data->mutex);

    // vystupovanie z voziku
    sem_wait(&data->leave_visitor);
    sem_wait(&data->mutex);
    fprintf(output,"%i: N %i: leaving\n",data->x,id);
    data->x++;
    sem_post(&data->visitor_ready); // som vystupeny :D!
    sem_post(&data->mutex);                                                                         
}