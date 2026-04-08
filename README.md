# Progetto_infolab

Progetto per il corso di **Informatica con Laboratorio**.

Implementazione in C++ di un parser e analizzatore di reti bayesiane
in formato `.bif`, con più varianti algoritmiche.

## Compilazione

```bash
# Brute Force
g++ .\builder.cpp .\parser.cpp .\analyzer_brute_force.cpp .\Main_bif_analyzer.cpp -o main_brute_force

# Independent Parents
g++ .\builder.cpp .\parser.cpp .\analyzer_independent_parents.cpp .\Main_bif_analyzer.cpp -o main_independent_parents

# Merged
g++ .\builder.cpp .\parser.cpp .\analyzer_merged.cpp .\Main_bif_analyzer.cpp -o main_merged

# Raffinato
g++ .\builder.cpp .\parser.cpp .\analyzer_raffinato.cpp .\Main_bif_analyzer.cpp -o main_raffinato
```

## Utilizzo esempio

```bash
./main_brute_force 
```
