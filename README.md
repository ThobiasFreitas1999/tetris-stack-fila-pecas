# Tetris Stack – Fila de Peças

Sabe aquela filinha de peças que aparece no Tetris mostrando quais vão cair depois? Pois é, esse projeto simula exatamente isso! É um programa em C que gerencia uma **fila de peças** do Tetris Stack. Você pode ver quais peças estão esperando, jogar a próxima da fila ou adicionar novas peças no final. Simples assim! 🎮

A ideia aqui é entender como funciona uma **fila** (tipo aquela fila do banco: quem chega primeiro, sai primeiro!).

---

## 📋 O que o programa faz

- ✅ Começa com **5 peças** já na fila (dá pra ter até 8)
- ✅ **Menu simples** com 3 opções: jogar uma peça, adicionar outra ou sair
- ✅ **Cria peças automaticamente** com seus tipos e números únicos
- ✅ **Mostra a fila** toda vez que você faz alguma coisa
- ✅ Código bem explicado e fácil de entender
- ✅ Funciona direitinho no **Windows, Linux e macOS**
- ✅ Exibe acentos corretamente (sim, isso é um desafio em C no Windows! 😅)

---

## 🎮 Como são as peças

Cada peça tem duas informações:

| O que é | Exemplo | Explicação |
|---------|---------|------------|
| **Tipo** | 'T', 'O', 'L' ou 'I' | A forma da peça (aqueles blocos do Tetris) |
| **Número** | 0, 1, 2, 3... | Um número único pra identificar cada peça |

**As peças aparecem sempre nessa ordem:** T → O → L → I → T → O → L → I... (e assim vai! 🔄)

---

## 🔄 Entendendo a fila

Imagina que você tem 8 lugares vazios e vai enchendo com peças:

```
Início: 5 peças já na fila

Fila: [T 0] [O 1] [L 2] [I 3] [T 4] [ ] [ ] [ ]
       ↑ próxima a sair               ↑ aqui entra a próxima
```

### **Como funciona:**
- **Jogar uma peça:** Tira da frente (a primeira que entrou é a primeira que sai!)
- **Adicionar peça:** Entra no final da fila
- **Circular:** Quando chega no fim, volta pro começo (tipo um carrossel 🎠)

É tipo uma fila de banco mesmo: quem chegou primeiro, sai primeiro!

---

## 🚀 Como compilar e executar

### **💻 Windows (PowerShell com GCC/MinGW)**

**Passo 1 - Compilar com suporte a UTF-8:**
```bash
gcc -finput-charset=UTF-8 -fexec-charset=UTF-8 tetris_fila.c -o tetris_fila.exe
```

**Passo 2 - Executar:**
```bash
.\tetris_fila.exe
```

### **🐧 Linux/macOS**

**Passo 1 - Compilar:**
```bash
gcc tetris_fila.c -o tetris_fila
```

**Passo 2 - Executar:**
```bash
./tetris_fila
```

---

## 🎯 Exemplo de uso

```
Fila de peças
[T 0] [O 1] [L 2] [I 3] [T 4]

Opções:
1 - Jogar peça (dequeue)
2 - Inserir nova peça (enqueue)
0 - Sair
Escolha: 1

Jogou peça: [T 0]

Fila de peças
[O 1] [L 2] [I 3] [T 4]

Opções:
1 - Jogar peça (dequeue)
2 - Inserir nova peça (enqueue)
0 - Sair
Escolha: 2

Peça inserida no fim da fila.

Fila de peças
[O 1] [L 2] [I 3] [T 4] [O 5]

Opções:
1 - Jogar peça (dequeue)
2 - Inserir nova peça (enqueue)
0 - Sair
Escolha: 0

Encerrando.
```

---

## 📚 O que você vai aprender

| Conceito | O que é isso na prática |
|----------|-------------------------|
| **Fila circular** | Reutilizar espaços vazios de forma inteligente |
| **FIFO** | First In, First Out = Primeiro a entrar, primeiro a sair |
| **Structs** | Criar seus próprios tipos de dados (tipo uma receita de bolo) |
| **Alocação dinâmica** | Pedir memória pro computador quando precisar |
| **Funções** | Organizar o código em pedacinhos que fazem coisas específicas |
| **Validações** | Checar se tá tudo certo antes de fazer algo (fila cheia? vazia?) |
| **Menu interativo** | Conversar com o programa através do terminal |

---

## 🔧 Principais funções do código

### `fila_init()` - Prepara tudo
Cria a fila do zero:
- Pede memória pro computador guardar as 8 peças
- Começa tudo zerado e pronto pra usar

### `enqueue()` - Adiciona peça no final
**Coloca uma peça nova no fim da fila:**
- Calcula onde é o final da fila
- Coloca a peça lá
- Aumenta o contador de peças
- Avisa se deu certo ou se a fila tá cheia

### `dequeue()` - Remove peça da frente
**Tira a primeira peça da fila:**
- Pega a peça que tá na frente
- Move o "início da fila" uma posição pra frente
- Diminui o contador de peças
- Avisa se deu certo ou se a fila tá vazia

### `gerarPeca()` - Cria peças novas
**Faz as peças aparecerem automaticamente:**
- Segue a ordem: T → O → L → I → T → O...
- Dá um número único pra cada peça
- **Detalhe esperto:** Só cria a peça se tiver espaço na fila!

### `imprimirFila()` - Mostra a fila
Exibe todas as peças na tela:
- Mostra da primeira até a última, em ordem
- Formato: `[tipo número]` pra cada peça
- Se tiver vazia, avisa!

### `fila_free()` - Limpa tudo no final
Libera a memória que foi usada:
- Devolve a memória pro computador
- Deixa tudo arrumadinho antes de sair

---

## ⚙️ Detalhes técnicos

### **Compatibilidade UTF-8 no Windows**
O código inclui funções especiais (`u8printf`, `u8vfprintf`) que:
- Convertem UTF-8 para UTF-16 no console Windows
- Usam `WriteConsoleW` para exibir acentos corretamente
- Mantém compatibilidade total com Linux/macOS

### **Leitura robusta de entrada**
A função `lerInt()`:
- Lê linha completa com `fgets`
- Ignora espaços em branco
- Valida se é número válido
- Retorna `-1` em caso de erro

### **Prevenção de desperdício de IDs**
- Peça só é gerada se houver espaço na fila
- Evita "pular" números na sequência de IDs
- Mantém consistência na numeração

---

## 📦 Arquivos que você vai ter

```
.
├── tetris_fila.c    # O código principal (é onde tá tudo!)
├── README.md        # Esse arquivo aqui que você tá lendo
└── tetris_fila.exe  # O programa compilado (Windows)
```

Simples assim! Um arquivo com o código e pronto. 😊

---

## 🎓 Objetivos de aprendizagem

Este projeto demonstra:
1. **Estrutura de dados fundamental:** Fila (Queue)
2. **Implementação circular:** Reaproveitamento eficiente de memória
3. **Gestão de memória:** Alocação e liberação dinâmica
4. **Modularização:** Separação clara de responsabilidades
5. **Interface de usuário:** Menu interativo simples
6. **Portabilidade:** Código funcional em múltiplos sistemas

---
