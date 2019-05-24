#include<stdio.h>
#include<stdlib.h>
#include <locale.h>

void print(int x[][10],int n,int m){
	int i,j;
	for(i=0;i<n;i++){
		printf("\n");
		for(j=0;j<m;j++){
			printf("%d\t",x[i][j]);
		}
	}
}

//Algoritmo de requisi��o de recursos
void res_request(int A[10][10],int N[10][10],int AV[10][10],int pid,int m)
{
	int reqmat[1][10];
	int i;
	printf("\n Entre com requisi��o adicional :- \n");
	for(i=0;i<m;i++){
		printf(" Requisi��o para o recurso %d : ",i+1);
		scanf("%d",&reqmat[0][i]);
	}

	for(i=0;i<m;i++)
		if(reqmat[0][i] > N[pid][i]){
			printf("\n Erro encontrado.\n");
			exit(0);
	}

	for(i=0;i<m;i++)
		if(reqmat[0][i] > AV[0][i]){
			printf("\n Recursos dispon�veis.\n");
			exit(0);
		}

	for(i=0;i<m;i++){
		AV[0][i]-=reqmat[0][i];
		A[pid][i]+=reqmat[0][i];
		N[pid][i]-=reqmat[0][i];
	}
}

//Algoritmo de seguran�a
int safety(int A[][10],int N[][10],int AV[1][10],int n,int m,int a[]){

	int i,j,k,x=0;
	int F[10],W[1][10];
	int pflag=0,flag=0;
	for(i=0;i<n;i++)
		F[i]=0;
	for(i=0;i<m;i++)
		W[0][i]=AV[0][i];

	for(k=0;k<n;k++){
		for(i=0;i<n;i++){
			if(F[i] == 0){
				flag=0;
				for(j=0;j<m;j++){
					if(N[i][j] > W[0][j])
						flag=1;
				}
				if(flag == 0 && F[i] == 0){
					for(j=0;j<m;j++)
						W[0][j]+=A[i][j];
					F[i]=1;
					pflag++;
					a[x++]=i;
				}
			}
		}
		if(pflag == n)
			return 1;
	}
	return 0;
}


//Algoritmo do banqueiro
void accept(int A[][10],int N[][10],int M[10][10],int W[1][10],int *n,int *m){
	int i,j;
	printf("\n Entre com o n�mero total de processos : ");
	scanf("%d",n);
	printf("\n Entre com o n�mero total de recursos : ");
	scanf("%d",m);
	for(i=0;i<*n;i++){
		printf("\n Processo %d\n",i+1);
		for(j=0;j<*m;j++){
			printf(" Aloca��o para o recurso %d : ",j+1);
			scanf("%d",&A[i][j]);
			printf(" M�ximo para o recurso %d : ",j+1);
			scanf("%d",&M[i][j]);
		}
	}
	printf("\n Recursos dispon�veis : \n");
	for(i=0;i<*m;i++){
		printf(" Recurso %d : ",i+1);
		scanf("%d",&W[0][i]);
	}

	for(i=0;i<*n;i++)
		for(j=0;j<*m;j++)
			N[i][j]=M[i][j]-A[i][j];

	printf("\n Matriz de aloca��o");
	print(A,*n,*m);
	printf("\n Matriz de requerimento m�ximo");
	print(M,*n,*m);
	printf("\n Matriz de necessidade");
	print(N,*n,*m);

}

int banker(int A[][10],int N[][10],int W[1][10],int n,int m){
	int j,i,a[10];
	j=safety(A,N,W,n,m,a);
	if(j != 0 ){
		printf("\n\n");
		for(i=0;i<n;i++)
		     printf(" P%d  ",a[i]);
		printf("\n Um sequ�ncia segura foi detectada.\n");
		return 1;
	}else{
		printf("\n Ocorreu um Deadlock.\n");
		return 0;
	}
}


int main(){
    setlocale(LC_ALL, "portuguese");
	int ret;
	int A[10][10];
	int M[10][10];
	int N[10][10];
	int W[1][10];
	int n,m,pid,ch;
	printf("\n Tratamento de DEADLOCK usando o Algoritmo do Banqueiro\n");
	accept(A,N,M,W,&n,&m);
	ret=banker(A,N,W,n,m);
	if(ret !=0 ){
		printf("\n Voc� tem uma requisi��o adicional ? (1=Sim|0=N�o)");
		scanf("%d",&ch);
		if(ch == 1){
			printf("\n Entre com o n�mero de processos. : ");
			scanf("%d",&pid);
			res_request(A,N,W,pid-1,m);
			ret=banker(A,N,W,n,m);
			if(ret == 0 )
				exit(0);
		}
	}else
		exit(0);
	return 0;
}
