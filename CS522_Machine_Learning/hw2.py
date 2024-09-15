import numpy as np
import matplotlib.pyplot as plt

# Define the AND gate samples
samples = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
labels = np.array([0, 0, 0, 1])

# Perform FLD
mean_class_0 = np.mean(samples[labels == 0], axis=0)
mean_class_1 = np.mean(samples[labels == 1], axis=0)
within_class_scatter = np.sum((samples[labels == 0] - mean_class_0) ** 2) + np.sum((samples[labels == 1] - mean_class_1) ** 2)
between_class_scatter = np.sum((mean_class_0 - mean_class_1) ** 2)
fld_projection = (mean_class_0 - mean_class_1) / within_class_scatter

# Perform PCA
covariance_matrix = np.cov(samples.T)
eigenvalues, eigenvectors = np.linalg.eig(covariance_matrix)
pca_projection = eigenvectors[:, np.argmax(eigenvalues)]

# Plot the samples
plt.scatter(samples[labels == 0, 0], samples[labels == 0, 1], c='r', label='Class 0')
plt.scatter(samples[labels == 1, 0], samples[labels == 1, 1], c='b', label='Class 1')

# Plot the decision boundaries
x = np.linspace(-0.5, 1.5, 100)
y_fld = fld_projection[1] / fld_projection[0] * x
y_pca = pca_projection[1] / pca_projection[0] * x
plt.plot(x, y_fld, 'g--', label='FLD+MD')
plt.plot(x, y_pca, 'm--', label='PCA+MD')

plt.xlabel('Input 1')
plt.ylabel('Input 2')
plt.title('Decision Boundaries Comparison')
plt.legend()
plt.grid(True)
plt.show()
