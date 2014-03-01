// This must be included only from the xcoro.h

struct xcoro {
	int filler;
};

struct cpu_ctx {
    void *esp;
    void *ebp;
    void *eip;
    void *edi;
    void *esi;
    void *ebx;
    void *r1;
    void *r2;
    void *r3;
    void *r4;
    void *r5;
};

struct xcoro_task {
	struct cpu_ctx ctx;
	char name[32];
	void (*entry_point)(void *);
	void *arg;
	void *stack;
	unsigned stack_size;
};
