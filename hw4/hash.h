
#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include "list.h"

#define HASH_INDEX(_addr,_size_mask) (((_addr) >> 2) & (_size_mask))

template<class Ele, class Keytype> class hash;

template<class Ele, class Keytype> class hash {
 private:
  unsigned my_size_log;
  unsigned my_size;
  unsigned my_size_mask;
  list<Ele,Keytype> *entries;
  list<Ele,Keytype> *get_list(unsigned the_idx);
#ifdef LIST_LOCK
  pthread_mutex_t * lock;
#endif
 public:
  void setup(unsigned the_size_log=5);
  void insert(Ele *e);
  Ele *lookup(Keytype the_key);
#ifdef LIST_LOCK
  void lock_list(Keytype the_key);
  void unlock_list(Keytype the_key);
#endif
  void print(FILE *f=stdout);
  void reset();
  void cleanup();
};

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::setup(unsigned the_size_log){
  int i;
  my_size_log = the_size_log;
  my_size = 1 << my_size_log;
  my_size_mask = (1 << my_size_log) - 1;
  entries = new list<Ele,Keytype>[my_size];
#ifdef LIST_LOCK  
  lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*my_size);

  for(i=0; i<16384; i++)
    pthread_mutex_init(&lock[i], NULL);
#endif
}

template<class Ele, class Keytype> 
list<Ele,Keytype> *
hash<Ele,Keytype>::get_list(unsigned the_idx){
  if (the_idx >= my_size){
    fprintf(stderr,"hash<Ele,Keytype>::list() public idx out of range!\n");
    exit (1);
  }
  return &entries[the_idx];
}

template<class Ele, class Keytype> 
Ele *
hash<Ele,Keytype>::lookup(Keytype the_key){
  list<Ele,Keytype> *l;

  l = &entries[HASH_INDEX(the_key,my_size_mask)];
  return l->lookup(the_key);
}  



#ifdef LIST_LOCK
template<class Ele, class Keytype> 
void
hash<Ele,Keytype>::lock_list(Keytype the_key){
  list<Ele,Keytype> *l;
  pthread_mutex_lock(&lock[HASH_INDEX(the_key, my_size_mask)]);
}

template<class Ele, class Keytype> 
void
hash<Ele,Keytype>::unlock_list(Keytype the_key){
  list<Ele,Keytype> *l;
  pthread_mutex_unlock(&lock[HASH_INDEX(the_key, my_size_mask)]);
}
#endif

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::print(FILE *f){
  unsigned i;

  for (i=0;i<my_size;i++){
    entries[i].print(f);
  }
}

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::reset(){
  unsigned i;
  for (i=0;i<my_size;i++){
    entries[i].cleanup();
  }
}

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::cleanup(){
  unsigned i;
  reset();
  delete [] entries;
}

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::insert(Ele *e){
  entries[HASH_INDEX(e->key(),my_size_mask)].push(e);
}


#endif
