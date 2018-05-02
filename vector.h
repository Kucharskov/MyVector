#pragma once
#include <algorithm>
#include <functional>
#include <stdexcept>

// Implementacja klasy std::vector
// Autor: Michał Kucharski (M. Kucharskov)
// Wersja: 2.1 (z dnia 2.05.2018)
// Wektor posiada inerpolację danych (odczyt wartości pośrednich)
// oraz paradygmat Copy-On-Write optymalizujący zużycie pamięci

class vector {
private:
	double * _data = nullptr;
	size_t _capacity = 0;
	size_t _size = 0;

	//Copy-On-Write
	size_t * _instances;
	void checkInstance();

	//Funkcje prywatne
	bool checkIndex(double);

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
	double & operator[](size_t);

	//Metody const
	size_t size() const { return _size; };
	size_t capacity() const { return _capacity; };
	size_t instances() const { return *_instances; };
	bool empty() const { return _size == 0; };

	//Metody
	double & at(size_t);
	void clear();
	void erase(size_t);
	void insert(size_t, double);
	double pop_back();
	void push_back(double);
	void reserve(size_t);
	void reverse();
	void sort(bool = false);

	//Metody stricte matematyczne
	double interpolate(double);
	double max();
	double min();

	//Iteratory dla range-based for (CPP 11)
	double * begin() { return &_data[0]; };
	double * begin() const { return &_data[0]; };
	double * end() { return &_data[_size]; };
	double * end() const { return &_data[_size]; };
};