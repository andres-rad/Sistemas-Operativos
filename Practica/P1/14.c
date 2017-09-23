pid shared_parent_pid;
pid shared_child_pid;

mem_share(&shared_parent_pid);
mem_share(&shared_child_pid);

shared_parent_pid = get_current_pid();
pid child = fork();

if (child == 0) {
  shared_child_pid = get_current_pid();
  bsend(shared_parent_pid, "hola");
  breceive(shared_parent_pid);
  exit(OK);
} else {
  breceive(shared_child_pid <=> child); <-- Si este proceso corre primero shared_child_pid
                                            tiene basura, seguro se cuelga indefinidamente
                                            (<=> reemplazar para solucionar)
  bsend(shared_child_pid, "chau");
  exit(OK);
}
