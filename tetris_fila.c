// tetris_fila.c
// Tetris Stack (nível novato)
// Fila circular de peças com menu: 1=dequeue, 2=enqueue, 0=sair
// Impressão em UTF-8 funcionando no Windows e no Linux/macOS.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <stdarg.h>
#ifdef _WIN32
  #include <windows.h>
#endif

// ---------- Impressão UTF-8 que funciona no Windows ----------
// Se for console do Windows, converte para UTF-16 e usa WriteConsoleW.
// Se for arquivo/pipe ou Linux/macOS, escreve UTF-8 direto.
#ifdef _WIN32
static int is_console(FILE *f, HANDLE *hout) {
    DWORD mode;
    if (f == stdout)      *hout = GetStdHandle(STD_OUTPUT_HANDLE);
    else if (f == stderr) *hout = GetStdHandle(STD_ERROR_HANDLE);
    else { *hout = INVALID_HANDLE_VALUE; return 0; }
    if (*hout == INVALID_HANDLE_VALUE) return 0;
    return GetConsoleMode(*hout, &mode) != 0;
}
#endif

static void u8vfprintf(FILE *f, const char *fmt, va_list ap) {
    va_list ap2; va_copy(ap2, ap);
    int n = vsnprintf(NULL, 0, fmt, ap2);
    va_end(ap2);
    if (n < 0) return;
    char *buf = (char*)malloc((size_t)n + 1);
    if (!buf) return;
    vsnprintf(buf, (size_t)n + 1, fmt, ap);

#ifdef _WIN32
    HANDLE hout;
    if (is_console(f, &hout)) {
        int wlen = MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
        if (wlen > 0) {
            wchar_t *wbuf = (wchar_t*)malloc((size_t)wlen * sizeof(wchar_t));
            if (wbuf) {
                MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, wlen);
                DWORD written;
                WriteConsoleW(hout, wbuf, (DWORD)(wlen - 1), &written, NULL);
                free(wbuf);
            }
        }
    } else {
        fwrite(buf, 1, (size_t)n, f);
    }
#else
    fwrite(buf, 1, (size_t)n, f);
#endif
    free(buf);
}
static void u8printf(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    u8vfprintf(stdout, fmt, ap);
    va_end(ap);
}
static void u8eprintf(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    u8vfprintf(stderr, fmt, ap);
    va_end(ap);
}

// ---------- Dados da fila ----------
typedef struct {
    char nome;  // Tipo da peça: 'I','O','T','L'
    int  id;    // Número único que só cresce quando entra na fila
} Peca;

typedef struct {
    Peca *v;    // Vetor circular
    int cap;    // Capacidade total
    int ini;    // Índice do primeiro (frente)
    int tam;    // Quantidade atual
} Fila;

// ---------- Operações da fila ----------
static void fila_init(Fila *f, int cap) {
    f->v = (Peca*)malloc(sizeof(Peca) * cap);
    f->cap = cap; f->ini = 0; f->tam = 0;
}
static void fila_free(Fila *f) {
    free(f->v); f->v = NULL; f->cap = f->ini = f->tam = 0;
}
static int fila_vazia(const Fila *f) { return f->tam == 0; }
static int fila_cheia(const Fila *f) { return f->tam == f->cap; }

static int enqueue(Fila *f, Peca p) {
    if (fila_cheia(f)) return 0;
    int fim = (f->ini + f->tam) % f->cap;
    f->v[fim] = p;
    f->tam++;
    return 1;
}
static int dequeue(Fila *f, Peca *out) {
    if (fila_vazia(f)) return 0;
    if (out) *out = f->v[f->ini];
    f->ini = (f->ini + 1) % f->cap;
    f->tam--;
    return 1;
}

// ---------- Exibição e leitura ----------
static void imprimirFila(const Fila *f) {
    u8printf("\nFila de peças\n");
    if (fila_vazia(f)) { u8printf("(vazia)\n"); return; }
    for (int i = 0; i < f->tam; ++i) {
        int idx = (f->ini + i) % f->cap;
        u8printf("[%c %d] ", f->v[idx].nome, f->v[idx].id);
    }
    u8printf("\n");
}
static int lerInt(void) {
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin)) return -1;
    char *p = buf; while (isspace((unsigned char)*p)) p++;
    if (!*p || (*p!='-' && !isdigit((unsigned char)*p))) return -1;
    return (int)strtol(p, NULL, 10);
}

// ---------- Geração de peças ----------
// Gira os tipos em ordem fixa e só aumenta o id quando a peça entra na fila.
static Peca gerarPeca(void) {
    static int prox_id = 0;
    static const char tipos[] = { 'T','O','L','I' };
    Peca p; p.nome = tipos[prox_id % 4]; p.id = prox_id++;
    return p;
}

// ---------- Programa ----------
int main(void) {
    setlocale(LC_ALL, ""); // ajuda fora do Windows; no Windows usamos a rotina acima

    enum { CAPACIDADE = 8, INICIAIS = 5 };

    Fila fila; fila_init(&fila, CAPACIDADE);

    // Carrega 5 peças iniciais
    for (int i = 0; i < INICIAIS; ++i) enqueue(&fila, gerarPeca());

    // Menu simples
    while (1) {
        imprimirFila(&fila);
        u8printf("\nOpções:\n");
        u8printf("1 - Jogar peça (dequeue)\n");
        u8printf("2 - Inserir nova peça (enqueue)\n");
        u8printf("0 - Sair\n");
        u8printf("Escolha: ");

        int op = lerInt();
        if (op == 0) {
            u8printf("Encerrando.\n");
            break;
        } else if (op == 1) {
            Peca jogada;
            if (dequeue(&fila, &jogada)) {
                u8printf("Jogou peça: [%c %d]\n", jogada.nome, jogada.id);
            } else {
                u8printf("Fila vazia. Nada a jogar.\n");
            }
        } else if (op == 2) {
            // Só gera peça se tiver espaço, para não “pular” id à toa
            if (!fila_cheia(&fila)) {
                Peca nova = gerarPeca();
                enqueue(&fila, nova);
                u8printf("Peça inserida no fim da fila.\n");
            } else {
                u8printf("Fila cheia. Não foi possível inserir.\n");
            }
        } else {
            u8printf("Opção inválida.\n");
        }
    }

    fila_free(&fila);
    return 0;
}
