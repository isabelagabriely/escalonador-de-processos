#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define CPUs 4
#define arrival_min 12
#define arrival_max 28
#define QUANTUM 2

int printer = 2;
int scanner = 1;
int modem = 1;
int cd = 2;
int mb = 1024;

typedef struct {
    int num_process;
    int arrival_time;
    int priority;
    int processor_time;
    int mbytes;
    int printers;
    int scanners;
    int modems;
    int cds;
} process;

typedef struct aux {
    process p;
    struct aux *prox;
} element;

typedef struct {
    element *begin;
    element *end;
} QUEUE;

void init_queue (QUEUE *q) {
    q->begin = NULL;
    q->end = NULL;
}

int pid () {
    int num = (rand() % (6000-1400) + 1400);
    return num;
}

void add_to_queue (QUEUE *q, process p) {
    element *new = (element*) malloc (sizeof (element));
    new->p = p;
    new->prox = NULL;

    if (q->begin == NULL)
        q->begin = new;

    else 
        q->end->prox = new;

    q->end = new;
}

void add_process (QUEUE *entry) {
    FILE *f = fopen ("input_values.txt", "r");
    process p;
    while (fscanf (f, "%d %d %d %d %d %d %d %d", &p.arrival_time, &p.priority, &p.processor_time, &p.mbytes, &p.printers, &p.scanners, &p.modems, &p.cds) != EOF) {
        p.num_process = pid();
        add_to_queue (entry, p);
    }

    fclose(f);
}

void agent (QUEUE *entry, QUEUE *real_time, QUEUE *user, int arrival) {
    element *e = entry->begin;

    while (e != NULL) {
        if (e->p.arrival_time == arrival) {
            if (e->p.priority == 0)
                add_to_queue (real_time, e->p);

            else
                add_to_queue (user, e->p);
        }
        e = e->prox;
    }
}

element* remove_from_queue (QUEUE *q) {
    if (q->begin == NULL)
    return NULL;

    element *p = q->begin;
    q->begin = q->begin->prox;

    if (q->begin == NULL) {
        q->end = NULL;
    }

    return p;
}

void present_queue (element *initial) {
    if (initial != NULL) {
        printf ("PID:%-4d ARRIVAL_TIME:%-4d PRIORITY:%-4d PROCESSOR_TIME:%-4d MBYTES:%-4d PRINTERS:%-4d SCANNER:%-4d MODEM:%-4d CDS:%-4d\n", initial->p.num_process, initial->p.arrival_time, initial->p.priority, initial->p.processor_time, initial->p.mbytes, initial->p.printers, initial->p.scanners, initial->p.modems, initial->p.cds);
        present_queue (initial->prox);
    }
}

int available_cpu (process *CPUS) {
    int i;
    for (i=0; i<CPUs; i++) {
        if (CPUS[i].priority == -1)
            return i;
    }

    return -1;
}

void clean_cpu (process *CPUS, int cpu) {
    CPUS[cpu].num_process = -1;
    CPUS[cpu].arrival_time = -1;
    CPUS[cpu].priority = -1;
    CPUS[cpu].processor_time = -1;
    CPUS[cpu].mbytes = -1;
    CPUS[cpu].printers = -1;
    CPUS[cpu].scanners = -1;
    CPUS[cpu].modems = -1;
    CPUS[cpu].cds = -1;
}

int remove_process (QUEUE *suspended, process *CPUS) {
    int i, ret;
    for (i=0; i<CPUs; i++) {
        if (CPUS[i].priority != 0) {
            add_to_queue (suspended, CPUS[i]);
            ret = i;
            printf ("..............PROCESSO %d SUSPENSO\n", CPUS[i].num_process);
            clean_cpu(CPUS, i);
        }
    }
    return ret;
}

