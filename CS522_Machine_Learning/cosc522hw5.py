
import numpy as np

# Given data
x = np.array([1.0, 2.0, 3.0, 4.0, 5.0])
p = np.array([0.07, 0.26, 0.61, 0.87, 0.97])
y = np.array([0, 0, 0, 1, 1])

# Reshape the input data
x = x.reshape(-1, 1)

# Add a column of ones to x for the intercept term
x = np.concatenate((np.ones_like(x), x), axis=1)

# Initialize the parameters
theta = np.zeros(x.shape[1])

# Define the sigmoid function
def sigmoid(z):
    return 1 / (1 + np.exp(-z))

# Define the cost function
def cost_function(theta, x, y):
    m = len(y)
    h = sigmoid(np.dot(x, theta))
    cost = (-1 / m) * np.sum(y * np.log(h) + (1 - y) * np.log(1 - h))
    return cost

# Define the gradient descent function
def gradient_descent(theta, x, y, alpha, num_iterations):
    m = len(y)
    costs = []
    for _ in range(num_iterations):
        h = sigmoid(np.dot(x, theta))
        gradient = (1 / m) * np.dot(x.T, (h - y))
        theta -= alpha * gradient
        cost = cost_function(theta, x, y)
        costs.append(cost)
    return theta, costs

# Set the learning rate and number of iterations
learning_rate = 0.01
num_iterations = 1000

# Perform gradient descent to find the optimal parameters
theta, costs = gradient_descent(theta, x, y, learning_rate, num_iterations)

# Predict the probability of passing for a given number of hours studied
hours_studied = 3.25
hours_studied = np.array([1, hours_studied])
predicted_probability = sigmoid(np.dot(hours_studied, theta))

print("Predicted probability of passing:", predicted_probability)
