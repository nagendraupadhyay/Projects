import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import time

# Read the training data
with open('train.txt', 'r') as file:
    lines = file.readlines()

# Parse the data into lists
x_values = []
y_values = []
class_labels = []
for line in lines:
    parts = line.strip().split()
    x_values.append(float(parts[0]))
    y_values.append(float(parts[1]))
    class_labels.append(int(parts[2]))

# Separate data for each class
class0_x = [x_values[i] for i in range(len(class_labels)) if class_labels[i] == 0]
class0_y = [y_values[i] for i in range(len(class_labels)) if class_labels[i] == 0]
class1_x = [x_values[i] for i in range(len(class_labels)) if class_labels[i] == 1]
class1_y = [y_values[i] for i in range(len(class_labels)) if class_labels[i] == 1]

# Plot the scatter plot
plt.figure(figsize=(8, 6))
plt.scatter(class0_x, class0_y, label='Class 0', color='blue')
plt.scatter(class1_x, class1_y, label='Class 1', color='red')
plt.xlabel('X Value')
plt.ylabel('Y Value')
plt.legend()
plt.title('Scatter Plot of Training Set')
plt.show()

# Task 2: Plot overall classification accuracy for different k values in kNN

# Load the test data
with open('test.txt', 'r') as file:
    lines = file.readlines()

# Parse the test data into lists
test_x_values = []
test_y_values = []
test_class_labels = []
for line in lines:
    parts = line.strip().split()
    test_x_values.append(float(parts[0]))
    test_y_values.append(float(parts[1]))
    test_class_labels.append(int(parts[2]))

# Function to calculate distance between two points
def calculate_distance(x1, y1, x2, y2):
    return np.sqrt((x1 - x2)**2 + (y1 - y2)**2)

# Function to classify using kNN
def knn_classify(k, test_x, test_y):
    predictions = []
    for i in range(len(test_x)):
        distances = [(calculate_distance(test_x[i], test_y[i], x_values[j], y_values[j]), class_labels[j]) for j in range(len(x_values))]
        distances.sort(key=lambda x: x[0])  # Sort by distance
        neighbors = distances[:k]  # Get the k nearest neighbors
        class_count = {0: 0, 1: 0}  # Count the occurrences of each class
        for neighbor in neighbors:
            class_count[neighbor[1]] += 1
        predicted_class = max(class_count, key=class_count.get)  # Choose the class with maximum occurrences
        predictions.append(predicted_class)
    return predictions

# Calculate accuracy for different k values
k_values = range(1, 21)  # Consider k from 1 to 20
accuracy_values = []

for k in k_values:
    predictions = knn_classify(k, test_x_values, test_y_values)
    correct_predictions = sum(1 for i in range(len(test_class_labels)) if predictions[i] == test_class_labels[i])
    accuracy = correct_predictions / len(test_class_labels)
    accuracy_values.append(accuracy)

# Plot the accuracy for different k values
plt.figure(figsize=(8, 6))
plt.plot(k_values, accuracy_values, marker='o')
plt.xlabel('k (Number of Neighbors)')
plt.ylabel('Overall Classification Accuracy')
plt.title('Overall Classification Accuracy vs. k in kNN')
plt.grid(True)
plt.show()


# Function to calculate Euclidean distance between two points
def calculate_euclidean_distance(x1, y1, x2, y2):
    return np.sqrt((x1 - x2)**2 + (y1 - y2)**2)

# Function to calculate Mahalanobis distance between two points
def calculate_mahalanobis_distance(x1, y1, x2, y2, cov_matrix):
    point1 = np.array([x1, y1])
    point2 = np.array([x2, y2])
    inv_cov_matrix = np.linalg.inv(cov_matrix)
    diff = point1 - point2
    return np.sqrt(np.dot(np.dot(diff, inv_cov_matrix), diff.T))

# Function to calculate the discriminant function for minimum Euclidean distance classifier
def euclidean_distance_classifier(x, y, class0_mean, class1_mean):
    distance_to_class0 = calculate_euclidean_distance(x, y, class0_mean[0], class0_mean[1])
    distance_to_class1 = calculate_euclidean_distance(x, y, class1_mean[0], class1_mean[1])
    if distance_to_class0 < distance_to_class1:
        return 0
    else:
        return 1

# Function to calculate the discriminant function for minimum Mahalanobis distance classifier
def mahalanobis_distance_classifier(x, y, class0_mean, class1_mean, class0_cov, class1_cov):
    distance_to_class0 = calculate_mahalanobis_distance(x, y, class0_mean[0], class0_mean[1], class0_cov)
    distance_to_class1 = calculate_mahalanobis_distance(x, y, class1_mean[0], class1_mean[1], class1_cov)
    if distance_to_class0 < distance_to_class1:
        return 0
    else:
        return 1

# Function to calculate the discriminant function for generic Bayesian decision rule
def bayesian_decision_rule(x, y, class0_mean, class1_mean, class0_cov, class1_cov):
    discriminant_class0 = -0.5 * calculate_mahalanobis_distance(x, y, class0_mean[0], class0_mean[1], class0_cov)
    discriminant_class1 = -0.5 * calculate_mahalanobis_distance(x, y, class1_mean[0], class1_mean[1], class1_cov)
    if discriminant_class0 > discriminant_class1:
        return 0
    else:
        return 1

