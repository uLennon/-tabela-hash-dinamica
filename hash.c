#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

float fatorCarga;
int tamanhoTabela;	
int dobra;
int m;
int p = 0;
int l = 0;
int posicaoClientes = -1;
	

typedef struct Cliente{
    int codCliente;
    char nome[100];
    int proximo;
}Cliente;

int menu();
void inserirCliente();
void atribuiTabela();
void salvaDadosTabela();
void lerDadosTabela();
int tamanhoCliente();
int hash(int chave, int tamanho, int l);
int hashl(int chave, int tamanho, int l);
int verificaArquivo(FILE *arquivo);
float calcularFatorCarga();
int atualizaTabelaExpandida(FILE *arqTabela, FILE* arqCliente, int codigo, int posicao);
int expandirTabela();
int tratamentoColisao(Cliente *cliente);
void inserir(Cliente *cliente);
void busca (int chave);
void dobraTabela();
void criaTabela(int tamanho);
void printaTabela();
void printaTabelaCliente();


int main(int argc, char const *argv[]){
	menu();
	return 0;
}

int menu(){
	int opcao;
	printf("-------------------Tabela Hash Dinamica-------------------\n");
	printf("Escolha uma opcao:\n");
	printf("1 - Criar uma tabela.\n");
	printf("2 - Exibir uma tabela.\n");
	printf("0 - Sair\n");
	scanf("%d",&opcao);
	if(opcao == 1){
		FILE *arqCliente = fopen("clientes.bin","wb");
		fclose(arqCliente);
		int escolha = 1;
		atribuiTabela();
		printf("\n\t\t\t Tabela criada \n\n");
		do{
			int op;
		    printf("1 - Inserir um cliente\n");
		    printf("2 - Buscar um cliente\n");
		    printf("3 - Exibir os clientes\n");
		    printf("0 - sair\n");
		    printf("Escolha uma opcao: ");
		    scanf("%d", &escolha);
		    switch(escolha){
		    	case 1:
		    		inserirCliente();
		    		break;
		    	case 2:
		    		int codigo;
			    	printf("Digite o codigo do cliente que deseja buscar: " );
			    	scanf("%d",&codigo);
			    	busca(codigo);
			    	break;
			    case 3: 
			    	printaTabelaCliente();
			    	break;
			    case 0: 
			    	break;
			    default:
            		printf("Opcao invalida.\n");
            		break;
		    }
		}while(escolha != 0);
		salvaDadosTabela();
	}else if(opcao == 2){
		lerDadosTabela();
		printf("------------------------Tabela---------------------------\n");
		printaTabela();
		printf("---------------------------------------------------------\n");
		printaTabelaCliente();
		salvaDadosTabela();
	}
	else{
		exit(1); 
	}
}

void inserirCliente(){
	printf("Digite o codigo do cliente: ");
	Cliente c1 ;
	scanf("%d",&c1.codCliente);
	c1.proximo = -1;
	printf("Digite o nome do cliente: ");
	scanf("%s", c1.nome);
	inserir(&c1);
	printf("---> Cliente: %s  com o codigo: %d [Adicionado]\n", c1.nome, c1.codCliente);
}

void atribuiTabela(){
	printf("Digite o tamanho da tabela: ");
	scanf("%d",&m);
	printf("Digite o fator de carga: ");
	scanf("%f",&fatorCarga);
    tamanhoTabela = m;
    dobra = m;
    criaTabela(m);
}

void salvaDadosTabela(){
	FILE *arqDados = fopen("dados.bin","wb");
	if(arqDados == NULL){
		printf("Erro no salvamento do arquivo Dados\n");
	}else{
		fwrite(&posicaoClientes,sizeof(int),1,arqDados); 
		fwrite(&tamanhoTabela ,sizeof(int),1,arqDados); 
		fwrite(&fatorCarga ,sizeof(float),1,arqDados);	
		fwrite(&dobra ,sizeof(int),1,arqDados);	
		fwrite(&m ,sizeof(int),1,arqDados);
		fwrite(&p ,sizeof(int),1,arqDados);
		fwrite(&l ,sizeof(int),1,arqDados);
		fclose(arqDados);
	}
}

