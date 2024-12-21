#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_LINE 100
#define INF INT_MAX

struct Vertex 
{
    int index;
	int cost;
	float score;
    struct Vertex* pnext;
};

struct Graph
{
    int nr;
    struct Vertex** vec;
};

struct Vertex* create_vertex(int index, int cost) 
{
    struct Vertex* v = (struct Vertex*)malloc(sizeof(struct Vertex));
    v->index = index;
	v->cost = cost;
	v->score = 0;
    v->pnext = NULL;

    return v;
}

struct Graph* create_graph(int n)
{
	struct Graph* G = (struct Graph*)malloc(sizeof(struct Graph));
	G->nr = n;
	G->vec = (struct Vertex**)calloc(n, sizeof(struct Vertex*));
	return G;
}

void free_graph(struct Graph* G)
{
	if (G == NULL)
		return;
	// eliberat lista de vertexi
	for (int i = 0; i < G->nr; i++)
		if (G->vec[i])
		{
			struct Vertex* p = G->vec[i];
			while (p)
			{
				struct Vertex* aux = p;
				p = p->pnext;
				free(aux);
			}
		}
	free(G->vec);
	free(G);
}

struct Vertex* clone(struct Vertex* source)
{
	if (source == NULL)
		return NULL;

	struct Vertex* ret = (struct Vertex*)malloc(sizeof(struct Vertex));
	ret->index = source->index;
	ret->cost = source->cost;
	ret->pnext = NULL;
	return ret;
}

void add_list_ascending(struct Vertex** list, struct Vertex* elem)
{
	if (list == NULL || elem == NULL)
		return;
	
	struct Vertex* nou = clone(elem);

	if (*list == NULL)
	{
		*list = nou;
		return;
	}

	// este un nou minim
	if (elem->cost < (*list)->cost)
	{
		nou->pnext = *list;
		*list = nou;

		return;
	}
	
	struct Vertex* p = (*list);
	struct Vertex* aux = p; //va arata catre ultimul element
						// sau ultimul element mai mic
	while (p && p->cost <= elem->cost)
	{
		aux = p;
		p = p->pnext;
	}

	aux->pnext = nou;

	
	// daca avem un singur element
	if (aux == p)
		return;

	nou->pnext = p;
}

void add_list(struct Vertex** list, struct Vertex* elem)
{
	if (list == NULL || elem == NULL)
		return;
	
	if (*list == NULL)
	{
		*list = elem;
		return;
	}

	// cel mai repede adaug direct in fata
	elem->pnext = *list;
	*list = elem;
}

void add_graph_undirected(struct Graph* graph, int src, int dest, int cost)
{
	if (graph == NULL)
		return;

	struct Vertex* first = create_vertex(src, cost);
	struct Vertex* second = create_vertex(dest, cost);

	add_list(&graph->vec[src], second);
	add_list(&graph->vec[dest], first);
}

void add_graph_directed(struct Graph* graph, int src, int dest, int cost)
{
	if (graph == NULL)
		return;

	struct Vertex* second = create_vertex(dest, cost);
	add_list(&graph->vec[src], second);
}


// aici nu modific nr_grup
void DFS(struct Graph* G, int* grup, int start, int nr_grup)
{
	if (G == NULL || grup == NULL )
		return;

	grup[start] = nr_grup;
	struct Vertex* p = G->vec[start];
	while (p)
	{
		if (grup[p->index] == -1)
			DFS(G, grup, p->index, nr_grup);
		p = p->pnext;
	}
}

void count_DFS(struct Graph* G, int* grup, int* nr_grup)
{
	if (G == NULL || grup == NULL || nr_grup == NULL)
		return;

	// parcurg lista de noduri
	for (int i = 0; i < G->nr; i++)
	{
		// si caut urmatorul nod nevizitat
		if (grup[i] < 0)
		{
			//am un grup nou
			(*nr_grup)++;
			grup[i] = *nr_grup;
			// marcam pe el si toate nodurile lui adiacente ca fiind
			// din acelasi grup
			DFS(G, grup, i, *nr_grup);
		}
	}
}

