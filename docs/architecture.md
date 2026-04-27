# Documentação de Arquitetura

Este documento explica as decisões arquiteturais do projeto e como as camadas se comunicam.

## Princípios de design aplicados

1. **Separation of Concerns**: cada camada tem responsabilidade única
2. **Dependency Inversion**: camadas altas não dependem de implementações concretas
3. **Data-Driven**: configurações vêm de JSON, não de código
4. **Single Responsibility**: cada classe faz UMA coisa

## Estrutura em camadas

```
Application (orquestração)
    ↓
Physics ← → Rendering
    ↓           ↓
 Data      Platform → UI
```

- **Physics**: cálculos quânticos, independente de tela
- **Rendering**: OpenGL, shaders, câmera
- **Platform**: janela (GLFW), input
- **UI**: Dear ImGui, painéis educativos
- **Data**: JSON com elementos e descrições

## Fluxo de um frame

1. **Input**: GLFW captura teclas/mouse
2. **Update**: Application checa se precisa regerar orbitais
3. **Render**: desenha anéis → nuvem → UI
4. **Swap**: mostra na tela

## Por que estas escolhas?

- **OpenGL moderno**: prepara pra 3D, performance escalável
- **Dear ImGui**: simples, integra direto com OpenGL
- **JSON**: adicionar elementos sem recompilar
- **Separação R/Y**: testa radial e angular isoladamente

Veja README.md para detalhes de uso.
