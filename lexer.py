from rply import LexerGenerator

class Lexer:
  def __init__(self):
    self.lexer = LexerGenerator()
  
  def add_tokens(self):
    self.lexer.add('OPEN_PAREN', r'\(')
    self.lexer.add('CLOSE_PAREN', r'\)')
    self.lexer.add('NUMBER', r'\d+')
    self.lexer.add('SUM', '\+')
    self.lexer.add('SUB', '\-')
    self.lexer.add('ID', r'[A-Za-z]+')
    self.lexer.add('EQUALS', r'\=')
    self.lexer.add('FUNC', r'fun')
    self.lexer.add('CPAREN_OPEN', r'\{')
    self.lexer.add('CPAREN_CLOSE', r'\}')
    self.lexer.ignore('\s+')
    
  def get_lexer(self):
    self.add_tokens()
    return self.lexer.build()