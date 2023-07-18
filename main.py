from lexer import Lexer
from parser import Parser

text_input = """
{
  a = 8
  b = 78
  c = {
    d = 78
  }
}
"""

lexer = Lexer().get_lexer()
tokens = lexer.lex(text_input)

pg = Parser()
pg.parse()
parser = pg.get_parser()
p = parser.parse(tokens)
p.eval()