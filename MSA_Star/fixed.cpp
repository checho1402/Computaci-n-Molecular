#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

string t, s;

struct nodo {
    nodo(int dato) : value(dato) {}
    int value;
    vector<pair<nodo*, int>> camino;
    vector<string> best;
    int maximo;
};

struct alineamiento {
    alineamiento(string i = "", string j = "") : s(i), t(j), s_S(i.size()), t_S(j.size()) {}
    string s, t;
    int s_S, t_S;
    vector<string> best_path;
    int maxi = INT8_MIN;
};
void print_matrix(const vector<vector<nodo>>& mat) {
    int n = mat.size();
    int m = mat[0].size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << mat[i][j].value << "\t";
        }
        cout << endl;
    }
}

void fill_string(string& path_s, string& path_t, int opc, int& i, int& j , alineamiento X)
{
    switch (opc)
    {
    case 1:
        path_s += X.s[X.s_S + i];
        path_t += X.t[X.t_S + j];
        i--;
        j--;
        break;
    case 2:
        path_s += X.s[X.s_S + i];
        path_t += "-";
        i--;
        break;
    case 3:
        path_s += "-";
        path_t += X.t[X.t_S + j];
        j--;
        break;
    }
}

int maximo(string s, string t) {
    int sum_s = 0;
    int sum_t = 0;
    bool first_gap_s = true;
    bool first_gap_t = true;

    for (int i = 0; i < s.length(); ++i) {
        if (s[i] == '-') {
            if (first_gap_s) {
                sum_s -= 2;
                first_gap_s = false;
            } else {
                sum_s -= 1;
              
            }
        }
        else first_gap_s = true;
      
        if (t[i] == '-') {
            if (first_gap_t) {
                sum_t -= 2;
                first_gap_t = false;
            } else {
                sum_t -= 1;
            }
        }
        else first_gap_t = true;
    }

    return sum_s + sum_t;
}

void all_paths(nodo* current, string path_s, string path_t, int i, int j, alineamiento &X)
{
    if (current == nullptr) return;
    while (!current->camino.empty())
    {
        int size_node = current->camino.size();
        int in_case_one = current->camino[0].second;
        if (size_node > 1)
        {
            for (int k = 1; k < size_node; k++)
            {
                int number = current->camino[k].second;
                string aux1 = path_s, aux2 = path_t;
                int aux_i = i, aux_j = j;
                fill_string(aux1, aux2, number, aux_i, aux_j, X);
                all_paths(current->camino[k].first, aux1, aux2, aux_i, aux_j, X);
            }
        }
        fill_string(path_s, path_t, in_case_one, i, j, X);
        current = current->camino[0].first;
    }
    int max_value = maximo(path_s, path_t);
    if (max_value >= X.maxi)
    {
        X.maxi = max_value;
        X.best_path = { path_s, path_t };
    }
    //cout<<"caminos: "<<endl<<path_s<<endl<<path_t<<endl;
}

nodo Neddleman_Wunch(string s, string t) {
    int n = s.length();
    int m = t.length();
    alineamiento X(s, t);
    //cout << "n: " << n << " m: " << m << endl;
    vector<string> soluciones;

    vector<vector<nodo>> mat(n + 1, vector<nodo>(m + 1, nodo(0)));

    // Llenar la primera COLUMNA con los valores deseados
    for (int j = 0, val = 0; j <= m; ++j, val -= 2) {
        mat[0][j].value = val;
        if (j > 0)
        {
            mat[0][j].camino.push_back(make_pair(&mat[0][j - 1], 3));
        }
    }

    // Llenar la primera FILA con los valores deseados
    for (int i = 0, val = 0; i <= n; ++i, val -= 2) {
        mat[i][0].value = val;
        if (i > 0)
        {
            mat[i][0].camino.push_back(make_pair(&mat[i - 1][0], 2));
        }
    }

    // Llenar la matriz con los valores deseados
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            vector<int> nums;
            if (s[i - 1] == t[j - 1]) {
                nums.push_back(mat[i - 1][j - 1].value + 1);
            }
            else
                nums.push_back(mat[i - 1][j - 1].value - 1);

            nums.push_back(mat[i - 1][j].value - 2);
            nums.push_back(mat[i][j - 1].value - 2);

            int maximo = max(nums[0], max(nums[1], nums[2]));
            if (nums[0] == maximo)
                mat[i][j].camino.push_back(make_pair(&mat[i - 1][j - 1], 1));
            if (nums[1] == maximo)
                mat[i][j].camino.push_back(make_pair(&mat[i - 1][j], 2));
            if (nums[2] == maximo)
                mat[i][j].camino.push_back(make_pair(&mat[i][j - 1], 3));

            mat[i][j].value = maximo;
        }
    }

    //cout << "Matriz final:" << endl;
    //print_matrix(mat);

    // Imprimir los caminos desde la última posición de la matriz
    //cout << "Score: " << mat[n][m].value << endl;
    all_paths(&mat[n][m], "", "", -1, -1, X);
    mat[n][m].best = X.best_path;
    mat[n][m].maximo = X.maxi;
    //cout << "best: "<< mat[n][m].best[1] <<endl;
  
  return mat[n][m];
}

