# Parallelizable Implementation of Prim's Algorithm  

This project implements **Prim's algorithm** for constructing a **Minimum Spanning Tree (MST)** using **sequential processing**, **OpenMP**, **OpenMPI**, and their combination.  

## Input and Output Data  

The program accepts the following command-line arguments:  
- **Input text file** containing an adjacency matrix of size `n x n`  
- **Output text file** where the MST result is written in the format `Edge - Weight`  
- **Number `n`**, representing the dimension of the square matrix  

For testing purposes, a symmetric matrix of two-digit numbers is generated, with zeros on the main diagonal.  

---

## 1️⃣ Sequential Implementation  

In this case, the algorithm executes **without parallelization**. The process includes:  
- Generating the matrix and writing it to `input.txt`  
- Running the algorithm starting from **node 0**  
- In each iteration, finding the edge that connects a **visited and unvisited node** with the minimal weight  
- Adding the selected edge to the MST and marking the node as visited  
- Measuring execution time using `clock_t` from the `<ctime>` library  

> 📝 *Matrix generation does not affect execution time measurement, as the matrix is generated before the measurement starts.*  

---

## 2️⃣ OpenMP Optimization  

This version utilizes **OpenMP** to parallelize key parts of the algorithm:  
- **`minEdge()` function** – parallelizes the loop that iterates through the `key` array to find the minimum value  
- **`primOpenMP()` function** – parallelizes the loop that updates the keys of each neighbor of the current node  

Both functions use **`#pragma omp parallel for`**, allowing loops to execute in parallel across multiple processor cores.  

Execution time measurement is performed using `omp_get_wtime()` from `<omp.h>`, as it accurately measures parallel code execution time.  

---

## 3️⃣ OpenMPI Optimization  

This version utilizes **OpenMPI**, where:  
- Each processor generates a **weight matrix** and computes the MST  
- The program runs on a specified number of processes, where **process with rank 0** performs computations while others wait  
- Parallelization occurs **by distributing tasks among processes**  
- Each process receives a **submatrix** of the adjacency matrix and computes the locally smallest edge weight  
- All processes send their results to process rank 0, which determines the **global minimum edge weight**  

Execution time is measured using `MPI_Wtime()`, ensuring accurate timing across multiple processors.  

---

## 4️⃣ Combined OpenMP and OpenMPI Implementation  

This version combines:  
- **`primMST()`** from the OpenMPI version  
- **`minEdge()`** from the OpenMP version  

This approach enables **parallelization at both thread and process levels**, achieving **maximum algorithm optimization**.  

