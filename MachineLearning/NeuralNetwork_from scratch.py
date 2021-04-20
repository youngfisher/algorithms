# this code consists of no 3rd party deep-learning library such as Tensor flow, PyTorch and so on.
# It is mainly to demonstrate how to write a neural network from scratch, with the aid of only mathmatics and numpy library.

import numpy as np
import matplotlib.pyplot as plt
import sys
from sklearn import datasets
if sys.version_info >= (3, 0):
    def xrange(*args, **kwargs):
        return iter(range(*args, **kwargs))


#load dataset
def load_dataset():
    iris = datasets.load_iris()
    X = iris.data
    y = iris.target
    return X, y

def train_test_split(X, y):
    idx = np.arange(len(X))
    train_size = int(len(X) * 2/3)
    val_size = int(len(X) * 1/6)
    np.random.shuffle(idx)
    X = X[idx]
    y = y[idx]
    X_train, X_val, X_test = X[:train_size], X[train_size:train_size+val_size], X[train_size+val_size:]
    y_train, y_val, y_test = y[:train_size], y[train_size:train_size+val_size], y[train_size+val_size:]

    return X_train, y_train, X_val, y_val, X_test, y_test

class TwoLayerCNN(object):
  """
  A two-layer fully-connected neural network. The net has an input dimension of
  N, a hidden layer dimension of H, and performs classification over C classes.
  We train the network with a softmax loss function and L2 regularization on the
  weight matrices. The network uses a ReLU nonlinearity after the first fully
  connected layer.

  In other words, the network has the following architecture:

  input - fully connected layer - ReLU - fully connected layer - softmax

  The outputs of the second fully-connected layer are the scores for each class.
  """

  def __init__(self, input_size, hidden_size, output_size, std=1e-4):
    """
    Initialize the model. Weights are initialized to small random values and
    biases are initialized to zero. Weights and biases are stored in the
    variable self.params, which is a dictionary with the following keys:

    W1: First layer weights; has shape (D, H)
    b1: First layer biases; has shape (H,)
    W2: Second layer weights; has shape (H, C)
    b2: Second layer biases; has shape (C,)

    Inputs:
    - input_size: The dimension D of the input data.
    - hidden_size: The number of neurons H in the hidden layer.
    - output_size: The number of classes C.
    """
    self.params = {}
    self.params['W1'] = std * np.random.randn(input_size, hidden_size)
    self.params['b1'] = np.zeros(hidden_size)
    self.params['W2'] = std * np.random.randn(hidden_size, output_size)
    self.params['b2'] = np.zeros(output_size)

  def loss(self, X, y=None, reg=0.0):
    """
    Compute the loss and gradients for a two layer fully connected neural
    network.

    Inputs:
    - X: Input data of shape (N, D). Each X[i] is a training sample.
    - y: Vector of training labels. y[i] is the label for X[i], and each y[i] is
      an integer in the range 0 <= y[i] < C. This parameter is optional; if it
      is not passed then we only return scores, and if it is passed then we
      instead return the loss and gradients.
    - reg: Regularization strength.

    Returns:
    If y is None, return a matrix scores of shape (N, C) where scores[i, c] is
    the score for class c on input X[i].

    If y is not None, instead return a tuple of:
    - loss: Loss (data loss and regularization loss) for this batch of training
      samples.
    - grads: Dictionary mapping parameter names to gradients of those parameters
      with respect to the loss function; has the same keys as self.params.
    """
    # Unpack variables from the params dictionary
    W1, b1 = self.params['W1'], self.params['b1']
    W2, b2 = self.params['W2'], self.params['b2']
    N = X.shape[0]
    D = X.shape[1]

    # Compute the forward pass
    scores = None
    #############################################################################
    # Full Mark: 1                                                              #
    # TODO: Perform the forward pass, computing the class scores for the input. #
    # Store the result in the scores variable, which should be an array of      #
    # shape (N, C).                                                             #
    #############################################################################

    # Using ReLUs as the Activation Function

    def ReLU(X): # X is a numpy array
        return np.maximum(0,X)
       
    def SoftMax(X): # X is a numpy array
        e_x = np.exp(X - np.max(X)) 
        return e_x / e_x.sum(axis=1).repeat(X.shape[1]).reshape(X.shape[0],X.shape[1]) 
        # sum direction along column axis
        # without axis=1, function will give wrong output on non 1-D vector
        # each row is an example. all columns in one row gives the whole classes. 
        # In a given row, we need probability of each column. 
    
    Layer_1      = np.dot(X,W1) + b1
    Activation_1 = ReLU(Layer_1)
    
    Layer_2      = np.dot(Activation_1,W2) + b2
    Activation_2 = SoftMax(Layer_2)

    scores       = Activation_2

    #############################################################################
    #                              END OF YOUR CODE                             #
    #############################################################################

    # If the targets are not given then jump out, we're done
    if y is None:
      return scores

    # Compute the loss
    loss = None
    
