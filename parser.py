from rply import ParserGenerator
import ast
import random

MIN = 0xFFF
MAX = 0xFFFFFF
lines = []
class Parser():
    def __init__(self):
        self.pg = ParserGenerator(
            # A list of all token names accepted by the parser.
            ['NUMBER', 'OPEN_PAREN', 'CLOSE_PAREN',
            'SUM', 'SUB', 'ID', 'EQUALS', 'FUNC', 'CPAREN_OPEN', 'CPAREN_CLOSE']
        )

    def parse(self):
        @self.pg.production('program : line | program line')
        def program(p):
            return ast.Root(p[-1]);
        
        @self.pg.production('line : expression | line expression')
        def line(p):
            if len(p) > 1:
              if len(p) == 2:
                lines.append(p[0])
              l = ast.Line(p[-1])
              lines.append(l)
              return lines
            return ast.Line(p[-1]);
        @self.pg.production('expression : expression SUM expression')
        @self.pg.production('expression : expression SUB expression')
        def expression(p):
            left = p[0]
            right = p[2]
            operator = p[1]
            if operator.gettokentype() == 'SUM':
                return ast.Sum(left, right)
            elif operator.gettokentype() == 'SUB':
                return ast.Sub(left, right)
        @self.pg.production('expression : CPAREN_OPEN line CPAREN_CLOSE')
        def block(p):
          return ast.Block(p[1], 'lambda' + str(random.randint(MIN, MAX)))
        @self.pg.production('expression : ID EQUALS expression')
        def assign(p):
          return ast.Assign(p[0], p[2])
        @self.pg.production('expression : NUMBER')
        def number(p):
            return ast.Number(p[0].value)
        @self.pg.error
        def error_handle(token):
            raise ValueError(token)

    def get_parser(self):
        return self.pg.build()