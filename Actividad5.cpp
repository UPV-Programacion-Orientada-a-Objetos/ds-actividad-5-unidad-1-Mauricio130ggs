#include <iostream>
#include <iomanip>

using namespace std;

template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:

    MatrizBase(int filas, int columnas) : _filas(filas), _columnas(columnas) {}

    virtual ~MatrizBase() {}

    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;

    MatrizBase<T>* operator+(const MatrizBase<T>& otra) const {
        return sumar(otra);
    }

    int getFilas() const { return _filas; }
    int getColumnas() const { return _columnas; }
};

template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T** _datos;

    void liberarMemoria() {
        if (_datos != nullptr) {
            for (int i = 0; i < this->_filas; i++) {
                delete[] _datos[i];
            }
            delete[] _datos;
            _datos = nullptr;
        }
    }

    void asignarMemoria() {
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; i++) {
            _datos[i] = new T[this->_columnas];
            // Inicializar en 0
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = T(0);
            }
        }
    }

public:
    MatrizDinamica(int filas, int columnas) : MatrizBase<T>(filas, columnas) {
        asignarMemoria();
    }

    ~MatrizDinamica() {
        cout << "Liberando memoria de Matriz Dinamica..." << endl;
        liberarMemoria();
    }

    MatrizDinamica(const MatrizDinamica<T>& otra) : MatrizBase<T>(otra._filas, otra._columnas) {
        asignarMemoria();
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = otra._datos[i][j];
            }
        }
    }

    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& otra) {
        if (this != &otra) {
            liberarMemoria();
            this->_filas = otra._filas;
            this->_columnas = otra._columnas;
            asignarMemoria();
            for (int i = 0; i < this->_filas; i++) {
                for (int j = 0; j < this->_columnas; j++) {
                    _datos[i][j] = otra._datos[i][j];
                }
            }
        }
        return *this;
    }

    MatrizDinamica(MatrizDinamica<T>&& otra) noexcept : MatrizBase<T>(otra._filas, otra._columnas) {
        _datos = otra._datos;
        otra._datos = nullptr;
        otra._filas = 0;
        otra._columnas = 0;
    }
    MatrizDinamica<T>& operator=(MatrizDinamica<T>&& otra) noexcept {
        if (this != &otra) {
            liberarMemoria();
            this->_filas = otra._filas;
            this->_columnas = otra._columnas;
            _datos = otra._datos;
            otra._datos = nullptr;
            otra._filas = 0;
            otra._columnas = 0;
        }
        return *this;
    }
    
    void cargarValores() override {
        cout << "Ingrese los valores para la matriz (" << this->_filas << "x" << this->_columnas << "):" << endl;
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                cout << "Posicion [" << i << "][" << j << "]: ";
                cin >> _datos[i][j];
            }
        }
    }
    
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            cout << "Error: Las dimensiones de las matrices no son compatibles para la suma." << endl;
            return nullptr;
        }
        
        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);

        const MatrizDinamica<T>* otraDinamica = dynamic_cast<const MatrizDinamica<T>*>(&otra);
        if (otraDinamica) {
            for (int i = 0; i < this->_filas; i++) {
                for (int j = 0; j < this->_columnas; j++) {
                    resultado->_datos[i][j] = this->_datos[i][j] + otraDinamica->_datos[i][j];
                }
            }
        }
        
        return resultado;
    }
    
    void imprimir() const override {
        for (int i = 0; i < this->_filas; i++) {
            cout << "| ";
            for (int j = 0; j < this->_columnas; j++) {
                cout << setw(6) << fixed << setprecision(1) << _datos[i][j];
                if (j < this->_columnas - 1) cout << " | ";
            }
            cout << " |" << endl;
        }
    }
    
    void setValor(int i, int j, T valor) {
        if (i >= 0 && i < this->_filas && j >= 0 && j < this->_columnas) {
            _datos[i][j] = valor;
        }
    }
};

template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
private:
    T _datos[M][N];

