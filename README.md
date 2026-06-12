# 🧩 Nonogram em C

Jogo de puzzle **Nonogram** desenvolvido em C com interface gráfica usando a biblioteca **Allegro 5**. Projeto acadêmico desenvolvido para a disciplina de Laboratório de Programação (ELC1065) na **Universidade Federal de Santa Maria (UFSM)**.

---

## 📋 Descrição

O Nonogram é um puzzle lógico onde o jogador deve preencher células de um tabuleiro com base em pistas numéricas dispostas nas linhas e colunas. Cada pista indica a quantidade e sequência de células consecutivas que devem ser pintadas naquela linha ou coluna.

Este projeto implementa um tabuleiro 8×8 com geração aleatória de puzzles e dois modos de jogo distintos.

---

## ✅ Funcionalidades

- 🎲 Geração aleatória de puzzles com validação automática
- 🔢 Cálculo automático das pistas por linha e coluna
- 🏆 Detecção automática de vitória
- 🎮 Dois modos de jogo: **Normal** e **Clássico**
- ↩️ Sistema de **desfazer** jogadas (Modo Clássico)
- ❤️ Sistema de **vidas** com feedback visual de erros (Modo Normal)
- 📊 Contador de células restantes (Modo Clássico)
- 🔄 Botões: Limpar tabuleiro, Novo puzzle, Voltar ao menu

---

## 🛠️ Tecnologias utilizadas

| Tecnologia | Uso |
|---|---|
| **C** | Linguagem principal |
| **Allegro 5** | Renderização gráfica, eventos de mouse e teclado |
---

## 📁 Estrutura do projeto

```
nonogram-c/
├── jogo.c              # Lógica principal do jogo
├── util.c              # Abstração da biblioteca Allegro 5
├── util.h              # Cabeçalho das funções utilitárias
├── Makefile            # Script de compilação
├── data/
│   └── Audiowide-Regular.ttf   # Fonte utilizada na interface
├── docs/
│   └── screenshots/    # Capturas de tela do jogo
└── README.md
```

---

## ⚙️ Como compilar

### Pré-requisitos

- GCC instalado
- Allegro 5 instalado

**No Ubuntu/Debian:**
```bash
sudo apt install gcc make liballegro5-dev
```

**No Arch Linux:**
```bash
sudo pacman -S gcc make allegro
```

### Compilação

```bash
make
```

Ou manualmente:
```bash
gcc -o jogo jogo.c util.c -lallegro -lallegro_main -lallegro_color -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_image -lm
```

---

## ▶️ Como executar

```bash
./jogo
```

> ⚠️ Execute sempre a partir da raiz do projeto para que a fonte em `data/` seja encontrada corretamente.

---

## 📸 Screenshots

### Menu Principal
![Menu Principal](docs/screenshots/menu_nonogram.png)

### Modo Normal — em andamento
![Modo Normal](docs/screenshots/tela_jogando_normal.png)

### Modo Clássico — em andamento
![Modo Clássico](docs/screenshots/tela_jogando_classico.png)

### Tela de Vitória
![Ganhou](docs/screenshots/tela_ganhou.png)

### Tela de Derrota
![Perdeu](docs/screenshots/tela_perdeu.png)

---

## 🎮 Como jogar

1. Ao abrir o jogo, escolha o modo de jogo no menu principal
2. Use as **pistas numéricas** nas bordas do tabuleiro para descobrir quais células pintar
3. **Clique** em uma célula para interagir com ela
4. Resolva o puzzle completando todas as células corretas

### Interpretando as pistas

Cada número indica um grupo de células consecutivas pintadas naquela linha ou coluna. Grupos diferentes são separados por pelo menos uma célula vazia.

**Exemplo:** a pista `3, 1` significa: três células pintadas consecutivas, depois ao menos uma vazia, depois uma célula pintada.

---

## 🕹️ Modos de jogo

### Modo Normal
- Você tem **3 vidas**
- Clicar em uma célula **errada** marca com um ✗ vermelho e perde uma vida
- Clicar em uma célula **correta** a pinta automaticamente
- O jogo termina quando as vidas acabam ou o puzzle é resolvido

### Modo Clássico
- **Sem penalidade** por erros — você decide livremente o que pintar
- Ciclo de estados por clique: `vazio → pintado → marcado (✗) → vazio`
- Botão **Desfazer** disponível para reverter a última jogada
- Contador **"Restam X / Y"** mostra o progresso em relação ao total de células corretas

---

## 📚 Aprendizados do projeto

- Uso de **ponteiros e passagem por referência** em C para gerenciar o estado do jogo entre funções
- Implementação de uma **pilha manual** (`historico[]` + `topo`) para o sistema de desfazer
- Integração com a biblioteca **Allegro 5**: renderização, eventos de mouse e gerenciamento de fontes
- Uso de **enums** para representar estados do jogo

---

## 🔮 Melhorias futuras

- [ ] Tabuleiros de tamanhos variados (5×5, 10×10, 15×15)
- [ ] Sistema de cronômetro
- [ ] Tela de fim de jogo mais elaborada com o tabuleiro resolvido
- [ ] Suporte a clique direito para marcar células diretamente no Modo Normal
- [ ] Níveis de dificuldade com controle de densidade do puzzle
- [ ] Salvamento e carregamento de puzzles

---

## 📄 Licença

Este projeto foi desenvolvido para fins acadêmicos na UFSM. Livre para uso educacional.

---

<p align="center">
  Desenvolvido por <strong>Arthur Trindade da Hora</strong> · UFSM · 2026
</p>