#    scores -= np.max(scores, axis=1, keepdims=True) # avoid numeric instability

    #############################################################################
    # Full Mark: 2                                                              #
    # TODO: Finish the forward pass, and compute the loss. This should include  #
    # both the data loss and L2 regularization for W1 and W2. Store the result  #
    # in the variable loss, which should be a scalar. Use the Softmax           #
    # classifier loss.                                                          #
    #############################################################################

    H = b1.shape[0]
    C = b2.shape[0]
    
    #mapping the label of y into binary matrix of size N*C
    column = y #
    row    = np.arange(N) #

    label_mapping = np.zeros(N*C) # build a 1-D vector of size N*C filled with zero
    
     # python equivalent of matlab function sub2ind
    arr = np.array([row,column])

    location = np.ravel_multi_index(arr,(N,C))

    label_mapping[location] = 1
    label_mapping = label_mapping.reshape(N,C)
    
    # calculate loss in 2 step. first without regularization. second calculate the regularization.
    first_part  = (-label_mapping*np.log(scores) - (1 - label_mapping)*np.log(1 - scores)).sum()/N
    second_part = reg*((b1*b1).sum() + (b2*b2).sum())/(2*N)
    
    loss = first_part + second_part #scalar

    #############################################################################
    #                              END OF YOUR CODE                             #
    #############################################################################

    # Backward pass: compute gradients
    grads = {}
    #############################################################################
    # Full Mark: 2                                                              #
    # TODO: Compute the backward pass, computing the derivatives of the weights #
    # and biases. Store the results in the grads dictionary. For example,       #
    # grads['W1'] should store the gradient on W1, and be a matrix of same size #
    #############################################################################

    def ReLUGradient(X):

        temp = X.copy()
        temp[X>0] = 1
        temp[X<0] = 0
        return temp
    
    dZ_Layer_2 = Activation_2 - label_mapping # N*C
    temp = ReLUGradient(Layer_1)
    dZ_Layer_1 = np.dot(dZ_Layer_2,np.transpose(W2))*temp # N*H
    
    dZ_tensor_Layer_1 = np.zeros((H,D,N))
    dZ_tensor_Layer_2 = np.zeros((C,H,N))
  
    """
    # 一维向量的乘法真是醉了
    print(dZ_Layer_1[1,:][:,np.newaxis])
    print(dZ_Layer_1[1,:].shape)
    print(X[1,:][np.newaxis,:])
    print(X[1,:].shape)

    print(np.dot(dZ_Layer_1[1,:][:,np.newaxis],X[1,:][np.newaxis,:]))
    
    print(np.dot(np.transpose(dZ_Layer_1[1,:]),X[1,:]).shape)
    """  



    for index in range(N):
        dZ_tensor_Layer_1[:,:,index] = np.dot(dZ_Layer_1[index,:][:,np.newaxis],X[index,:][np.newaxis,:])
        dZ_tensor_Layer_2[:,:,index] = np.dot(dZ_Layer_2[index,:][:,np.newaxis],Activation_1[index,:][np.newaxis,:])
        
    grad_Layer_1 = dZ_tensor_Layer_1.sum(axis=2)/N # H*D
    grad_Layer_2 = dZ_tensor_Layer_2.sum(axis=2)/N # C*H

    # W2 gradient
    dW2 = np.transpose(grad_Layer_2)

    # b2 gradient
    db2 = np.transpose(np.sum(dZ_Layer_2,axis=0)/N)
    
    # W1 gradient
    dW1 = np.transpose(grad_Layer_1) 

    # b1 gradient
    db1 = np.transpose(np.sum(dZ_Layer_1,axis=0)/N)

    # regularization gradient

    # store the results in the grads dictionary
    grads = {'W1':dW1, 'b1':db1, 'W2':dW2, 'b2':db2}

    #############################################################################
    #                              END OF YOUR CODE                             #
    #############################################################################

    return loss, grads

  def train(self, X, y, X_val, y_val,
            learning_rate=1e-3, learning_rate_decay=0.95,
            reg=5e-6, num_iters=1000,
            batch_size=200, verbose=False):
    """
    Train this neural network using stochastic gradient descent.

    Inputs:
    - X: A numpy array of shape (N, D) giving training data.
    - y: A numpy array f shape (N,) giving training labels; y[i] = c means that
      X[i] has label c, where 0 <= c < C.
    - X_val: A numpy array of shape (N_val, D) giving validation data.
    - y_val: A numpy array of shape (N_val,) giving validation labels.
    - learning_rate: Scalar giving learning rate for optimization.
    - learning_rate_decay: Scalar giving factor used to decay the learning rate
      after each epoch.
    - reg: Scalar giving regularization strength.
    - num_iters: Number of steps to take when optimizing.
    - batch_size: Number of training examples to use per step.
    - verbose: boolean; if true print progress during optimization.
    """
    num_train = X.shape[0]
    iterations_per_epoch = max(num_train / batch_size, 1)

    # Use SGD to optimize the parameters in self.model
    loss_history = []
    train_acc_history = []
    val_acc_history = []

    for it in xrange(num_iters):

      #########################################################################
      # Full Mark: 0.5                                                        #
      # TODO: Create a random minibatch of training data and labels using     #
      # given num_train and batch_size, storing them in X_batch and y_batch   #
      # respectively.                                                         #
      #########################################################################
      
      if batch_size > num_train:
            print("batch_size too big")
        
      random_index = np.random.choice(num_train,batch_size,replace=False)
        
      X_batch = X[random_index]
      y_batch = y[random_index]

      #########################################################################
      #                             END OF YOUR CODE                          #
      #########################################################################

      # Compute loss and gradients using the current minibatch
      loss, grads = self.loss(X_batch, y=y_batch, reg=reg)
      loss_history.append(loss)

      #########################################################################
      # Full Mark: 0.5                                                        #
      # TODO: Use the gradients in the grads dictionary to update the         #
      # parameters of the network (stored in the dictionary self.params)      #
      # using stochastic gradient descent. You'll need to use the gradients   #
      # stored in the grads dictionary defined above.                         #
      #########################################################################

      W1, b1 = self.params['W1'], self.params['b1']
      W2, b2 = self.params['W2'], self.params['b2']

      self.params['W1'] = W1 - grads['W1']*learning_rate
      self.params['W2'] = W2 - grads['W2']*learning_rate
      self.params['b1'] = b1 - grads['b1']*learning_rate
      self.params['b2'] = b2 - grads['b2']*learning_rate

      #########################################################################
      #                             END OF YOUR CODE                          #
      #########################################################################

      if verbose and it % 10 == 0:
        print('iteration %d / %d: loss %f' % (it, num_iters, loss))

      # Every epoch, check train and val accuracy and decay learning rate.

        # Check accuracy
      train_acc = (self.predict(X_batch) == y_batch).mean()
      val_acc = (self.predict(X_val) == y_val).mean()
      train_acc_history.append(train_acc)
      val_acc_history.append(val_acc)

      if it % iterations_per_epoch == 0:
        # Decay learning rate
       learning_rate *= learning_rate_decay

    return {
      'loss_history': loss_history,
      'train_acc_history': train_acc_history,
      'val_acc_history': val_acc_history,
    }

  def predict(self, X):
    """
    Use the trained weights of this two-layer network to predict labels for
    data points. For each data point we predict scores for each of the C
    classes, and assign each data point to the class with the highest score.

    Inputs:
    - X: A numpy array of shape (N, D) giving N D-dimensional data points to
      classify.

    Returns:
    - y_pred: A numpy array of shape (N,) giving predicted labels for each of
      the elements of X. For all i, y_pred[i] = c means that X[i] is predicted
      to have class c, where 0 <= c < C.
    """

    ###########################################################################
    # Full Mark: 1                                                            #
    # TODO: Implement this function                                           #
    ###########################################################################

    W1, b1 = self.params['W1'], self.params['b1']
    W2, b2 = self.params['W2'], self.params['b2']

    def ReLU(X): # X is a numpy array
        return np.maximum(0,X)
       
    def SoftMax(X): # X is a numpy array
        e_x = np.exp(X- np.max(X))
        return e_x / e_x.sum(axis=1).repeat(X.shape[1]).reshape(X.shape[0],X.shape[1]) 
        # sum direction along column axis
        # without axis=1, function will give wrong output on non 1-D vector
        # each row is an example. all columns in one row gives the whole classes. 
        # In a given row, we need probability of each column. 
    
    # layers of the nn
    Layer_1      = np.dot(X,W1) + b1 # N*H
    Activation_1 = ReLU(Layer_1)
    
    Layer_2      = np.dot(Activation_1,W2) + b2 # N*C
    Activation_2 = SoftMax(Layer_2)    
    
    y_pred = np.argmax(Activation_2,axis=1)

    ###########################################################################
    #                              END OF YOUR CODE                           #
    ###########################################################################

    return y_pred

    # To check your implementations.
