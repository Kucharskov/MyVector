# MyVector
Własna implementacja std::vector z interpolacją danych i paradygmatem copy-on-write.
## Metody
Z założenia większość metod działa identycznie jak w STL'owej implementacji vector'a.
## Metody "dodatkowe"
`double avg();` - zwraca wartość średnią z wektora  
`double interpolate(double pos)` - pozwala na interpolację danych, dla parametru całkowitego działa jak metoda at()  
`double max();` - zwraca wartość maksymalną wrzuconą do wektora  
`double min();` - zwraca wartość minimalną wrzuconą do wektora  
`void reverse();` - odwraca dane w wektorze  
`void sort(bool reverse);` - sortuje dane w wektorze w kolejności rosnącej (reverse odwraca kolejność)  
## Interpolacja
Dzięki mechanizmowi interpolacji danych, można odczytywać dane z pomiędzy komórek:
```
vector v(2);
v.push_back(0);     //v[0] = 0
v.push_back(10);    //v[1] = 10
std::cout << v.interpolate(0); //Zwraca 0
std::cout << v.interpolate(0.65); //Zwraca 6.5
```
## English

# MyVector
MyVector project is my own implementation of std::vector with data interpolation and copy-on-write paradigm implemented.
## Methods used
For an instance, most of the methods are STL-like implementations.
## Additional methods
`double avg();` - returns avarage value from a vector,
`double interpolate(double pos)` - interpolates given double value, in case of int type value, it will behave like at() method,
`double max();` - returns maximal value found in a vector,
`double min();` - returns minimal value found in a vector,
`void reverse();` - reverses an order of elements stored in a vector,
`void sort(bool reverse);` - sorts data stored in a vector in ascending order, "bool reverse" = true will cause sorting in descending order.
## Interpolation
Due to used interpolation mechanism this implementation allows to read data from between cells:
```
vector v(2);
v.push_back(0);     //v[0] = 0
v.push_back(10);    //v[1] = 10
std::cout << v.interpolate(0); //returns 0
std::cout << v.interpolate(0.65); //returns 6.5
```
