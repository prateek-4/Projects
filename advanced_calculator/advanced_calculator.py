
class SimpleCalculator:
    def __init__(self):
        self.history = []
    
    def evaluate_expression(self, input_expression):
        alpha=input_expression
        input_expression.replace(" ", "")
        if input_expression[-1] in ['*','+','-','/']:
            self.history.append((str(alpha), 'Error'))
            return 'Error'
        dict=[]
        for i in range(len(input_expression)):
            if input_expression[i] in ['*','+','-','/']:
                dict.append(input_expression[:i])
                dict.append(input_expression[i])
                dict.append(input_expression[i+1:])


    
        if len(dict)==3:
            operand1 = int(dict[0])
            operand2 = int(dict[-1])
            operation=dict[1]

            
            if operation == '+':
                result = float(operand1 + operand2)
            elif operation == '-':
                result = float(operand1 - operand2)
            elif operation == '*':
                result = float(operand1 * operand2)
            elif operation == '/':
                if operand2 == 0:
                    self.history.append((str(alpha), 'Error'))
                    return 'Error'
                result = float(operand1 / operand2)
            else:
                self.history.append((str(alpha), 'Error'))
                return 'Error'
            
            self.history.append((str(alpha), result))
            return result
    
    def get_history(self):
        return self.history

class Stack:
    def __init__(self):
        self.item = []

    def is_empty(self):
        if len(self.item) == 0:
            return True
        return False

    def push(self, item):
        self.item.append(item)

    def peek(self):
        if len(self.item) == 0:
            return 'Error'
        return self.item[-1]

    def pop(self):
        if not self.is_empty():
            return self.item.pop()
        

    def get(self, index):
        return self.item[index]

    def __str__(self):
                self.a=self.item
                self.a.reverse()
                s=''
                for i in self.a[:len(self.a)-1]:
                     s+=str(i)+' '
                s+=str(self.a[-1])
                return s

    def __len__(self):
        return len(self.item)


class AdvancedCalculator(SimpleCalculator):
    def __init__(self):
        self.history=[]
        self.operators = "+-*/(){}"
        self.postfix = Stack()

    def has_precedence(self, op1, op2):
        if op1 == "(" or op1 == ")" or op1 == "{" or op1 == "}":
            return False

        if (op1 == "+" or op1 == "-") and (op2 == "*" or op2 == "/"):
            return False

        return True

    def infix_to_postfix(self, tokens):
        stack = []

        for cur_token in tokens:
            token = str(cur_token)

            if "+-*/".find(token) > -1:
                while (len(stack) > 0 and self.has_precedence(stack[-1], token)):
                    self.postfix.push(stack.pop())
                stack.append(token)
                continue

            if token == "(" or token == "{":
                stack.append(token)
                continue

            if token == ")":
                while len(stack) > 0 and stack[-1] != '(':
                    self.postfix.push(stack.pop())
                if len(stack) == 0:
                    
                    break
                stack.pop()
                continue

            if token == "}":
                while len(stack) > 0 and stack[-1] != '{':
                    self.postfix.push(stack.pop())
                if len(stack) == 0:
                
                    break
                stack.pop()
                continue

            self.postfix.push(cur_token)
        while len(stack) > 0:
            self.postfix.push(stack.pop())

    def evaluate_expression(self, input_expression):
        tokens = self.tokenize(input_expression)
        if not self.check_brackets(input_expression):
            self.history.append((str(input_expression),'Error'))
            return 'Error'
        else:
            result = self.evaluate_list_tokens(tokens)
            if str(result) == "Error":
                return "Error"
            self.history.append((str(input_expression), float(result)))
            return result

    def get_number(self, input_string, cur_pos):
        num_chars = 0
        token = ""
        for i in range(cur_pos, len(input_string)):
            if input_string[i] < "0" or input_string[i] > "9":
                break
            token += input_string[i]
            num_chars += 1
        return num_chars, token

    def tokenize(self, input_expression):
        tokens = []

        len_infix = len(input_expression)
        cur_pos = 0
        while cur_pos < len_infix:
            token = ""

            if input_expression[cur_pos] == " " or input_expression[cur_pos] == "\t":
                cur_pos += 1
                continue

            i, token = self.get_number(input_expression, cur_pos)
            if i > 0:
                tokens.append(int(token))
                cur_pos += i
                continue

            if self.operators.find(input_expression[cur_pos]) > -1:
                tokens.append(input_expression[cur_pos])
                cur_pos += 1
                continue

            break

        return tokens

    def check_brackets(self, list_tokens):
        brackets = []
        in_round_brackets = 0
        for t in list_tokens:
            st = str(t)

            if st == "(":
                brackets.append(st)
                in_round_brackets += 1

            elif st == "{":
                if in_round_brackets > 0:
                    return False
                brackets.append(st)

            elif st == ")":                
                if len(brackets) == 0 or brackets[-1] != "(":
                    return False
                brackets.pop()
                in_round_brackets -= 1

            elif st == "}":                
                if len(brackets) == 0 or brackets[-1] != "{":
                    return False
                brackets.pop()

        if len(brackets) != 0:
            return False
        
        return True


    def evaluate_list_tokens(self, list_tokens):
        self.infix_to_postfix(list_tokens)
        op = []
        for i in range(len(self.postfix)):
            cur = self.postfix.get(i)

            if str(cur) == "+":
                if len(op) < 2:
                    return "Error"
                op2 = op.pop()
                op1 = op.pop()
                op.append(op1 + op2)

            elif str(cur) == "-":
                if len(op) < 2:
                    return "Error"
                op2 = op.pop()
                op1 = op.pop()
                op.append(op1 - op2)

            elif str(cur) == "*":
                if len(op) < 2:
                    return "Error"
                op2 = op.pop()
                op1 = op.pop()
                op.append(op1 * op2)

            elif str(cur) == "/":
                if len(op) < 2:
                    return "Error"
                op2 = op.pop()
                op1 = op.pop()
                op.append(op1 / op2)
            
            else:
                op.append(cur)

        return op[0]
    def get_history(self):
        self.history.reverse()
        return self.history

