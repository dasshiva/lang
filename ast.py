exprs = []

class Root():
  def __init__(self, p):
    exprs.append(p)
  def eval(self):
    for expr in exprs:
      print(expr.eval())

class Function: 
  name = ""
  def __init__(self, lines, name):
    self.code = lines
    self.name = name
    print(self.code)
  def __str__(self):
    return f"Function {self.name}"
    
class Block(): 
  val = None
  def __init__(self, expr, name):
    self.val = expr
    self.name = name
  
  def printall(self, expr):
    for ex in expr:
      if isinstance(ex, list):
        self.printall(ex)
      print(ex.eval())
      
  def eval(self):
    if isinstance(self.val, list):
      for expr in self.val:
        if isinstance(expr, list):
          pass
          #print(expr[2])
          #self.printall(expr)
        else:
          print(expr.eval())
      return
    print(f'Block with expression {self.val}')
    return Function(self.val, self.name)

class Line():
  def __init__(self, line):
    self.line = line
  def eval(self):
    return self.line
    
class Number():
    def __init__(self, value):
        self.value = value

    def eval(self):
        return int(self.value)


class BinaryOp():
    def __init__(self, left, right):
        self.left = left
        self.right = right


class Sum(BinaryOp):
    def eval(self):
        return self.left.eval() + self.right.eval()


class Sub(BinaryOp):
    def eval(self):
        return self.left.eval() - self.right.eval()


class Print():
    def __init__(self, value):
        self.value = value

    def eval(self):
        print(self.value.eval())

class Assign():
    def __init__(self, name, value):
        self.name = name.value
        self.value = value.eval()
    
    def eval(self):
        print(f'Variable {self.name} has value {self.value}')