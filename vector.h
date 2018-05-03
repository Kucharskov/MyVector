#pragma once
#include <numeric>
#include <algorithm>
#include <functional>
#include <stdexcept>

// Implementacja klasy std::vector
// Autor: Michał Kucharski (M. Kucharskov)
// Wersja: 4.1 (z dnia 4.05.2018)
// Wektor posiada inerpolację danych (odczyt wartości pośrednich)
// oraz paradygmat Copy-On-Write optymalizujący zużycie pamięci.
// Funkcje min(), max() są złożoności czasowej O(1)

class vector {
private:
	double * _data = nullptr;
	size_t _capacity = 0;
	size_t _size = 0;

	//Przechowywanie indeksów ekstremów
	//bool - flaga naruszenia ekstremum
	//size_t - indeks wartości ekstremum
	std::pair<bool, size_t> _min, _max;

	//Copy-On-Write
	size_t * _instances;
	void checkInstance();

	//Funkcje prywatne
	bool checkIndex(double);

	//Klasa pośrednicząca w dostepie do danych (COW)
	class double_proxy {
	private:
		vector& _v;
		size_t _pos;

	public:
		//Konstruktor
		double_proxy(vector&, const size_t);

		//Operatory 
		double_proxy& operator=(const double);
		double_proxy& operator=(const double_proxy&);
		operator double() const { return _v._data[_pos]; }
	};

public:
	//Konstrutkor domyślny
	vector(size_t = 1);

	//Kontruktor inicializujący
	vector(std::initializer_list<double>);

	//Konstruktor kopiujący
	vector(const vector &);

	//Destruktor
	~vector();

	//Operatory
	vector &operator=(const vector &);
	bool operator==(const vector &);
	double_proxy operator[](size_t);

	//Metody const
	size_t size() const { return _size; }
	size_t capacity() const { return _capacity; }
	size_t instances() const { return *_instances; }
	bool empty() const { return _size == 0; }

	//Metody
	double_proxy at(size_t);
	void clear();
	void erase(size_t);
	void insert(size_t, double);
	double pop_back();
	void push_back(double);
	void push_back(std::initializer_list<double>);
	void reserve(size_t);
	void reverse();
	void sort(bool = false);

	//Metody stricte matematyczne
	double avg();
	double interpolate(double);
	double max();
	double min();

	//Iteratory dla range-based for (CPP 11)
	double * begin() { return &_data[0]; };
	double * begin() const { return &_data[0]; };
	double * end() { return &_data[_size]; };
	double * end() const { return &_data[_size]; };
};