#include "vector.h"

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
	return (pos < 0 || pos >= _size);
}

vector::vector(size_t c) :
	_capacity(c),
	_size(0) {
	if (c == 0) {
		//TODO: Rzucanie wyjątku
		std::cout << "Err: Vector size can't be 0!\n";
	}

	//Deklaracja tablicy
	_data = new double[_capacity];

	//Licznik instancji (COW)
	_instances = new size_t(1);
}

vector::vector(vector &copy) {
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

	//Muszą mieć tą samą pojemność
	if (_capacity != other._capacity) return false;
		
	//Jeżeli są z tej samej instancji to są równe
	if (_instances == other._instances) return true;
	
	//Sprawdzanie poszczególnych elementów
	for (size_t i = 0; i < _size; i++)
		if (_data[i] != other._data[i]) return false;
	
	//Ostatecznie równe
	return true;
}

double vector::operator[](double pos) {
	//Zwracanie wartości (zabezpieczenie w metodzie at)
	return at(pos);
}

double vector::at(double pos) {
	if (checkIndex(pos)) {
		//TODO: Rzucanie wyjątku o niepoprawne odwołanie elementu
		std::cout << "Err: Invalid position to get value!\n";
	}

	//Obliczenie dolnej granicy
	int posDown = static_cast<int>(std::floor(pos));

	//Zwracanie wartości dla pozycji całkowitej
	if (pos == posDown) return _data[posDown];

	//Obliczenie górnej granicy
	int posUp = static_cast<int>(std::ceil(pos));

	//Interpolacja
	return _data[posDown] + (_data[posUp] - _data[posDown]) * ((pos - posDown)/(posUp - posDown));
}

void vector::clear() {
	//Ustawienie ilości zajętych elementów na 0
	_size = 0;
}

bool vector::empty() {
	//Wektor jest pusty gdy ma zero elementów
	return _size == 0;
}

void vector::insert(size_t pos, double value) {
	if (checkIndex(pos)) {
		//TODO: Rzucanie wyjątku
		std::cout << "Err: Invalid position to insert!\n";
	}

	//Copy-On-Write
	checkInstance();

	//Podmiana wartości
	_data[pos] = value;
}

double vector::max() {
	if (empty()) {
		//TODO: Rzucanie wyjątku
		std::cout << "Err: Can't find max in empty vector!\n";
	}

	//Zwracanie wartości największej
	return *std::max_element(begin(), end());
}

double vector::min() {
	if (empty()) {
		//TODO: Rzucanie wyjątku
		std::cout << "Err: Can't find min in empty vector!\n";
	}

	//Zwracanie wartości najmniejszej
	return *std::min_element(begin(), end());
}

double vector::pop_back() {
	if (empty()) {
		//TODO: Rzucanie wyjątku
		std::cout << "Err: Cant pop from empty vector!\n";
	}

	//Wyrzucanie ostatniej wartości z tablicy
	return _data[_size--];
}

void vector::push_back(double value) {
	//Copy-On-Write
	checkInstance();

	//Gdy w tablicy nie ma miejsca...
	if (_size + 1 > _capacity) {
		//Deklaracja nowej dwa razy większej
		_capacity *= 2;
		double *newData = new double[_capacity];

		//Kopia danych
		std::copy_n(_data, _size, newData);

		//Usunięcie starej tablicy i przepięcie na nową
		delete[] _data;
		_data = newData;
	}

	//Wpisanie elementu
	_data[_size++] = value;
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