void lerDadosTabela(){
	FILE *arqDados = fopen("dados.bin","rb");
	if(arqDados == NULL){
		printf("Erro na abertura do arquivo Dados\n");
	}else{
		fread(&posicaoClientes,sizeof(int),1,arqDados); 
		fread(&tamanhoTabela ,sizeof(int),1,arqDados); 
		fread(&fatorCarga ,sizeof(float),1,arqDados);	
		fread(&dobra ,sizeof(int),1,arqDados);	
		fread(&m ,sizeof(int),1,arqDados);
		fread(&p ,sizeof(int),1,arqDados);
		fread(&l ,sizeof(int),1,arqDados);
		fclose(arqDados);
	}
}

int tamanhoCliente(){
	return sizeof(int)
		   + (sizeof(char) * 100)
		   + sizeof(int);
}

int hash(int chave, int tamanho, int l){
    int calculo = tamanho * pow(2,l);
    return chave % calculo;
}

int hashl(int chave, int tamanho, int l){
    l = l +1;
    int calculo = tamanho * pow(2,l);
    return chave % calculo;
}

int verificaArquivo(FILE *arquivo){
	if (arquivo == NULL){
		printf("Erro ao abrir o arquivo");
		return 1;
	}
	return 0;
}

float calcularFatorCarga(){
	if(posicaoClientes/tamanhoTabela >= fatorCarga){
		return 1;
	}
	return 0;
}
	
int atualizaTabelaExpandida(FILE *arqTabela, FILE* arqCliente, int codigo, int posicao){
	int posicaoHash = hashl(codigo,m,l); 
	fseek(arqCliente,posicao*tamanhoCliente(),SEEK_SET);
	int endereco;  
    fseek(arqTabela,posicaoHash*sizeof(int),SEEK_SET); 
    fread(&endereco, sizeof(int), 1, arqTabela);

    if (endereco != -1) { 
        fseek(arqCliente, endereco * tamanhoCliente(), SEEK_SET);
        Cliente *cliente = (Cliente *)malloc(sizeof(Cliente));
        fread(cliente, sizeof(Cliente), 1, arqCliente);
        while(cliente->proximo != -1){ 
            endereco = cliente->proximo;
            fseek(arqCliente, endereco * tamanhoCliente(), SEEK_SET);
            fread(cliente, sizeof(Cliente), 1, arqCliente);
            if (cliente->codCliente == codigo) {  
                free(cliente);
                return 0;
            }
        }
        cliente->proximo = posicao;
        fseek(arqCliente, endereco * tamanhoCliente(), SEEK_SET);
        fwrite(cliente, sizeof(Cliente), 1, arqCliente);
        fseek(arqCliente, posicao*tamanhoCliente(),SEEK_SET);
       	Cliente *clienteAlterado = (Cliente *) malloc(sizeof(Cliente));
       	fread(clienteAlterado,sizeof(Cliente), 1, arqCliente);
       	clienteAlterado->proximo = -1;
       	fseek(arqCliente, posicao*tamanhoCliente(),SEEK_SET);
       	fwrite(clienteAlterado,sizeof(Cliente),1,arqCliente);
        free(cliente);
        free(clienteAlterado);
    }
    else {  
        fseek(arqTabela, posicaoHash * sizeof(int), SEEK_SET);  
        fwrite(&posicao, sizeof(int), 1, arqTabela);
       	fseek(arqCliente, posicao*tamanhoCliente(),SEEK_SET);
       	Cliente *clienteAlterado = (Cliente *) malloc(sizeof(Cliente));
       	fread(clienteAlterado,sizeof(Cliente), 1, arqCliente);
       	clienteAlterado->proximo = -1;
       	fseek(arqCliente, posicao*tamanhoCliente(),SEEK_SET);
       	fwrite(clienteAlterado,sizeof(Cliente),1,arqCliente);
        free(clienteAlterado);
    }
	return 0;
}

