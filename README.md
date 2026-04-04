# Maps 2110 - Flight Route Search Project

This workspace contains two C++ code paths around the same flight-network idea:

1. A modern route-search CLI in `source files/` (`routeSearch`)
2. A legacy/project path in `project/` (`WA`, `graphGen`)

The route data is in `data files/flight.txt` and city names are in `data files/city.name`.

## Workspace Layout

- `source files/`
  - `main.cpp` - CLI entry (`routeSearch <question#> ...`)
  - `graph.cpp`, `graph.h` - adjacency-list graph and shortest-path helper
  - `search.cpp`, `search.h` - question logic (Q1, Q3, Q4)
- `data files/`
  - `flight.txt` - directed flight graph input
  - `city.name` - city list used by legacy tools
- `project/`
  - `WA.cpp` - legacy adjacency-matrix implementation (supports Q3 and Q4)
  - `graphGen.cpp` - random graph generator utility
- `build/docs/`
  - `README.txt` - older documentation notes
  - `makefile` - minimal legacy placeholder

## Prerequisites

- C++17 compiler (examples below use MinGW g++)
- On Windows in this workspace, examples use:
  - `C:/msys64/mingw64/bin/g++.exe`

## Build and Run (Main CLI)

### Build routeSearch

From workspace root:

```powershell
Set-Location "c:/Users/Mercu/OneDrive/Desktop/Maps 2110/source files"
C:/msys64/mingw64/bin/g++.exe -std=c++17 main.cpp graph.cpp search.cpp -o routeSearch.exe
```

### Important runtime note

`routeSearch` reads `flight.txt` by relative path. Run it from `data files/` so the input is found.

```powershell
Set-Location "c:/Users/Mercu/OneDrive/Desktop/Maps 2110/data files"
& "c:/Users/Mercu/OneDrive/Desktop/Maps 2110/source files/routeSearch.exe" <question#> [args...]
```

## routeSearch Usage

General form:

```text
routeSearch <question#> [args...]
```

### Q1

```text
routeSearch 1 <city_A> <city_B> <max_connections>
```

### Q3

```text
routeSearch 3 <city_A>
```

Expected output form:

```text
city_A to city_x to city_y ... to city_z to city_A
smallest number of connection: N
```

If no valid route exists:

```text
There is no such a route.
```

### Q4

```text
routeSearch 4 <city_A> <city_B> <city_C>
```

Expected output form:

```text
You three should meet at city_D
Route for first person: city_A to city_x ... to city_D (3 connections)
Route for second person: city_B to city_y ... to city_D (1 connections)
Route for third person: city_C to city_z ... to city_D (0 connections)
Total number of connection: 4
```

If no valid meeting city exists:

```text
There is no such a city.
```

## Legacy Tools (project/)

### Build WA

```powershell
Set-Location "c:/Users/Mercu/OneDrive/Desktop/Maps 2110/project"
C:/msys64/mingw64/bin/g++.exe -std=c++17 WA.cpp -o WA.exe
```

### Run WA

Run from a directory where `city.name` and `flight.txt` are available (for example `data files/`):

```powershell
Set-Location "c:/Users/Mercu/OneDrive/Desktop/Maps 2110/data files"
& "c:/Users/Mercu/OneDrive/Desktop/Maps 2110/project/WA.exe" 3 "<city_A>"
& "c:/Users/Mercu/OneDrive/Desktop/Maps 2110/project/WA.exe" 4 "<city_A>" "<city_B>" "<city_C>"
```

### Build graphGen

```powershell
Set-Location "c:/Users/Mercu/OneDrive/Desktop/Maps 2110/project"
C:/msys64/mingw64/bin/g++.exe -std=c++17 graphGen.cpp -o graphGen.exe
```

## Troubleshooting

- `Error: could not open flight.txt`
  - Run from `data files/`, or copy/link `flight.txt` to the executable working directory.
- Route output always says no route
  - Verify exact city spelling and punctuation (for example `Hong Kong, SAR`).
- Build fails with compiler not found
  - Update the g++ path to your installed toolchain.

## Notes

- The graph is directed: `A -> B` does not imply `B -> A`.
- City names are case-sensitive and punctuation-sensitive because they are parsed as exact strings.
