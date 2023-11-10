## Zadanie 01

### Zadanie - Niemal typowa gra w życie
### Idea

Program prowadzi symulację podobną do Gry w życie. Zasady zabawy zostały jednak zmienione i dodany został dodatkowy czynnik w postaci zanieczyszczenia środowiska. Zanieczyszczenia umożliwiają zmiany reguł: np. duże zanieczyszczenie wpływa na przeżywalność komórek.

Działanie programu można zobaczyć w postaci prostej animacji. Po lewej stronie stan komórek. Po stronie prawej poziom zanieczyszczenia. Uwaga: jedna komórka ma szerokość dwóch znaków. Przepraszam za artefakty, które wygenerowały się w trakcie zapisu stanu terminala.

Animacja
Wersja sekwencyjna kodu

Udostępniam kod w wersji sekwencyjnej. Implementacja interfejsu Life to LifeSequentialImplementation. UWAGA: ta wersja działa tylko z jednym procesem. Main.cpp uwzględnia MPI - dzięki tej części kodu można zobaczyć jak funkcjonować będzie wywoływanie metod Life w wersji współbieżnej. Warto zwrócić uwagę na to, które z metod (i kiedy) wykonywane są wyłącznie w procesie o numerze 0, a jakie wykonane zostaną wszystkimi procesami.

### Zadanie

Zadanie polega na napisaniu kodu, który używając MPI przyspieszy obliczenia.

#### Wymagania dla aplikacji równoległej:

- Wersja równoległa i sekwencyjna muszą dawać ten sam wynik.
- Dodatkowe procesy mają przyspieszać obliczenia.
- Wszystkie procesy mają uczestniczyć w obliczeniach. Czyli, już dwa procesy mają przyspieszyć rachunek.
- Program ma działać efektywnie - tj. dostając do dyspozycji kolejne rdzenie/procesory ma skracać czas potrzebny do uzyskania efektu.
- Kod musi dać się skompilować i uruchomić na udostępnianych komputerach Wydziałowych (klaster). Proszę używać MPI w wersji OpenMPI.

### Dostarczanie rozwiązania:

Rozwiązanie (klasa LifeParallelImplementation) ma rozszerzać klasę Life
Klasa LifeParallelImplementation ma posiadać konstruktor bezparametrowy.
Nagłówek o nazwie LifeParallelImplementation.h - proszę również dostarczyć.
Pliki .cpp i .h proszę wgrywać do systemu osobno.
Uwaga: jeśli ktoś z Państwa chce dostarczyć więcej niż jedno rozwiązanie w ramach jednego terminu, to proszę zadbać o ponowne wgranie zarówno plików .cpp, jak i jeśli jest taka potrzeba, odpowiedniego pliku .h - nazwy tych plików muszą do siebie pasować. Czyli np. LifeParallelImplementation2.cpp i LifeParallelImplementation2.h Proszę jednak w include nadal używać LifeParallelImplementation.h (pliki otrzymają odpowiednie nazwy przed ich kompilacją).
Warto zadbać o to, aby Państwa kod nie wyświetlał komunikatów na terminalu. Coś takiego może znacząco pogorszyć efektywności pracy programu.
Proszę za pomocą include wskazywać wyłącznie pliki nagłówkowe (.h)