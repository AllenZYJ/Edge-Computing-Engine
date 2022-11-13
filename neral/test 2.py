class Tensor:
    def __init__(self, data, depend=[]):
        """初始化"""
        self.data = data  
        self.depend = depend 
        self.grad = 0 
    def __mul__(self, data):
        """乘法"""
        def grad_fn1(grad):
            return grad * data.data 
        def grad_fn2(grad):
            return grad * self.data  
        depend = [(self, grad_fn1), (data, grad_fn2)]
        new = Tensor(self.data * data.data, depend) 
        return new 
    def __rmul__(self, data):
        def grad_fn1(grad):
            return grad * data.data 
        def grad_fn2(grad):
            return grad * self.data  
        depend = [(self, grad_fn1), (data, grad_fn2)]
        new = Tensor(self.data * data.data, depend) 
        return new 
    def __add__(self, data):
        """加法"""
        def grad_fn(grad):
            return grad   
        depend = [(self, grad_fn), (data, grad_fn)]
        new = Tensor(self.data * data.data, depend) 
        return new    
    def __radd__(self, data):
        def grad_fn(grad):
            return grad   
        depend = [(self, grad_fn), (data, grad_fn)]
        new = Tensor(self.data * data.data, depend) 
        return new  
    def __repr__(self):
        return f"Tensor:{self.data}"
    def backward(self, grad=None):
        """
        反向传播，需要递归计算
        """
        if grad == None:
            self.grad = 1 
        else:
            # 这一步用于计算图中的分支
            self.grad += grad
        # 这一步是递归计算
        for tensor, grad_fn in self.depend:
            bw = grad_fn(self.grad)
            tensor.backward(bw)
x = Tensor(4) 
f = x * x 
g = x * x 
y = f + g
y.backward()
print(x)
print(y, g.grad, x.grad)