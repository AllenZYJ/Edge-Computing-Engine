# -*- coding: utf-8 -*-
import numpy as np

# N is batch size; D_in is input dimension;
# H is hidden dimension; D_out is output dimension.
N, D_in, H, D_out = 1, 2, 3, 1

# Create random input and output data
x = np.random.randn(N, D_in)
y = np.random.randn(N, D_out)
print(x)
# Randomly initialize weights
w1 = np.random.randn(D_in, H)
w2 = np.random.randn(H, D_out)
print("w1:",w1)
print("-----------------------")
print("w2:",w2)
learning_rate = 1e-6
for t in range(5):
    # Forward pass: compute predicted y
    h = x.dot(w1)
    h_relu = np.maximum(h, 0)
    print("h:",h_relu)
    y_pred = h_relu.dot(w2)
    print("y_pred: ",y_pred)
    # Compute and print loss
    loss = np.square(y_pred - y).sum()
    print(t, loss)
    # Backprop to compute gradients of w1 and w2 with respect to loss
    grad_y_pred = 2.0 * (y_pred - y)
    print("grad:",grad_y_pred)
    grad_w2 = h_relu.T.dot(grad_y_pred)#完成
    print("gradw2: ",grad_w2)
    grad_h_relu = grad_y_pred.dot(w2.T)
    grad_h = grad_h_relu.copy()
    grad_h[h < 0] = 0
    grad_w1 = x.T.dot(grad_h)

    # Update weights
    w1 -= learning_rate * grad_w1
    w2 -= learning_rate * grad_w2