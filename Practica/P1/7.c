#define HIJOS 2
#define SIZE 5

void taka_runner() {
  while (true) {
    temp = tiki;
    temp++;
    taka = temp;
  }
}

void tiki_taka() {
  while (true) {
    temp = taka;
    temp++;
    tiki = temp;
  }
}

int main(){
  pid_t childs[HIJOS];
  int tiki;
  int taka;

  share_mem(&tiki);
  share_mem(&taka);
  int temp;

  for(int i = 0; i < HIJOS; i++){
    int cur = fork();
    if(cur == -1){
      perror("FORK error");
    }
    if(childs[i] == 0){
      if(i == 0){
         tiki_runner();
      } else {
        taka_runner();
      }

    } else {
      childs[i] = cur;
    }
  }
  return 0;
}
