#include <vector>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <ostream>

using namespace std;

enum ESTADO : int {
    NONE = 0,
    SAND = 1,
    EDGE = 2
} typedef ESTADO;

class Automato {
    public:
        Automato(const int lattice_size);
        void print();
        void update();
        int size;
        vector< vector<ESTADO> > lattice;
        vector< vector<ESTADO> > next_lattice;
        string get_color(ESTADO ESTADO);
};

string Automato::get_color(ESTADO ESTADO) {
    switch(ESTADO) {
        case SAND:
            return "\033[" + to_string(43) + "m";
        case EDGE:
            return "\033[" + to_string(42) + "m";
        case NONE:
        default:
            return "\033[" + to_string(49) + "m";
    }
}

Automato::Automato(const int lattice_size) : size(lattice_size), lattice(lattice_size, vector <ESTADO>(lattice_size, NONE))
{
    for (int i = 0; i < size; i++) {
      lattice[i][size-2] = EDGE;
    }

    next_lattice = lattice;
    get_color(NONE); 
    get_color(SAND);
    get_color(EDGE);
}

void Automato::print() {
    for (int i = 1; i < size; i++) {
        for (int j = 1; j < size; j++) {
            cout << " " << get_color(lattice[j][i]) << "  ";
        }
        cout << get_color(NONE) << endl;
    }
}

void Automato::update() {
    next_lattice = lattice;
    {
        int s6,s7,s8,s9,s11,s12,s14,s15,s17,s18,s19,s20;
        #pragma omp for
        for (int j = 2; j < size-2; j++) {
            for (int k = 2; k < size-2; k++) {
                if (lattice[j][k] == NONE) {
                    s6 = lattice[j-2][k-1];
                    s7 = lattice[j-1][k-1];
                    s8 = lattice[j][k-1];
                    s9 = lattice[j+1][k-1];
                    s11 = lattice[j-2][k];
                    s12 = lattice[j-1][k];
                    s14 = lattice[j+1][k];
                

                    if (s8 == SAND)
                    next_lattice[j][k] = SAND;
                    else if (s8 == NONE && s9 == SAND && s14 >= SAND)
                    next_lattice[j][k] = SAND;
                    else if (s6 == NONE && s7 == SAND && s8 == NONE && (s9 == NONE || s9 == EDGE) && s11 >= SAND && s12 >= SAND && (s14 == NONE || s14 == EDGE))
                    next_lattice[j][k] = SAND;
                    else if (s6 >= SAND && s7 == SAND && s8 == NONE && (s9 == NONE || s9 == EDGE) && s11 >= SAND && s12 >= SAND && (s14 == NONE || s14 == EDGE))
                    next_lattice[j][k] = SAND;
                }

                if (lattice[j][k] == SAND) {
                    s12 = lattice[j-1][k];
                    s14 = lattice[j+1][k];
                    s15 = lattice[j+2][k];
                    s17 = lattice[j-1][k+1];
                    s18 = lattice[j][k+1];
                    s19 = lattice[j+1][k+1];
                    s20 = lattice[j+2][k+1];


                    if (s18 == NONE)
                    next_lattice[j][k] = NONE;
                    else if (s12 == NONE && s17 == NONE && s18 >= SAND)
                    next_lattice[j][k] = NONE;
                    else if (s12 == NONE && s14 == NONE && (s15 == NONE || s15 == EDGE) && s17 >= SAND && s18 >= SAND && s19 == NONE && (s20 == NONE || s20 == EDGE))
                    next_lattice[j][k] = NONE;
                    else if (s12 >= SAND && s14 == NONE && (s15 == NONE || s15 == EDGE) && s17 >= SAND && s18 >= SAND  && s19 == NONE && (s20 == NONE || s20 == EDGE))
                    next_lattice[j][k] = NONE;
                }
            }
        }
    }
    lattice = next_lattice;
}


int main() {
    int lattice_size = 40;
    int n_sands = 200;

    Automato automata = Automato(lattice_size);

    int centerX = lattice_size / 2;
    int centerY = lattice_size / 2;
    int base = 10;
    int height = 5;

    // Criar triângulo de areia
    for (int i = 0; i < height; i++) {
        for (int j = centerX - (base / 2) + i; j <= centerX + (base / 2) - i; j++) {
            automata.lattice[centerY-i][j] = SAND;
            n_sands++;
        }
    }

    for (int i = 0; i < 80; i++) {
        automata.update();
        system("clear");
        automata.print();
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    return 0;
}
