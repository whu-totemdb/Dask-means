#include <vector>
#include <algorithm>

class KMeansBase {
protected:
    std::vector<std::vector<double>> dataset;
    std::vector<std::vector<double>> centroids;
    std::vector<int> labels;    // noting which cluster the data point is assigned to
    int dataScale;
    int k;
    int maxIterations;
    double convergenceThreshold;

public:
    KMeansBase(int dataScale, int k, int maxIterations = 30, double convergenceThreshold = 1e-4);

    virtual ~KMeansBase() {}

    virtual void run(const std::vector<std::vector<double>>& inputData) {}

    virtual std::vector<int> output(const std::vector<std::vector<double>>& inputData) = 0;

protected:
    virtual void initializeCentroids();

    virtual void assignLabels() {}  // assign all data points to a cluster with an intiger label

    virtual void updateCentroids(const std::vector<int>& labels) {}

    bool hasConverged(const std::vector<std::vector<double>>& newCentroids) { return false; }
};

KMeansBase::KMeansBase(int dataScale, int k, int maxIterations = 30, double convergenceThreshold = 1e-4)
    : dataScale(dataScale), k(k), maxIterations(maxIterations), convergenceThreshold(convergenceThreshold) {
        labels = std::vector<int>(dataScale, 0);
    }

// initialize
void KMeansBase::initializeCentroids() {

}
