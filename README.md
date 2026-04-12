# Prism

A standalone, headless C++ analysis engine. Accepts a C++ repository as input, analyses its structure, and produces a structured `analysis.json` file describing the codebase in full.

Prism is a pure data pipeline — no UI, no rendering, no window management. It can be run from a terminal, a CI job, or a build script. The primary consumer of its output is [Pharos](../pharos), the architecture visualisation UI, but the output schema is tool-agnostic.

______________________________________________________________________

## Usage

```bash
prism --project /path/to/repo --output analysis.json
```

### Options

| Flag | Description |
| --- | --- |
| `--project <path>` | Path to the root of the repository to analyse |
| `--output <path>` | Path to write `analysis.json` (default: `./analysis.json`) |
| `--compile-commands <path>` | Path to `compile_commands.json` (default: auto-detected in project root) |
| `--depth <level>` | Maximum hierarchy depth to analyse (default: full) |
| `--verbose` | Print progress and diagnostic output |

A `compile_commands.json` is required. Most C++ build systems can generate one — for CMake, pass `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`.

______________________________________________________________________

## Pipeline

Prism processes a repository in four sequential stages:

```
Repository (source files + compile_commands.json)
        │
        ▼
┌─────────────────────┐
│  Stage 1 — Parser   │  AST traversal via libclang
└─────────────────────┘
        │  ASTNode collection
        ▼
┌──────────────────────────────┐
│  Stage 2 — Graph Builder     │  Nodes, edges, weights, hierarchy
└──────────────────────────────┘
        │  DependencyGraph
        ▼
┌──────────────────────────────┐
│  Stage 3 — Metrics Engine    │  Complexity, coupling, compile impact
└──────────────────────────────┘
        │  Annotated DependencyGraph
        ▼
┌─────────────────────────────┐
│  Stage 4 — Exporter         │  Serialises to analysis.json
└─────────────────────────────┘
        │
        ▼
  analysis.json
```

Each stage has a single responsibility and communicates with the next via a well-defined data structure. Stages can be developed and tested independently.

### Stage 1 — Parser

Uses `libclang` to traverse the AST of every translation unit and extract raw structural facts: class and struct declarations, include relationships, function declarations and call references, inheritance relationships, and namespace boundaries. Outputs a flat `ASTNode` collection — no graph structure yet.

### Stage 2 — Graph Builder

Constructs the full graph data model from the raw `ASTNode` collection. Builds the **node hierarchy** (a strict parent/child tree from project → module → namespace → class → file → function) and the **dependency graph** (a directed graph of typed, weighted edges between nodes at any level). These are distinct structures — both are required in the output.

### Stage 3 — Metrics Engine

Walks the constructed graph and annotates every node with computed metrics. Runs as a separate pass so it has access to full graph topology. Computes structural metrics (coupling, instability, circular dependency participation), code metrics (lines of code, cyclomatic complexity, method counts), and C++-specific metrics (include depth, transitive include count, compile impact).

### Stage 4 — Exporter

Serialises the fully annotated graph to `analysis.json` using `nlohmann/json`. Owns the output schema.

______________________________________________________________________

## Output Schema

```json
{
  "metadata": {
    "project": "UmbraEngine",
    "generated_at": "2025-01-01T00:00:00Z",
    "prism_version": "0.1.0",
    "file_count": 312,
    "node_count": 1840,
    "edge_count": 4201
  },
  "nodes": [ ... ],
  "edges": [ ... ]
}
```

**Node types:** `project` `module` `namespace` `class` `struct` `file` `function` `method`

**Edge types:** `include_dependency` `inheritance` `symbol_usage` `function_call` `composition`

The schema carries both the hierarchical node tree and the flat edge list. Downstream consumers should not need to recompute one from the other.

______________________________________________________________________

## Project Structure

```
prism/
├── CMakeLists.txt
├── main.cpp
├── include/
│   ├── ast-node.hpp
│   ├── graph-node.hpp
│   ├── dependency-graph.hpp
│   ├── metrics-engine.hpp
│   ├── parser.hpp
│   ├── exporter.hpp
│   └── schema.hpp
├── src/
│   ├── dependency-graph.cpp
│   ├── metrics-engine.cpp
│   ├── parser.cpp
│   └── exporter.cpp
├── test/
│   ├── test-main.cpp
│   ├── parser.test.cpp
│   ├── graph-builder.test.cpp
│   ├── metrics-engine.test.cpp
│   └── exporter.test.cpp
```

______________________________________________________________________

## Tech Stack

| Component | Technology |
| --- | --- |
| Language | C++20 |
| Build system | CMake |
| C++ parser | libclang |
| Compilation database | compile_commands.json |
| JSON serialisation | nlohmann/json |
| Testing | Cimmerian |
| Logging | Firefly |

**Compiler requirements:** C++20 or later with `std::format` support — GCC 13+, Clang 16+, or MSVC 19.29+.

**Target platforms:** Linux (primary), macOS, Windows (secondary).

______________________________________________________________________

## Building

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

To run the test suite:

```bash
cmake --build build --target prism_tests
./build/prism_tests
```

______________________________________________________________________

## Relationship to Pharos

Prism is a dependency of Pharos, not a component of it. The boundary is `analysis.json`.

```
prism  ──produces──▶  analysis.json  ──consumed by──▶  pharos
```

Pharos does not call into Prism at runtime — it reads the output file. This means Prism can be run separately and its output cached, and Pharos can be developed and tested against a static `analysis.json` without running the full analysis pipeline.

______________________________________________________________________

## Non-Goals

- Does not render anything
- Does not manage a UI or window
- Does not perform refactoring or modify source files
- Does not support languages other than C++ in its first version