# Calculate the mean and covariance matrices for each class
class0_mean = [np.mean(class0_x), np.mean(class0_y)]
class1_mean = [np.mean(class1_x), np.mean(class1_y)]
class0_cov = np.cov(class0_x, class0_y)
class1_cov = np.cov(class1_x, class1_y)

# Choose the best k value obtained from Task 2
best_k = accuracy_values.index(max(accuracy_values)) + 1

# Initialize variables to store the metrics
algorithm_names = ['Minimum Euclidean Distance', 'Minimum Mahalanobis Distance', 'Generic Bayesian Decision Rule', 'kNN Classifier (k={})'.format(best_k)]
overall_accuracy = []
classwise_accuracy = []
runtimes = []

# Calculate metrics for each algorithm
for algorithm_name in algorithm_names:
    start_time = time.time()
    predictions = []
    if algorithm_name == 'kNN Classifier (k={})'.format(best_k):
        predictions = knn_classify(best_k, test_x_values, test_y_values)
    else:
        for i in range(len(test_x_values)):
            if algorithm_name == 'Minimum Euclidean Distance':
                predicted_class = euclidean_distance_classifier(test_x_values[i], test_y_values[i], class0_mean, class1_mean)
            elif algorithm_name == 'Minimum Mahalanobis Distance':
                predicted_class = mahalanobis_distance_classifier(test_x_values[i], test_y_values[i], class0_mean, class1_mean, class0_cov, class1_cov)
            else:
                predicted_class = bayesian_decision_rule(test_x_values[i], test_y_values[i], class0_mean, class1_mean, class0_cov, class1_cov)
            predictions.append(predicted_class)
    end_time = time.time()

    # Calculate metrics
    correct_predictions = sum(1 for i in range(len(test_class_labels)) if predictions[i] == test_class_labels[i])
    accuracy = correct_predictions / len(test_class_labels)
    class0_correct_predictions = sum(1 for i in range(len(test_class_labels)) if predictions[i] == test_class_labels[i] == 0)
    class1_correct_predictions = sum(1 for i in range(len(test_class_labels)) if predictions[i] == test_class_labels[i] == 1)
    class0_accuracy = class0_correct_predictions / sum(1 for label in test_class_labels if label == 0)
    class1_accuracy = class1_correct_predictions / sum(1 for label in test_class_labels if label == 1)
    runtime = end_time - start_time

    # Store the metrics
    overall_accuracy.append(accuracy)
    classwise_accuracy.append([class0_accuracy, class1_accuracy])
    runtimes.append(runtime)

# Print the table
table = pd.DataFrame({'Algorithm': algorithm_names,
                      'Overall Accuracy': overall_accuracy,
                      'Class 0 Accuracy': [accuracy[0] for accuracy in classwise_accuracy],
                      'Class 1 Accuracy': [accuracy[1] for accuracy in classwise_accuracy],
                      'Runtime (s)': runtimes})
print(table)

# Task 5: Generate decision boundaries and plot them
# Generate a mesh grid for plotting decision boundaries
x_min, x_max = min(x_values) - 1, max(x_values) + 1
y_min, y_max = min(y_values) - 1, max(y_values) + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.01),
                     np.arange(y_min, y_max, 0.01))

# Flatten the mesh grid and predict for each point
mesh_points = np.c_[xx.ravel(), yy.ravel()]

# Ensure mesh_points is a 2D array
mesh_points = np.asarray(mesh_points)

# Predict for each point using the different classifiers
predictions_euclidean = np.array([euclidean_distance_classifier(point[0], point[1], class0_mean, class1_mean) for point in mesh_points])
predictions_mahalanobis = np.array([mahalanobis_distance_classifier(point[0], point[1], class0_mean, class1_mean, class0_cov, class1_cov) for point in mesh_points])
predictions_bayesian = np.array([bayesian_decision_rule(point[0], point[1], class0_mean, class1_mean, class0_cov, class1_cov) for point in mesh_points])
predictions_knn = knn_classify(best_k, mesh_points[:, 0], mesh_points[:, 1])

# Reshape the predictions for contour plotting
predictions_euclidean = predictions_euclidean.reshape(xx.shape)
predictions_mahalanobis = predictions_mahalanobis.reshape(xx.shape)
predictions_bayesian = predictions_bayesian.reshape(xx.shape)
predictions_knn = np.array(predictions_knn).reshape(xx.shape)

# Plot the scatter plot and decision boundaries
plt.figure(figsize=(12, 10))
plt.scatter(class0_x, class0_y, label='Class 0', color='blue')
plt.scatter(class1_x, class1_y, label='Class 1', color='red')
plt.contourf(xx, yy, predictions_euclidean, alpha=0.3, cmap='viridis', levels=[-1, 0, 1], linestyles=['dashed', 'solid', 'dashed'])
plt.contourf(xx, yy, predictions_mahalanobis, alpha=0.3, cmap='viridis', levels=[-1, 0, 1], linestyles=['dashed', 'solid', 'dashed'])
plt.contourf(xx, yy, predictions_bayesian, alpha=0.3, cmap='viridis', levels=[-1, 0, 1], linestyles=['dashed', 'solid', 'dashed'])
plt.contourf(xx, yy, predictions_knn, alpha=0.3, cmap='viridis', levels=[-1, 0, 1], linestyles=['dashed', 'solid', 'dashed'])

plt.xlabel('X Value')
plt.ylabel('Y Value')
plt.legend()
plt.title('Decision Boundaries for Parametric Learning Algorithms and kNN')
plt.show()