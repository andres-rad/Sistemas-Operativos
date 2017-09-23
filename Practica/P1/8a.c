// a)
int main(){
  pid_t child;
  pid_t parent = get_current_id();

  if( (int p = fork()) == -1) perror("ERROR FORK");

  if(p == 0){
    /* Proceso Hijo */
    while(true)}{
      int msg_c = brecieve(parent);
      bsend(parent, ++msg_c);
    }
  } else {
    /*Proceso Padre*/
    int msg = 0;
    while(true){
      bsend(p, msg);
      msg = ++brecieve(p);
  }

  return 0;
}

// b)

#define HIJOS 2

int main(){
  pid_t child[HIJOS];
  pid_t parent = get_current_id();

  for(int i = 0; i < HIJOS; i++){
    if((int p = fork()) == -1) perror("ERROR FORK");

    if(p == 0){
      switch(i){
        case 0:
          pid_t hermano1 = (pid_t) brecieve(parent);
          bsend(parent, 0); //Envio confirmacion
          while(true)
            bsend(hermano1, ++brecieve(parent));
          break;

        case 1:
            pid_t hermano0 = (pid_t) brecieve(parent);
            bsend(parent, 0);
          while(true)
            bsend(parent, ++brecieve(hermano0));
          break;
        }
    } else {
      child[i] = p
    }
  }
  bsend(child[0], (int)child[1]);
  brecieve(child[0]); //Me asegura de que haya solo 1 mensaje activo
  bsend(child[1], (int)child[0]);
  breceive(child[1]);
  int msg = 0;
  while(true){
    bsend(child[0], msg);
    msg = ++brecieve(child[1]);
  }
  return 0;
}

// c) ?? preguntar
