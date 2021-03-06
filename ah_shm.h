#pragma GCC diagnostic ignored "-Wwrite-strings"
#ifndef AH_SHM_H
#define AH_SHM_H

/*
g++ --std=c++11 `pkg-config --cflags glib-2.0` -g shm_client.cpp -o shm_client.o `pkg-config --libs glib-2.0`
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glib.h>

#define SHMSZ 27

template <class T>
class shm_server {
public:
	char c;
	int shmid;
	key_t key;
	T *shm;

	shm_server(key_t);
	~shm_server();
	void set(T);
	T get(void);
};

template <class T>
shm_server<T>::shm_server(key_t _key){
	// use the passed in key to set the location
    key = _key;
		printf("size of T: %lu\n", sizeof(T));
		printf("key: %d\n", key);
		printf("flags: %i\n", IPC_CREAT | 0666);
    // create the actual segment
    if ((shmid = shmget(key, sizeof(T), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm = (T*)shmat(shmid, NULL, 0)) == (T *) -1) {
        perror("shmat");
        exit(1);
    }

}

template <class T>
shm_server<T>::~shm_server(){
	int ret = shmdt(shm);
	if (ret != 0) perror("shmdt");
	ret = shmctl(key, IPC_RMID, NULL);
	if (ret != 0) perror("shmctl");
}

template <class T>
void shm_server<T>::set(T val){
	memcpy (shm, &val, sizeof(T));
}

template <class T>
T shm_server<T>::get(void){
	return *shm;
}

template <class T>
class shm_client {
public:
	int shmid;
	key_t key;
	T *shm;

	shm_client(key_t);
	~shm_client();
	T get(void);
};

template <class T>
shm_client<T>::shm_client(key_t _key){
    // use the passed in key to set the location
    key = _key;

    // find the actual segment
    if ((shmid = shmget(key, sizeof(T), 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    //attach the segment to local memory
    if ((shm = (T*)shmat(shmid, NULL, 0)) == (T *) -1) {
        perror("shmat");
        exit(1);
    }

}

template <class T>
shm_client<T>::~shm_client(void){
	int ret = shmdt(shm);
	if (ret != 0) perror("shmdt");
}

template <class T>
T shm_client<T>::get(void){
	return *shm;
}


#endif
