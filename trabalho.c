/* ========================================================================== *
 * LEGENDA DE DECLARAÇÃO DE USO DE LLM (Inteligência Artificial)              *
 * Trabalho Final - Estrutura de Dados I (2026-2)                             *
 * ========================================================================== *
 * Este programa utilizou auxílio de Inteligência Artificial (LLM) como       *
 * ferramenta de apoio educacional e de desenvolvimento. 
=============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- ESTRUTURAS ---

typedef struct usuario {
    char name[50];
    char email[50];
    struct usuario *prev, *next; 
} User;

typedef struct sentinela {
    User *first, *tail;           
    int qtd;
} sent;

typedef struct book {
    char title[50];
    char author[50];
    int year;
    int id;
    int status; // 0 = disponível, 1 = emprestado
    char email[50]; // email pertencente ao livro
    struct book *prev, *next;
} Book;

// --- ESTRUTURA AUXILIAR DE ÁRVORE PARA BUSCA OTIMIZADA POR ID ---

typedef struct arvore {
    int id;
    Book *bookPtr; // Ponteiro direto para o livro na lista encadeada
    struct arvore *right, *left;
} bst;

typedef struct tree_sentinela {
    bst* root;
    int qtd;
} tree; 

// Estrutura da Biblioteca
typedef struct library {
    Book *first, *tail;
    tree *treeID;
    int qtd;
} Library;

// --- FUNÇÕES DE MANIPULAÇÃO DA ÁRVORE ---

bst* createNode(int id, Book *bookPtr) {
    bst* Node = (bst*) malloc(sizeof(bst));
    if (Node == NULL) return NULL;
    Node->id = id;
    Node->bookPtr = bookPtr;
    Node->right = NULL;
    Node->left = NULL;
    return Node;
}

void insertTree(tree *t, int id, Book *bookPtr) {
    bst* node = createNode(id, bookPtr);
    if (node == NULL) return;

    if (t->root == NULL) {
        t->root = node;
        t->qtd++;
        return;
    }

    bst* aux = t->root;
    bst* parent = NULL;

    while (aux != NULL) {
        parent = aux;
        if (id >= aux->id) {
            aux = aux->right;
        } else {
            aux = aux->left;
        }
    }

    if (node->id < parent->id) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    t->qtd++;
}

// Retorna diretamente o ponteiro do livro se encontrado na BST O(log n)
Book* searchBst(tree* s, int id) {
    if (s == NULL) return NULL;
    bst* aux = s->root;
    while (aux != NULL) {
        if (aux->id == id) {
            return aux->bookPtr;
        }
        if (id < aux->id) {
            aux = aux->left;
        } else {
            aux = aux->right;
        }
    }
    return NULL;
}


tree *excludeInBst(tree *n, int v)
{
    if (n == NULL || n->root == NULL) return n;
    
    bst *aux = n->root, *parent = aux;
    while (aux != NULL) {
        if (v < aux->id) {
            parent = aux; 
            aux = aux->left;
        } else if (v > aux->id) {
            parent = aux;
            aux = aux->right;
        } else {
            break;
        }
    }
    
    if (aux == NULL) return n; // não encontrou o dado, retorna a raiz atual

    // Caso 1: Nó sem filhos
    if (aux->left == NULL && aux->right == NULL) {
        if (aux == n->root) { 
            free(aux);
            return NULL; // Árvore ficou vazia
        }

        if (aux->id <= parent->id) parent->left = NULL;
        else                       parent->right = NULL;
        free(aux);
        
        return n; // Retorna a raiz inalterada
    }
    
    // Caso 2: Nó com apenas um filho
    if (aux->left == NULL || aux->right == NULL) {
        bst *sub = (aux->left != NULL) ? aux->left : aux->right;
    
        if (aux == n->root) { // CORRIGIDO: Compara com n->root
            free(aux);
            n->root = sub;
            return n; // O filho se torna a nova raiz
        }
    
        if (aux == parent->left) {
            parent->left = sub;
        } else {
            parent->right = sub;
        }
    
        free(aux);
        return n; // Retorna a raiz atual
    }

    // Caso 3: Nó com dois filhos
    if (aux->left != NULL && aux->right != NULL) {
        bst *succParent = aux;
        bst *succ = aux->right;
    
        while (succ->left != NULL) {
            succParent = succ;
            succ = succ->left;
        }
    
        aux->id = succ->id;
        aux->bookPtr = succ->bookPtr;
    
        if (succ == succParent->left) {
            succParent->left = succ->right;
        } else {
            succParent->right = succ->right;
        }
    
        free(succ);
        return n; // Retorna a raiz atual
    }

    return n;
}





void freeBST(bst *node) {
    if (node != NULL) {
        freeBST(node->left);
        freeBST(node->right);
        free(node);
    }
}

// --- FUNÇÕES AUXILIARES DE STRING (Com string.h) ---

int compareSTR(char* x, char* y) {
    // strcasecmp compara ignorando maiúsculas/minúsculas (disponível com string.h / c.std)
    // Caso prefira strcmp sensível a maiúsculas, basta trocar.
    return strcasecmp(x, y) > 0 ? 1 : 0;
}

int strEqual(char *a, char *b) {
    return strcasecmp(a, b) == 0;
}

// --- CRIAÇÃO ---

User* createUser() {
    User *x = (User*) malloc(sizeof(User));
    
    if (x == NULL) return NULL;

    // 3. Esvazia o buffer do teclado (apaga o Enter sobrando).
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    
    printf("Nome: ");
    fgets(x->name, sizeof(x->name), stdin); //fgets aceita espaços
    
    // 5. Apaga a quebra de linha (\n) do final do nome.
    x->name[strcspn(x->name, "\n")] = '\0';

    printf("Email: ");
    fgets(x->email, sizeof(x->email), stdin);
    
    // 7. Apaga a quebra de linha (\n) do final do email.
    x->email[strcspn(x->email, "\n")] = '\0';

    // 8. Desconecta os ponteiros da lista (inicializa em NULL).
    x->prev = NULL;
    x->next = NULL;
    
    return x;
}

Book* createBook() {
    Book *x = (Book*) malloc(sizeof(Book));
    if (x == NULL) return NULL;

    // Limpa qualquer resíduo pendente no buffer do teclado antes de começar
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Titulo:\n");
    fgets(x->title, sizeof(x->title), stdin);
    x->title[strcspn(x->title, "\n")] = '\0'; // Remove o '\n' de forma limpa com string.h

    printf("Autor:\n");
    fgets(x->author, sizeof(x->author), stdin);
    x->author[strcspn(x->author, "\n")] = '\0';

    printf("Ano de publicacao:\n");
    scanf("%d", &x->year);

    printf("Codigo (ID):\n");
    scanf("%d", &x->id);

    x->status = 0; 
    x->email[0] = '\0';
    x->prev = NULL;
    x->next = NULL;

    return x;
}

// --- CADASTRO ---

void registerBook(Library *n) {
    Book *x = createBook();

    if (x == NULL) return;

    // Verificação rápida usando a Árvore Binária
    if (searchBst(n->treeID, x->id) != NULL) {
        printf("Erro: Ja existe um livro cadastrado com este ID (verificado via BST)!\n");
        free(x);
        return;
    }

    // Inserção ordenada por ID na lista duplamente encadeada
    if (n->first == NULL) {
        n->first = x;
        n->tail = x;
    } else if (x->id < n->first->id) {
        x->next = n->first;
        n->first->prev = x;
        n->first = x;
    } else {
        Book *aux = n->first;
        while (aux != NULL && aux->id < x->id) {
            aux = aux->next;
        }

        if (aux == NULL) {
            x->prev = n->tail;
            n->tail->next = x;
            n->tail = x;
        } else {
            x->next = aux;
            x->prev = aux->prev;
            aux->prev->next = x;
            aux->prev = x;
        }
    }

    // Insere na BST passando o ID e o ponteiro direto para o livro criado
    insertTree(n->treeID, x->id, x); 
    n->qtd++;

    printf("Livro cadastrado com sucesso!\n");
}

void registerUser(sent* s) {
    User* x = createUser();
    if (x == NULL) {
        printf("Falha de alocacao!\n");
        return;
    }

    User* aux_check = s->first;
    while (aux_check != NULL) {
        if (strEqual(aux_check->email, x->email)) {
            printf("Erro: Ja existe um usuario cadastrado com este email!\n");
            free(x);
            return;
        }
        aux_check = aux_check->next;
    }

    if (s->first == NULL) {
        s->first = x;
        s->tail = x;
    } else if (compareSTR(x->name, s->first->name) == 0) {
        x->next = s->first;
        s->first->prev = x;
        s->first = x;
    } else {
        User* aux = s->first;
        while (aux != NULL && compareSTR(x->name, aux->name) == 1) {
            aux = aux->next;
        }

        if (aux == NULL) {
            x->prev = s->tail;
            s->tail->next = x;
            s->tail = x;
        } else {
            x->next = aux;
            x->prev = aux->prev;
            aux->prev->next = x;
            aux->prev = x;
        }
    }

    s->qtd++;
    printf("Usuario cadastrado com sucesso!\n");
}

// --- BUSCAS OTIMIZADAS ---

Book* searchCode(Library *s, int code) {
    Book* aux = searchBst(s->treeID, code);
    if (aux != NULL) {
        printf("\n--- Livro Encontrado (via BST O(log n)) ---\n");
        printf("Titulo: %s\n", aux->title);
        printf("Autor: %s\n", aux->author);
        printf("Ano: %d\n", aux->year);
        printf("ID: %d\n", aux->id);
        printf("Status: %s\n", aux->status == 0 ? "Disponivel" : "Emprestado");
        if (aux->status == 1) {
            printf("Emprestado para: %s\n", aux->email);
        }
        return aux;
    }
    printf("Livro não encontrado\n");
    return NULL;
}

void searchAuthor(Library *s, char* authorName) {
    Book* aux = s->first;
    int encontrado = 0;
    
    printf("\n--- Buscando livros do autor: %s ---\n", authorName);
    while (aux != NULL) {
        if (strEqual(aux->author, authorName)) {
            printf("- ID: %d | Titulo: %s (Ano: %d)\n", aux->id, aux->title, aux->year);
            encontrado = 1;
        }
        aux = aux->next;
    }
    
    if (encontrado == 0) {
        printf("Livro não encontrado\n");
    }
}

User* searchEmail(sent* s, char *email) {
    User* aux = s->first;

    printf("\n--- Buscando Usuario com o email: %s ---\n", email);
    while (aux != NULL) {
        if (strEqual(aux->email, email)) {
            printf("- Nome: %s | Email: %s\n", aux->name, aux->email);
            return aux;
        }
        aux = aux->next;
    }

    printf("Usuario não cadastrado\n");
    return NULL;
}

User* searchName(sent* s, char *name) {
    User* aux = s->first;

    printf("\n--- Buscando Usuario com o nome: %s ---\n", name);
    while (aux != NULL) {
        if (strEqual(aux->name, name)) {
            printf("- Nome: %s | Email: %s\n", aux->name, aux->email);
            return aux;
        }
        aux = aux->next;
    }

    printf("Usuario não cadastrado\n");
    return NULL;
}

void searchEmprestimosUser(Library *lib, sent *users, char* email) {
    User* u = users->first;
    int usuarioExiste = 0;
    while (u != NULL) {
        if (strEqual(u->email, email)) {
            usuarioExiste = 1;
            break;
        }
        u = u->next;
    }

    if (!usuarioExiste) {
        printf("Usuario não cadastrado\n");
        return;
    }

    Book* aux = lib->first;
    int temLivros = 0;
    
    printf("\n--- Livros em posse do usuario (%s) ---\n", email);
    while (aux != NULL) {
        if (aux->status == 1 && strEqual(aux->email, email)) {
            printf("- ID: %d | Titulo: %s (Autor: %s)\n", aux->id, aux->title, aux->author);
            temLivros = 1;
        }
        aux = aux->next;
    }

    if (!temLivros) {
        printf("Nenhum livro emprestado no momento para este usuario.\n");
    }
}

// --- ATUALIZAÇÃO ---

void updateBook(Library* s, int id) {
    Book* aux = searchBst(s->treeID, id);
    if (aux != NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        printf("Digite o novo titulo do livro: ");
        fgets(aux->title, sizeof(aux->title), stdin);
        aux->title[strcspn(aux->title, "\n")] = '\0';

        printf("Digite o novo nome do autor: ");
        fgets(aux->author, sizeof(aux->author), stdin);
        aux->author[strcspn(aux->author, "\n")] = '\0';

        printf("Digite o novo ano de publicacao: ");
        scanf("%d", &aux->year);

        printf("Livro atualizado com sucesso!\n");
        return;
    }
    printf("ID não encontrado!\n");
}

void updateUser(sent* s, char* email) {
    User* aux = s->first;
    while (aux != NULL) {
        if (strEqual(aux->email, email)) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Novo nome: ");
            fgets(aux->name, sizeof(aux->name), stdin);
            aux->name[strcspn(aux->name, "\n")] = '\0';

            printf("Novo email: ");
            fgets(aux->email, sizeof(aux->email), stdin);
            aux->email[strcspn(aux->email, "\n")] = '\0';

            printf("Usuario atualizado com sucesso!\n");
            return;
        }
        aux = aux->next;
    }
    printf("Email não encontrado!\n");
}

// --- EXCLUSÃO ---

void excludeForID(Library* s, int id) {
    if (s->first == NULL) {
        printf("A biblioteca esta vazia\n");
        return;
    }

    // 1. Primeiro buscamos o livro
    Book* aux = searchBst(s->treeID, id);

    
    if (aux == NULL) {
        printf("Livro com o ID %d não foi encontrado.\n", id);
        return;
    }

    
    if (aux->status == 1) {
        printf("O livro está emprestado no momento. Espere sua devolução para a exclusão.\n");
        return;
    }

    
    if (aux == s->first) {
        s->first = aux->next;
        if (s->first != NULL) s->first->prev = NULL;
        else s->tail = NULL;
    } else if (aux == s->tail) {
        s->tail = aux->prev;
        s->tail->next = NULL;
    } else {
        aux->prev->next = aux->next;
        aux->next->prev = aux->prev;
    }

    free(aux); 
    s->qtd--;
    
    // 5. Remove da BST e atualiza contadores
    s->treeID = excludeInBst(s->treeID, id);
    if (s->treeID->qtd > 0) s->treeID->qtd--;

    printf("Exclusao bem sucedida!\n");
}

void excludeForEmail(sent* s, char* email) {
    if (s->first == NULL) {
        printf("A lista de usuarios esta vazia\n");
        return;
    }

    User* aux = s->first;
    while (aux != NULL) {
        if (strEqual(aux->email, email)) {
            if (aux == s->first) {
                s->first = aux->next;
                if (s->first != NULL) s->first->prev = NULL;
                else s->tail = NULL;
            } else if (aux == s->tail) {
                s->tail = aux->prev;
                s->tail->next = NULL;
            } else {
                aux->prev->next = aux->next;
                aux->next->prev = aux->prev;
            }

            free(aux); 
            s->qtd--;
            printf("Usuario excluido com sucesso!\n");
            return;
        }
        aux = aux->next;
    }
    printf("Usuario com o email %s não foi encontrado.\n", email);
}

// --- EMPRÉSTIMO E DEVOLUÇÃO ---

void loanBook(Library* s, sent* users, int id) {    
    Book* aux = searchBst(s->treeID, id);
    if (aux != NULL) {
        if (aux->status != 0) {
            printf("O Livro ja esta emprestado!\n");
            return;
        } else {
            char emailTemp[50];
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Email do usuario que pegou emprestado: ");
            fgets(emailTemp, sizeof(emailTemp), stdin);
            emailTemp[strcspn(emailTemp, "\n")] = '\0';

            User* u = users->first;
            int existe = 0;
            while (u != NULL) {
                if (strEqual(u->email, emailTemp)) {
                    existe = 1;
                    break;
                }
                u = u->next;
            }

            if (!existe) {
                printf("Erro: Usuario não cadastrado! Emprestimo cancelado.\n");
                return;
            }

            aux->status = 1; 
            strcpy(aux->email, emailTemp); // Copia limpa usando string.h

            printf("Livro emprestado com sucesso!\n");
            return;
        }
    }
    printf("Livro não encontrado!\n");
}

void returnBook(Library *s, int id) {
    Book* aux = searchBst(s->treeID, id);
    if (aux != NULL) {
        if (aux->status == 0) {
            printf("O Livro não esta emprestado!\n");
            return;
        } else {
            aux->status = 0; 
            aux->email[0] = '\0'; 
            printf("Livro devolvido com sucesso!\n");
            return;
        }
    }
    printf("Livro não encontrado!\n");
}

// --- LIMPEZA DE MEMÓRIA ---

void freeLibrary(Library *lib) {
    Book *aux = lib->first;
    while (aux != NULL) {
        Book *temp = aux;
        aux = aux->next;
        free(temp); 
    }

    if (lib->treeID != NULL) {
        freeBST(lib->treeID->root);
        free(lib->treeID);
    }

    lib->first = NULL;
    lib->tail = NULL;
    lib->treeID = NULL;
    lib->qtd = 0;
}

void freeUsers(sent *users) {
    User *aux = users->first;
    while (aux != NULL) {
        User *temp = aux;
        aux = aux->next;
        free(temp); 
    }
    users->first = NULL;
    users->tail = NULL;
    users->qtd = 0;
}

// --- FUNÇÃO MAIN ---

int main() {
    tree *arvoreIds = (tree*) malloc(sizeof(tree));
    arvoreIds->root = NULL;
    arvoreIds->qtd = 0;

    Library lib = {NULL, NULL, arvoreIds, 0};
    sent users = {NULL, NULL, 0};

    int opcao = -1;
    int subOpcao;
    int id, tipo;
    char buffer[50];

    while (opcao != 0) {
        printf("\n=== SISTEMA DE CONTROLE DE BIBLIOTECA ===\n");
        printf("1. Cadastro\n");
        printf("2. Consulta\n");
        printf("3. Atualizacao\n");
        printf("4. Exclusao\n");
        printf("5. Emprestimo\n");
        printf("6. Devolucao\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            subOpcao = -1;
            while (subOpcao != 0) {
                printf("\n-- MENU CADASTRO --\n");
                printf("1. Livros\n");
                printf("2. Usuarios\n");
                printf("0. Voltar\n");
                printf("Escolha: ");
                scanf("%d", &subOpcao);

                if (subOpcao == 1) registerBook(&lib);
                else if (subOpcao == 2) registerUser(&users);
            }
        }
        else if (opcao == 2) {
            subOpcao = -1;
            while (subOpcao != 0) {
                printf("\n-- MENU CONSULTA --\n");
                printf("1. Livros\n");
                printf("2. Usuarios\n");
                printf("3. Emprestimos de um usuario\n");
                printf("0. Voltar\n");
                printf("Escolha: ");
                scanf("%d", &subOpcao);

                if (subOpcao == 1) {
                    printf("1. Buscar por Codigo (ID)\n2. Buscar por Autor\nEscolha: ");
                    scanf("%d", &tipo);
                    if (tipo == 1) {
                        printf("Digite o ID: ");
                        scanf("%d", &id);
                        searchCode(&lib, id);
                    } else if (tipo == 2) {
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                        printf("Digite o autor: ");
                        fgets(buffer, sizeof(buffer), stdin);
                        buffer[strcspn(buffer, "\n")] = '\0';
                        searchAuthor(&lib, buffer);
                    }
                }
                else if (subOpcao == 2) {
                    printf("1. Buscar por Email\n2. Buscar por Nome\nEscolha: ");
                    scanf("%d", &tipo);
                    if (tipo == 1) {
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                        printf("Digite o email: ");
                        fgets(buffer, sizeof(buffer), stdin);
                        buffer[strcspn(buffer, "\n")] = '\0';
                        searchEmail(&users, buffer);
                    } else if (tipo == 2) {
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                        printf("Digite o nome: ");
                        fgets(buffer, sizeof(buffer), stdin);
                        buffer[strcspn(buffer, "\n")] = '\0';
                        searchName(&users, buffer);
                    }
                }
                else if (subOpcao == 3) {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                    printf("Digite o email do usuario: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    buffer[strcspn(buffer, "\n")] = '\0';
                    searchEmprestimosUser(&lib, &users, buffer);
                }
            }
        }
        else if (opcao == 3) {
            subOpcao = -1;
            while (subOpcao != 0) {
                printf("\n-- MENU ATUALIZACAO --\n");
                printf("1. Livros\n");
                printf("2. Usuarios\n");
                printf("0. Voltar\n");
                printf("Escolha: ");
                scanf("%d", &subOpcao);

                if (subOpcao == 1) {
                    printf("Digite o ID do livro a atualizar: ");
                    scanf("%d", &id);
                    updateBook(&lib, id);
                }
                else if (subOpcao == 2) {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                    printf("Digite o email do usuario a atualizar: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    buffer[strcspn(buffer, "\n")] = '\0';
                    updateUser(&users, buffer);
                }
            }
        }
        else if (opcao == 4) {
            subOpcao = -1;
            while (subOpcao != 0) {
                printf("\n-- MENU EXCLUSAO --\n");
                printf("1. Livros\n");
                printf("2. Usuarios\n");
                printf("0. Voltar\n");
                printf("Escolha: ");
                scanf("%d", &subOpcao);

                if (subOpcao == 1) {
                    printf("Digite o ID do livro a excluir: ");
                    scanf("%d", &id);
                    excludeForID(&lib, id);
                }
                else if (subOpcao == 2) {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                    printf("Digite o email do usuario a excluir: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    buffer[strcspn(buffer, "\n")] = '\0';
                    excludeForEmail(&users, buffer);
                }
            }
        }
        else if (opcao == 5) {
            printf("Digite o ID do livro para emprestimo: ");
            scanf("%d", &id);
            loanBook(&lib, &users, id);
        }
        else if (opcao == 6) {
            printf("Digite o ID do livro para devolucao: ");
            scanf("%d", &id);
            returnBook(&lib, id);
        }
        else if (opcao == 0) {
            freeLibrary(&lib);
            freeUsers(&users);
            printf("\nEncerrando o programa e liberando a memoria com sucesso!\n");
        }
    }

    return 0;
}