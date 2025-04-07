#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

using namespace std;

struct Edge {
    int inicio, fin;
    int overlap;
    Edge(int a, int b, int o) : inicio(a), fin(b), overlap(o) {}
};

vector<Edge> ensamblado(vector<string> secuencias) 
{
  vector<Edge> edges;
  int n = secuencias.size();
  for (int i = 0; i < n; ++i) 
  {
      for (int j = 0; j < n ; ++j) 
      {
        if (i != j) 
          {
            int overlap = 0;
            int minimo = min(secuencias[i].size(), secuencias[j].size());
            for (int k = minimo; k > 0; --k) 
            {
                if (secuencias[i].substr(secuencias[i].size() - k) == secuencias[j].substr(0, k)) 
                {
                    overlap = k;
                    Edge aux(i, j, overlap);
                    edges.push_back(aux);
                    break;
                }
            }
          }
      }
  }

  sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
      return a.overlap > b.overlap;
  });

  vector<int> in(n, 0);
  vector<int> out(n, 0);

  // Almacenar aristas seleccionadas
  vector<Edge> selectedEdges;

  // Procesar aristas
  for (const Edge &edge : edges) {
      if (in[edge.fin] == 0 && out[edge.inicio] == 0) {
          // Seleccionar arista
          selectedEdges.push_back(edge);
          // Actualizar grados de entrada y salida
          in[edge.fin]++;
          out[edge.inicio]++;
      }
      // Verificar si hay solo una componente conectada
      if (count(out.begin(), out.end(), 1) == n - 1) {
          break;
      }
  }
  return selectedEdges;
}

string ensamblarSuperstring(vector<string> &secuencias, vector<Edge> &selected) {
    // Inicializar el superstring con la primera secuencia
    string superstring = secuencias[selected[0].inicio];
    cout << secuencias[selected[0].inicio] << endl;
    int ovelap = 0;
    // Iterar sobre las aristas seleccionadas para construir el superstring
    for (int i = 0; i < selected.size(); ++i) {
        // Obtener la secuencia de la arista actual
        if(i!= 0 && selected[i].inicio != selected[i-1].fin)
        {
          string sec = secuencias[selected[i].inicio];
          ovelap += secuencias[selected[i-1].fin].length();
          for (int i = 0; i < ovelap; ++i) {
              std::cout << ' ';
          }
          cout <<sec <<endl;
          superstring += sec;
        }
        string sec = secuencias[selected[i].fin];
        // Calcular el overlap
        int overlap = selected[i].overlap;
        ovelap += secuencias[selected[i].inicio].length() - overlap;
        for (int i = 0; i < ovelap; ++i) {
            std::cout << ' ';
        }
        cout <<sec <<endl;
        // Agregar la porción de la secuencia que no se superpone al superstring
        superstring += sec.substr(overlap);
    }
    cout << superstring <<endl;
    return superstring;
}


vector<int> encontrarCaminoHamiltoniano(vector<vector<int>> grafo) {
    int n = grafo.size();
    vector<int> camino;
    vector<bool> visitado(n, false);
    int inicio = 0;

    // Iniciar desde el primer nodo
    camino.push_back(inicio);
    visitado[inicio] = true;

    // Iterar sobre los nodos restantes
    for (int i = 0; i < n - 1; ++i) {
        int nodo_actual = camino.back();
        int siguiente_nodo = -1;

        // Encontrar el siguiente nodo disponible con mayor peso en la matriz de adyacencia
        for (int vecino = 0; vecino < n; ++vecino) {
            if (!visitado[vecino] && (siguiente_nodo == -1 || grafo[nodo_actual][vecino] > grafo[nodo_actual][siguiente_nodo])) {
                siguiente_nodo = vecino;
            }
        }

        // Agregar el siguiente nodo al camino y marcarlo como visitado
        camino.push_back(siguiente_nodo);
        visitado[siguiente_nodo] = true;
    }

    return camino;
}

// Función para ensamblar las secuencias en un superstring utilizando un camino hamiltoniano
string ensamblarhamilton(vector<string>& secuencias, const vector<int>& camino) {
    string superstring = secuencias[camino[0]]; // Inicializar con la primera secuencia
    for (int i = 1; i < camino.size(); ++i) {
        int overlap = 0;
        int inicio = camino[i - 1];
        int fin = camino[i];
        int minimo = min(secuencias[inicio].size(), secuencias[fin].size());
        for (int k = minimo; k > 0; --k) {
            if (secuencias[inicio].substr(secuencias[inicio].size() - k) == secuencias[fin].substr(0, k)) {
                overlap = k;
                break;
            }
        }
        superstring += secuencias[fin].substr(overlap);
    }
    cout << "Hamilton"<< endl<< superstring<< endl;
    return superstring;
}