struct Vertex* get_minim_filter(int* vizitate, struct Vertex** list_optiuni)
{
	if (vizitate == NULL || *list_optiuni == NULL)
		return NULL;

	struct Vertex *ret = NULL;
	while (1)
	{
		struct Vertex* p = *list_optiuni;
		if (p == NULL)
			return ret;
		// daca nu este vizitat
		if (vizitate[p->index] != 1)
		{
			ret = p;
			vizitate[p->index] = 1;
			// il scoatem din lista de minime
			*list_optiuni = p->pnext;
			break;
		}
		// daca a fost vizitat intre timp
		// il scoatem din lista de minime
		else
		{
			*list_optiuni = p->pnext;
			free(p);
		}			
	}

	return ret;
}

int MST(struct Graph *G, int start, int* vizitate, struct Vertex** list_optiuni)
{
	if (G == NULL || vizitate == NULL || list_optiuni == NULL)
		return -1;

	// vizitez nodul curent
	vizitate[start] = 1;
	// iau toti vecinii
	struct Vertex *p = G->vec[start];
	while (p)
	{
		if (!vizitate[p->index])
			add_list_ascending(list_optiuni, p);
		p = p->pnext;	
	}

	// iau urmatorul minim din toate optiunile de a extinde MST
	struct Vertex *min = get_minim_filter(vizitate, list_optiuni);
	if (min == NULL)
		return 0;
	
	int min_cost = min->cost;
	int min_index = min->index;

	free(min);
	int ret = min_cost + MST(G, min_index, vizitate, list_optiuni);

	return ret;
}

int find(char** noduri, const char* val, int n)
{
	for (int i = 0; i < n; i++)
		if (strcmp(val, noduri[i]) == 0)
			return i;
	return -1;
}

int comparator(const void *p, const void *q) 
{
    int l = *((int*)p);
    int r = *((int *)q); 
    return l - r;
}

// DE SCOS
void print_graph(struct Graph* graph) {
    int i;
    for (i = 0; i < graph->nr; i++) {
        struct Vertex* vertex = graph->vec[i];
        printf("Adjacency list of vertex %d: ", i);
        while (vertex != NULL) {
            printf("%d -> ", vertex->index);
            vertex = vertex->pnext;
        }
        printf("NULL\n");
    }
}

void update_score(struct Graph* G, int* depth)
{
	if (G == NULL || depth == NULL)
		return;
	int N = G->nr;
	for (int i = 0; i < N; i++)
		if (G->vec[i])
		{
			struct Vertex *p = G->vec[i];
			while (p)
			{			
				p->score = ((float)p->cost) / depth[p->index];			
				p = p->pnext;
			}
		}
}

int get_cost(struct Graph* g, int src, int dst)
{
	if (g == NULL)
		return -1;
	struct Vertex* p = g->vec[src];
	
	while (p && p->index != dst)
		p = p->pnext;

	return p->cost;
}

