#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
  
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
  
}

long hash( char * key, long capacity) {
  
    unsigned long hash = 0;
    char * ptr;
  
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
  
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {

    if (map == NULL || key == NULL)
        return;
    
    long index = hash(key, map->capacity);
    long original_index = index;
  
    while (map->buckets[index] != NULL && !is_equal(map->buckets[index]->key, key)) { 
      
        index = (index + 1) % map->capacity; 
      
        if (index == original_index) { 
            
            enlarge(map);
            index = hash(key, map->capacity);
            original_index = index;
          
        }
    }
  
    if (map->buckets[index] != NULL && is_equal(map->buckets[index]->key, key)) {
      
        free(map->buckets[index]->value);
        map->buckets[index] = createPair(key, value);
      
    } else {
      
        map->buckets[index] = createPair(key, value);
        map->size++;
      
    }
}

void enlarge(HashMap * map) {

  Pair ** oldBuckets = map->buckets;
  long oldCapacity = map->capacity;

  map->capacity *= 2;
  map->buckets = (Pair **) malloc(sizeof(Pair *) * map->capacity);
  map->size = 0;

  for (int i = 0; i < oldCapacity; i++) {

    if (oldBuckets[i] != NULL && oldBuckets[i]->key != NULL) {

      insertMap(map, oldBuckets[i]->key, oldBuckets[i]->value);
      
    }
  }
  
  enlarge_called = 1; //no borrar (testing purposes)
  free(oldBuckets);
  
}


HashMap * createMap(long capacity) {

    HashMap * map = (HashMap *) malloc(sizeof(HashMap));
  
    map->buckets = (Pair **) calloc(capacity, sizeof(Pair *));
    map->capacity = capacity;
    map->size = 0;
    map->current = -1;
    return map;
  
}

void eraseMap(HashMap * map,  char * key) {

  if (map == NULL || key == NULL) 
    return;

  long index = hash(key, map->capacity);
  long original_index = index;

  while (map->buckets[index] != NULL && !is_equal(map->buckets[index]->key, key)) {

    index = (index + 1) % map->capacity;

    if (index == original_index) {

      return;
      
    }
  }

  if (map->buckets[index] != NULL && is_equal(map->buckets[index]->key, key)) {

    map->buckets[index]->key = NULL;
    map->buckets[index]->key = NULL;
    map->size--;
    
  } 
}

Pair * searchMap(HashMap * map, char * key) {
  
    if (map == NULL || key == NULL) 
        return NULL;
    
    long index = hash(key, map->capacity);
    long original_index = index;
  
    while (map->buckets[index] != NULL && !is_equal(map->buckets[index]->key, key)) {
      
        index = (index + 1) % map->capacity;
      
        if (index == original_index) {
            return NULL; // Key not found
        }
    }
  
    if (map->buckets[index] != NULL && is_equal(map->buckets[index]->key, key)) {
      
        map->current = index;
        return map->buckets[index];
      
    }
    return NULL; 
}

Pair * firstMap(HashMap * map) {

  if (map == NULL)
    return NULL;

  for (long i = 0; i < map->capacity; i++) {

    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {

      map->current = i;
      return map->buckets[i];
      
    }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) {

  if (map == NULL || map->buckets == NULL)
    return NULL;

  long index = map->current + 1;

  while (index < map->capacity) {

    if (map->buckets[index] != NULL && map->buckets[index]->key != NULL) {

      map->current = index;
      return map->buckets[index];
    }
    index++;
  }
  
  map->current = -1;
  return NULL;
}
