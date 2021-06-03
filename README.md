# Escalonamento de Processos
Simulador de um Escalonador de Processos

1.  O sistema simula como acontece o escalonamento de processos utilizando agendamento Round-robin.

2.  Informações do sistema:
    * CPUs: 4 unid
    * arrival_min: 12 unid
    * arrival_max: 28 unid
    * QUANTUM: 2 unid
    * printer: 2 unid
    * scanner: 1 unid
    * modem: 1 unid
    * cd: 2 unid
    * mb: 1024 MB

3.  `input_values.txt` contém as seguintes informações de cada processo:
    * `arrival_time`: tempo (em unidade) que o processo solicitou execução.
    * `priority`: prioridade de execução do processo [0,3], sendo 0 a maior prioridade e 3 a menor.
    * `processor_time`: tempo (em unidade) que o processo levará para executar.
    * `mbytes`: tamanho (em megabytes) do processo.
    * `printers`: quantidade de impressoras que o processo utilizará.
    * `scanners`: quantidade de escaneadoras que o processo utilizará.
    * `modems`: quantidade de modems que o processo utilizará.
    * `cds`: quantidade de CDs que o processo utilizará.