int expandirTabela(){
	if(calcularFatorCarga()){
		FILE *arqTabela = fopen("tabela.bin", "rb+");   
		if(verificaArquivo(arqTabela)){					  
			printf(" na tabela \n");					
		}
		fseek(arqTabela,0,SEEK_END);
		int numero = -1;
		fwrite(&numero, sizeof(int),1, arqTabela);
		tamanhoTabela++;
		FILE *arqCliente = fopen("clientes.bin","rb+");
		if(verificaArquivo(arqCliente)){
			printf(" nos clientes \n");
		}
		fseek(arqTabela,p*sizeof(int),SEEK_SET);
		int numeroTabela;
		fread(&numeroTabela,sizeof(int),1, arqTabela); 

		if(numeroTabela == -1){
			p++;
			return 1;
		}
		else{
			int limpa = -1;  
			fseek(arqCliente,numeroTabela*tamanhoCliente(),SEEK_SET);
			Cliente *cliente = (Cliente *)malloc(sizeof(Cliente));
			fread(cliente,sizeof(Cliente),1,arqCliente);
			fseek(arqTabela,p*sizeof(int),SEEK_SET);
			fwrite(&limpa,sizeof(int),1,arqTabela);
				while(cliente->proximo != -1){
					atualizaTabelaExpandida(arqTabela,arqCliente,cliente->codCliente,numeroTabela);
					numeroTabela = cliente->proximo;
					fseek(arqCliente,numeroTabela*tamanhoCliente(),SEEK_SET);
					fread(cliente,sizeof(Cliente),1,arqCliente);
				}	
			atualizaTabelaExpandida(arqTabela,arqCliente,cliente->codCliente,numeroTabela); 
			free(cliente);
		}									
		fclose(arqCliente);
		fclose(arqTabela);
		p++;
	}
	return 0;
}

int tratamentoColisao(Cliente *cliente){
	FILE *arqTabela = fopen("tabela.bin", "rb+");
	if(verificaArquivo(arqTabela)){
		printf(" na tabela \n");
	}
	FILE *arqCliente = fopen("clientes.bin","rb+");
	if(verificaArquivo(arqCliente)){
		printf(" nos clientes \n");
	}
	int posicaoHash = hash(cliente->codCliente,m,l);   
	fseek(arqTabela,posicaoHash*sizeof(int),SEEK_SET);
	int numeroTabela;
	fread(&numeroTabela,sizeof(int),1, arqTabela);
	if(numeroTabela != -1) {
		fseek(arqCliente,numeroTabela*tamanhoCliente(),SEEK_SET);   
		Cliente *c1 = (Cliente *) malloc(sizeof(Cliente));
		fread(c1,sizeof(Cliente),1,arqCliente);  
		int posicaoClienteAnterior = numeroTabela;

    	while(c1->proximo != -1 ){
    		posicaoClienteAnterior = c1->proximo;
    		fseek(arqCliente,posicaoClienteAnterior*tamanhoCliente(),SEEK_SET);   
    		fread(c1,sizeof(Cliente),1,arqCliente); 
    	}
    	fseek(arqCliente, 0, SEEK_END);  
    	fwrite(cliente, sizeof(Cliente), 1, arqCliente); 
    	posicaoClientes++;								 
    	c1->proximo = posicaoClientes;
    	fseek(arqCliente, posicaoClienteAnterior*tamanhoCliente(),SEEK_SET); 
    	fwrite(c1,sizeof(Cliente),1,arqCliente); 

    	fclose(arqCliente);
		fclose(arqTabela);
		free(c1);
		return 0;
	}
	fclose(arqCliente);
	fclose(arqTabela);
	return 1;
}

