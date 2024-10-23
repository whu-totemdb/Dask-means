from skopt import gp_minimize
from skopt.space import Real, Categorical, Integer
import joblib
from sklearn.preprocessing import PolynomialFeatures
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error

import gpytorch
import torch
import matplotlib.pyplot as plt


class BayesianOptimizer:
    def __init__(self, degree=4):
        self.data = None
        self.X_train = None
        self.X_test = None
        self.y_train = None
        self.y_test = None
        self.model = joblib.load('nlr.pkl')
        self.degree = degree
        self.poly = PolynomialFeatures(degree=self.degree)

        # params for bayesian optimize
        self.a = None
        self.b = None

    def load_data(self, file_path="kmeans-time-series.csv"):
        self.data = np.genfromtxt(file_path, delimiter=',')
        X = self.data[:, 0:5]
        y = self.data[:, 5:34]
        print("y.shape", y.shape)

        # poly mse = 143
        X_poly = self.poly.fit_transform(X)
        print(X_poly.shape)
        self.X_train, self.X_test, self.y_train, self.y_test = train_test_split(X_poly, y, test_size=0.1,
                                                                                random_state=12)
        print("y_test.shape", self.y_test.shape)
        print("load dataset successfully!")

    def predict(self):
        self.a = self.model.predict(self.X_test)
        print(self.a.shape)
        self.b = self.y_test
        mse = mean_squared_error(self.y_test, self.a)
        print("MSE:", mse)
        print("predicted result: ", self.a)
        print("real value: ", self.y_test)

    def objective_old(self, params):
        reg_param = params[0]
        self.model.set_params(reg_param=reg_param)
        self.model.fit(self.X_train, self.y_train)
        predictions = self.model.predict(self.X_test)
        mse = mean_squared_error(self.y_test, predictions)
        return mse

    def objective_new(self, params):
        scale, shift = params
        c = self.a * scale + shift
        mse = mean_squared_error(c, self.b)
        # error = np.mean(np.abs(c - self.b))
        return mse

    def objective_individual(self, params):
        scales = params[:-1]
        shift = params[-1]
        c = self.a * np.reshape(scales, (1, 29)) + shift  # element-wise multiplication
        mse = mean_squared_error(self.b, c)
        return mse

    def optimize_new(self):
        param_ranges = [(-2.0, 2.0), (-1.0, 1.0)]
        result = gp_minimize(self.objective_new, param_ranges, n_calls=50, random_state=0)
        print("best para: scale=%.4f, shift=%.4f" % (result.x[0], result.x[1]))
        print("min error: %.4f" % result.fun)

    def optimize_individual(self):
        param_ranges = [Real(0.1, 2.0, name='scale_{}'.format(i)) for i in range(29)]
        param_ranges.append(Real(-10, 10, name='shift'))
        result = gp_minimize(self.objective_individual, param_ranges, n_calls=500, random_state=0)
        print("best para: scale=%.4f, shift=%.4f" % (result.x[0], result.x[1]))
        print("min error: %.4f" % result.fun)


if __name__ == '__main__':
    # optimizer = BayesianOptimizer(degree=3)
    # optimizer.load_data('kmeans-time-series.csv')
    # optimizer.predict()
    # optimizer.optimize_individual()
    print()
