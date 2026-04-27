# Notas de Física — Orbitais Atômicos

Este documento explica a matemática por trás do código em `src/physics/`.

## A equação de Schrödinger

Para um átomo hidrogenoide (1 elétron, núcleo com carga Z):

```
Ĥψ = Eψ
```

Onde Ĥ é o hamiltoniano (energia cinética + potencial coulombiano).

## Solução: separação em coordenadas esféricas

```
ψ_nlm(r, θ, φ) = R_nl(r) · Y_lm(θ, φ)
```

- **R_nl(r)**: parte radial (depende de n, l)
- **Y_lm(θ, φ)**: harmônicos esféricos (parte angular)

## Números quânticos

- **n** (principal): energia e tamanho (1, 2, 3, ...)
- **l** (azimutal): forma (0=s, 1=p, 2=d, 3=f)
- **m** (magnético): orientação (-l..+l)

Regras: `0 ≤ l < n` e `-l ≤ m ≤ +l`.

## Parte radial R_nl(r)

Fórmula (unidades atômicas, a₀ = 1):

```
R_nl(r) = N · exp(-ρ/2) · ρ^l · L^(2l+1)_(n-l-1)(ρ)
```

Onde:
- ρ = 2Zr/n
- N = fator de normalização
- L = polinômio associado de Laguerre

**Implementação**: `RadialFunction.cpp` usa recorrência pra Laguerre.

## Parte angular Y_lm(θ, φ)

Harmônicos esféricos **reais**:

```
Y_lm(θ, φ) = N · P_l^|m|(cos θ) · { cos(mφ) se m>0
                                    { sin(|m|φ) se m<0
                                    { 1 se m=0
```

Onde P = polinômio associado de Legendre.

**Implementação**: `SphericalHarmonics.cpp` usa recorrência pra Legendre.

## Densidade de probabilidade

```
|ψ|² = (R_nl · Y_lm)²
```

Essa é a **probabilidade por unidade de volume** de encontrar o elétron em (r, θ, φ).

## Amostragem (Rejection Sampling)

Queremos N pontos distribuídos com probabilidade ∝ |ψ|².

Algoritmo:
1. Sorteia (x, z) aleatório no plano
2. Calcula |ψ(x, z)|²
3. Aceita com probabilidade |ψ|² / max(|ψ|²)
4. Repete até ter N pontos

**Implementação**: `OrbitalSampler.cpp`.

## Modelo de Bohr (aproximação clássica)

Raio da n-ésima camada:
```
r_n = n² · a₀ / Z
```

Energia:
```
E_n = -13.6 eV · Z² / n²
```

**Uso no código**: desenhamos os anéis de Bohr **por cima** da nuvem quântica pra fins didáticos (facilita identificar os níveis n=1, 2, 3).

## Limitações da aproximação hidrogenoide

Para O (8 elétrons), tratamos cada um como se fosse sozinho. Na realidade:

- Elétrons se repelem (repulsão coulombiana)
- Orbitais se distorcem (screening nuclear)
- Energias mudam (regras de Hund, configuração eletrônica)

**Solução exata**: Hartree-Fock ou DFT (não implementado aqui).

**Por que ainda funciona visualmente?**: a forma qualitativa (s esférico, p halter, d trevo) se preserva.

## Referências matemáticas

- Griffiths, *Introduction to Quantum Mechanics* (cap. 4)
- Arfken & Weber, *Mathematical Methods for Physicists* (funções especiais)
- NIST Digital Library (tabelas de orbitais)

---

**Conclusão**: o código implementa a solução analítica exata da equação de Schrödinger para hidrogenoides, com aproximação razoável pra átomos multieletrônicos.
