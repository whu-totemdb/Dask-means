
#include "utils/utils.h"
using namespace Utils;


#include "KMeansBase.h"

class Lloyd : public KMeansBase {
public:
    Lloyd(int max_iterations = 30, double convergence_threshold = 1e-4);
    
    ~Lloyd();

    void run() override;

protected:
    void assignLabels() override;

    void updateCentroids() override;

};


Lloyd::Lloyd(int max_iterations = 30, double convergence_threshold = 1e-4)
    : KMeansBase(max_iterations, convergence_threshold) {}

Lloyd::~Lloyd() {}

// Lloyd: compute the distance from each data point to each centroid
void Lloyd::assignLabels() {
    for (int j = 0; j < data_scale; j++) {
        auto data_point = dataset[j];
        int nearest_centroid_id = 0;
        double nearest_centroid_dis = HUGE_VAL;

        for (int i = 0; i < k; i++) {
            double dis = distance1(data_point, centroid_list[i]->getCoordinate());

            if (dis >= nearest_centroid_dis)
                continue;
            nearest_centroid_dis = dis;
            nearest_centroid_id = i;
        }

        // assign the data point to a cluster
        labels[j] = nearest_centroid_id;
        centroid_list[nearest_centroid_id]->getCluster()->getDataIdList().push_back(j);
    }
}
