#pragma once
#include <numeric>
#include <algorithm>
#include <functional>
#include <stdexcept>

// Implementacja klasy std::vector
// Autor: Michał Kucharski (M. Kucharskov)
// Wersja: 3.5 (z dnia 3.05.2018)
// Wektor posiada inerpolację danych (odczyt wartości pośrednich)
// oraz paradygmat Copy-On-Write optymalizujący zużycie pamięci.
// Funkcje min(), max() są złożoności czasowej O(1)

class vector {
private:
	double * _data = nullptr;
	size_t _capacity = 0;
	size_t _size = 0;
	size_t _iMin, _iMax;

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
		operator double() const { return _v._data[_pos]; }
	};

public:
	//Konstrutkor
	vector(size_t = 1);

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