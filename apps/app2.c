unsigned long long text_section_top = 0x105000000;

void app2() {
  while (1) {
    char *str = "Hello from app2\r\n";

    // TODO: Say hello from here by issuing `puts` syscall
    unsigned long long id;
    id = 1;
    unsigned long long syscall_ret;
    asm volatile (
      "movq %[id], %%rax\n"
      "movq %[id], %%rdi\n"
      "movq %[arg1], %%rsi\n"
      "int $0x80\n"
      "movq %%rax, %[ret]\n"
      :[ret]"=r"(syscall_ret)
      :[id]"g"(id),
      [arg1]"m"((unsigned long long) str)
    );

    volatile int i = 100000000;
    while (i--);
  }

  asm volatile ("jmp *%0" :: "m"(text_section_top));
}