X,y =load_dataset()
X_train, y_train, X_val, y_val, X_test, y_test=train_test_split(X, y)


###########################################################################
# Full Mark: 1                                                            #
# TODO: 1. Using TwoLayerCNN to train on given datasets                   #
#       2. Print out the final loss                                       #
#       3. Print out the test accuracy                                    #
###########################################################################

input_size  = X_train.shape[1]
hidden_size = 20
num_classes = 3
net = TwoLayerCNN(input_size, hidden_size, num_classes)
# TODO

###########################################################################
#                              END OF YOUR CODE                           #
###########################################################################

###########################################################################
# Full Mark: 0.5                                                          #
# TODO: Plot training loss history                                        # 
###########################################################################

history = net.train(X_train, y_train, X_val, y_val,batch_size=30,num_iters=100000)

fig = plt.figure()
ax1 = plt.subplot(2,1,1)

plt.plot(history['loss_history'])

###########################################################################
#                              END OF YOUR CODE                           #
###########################################################################   
    
plt.xlabel('iteration')
plt.ylabel('training loss')
plt.title('Training Loss history')
plt.show()

fig = plt.figure(1)
ax2 = fig.add_subplot(2,1,2)

plt.plot(history['train_acc_history'])
plt.plot(history['val_acc_history'])

###########################################################################
#                              END OF YOUR CODE                           #
###########################################################################  

plt.xlabel('Epoch')
plt.ylabel('Classification accuracy')
plt.title('Classification accuracy history')
plt.legend(labels=['train_accu','val_acc'])
plt.show()
