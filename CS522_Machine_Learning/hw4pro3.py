import numpy as np
import matplotlib.pyplot as plt

# Load the dataset
train_data = np.genfromtxt('pima.tr', dtype=None, encoding=None)
test_data = np.genfromtxt('pima.te', dtype=None, encoding=None)

# Extract labels from train_data and test_data
train_labels = train_data[:, -1].astype(np.float64)
test_labels = test_data[:, -1].astype(np.float64)

# Extract features from train_data and test_data
train_features = train_data[:, :-1]
test_features = test_data[:, :-1]

# Separate features and labels
train_features = train_data[:, :-1]
train_labels = train_data[:, -1]
test_features = test_data[:, :-1]
test_labels = test_data[:, -1]

# Standardize the data
mean_train = np.mean(train_features, axis=0)
std_train = np.std(train_features, axis=0)
train_features = (train_features - mean_train) / std_train
test_features = (test_features - mean_train) / std_train

# kNN Classifier using only numpy
def knn_classifier(train_data, train_labels, test_data, k=5):
    distances = np.sqrt(((train_data - test_data[:, np.newaxis]) ** 2).sum(axis=2))
    knn_indices = np.argpartition(distances, k, axis=1)[:, :k]
    knn_labels = train_labels[knn_indices]
    pred_labels = np.array([np.bincount(row).argmax() for row in knn_labels])
    pred_probs = np.array([np.mean(row) for row in knn_labels])
    return pred_labels, pred_probs

# Manually compute ROC Curve
def compute_roc_manual(test_labels, scores):
    thresholds = sorted(scores, reverse=True)
    tpr = []
    fpr = []
    for threshold in thresholds:
        positive_preds = scores >= threshold
        tp = np.sum((positive_preds == 1) & (test_labels == 1))
        fp = np.sum((positive_preds == 1) & (test_labels == 0))
        fn = np.sum((positive_preds == 0) & (test_labels == 1))
        tn = np.sum((positive_preds == 0) & (test_labels == 0))
        
        tpr.append(tp / (tp + fn))
        fpr.append(fp / (fp + tn))
    return np.array(fpr), np.array(tpr)

# Generate ROC curve for MPP classifier
def generate_roc_curve_mpp(train_features, train_labels, test_features, test_labels, priors=None):
    roc_curves = {}
    for ratio in range(21):
        prior_ratio = ratio / 10.0
        priors = [prior_ratio / (1 + prior_ratio), 1 / (1 + prior_ratio)]
        _, posteriors = mpp_classifier(train_features, train_labels, test_features, priors=priors)
        fpr, tpr = compute_roc_manual(test_labels, posteriors)
        roc_curves[prior_ratio] = (fpr, tpr)
    return roc_curves

# MPP Classifier
def mpp_classifier(train_features, train_labels, test_features, priors=None):
    classes = np.unique(train_labels)
    covs = []
    means = []
    
    # Calculate covariance matrices and means for each class
    for c in classes:
        subset = train_features[train_labels == c]
        covs.append(np.cov(subset, rowvar=False))
        means.append(np.mean(subset, axis=0))
        
    # Predict labels for test data based on maximum posterior probability
    predictions = []
    posteriors = []

    for point in test_features:
        probs = []
        for c in classes:
            prob = multivariate_normal_pdf(point, means[c], covs[c])
            if priors:
                prob *= priors[c]
            probs.append(prob)
        predictions.append(np.argmax(probs))
        posteriors.append(probs[1] / sum(probs))
        
    return predictions, posteriors

# Multivariate Normal PDF
def multivariate_normal_pdf(x, mean, cov):
    size = len(x)
    det = np.linalg.det(cov)
    norm_const = 1.0 / (np.power(2 * np.pi, size / 2) * np.power(det, 0.5))
    x_mu = x - mean
    inv = np.linalg.inv(cov)
    result = np.exp(-0.5 * np.dot(np.dot(x_mu, inv), x_mu.T))
    
    return norm_const * result

# Generate ROC curve for kNN classifier
def generate_roc_curve_knn(train_features, train_labels, test_features, test_labels, k=5):
    _, knn_probs = knn_classifier(train_features, train_labels, test_features, k=k)
    fpr_knn, tpr_knn = compute_roc_manual(test_labels, knn_probs)
    return fpr_knn, tpr_knn

# Generate ROC curves for MPP classifier
roc_curves_mpp = generate_roc_curve_mpp(train_features, train_labels, test_features, test_labels)

# Generate ROC curve for kNN classifier
fpr_knn, tpr_knn = generate_roc_curve_knn(train_features, train_labels, test_features, test_labels)

# Plot ROC curves
plt.figure(figsize=(10, 6))
for ratio, roc_curve in roc_curves_mpp.items():
    fpr, tpr = roc_curve
    plt.plot(fpr, tpr, label=f"MPP (Ratio={ratio})")

plt.plot(fpr_knn, tpr_knn, label="kNN (k=5)")
plt.plot([0, 1], [0, 1], linestyle='--', color='gray', label='Random')
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('ROC Curve')
plt.legend()
plt.grid(True)
plt.show()