int* shortest_path(struct Graph* G, int start, int dest, 
					int* nr_hops, int* total)
{
	if (G == NULL || nr_hops == NULL)
		return NULL;

	int N = G->nr;
	float* dist = (float*)malloc(N * sizeof(float));
    int* prev = (int*)malloc(N * sizeof(int));
    int* vizitate = (int*)calloc(N, sizeof(int));

    for (int i = 0; i < N; i++)
	{
        dist[i] = INF;
        prev[i] = -1;
    }
	dist[start] = 0;

	// while algoritm de calcul al distantelor
	while (1)
	{
		float min_dist = INT_MAX;
        int next = -1; // urmatorul varf care va fi adaugat in cale

		for (int i = 0; i < N; i++) 
		{
            if (!vizitate[i] && dist[i] <= min_dist) // < ?
			{
                min_dist = dist[i];
                next = i;
            }
        }
		
		if (next == -1) // nu mai putem continua
			break;

		vizitate[next] = 1;
		// ma uit la vecinii nodului adaugat
		struct Vertex* p = G->vec[next];
		while (p) 
		{
			// updatez vecinii daca am gasit o metoda mai buna de a ajunge la ai
            if (!vizitate[p->index] && dist[next] + p->score < dist[p->index]) 
			{
                dist[p->index] = dist[next] + p->score;
				// calea catre destinatie merge din nodul curent prin
				// vecin p->index                
				prev[p->index] = next;
            }

            p = p->pnext;
        }
	}

	 // vedem daca avem cale
    if (prev[dest] == -1)
	{
		*nr_hops = -1;
		free(dist);
		free(prev);
		free(vizitate);
		return NULL;
	}

	// construim calea pe care o vom intoarce
    int nod_curent = dest;
	int* path = (int*)malloc(N * sizeof(int));
	
	while (nod_curent != -1)
	{
		int src = nod_curent;
    	path[(*nr_hops)++] = nod_curent;
    	nod_curent = prev[nod_curent];
		if (nod_curent != -1)
			*total = *total + get_cost(G, nod_curent, src);
    }  

	// eliberat memoria
	free(dist);
	free(prev);
	free(vizitate);

	return path;
}

