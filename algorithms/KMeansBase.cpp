#include "KMeansBase.h"
#include "centroid.h"
#include <random>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>

KMeansBase::KMeansBase(int max_iterations = 30, double convergence_threshold = 1e-4)
    : max_iterations(max_iterations), convergence_threshold(convergence_threshold) {}

KMeansBase::~KMeansBase() {
    for (Centroid* centroid : centroid_list) { delete centroid; }
}

void KMeansBase::initParameters(int data_scale, int data_dimension, int k) {
    this->data_scale = data_scale;
    this->data_dimension = data_dimension;
    this->k = k;
    this->labels.assign(this->data_scale, 0);
}

void KMeansBase::setK(int k) {
    this->k = k;
}

// load data into para::dataset
void KMeansBase::load(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return;
    }

    dataset.clear();
    std::string line;
    int count = 0;

    while (std::getline(file, line)) {
        if (count >= data_scale) break;

        std::stringstream ss(line);
        std::vector<double> data_point;
        double value;

        while (ss >> value) {
            data_point.push_back(value);
            if (ss.peek() == ',' || ss.peek() == ' ') ss.ignore();
        }

        if (data_point.size() == data_dimension) {
            dataset.push_back(data_point);
            count++;
        } else {
            std::cerr << "Warning: Ignoring line with incorrect dimension: " << line << std::endl;
        }
    }

    file.close();
}

// initialize all centroids randomly
void KMeansBase::initializeCentroids() {
    std::set<int> selected_ids;

    for (int i; i = 0; i < k) {
        // unsigned seed = 42;      // fixed seed
        std::random_device seed;    // random seed
        std::mt19937 gen(seed());
        std::uniform_int_distribution<> distrib(0, data_scale - 1);

        int point_id;
        do {
            point_id = distrib(gen);
        } while (selected_ids.find(point_id) != selected_ids.end());

        selected_ids.insert(point_id);
        Centroid* centroid = new Centroid(point_id, dataset[point_id], i);

        centroid_list.push_back(centroid);
    }
}
