# Produtor/Consumidor em um RTOS utilizando STM32F103

[Objetivos](#objetivos)

[Implementação do semáforo](#implementação-do-semáforo)

[Sobre o RTOS utilizado](#sobre-o-rtos-utilizado)

[Utilização e Visualização](#utilização-e-visualização)

![BluepillStlink](/assets/bluepill_stlink.jpg)

## Objetivos
Esse repositório contém a solução para o problema do produtor/consumidor. Esse clássico problema deveria ser implementado em um microcontrolador, STM32F103, utilizando um Real-Time Operating System (RTOS). O RTOS utilizado foi o MInimal Real-time Operating System (MiROS)

## Produtor/Consumidor
Um exemplo de sistema envolvendo produtor/consumidor consiste em duas *threads* manipularem o mesmo buffer. A principal dificuldade tornar a inserção e retirada desses elementos de forma segura. 

Uma das principais soluções é utilizar semáforos, que permitem a inserção, retirada, e manipulação dos dados na área crítica de forma segura e eficiente. 

## Implementação do semáforo
Devido ao fato do sistema de tempo real utilizado (MiROS) não possuir semáforos, foi necessário implementá-lo.

A implementação foi feita utilizando os arquivos *semaphore.h* e *semaphore.c*. O semáforo contruído possui três funções básicas:

1. `void sem_init(semaphore* s, int32_t init_value)`
2. `void sem_wait(semaphore* s)`
3. `void sem_post(semaphore* s)`

Onde *semaphore* representa a seguinte estrutura:
```
typedef struct {
	int32_t semCount;
	bool isBlocked;
} semaphore;
```

A primeira função inicializa o semáforo, a segunda função é responsável por decrementar o valor do semáforo, podendo ficar em espera até que o seu valor possa de fato ser decrementado. A terceira função é responsável por incrementar o valor do semáforo. Além disso, foi necessário utilizar uma variável booleana, *isBlocked*, para controlar o estado de bloqueio do semáforo.

No arquivo *main.c*, foram criados três semáforos: *semEmpty*, *semFull* e *mutex*. O *mutex* serve para proteger a região crítica enquanto *semEmpty* e *semFull* controlam a quantidade de elementos no buffer.

## Sobre o RTOS utilizado
Como dito anteriormente, o RTOS utilizado foi o (MiROS). Ele possui funções essenciais para o gerenciamento de tarefas, e as que foram utilizadas nesse projeto foram:
1. `void OS_init(void *stkSto, uint32_t stkSize)`
2. `void OS_sched(void)`
3. `void OS_run(void)`
4. `void OS_delay(uint32_t ticks)`
5. `void OSThread_start(OSThread *me, uint8_t prio, OSThreadHandler threadHandler, void *stkSto, uint32_t stkSize)`

A função *OS_Init* é chamada no início da *main.c*, e são passados parâmetros *stackIdleThread*.

A função *OS_sched* é responsável pelo escalonamento, que gerencia a mudança de tarefas.

A função *OS_run* é chamada no final da *main.c*, para transferir controle ao RTOS com as tarefas criadas.

A função *OSThread_start* inicia uma nova tarefa, e no projeto foi utilizada para iniciar as tarefas de consumo e produção.

## Utilização e Visualização
Na STM32CubeIde, execute o código em modo *debug*. 

Após iniciar nesse modo, selecione a visualização das variáveis *buffer*, *bufferCount* e *consumedValue*.   

A variável *buffer* começará a ser preenchida por elementos. À medida que dados são inseridos no *buffer*, a váriavel *bufferCount* guardará quantos elementos estão presentes e a variável *consumedValue* indica qual foi o último elemento consumido no *buffer*. Cada vez que um elemento é consumido, é colocado 0 na posição do *buffer* do elemento consumido.

Como a função que produz elementos possui um menor *delay* que a função que consome, eventualmente o buffer será preenchido por todos os elementos, com a capacidade indicada pela variável *BUFFER_SIZE*. A partir desse ponto, apenas o último elemento será consumido e, rapidamente, será produzido de novo um novo elemento no mesmo lugar. Assim, o programa ficará nesse ciclo.