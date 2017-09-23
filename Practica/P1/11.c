int result;
pid_t pid_derecha;
pid_t pid_izquierda;

void proceso_izquierda() {
  result = 0;
  while (true) {
    while(!bsend(pid_derecha, result)){}
    result = computo_muy_dificil_1();
    }
}

void proceso_derecha() {
  while(true) {
    result = computo_muy_dificil_2();
    int left_result;
    while(!receive(pid_izquierda, &left_result)){}
    printf(" %s %s", left_result, result);
  }
}
