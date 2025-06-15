# 5-Coloring of Planar Graphs

## Description

An algorithm for coloring any planar graph with **at most 5 colors** is implemented, ensuring that no two adjacent vertices share the same color. Both unoptimized (O(n²)) and optimized (close to O(n)) versions in C++ are provided for comparison.

## What is a Planar Graph?

A **graph** is defined as a pair of sets: a set of **vertices** and a set of **edges**, where each edge connects a pair of different vertices.  
A **planar graph** is a graph that can be drawn on a plane without any edges crossing.

A **proper vertex coloring** of a graph is a rule for assigning colors (or labels) to its vertices such that no two connected vertices share the same color.

Two of the most well-known **non-planar** graphs are:
- **K₅**: the complete graph with five vertices.
- **K₃,₃**: the complete bipartite graph with three vertices in each partition.  
These graphs cannot be drawn in a plane without edge crossings.

Planar graphs are of practical interest — for example, they can represent **maps** of countries or regions. On a flat map, we can label each region with a vertex, and draw an edge between vertices if their corresponding regions share a border.  
Thus, any map can be translated into a planar graph, and vice versa.

## Implementation Details

The project includes two C++ implementations:

### 1. Unoptimized Version (O(n²))
- Based on **Euler's formula** and the **Five Color Theorem**.
- Works correctly for any planar graph.
- Guarantees that no two adjacent vertices share the same color.
- Slower on large graphs but always within 5-color constraint.

### 2. Optimized Version (Greedy, ~O(n))
- Uses a **greedy coloring algorithm**, limited to 5 colors.
- Much faster on graphs with up to 1000 vertices and many edges.
- However, since it does not include full planar checking or backtracking, in rare complex planar cases it might require more than 5 colors.
- This limitation contradicts the problem’s requirement.  
  In such cases, a more advanced approach like the **Japanese algorithm** (a proven universal solution for 5-coloring planar graphs) can be used.

## Input Format
- Number of vertices.
- List of edges (each edge connects two vertices).

## Output Format
- A color (1–5) assigned to each vertex.

---

This project demonstrates how combinatorics, geometry, and graph theory combine to solve a classic theoretical and practical problem efficiently.
