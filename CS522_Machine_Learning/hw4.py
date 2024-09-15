import numpy as np
import matplotlib.pyplot as plt

# Define the function
def f(t):
    return 50 * np.sin(t) + t**2

# Define the derivative of the function
def df(t):
    return 50 * np.cos(t) + 2*t

# Implement gradient descent
def gradient_descent(starting_point, learning_rate, num_iterations):
    t = starting_point
    path = [t]  # Store the convergence path
    
    for _ in range(num_iterations):
        gradient = df(t)
        t = t - learning_rate * gradient
        path.append(t)
    
    return t, path

# Set the parameters
starting_points = [7, 1]
learning_rate = 0.1
num_iterations = 100

# Run gradient descent for each starting point
for starting_point in starting_points:
    minimum, path = gradient_descent(starting_point, learning_rate, num_iterations)
    
    # Print the minimum value
    print("Starting point:", starting_point)
    print("The minimum value of f(t) is:", f(minimum))
    
    # Print the convergence path
    print("Convergence path:", path)
    print()
    
    # Generate values for t along the convergence path
    t_path = np.array(path)
    
    # Calculate the corresponding values of f(t) along the convergence path
    y_path = f(t_path)
    
    # Plot the convergence path with labels
    label = "Convergence Path of {}".format(starting_point)
    plt.plot(t_path, y_path, 'o-', label=label)
    
# Generate values for t
t = np.linspace(-10, 10, 1000)

# Calculate the corresponding values of f(t)
y = f(t)

# Plot the function
plt.plot(t, y, label='f(t) = 50 * sin(t) + t^2')
plt.xlabel('t')
plt.ylabel('f(t)')
plt.title('Plot of f(t) = 50 * sin(t) + t^2')
plt.grid(True)
plt.legend()
plt.show()