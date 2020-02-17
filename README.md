# SI unit types

Basic representation of units of measurement in the type system. Allows
calculating with numbers which have strong units of measurement, as well as
printing them.

Example:

``` cpp
auto distance = 13421.1232_m;
auto time = 21.6_s;
auto speed = distance / time;

std::cout << speed << '\n'; // Prints 289896.26112 m s^-1
```