void reverse(int* v, int n)
{
	if (v == NULL)
		return;	
	for (int i = 0; i < n / 2; i++)
	{
		int aux = v[i];
		v[i] = v[n - 1 - i];
		v[n - 1 - i] = aux;
	}
		
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Incorrect usage: ./tema3 nr_cerinta\n");
		return 1;
	}
	char *endptr;
	int n = (int)strtol(argv[1], &endptr, 10);

	if (*endptr != '\0')
	{
		printf("Second argument must be '1' or '2'\n");
		return 2;
	}

	char** noduri;
	int nr_noduri = 0;

	int nr_grup = 0;	
	int* grup;

	FILE* fin = fopen("tema3.in", "r");
	if (fin == NULL)
	{
		printf("tema3.in missing\n");
		return 3;
	}
	int N, M;
	fscanf(fin, "%d%d", &N, &M);
	
	char dump; // sa iau newline dupa fscanf ptr primul fgets
	while ((dump = fgetc(fin)) != EOF && dump != '\n')
		; // empty

	noduri = (char**)calloc(N, sizeof(char*));
	
	struct Graph* G = create_graph(N);
	
	// corabia si insula pentru cerinta 2
	if (n == 2)
	{
		// mereu corabia va fi nodul 0 si insula nodul 1
		char *corabie = (char*)malloc((strlen("Corabie") + 1) * sizeof(char));
		strcpy(corabie, "Corabie");
		char* insula = (char*)malloc((strlen("Insula") + 1) * sizeof(char));
		strcpy(insula, "Insula");
		noduri[0] = corabie;
		noduri[1] = insula;

		nr_noduri = 2;
	}

	int cost;
	int first_index, second_index;
	for (int i = 0; i < M; i++)
	{
		char line[MAX_LINE];
		fgets(line, sizeof(line), fin);

		char* first = strtok(line, " \n");
					
		first_index = find(noduri, first, nr_noduri);
		// adaugam primul nume de locatie			
		if (first_index == -1)
		{
			first_index = nr_noduri++;
			noduri[first_index] = (char*)calloc(strlen(first) + 1, sizeof(char));
			strcpy(noduri[first_index], first);
		}
		
		char* second = strtok(NULL, " \n");
		second_index = find(noduri, second, nr_noduri);
		if (second_index == -1)
		{
			second_index = nr_noduri++; // il adaug si pe asta la vectorul de noduri
			noduri[second_index] = (char*)calloc(strlen(second) + 1, sizeof(char));
			strcpy(noduri[second_index], second);
		}

		char* cst = strtok(NULL, " \n");
		cost = (int)strtol(cst, &endptr, 10);
	
		if (n == 1)
			add_graph_undirected(G, first_index, second_index, cost);
		else if (n == 2)
			add_graph_directed(G, first_index, second_index, cost);

	} // for citire muchii graf
	
	// prima cerinta
	if (n == 1)
		fclose(fin);
					
	// prima cerinta
	if (n == 1)
	{
		grup = (int*)malloc(N * sizeof(int));
		// initial toate nodurile sunt nevizitate
		for (int i = 0; i < N; i++)
			grup[i] = -1;

		//parcurgem graful in adancime
		count_DFS(G, grup, &nr_grup);

		//numararea costurilor arborilor
		int *mst = (int*)malloc(nr_grup * sizeof(int));
		for (int i = 0; i < nr_grup; i++)
			mst[i] = -1;

		// pentru fiecare grup calculez mst
		for (int i = 0; i < N; i++)
		{
			if (mst[grup[i] - 1] == -1)
			{
				int* vizitate = (int*)calloc(N, sizeof(int));
				struct Vertex* lista = NULL;
				mst[grup[i] - 1] = MST(G, i, vizitate, &lista);
				
				free(vizitate);
			}
		}
		// sortarea
		qsort(mst, nr_grup, sizeof(int), comparator);

		// afisarea datelor
		FILE *fout = fopen("tema3.out", "w");
		fprintf(fout, "%d\n", nr_grup);
		for (int i = 0; i < nr_grup; i++)
			fprintf(fout, "%d\n", mst[i]);

		fclose(fout);

		// eliberarea memoriei
		free(mst);
		free(grup);
	}
	else if (n == 2)
	{
		int* depth = (int*)malloc(N * sizeof(int));

		// citim distantele
		for (int i = 0; i < N; i++)
		{
			char line[MAX_LINE];
			fgets(line, sizeof(line), fin);

			char* nod = strtok(line, " \n");
			int nod_index = find(noduri, nod, nr_noduri);

			char* dst = strtok(NULL, " \n");
			int deep = (int)strtol(dst, &endptr, 10);

			depth[nod_index] = deep;
		}

		// citim tezaurul
		int treasure;
		fscanf(fin, "%d", &treasure);

		fclose(fin);

		//print_graph(G);

		// calculam scorurile arcelor
		update_score(G, depth);

		int num_hops = 0;
		int total = 0;		
		int* rez1 = shortest_path(G, 0, 1, &num_hops, &total);

		FILE *fout = fopen("tema3.out", "w");		
		if (rez1 == NULL)
		{
			fprintf(fout, "Echipajul nu poate ajunge la insula");
			fclose(fout);

			free(depth);
			free_graph(G);
			for (int i = 0; i < N; i++)
					if(noduri[i])
						free(noduri[i]);
			free(noduri); 
			return 4;
		}
		free(rez1);

		num_hops = total = 0;

		int* rez2 = shortest_path(G, 1, 0, &num_hops, &total);
		if (rez2 == NULL)
		{
			fprintf(fout, "Echipajul nu poate transporta comoara inapoi la corabie");
			fclose(fout);

			free(depth);
			free_graph(G);
			for (int i = 0; i < N; i++)
					if(noduri[i])
						free(noduri[i]);
			free(noduri); 
			return 5;
		}
		
		reverse(rez2, num_hops);
		// calculam  si greutatea maxima
		int max = INF;

		// afisam calea de la insula la corabie
		for (int i = 0; i < num_hops; i++)
		{
			if (i != 0)
				fprintf(fout, " ");
			fprintf(fout, "%s", noduri[rez2[i]]);
			if (depth[rez2[i]] < max && rez2[i] > 1)
				max = depth[rez2[i]];	
		}
		fprintf(fout, "\n");
		fprintf(fout, "%d\n", total);
		fprintf(fout, "%d\n", max);
		fprintf(fout, "%d\n", treasure / max);
		fclose(fout);

		free(depth);
		free(rez2);
	}

	free_graph(G);
	for (int i = 0; i < N; i++)
			if(noduri[i])
				free(noduri[i]);
		free(noduri); 

	return 0;
}
