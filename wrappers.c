/************************************************
 * Wrappers for system call functions
 ************************************************/
#include "wrappers.h"

pid_t Fork(void)
{
    pid_t n;
    n = fork() ;
    if ( n < 0 )
    {        
        perror("Fork failed");
        exit( -1 ) ;
    }
    return n ;
}

int  Shmget(key_t key, size_t size, int shmflg)
{
    int   shmid ;
    char  buf[100] ;

    shmid = shmget( key , size , shmflg ) ;
    if ( shmid == -1 ) 
    {
        snprintf ( buf, 100 , "Failed to get shared memory key=%d" , key );
        perror( buf ) ;
        exit( -1 ) ;
    }
    return shmid ;
}

void *Shmat(int shmid, const void *shmaddr, int shmflg)
{
    void *p ;
    char  buf[100] ;

    p = shmat( shmid , shmaddr , shmflg );
    if ( p == (void *) -1 ) 
    {
        snprintf ( buf , 100 , "Failed to attach shared memory id=%d" , shmid );
        perror( buf ) ;
        exit( -1 ) ;
    }
    return p;    
}

int  Shmdt( const void *shmaddr )
{
    int code ;

    code = shmdt( shmaddr );
    if ( code != 0  ) 
    {
        perror( "shmdt Failed" ) ;
        exit( -1 ) ;
    }
    return code ;    
}

/* unix-style error 
void unix_error(char *msg) 
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit( -1 );
}
*/

sem_t  *Sem_open(const char *name, int oflag, mode_t mode, unsigned int value)
{
    sem_t  *s;
    char  buf[100] ;

    s = sem_open( name , oflag , mode , value );
    if ( s == SEM_FAILED ) 
    { 
        snprintf ( buf , 100 , "Failed to open semaphore '%s'" , name );
        perror( buf ) ; 
        exit( -1 ); 
    }
    return s;
}

sem_t  *Sem_open2( const char *name, int oflag )
{
    sem_t  *s;
    char  buf[100] ;

    s = sem_open( name , oflag );
    if ( s == SEM_FAILED ) 
    { 
        snprintf ( buf , 100 , "Failed to open semaphore '%s'" , name );
        perror( buf ) ; 
        exit( -1 ); 
    }
    return s;
}

int  Sem_close(sem_t *sem)
{
    int code ;

    code = sem_close( sem ) ;
    if ( code != 0 )
    {
        perror("sem_close failed");
        exit( -1 ) ;
    } 
    return code ;    
}

int  Sem_unlink( const char *name )
{
    int code ;
    char  buf[100] ;

    code = sem_unlink( name ) ;
    if ( code != 0 )
    {
        snprintf ( buf , 100 , "Failed to unlink semaphore '%s'" , name );
        perror( buf ) ; 
        exit( -1 ) ;
    } 
    return code ;    
}

int  Sem_wait(sem_t *sem) 
{
    int code ;

    code = sem_wait(sem) ;
    if ( code != 0 )
    {
        perror("sem_wait failed");
        exit( -1 ) ;
    }
    return code ;
}

int  Sem_post(sem_t *sem) 
{
    int code ;

    code = sem_post(sem)  ;
    if ( code != 0 )
    {
        perror("sem_post failed");
        exit( -1 ) ;
    }
    return code ;
}

int  Sem_init(sem_t *sem, int pshared, unsigned int value) 
{
    int code ;
    
    code = sem_init( sem, pshared, value ) ;
    if ( code != 0 )
    {
        perror("sem_init failed");
        exit( -1 ) ;
    }
    return code ;
}

int  Sem_destroy(sem_t *sem) 
{
    int code ;

    code = sem_destroy( sem ) ;
    if ( code < 0)
    {
        perror("sem_destroy error");
        exit( -1 ) ;
    }
    return code ;
}

int   Msgget( key_t key, int msgflg )
{
    int code ;
    char  buf[100] ;

    code = msgget( key , msgflg ) ;
    if ( code == -1 )
    {
        snprintf ( buf , 100 , "Failed to get Msg queue with key=%d" , key );
        perror( buf ) ; 
        exit( -1 ) ;
    } 
    return code ;    
    
}

void  Pthread_create(   pthread_t *tidp, pthread_attr_t *attrp
                      , void * (*routine)(void *), void *argp ) 
{
    int code;

    code = pthread_create(tidp, attrp, routine, argp) ;
    if (code != 0)
    {
        perror("Pthread_create error");
        exit( -1 ) ;
    } 
}

void  Pthread_join(pthread_t tid, void **thread_return) 
{
    int code;

    code = pthread_join( tid, thread_return ) ;
    if (code != 0)
    {
        perror("Pthread_join error");
        exit( -1 ) ;
    } 
}
key_t Ftok(char *pathname, char proj_id) {
    int key;
    key = ftok(pathname, proj_id);
    if (key == -1) {
        perror("Frok failed");
        exit(-1);
    }
}