void schedule (process p, QUEUE *blocked, QUEUE *suspended, process *CPUS) {
    int cpu = available_cpu(CPUS);

    if (p.priority == 0) {
        if (cpu == -1)
            cpu = remove_process (suspended, CPUS);
    }

    else if (p.priority != -1) {
        if (p.mbytes > mb || p.printers > printer || p.scanners > scanner || p.modems > modem || p.cds > cd || cpu == -1) {
            printf ("..............PROCESSO %d BLOQUEADO\n", p.num_process);
            add_to_queue (blocked, p);
            return;
        }
    }

    mb-=p.mbytes;
    printer-=p.printers;
    scanner-=p.scanners;
    modem-=p.modems;
    cd-=p.cds;

    CPUS[cpu] = p;
}

void return_resources (process p) {
    mb+=p.mbytes;
    printer+=p.printers;
    scanner+=p.scanners;
    modem+=p.modems;
    cd+=p.cds;
}

void policies (QUEUE *q, QUEUE *blocked, QUEUE *suspended, process *CPUS) {

    element *e = remove_from_queue (q);

    while (e != NULL) {
        schedule (e->p, blocked, suspended, CPUS);
        e = remove_from_queue (q);
    }
}

void execute (process *CPUS, int arrival, QUEUE* userX2, int x) {
    int i;
    for (i=0; i<CPUs; i++) {
        if (CPUS[i].priority != -1) {

            int t = 1;

            if (CPUS[i].arrival_time == arrival)
                t = 0;

            printf ("CPU%d executando: ", i);
            printf ("PID: %d ", CPUS[i].num_process);
            printf ("Prioridade: %d ", CPUS[i].priority);
            printf ("Tempo restante de processamento: %d ", CPUS[i].processor_time-=t);
            printf ("Mbytes: %d ", CPUS[i].mbytes);
            printf ("Impressoras: %d ", CPUS[i].printers);
            printf ("Scanner: %d ", CPUS[i].scanners);
            printf ("Modem: %d ", CPUS[i].modems);
            printf ("CDs: %d ", CPUS[i].cds);
            printf ("\n");

            if (CPUS[i].processor_time == 0) {
                printf ("..............PROCESSO %d: CONCLUIDO\n", CPUS[i].num_process);
                return_resources (CPUS[i]);
                clean_cpu (CPUS, i);
            }

            else if (CPUS[i].priority != 0 && CPUS[i].priority != -1)
                if (CPUS[i].arrival_time+QUANTUM*x == arrival) {
                    add_to_queue (userX2, CPUS[i]);
                    printf ("..............PROCESSO %d: FIM DO QUANTUM\n", CPUS[i].num_process);
                    return_resources (CPUS[i]);
                    clean_cpu (CPUS, i);
                }
        }
    }
}

int main (void) {
    QUEUE entry;
    init_queue (&entry);
    add_process (&entry);
    printf ("\t\t\tESCALONADOR DE PROCESSOS\n");
    printf ("PROCESSOS:\n");
    present_queue(entry.begin);

    int i;
    process *CPUS = (process*) malloc (sizeof (process) * 4);
    for (i=0; i<CPUs; i++) {
        clean_cpu(CPUS, i);
    }

    QUEUE real_time;
    QUEUE user;
    QUEUE suspended;
    QUEUE blocked;
    QUEUE userX2;

    init_queue (&real_time);
    init_queue (&user);
    init_queue (&suspended);
    init_queue (&blocked);
    init_queue (&userX2);

    for (i=arrival_min; i<=arrival_max; i++) {

        int q = 1;

        printf ("\nTEMPO:%d\n", i);

        agent(&entry, &real_time, &user, i);

        if (real_time.begin != NULL) 
            policies (&real_time, &blocked, &suspended, CPUS);

        if (user.begin != NULL)
            policies (&user, &blocked, &suspended, CPUS);

        if (suspended.begin != NULL)
            policies (&suspended, &blocked, &suspended, CPUS);

        if (userX2.begin != NULL) {
            q = 2;
            policies (&userX2, &blocked, &suspended, CPUS);
        }

        if (mb == 1024 && printer == 2 && scanner == 1 && modem == 1 && cd == 2)
            if (blocked.begin != NULL)
            policies (&blocked, &blocked, &suspended, CPUS);

        execute(CPUS, i, &userX2, q);
        printf("\n");
    }
}