void inserir(Cliente *cliente){
	expandirTabela();
	dobraTabela();
	int chave = hash(cliente->codCliente,m,l);
	int achou = tratamentoColisao(cliente);
	
	if(achou != 0){			
		posicaoClientes++;
		FILE *arqCliente = fopen("clientes.bin","ab");
		fwrite(cliente, sizeof(Cliente), 1, arqCliente);
		fclose(arqCliente);
	
		FILE *arqTabela = fopen("tabela.bin", "rb+");
		fseek(arqTabela,chave*sizeof(int),SEEK_SET);
		fwrite(&posicaoClientes, sizeof(int),1, arqTabela);
		fclose(arqTabela);
	}
}

void busca(int chave){
	FILE *arqTabela = fopen("tabela.bin", "rb");
	if(verificaArquivo(arqTabela)){
		printf(" na tabela \n");
	}
	FILE *arqCliente = fopen("clientes.bin","rb");
	if(verificaArquivo(arqCliente)){
		printf(" nos clientes \n");
	}
	int posicaoTabela = hash(chave,m,l);
	if(posicaoTabela < p){
		posicaoTabela = hashl(chave,m,l);
	}
	if(fseek(arqTabela,posicaoTabela*sizeof(int),SEEK_SET)!= 0){
		printf("Erro na busca da tabela\n");
	}
	int endereco;
	fread(&endereco,sizeof(int),1,arqTabela);
	if(fseek(arqCliente,endereco*tamanhoCliente(),SEEK_SET) != 0){
		printf("Erro na busca da tabela Clientes\n");
	}
	Cliente cliente;
	fread(&cliente,sizeof(Cliente),1,arqCliente);

	if(cliente.codCliente == chave){
		printf("Cliente %s  -- [%d] encontrado\n", cliente.nome, cliente.codCliente);
	}
	if(cliente.codCliente != chave){
		while(cliente.proximo != -1 && chave != cliente.codCliente){
			fseek(arqCliente,cliente.proximo*tamanhoCliente(),SEEK_SET);   
    		fread(&cliente,sizeof(Cliente),1,arqCliente); 
		}
		if(cliente.codCliente == chave){
			printf("[%d] : %s  \n", cliente.codCliente,cliente.nome);
		}
		else{
			printf("Cliente nao encontrado\n");
		}
	}
	fclose(arqCliente);
	fclose(arqTabela);
}

void dobraTabela(){
	if(p == dobra){
		l++;
		p = 0;
		dobra = tamanhoTabela;
	}
}

void criaTabela(int tamanho) {
    FILE *arquivo;
    int numero=-1;
    arquivo = fopen("tabela.bin", "w+b");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
    }
    for (int i = 0; i < tamanho; i++) {
        if(fwrite(&numero, sizeof(int), 1, arquivo) != 1){
        	printf("Error ao gravar\n");
        }
    }
    fclose(arquivo);
}

void printaTabela(){
	FILE *arquivo = fopen("tabela.bin","rb");
	fseek(arquivo,0,SEEK_SET);
    if(arquivo == NULL){
        printf("Erro no arquivo\n");
    }else{
    	int numero;
	    while(fread(&numero, sizeof(int), 1, arquivo) == 1) {
	        printf("%d\n", numero);   
	    }
	}
	fclose(arquivo);
}

void printaTabelaCliente(){
	FILE *arquivo = fopen("clientes.bin","rb");
	fseek(arquivo,0,SEEK_SET);
    if(arquivo == NULL){
        printf("Erro no arquivo\n");
    }else{
    	Cliente cliente;
    	int numero = 0;
	    while(fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
	        printf("Posicao %d: Nome do Cliente: %s | Codigo: %d  |  proximo  %d  \n",numero, cliente.nome,cliente.codCliente,cliente.proximo);   
	    	numero++;
	    }
	}
	fclose(arquivo);
}