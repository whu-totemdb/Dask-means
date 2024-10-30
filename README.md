# Dask-means-cpp
## 1 Introduction

***Dask-means*** is a fast, memory-efficient, and cost-predictable <u>**da**</u>taset <u>**s**</u>implification **<u>$k$</u>-means** algorithm for large-scale spatial vectors. This repo holds the source code and scripts for reproducing the key experiments of our paper: *On Simplifying Large-Scale Spatial Vectors: Fast, Memory-Efficient, and Cost-Predictable k-means*.

## 2 Datasets

The datasets we use are all low-dimensional (2-3 dimensional) spatial vectors, with brief information as shown in the list below:

|                           Datasets                           | Dataset Scale | Dimension |                         Description                          |
| :----------------------------------------------------------: | :-----------: | :-------: | :----------------------------------------------------------: |
| **[T-drive](https://www.microsoft.com/en-us/research/publication/t-drive-trajectory-data-sample/)** |     11.3M     |     2     | The T-Drive trajectory dataset contains GPS trajectories of 10,357 taxis in Beijing from February 2 to February 8, 2008. |
| **[Proto](https://figshare.com/articles/dataset/Porto_taxi_trajectories/12302165?file=22677902)** |     1.27M     |     2     | A CSV dataset containing taxi trajectories recorded over one year (from 2013/07/01 to 2014/06/30) in the city of Porto, in Portugal. |
|      **[Argo-AVL](https://www.argoverse.org/av2.html)**      |     17.7M     |     2     | Trajectory dataset from Argoverse, recording the trajectory data of autonomous vehicles. |
| **[Argo-PC](https://github.com/ApolloScapeAuto/dataset-api/tree/master/3d_detection_tracking)** |     383M      |     3     | Point cloud dataset from Argoverse, representing lidar-detected objects surrounding vehicles. |
| **[3D-RD](https://networkrepository.com/3D-spatial-network.php)** |    0.434M     |     3     | The 3D-spatial-network dataset is a machine learning dataset that contains 3D road network information of the North Jutland region in Denmark. |
|            **[Shapenet](https://shapenet.org/)**             |      10M      |     3     | Shapenet is a widely used point cloud dataset for 3D shape understanding and analysis. |


## 3 Comparison Algorithms

#### [Least squares quantization in PCM](https://hal.science/hal-04614938/document)

- The most widely used $k$-means algorithm.
- The code is in `./algorithms/Lloyd.h` and `./algorithms/Lloyd.cpp`. 

#### [Using the Triangle Inequality to Accelerate k-Means](https://cdn.aaai.org/ICML/2003/ICML03-022.pdf)

- For each spatial vector, this algorithm maintains one global upper bound and $k$ lower bounds, and uses the triangle inequality to accelerate $k$-means.
- The code is in `./algorithms/Elkan.h` and `./algorithms/Elkan.cpp`.

#### [Making k-means even faster](https://epubs.siam.org/doi/pdf/10.1137/1.9781611972801.12)

- Using the triangle inequality. But for each spatial vector, only one upper bound and the minimum lower bound to any cluster centroids are maintained.
- The code is in `./algorithms/Hamerly.h` and `./algorithms/Hamerly.cpp`.

#### [Accelerated k-means with adaptive distance bounds](http://opt.kyb.tuebingen.mpg.de/papers/opt2012_paper_13.pdf)

- For each spatial vector, the algorithm sorts all centroids according to the distance to the spatial vector and then maintains $1<b<k$ nearest distances as lower bounds.
- The code is in `./algorithms/Drake.h` and `./algorithms/Drake.cpp`.

#### [Yinyang K-Means: A Drop-In Replacement of the Classic K-Means with Consistent Speedup](https://proceedings.mlr.press/v37/ding15.pdf)

- The algorithm groups the clusters using classic $k$-means first and maintains bounds for each group. Then it implements a pruning method with multiple filters to accelerate $k$-means.
- The code is in `./algorithms/Yinyang.h` and `./algorithms/Yinyang.cpp`.

#### [A Fast Adaptive k-means with No Bounds](https://par.nsf.gov/servlets/purl/10286756)

- Organize spatial vectors using a hyperspherical structure to avoid boundary maintenance.
- The code is in the `./algorithms/NoBound.h`.

#### [A Dual-Tree Algorithm for Fast k-means Clustering with Large k](https://epubs.siam.org/doi/pdf/10.1137/1.9781611974973.34)

- Organize spatial vectors using a dual-tree structure to accelerate the execution of the $k$-means algorithm.
- The code is in `./algorithms/DualTree.h` and `./algorithms/DualTree.cpp`.

## 4 How to Run Dask-means

### 4.1 Code with comparison algorithms

You can find all source code of comparison algorithms in `./algorithms/`. 

**Parameter configuration:**

You can run the code in `./interface.cpp`. But before that, several parameters should be set:

```c++
// rewrite your dataset path here, for example
std::string data_path = "../dataset/dataset_name.txt";
std::string output_path = "../output/output_name.csv";

int data_scale = 1000000;       // total data scale
int data_dimension = 2;         // data dimension of the dataset
std::vector<int> k_list(3);     // "k" list in k-means
k_list[0] = 100;
k_list[1] = 1000;
k_list[2] = 10000;
```

**Compile and run:**

Go to the project file `./dask-means-cpp/`, and then enter the following command in the terminal:

```sh
mkdir build
cd build
cmake ..
make
./DaskMeans
```

### 4.2 Cost Model

**Runtime Prediction**

- You can find all the relevant code for runtime adjustments in `./predictor/`.
- The code for runtime prediction is in the file `./predictor/main.py`.
- The code for runtime adjustment is in the file `./predictor/GaussinProcess.py`.

### 4.3 Run Dask-means on Android

We added a JNI file `./interface_jni.cpp`, and within it, we implemented the Java interfaces for various $k$-means algorithms. To run Dask-means on Android, you should follow these several steps:
- Firstly, you need to clone this project into the `./main/cpp/` folder in your Android project.
- Secondly, you need to declare the position of the camke file in `build.gradle(:app)` to expose all the cpp files to your Android project.
  ```java
  externalNativeBuild {
        cmake {
            path "./src/main/cpp/CMakeLists.txt"
        }
  }
  ```
- Then, you need to declare all the functions in the `MainActivity.java` by entering the following code:
  ```java
  public native double testLloyd();
  public native double testDaskMeans();
  public native double testNoBound();
  public native double testDualTree();
  public native double testHamerly();
  public native double testDrake();
  public native double testYinYang();
  public native double testElkan();
  ```
