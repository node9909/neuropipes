#include "piperegistry.h"
#include "pipe.h"
#include "linkedlist.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "register_pipes.h"

#define MAX_PARAM_LEN 32

//TODO hashtable ht (char* -> struct) where struct contains init, run, and input_spec
hashtable* ht; //hashtable_create();

int piperegistry_init()  {  //???
  ht = hashtable_create();
  if (ht == NULL)  {
    return 0;
  }

  register_pipes();

  return 1;
} 

//destroy register?

int piperegistry_register(char *name, int(*init)(pipe_*, linkedlist*), int(*run)(pipe_*, linkedlist*), int(*kill)(pipe_*, linkedlist*), char *valid_inputs)  {
  pipedes *pd = (pipedes*)malloc(sizeof(pipedes));
  if (pd == NULL)  {
    fprintf(stderr, "register_pipe: failed to allocate memory\n");
    return 0;
  }
  pd->init = init;
  pd->run = run;
  pd->kill = kill;
  pd->valid_inputs = (char*)malloc(sizeof(char)*strlen(valid_inputs));
  strncpy(pd->valid_inputs, valid_inputs, strlen(valid_inputs));
  if (hashtable_insert(ht,name, (void*)pd) == 0)  {  //name already in table
    free(pd->valid_inputs);
    free(pd);
    fprintf(stderr, "register pipe: failed to register pipe %s\n", name);
    return 0;
  }
  return 1;
}

int piperegistry_deregister(char* name)  {
  pipedes *pd = (pipedes*)hashtable_lookup(ht, name);
  if (pd == NULL)  {
    fprintf(stderr, "deregister_pipe: failed to lookup name\n");
    return 0;
  }
  hashtable_remove(ht, name);
  free(pd->valid_inputs); //?
  free(pd);
  return 1;
}

//return list of valid input data (char*) 
linkedlist* piperegistry_get_valid_inputs(char *name)  {
  return NULL;
}

hashtable *piperegistry_ht()  {
  return ht;
}
