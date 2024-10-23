import matplotlib
import torch
import matplotlib.pyplot as plt
import gpytorch
from matplotlib import font_manager
from sklearn.metrics import mean_squared_error, mean_absolute_error
from main import MyNonLinearRegression
import numpy as np

matplotlib.use('Agg')
plt.style.use("bmh")
plt.rcParams["image.cmap"] = "Blues"


class SimpleSincKernel(gpytorch.kernels.Kernel):
    has_lengthscale = True

class BaseGPModel(gpytorch.models.ExactGP):
    def __init__(self, train_x, train_y, likelihood):
        super().__init__(train_x, train_y, likelihood)
        # self.mean_module = gpytorch.means.ConstantMean()
        # self.mean_module = gpytorch.means.LinearMean(1)
        # self.mean_module = gpytorch.means.ZeroMean()
        self.covar_module = gpytorch.kernels.RBFKernel()
        self.covar_module = SimpleSincKernel()

    def forward(self, x):
        mean_x = self.mean_module(x)
        covar_x = self.covar_module(x)
        return gpytorch.distributions.MultivariateNormal(mean_x, covar_x)


class GpExecution:
    def __init__(self):
        self.xs = torch.linspace(0, 28, 141).unsqueeze(1)
        # self.xs = torch.linspace(0, 35, 180).unsqueeze(1)
        self.index = None
        self.GPmodel = None
        self.likelihood = None

        # train mnlr
        self.mnlr = MyNonLinearRegression(degree=4)
        self.mnlr.load_data()
        self.value_predict = np.array(self.mnlr.predict())
        self.value_real = np.array(self.mnlr.y_test)

        # set (x,y) for GP
        # self.train_x = torch.tensor([self.xs[i] for i in range(1, 145, 5)][:self.index])
        # self.train_y = torch.tensor([self.value_real[i] / self.value_predict[i] for i in range(29)][:self.index])
        self.train_x = None
        self.train_y = None
        self.row_value_real = None
        self.row_value_perdict = None

    def set_train_xy(self, row, index):
        self.index = index
        self.row_value_real = self.value_real[row]
        self.row_value_perdict = self.value_predict[row]
        # print(self.row_value_real)
        # print(self.row_value_perdict)

        self.train_x = torch.tensor([self.xs[i] for i in range(0, 141, 5)][:index], dtype=torch.float32)
        # self.train_x = torch.tensor([self.xs[i] for i in range(1, 180, 5)][:index], dtype=torch.float32)
        self.train_y = torch.tensor([self.row_value_real[i] / self.row_value_perdict[i] for i in range(29)][:index],
                                    dtype=torch.float32)
        # print(self.train_x.shape)
        # print(self.train_y.shape)

    def build_gp_model(self):
        lengthscale = 100000
        noise = 1e-4

        self.likelihood = gpytorch.likelihoods.GaussianLikelihood()
        self.GPmodel = BaseGPModel(self.train_x, self.train_y, self.likelihood)

        # fix the hyperparameters
        self.GPmodel.covar_module.lengthscale = lengthscale
        self.GPmodel.likelihood.noise = noise
        self.GPmodel.mean_module.constant = 1

        self.GPmodel.eval()
        self.likelihood.eval()

    def draw_gp_map(self, destination="image/2.4.5.pdf"):
        with torch.no_grad():
            predictive_distribution = self.likelihood(self.GPmodel(self.xs))
            predictive_mean = predictive_distribution.mean
            predictive_lower, predictive_upper = predictive_distribution.confidence_region()

            torch.manual_seed(0)
            # samples = predictive_distribution.sample(torch.Size([5]))

        plt.figure(figsize=(8, 6))
        ax = plt.gca()
        ax.spines['right'].set_visible(False)
        ax.spines['top'].set_visible(False)
        ax.set_facecolor('none')
        ax.patch.set_alpha(0.0)
        ax.grid(False)
        ax.tick_params(axis='both', which='major', labelsize=15)

        # plt.plot(xs, ys, label="objective", c="28")
        plt.scatter(self.train_x, self.train_y, marker="x", c="k", label="actual ratio")
        plt.plot(self.xs, predictive_mean, label="mean of predicted ratio")
        plt.fill_between(
            self.xs.flatten(), predictive_upper, predictive_lower, alpha=0.3, label="95% Confidence Interval"
        )
        # plt.plot(self.xs, samples[0, :], alpha=0.5, label="samples")
        # for i in range(1, samples.shape[0]):
        #     plt.plot(self.xs, samples[i, :], alpha=0.5)
        font_properties = font_manager.FontProperties(size=16)
        plt.legend(loc='upper left', fontsize=15, frameon=True, facecolor='none', edgecolor='none', prop=font_properties)
        plt.savefig(destination)

    def get_gp_error(self):
        with torch.no_grad():
            # Create a tensor from a list of tensors selected by indices
            input_x = torch.tensor([self.xs[i] for i in range(0, 141, 5)])
            given_x_distribution = self.likelihood(self.GPmodel(input_x))
            given_x_mean = given_x_distribution.mean

        rate = [mean.item() for mean in given_x_mean]
        # print(rate)
        real_time = self.row_value_real
        predict_time = [self.row_value_perdict[i] * rate[i] for i in range(29)]
        return predict_time[self.index]
        # return [real_time[self.index + 1], predict_time[self.index + 1], self.row_value_perdict[self.index + 1]]
        # print(real_time)
        # print(predict_time)
        # print([mean_squared_error([self.row_value_perdict[i]], [self.row_value_real[i]])
        #        for i in range(len(self.row_value_perdict))])    # mse in the first phase
        #
        # mse = mean_squared_error([real_time[self.index + 1]], [predict_time[self.index + 1]])
        # print("index: ", self.index)
        # print("mse:", mse)
        # return mse


def error_calculator():
    iter = 10
    adjust_map = np.zeros((201, iter))

    gp = GpExecution()
    for row in range(201):
        for index in range(iter):
            gp.set_train_xy(row, index)
            gp.build_gp_model()
            # gp.draw_gp_map()
            predict_time = gp.get_gp_error()
            adjust_map[row][index] = predict_time

    real_time = np.array(gp.value_real).T
    adjust_map = np.array(adjust_map).T
    # print(self.y_test.shape)
    # print(predictions.shape)
    print("======== print error ========")
    mse_list = []
    for i in range(iter):
        mse = mean_squared_error(real_time[i], adjust_map[i])
        # print(f"{i}-th iteration's MSE: ", mse)
        mse_list.append(mse)
    print("mse: ", mse_list)

    mae_list = []
    for i in range(iter):
        mae = mean_absolute_error(real_time[i], adjust_map[i])
        # print(f"{i}-th iteration's MSE: ", mse)
        mae_list.append(mae)
    print("mae: ", mae_list)

    wmape_list = []
    for i in range(iter):
        wmape = np.sum(np.abs(real_time[i] - adjust_map[i])) / np.sum(np.abs(real_time[i]))
        wmape_list.append(wmape)
    print("WMAPE: ", wmape_list)

    smape_list = []
    for i in range(iter):
        smape = 100 / len(real_time[i]) * np.sum(
            2 * np.abs(adjust_map[i] - real_time[i]) / (np.abs(real_time[i]) + np.abs(adjust_map[i])))
        smape_list.append(smape)
    print("SMAPE:", smape_list)


if __name__ == '__main__':
    error_calculator()

    # gp = GpExecution()
    # gp.set_train_xy(10, 4)
    # gp.build_gp_model()
    # # gp.draw_gp_map("image/4th iteration.pdf")
    # predict_time = gp.get_gp_error()
    # print(predict_time)
