#include "vector.h"

vector::double_proxy::double_proxy(vector &vec, const size_t pos) :
	_v(vec), _pos(pos) {
}

vector::double_proxy & vector::double_proxy::operator=(const double val) {
	//Sprawdzenie instancji (COW)
	_v.checkInstance();

	//Podmiana wartości
	_v._data[_pos] = val;

	//Zwrócenie proxy z wartością
	return *this;
}

void vector::checkInstance() {
	//Przy większej ilości instancji (COW)
	if (*_instances > 1) {
		//Kopia głęboka danych
		double *temp = new double[_capacity];
		std::copy_n(_data, _size, temp);

		//Przepięcie na nową tablicę
		_data = temp;

		//Odpięcie instancji z licznika
		(*_instances)--;

		//Utworzenie nowego licznika instancji
		_instances = new size_t(1);
	}
}

bool vector::checkIndex(double pos) {
	//Zwracanie czy pozycja mieście się w zakresie danych
	return (pos < 0 || std::ceil(pos) >= _size);
}

vector::vector(size_t c) :
	_capacity(c), _size(0) {
	//Rzucanie wyjątku
	if (c == 0)	throw std::length_error("Err: Vector size can't be 0!");
	
	//Licznik instancji (COW)
	_instances = new size_t(1);

	//Zarezerwowanie miejsca w pamięci
	reserve(_capacity);
}

vector::vector(const vector &copy) {
	//Kopia płytka (COW)
	_capacity = copy._capacity;
	_size = copy._size;
	_data = copy._data;
	_instances = copy._instances;

	//Zwiększenie licznika instancji (COW)
	(*_instances)++;
}

vector::~vector() {
	//Zmniejszanie licznika instancji (COW)
	(*_instances)--;

	//Przy braku instancji usuwanie wskaźników
	if (*_instances == 0) {
		delete[] _data;
		delete _instances;
	}
}

vector & vector::operator=(const vector &copy) {
	//Kopia płytka (COW)
	_capacity = copy._capacity;
	_size = copy._size;
	_data = copy._data;
	_instances = copy._instances;

	//Zwiększenie licznika instancji (COW)
	(*_instances)++;

	//Zwracanie obiektu
	return *this;
}

bool vector::operator==(const vector &other) {
	//Muszą mieć tą samą ilość elementów
	if (_size != other._size) return false;
			
	//Jeżeli są z tej samej instancji to są równe
	if (_instances == other._instances) return true;

	//Muszą mieć tą samą pojemność
	if (_capacity != other._capacity) return false;
	
	//Sprawdzanie poszczególnych elementów
	for (size_t i = 0; i < _size; i++)
		if (_data[i] != other._data[i]) return false;
	
	//Ostatecznie równe
	return true;
}

vector::double_proxy vector::operator[](size_t pos) {
	//Zwracanie działania metody at()
	return at(pos);
}

vector::double_proxy vector::at(size_t pos) {
	//Rzucanie wyjątku
	if (checkIndex(pos)) throw std::out_of_range("Err: Invalid position to get value!");

	//Zwracanie referencji do elementu
	return double_proxy(*this, pos);
}

void vector::clear() {
	//Ustawienie ilości zajętych elementów na 0
	_size = 0;
}

void vector::erase(size_t pos) {
	//Rzucanie wyjątku
	if (checkIndex(pos)) throw std::out_of_range("Err: Invalid position to erase!");

	//Copy-On-Write
	checkInstance();

	//Przesunięcie danych
	std::copy_n(_data + pos, _size - pos, _data + pos - 1);

	//Zmniejszenie rozmiaru
	_size--;
}

void vector::insert(size_t pos, double value) {
	//Rzucanie wyjątku
	if (checkIndex(pos)) throw std::out_of_range("Err: Invalid position to insert!");

	//Copy-On-Write
	checkInstance();

	//Gdy w tablicy nie ma miejsca...
	if (_size + 1 > _capacity) {
		//Zarezerwowanie miejsca dwa razy większego
		_capacity *= 2;
		reserve(_capacity);
	}

	//Przesunięcie danych
	std::copy_n(_data + pos, _size - pos, _data + pos + 1);

	//Zwiększenie rozmiaru
	_size++;

	//Wstawienie wartości
	_data[pos] = value;
}

double vector::avg() {
	//Rzucanie wyjątku
	if (empty()) throw std::length_error("Err: Can't find avg in empty vector!");

	//Zwracanie średniej
	return std::accumulate(begin(), end(), 0.0) / _size;
}

double vector::interpolate(double pos) {
	//Rzucanie wyjątku
	if (checkIndex(pos)) throw std::out_of_range("Err: Invalid position to get value!");

	//Obliczenie dolnej granicy
	int posDown = static_cast<int>(std::floor(pos));

	//Zwracanie wartości dla pozycji całkowitej
	if (pos == posDown) return at(posDown);

	//Obliczenie górnej granicy
	int posUp = static_cast<int>(std::ceil(pos));

	//Interpolacja
	return _data[posDown] + (_data[posUp] - _data[posDown]) * ((pos - posDown) / (posUp - posDown));
}

double vector::max() {
	//Rzucanie wyjątku
	if (empty()) throw std::length_error("Err: Can't find max in empty vector!");

	//Zwracanie wartości największej
	return *std::max_element(begin(), end());
}

double vector::min() {
	//Rzucanie wyjątku
	if (empty()) throw std::length_error("Err: Can't find min in empty vector!");

	//Zwracanie wartości najmniejszej
	return *std::min_element(begin(), end());
}

double vector::pop_back() {
	//Rzucanie wyjątku
	if (empty()) throw std::length_error("Err: Cant pop from empty vector!");

	//Wyrzucanie ostatniej wartości z tablicy
	return _data[_size--];
}

void vector::push_back(double value) {
	//Copy-On-Write
	checkInstance();

	//Gdy w tablicy nie ma miejsca...
	if (_size + 1 > _capacity) {
		//Zarezerwowanie miejsca dwa razy większego
		_capacity *= 2;
		reserve(_capacity);
	}

	//Wpisanie elementu
	_data[_size++] = value;
}

void vector::reserve(size_t newSize) {
	//Copy-On-Write
	checkInstance();

	//Przerwanie w przypadku rezerwacji mniejszej ilości miejsca
	if (_size >= newSize) return;

	//Ustalenie nowej pojemności
	_capacity = newSize;

	//Kopia danych
	double *newData = new double[_capacity];
	std::copy_n(_data, _size, newData);

	//Usunięcie starej tablicy i przepięcie na nową
	delete[] _data;
	_data = newData;
}

void vector::reverse() {
	//Jeżeli tablica nie jest pusta...
	if (!empty()) {
		//Copy-On-Write
		checkInstance();

		//Odwróć zawartość tablicy
		std::reverse(begin(), end());
	}
}

void vector::sort(bool reverse) {
	//Jeżeli tablica nie jest pusta...
	if (!empty()) {
		//Copy-On-Write
		checkInstance();

		//Sortowanie (optymalizowane przez kompilator)
		if (reverse == false) std::sort(begin(), end());
		else std::sort(begin(), end(), std::greater<>());
	}
}