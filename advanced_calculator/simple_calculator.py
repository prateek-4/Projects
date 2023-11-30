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
        self.history.reverse()
        return self.history