vector<string> MSA_star(vector <string> sequences)
{

  cout << "---------------MSA STAR --------------" <<endl;
  int size = sequences.size();
  vector<string> alignments(size);
  vector<vector<nodo>> mat(size, vector<nodo>(size, nodo(0)));
  for (int i = 0; i < size; ++i) {
      for (int j = i + 1; j < size; ++j) 
      {
          mat[i][j] = Neddleman_Wunch(sequences[i], sequences[j]);
          mat[j][i] = mat[i][j];
      }
  }
  print_matrix(mat);

  pair <int, int> index; //suma / indice
  int maximo = INT8_MIN;
  for(int i = 0; i < size; ++i)
    {
      int sum = 0;
      for(int j = 0; j < size; ++j)
        sum += mat[i][j].value;
      if(sum > maximo)
      {
        maximo = sum;
        index = make_pair(sum, i);
      }
    }
  cout << "Score: "<< index.first<<endl;
  vector <string> solucion;
  //cout << "real: "<<sequences[index.second] << endl;
  solucion.push_back(sequences[index.second]);
  int len_max = -1;
  for(int i = 0; i < size; ++i)
    {
      if (i != index.second)
      {
        string best_path = mat[index.second][i].best[1];
        int size = best_path.size();
        if(size > len_max) len_max = size;
        solucion.push_back(best_path);
        
        //cout << "solu:"<<endl<< mat[index.second][i].best[0] <<endl <<mat[index.second][i].best[1] << endl;
      }
    }
  cout << "Solucion: " << endl;
  for(auto& s : solucion)
    {
      s.resize(len_max,'-');
      cout << s << endl;

    }

    
  return alignments;
}

void extract_sequences(string filename, vector<string> &forward_sequences, vector<string>& reverse_sequences) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        // Ignorar líneas vacías o que comiencen con espacio
        if (line.empty())
            continue;

        // Encontrar la letra 'F' o 'R'
        size_t pos_f = line.find("F:");
        size_t pos_r = line.find("R:");

        // Si se encuentra la letra 'F', extraer la secuencia correspondiente
        if (pos_f != string::npos) {
            string secuencia = line.substr(pos_f + 8); // Ignorar "F: 
            string secu = secuencia.substr(0, secuencia.length() - 5);
            forward_sequences.push_back(secu);
        }

        // Si se encuentra la letra 'R', extraer la secuencia correspondiente
        if (pos_r != string::npos) {
            string secuencia = line.substr(pos_r + 8); // Ignorar "R: "
            string secu = secuencia.substr(0, secuencia.length() - 5);
            reverse_sequences.push_back(secu);
        }
    }

    file.close();
}

string reverse(string line)
{
  string result = "";
  for(int i = line.length() - 1; i >= 0; i--)
    {
      if(line[i] == 'A')
        result += 'T';
      if(line[i] == 'T')
        result += 'A';
      if(line[i] == 'C')
        result += 'G';
      if(line[i] == 'G')
        result += 'C';
    }
  return result;
}
int main() {
/*
  ifstream file("Sequencias.txt");
  string line, total;
  vector<string> adns;
  while (std::getline(file, line)) {
    if (line.empty())
      continue;
    if (line.find("Bacteria") != std::string::npos)
      continue;
    if (line.find("Sars-Cov") != std::string::npos ||
        line.find("Influenza") != std::string::npos) {
      if (!total.empty()) // empezando nueva decodificación
      {
        adns.push_back(total);
        total.clear();
      }
      continue;
    }

    for (char c : line) {
      if (!isdigit(c) && !isspace(c) && c != '`')
        total += c;
    }
  }
  adns.push_back(total);
  
      for (const string& adn : adns)
        {
          cout << "Secuencia: " << adn.size() << endl;
        }
  */
  // s = adns[0];
  // t = adns[1];
  //cin >> s >> t;
  //s_S = s.size();
  //t_S = t.size();
  //nodo score = Neddleman_Wunch(s, t);
  vector<string> forward_sequences;
  vector<string> reverse_sequences;

  extract_sequences("BRCA1.txt", forward_sequences, reverse_sequences);

  cout << "Secuencias Forward (F):" << endl;
  for (const string& seq : forward_sequences) {
      cout << seq << endl;
  }

  cout << "Secuencias Reverse (R):" << endl;
  for (const string& seq : reverse_sequences) {
      cout << seq << endl;
  }

  //PRUEBA FORWARD
  cout << "---------PRUEBA FORWARD-------------" << endl;  
  vector<string> alignments = MSA_star(forward_sequences);

  cout << "---------PRUEBA REVERSE-------------" << endl;
  vector<string> alignments_reverse = MSA_star(reverse_sequences);

  vector<string> new_sequences;
  for(int i = 0; i < 12; ++i)
    {
      if(i % 2 == 0)
        new_sequences.push_back(forward_sequences[i/2]);
      else
        new_sequences.push_back(reverse(reverse_sequences[i/2]));
    }

  cout << "--------PRUEBA TOTAL REVERSED---------" << endl;
  vector<string> alignments_total = MSA_star(new_sequences);

  cout << "--------PRUEBA TOTAL SIN REVERSED---------" << endl;
  vector<string> new_sequences2;
  for(int i = 0; i < 12; ++i)
    {
      if(i % 2 == 0)
        new_sequences2.push_back(forward_sequences[i/2]);
      else
        new_sequences2.push_back(reverse_sequences[i/2]);
    }
  vector<string> alignments_total2 = MSA_star(new_sequences2);
  return 0;
}