public:
    MatrizEstatica() : MatrizBase<T>(M, N) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                _datos[i][j] = T(0);
            }
        }
    }
    
    ~MatrizEstatica() {
        cout << "Destruyendo Matriz Estatica..." << endl;
    }
    
    void cargarValores() override {
        cout << "Ingrese los valores para la matriz estatica (" << M << "x" << N << "):" << endl;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                cout << "Posicion [" << i << "][" << j << "]: ";
                cin >> _datos[i][j];
            }
        }
    }
    
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            cout << "Error: Las dimensiones de las matrices no son compatibles para la suma." << endl;
            return nullptr;
        }
        
        MatrizEstatica<T, M, N>* resultado = new MatrizEstatica<T, M, N>();
        
    
        const MatrizEstatica<T, M, N>* otraEstatica = dynamic_cast<const MatrizEstatica<T, M, N>*>(&otra);
        if (otraEstatica) {
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    resultado->_datos[i][j] = this->_datos[i][j] + otraEstatica->_datos[i][j];
                }
            }
        }
        
        return resultado;
    }
    
    void imprimir() const override {
        for (int i = 0; i < M; i++) {
            cout << "| ";
            for (int j = 0; j < N; j++) {
                cout << setw(6) << fixed << setprecision(1) << _datos[i][j];
                if (j < N - 1) cout << " | ";
            }
            cout << " |" << endl;
        }
    }
    
    void setValor(int i, int j, T valor) {
        if (i >= 0 && i < M && j >= 0 && j < N) {
            _datos[i][j] = valor;
        }
    }
};

int main() {
    cout << "--- Sistema Generico de Algebra Lineal ---" << endl;
    cout << ">> Demostracion de Genericidad (Tipo FLOAT) <<" << endl << endl;
    
    cout << "// 1. Creacion de Matriz Dinamica A (3x2)..." << endl;
    MatrizBase<float>* A = new MatrizDinamica<float>(3, 2);
    

    dynamic_cast<MatrizDinamica<float>*>(A)->setValor(0, 0, 1.5f);
    dynamic_cast<MatrizDinamica<float>*>(A)->setValor(0, 1, 2.0f);
    dynamic_cast<MatrizDinamica<float>*>(A)->setValor(1, 0, 0.0f);
    dynamic_cast<MatrizDinamica<float>*>(A)->setValor(1, 1, 1.0f);
    dynamic_cast<MatrizDinamica<float>*>(A)->setValor(2, 0, 4.5f);
    dynamic_cast<MatrizDinamica<float>*>(A)->setValor(2, 1, 3.0f);
    
    cout << "A =" << endl;
    A->imprimir();
    cout << endl;
    
    cout << "// 2. Creacion de Matriz Estatica B (3x2)..." << endl;
    MatrizBase<float>* B = new MatrizEstatica<float, 3, 2>();
    
    dynamic_cast<MatrizEstatica<float, 3, 2>*>(B)->setValor(0, 0, 0.5f);
    dynamic_cast<MatrizEstatica<float, 3, 2>*>(B)->setValor(0, 1, 1.0f);
    dynamic_cast<MatrizEstatica<float, 3, 2>*>(B)->setValor(1, 0, 2.0f);
    dynamic_cast<MatrizEstatica<float, 3, 2>*>(B)->setValor(1, 1, 3.0f);
    dynamic_cast<MatrizEstatica<float, 3, 2>*>(B)->setValor(2, 0, 1.0f);
    dynamic_cast<MatrizEstatica<float, 3, 2>*>(B)->setValor(2, 1, 1.0f);
    
    cout << "B =" << endl;
    B->imprimir();
    cout << endl;
    
    cout << "// 3. Operacion Polimorfica (Suma)" << endl;
    cout << "SUMANDO: Matriz C = A + B ..." << endl;
    cout << "(La suma es manejada por el metodo virtual de MatrizDinamica)" << endl << endl;
    
    MatrizBase<float>* C = A->sumar(*B);
    
    
    if (C != nullptr) {
        cout << "Matriz Resultado C (3x2, Tipo FLOAT):" << endl;
        C->imprimir();
        cout << endl;
    }
    

    cout << ">> Demostracion de Limpieza de Memoria <<" << endl;
    
    cout << "Llamando al destructor de C..." << endl;
    delete C;
    
    cout << "Llamando al destructor de A..." << endl;
    delete A;
    
    cout << "Llamando al destructor de B..." << endl;
    delete B;
    
    cout << "Sistema cerrado." << endl;
    
    return 0;
}