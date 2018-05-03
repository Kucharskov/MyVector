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

vector::vector(std::initializer_list<double> data) :
	_capacity(data.size()) {

	//Licznik instancji (COW)
	_instances = new size_t(1);

	//Zarezerwowanie miejsca w pamięci
	reserve(_capacity);

	//Wrzucenie danych z listy inicializującej
	for (auto el : data) push_back(el);
}

vector::vector(const vector &copy) {
	//Kopia płytka (COW)
	_capacity = copy._capacity;
	_size = copy._size;
	_data = copy._data;
	_instances = copy._instances;
	_min = copy._min;
	_max = copy._max;

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
	_min = copy._min;
	_max = copy._max;

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
	for (size_t i(0); i < _size; i++)
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
	std::copy_n(_data + pos + 1, _size - 1, _data + pos);
	
	//Zmniejszenie rozmiaru
	_size--;

	//Poszukiwanie min / max
	//Gdy rozmiar wynosi 1 to min = max = element 0
	if (_size == 1) _min.second = _max.second = 0;
	else {
		//Gdy usunięto element przed min / max to przesuń indeksy
		if (pos < _min.second) _min.second--;
		if (pos < _max.second) _max.second--;
		//Jeżeli wyrzucono ekstremum ustaw flagę naruszenia
		if (pos == _min.second) _min.first = true;
		if (pos == _max.second) _max.first = true;
	}
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

	//Poszukiwanie min / max
	//Przesuń indeks, jeżeli wstawiono coś przed ekstremum
	//Gdy wrzucono nowe ekstremum, ustaw indeks
	if (pos <= _min.second) _min.second++;
	if (value < _data[_min.second]) _min.second = pos;
	if (pos <= _max.second) _max.second++;
	if (value > _data[_max.second]) _max.second = pos;
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

	//Jeżeli flaga naruszenia maksimum jest ustawiona
	if (_max.first == true) {
		//Znajdź faktyczny indeks maksimum
		_max.second = std::distance(begin(), std::max_element(begin(), end()));
		//Zgaś flagę
		_max.first == false;
	}
	
	//Zwracanie wartości największej
	return _data[_max.second];
}

double vector::min() {
	//Rzucanie wyjątku
	if (empty()) throw std::length_error("Err: Can't find min in empty vector!");

	//Jeżeli flaga naruszenia minimum jest ustawiona
	if (_min.first == true) {
		//Znajdź faktyczny indeks minimum
		_min.second = std::distance(begin(), std::min_element(begin(), end()));
		//Zgaś flagę
		_min.first == false;
	}

	//Zwracanie wartości najmniejszej
	return _data[_min.second];
}

double vector::pop_back() {
	//Rzucanie wyjątku
	if (empty()) throw std::length_error("Err: Cant pop from empty vector!");

	//Wyrzucenie ostatniego elementu poprzez przycięcie go rozmiarem
	_size--;

	//Poszukiwanie min / max
	//Gdy rozmiar wynosi 1 to min = max = element 0
	if (_size == 1) _min.second = _max.second = 0;
	else {
		//Jeżeli wyrzucono ekstremum ustaw flagę naruszenia
		if (_size == _min.second) _min.first = true;
		if (_size == _max.second) _max.first = true;
	}

	//Zwrócenie usuniętego elementu
	return _data[_size + 1];
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

	//Poszukiwanie min / max
	//Gdy rozmiar wynosi 1 to min = max = element 0
	if (_size == 1) _min.second = _max.second = 0;
	else {
		//Gdy wrzucany jest kolejny element sprawdź czy jest ekstremum
		if (value < _data[_min.second]) _min.second = _size - 1;
		if (value > _data[_max.second]) _max.second = _size - 1;
	}
}

void vector::push_back(std::initializer_list<double> data) {
	//Copy-On-Write
	checkInstance();

	//Gdy w tablicy nie ma miejsca...
	if (_size + data.size() > _capacity) {
		//Zarezerwowanie wystarczającej ilości miejsca
		_capacity = _size + data.size();
		reserve(_capacity);
	}

	//Wrzucenie danych z listy inicializującej
	for (auto el : data) {
		_data[_size++] = el;

		//Poszukiwanie min / max
		//Gdy rozmiar wynosi 1 to min = max = element 0
		if (_size == 1) _min.second = _max.second = 0;
		else {
			//Gdy wrzucany jest kolejny element sprawdź czy jest ekstremum
			if (el < _data[_min.second]) _min.second = _size - 1;
			if (el > _data[_max.second]) _max.second = _size - 1;
		}
	}
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

		//Odbicie min / max
		_min.second = _size - 1 - _min.second;
		_max.second = _size - 1 - _max.second;
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

		//Poszukiwanie min / max
		//Gdy rozmiar wynosi 1 to min = max = element 0
		if (_size == 1) _min.second = _max.second = 0;
		//Posortowane elementy, więc min jest na miejscu 0, a max jest ostatnie
		_min.second = 0;
		_max.second = _size - 1;
		//Przy sortowaniu odwrotnym, min i max są zamienione
		if (reverse) std::swap(_min.second, _max.second);
	}
}