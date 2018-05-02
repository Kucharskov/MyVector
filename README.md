# MyVector
Własna implementacja std::vector z interpolacją danych i paradygmatem copy-on-write.
## Metody
Z założenia większośc metod działa identycznie jak w STL'owej implementacji vector'a.
## Metody "dodatkowe"
`double avg();` - zwraca wartość średnią z wektora  
`double interpolate(double pos)` - pozwala na interpolację danych, dla parametru całkowitego działa jak metoda at()  
`double max();` - zwraca wartość maksymalną wrzuconą do wektora  
`double min();` - zwraca wartość minimalną wrzuconą do wektora  
`void reverse();` - odwraca dane w wektorze  
`void sort(bool reverse);` - sortuje dane w wektorze w kolejności rosnącej (reverse odwraca kolejnosć)  
## Interpolacja
Dzięki mechanizmowi interpolacji danych, można odczytywać dane z pomiędzy komórek:
```
vector v;
v.push_back(0);     //v[0] = 0
v.push_back(10);    //v[1] = 10
std::cout << v.interpolate(0.5); //Zwraca 5
std::cout << v.interpolate(0.65); //Zwraca 6.5
```
