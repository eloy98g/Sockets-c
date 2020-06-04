#include <vector>
#include <cassert>

#ifndef TABLERO_HPP
#define TABLERO_HPP

#define BOMBA -1

class Tablero{
	private:
		std::vector < std::vector <int> > tablero_;

		void inline setCasilla(int i, int j, int valor){
			#ifndef NDEBUG
				assert(i >= 0);
				assert(i < 10);
				assert(j >= 0);
				assert(j < 10);
			#endif

			tablero_[i][j] = valor;
		}

		void asignarValorCasilla(int i, int j);
	public:
		inline Tablero():tablero_(10){ for(int i = 0; i < 10; i++) tablero_[i].resize(10);}

		void iniciarTablero();
		void mostrarTablero() const;

		int inline getCasilla(int i, int j) const{
			#ifndef NDEBUG
				assert(i >= 0);
				assert(i < 10);
				assert(j >= 0);
				assert(j < 10);
			#endif

			return tablero_[i][j];
		}

		bool inline hasBomba(int i, int j) const{return getCasilla(i, j) == BOMBA;}
};

#endif