string ensambling(vector<string>& secuencias, const vector<Edge>& selected) {
    int n = secuencias.size();
    // Construir el grafo de superposición
    vector<vector<int>> grafo(n, vector<int>(n, 0));
    for (const Edge& edge : selected) {
        grafo[edge.inicio][edge.fin] = edge.overlap;
    }
    // Encontrar un camino hamiltoniano en el grafo de superposición
    vector<int> camino = encontrarCaminoHamiltoniano(grafo);
    // Ensamblar las secuencias en un superstring utilizando el camino hamiltoniano
    return ensamblarhamilton(secuencias, camino);
}
/*
vector<string> assembleShortestCommonSuperstring(vector<string> sequences) {
    // Construir el grafo de superposición
    vector<Edge> edges;
    int n = sequences.size();
    for (int i = 0; i < n; ++i) 
    {
        for (int j = 0; j < n; ++j) 
        {
            if (i != j) 
            {

              //vector<string> sequences = {"TACGA", "ACCC", "CTAAAG", "GACA"};
              cout << "----------nuevo----------"<<endl;
                int overlap = 0;
                for (int k = min(sequences[i].size(), sequences[j].size()); k > 0; --k) 
                {
                  cout <<sequences[i].substr(sequences[i].size() - k )<< endl;
                  cout << sequences[j].substr(0, k)<<endl;
                  cout << "another one " << k << endl;
                    if (sequences[i].substr(sequences[i].size() - k) == sequences[j].substr(0, k)) 
                    {
                      
                        overlap = k;
                        break;
                    }
                }
              
                if (overlap > 0) 
                {
                    edges.emplace_back(i, j, overlap); 
                }
            }
        }
    }
  for(int i = 0; i < edges.size(); i++)
    {
      cout << edges[i].inicio << " " << edges[i].fin << " " << edges[i].overlap << endl;
    }

  
    // Ordenar las aristas por superposición descendente
    sort(edges.begin(), edges.end(), [&](const Edge& a, const Edge& b) {
        return sequences[a.from].size() + sequences[a.to].size() - a.overlap < 
               sequences[b.from].size() + sequences[b.to].size() - b.overlap;
    });

    // Inicializar conjuntos disjuntos
    vector<int> parent(n);
    iota(parent.begin(), parent.end(), 0);

    // Procesar las aristas
    vector<Edge> selectedEdges;
    for (const auto& edge : edges) {
        if (parent[edge.to] != parent[edge.from]) {
            selectedEdges.push_back(edge);
            int oldParent = parent[edge.to];
            for (int i = 0; i < n; ++i) {
                if (parent[i] == oldParent) {
                    parent[i] = parent[edge.from];
                }
            }
        }
    }
  cout <<"ahh" <<endl;
    // Construir el shortest common superstring
    vector<string> superstring;
    for (int i = 0; i < n; ++i) {
        if (parent[i] == i) {
            superstring.push_back(sequences[i]);
        }
    }
  cout <<"ahh" <<endl;
    for (const auto& edge : selectedEdges) {
        superstring[parent[edge.from]] += sequences[edge.to].substr(edge.overlap);
    }
  cout <<"ahh" <<endl;
    return superstring;
}
*/
int main() {
    //vector<string> sequences = {"AAAA", "AACC", "CCGT", "GTCC"};
    //                            0        1      2          3  
    //vector<string> sequences = {"TACGA", "ACCC", "CTAAAG", "GACA"};
    vector<string> sequences = {"AGTATTGGCAATC", "AATCGATG", "ATGCAAACCT", "CCTTTTGG" , "TTGGCAATCACT"};
    vector<Edge> aristas_selec = ensamblado(sequences);
    string superstring = ensamblarSuperstring(sequences, aristas_selec);
    string superstring_2 = ensambling(sequences, aristas_selec);
    // Imprimir el shortest common superstring resultante
    cout << "longitud del superstring: "<<superstring.length()<<endl;
  cout << "longitud del hamilton: "<<superstring_2.length()<<endl;
    cout << endl;

    return 0;
}
