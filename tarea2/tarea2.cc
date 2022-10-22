
#include <iostream>
#include <vector>
#include <stdio.h>
#include <list>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include<semaphore.h>


using namespace std;
#define N 10
pthread_t tid[N];
sem_t hola;

struct Grafo
{
	int V;
	list<int> *adj;
	int peso;
	vector<vector<int>> aux;

	Grafo(int v)
	{
		V = v;
		adj = new list<int>[v];
	}

	void camino(int u, int v, int peso2)
	{
		this->peso = peso2;
		adj[u].push_back(v);
	}

	void todosloscaminos(int s, int d)
	{
		// marca todos los vertices como no visitados
		bool *visitado = new bool[V];

		//  para guardar caminos
		int *camino = new int[V];
		int path_index = 0; // inicializo en 0

		// marcando como no visitados
		for (int i = 0; i < V; i++)
			visitado[i] = false;

		// recursivo para ir imprimiendo los caminos
		crearRuta(s, d, visitado, camino, path_index);
	}

	void crearRuta(int u, int d, bool visitado[], int camino[], int &index)
	{
		visitado[u] = true;
		camino[index] = u;
		index++;

		if (u == d)
		{
			vector<int> nuevoc;
			for (int i = 0; i < index; i++)
				nuevoc.push_back(camino[i]);
			aux.push_back(nuevoc);
		}
		else // si el vertice no es el destino
		{
			// se repite para todos los vertices adj

			list<int>::iterator i;
			for (i = adj[u].begin(); i != adj[u].end(); ++i)
				if (!visitado[*i])
					crearRuta(*i, d, visitado, camino,
							  index);
		}

		// elimina el vertice del arreglo y lo marca como no visitado
		index--;
		visitado[u] = false;
	}
};
void *pasarcamino(void *arg)
{
	Grafo g(10);
	g.camino(0, 1, 4);
	g.camino(0, 2, 1);
	g.camino(0, 3, 3);
	g.camino(1, 4, 5);
	g.camino(2, 4, 5);
	g.camino(3, 4, 6);
	g.camino(4, 5, 10);
	g.camino(4, 6, 10);
	g.camino(5, 7, 22);
	g.camino(6, 7, 7);
	g.camino(7, 8, 8);
	g.camino(8, 9, 9);

	int s = 0, d = 9;
	
 	sem_init(&hola, 0, 10);
	sem_wait(&hola); 
	g.todosloscaminos(s, d);

	int tam = g.aux.size();
	int random = rand() % tam;

	for (auto v : g.aux[random])
	{
		cout << v << " ";
	}
	cout << endl;

sem_post(&hola); 
}


int main()
{
	srand(time(NULL));

	for (int i = 0; i < 10; i++) //threads que quiero
	{
		for ( int j = 0; j < 2; j++) //vez que recorre 1 thread en el grafo en este caso son 2
		{
			 pthread_create(&(tid[i]), NULL, &pasarcamino, NULL);

		}	
			
	}

	for (int i = 0; i < N; i++) //sincronizacion por thread
	{
		pthread_join(tid[i], NULL);
	}

	return 0;
}
