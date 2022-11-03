#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

short sudoku[9][9];
short solution[9][9];

void readFromFile(string);
void writeToFile(string);
void printSudoku();
void printSolution();
bool isValid();
bool elementoUnicoRigaColonna(short, short);
bool elementoUnicoSubQuadrato(short, short);
bool isUnico(short, short);

void solve();

int main()
{
    cout << "leggo input" << endl;
    readFromFile("input_hard2.txt");
    cout << " ------------------------ " << endl;
    printSudoku();

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            solution[i][j] = sudoku[i][j];

    cout << "sudoku is " << (isValid() ? "" : "not ") << "valid" << endl;

    solve();
    cout << "finito" << endl;
    printSolution();
    cout << "sudoku is " << (isValid() ? "" : "not ") << "valid" << endl;
    return 0;
}

bool isValid()
{
    bool valid = true;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            valid = elementoUnicoRigaColonna(i, j);
            valid = elementoUnicoSubQuadrato(i, j);
            if (!valid)
                return false;
        }
    return true;
}

void readFromFile(string path)
{
    ifstream file(path);
    std::string line;
    if (file.is_open())
    {
        int i = 0, j = 0;
        while (file)
        {
            getline(file, line);
            istringstream reader(line);
            for (int number, j = 0; reader >> number; j++)
            {
                sudoku[i][j] = number;
            }
            i++;
        }
    }
    else
    {
        cout << "errore apertura file";
    }
}

void printSudoku()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
            cout << sudoku[i][j] << " ";
        cout << endl;
    }
}
void printSolution()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
            cout << solution[i][j] << " ";
        cout << endl;
    }
}

bool elementoUnicoRigaColonna(short r, short c)
{
    // salto placeholder
    if (solution[r][c] == 0)
        return true;
    // riga
    for (int i = 0; i < 9; i++)
    {
        if (i != c && solution[r][i] == solution[r][c])
            return false;
    }
    // colonna
    for (int i = 0; i < 9; i++)
    {
        if (i != r && solution[i][c] == solution[r][c])
            return false;
    }
    return true; // non ci sono altri elementi uguali sulla stessa riga e colonna
}
bool elementoUnicoSubQuadrato(short r, short c)
{
    // salto placeholder
    if (solution[r][c] == 0)
        return true;
    // 0 - 2->0, 3 - 5->3, 6 - 8->6
    // cout << endl << r << " - " << c << endl;
    short r1 = r - (r % 3);
    short c1 = c - (c % 3);
    for (int i = r1; i < r1 + 3; i++)
    {
        for (int j = c1; j < c1 + 3; j++)
        {
            // cout << solution[i][j] << " ";
            if (solution[i][j] == solution[r][c] && !(i == r && j == c))
                return false;
        }
        // cout << endl;
    }
    return true; // elemento unico nel subquadrato
}
bool isUnico(short r, short c)
{
    // elemento unico sulla riga/colonna e nel subquadrato
    return elementoUnicoRigaColonna(r, c) && elementoUnicoSubQuadrato(r, c);
}

// risoluzione
void solve()
{
    // per tutte le righe
    for (int i = 0; i < 9; i++)
    {
        // per tutte le colonne
        for (int j = 0; j < 9;)
        {
            // se è cella da riempire
            if (sudoku[i][j] == 0)
            {
                // incremento di 1 fino a che l'elemento non è unico sulla riga/colonna e subquadrato
                // e minore di 10 (non ho sforato i tentativi)
                do
                {
                    solution[i][j]++;
                } while (!isUnico(i, j) && solution[i][j] < 10);

                // se arrivo a 10 allora qualche numero precedente è sbagliato
                // quindi recupero il precedente, lo incremento e riparto da li in avanti
                if (solution[i][j] >= 10)
                {
                    solution[i][j] = 0; // risetto a 0 quello sbagliato
                    // torno indietro fino alla primo numero trovato
                    int mi = i, mj = j;
                    for (int ii = mi; ii >= 0; ii--)
                    {
                        for (int jj = mj; jj >= 0; jj--)
                        {
                            if (sudoku[ii][jj] == 0 && (ii != i || jj != j)) // (salto la prima posizione)
                            {
                                i = ii;
                                j = jj;
                                jj = 0;
                                ii = 0;
                            }
                        }
                        mj = 8;
                    }
                }
                else // il numero trovato sembra andare bene, vado avanti
                    j++;
            }
            else
                j++;
        }
        cout << "working ... " << i << endl;
    }
    cout << "finito" << endl;
}